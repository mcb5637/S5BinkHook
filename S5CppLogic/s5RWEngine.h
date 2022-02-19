#pragma once
#include "s5data.h"

namespace BBRw {
	class IEngine {
	public:
		virtual ~IEngine() = default;
	};

	class CEngine : public IEngine {
	public:
		static inline constexpr int vtp = 0x76B534;
	};
}
