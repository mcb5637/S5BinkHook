#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>

namespace EVid {
	class CVideoTextureRW {
	public:
		virtual ~CVideoTextureRW() = default;

		static constexpr int vtp = 0x788650;

		void* RenderObject = nullptr;
		int SizeX = 0, SizeY = 0;
		PADDINGI(1);

	};
	static_assert(sizeof(CVideoTextureRW) == 5 * 4);

	class IVideoCollection {
	public:
		virtual void __stdcall Destroy() = 0;
		virtual int __stdcall OpenVideo(const char* path) = 0;
		virtual int __stdcall Start(const char* path, bool looped) = 0;
		virtual void __stdcall Stop() = 0;
		virtual void __thiscall UpdateFrame() = 0;
		virtual void __stdcall GetSize(int* x, int* y) = 0;
	private:
		virtual void getsomethngfromactivevid() = 0;
	public:
		virtual ~IVideoCollection() = default;


		static constexpr int vtp = 0x788598;

	};
	class IVideoCollectionRW : public IVideoCollection {

	};
	class CVideoCollectionRW : public IVideoCollectionRW {
	public:
		virtual void __stdcall Init(int sx, int sy) = 0;
		virtual void* __stdcall GetRender() = 0;

		static constexpr int vtp = 0x7885BC;


		BB::CIDManagerEx* IdManager = nullptr;
		shok::Vector<int> Buffer; // what exactly is in here?
		int CurrentPlaying = 0; // 6
		CVideoTextureRW* Texture = nullptr;
	};
	static_assert(offsetof(CVideoCollectionRW, CurrentPlaying) == 6 * 4);
	static_assert(sizeof(CVideoCollectionRW) == 8*4);

	class IVideo {
	public:
		virtual void __stdcall Destroy() = 0;
	private:
		virtual void uk() = 0;
	public:
		virtual CVideoCollectionRW* CreateVideoCollection() = 0;
		virtual ~IVideo() = default;

		static constexpr int vtp = 0x788560;
	};
	class CVideo : public IVideo {
	public:

		static constexpr int vtp = 0x788574;
	};
}
