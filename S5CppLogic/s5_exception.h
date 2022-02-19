#pragma once
#include "s5_forwardDecls.h"

namespace BB {
	class CException {
	public:
		virtual ~CException() = default;
		virtual bool __stdcall CopyMessage(char* buffer, size_t buffLen) = 0;

		static inline constexpr int vtp = 0x761AA8;
	};

	class CInvalidIDException : public CException {
	public:
		static inline constexpr int vtp = 0x77F8A4;
	};
}
