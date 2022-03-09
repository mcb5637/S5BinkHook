#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

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
