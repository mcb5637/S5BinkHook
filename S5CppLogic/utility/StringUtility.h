#pragma once
#include <string>
#include <string_view>

namespace CppLogic::StringUtil {
	std::string ANSIToUTF8(std::string_view data);
}
