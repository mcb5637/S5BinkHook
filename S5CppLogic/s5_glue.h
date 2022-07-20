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
	//constexpr int i = offsetof(CEffectsPropsMgr, EffectsDisplayProps) / 4;

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

	class IGoodsPropsMgr {

	};
	class CGoodsPropsMgr : public IGoodsPropsMgr { // size 16, vtable 7

	};

	class IDamageClassPropsMgr {

	};
	class CDamageClassesPropsMgr : public IDamageClassPropsMgr { // size 11, vtable 6
	public:
		static inline constexpr int vtp = 0x788A54;
	};

	class IHeadsPropsMgr {

	};
	class CGlueHeadsPropsMgr : public IHeadsPropsMgr { // size 29, vtable 8

	};

	class ITerrainPropsMgr { // vtable 7
	public:
		virtual void __stdcall Destroy() = 0;

		static inline constexpr int vtp = 0x788AD8;
	};
	class CTerrainPropsMgr : public ITerrainPropsMgr { //size 20
	public:
		static inline constexpr int vtp = 0x788BB4;
	};

	struct WaterTypeData { // size 13
		struct LogicData {
			bool Freezes = true;
		} Logic;
		struct DisplayData {
			shok::Color Color; // i think this is B,G,R,A?, a always set to 0
			float TransparencyFactor = 0.01f;
			float TransparencyOffset = 0;
			float TransparencyMin = 0.25f;
			float TransparencyMax = 0.75f; // 4
			bool DrawShoreWave = false;
			bool Freezes = false;
			PADDING(2);
			int IceBaseTexture = 0;
			int IceDetailTexture = 0; // 7
			// after this, gets calculated by initialize
			float TransparencyFactorTimes255 = 0;
			float TransparencyOffsetTimes255 = 0;
			float TransparencyMinTimes255 = 0;
			float TransparencyMaxTimes255 = 0;

			void Initialize();
		} Display;

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA0DF38);
	};
	class WaterPropsLogic {
	public:
		BB::CIDManagerEx* WaterTypeManager;
		shok::Vector<WaterTypeData::LogicData> WaterLogic;
	};
	class WaterPropsDisplay {
	public:
		unsigned int ReflectionRasterSize;
		float WaterFlowSpeed;
		char* WaterBumpTexName;
		int WaterBumpTexNum;
		float WaterAnimDuration;
		char* SkyTexName;

		struct ShoreWaveKeyFrameData {
			float Pos;
			float Alpha;
			float Scale;
		};
		shok::Vector<ShoreWaveKeyFrameData> ShoreWaveKeyFrame; //CGlueWaterPropsMgr 13
		BB::CIDManagerEx* ShoreWaveTypeManager;
		BB::CIDManagerEx* WaterTypeManager; // 18
		shok::Vector<WaterTypeData::DisplayData> WaterDisplay; // 19
		struct ShoreWaveData {
			shok::String ShoreWaveTexName;
			float ShoreWaveAnimDuration;
			shok::Vector<ShoreWaveKeyFrameData> ShoreWaveKeyFrame;
		};
		shok::Vector<ShoreWaveData> ShoreWave; //CGlueWaterPropsMgr 23
	};
	class IGlueWaterPropsMgr { // vtable 6, +4 load
	public:
		static inline constexpr int vtp = 0x788BF0;

		virtual void __stdcall Destroy() = 0;
		virtual BB::CIDManagerEx* __stdcall GetWaterTypeManager() = 0;
		virtual WaterPropsLogic* __stdcall GetLogic() = 0;
		virtual WaterPropsDisplay* __stdcall GetDisplay() = 0;
		virtual void __stdcall Load(const char* file) = 0;
	protected:
		virtual ~IGlueWaterPropsMgr() = default;
	};
	class CGlueWaterPropsMgr : public IGlueWaterPropsMgr { // size 31
	public:
		BB::CIDManagerEx* WaterTypeManager; // 1
		WaterPropsLogic Logic;
		WaterPropsDisplay Display; // 7

		shok::Vector<WaterTypeData> WaterType; // 27 seems to get cleared after load

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA0DE88);
		static inline constexpr int vtp = 0x788C94;

		// clears idmanagers
		void ReloadWaterTypes();
		void LoadWaterTypeFromExtraFile(int id);
	};
	static_assert(offsetof(WaterPropsDisplay, ShoreWave) == 16 * 4);
	static_assert(offsetof(CGlueWaterPropsMgr, WaterType) == 27 * 4);
	static_assert(sizeof(WaterTypeData) == 13 * 4);
	static_assert(sizeof(CGlueWaterPropsMgr) == 31 * 4);

	class CGluePropsMgr {
	public:
		virtual ~CGluePropsMgr() = default;

		CGlueAnimsPropsMgr* AnimPropsManager;
		CEffectsPropsMgr* EffectPropsManager;
		CEntitiesPropsMgr* EntitiesPropsManager;
		CGoodsPropsMgr* GoodsPropsManager;
		CDamageClassesPropsMgr* DamageClassesPropsManager;
		CGlueHeadsPropsMgr* HeadsPropsManager;
		CTerrainPropsMgr* TerrainPropsManager;
		CGlueWaterPropsMgr* WaterPropsManager;

		static inline constexpr int vtp = 0x788720;
	};
}

