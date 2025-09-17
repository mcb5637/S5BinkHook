#pragma once
#include <string>

namespace CppLogic::WinAPI {
	class FileDialog {
	public:
		bool Open = true;
		const char* Filter = "All Files (*.*)\0*.*\0";
		const char* Title = nullptr;
		std::string SelectedPath;

		bool Show();
	};

	template<class C, class ... A>
	class OnScopeExit {
		C Call;
		std::tuple<A...> Args;

	public:
		explicit OnScopeExit(C c, A ... args) : Call(c), Args(args...) {
		}

		OnScopeExit(const OnScopeExit&) = delete;
		OnScopeExit(OnScopeExit&&) = delete;
		OnScopeExit& operator=(const OnScopeExit&) = delete;
		OnScopeExit& operator=(OnScopeExit&&) = delete;

		~OnScopeExit() {
			std::apply(Call, Args);
		}
	};
}
