#pragma once
#include <type_traits>

template<class T>
requires std::is_enum_v<T>
class enum_is_flags : public std::false_type {};

template<class T>
requires std::is_enum_v<T> && enum_is_flags<T>::value
constexpr T& operator&=(T& a, T b) {
	using u = std::underlying_type<T>::type;
	a = static_cast<T>(static_cast<u>(a) & static_cast<u>(b));
	return a;
}

template<class T>
requires std::is_enum_v<T> && enum_is_flags<T>::value
constexpr T& operator|=(T& a, T b) {
	using u = std::underlying_type<T>::type;
	a = static_cast<T>(static_cast<u>(a) | static_cast<u>(b));
	return a;
}

template<class T>
requires std::is_enum_v<T> && enum_is_flags<T>::value
constexpr T& operator^=(T& a, T b) {
	using u = std::underlying_type<T>::type;
	a = static_cast<T>(static_cast<u>(a) ^ static_cast<u>(b));
	return a;
}

template<class T>
requires std::is_enum_v<T> && enum_is_flags<T>::value
constexpr T operator~(T a) {
	using u = std::underlying_type<T>::type;
	return static_cast<T>(~static_cast<u>(a));
}

template<class T>
requires std::is_enum_v<T> && enum_is_flags<T>::value
constexpr T operator&(T a, T b) {
	a &= b;
	return a;
}

template<class T>
requires std::is_enum_v<T> && enum_is_flags<T>::value
constexpr T operator|(T a, T b) {
	a |= b;
	return a;
}

template<class T>
requires std::is_enum_v<T> && enum_is_flags<T>::value
constexpr T operator^(T a, T b) {
	a ^= b;
	return a;
}
