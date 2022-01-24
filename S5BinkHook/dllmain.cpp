// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "binkw_orig.h"
#pragma pack(1)

FARPROC p[84] = { 0 };

double __declspec(dllimport) __stdcall GetCppLogicVersion();

extern "C" BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	/*static HINSTANCE hL;
	static HINSTANCE payload;*/
	if (reason == DLL_PROCESS_ATTACH)
	{
		/*hL = LoadLibrary(_T("binkw32_orig.dll"));
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
		p[83] = GetProcAddress(hL, "_YUV_blit_YV12@56");*/
		//payload = LoadLibrary(_T("S5CppLogic.dll"));
		GetCppLogicVersion();
	}
	if (reason == DLL_PROCESS_DETACH) {
		/*if (hL)
			FreeLibrary(hL);*/
		/*if (payload)
			FreeLibrary(payload);*/
	}
	return true;
}

extern "C" int _stdcall Proxy_BinkBufferSetDirectDraw(int a, int b)
{
	return _BinkBufferSetDirectDraw(a, b);
}

extern "C" int _stdcall Proxy_BinkIsSoftwareCursor(int a, int b)
{
	return _BinkIsSoftwareCursor(a, b);
}

extern "C" int _stdcall Proxy_BinkCheckCursor(int hWnd, int a2, int a3, int a4, int a5)
{
	return _BinkCheckCursor(hWnd, a2, a3, a4, a5);
}

extern "C" int _stdcall Proxy_BinkRestoreCursor(int a)
{
	return _BinkRestoreCursor(a);
}

extern "C" int _stdcall Proxy_BinkBufferSetResolution(int a, int b, int c)
{
	return _BinkBufferSetResolution(a, b, c);
}

extern "C" void _stdcall Proxy_BinkBufferCheckWinPos(int* a, int* b, int* c)
{
	return _BinkBufferCheckWinPos(a, b, c);
}

extern "C" int _stdcall Proxy_BinkBufferSetOffset(int a, int b, int c)
{
	return _BinkBufferSetOffset(a, b, c);
}

extern "C" int _stdcall Proxy_BinkBufferOpen(int a, int b, int c, int d)
{
	return _BinkBufferOpen(a, b, c, d);
}

extern "C" void _stdcall Proxy_BinkBufferClose(int* a)
{
	return _BinkBufferClose(a);
}

extern "C" int _stdcall Proxy_BinkBufferLock(int a)
{
	return _BinkBufferLock(a);
}

extern "C" int _stdcall Proxy_BinkBufferUnlock(int* a)
{
	return _BinkBufferUnlock(a);
}

extern "C" int _stdcall Proxy_BinkBufferBlit(int a, int b, int c)
{
	return _BinkBufferBlit(a, b, c);
}

extern "C" int _stdcall Proxy_BinkBufferSetScale(int* a, int b, int c)
{
	return _BinkBufferSetScale(a, b, c);
}

extern "C" int _stdcall Proxy_BinkBufferSetHWND(int a, int b)
{
	return _BinkBufferSetHWND(a, b);
}

extern "C" auto _stdcall Proxy_BinkBufferGetDescription(int a)
{
	return _BinkBufferGetDescription(a);
}

extern "C" auto _stdcall Proxy_BinkBufferGetError()
{
	return _BinkBufferGetError();
}

extern "C" int _stdcall Proxy_BinkBufferClear(int* a, int b)
{
	return _BinkBufferClear(a, b);
}

extern "C" int _stdcall Proxy_BinkDDSurfaceType(int* a)
{
	return _BinkDDSurfaceType(a);
}

extern "C" int _stdcall Proxy_BinkDX8SurfaceType(int* a)
{
	return _BinkDX8SurfaceType(a);
}

extern "C" int _stdcall Proxy_BinkDX9SurfaceType(int* a)
{
	return _BinkDX9SurfaceType(a);
}

extern "C" auto _stdcall Proxy_BinkSetError(char* a)
{
	return _BinkSetError(a);
}

extern "C" auto _stdcall Proxy_BinkGetError()
{
	return _BinkGetError();
}

extern "C" int _stdcall Proxy_BinkSetSoundSystem(int a, int b)
{
	return _BinkSetSoundSystem(a, b);
}

extern "C" int _stdcall Proxy_BinkSetFrameRate(int a, int b)
{
	return _BinkSetFrameRate(a, b);
}

extern "C" int _stdcall Proxy_BinkSetIOSize(int a)
{
	return _BinkSetIOSize(a);
}

extern "C" int _stdcall Proxy_BinkSetIO(int a)
{
	return _BinkSetIO(a);
}

extern "C" int _stdcall Proxy_BinkSetSimulate(int a)
{
	return _BinkSetSimulate(a);
}

extern "C" int _stdcall Proxy_BinkSetSoundTrack(unsigned int a, int b)
{
	return _BinkSetSoundTrack(a, b);
}

extern "C" int* _stdcall Proxy_BinkOpen(int a, int b)
{
	return _BinkOpen(a, b);
}

extern "C" auto _stdcall Proxy_BinkCopyToBuffer(int* a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	return _BinkCopyToBuffer(a1, a2, a3, a4, a5, a6, a7);
}

extern "C" auto _stdcall Proxy_BinkCopyToBufferRect(unsigned int* a1, int a2, int a3, int a4, int a5, int a6, unsigned int a7, unsigned int a8, unsigned int a9, unsigned int a10, int a11)
{
	return _BinkCopyToBufferRect(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
}

extern "C" int _stdcall Proxy_BinkDoFrame(int a)
{
	return _BinkDoFrame(a);
}

extern "C" void _stdcall Proxy_BinkNextFrame(int a)
{
	return _BinkNextFrame(a);
}

extern "C" int _stdcall Proxy_BinkGetKeyFrame(int a, int b, int c)
{
	return _BinkGetKeyFrame(a, b, c);
}

extern "C" int _stdcall Proxy_BinkGoto(int a, int b, int c)
{
	return _BinkGoto(a, b, c);
}

extern "C" void _stdcall Proxy_BinkClose(int a)
{
	return _BinkClose(a);
}

extern "C" int _stdcall Proxy_BinkWait(int a)
{
	return _BinkWait(a);
}

extern "C" int _stdcall Proxy_BinkPause(int* a, int b)
{
	return _BinkPause(a, b);
}

extern "C" void _stdcall Proxy_BinkGetSummary(int a, int b)
{
	return _BinkGetSummary(a, b);
}

extern "C" int _stdcall Proxy_BinkGetRealtime(int a, int b, int c)
{
	return _BinkGetRealtime(a, b, c);
}

extern "C" int _stdcall Proxy_BinkGetRects(int a, int b)
{
	return _BinkGetRects(a, b);
}

extern "C" int _stdcall Proxy_BinkService(int a)
{
	return _BinkService(a);
}

extern "C" void _stdcall Proxy_BinkSetVolume(int a, int b, int c)
{
	return _BinkSetVolume(a, b, c);
}

extern "C" void _stdcall Proxy_BinkSetMixBins(int a, int b, int c, int d)
{
	return _BinkSetMixBins(a, b, c, d);
}

extern "C" void _stdcall Proxy_BinkSetMixBinVolumes(int a, int b, int c, int d, int e)
{
	return _BinkSetMixBinVolumes(a, b, c, d, e);
}

extern "C" void _stdcall Proxy_BinkSetPan(int a, int b, int c)
{
	return _BinkSetPan(a, b, c);
}

extern "C" void* _stdcall Proxy_BinkLogoAddress()
{
	return _BinkLogoAddress();
}

extern "C" int _stdcall Proxy_BinkGetTrackType(int a, int b)
{
	return _BinkGetTrackType(a, b);
}

extern "C" int _stdcall Proxy_BinkGetTrackMaxSize(int a, int b)
{
	return _BinkGetTrackMaxSize(a, b);
}

extern "C" int _stdcall Proxy_BinkGetTrackID(int a, int b)
{
	return _BinkGetTrackID(a, b);
}

extern "C" int _stdcall Proxy_BinkOpenTrack(int a, int b)
{
	return _BinkOpenTrack(a, b);
}

extern "C" void _stdcall Proxy_BinkCloseTrack(int a)
{
	return _BinkCloseTrack(a);
}

extern "C" int _stdcall Proxy_BinkGetTrackData(int a, int b)
{
	return _BinkGetTrackData(a, b);
}

extern "C" int _stdcall Proxy_BinkSetVideoOnOff(int a, int b)
{
	return _BinkSetVideoOnOff(a, b);
}

extern "C" int _stdcall Proxy_BinkSetSoundOnOff(int a, int b)
{
	return _BinkSetSoundOnOff(a, b);
}

extern "C" int _stdcall Proxy_BinkSetMemory(int a, int b)
{
	return _BinkSetMemory(a, b);
}

extern "C" int _stdcall Proxy_BinkOpenWaveOut(int a)
{
	return _BinkOpenWaveOut(a);
}

extern "C" int _stdcall Proxy_BinkOpenDirectSound(int a)
{
	return _BinkOpenDirectSound(a);
}

extern "C" int _stdcall Proxy_BinkOpenMiles(int a)
{
	return _BinkOpenMiles(a);
}

extern "C" int _stdcall Proxy_RADTimerRead()
{
	return _RADTimerRead();
}

extern "C" int _stdcall Proxy_YUV_init(int a)
{
	return _YUV_init(a);
}

extern "C" int _stdcall Proxy_YUV_blit_16bpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_16bpp(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_16bpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_16bpp_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_16a4bpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_16a4bpp(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_16a4bpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_16a4bpp_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_16a1bpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_16a1bpp(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_16a1bpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_16a1bpp_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_32bpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_32bpp(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_32bpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_32bpp_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_32abpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_32abpp(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_32abpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_32abpp_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_YUY2(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_YUY2(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_YUY2_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_YUY2_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_32rbpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_32rbpp(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_32rbpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_32rbpp_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_32rabpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_32rabpp(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_32rabpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_32rabpp_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_24bpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_24bpp(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_24bpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_24bpp_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_24rbpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_24rbpp(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_24rbpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_24rbpp_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_UYVY(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
	return _YUV_blit_UYVY(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

extern "C" int _stdcall Proxy_YUV_blit_UYVY_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
{
	return _YUV_blit_UYVY_mask(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

extern "C" int _stdcall Proxy_YUV_blit_YV12(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14)
{
	return _YUV_blit_YV12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
}


