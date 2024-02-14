#pragma once
#include <utility>
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_idmanager.h"
#include "luaext.h"

#include <magic_enum.hpp>

namespace CppLogic::MagicEnum {
	template<size_t N>
	struct StringHolder {
		std::array<char, N + 1> Data;

		consteval StringHolder(magic_enum::detail::str_view v) {
			if (v.size_ != N)
				throw std::invalid_argument{ "size missmatch" };
			std::copy_n(v.str_, N, Data.data());
			Data[N] = 0;
		}
		constexpr operator std::string_view() const {
			return { Data.data(), N };
		}
	};
	template<StringHolder D>
	struct Holder {
		static constexpr auto value = D;
	};

	template<class En, En... Sparse>
	requires std::is_enum_v<En>
	class EnumIdManager {
		template<En E>
		static consteval auto ConstructSingle() {
			constexpr auto d = magic_enum::detail::n<E>();
			return StringHolder<d.size_>{d};
		}
		template<En E>
		static consteval std::pair<En, std::string_view> ConstructPair() {
			using h = Holder<ConstructSingle<E>()>;
			return std::pair(E, static_cast<std::string_view>(h::value));
		}
		static consteval auto Build() {
			return std::array{ ConstructPair<Sparse>()... };
		}

		static constexpr const auto entries = magic_enum::enum_entries<En>();
		static constexpr const auto sparse_entries = Build();
	public:
		// does not add
		En GetIdByName(const char* name) const {
			std::string_view n{ name };
			for (const auto& [e, en] : sparse_entries) {
				if (en == n)
					return e;
			}
			auto v = magic_enum::enum_cast<En>(n);
			if (!v.has_value())
				return En::Invalid;
			return *v;
		}
		const char* GetNameByID(En id) const {
			for (const auto& [e, en] : sparse_entries) {
				if (id == e)
					return en.data();
			}
			return magic_enum::enum_name(id).data();
		}

		void PushToState(lua::State L) const {
			L.NewTable();
			for (const auto& [id, name] : entries) {
				L.Push(name);
				L.Push(static_cast<int>(id));
				L.SetTableRaw(-3);
			}
			for (const auto& [id, name] : sparse_entries) {
				L.Push(name);
				L.Push(static_cast<int>(id));
				L.SetTableRaw(-3);
			}
		}
	};
}
