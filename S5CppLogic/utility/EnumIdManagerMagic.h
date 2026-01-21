#pragma once
#include <luaext.h>
#include <utility/ConstexprString.h>

#include <magic_enum/magic_enum.hpp>

namespace CppLogic::MagicEnum {
	template<ConstexprString D>
	struct Holder {
		static constexpr auto value = D;
	};
	template<class En>
	concept EnumHasInvalid = requires {
		{En::Invalid};
	};

	template<class En, En... Sparse>
	requires std::is_enum_v<En>
	class EnumIdManager {
		template<En E>
		static consteval auto ConstructSingle() {
			constexpr auto d = magic_enum::detail::n<E>();
			return ConstexprString<d.size_+1>{std::string_view(d.str_, d.size_)};
		}
		template<En E>
		static consteval std::pair<En, std::string_view> ConstructPair() {
			using h = Holder<ConstructSingle<E>()>;
			return std::pair(E, static_cast<std::string_view>(h::value));
		}
		static consteval auto Build() {
			if constexpr (sizeof...(Sparse) > 0)
				return std::array{ ConstructPair<Sparse>()... };
			else
				return 1; // placeholder so sparse_entries has some valid type, should never be accessed
		}

		static constexpr const auto entries = magic_enum::enum_entries<En>();
		static constexpr const auto sparse_entries = Build();
	public:
		// does not add
		En GetIdByName(const char* name) const {
			std::string_view n{ name };
			if constexpr (sizeof...(Sparse) > 0) {
				for (const auto& [e, en] : sparse_entries) {
					if (en == n)
						return e;
				}
			}
			auto v = magic_enum::enum_cast<En>(n);
			if (!v.has_value()) {
				if constexpr (EnumHasInvalid<En>) {
					return En::Invalid;
				}
				else {
					throw std::invalid_argument{ std::format("invalid id for type {}: {}", typename_details::type_name<En>(), n) };
				}
			}
			return *v;
		}
		const char* GetNameByID(En id) const {
			if constexpr (sizeof...(Sparse) > 0) {
				for (const auto& [e, en] : sparse_entries) {
					if (id == e)
						return en.data();
				}
			}
			return magic_enum::enum_name(id).data();
		}

		void PushToState(luaext::State L) const {
			L.NewTable();
			for (const auto& [id, name] : entries) {
				L.Push(name);
				L.Push(static_cast<int>(id));
				L.SetTableRaw(-3);
			}
			if constexpr (sizeof...(Sparse) > 0) {
				for (const auto& [id, name] : sparse_entries) {
					L.Push(name);
					L.Push(static_cast<int>(id));
					L.SetTableRaw(-3);
				}
			}
		}
	};
}
