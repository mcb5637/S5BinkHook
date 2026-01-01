#include <stdexcept>

#include "StringUtility.h"
#include <Win.h>

std::wstring To16(std::string_view data) {
	size_t len = MultiByteToWideChar(CP_ACP, 0, data.data(), static_cast<int>(data.size()), nullptr, 0);
	std::wstring r{};
	r.resize(len);
	if (!MultiByteToWideChar(CP_ACP, 0, data.data(), static_cast<int>(data.size()), r.data(), static_cast<int>(r.size())))
		throw std::invalid_argument("data");
	return r;
}
std::string To8(std::wstring_view data) {
	size_t len = WideCharToMultiByte(CP_UTF8, 0, data.data(), static_cast<int>(data.size()), nullptr, 0, nullptr, nullptr);
	std::string r{};
	r.resize(len);
	if (!WideCharToMultiByte(CP_UTF8, 0, data.data(), static_cast<int>(data.size()), r.data(), static_cast<int>(r.size()), nullptr, nullptr))
		throw std::invalid_argument("data");
	return r;
}

std::string CppLogic::StringUtil::ANSIToUTF8(std::string_view data)
{
	if (data.empty())
		return "";

	return To8(To16(data));
}
