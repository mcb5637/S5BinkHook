#pragma once
#include <random>

#include "enumflags.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace EGL {
	class ITerrainVertexColors {
	public:
		virtual ~ITerrainVertexColors() = default;
		// more funcs for serualization

		static inline constexpr int vtp = 0x7841DC;
	};
	class CTerrainVertexColors : public ITerrainVertexColors {
	public:
		int ArraySizeX, ArraySizeY;
		int* VertexColors;

		static inline constexpr int vtp = 0x7841F8;

		void ToTerrainCoord(shok::Position& p, int* out);
		bool IsCoordValid(int* out);
		int GetTerrainVertexColor(shok::Position& p);
		void SetTerrainVertexColor(shok::Position& p, int col); // a,r,g,b each int8
	};

	class CGLETerrainHiResDisplay : public BB::IObject {

	};
	class CGLETerrainHiRes : public CGLETerrainHiResDisplay {
	public:
		friend class EGL::CGLELandscape;
		shok::Vector<int16_t> TerrainHeights;
		int MaxSizeX, MaxSizeY; // 5
		int ArraySizeX, ArraySizeY; // 7

		static inline constexpr int vtp = 0x7837B0;

		static void ToTerrainCoord(const shok::Position& p, int* out);
		bool IsCoordValid(int* out);
		bool IsCoordValid(int x, int y);
		int GetTerrainHeight(const shok::Position& p);
		void SetTerrainHeight(const shok::Position& p, int h); // int16
	private:
		int GetTerrainHeight(int x, int y);
	};

	class CGLETerrainLowResDisplay : public BB::IObject {

	};
	class CGLETerrainLowRes : public CGLETerrainLowResDisplay {
	public:
		friend class EGL::CGLELandscape;
		shok::Vector<int> Data; // terrain type &0xFF, water type &0x3F00 >>8, water height &0x3FFFC000 >>14
		shok::Vector<int> BridgeHeights;
		int MaxSizeX, MaxSizeY; // 9
		int ArraySizeX, ArraySizeY; // 11

		static inline constexpr int vtp = 0x7837C0;

		static void ToQuadCoord(const shok::Position& p, int* out);
		bool IsCoordValid(const int* out);
		bool IsCoordValid(int x, int y);
		bool IsBridgeHeightCoordValid(int x, int y);
		int GetTerrainTypeAt(const shok::Position& p);
		void SetTerrainTypeAt(const shok::Position& p, int tty); // byte (int8)
		int GetWaterTypeAt(const shok::Position& p);
		void SetWaterTypeAt(const shok::Position& p, int wty); // int6
		int GetWaterHeightAt(const shok::Position& p);
		void SetWaterHeightAt(const shok::Position& p, int wh); // int16
		int GetBridgeHeight(const shok::Position& p);
		void SetBridgeHeight(const shok::Position& p, int bh);
		// hires coords if HiResBridgeHeightEnabled
		inline int* GetBridgeHeightP(int x, int y);


		static bool HiResBridgeHeightEnabled;
		static void EnableHiResBridgeHeight();
		static void ClearBridgeArea();
		static int* Dbg_bh;
	private:
		int GetWaterHeightAt(int x, int y);
		void CheckBridgeHeightSize();
		float GetBridgeHeightFloat(float x, float y);
	};

	class CTiling { // this thing has some strange inheritance going on...
		virtual void unknown0() = 0;
		virtual void unknown1() = 0;
		virtual void unknown2() = 0;
		virtual void unknown3() = 0;
		virtual void unknown4() = 0;
	public:
		virtual void OnPreBlockingAddedAt(int x, int y) = 0;
		virtual void OnPreBlockingMode1Removed(int x, int y) = 0;
		virtual void OnPostBlockingMode2Removed(int x, int y) = 0; // 5
	private:
		virtual void unknown5() = 0;
		virtual void unknown6() = 0;
		virtual void unknown7() = 0;
		virtual void unknown8() = 0;
		virtual void unknown9() = 0;
		virtual void unknown10() = 0;
	public:
		virtual bool GetSomeGlobal() = 0; // 14
	public:
		static inline constexpr int vtp = 0x783BAC;
	};

	class IRegionInfo {
	public:
		virtual ~IRegionInfo() = default;
	};
	class CRegionInfo : public IRegionInfo {
		static inline constexpr int vtp = 0x783878;
	};

	class CLandscapeBase {
		virtual void unknown() = 0;
	};
	class CGLELandscape : public CLandscapeBase {
	public:
		enum class BlockingMode : byte {
			None = 0x0,
			Blocked = 0x1,
			BridgeArea = 0x2,
			BuildBlock = 0x4,
			TerrainSlope = 0x8,
		};

		EGL::LandscapeBlockingData* BlockingData;
		PADDINGI(1); //  0 array?
		EGL::CTiling* Tiling;
		PADDINGI(3); // set/list of BB::TSlotEx1<EGL::CGLEGameLogic,EGL::C2DVector const &>
		EGL::CGLETerrainHiRes* HiRes;
		EGL::CGLETerrainLowRes* LowRes;
		EGL::CTerrainVertexColors* VertexColors; // 9
		bool IsNotWinter;

		static inline constexpr int vtp = 0x783C38;

		// only works with 90° rotation steps, cause behind the scenes its still an AA rect. maybe fix if needed.
		struct AdvancedAARectIterator {
			struct Coord {
				int x = 0, y = 0;
			};
			Coord Low, High;

			struct Iter {
			private:
				const AdvancedAARectIterator* I;
				Coord Curr;
			public:
				const Coord& operator*() const;
				bool operator==(const Iter& o) const;
				bool operator!=(const Iter& o) const;
				Iter& operator++();
				Iter operator++(int);
				Iter(const AdvancedAARectIterator& i, const Coord& c);
			};

			AdvancedAARectIterator(const shok::Position& p, const shok::AARect& area, float rot, bool LowRes, bool AddOne);
			AdvancedAARectIterator(const shok::Position& p, const shok::AARect& area, float rot, bool LowRes);
			bool HasNext(const Coord& Curr) const;
			void ToNext(Coord& Curr) const;
			Iter begin() const;
			Iter end() const;
		};

		int GetSector(const shok::Position* p);
		bool GetNearestPositionInSector(const shok::Position* pIn, float range, int sector, shok::Position* pOut);
		shok::Position GetNearestFreePos(const shok::Position* p, float range);
		bool IsValidPos(shok::Position* p);
		shok::Position GetMapSize();
		bool IsPosBlockedInMode(const shok::Position* p, BlockingMode mode);
		BlockingMode GetBlocking(const shok::Position& p);
		void FlattenPosForBuilding(const shok::Position& p, const shok::AARect& area, float rot);
		// block for vector of aarect: thiscall 577B07 (this, pos*, vector<aarect>*, float, byte*)
		// unblock for vector of aarect: thiscall 577C12 (this, pos*, vector<aarect>*, float, byte*)
		void ApplyBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode);
		void RemoveBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode);
		void AdvancedApplyBridgeHeight(const shok::Position& p, const shok::AARect& area, float rot, int height);
		void UpdateBlocking(const shok::AARect& area);
		void WeatherChangeBlockingUpdate();
		void AdvancedRemoveBridgeHeight(const shok::Position& p, const shok::AARect& area, float rot);
		void AdvancedApplyBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode);
		void AdvancedRemoveBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode);
		bool IsAreaUnblockedInMode(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode mode, bool AddOne);
		bool IsAreaNotUnderWater(const shok::Position& p, const shok::AARect& area, float rot, bool AddOne);
	private:
		void RemoveSingleBlockingPoint(int x, int y, BlockingMode mode); // this probably got inlined by the compiler originally...
	};
	template<>
	class ::enum_is_flags<CGLELandscape::BlockingMode> : public std::true_type {};
	//constexpr int i = offsetof(CGLELandscape, CurrentWeather) / 4;

	class LandscapeBlockingData {
	public:
		friend class EGL::CGLELandscape;
		int ArraySizeXY;
		byte* data;

		bool IsCoordValid(int x, int y);
		shok::Position GetFreeBuildingPlacementPos(const GGL::CGLBuildingProps* bprops, const shok::PositionRot& pos, float range);
		inline EGL::CGLELandscape::BlockingMode GetBlockingData(int x, int y);
	};

	class CPlayerExplorationHandler : public BB::IObject {
	public:
		struct ExCircle {
			int CenterV, CenterW, Radius;
		};

		int PlayerID;
		int ShareExplorationWithPlayersMask;
		int SizeX, SizeY; // 3
		EGL::C2DArray1Bit* ExplorationMapWork; // 5 updating?
		EGL::C2DArray1Bit* ExplorationMapCurrent;
		EGL::C2DArray1Bit* ExplorationMapSeen;
		shok::Vector<ExCircle> ExplorationCircle; // 8
		unsigned int UpdateCounter; // 12

		bool IsPositionExplored(const shok::Position& p);

		static inline constexpr int vtp = 0x784E04;
		static constexpr unsigned int Identifier = 0xEE20FA93;
	};

	class CPlayerExplorationUpdate : public BB::IObject {
	public:
		int State;
		int FirstPlayerToUpdate;
		int LastPlayerToUpdate;
		int DrawCirclesCurrentPlayerToUpdate;
		int DrawCirclesCurrentPlayerDrawCircles;
		int DrawCirclesCurrentIndex;
		int DrawCirclesTurnCounter;
		unsigned int DrawCirclesNumCirclesToDrawPerTurn;

		static inline constexpr int vtp = 0x784D1C;
		static constexpr unsigned int Identifier = 0x81E57CD;

		void SetPlayersToUpdate(int first, int last);
	};
	static_assert(sizeof(CPlayerExplorationUpdate) == 9 * 4);

	class PlayerManager { // name from the file in savegames
	public:
		struct Player {
			bool PlayerInGame;
			BB::IObject* PlayerData; // nullptr mp related?
			CPlayerExplorationHandler* ExplorationHandler;
			BB::IObject* FeedbackHandler; // EGL::CPlayerFeedbackHandler
			BB::IObject* EntityVectorMap; // EGL::CEntityVectorMap
		};
		Player Players[9];
		PADDINGI(1); // seridata-> belongs to array, 0 wtf???
		CPlayerExplorationUpdate* ExplorationUpdate; //46

		EGL::CPlayerExplorationHandler* GetExplorationHandlerByPlayer(int pl);
		void SetShareExplorationFlag(int pl1, int pl2, bool share);
		void ActivateUpdateOfExplorationForAllPlayers();
		CPlayerExplorationUpdate* GetUpdate(); // creates if nullptr

		// seridata of this thing is 0x897508, but it is missing its guard
	};
	static_assert(sizeof(PlayerManager::Player) * 9 + 4 == 184);
	static_assert(offsetof(PlayerManager, ExplorationUpdate) == 46 * 4);

	struct LogicGameTime {
		int Tick;
		int TicksPerMS;
		float CurrentWeatherSpeedFactor;
	};

	struct RegionDataEntity { // not sure if it has anything to do with region
		struct Entry {
			shok::Vector<EGL::CGLEEntity*> EntitiesByAccessCategory[7];
			PADDINGI(2);

			shok::Vector<EGL::CGLEEntity*>& GetByAccessCategory(shok::AccessCategory ac);
		};
		shok::Vector<Entry> Data;

		Entry* GetEntry(const shok::Position& p);
		void GetEntryComponents(const shok::Position& p, int& x, int& y);
		Entry* GetEntry(int x, int y);
		int GetSingleEntryComponent(float x);
	private:
		size_t GetEntryIdByComponents(int x, int y);
		size_t GetEntryId(const shok::Position& p);
		// add entity 587AF1
		// remove entity 587A1C
		// on entity move 587B4D __thiscall(this, entity, lastpos)
	};
	static_assert(sizeof(RegionDataEntity::Entry) == 120);

	class IGLEGameLogic : public BB::IPostEvent {

	};
	using RandomNumberGenerator = std::linear_congruential_engine<uint32_t, 1812433253, 1, 0>;
	static_assert(sizeof(RandomNumberGenerator) == 4);
	class CGLEGameLogic : public IGLEGameLogic {
		PADDINGI(6);
		LogicGameTime* InGameTime; // 7
		PADDINGI(1);
		EGL::CGLELandscape* Landscape; // 9
		RegionDataEntity RegionDataEntityObj; // 10
		PADDINGI(5);
		EGL::PlayerManager* PlayerMng; // 19
		PADDINGI(8);
		shok::Vector<EGL::CGLEEntity*> ToDestroy; // 28 not sure of something other that entities ends up here
		PADDINGI(2);
		RandomNumberGenerator RNG; // 34 pretty sure this is not c++ std originally, but this one does the same as the original.

	private:
		virtual void unknown1() = 0;
	public:
		virtual ~CGLEGameLogic() = default; // 2
	private:
		virtual int get2ndmember() = 0;
		virtual void unknown3() = 0;
		virtual int get1stmember() = 0; // 5
		virtual void unknown4() = 0;
		virtual void unknown5() = 0;
		virtual void unknown6() = 0;
		virtual void unknown7() = 0;
		virtual void unknown8() = 0; // 10
		virtual void unknown9() = 0;
		virtual void unknown10() = 0;
		virtual void Tick() = 0;
		virtual int GetTickVT() = 0;
		virtual void StartMap(const char* mapPath) = 0; // 15
		virtual void Serialize(const char* folder) = 0;
		virtual void Deserialize(const char* folder) = 0;
		virtual void unknown16() = 0;
		virtual void unknown17() = 0;
		virtual void unknown18() = 0; // 20
		virtual void unknown19() = 0;
		virtual int CreateEntity(EGL::CGLEEntityCreator* data, int i) = 0; // 22
	public:
		virtual int CreateEffect(EGL::CGLEEffectCreator* data) = 0;

		static inline constexpr int vtp = 0x7839CC;

		int CreateEntity(EGL::CGLEEntityCreator* cr);
		int GetTimeMS();
		int GetTick();
		float GetTimeSeconds();
		void ClearToDestroy();

		static void HookCreateEffect();
		static void(*CreateEffectHookCallback)(int id);

		static inline EGL::CGLEGameLogic** const GlobalObj = reinterpret_cast<EGL::CGLEGameLogic**>(0x895DAC); // also 85A3A4
		static inline int* const MapSize = reinterpret_cast<int*>(0x898B74);

	private:
		int CreateEffectOverride(EGL::CGLEEffectCreator* data);
	};
	static_assert(offsetof(CGLEGameLogic, RNG) == 4 * 34);
	//constexpr int i = offsetof(CGLEGameLogic, RNG)/4;
}

namespace GGL {
	class IWeatherSystem {
		// no vtable
	};
	class CWeatherHandler : public IWeatherSystem {
		virtual void GetStateChangingProgress(void*) = 0;
	public:
		virtual shok::WeatherState GetCurrentWeatherState() = 0;
		struct WeatherElementData {
			int StartTimeOffset = 0; // ticks, sec*10
			int Length = 0; // ticks, sec*10
			shok::WeatherState State = static_cast<shok::WeatherState>(0);
			PADDINGI(1); // nonperiodic index?
			bool IsPeriodic = false;
			PADDING(3);
			int WeatherIndex = 0; //5
			int Forerun = 0;
			int GfxSet = 0;
			int Transition = 0;
		};

		shok::WeatherState CurrentWeatherState;
		int CurrentWeatherIndex;
		PADDINGI(1);// 3 next nonperiodic weather?
		int NextWeatherIndex;
		int CurrentWeatherOffset; // 5
		shok::Map<int, WeatherElementData> Elements;
		int NextPeriodicWeatherStartTimeOffset;
		struct { // 10
			int CurrentWeatherGFXState; //0
			int FromGFXState;
			int StartedTimeOffset;
			int ToGFXState; //3 0 if no chnage
			int TransitionLength;
			shok::WeatherState StateToChangeFrom;
			shok::WeatherState StateToChangeTo;
			int WeatherStateChangeTimeOffset; //7
			int WIndexToChangeTo;
			int State; // 1 changing, 2 fix
		} WeatherChange;


		static inline constexpr int vtp = 0x770140;
		static inline constexpr int TypeDesc = 0x811DE8;

		shok::WeatherState GetNextWeatherState();
		int GetTicksToNextPeriodicWeatherChange();
		void AddWeatherElement(int state, int dur, bool peri, int forerun, int gfx, int transition); // all times in ticks
		void SetOffset(int o);
		void ClearQueue(shok::WeatherState state, int dur, int forerun, int gfx, int transition);

		static inline BB::SerializationData* (__stdcall* const SerializationData)() = reinterpret_cast<BB::SerializationData * (__stdcall*)()>(0x49ECE9);
	};

	class IGLGameLogic {
	public:
		virtual void __stdcall Destroy() = 0;
	protected:
		virtual ~IGLGameLogic() = default;
	private:
		virtual int SomethingStream(BB::IStream* s) = 0;
		virtual void unknown0(int) = 0;
		virtual void __stdcall Tick() = 0;
		virtual void Serialize(const char* savename) = 0; // 5
		virtual void Deserialize(const char* savename) = 0;
		virtual void unknown3(int) = 0;
		virtual GGL::CWeatherHandler* GetWeatherHandler() = 0;
		virtual void unknown4(int, int) = 0;
		virtual void unknown5() = 0;
	public:
		virtual void AddNetEventHandler(EGL::IGLEHandler<BB::CEvent, void>* h) = 0;
		virtual void HandleNetEvent(BB::CEvent* ev) = 0;
	};
	class CGLGameLogic : public IGLGameLogic, public BB::IPostEvent {
		PADDINGI(8);
	public:
		GGL::CPlayerStatus** players; // 10
		GGL::CWeatherHandler* WeatherHandler;
		shok::TechCategoryManager* TechCategoryManager;
		shok::TechManager* TechManager; // 13
		PADDINGI(1);
		bool GlobalInvulnerability, AreWeatherMachineEffectsActive;
		PADDING(2);

		static inline constexpr int vtp = 0x76E018;

		GGL::CPlayerStatus* GetPlayer(int i);
		shok::Technology* GetTech(int i);
		void EnableAlarmForPlayer(int pl);
		void DisableAlarmForPlayer(int pl);
		void UpgradeSettlerCategory(int pl, int ucat);
		void PlayerActivateWeathermachine(int player, int weathertype);
		void PlayerBlessSettlers(int player, int blessCat);
		void SetDiplomacy(int p1, int p2, shok::DiploState state);

		static inline GGL::CGLGameLogic** const GlobalObj = reinterpret_cast<GGL::CGLGameLogic**>(0x85A3A0);
		// create net event handlers thiscall 0x49FD49()
		static inline BB::SerializationData* (__stdcall* const SerializationDataPlayerArray)() = reinterpret_cast<BB::SerializationData * (__stdcall*)()>(0x49BE76);
		// only globalinvul and weathermachineactive
		static inline BB::SerializationData* (__stdcall* const SerializationData)() = reinterpret_cast<BB::SerializationData * (__stdcall*)()>(0x49AC82);

		// on tech researched event void __stdcall(this, const GGL::CNetEventEventTechnologyPlayerIDAndEntityID&) 0x49A75F, put into GGL::PlayerTechManager
	};
}
