#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_RWEngine.h"

namespace RWE::P2D {
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
		void* LengthData; // 77
		void(__cdecl* RenderTxt)(Rt2dFont* f, const char* str, float height, RWE::RwV2d* anchor, Rt2dBrush* brush);

		float* GetLengthData();
		float GetHeight();
		float GetStringWidth(const char* s, float height);
		float GetStringWidth(const wchar_t* s, float height);
		void RenderText(const wchar_t* s, float height, RWE::RwV2d* anchor, Rt2dBrush* brush);
		void RenderText(const char* s, float height, RWE::RwV2d* anchor, Rt2dBrush* brush);
	};
}
