#pragma once
#include <shok/s5_forwardDecls.h>

namespace BB {
	class CException {
	public:
		virtual ~CException() = default;
		virtual bool __stdcall CopyMessage(char* buffer, size_t buffLen) const = 0;

		static inline constexpr int vtp = 0x761AA8;
	};

	class CInvalidIDException : public CException {
	public:
		static inline constexpr int vtp = 0x77F8A4;
	};

	class CFileException : public CException {
	public:
		PADDINGI(2);
		int Reason;
		char* File;

		static inline constexpr int vtp = 0x77F46C;
	};
}

// standard libary used by shok (no idea why this is not std::exception)
class exception {
public:
	virtual ~exception() = default;
	virtual const char* what() const = 0;

	static inline constexpr int vtp = 0x788EF8;
};
