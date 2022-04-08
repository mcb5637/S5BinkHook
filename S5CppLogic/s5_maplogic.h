#pragma once
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
		EGL::CTerrainVertexColors* VertexColors;

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
		void AdvancedRemoveBridgeHeight(const shok::Position& p, const shok::AARect& area, float rot);
		void AdvancedApplyBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode);
		void AdvancedRemoveBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode);
		bool IsAreaUnblockedInMode(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode mode, bool AddOne);
		bool IsAreaNotUnderWater(const shok::Position& p, const shok::AARect& area, float rot, bool AddOne);
	private:
		void RemoveSingleBlockingPoint(int x, int y, BlockingMode mode); // this probably got inlined by the compiler originally...
	};
	constexpr EGL::CGLELandscape::BlockingMode operator&(EGL::CGLELandscape::BlockingMode a, EGL::CGLELandscape::BlockingMode b) {
		using under = std::underlying_type<EGL::CGLELandscape::BlockingMode>::type;
		return static_cast<EGL::CGLELandscape::BlockingMode>(static_cast<under>(a) & static_cast<under>(b));
	}
	constexpr EGL::CGLELandscape::BlockingMode operator|(EGL::CGLELandscape::BlockingMode a, EGL::CGLELandscape::BlockingMode b) {
		using under = std::underlying_type<EGL::CGLELandscape::BlockingMode>::type;
		return static_cast<EGL::CGLELandscape::BlockingMode>(static_cast<under>(a) | static_cast<under>(b));
	}
	constexpr EGL::CGLELandscape::BlockingMode operator^(EGL::CGLELandscape::BlockingMode a, EGL::CGLELandscape::BlockingMode b) {
		using under = std::underlying_type<EGL::CGLELandscape::BlockingMode>::type;
		return static_cast<EGL::CGLELandscape::BlockingMode>(static_cast<under>(a) ^ static_cast<under>(b));
	}

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
		PADDINGI(2);
		int SizeX, SizeY; // 3

		bool IsPositionExplored(const shok::Position& p);

		static inline constexpr int vtp = 0x784E04;
	};

	class GameLogicExplorationStuff { // not sure what exaclty this is
	public:
		EGL::CPlayerExplorationHandler* GetExplorationHandlerByPlayer(int pl);
	};

	class IGLEGameLogic : public BB::IPostEvent {

	};
	class CGLEGameLogic : public IGLEGameLogic {
		PADDINGI(6);
		int* InGameTime; // 7
		PADDINGI(1);
		EGL::CGLELandscape* Landscape; // 9
		PADDINGI(9);
		EGL::GameLogicExplorationStuff* SomeStuff;

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
		virtual void unknown11() = 0;
		virtual void unknown12() = 0;
		virtual void unknown13() = 0; // 15
		virtual void unknown14() = 0;
		virtual void unknown15() = 0;
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

		void HookCreateEffect(); // currently unused
		static void(*CreateEffectHookCallback)(int id, void* ret);

		static inline EGL::CGLEGameLogic** const GlobalObj = reinterpret_cast<EGL::CGLEGameLogic**>(0x895DAC); // also 85A3A4
		static inline int* const MapSize = reinterpret_cast<int*>(0x898B74);
	};
}

namespace GGL {
	class IWeatherSystem {

	};
	class CWeatherHandler : public IWeatherSystem {
		virtual void GetStateChangingProgress(void*) = 0;
	public:
		virtual int GetCurrentWeatherState() = 0;
		struct WeatherElementData {
			int StartTimeOffset = 0; // ticks, sec*10
			int Length = 0; // ticks, sec*10
			int State = 0;
			PADDINGI(1); // nonperiodic index?
			bool IsPeriodic = false;
			PADDING(3);
			int WeatherIndex = 0; //5
			int Forerun = 0;
			int GfxSet = 0;
			int Transition = 0;
		};
		struct KeyAndWeatherElement {
			int WeatherIndex;
			WeatherElementData WeatherElement;
		};

		int CurrentWeatherState;
		int CurrentWeatherIndex;
		PADDINGI(1);// 3 next nonperiodic weather?
		int NextWeatherIndex;
		int CurrentWeatherOffset; // 5
		shok::Set<KeyAndWeatherElement> Elements;
		int NextPeriodicWeatherStartTimeOffset;
		struct { // 10
			int CurrentWeatherGFXState; //0
			int FromGFXState;
			int StartedTimeOffset;
			int ToGFXState; //3 0 if no chnage
			int TransitionLength;
			int StateToChangeFrom;
			int StateToChangeTo;
			int WeatherStateChangeTimeOffset; //7
			int WIndexToChangeTo;
			int State; // 1 changing, 2 fix
		} WeatherChange;


		static inline constexpr int vtp = 0x770140;
		static inline constexpr int TypeDesc = 0x811DE8;

		int GetNextWeatherState();
		int GetTicksToNextPeriodicWeatherChange();
		void AddWeatherElement(int state, int dur, bool peri, int forerun, int gfx, int transition); // all times in ticks
		void ClearQueue(int state, int dur, int forerun, int gfx, int transition);

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
		virtual void Serialize(const char* savename) = 0;
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
		PADDINGI(1);
	private:
		shok::Vector<shok::Technology*>* TechList;
	public:
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

		static inline GGL::CGLGameLogic** const GlobalObj = reinterpret_cast<GGL::CGLGameLogic**>(0x85A3A0);
		// create net event handlers thiscall 0x49FD49()
		static inline BB::SerializationData* (__stdcall* const SerializationDataPlayerArray)() = reinterpret_cast<BB::SerializationData * (__stdcall*)()>(0x49BE76);
		// only globalinvul and weathermachineactive
		static inline BB::SerializationData* (__stdcall* const SerializationData)() = reinterpret_cast<BB::SerializationData * (__stdcall*)()>(0x49AC82);
	};
}
