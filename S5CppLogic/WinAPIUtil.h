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
}
