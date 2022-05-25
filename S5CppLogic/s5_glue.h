#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_entitytype.h"

namespace GGlue {
	class IAnimsPropsMgr {
		// no vtable
	};
	class CGlueAnimsPropsMgr : public IAnimsPropsMgr {
	private:
		virtual void freesomething(void*) = 0;
		// load anims?
		// return p1+4
	public:
		static inline constexpr int vtp = 0x788D8C;
	};

	class IEffectsPropsMgr : public ECore::ICheckData {
		// no vtable
	};
	class CEffectsPropsMgr : public IEffectsPropsMgr {
		virtual void uk1() = 0;
		virtual void uk2() = 0;
		virtual void uk3() = 0;
		virtual void uk4() = 0;
		virtual void uk5() = 0;
		virtual void __stdcall LoadData(const char* filename) = 0; // not completely sure
	public:
		static inline constexpr int vtp = 0x7888D4;
	};

	class IEntitiesPropsMgr : public ECore::ICheckData {
		// no vtable
	};
	class CEntitiesPropsMgr : public IEntitiesPropsMgr {
	public:
		virtual void __stdcall LoadNameLists() = 0; // loads EntityTypeManager, TaskListManager, AnimSetManager, RaceManager from list xml (not the actual objects)
		virtual void __stdcall LoadEntitytypes() = 0;

		BB::CIDManagerEx* AnimCategoryManager;
		BB::CIDManagerEx* AnimSetManager;
		BB::CIDManagerEx* EntityCategoryManager;
		BB::CIDManagerEx* EntityTypeManager;
		BB::CIDManagerEx* AnimManager; // 5
		BB::CIDManagerEx* EffectTypeManager;
	private:
		BB::CIDManagerEx* unknown1;
	public:
		BB::CIDManagerEx* RaceManager; // unused?
		BB::CIDManagerEx* TaskListManager;
		BB::CIDManagerEx* UpgradeCategoryManager; // 10
		BB::CIDManagerEx* BlessCategoryManager;
		BB::CIDManagerEx* BuildingBannerStatusManager;
	private:
		BB::CIDManagerEx* unknown2;
		BB::CIDManagerEx* unknown3;
		void* CIDGroupsEx; // BB::CIDGroupsEx buildingbanner stuff
	public:
		EGL::CGLEEntitiesProps CGLEEntitiesProps;

		static inline constexpr int vtp = 0x7887B4;
	};


	class CGluePropsMgr {
	public:
		virtual ~CGluePropsMgr() = default;

		CGlueAnimsPropsMgr* AnimPropsManager;
		CEffectsPropsMgr* EffectPropsManager;
		CEntitiesPropsMgr* EntitiesPropsManager;


		static inline constexpr int vtp = 0x788720;
	};
}

