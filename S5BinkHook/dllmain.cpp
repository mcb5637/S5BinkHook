// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#pragma pack(1)

FARPROC p[84] = { 0 };

extern "C" BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	static HINSTANCE hL;
	static HINSTANCE payload;
	if (reason == DLL_PROCESS_ATTACH)
	{
		hL = LoadLibrary(_T("binkw32_orig.dll"));
		if (!hL) {
			return false;
		}
		p[0] = GetProcAddress(hL, "_BinkBufferSetDirectDraw@8");
		p[1] = GetProcAddress(hL, "_BinkIsSoftwareCursor@8");
		p[2] = GetProcAddress(hL, "_BinkCheckCursor@20");
		p[3] = GetProcAddress(hL, "_BinkRestoreCursor@4");
		p[4] = GetProcAddress(hL, "_BinkBufferSetResolution@12");
		p[5] = GetProcAddress(hL, "_BinkBufferCheckWinPos@12");
		p[6] = GetProcAddress(hL, "_BinkBufferSetOffset@12");
		p[7] = GetProcAddress(hL, "_BinkBufferOpen@16");
		p[8] = GetProcAddress(hL, "_BinkBufferClose@4");
		p[9] = GetProcAddress(hL, "_BinkBufferLock@4");
		p[10] = GetProcAddress(hL, "_BinkBufferUnlock@4");
		p[11] = GetProcAddress(hL, "_BinkBufferBlit@12");
		p[12] = GetProcAddress(hL, "_BinkBufferSetScale@12");
		p[13] = GetProcAddress(hL, "_BinkBufferSetHWND@8");
		p[14] = GetProcAddress(hL, "_BinkBufferGetDescription@4");
		p[15] = GetProcAddress(hL, "_BinkBufferGetError@0");
		p[16] = GetProcAddress(hL, "_BinkBufferClear@8");
		p[17] = GetProcAddress(hL, "_BinkDDSurfaceType@4");
		p[18] = GetProcAddress(hL, "_BinkDX8SurfaceType@4");
		p[19] = GetProcAddress(hL, "_BinkDX9SurfaceType@4");
		p[20] = GetProcAddress(hL, "_BinkSetError@4");
		p[21] = GetProcAddress(hL, "_BinkGetError@0");
		p[22] = GetProcAddress(hL, "_BinkSetSoundSystem@8");
		p[23] = GetProcAddress(hL, "_BinkSetFrameRate@8");
		p[24] = GetProcAddress(hL, "_BinkSetIOSize@4");
		p[25] = GetProcAddress(hL, "_BinkSetIO@4");
		p[26] = GetProcAddress(hL, "_BinkSetSimulate@4");
		p[27] = GetProcAddress(hL, "_BinkSetSoundTrack@8");
		p[28] = GetProcAddress(hL, "_BinkOpen@8");
		p[29] = GetProcAddress(hL, "_BinkCopyToBuffer@28");
		p[30] = GetProcAddress(hL, "_BinkCopyToBufferRect@44");
		p[31] = GetProcAddress(hL, "_BinkDoFrame@4");
		p[32] = GetProcAddress(hL, "_BinkNextFrame@4");
		p[33] = GetProcAddress(hL, "_BinkGetKeyFrame@12");
		p[34] = GetProcAddress(hL, "_BinkGoto@12");
		p[35] = GetProcAddress(hL, "_BinkClose@4");
		p[36] = GetProcAddress(hL, "_BinkWait@4");
		p[37] = GetProcAddress(hL, "_BinkPause@8");
		p[38] = GetProcAddress(hL, "_BinkGetSummary@8");
		p[39] = GetProcAddress(hL, "_BinkGetRealtime@12");
		p[40] = GetProcAddress(hL, "_BinkGetRects@8");
		p[41] = GetProcAddress(hL, "_BinkService@4");
		p[42] = GetProcAddress(hL, "_BinkSetVolume@12");
		p[43] = GetProcAddress(hL, "_BinkSetMixBins@16");
		p[44] = GetProcAddress(hL, "_BinkSetMixBinVolumes@20");
		p[45] = GetProcAddress(hL, "_BinkSetPan@12");
		p[46] = GetProcAddress(hL, "_BinkLogoAddress@0");
		p[47] = GetProcAddress(hL, "_BinkGetTrackType@8");
		p[48] = GetProcAddress(hL, "_BinkGetTrackMaxSize@8");
		p[49] = GetProcAddress(hL, "_BinkGetTrackID@8");
		p[50] = GetProcAddress(hL, "_BinkOpenTrack@8");
		p[51] = GetProcAddress(hL, "_BinkCloseTrack@4");
		p[52] = GetProcAddress(hL, "_BinkGetTrackData@8");
		p[53] = GetProcAddress(hL, "_BinkSetVideoOnOff@8");
		p[54] = GetProcAddress(hL, "_BinkSetSoundOnOff@8");
		p[55] = GetProcAddress(hL, "_BinkSetMemory@8");
		p[56] = GetProcAddress(hL, "_BinkOpenWaveOut@4");
		p[57] = GetProcAddress(hL, "_BinkOpenDirectSound@4");
		p[58] = GetProcAddress(hL, "_BinkOpenMiles@4");
		p[59] = GetProcAddress(hL, "_RADTimerRead@0");
		p[60] = GetProcAddress(hL, "_YUV_init@4");
		p[61] = GetProcAddress(hL, "_YUV_blit_16bpp@52");
		p[62] = GetProcAddress(hL, "_YUV_blit_16bpp_mask@60");
		p[63] = GetProcAddress(hL, "_YUV_blit_16a4bpp@52");
		p[64] = GetProcAddress(hL, "_YUV_blit_16a4bpp_mask@60");
		p[65] = GetProcAddress(hL, "_YUV_blit_16a1bpp@52");
		p[66] = GetProcAddress(hL, "_YUV_blit_16a1bpp_mask@60");
		p[67] = GetProcAddress(hL, "_YUV_blit_32bpp@52");
		p[68] = GetProcAddress(hL, "_YUV_blit_32bpp_mask@60");
		p[69] = GetProcAddress(hL, "_YUV_blit_32abpp@52");
		p[70] = GetProcAddress(hL, "_YUV_blit_32abpp_mask@60");
		p[71] = GetProcAddress(hL, "_YUV_blit_YUY2@52");
		p[72] = GetProcAddress(hL, "_YUV_blit_YUY2_mask@60");
		p[73] = GetProcAddress(hL, "_YUV_blit_32rbpp@52");
		p[74] = GetProcAddress(hL, "_YUV_blit_32rbpp_mask@60");
		p[75] = GetProcAddress(hL, "_YUV_blit_32rabpp@52");
		p[76] = GetProcAddress(hL, "_YUV_blit_32rabpp_mask@60");
		p[77] = GetProcAddress(hL, "_YUV_blit_24bpp@52");
		p[78] = GetProcAddress(hL, "_YUV_blit_24bpp_mask@60");
		p[79] = GetProcAddress(hL, "_YUV_blit_24rbpp@52");
		p[80] = GetProcAddress(hL, "_YUV_blit_24rbpp_mask@60");
		p[81] = GetProcAddress(hL, "_YUV_blit_UYVY@52");
		p[82] = GetProcAddress(hL, "_YUV_blit_UYVY_mask@60");
		p[83] = GetProcAddress(hL, "_YUV_blit_YV12@56");
		payload = LoadLibrary(_T("S5CppLogic.dll"));
	}
	if (reason == DLL_PROCESS_DETACH) {
		FreeLibrary(hL);
		FreeLibrary(payload);
	}
	return TRUE;
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferSetDirectDraw()
{
	__asm
	{
		jmp p[0 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkIsSoftwareCursor()
{
	__asm
	{
		jmp p[1 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkCheckCursor()
{
	__asm
	{
		jmp p[2 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkRestoreCursor()
{
	__asm
	{
		jmp p[3 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferSetResolution()
{
	__asm
	{
		jmp p[4 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferCheckWinPos()
{
	__asm
	{
		jmp p[5 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferSetOffset()
{
	__asm
	{
		jmp p[6 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferOpen()
{
	__asm
	{
		jmp p[7 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferClose()
{
	__asm
	{
		jmp p[8 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferLock()
{
	__asm
	{
		jmp p[9 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferUnlock()
{
	__asm
	{
		jmp p[10 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferBlit()
{
	__asm
	{
		jmp p[11 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferSetScale()
{
	__asm
	{
		jmp p[12 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferSetHWND()
{
	__asm
	{
		jmp p[13 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferGetDescription()
{
	__asm
	{
		jmp p[14 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferGetError()
{
	__asm
	{
		jmp p[15 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkBufferClear()
{
	__asm
	{
		jmp p[16 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkDDSurfaceType()
{
	__asm
	{
		jmp p[17 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkDX8SurfaceType()
{
	__asm
	{
		jmp p[18 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkDX9SurfaceType()
{
	__asm
	{
		jmp p[19 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetError()
{
	__asm
	{
		jmp p[20 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkGetError()
{
	__asm
	{
		jmp p[21 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetSoundSystem()
{
	__asm
	{
		jmp p[22 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetFrameRate()
{
	__asm
	{
		jmp p[23 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetIOSize()
{
	__asm
	{
		jmp p[24 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetIO()
{
	__asm
	{
		jmp p[25 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetSimulate()
{
	__asm
	{
		jmp p[26 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetSoundTrack()
{
	__asm
	{
		jmp p[27 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkOpen()
{
	__asm
	{
		jmp p[28 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkCopyToBuffer()
{
	__asm
	{
		jmp p[29 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkCopyToBufferRect()
{
	__asm
	{
		jmp p[30 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkDoFrame()
{
	__asm
	{
		jmp p[31 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkNextFrame()
{
	__asm
	{
		jmp p[32 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkGetKeyFrame()
{
	__asm
	{
		jmp p[33 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkGoto()
{
	__asm
	{
		jmp p[34 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkClose()
{
	__asm
	{
		jmp p[35 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkWait()
{
	__asm
	{
		jmp p[36 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkPause()
{
	__asm
	{
		jmp p[37 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkGetSummary()
{
	__asm
	{
		jmp p[38 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkGetRealtime()
{
	__asm
	{
		jmp p[39 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkGetRects()
{
	__asm
	{
		jmp p[40 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkService()
{
	__asm
	{
		jmp p[41 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetVolume()
{
	__asm
	{
		jmp p[42 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetMixBins()
{
	__asm
	{
		jmp p[43 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetMixBinVolumes()
{
	__asm
	{
		jmp p[44 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetPan()
{
	__asm
	{
		jmp p[45 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkLogoAddress()
{
	__asm
	{
		jmp p[46 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkGetTrackType()
{
	__asm
	{
		jmp p[47 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkGetTrackMaxSize()
{
	__asm
	{
		jmp p[48 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkGetTrackID()
{
	__asm
	{
		jmp p[49 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkOpenTrack()
{
	__asm
	{
		jmp p[50 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkCloseTrack()
{
	__asm
	{
		jmp p[51 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkGetTrackData()
{
	__asm
	{
		jmp p[52 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetVideoOnOff()
{
	__asm
	{
		jmp p[53 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetSoundOnOff()
{
	__asm
	{
		jmp p[54 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkSetMemory()
{
	__asm
	{
		jmp p[55 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkOpenWaveOut()
{
	__asm
	{
		jmp p[56 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkOpenDirectSound()
{
	__asm
	{
		jmp p[57 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_BinkOpenMiles()
{
	__asm
	{
		jmp p[58 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_RADTimerRead()
{
	__asm
	{
		jmp p[59 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_init()
{
	__asm
	{
		jmp p[60 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_16bpp()
{
	__asm
	{
		jmp p[61 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_16bpp_mask()
{
	__asm
	{
		jmp p[62 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_16a4bpp()
{
	__asm
	{
		jmp p[63 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_16a4bpp_mask()
{
	__asm
	{
		jmp p[64 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_16a1bpp()
{
	__asm
	{
		jmp p[65 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_16a1bpp_mask()
{
	__asm
	{
		jmp p[66 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_32bpp()
{
	__asm
	{
		jmp p[67 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_32bpp_mask()
{
	__asm
	{
		jmp p[68 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_32abpp()
{
	__asm
	{
		jmp p[69 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_32abpp_mask()
{
	__asm
	{
		jmp p[70 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_YUY2()
{
	__asm
	{
		jmp p[71 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_YUY2_mask()
{
	__asm
	{
		jmp p[72 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_32rbpp()
{
	__asm
	{
		jmp p[73 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_32rbpp_mask()
{
	__asm
	{
		jmp p[74 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_32rabpp()
{
	__asm
	{
		jmp p[75 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_32rabpp_mask()
{
	__asm
	{
		jmp p[76 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_24bpp()
{
	__asm
	{
		jmp p[77 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_24bpp_mask()
{
	__asm
	{
		jmp p[78 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_24rbpp()
{
	__asm
	{
		jmp p[79 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_24rbpp_mask()
{
	__asm
	{
		jmp p[80 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_UYVY()
{
	__asm
	{
		jmp p[81 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_UYVY_mask()
{
	__asm
	{
		jmp p[82 * 4];
	}
}

extern "C" __declspec(naked) void _stdcall Proxy_YUV_blit_YV12()
{
	__asm
	{
		jmp p[83 * 4];
	}
}


