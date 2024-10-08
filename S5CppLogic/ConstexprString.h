#pragma once
#include <utility>

namespace CppLogic {
	template<size_t N>
	struct ConstexprString {
		constexpr ConstexprString() = default;

		std::array<char, N> Data;

		explicit constexpr ConstexprString(const char (&v)[N]) {
			std::copy_n(v, N - 1, Data.data());
			Data[N - 1] = 0;
		}
		explicit constexpr ConstexprString(std::string_view v) {
			if (v.size() + 1 != N)
				throw std::invalid_argument{ "size missmatch" };
			std::copy_n(v.data(), N - 1, Data.data());
			Data[N - 1] = 0;
		}
		constexpr std::string_view view() const noexcept {
			return { Data.data(), N - 1 };
		}
		constexpr operator std::string_view() const noexcept {
			return view();
		}
		constexpr const char* data() const noexcept {
			return Data.data();
		}
		constexpr bool empty() const noexcept {
			return view().empty();
		}

		template<size_t M>
		constexpr ConstexprString<N + M - 1> operator+(const ConstexprString<M>& o) const {
			ConstexprString<N + M - 1> r{};
			std::copy_n(this->Data.data(), N - 1, r.Data.data());
			std::copy_n(o.Data.data(), M - 1, r.Data.data() + (N-1));
			r.Data[N + M - 2] = '\0';
			return r;
		}
	};
}
