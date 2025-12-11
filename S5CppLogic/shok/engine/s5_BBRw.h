#pragma once
#include <utility/enumflags.h>
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/engine/s5_RWEngine.h>
#include <shok/engine/directx.h>
#include <shok/globals/s5_idmanager.h>

namespace BBRw {
	class CEffect {
	public:
		virtual ~CEffect() = default;

		static inline constexpr int vtp = 0x76BBF4;
	};
}

namespace BB {
	template<class T>
	requires std::same_as<T, RWE::RwTexture*> || std::same_as<T, BBRw::CEffect*>
	class TResourceManager;

	template<class T>
	requires std::same_as<T, RWE::RwTexture*> || std::same_as<T, BBRw::CEffect*> // not sure if others exist
	class TResourceProxyBase {
	public:
		virtual ~TResourceProxyBase() = default;
		virtual T __stdcall Load() = 0;
		virtual void __stdcall Free(T d) = 0;
		virtual void __stdcall Override(T d) {
			FreeCache();
			Cache = d;
		};

		T Cache;
		PADDINGI(1); // refCounter?
		TResourceManager<T>* Manager; // 3
		int Id;

		T Get() { // 0x46CFAD
			if (!Cache) {
				Cache = Load();
			}
			return Cache;
		}
		void __stdcall FreeCache() { // 0x4728BE
			if (Cache) {
				Free(Cache);
				Cache = nullptr;
			}
		}

		// vtp for RWE::RwTexture* 0x769F9C
		// vtp for BBRw::CEffect* 0x76979C
	};
	template<class T>
	requires std::same_as<T, RWE::RwTexture*> || std::same_as<T, BBRw::CEffect*> // not sure if others exist
	class TResourceProxyResMgr : public TResourceProxyBase<T> {

		// vtp for RWE::RwTexture* 0x769FD0
		// vtp for BBRw::CEffect* 0x7697B0
	};
#ifdef __INTELLISENSE__
#pragma diag_suppress 2784
#endif
	static_assert(offsetof(TResourceProxyBase<RWE::RwTexture*>, Cache) == 4 * 1);
	static_assert(offsetof(TResourceProxyResMgr<RWE::RwTexture*>, Cache) == 4 * 1);
#ifdef __INTELLISENSE__
#pragma diag_default 2784
#endif

	template<class T>
	requires std::same_as<T, RWE::RwTexture*> || std::same_as<T, BBRw::CEffect*> // not sure if others exist
	class TResourceManager {
	public:
		virtual ~TResourceManager() = default;
		virtual T __stdcall Read(const char* name) = 0;
		virtual void __stdcall DestroyManaged(T) = 0;


		BB::CIDManagerEx* IdManager;
		shok::Vector<TResourceProxyResMgr<T>*> Data; // 2

		void PopId(int id) {
			if (id + 1 != static_cast<int>(Data.size()))
				throw std::out_of_range{ "invaild id" };
			auto v = Data.SaveVector();
			v.Vector[id]->FreeCache();
			delete v.Vector[id];
			v.Vector.pop_back();
			IdManager->RemoveID(id);
		}

		// vtp for RWE::RwTexture* 0x76BD90
		// vtp for BBRw::CEffect* 0x76B1D8
	};
}

namespace BBRw {
	class CRwTextures : public BB::TResourceManager<RWE::RwTexture*> {
	public:
		static inline constexpr int vtp = 0x76BDA0;

		BB::TResourceProxyResMgr<RWE::RwTexture*>* Get(const char* name);
		BB::TResourceProxyResMgr<RWE::RwTexture*>* Get(shok::SelectionTextureId id);
	};

	class IEffects : public BB::TResourceManager<BBRw::CEffect*> {
	public:
		virtual void Destroy() = 0;
		// 2 more vfuncs

		static inline constexpr int vtp = 0x76B1E8;

		BB::TResourceProxyResMgr<BBRw::CEffect*>* Get(const char* name);
	};

	class IEngine {
	public:
		virtual ~IEngine() = default;
		virtual void Destroy() = 0;
	private:
		virtual void emptyFunc1() = 0;
	public:
		virtual void GetWindowSize(int* width, int* height, int* bitDepth) = 0;
	};

	class IDynTexRef {
	public:
		virtual ~IDynTexRef() = default;
		virtual void Destroy() = 0;
		// not sure what type does
		virtual void CreateRaster(int x, int y, bool type = true) = 0;
		// destroys raster and clears all vars
		virtual void Reset() = 0;
		// locks with write & nofetch
		virtual byte* Lock() = 0;
		virtual void Unlock() = 0;

		RWE::RwRaster* Raster;
		RWE::RwTexture* Texture;
		int Stride;
		int Width, Height;
		bool Empty, Type;

		// ctor 49261E()
		// clear 4926B4()
	};
	class CDynTexRef : public IDynTexRef { // size 13
	public:

		PADDINGI(5); // 10 D3DFORMAT
		uint8_t* Data; // needs confirmation

		// ctor 49267A()
	};
	static_assert(sizeof(CDynTexRef) == 13 * 4);

	class IDynTexMgr {
	public:
		virtual ~IDynTexMgr() = default;
		virtual void Destroy() = 0;
		virtual CDynTexRef* Create() = 0;
	};
	class CDynTexMgr : public IDynTexMgr { // size 2
	public:
		PADDINGI(1); // int 0
		// new 492882
	};

	class CEngine : public IEngine {
	public:
		static inline constexpr int vtp = 0x76B534;
		HWND MainWindow;
		IEffects* Effects;
		CDynTexMgr* DynTexManager;
		CRwTextures* SelectionTextures; // also shorewave

		// init 0x48C77B

		static inline CEngine** const GlobalObj = reinterpret_cast<CEngine**>(0x8595E8); // technically IEngine, but CEngine is the only thing that ever ends up here
	};

	// size 4*4, id 0x44574D02, 48F466 attach
	struct UnnamedAtomicPlugin {
		PADDINGI(2);
		byte SrcBlend, DestBlend; // RwBlendFunction truncated to byte
		byte PlayerColor;
		byte Flags; // 0x8 has skin, 0x20 has terraindecal?
	};

	// size 7*4, id 0x44574D01, 473344 attach
	struct UnnamedClumpPlugin {
		PADDINGI(3);
		ED::ModelData* ModelData;
	};
}

template<>
inline auto CppLogic::GetIdManager<shok::SelectionTextureId>() {
	auto mng = (*BBRw::CEngine::GlobalObj)->SelectionTextures->IdManager;
	return CppLogic::EnumIdManager<shok::SelectionTextureId>{mng};
}
