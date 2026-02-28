#pragma once
#include <type_traits>

template<class T>
requires std::is_enum_v<T>
class enum_is_flags : public std::false_type {};

template<class T>
requires std::is_enum_v<T>
class enum_is_iter : public std::false_type {};

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

template<class T>
requires std::is_enum_v<T> && enum_is_iter<T>::value
constexpr T operator+(T a, T b) {
	using under = std::underlying_type_t<T>;
	return static_cast<T>(static_cast<under>(a) + static_cast<under>(b));
}
template<class T>
requires std::is_enum_v<T> && enum_is_iter<T>::value
constexpr T operator-(T a, T b) {
	using under = std::underlying_type_t<T>;
	return static_cast<T>(static_cast<under>(a) - static_cast<under>(b));
}
template<class T>
requires std::is_enum_v<T> && enum_is_iter<T>::value
constexpr T& operator++(T& a) {
	a = a + static_cast<T>(1);
	return a;
}
template<class T>
requires std::is_enum_v<T> && enum_is_iter<T>::value
constexpr T& operator--(T& a) {
	a = a - static_cast<T>(1);
	return a;
}
