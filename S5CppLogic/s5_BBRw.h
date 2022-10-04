#pragma once
#include "enumflags.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_RWEngine.h"
#include "directx.h"

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
		BB::TResourceProxyResMgr<RWE::RwTexture*>* Get(int id);
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
	};

	class CEngine : public IEngine {
	public:
		static inline constexpr int vtp = 0x76B534;
		PADDINGI(1);
		IEffects* Effects;
		PADDINGI(1);
		CRwTextures* SelectionTextures; // also shorewave
	};
}
