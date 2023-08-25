#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_entitytype.h"
#include "s5_effecttype.h"
#include "s5_config.h"

namespace GGlue {
	class IAnimsPropsMgr {
		// no vtable
	};
	class CGlueAnimsPropsMgr : public IAnimsPropsMgr {
	public:
		virtual void __stdcall Destroy() = 0;
		virtual void __stdcall LoadAnimData(void* unknown, GD::CDDisplay* display, BB::CIDManagerEx* animmanager) = 0;
		virtual shok::Vector<EGL::CGLEAnimProps*>* __stdcall GetData() = 0;

		static inline constexpr int vtp = 0x788D8C;

		shok::Vector<EGL::CGLEAnimProps*> Data;

		void CreateAnimProps(shok::AnimationId id);
		void PopAnimPops(shok::AnimationId id);
	};
	static_assert(sizeof(CGlueAnimsPropsMgr) == 5 * 4);

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

		void LoadEffectTypeFromExtraFile(shok::EffectTypeId id);
		// remember to also free in idmanager, free last id first
		void FreeEffectType(shok::EffectTypeId id);
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
		ED::EntityTypeDisplayProps DisplayProps;
		shok::Vector<GGlue::CGlueEntityProps> EntityTypes; // 15
		// Entities.xml Display ?

		static inline constexpr int vtp = 0x7887B4;

		void LoadEntityTypeByID(shok::EntityTypeId id);
		// remember to also free in idmanager, free last id first
		void PopEntityType(shok::EntityTypeId id);
		// remember to also free in idmanager, free last id first
		void FreeEntityType(shok::EntityTypeId id);
		void RefreshDisplayFlags();
	};

	class IGoodsPropsMgr {

	};
	class CGoodsPropsMgr : public IGoodsPropsMgr { // size 16, vtable 7

	};

	class IDamageClassPropsMgr {
	public:
		virtual void __stdcall Destroy() = 0;
		virtual BB::CIDManagerEx* __stdcall GetIdManager() = 0;
		virtual GGL::DamageClassesHolder __stdcall GetLogic() = 0;
		virtual void __stdcall Load(const char* file) = 0;
		virtual ~IDamageClassPropsMgr() = default;
		virtual void __stdcall Save(const char* file) = 0; // probably not working?
	};
	class CDamageClassesPropsMgr : public IDamageClassPropsMgr { // size 11, vtable 6
	public:
		BB::CIDManagerEx* DamageClassManager;
		GGL::DamageClassesHolder Logic; // then copied into here, only these are used, contains same objects (except if there are holes in the ids)
		shok::Vector<GGL::CDamageClassProps*> DamageClass; // loaded into here

		static inline constexpr int vtp = 0x788A54;
		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA0D228);

		void AddDamageClass(shok::DamageClassId id, GGL::CDamageClassProps* c);
		void ResetDamageClasses();
	};
	static_assert(sizeof(CDamageClassesPropsMgr) == 11 * 4);
	static_assert(offsetof(CDamageClassesPropsMgr, Logic) == 2 * 4);

	class IHeadsPropsMgr {

	};
	class CGlueHeadsPropsMgr : public IHeadsPropsMgr { // size 29, vtable 8

	};

	struct TerrainTypeData {
		struct LogicData {
			bool Blocked = false, BuildBlocked = false;
			float WalkModifier = 1.0f;
		} Logic;
		struct DisplayData {
			int Priority = 0;
			shok::TerrainTextureId BaseTexture = {};
			shok::TerrainTextureId TransitionsTexture = {};
			shok::TerrainTextureId SnowTexture = {};
			int Quads = 0;
			shok::Color Color{ 0xFF, 0, 0, 0 };
			shok::String ReplacementTerrainType{};
			bool TransitionsColorModulate = false;
		} Display;

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA0D958);
	};
	static_assert(sizeof(TerrainTypeData) == 16 * 4);
	class TerrainPropsLogic {
	public:
		BB::CIDManagerEx* TerrainTypeManager;
		shok::Vector<TerrainTypeData::LogicData> LogicProps;
	};
	class TerrainPropsDisplay {
	public:
		char* TransitionTexture;
		char* SnowTransitionTexture;
		BB::CIDManagerEx* TerrainTypeManager;
		BB::CIDManagerEx* TerrainTextureManager;
		shok::Vector<TerrainTypeData::DisplayData> DisplayProps;
	};
	class ITerrainPropsMgr { // vtable 7
	public:
		virtual void __stdcall Destroy() = 0;
		virtual BB::CIDManagerEx* __stdcall GetTerrainTypeManager() = 0;
		virtual BB::CIDManagerEx* __stdcall GetTerrainTextureManager() = 0;
		virtual TerrainPropsLogic* __stdcall GetLogicProps() = 0;
		virtual TerrainPropsDisplay* __stdcall GetDisplayProps() = 0;
		virtual void __stdcall Load(const char* file) = 0;
		virtual ~ITerrainPropsMgr() = default;

		static inline constexpr int vtp = 0x788AD8;
	};
	class CTerrainPropsMgr : public ITerrainPropsMgr { //size 20
	public:
		BB::CIDManagerEx* TerrainTypeManager;
		BB::CIDManagerEx* TerrainTextureManager;
		TerrainPropsLogic Logic; // 3
		TerrainPropsDisplay Display; // 8
		shok::Vector<TerrainTypeData> TerrainType; //16, cleared after loading

		static inline constexpr int vtp = 0x788BB4;

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA0D8A8);

		void ReloadTerrainTypes();
		void LoadTerrainTypeFromExtraFile(shok::TerrainTypeId id);
	};
	//constexpr int i = offsetof(CTerrainPropsMgr, TerrainType) / 4;

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
		void LoadWaterTypeFromExtraFile(shok::WaterTypeId id);
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

namespace EGL {
	class AnimSetManager {
	public:
		shok::Vector<CGLEAnimSet*> AnimSets;

		CGLEAnimSet* GetAnimSet(shok::AnimSetId id);

		// remember to also free in idmanager, free last id first
		void FreeAnimSet(shok::AnimSetId id);
		void LoadAnimSet(shok::AnimSetId id);
		// remember to also free in idmanager, free last id first
		void PopAnimSet(shok::AnimSetId id);


		static inline AnimSetManager** const GlobalObj = reinterpret_cast<AnimSetManager**>(0x895DE0);
	};
}
