#include "pch.h"

#include "StringUtility.h"

std::wstring To16(std::string_view data) {
	size_t len = MultiByteToWideChar(CP_ACP, 0, data.data(), data.size(), nullptr, 0);
	std::wstring r{};
	r.resize(len);
	if (!MultiByteToWideChar(CP_ACP, 0, data.data(), data.size(), r.data(), r.size()))
		throw std::invalid_argument("data");
	return r;
}
std::string To8(std::wstring_view data) {
	size_t len = WideCharToMultiByte(CP_UTF8, 0, data.data(), data.size(), nullptr, 0, 0, nullptr);
	std::string r{};
	r.resize(len);
	if (!WideCharToMultiByte(CP_UTF8, 0, data.data(), data.size(), r.data(), r.size(), 0, nullptr))
		throw std::invalid_argument("data");
	return r;
}

std::string CppLogic::StringUtil::ANSIToUTF8(std::string_view data)
{
	if (data.empty())
		return "";

	return To8(To16(data));
}
