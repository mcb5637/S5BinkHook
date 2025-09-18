#pragma once
#include <tuple>
#include <optional>

namespace CppLogic {
	template<class C, class ... A>
	class OnScopeExit {
		struct DataT {
			C Call;
			std::tuple<A...> Args;

			explicit DataT(C c, A ... args) : Call(c), Args(args...) {}
		};
		std::optional<DataT> Data;

	public:
		explicit OnScopeExit(C c, A ... args) : Data(DataT{ c, args... }) {
		}

		OnScopeExit(const OnScopeExit&) = delete;
		OnScopeExit(OnScopeExit&&) = delete;
		OnScopeExit& operator=(const OnScopeExit&) = delete;
		OnScopeExit& operator=(OnScopeExit&&) = delete;

		void Defuse() {
			Data = std::nullopt;
		}

		~OnScopeExit() {
			if (Data.has_value())
				std::apply(Data->Call, Data->Args);
			Data = std::nullopt;
		}
	};
}
