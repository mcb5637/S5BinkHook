#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_entitytype.h"
#include "s5_effecttype.h"

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
	public:
		virtual void __stdcall LoadData(const char* filepath) = 0;

		BB::CIDManagerEx* EffectTypeManager;
		BB::CIDManagerEx* WeatherEffectTextureManager;
		EGL::CGLEEffectsProps EffectsLogicProps;
		EGL::EffectsDisplayProps EffectsDisplayProps;
		EGL::EffectsProps EffectsProps;

		static inline constexpr int vtp = 0x7888D4;

		void LoadEffectTypeFromExtraFile(int id);
		// remember to also free in idmanager, free last id first
		void FreeEffectType(int id);
		void ReloadAllEffectTypes();
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
		EGL::CGLEEntitiesProps CGLEEntitiesProps; // 16 (Logic from Entities.xml ? )
		// Entities.xml Display ?

		static inline constexpr int vtp = 0x7887B4;

		void LoadEntityTypeByID(int id);
		// remember to also free in idmanager, free last id first
		void PopEntityType(int id);
		// remember to also free in idmanager, free last id first
		void FreeEntityType(int id);
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

