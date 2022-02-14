#pragma once



extern "C" {
	__declspec(dllimport) int __stdcall _BinkBufferSetDirectDraw(int, int);
	__declspec(dllimport) int __stdcall _BinkIsSoftwareCursor(int a1, int hCursor);
	__declspec(dllimport) int __stdcall _BinkCheckCursor(int hWnd, int a2, int a3, int a4, int a5);
	__declspec(dllimport) int __stdcall _BinkRestoreCursor(int a1);
	__declspec(dllimport) int __stdcall _BinkBufferSetResolution(int a1, int a2, int a3);
	__declspec(dllimport) void __stdcall _BinkBufferCheckWinPos(int* a1, int* a2, int* a3);
	__declspec(dllimport) int __stdcall _BinkBufferSetOffset(int a1, int a2, int a3);
	__declspec(dllimport) int __stdcall _BinkBufferOpen(int hWnd, int a3, int a4, int a5);
	__declspec(dllimport) void __stdcall _BinkBufferClose(int* a1);
	__declspec(dllimport) int __stdcall _BinkBufferLock(int a1);
	__declspec(dllimport) int __stdcall _BinkBufferUnlock(int* a1);
	__declspec(dllimport) int __stdcall _BinkBufferBlit(int a, int b, int c);
	__declspec(dllimport) int __stdcall _BinkBufferSetScale(int* a1, int a2, int a3);
	__declspec(dllimport) int __stdcall _BinkBufferSetHWND(int a1, int a2);
	__declspec(dllimport) const char* __stdcall _BinkBufferGetDescription(int a1);
	__declspec(dllimport) const char* __stdcall _BinkBufferGetError();
	__declspec(dllimport) int __stdcall _BinkBufferClear(int* a1, int a2);
	__declspec(dllimport) int __stdcall _BinkDDSurfaceType(int* a1);
	__declspec(dllimport) int __stdcall _BinkDX8SurfaceType(int* a1);
	__declspec(dllimport) int __stdcall _BinkDX9SurfaceType(int* a1);
	__declspec(dllimport) char* __stdcall _BinkSetError(char* a1);
	__declspec(dllimport) char* __stdcall _BinkGetError();
	__declspec(dllimport) int __stdcall _BinkSetSoundSystem(int a1, int a2);
	__declspec(dllimport) int __stdcall _BinkSetFrameRate(int a1, int a2);
	__declspec(dllimport) int __stdcall _BinkSetIOSize(int a1);
	__declspec(dllimport) int __stdcall _BinkSetIO(int a1);
	__declspec(dllimport) int __stdcall _BinkSetSimulate(int a1);
	__declspec(dllimport) int __stdcall _BinkSetSoundTrack(unsigned int a1, int a2);
	__declspec(dllimport) int* __stdcall _BinkOpen(int hFile, int a2);
	__declspec(dllimport) int __stdcall _BinkCopyToBuffer(int* a1, int a2, int a3, int a4, int a5, int a6, int a7);
	__declspec(dllimport) signed int __stdcall _BinkCopyToBufferRect(unsigned int* a1, int a2, int a3, int a4, int a5, int a6, unsigned int a7, unsigned int a8, unsigned int a9, unsigned int a10, int a11);
	__declspec(dllimport) int __stdcall _BinkDoFrame(int a1);
	__declspec(dllimport) void __stdcall _BinkNextFrame(int a);
	__declspec(dllimport) int __stdcall _BinkGetKeyFrame(int a1, int a2, int a3);
	__declspec(dllimport) int __stdcall _BinkGoto(int a, int b, int c);
	__declspec(dllimport) void __stdcall _BinkClose(int a1);
	__declspec(dllimport) int __stdcall _BinkWait(int a);
	__declspec(dllimport) int __stdcall _BinkPause(int* a, int b);
	__declspec(dllimport) void __stdcall _BinkGetSummary(int a, int b);
	__declspec(dllimport) int __stdcall _BinkGetRealtime(int a, int b, int c);
	__declspec(dllimport) int __stdcall _BinkGetRects(int a, int b);
	__declspec(dllimport) int __stdcall _BinkService(int a);
	__declspec(dllimport) void __stdcall _BinkSetVolume(int a, int b, int c);
	__declspec(dllimport) void __stdcall _BinkSetMixBins(int a, int b, int c, int d);
	__declspec(dllimport) void __stdcall _BinkSetMixBinVolumes(int a, int b, int c, int d, int e);
	__declspec(dllimport) void __stdcall _BinkSetPan(int a, int b, int c);
	__declspec(dllimport) void* __stdcall _BinkLogoAddress();
	__declspec(dllimport) int __stdcall _BinkGetTrackType(int a, int b);
	__declspec(dllimport) int __stdcall _BinkGetTrackMaxSize(int a, int b);
	__declspec(dllimport) int __stdcall _BinkGetTrackID(int a, int b);
	__declspec(dllimport) int __stdcall _BinkOpenTrack(int a, int b);
	__declspec(dllimport) void __stdcall _BinkCloseTrack(int a);
	__declspec(dllimport) int __stdcall _BinkGetTrackData(int a, int b);
	__declspec(dllimport) int __stdcall _BinkSetVideoOnOff(int a, int b);
	__declspec(dllimport) int __stdcall _BinkSetSoundOnOff(int a, int b);
	__declspec(dllimport) int __stdcall _BinkSetMemory(int a, int b);
	__declspec(dllimport) int __stdcall _BinkOpenWaveOut(int a);
	__declspec(dllimport) int __stdcall _BinkOpenDirectSound(int a);
	__declspec(dllimport) int __stdcall _BinkOpenMiles(int a);
	__declspec(dllimport) int __stdcall _RADTimerRead();
	__declspec(dllimport) int __stdcall _YUV_init(int a);
	__declspec(dllimport) int __stdcall _YUV_blit_16bpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_16bpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_16a4bpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_16a4bpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_16a1bpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_16a1bpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_32bpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_32bpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_32abpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_32abpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_YUY2(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_YUY2_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_32rbpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_32rbpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_32rabpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_32rabpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_24bpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_24bpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_24rbpp(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_24rbpp_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_UYVY(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
	__declspec(dllimport) int __stdcall _YUV_blit_UYVY_mask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15);
	__declspec(dllimport) int __stdcall _YUV_blit_YV12(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);
}