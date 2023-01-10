#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_RWEngine.h"

namespace RWE::P2D {
	struct Device {

		static inline int(__cdecl* const GetStep)(RwV2d* xstep, RwV2d* ystep, RwV2d* origin) = reinterpret_cast<int(__cdecl*)(RwV2d*, RwV2d*, RwV2d*)>(0x707400);
	};

	struct CTM {

		static inline int(__cdecl* const Push)() = reinterpret_cast<int(__cdecl*)()>(0x707170);
		static inline int(__cdecl* const Translate)(float x, float y) = reinterpret_cast<int(__cdecl*)(float x, float y)>(0x707250);
		static inline int(__cdecl* const Scale)(float x, float y) = reinterpret_cast<int(__cdecl*)(float x, float y)>(0x707200);
		static inline RwMatrix* (__cdecl* const Get)() = reinterpret_cast<RwMatrix * (__cdecl*)()>(0x7072A0);
		static inline int (__cdecl* const Pop)() = reinterpret_cast<int(__cdecl*)()>(0x7071D0);
		static inline int(__cdecl* const SetIdentity)() = reinterpret_cast<int(__cdecl*)()>(0x706D60);

	};

	struct Rt2dBrush {

	};
	struct Rt2dFont {
	public:
		int IsOutline;
		float Size, Offset, Spacing;
		byte Flags; // 2 -> is wchar
		PADDING(3);
		PADDINGI(4);
		int16_t CharData[128]; // 9
		int ExtCharOffset; // 73
		unsigned int ExtCharLength;
		int16_t* ExtCharData;
		PADDINGI(1);
		void* LengthData; // 77 RwSList*
		void(__cdecl* RenderTxt)(Rt2dFont* f, const char* str, float height, RWE::RwV2d* anchor, Rt2dBrush* brush);

		float* GetLengthData();
		float GetHeight();
		float GetStringWidth(const char* s, float height);
		float GetStringWidth(const wchar_t* s, float height);
		void RenderText(const wchar_t* s, float height, RWE::RwV2d* anchor, Rt2dBrush* brush);
		void RenderText(const char* s, float height, RWE::RwV2d* anchor, Rt2dBrush* brush);
		bool IsWchar();

		void Destroy();

		/**
		 * \ingroup rt2ddatatypes
		 * \ref Rt2dFontCallBackRead
		 * Rt2dFontCallBackRead represents the function used by \ref Rt2dFontRead to read
		 * the specified font from a disk file. This function should return a
		 * pointer to the font to indicate success. The returned font is owned by
		 * the Rt2d internal font dictionary, and is destroyed on calling
		 * \ref Rt2dClose
		 *
		 * \param name is the name of the font to read
		 *
		 * \return return the font if successful, NULL otherwise
		 *
		 * \see Rt2dFontSetReadCallBack
		 * \see Rt2dFontGetReadCallBack
		 */
		typedef Rt2dFont* (*Rt2dFontCallBackRead)(const char* name);
		static inline Rt2dFontCallBackRead*const Rt2dFontReadCallBack = reinterpret_cast<Rt2dFontCallBackRead*>(0xA1C320);

		static inline Rt2dFont* (__cdecl* const Create)() = reinterpret_cast<Rt2dFont* (__cdecl*)()>(0x709A20);

	private:
		// defult render cb
		static inline void(__cdecl* const FontShowBitmap)(Rt2dFont* f, const char* str, float height, RWE::RwV2d* anchor, Rt2dBrush* brush) = reinterpret_cast<void(__cdecl*)(Rt2dFont*, const char*, float, RWE::RwV2d*, Rt2dBrush*)>(0x709140);
	};
}
