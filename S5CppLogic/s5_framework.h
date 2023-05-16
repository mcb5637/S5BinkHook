#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_sound.h"
#include "s5_glue.h"
#include "s5_scriptsystem.h"
#include "s5_filesystem.h"

namespace ECore {
	class IReplayStreamExtension {
		virtual void unknown0();
	};
}

namespace GS3DTools {
	class CMapData : public ECore::IReplayStreamExtension {
	public:
		shok::String MapName;
		int MapType;
		shok::String MapCampagnName;
		shok::String MapGUID; // theoretically a struct with only a string as member Data

		CMapData& operator=(const CMapData& o);

		static inline constexpr int vtp = 0x761D34;
		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x84E7D0);
	};
}

namespace Framework {
	struct MapInfo {
		shok::String NameKey, DescKey, Name, Desc;
		int SizeX, SizeY;
		bool MPFlag; // 30
		PADDING(3);
		int MPPlayerCount;
		int MPGameOptionFlagSet;
		shok::String MiniMapTextureName;
		bool IsExternalmap; // ? 40
		PADDING(3);
		shok::Vector<int> Keys; // check goes for every map key has to be in shok keys
		struct {
			shok::String Data;
		} GUID;
		shok::String MapFileName, MapFilePath;
	};
	static_assert(sizeof(Framework::MapInfo) == 66 * 4);
	//constexpr int i = offsetof(MapInfo, unknown) / 4;

	struct CampagnInfo {
		shok::Vector<Framework::MapInfo> Maps;
		shok::String CampagnName;

		int GetMapIndexByName(const char* s);
		Framework::MapInfo* GetMapInfoByName(const char* n);

		// load 51AA04 __thiscall(path, shok::Vector<int>* Keys, bool isSp)
	};
	static_assert(sizeof(CampagnInfo) == 11 * 4);
	struct ActualCampagnInfo { // CampaignInfo.xml in the campagn folder
		struct MapInfo {
			int ID;
			int NextMapID;
			shok::String Name;

			static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x87ED18);
		};

		int Desc;
		shok::Vector<MapInfo> Maps; // as far as i know unused
		shok::String Name;
		CampagnInfo Info; // not serialized

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x87EDD0);
	};
	static_assert(sizeof(ActualCampagnInfo) == 23 * 4);

	struct SaveData {
		friend struct SavegameSystem;
		shok::String SavePath;
		GS3DTools::CMapData MapData;
		shok::String AdditionalInfo; // savegame name
		shok::Vector<int> Key; // seems to be unused

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x84E920);
		// ctor 403180, dtor 403083

	private:
		void __thiscall SaveGameOverride(const char* path, GGL::CGLGameLogic* gl, GS3DTools::CMapData* mapdata, const char* name);
	};
	static_assert(sizeof(SaveData) == 41 * 4);

	struct SavegameSystem {
		SaveData* CurrentSave;
		char* SaveDir;
		char* DebugSaveDir;

		bool LoadSaveData(const char* name); // fills CurrentSave
		void SaveGame(const char* slot, GS3DTools::CMapData* mapdata, const char* name, bool debugSave = false);

		static inline Framework::SavegameSystem* (* const GlobalObj)() = reinterpret_cast<Framework::SavegameSystem * (* const)()>(0x403158);

		static void (*OnGameSavedTo)(const char* folder, const char* savename);
		static void HookSaveGame();
	};
}

namespace GDB {
	class CValue : public BB::IObject {
	public:
		float Data;

		static inline constexpr int vtp = 0x76300C;
		static inline constexpr unsigned int Identifier = 0x93BCC6F3;
	};

	class CString : public BB::IObject {
	public:
		shok::String Data;

		static inline constexpr int vtp = 0x76302C;
		static inline constexpr unsigned int Identifier = 0xAA9646F3;
	};

	class CList : public BB::IObject {
	public:
		shok::Map<shok::String, BB::IObject*> Entries; // GDB::CValue, GDB::CString, or GDB::CList

		bool IsKeyValid(const char* key);
		const char* GetString(const char* key);
		void SetString(const char* key, const char* value);
		float GetValue(const char* key);
		void SetValue(const char* key, float value);

		virtual unsigned int __stdcall GetClassIdentifier() const override;

		static inline constexpr int vtp = 0x76289C;
		static inline constexpr int TypeDesc = 0x80040C;
		static inline constexpr unsigned int Identifier = 0x5070AAC3;

		void RemoveKey(const std::string& k);
	};
}

class CTimeManager {
public:
	PADDINGI(1); // 0 int
	PADDINGI(1); // 1 int
	double CurrentGameTimeFactor; // 2
	PADDINGI(1); // 4 int
	PADDINGI(1); // 5 float?
	double FixedUpdateTime; // 6
	PADDINGI(2); // 8 double
	double RealTimeMS; // 10 since start of exe, updated each tick
	PADDINGI(2); // 12 double update time?

	void SetFixedUpdateTime(double t);
	void SetTimeFactor(double f);
	// 406DE2 updates 12 as 10 - 8 / 2 (or just 10 if 0)
};

namespace Framework {
	class IGameCallBacks {
		virtual void unknown0() = 0;
	};
	class CEventTimeManager_UnknownInt {
		PADDINGI(1);
	};
	class CEventTimeManager : public BB::IPostEvent, private CEventTimeManager_UnknownInt, public CTimeManager {
	public:
		PADDINGI(1); // 1 ints
		lua_State* LuaState; // 17

		static inline constexpr int vtp = 0x7630CC;

		virtual void __stdcall PostEvent(BB::CEvent* ev) override;
	};
	static_assert(sizeof(CEventTimeManager) == 4 * 18);

	class CCheckSumCalculator : public BB::IPostEvent {
	public:
		static inline constexpr int vtp = 0x76306C;

		virtual void __stdcall PostEvent(BB::CEvent* ev) override;
	};
	class CEscapeEventHandler : public BB::IPostEvent {
	public:
		static inline constexpr int vtp = 0x763074;

		virtual void __stdcall PostEvent(BB::CEvent* ev) override;
	};

	struct GameModeStartMapData {
		int zero;
		CLuaDebuggerPort* LuaDebuggerPort;
		int* ptoone;
		GS3DTools::CMapData* MapToLoad;
		int* ptozero;
		const char* Folder;
	};

	class AGameModeBase : public BB::IPostEvent {
	public:
		virtual ~AGameModeBase() = default;
		// 2 more pure virt funcs

		lua_State* IngameLuaState;
		PADDINGI(1); // EScr::CInputHandler
		IGameCallBacks* GameCallbacks;
		PADDINGI(2); //0,  EGL::CGLEAnimProps
		CEventTimeManager TimeManager; // 6, CTimeManager 8
		PADDINGI(1394);
		CCheckSumCalculator CheckSumCalc;
		PADDINGI(4);
		CEscapeEventHandler EscapeHandler;
		PADDINGI(2);
		bool IsExternalMap; // 1426
		PADDINGI(1);

		static inline constexpr int vtp = 0x76307C;

		bool AddArchiveIfExternalmap(GameModeStartMapData* data, GS3DTools::CMapData* map, const char* path);
		void RemoveArchiveIfExternalmap();

		static void (*PreStartMap)(lua_State* ingame, const char* name, const char* path, bool externalmap);
		static void (*PreStartMap2)(lua_State* ingame, const char* name, const char* path, bool externalmap);
		static void HookStartMap();
		static bool DoNotRemoveNextArchive;
		static void HookRemoveArchive();
		static void (*PreLoadSave)(lua_State* ingame, GameModeStartMapData* data, bool externalmap);
		static void (*PreLoadSave2)(lua_State* ingame, GameModeStartMapData* data, bool externalmap);
		static void HookLoadSave();

	private:
		bool __thiscall StartMapOverride(const char* name, const char* path);
		void __thiscall RemoveArchiveIfExternalmapOverride();
		void __fastcall OnSaveLoadedEx(Framework::GameModeStartMapData* d);
	};
	static_assert(offsetof(AGameModeBase, IsExternalMap) == 5704);
	static_assert(offsetof(AGameModeBase, EscapeHandler) == 1423 * 4);
	static_assert(offsetof(AGameModeBase, CheckSumCalc) == 1418 * 4);
	//constexpr int i = offsetof(AGameModeBase, TimeManager.RealTimeMS)/4;
	class CSinglePlayerMode : public AGameModeBase {
	public:
		class CNetworkEvent : public BB::IPostEvent {
			CEventTimeManager* TimeManager1;
			CEventTimeManager* TimeManager2;
			BB::CBinarySerializer* BinarySerializer;
			BB::CMemoryStream MemoryStream;

			static inline constexpr int vtp = 0x7632C0;

			virtual void __stdcall PostEvent(BB::CEvent* ev) override;
		};
		CNetworkEvent NetworkEvent;

		static inline constexpr int vtp = 0x7632C8;
	};
	static_assert(sizeof(CSinglePlayerMode) == 1438 * 4);
	class CMultiPlayerMode : public AGameModeBase {
	public:
		class CNetworkEvent : public BB::IPostEvent {
			CEventTimeManager* TimeManager;

			static inline constexpr int vtp = 0x7631B4;

			virtual void __stdcall PostEvent(BB::CEvent* ev) override;
		};
		CNetworkEvent NetworkEvent;

		static inline constexpr int vtp = 0x7631BC;
	};
	static_assert(sizeof(CMultiPlayerMode) == 1430 * 4);


	class CMain : public BB::IPostEvent, public IGameCallBacks, public ESnd::IAmbientSoundInfo {
	public:
		enum class Mode : int {
			MainMenu = 1,
			Singleplayer = 2,
			Multiplayer = 3,
		};
		enum class NextMode : int {
			NoChange = 0,
			StartMapSP = 1,
			ToMainMenu = 2,
			LoadSaveSP = 3,
			StartMapMP = 4,
			RestartMapSP = 5,
			LeaveGame = 6,
		};


		Mode CurrentMode; // 1 mainmenu, 2 ingame (sp?)
		GS3DTools::CMapData CurrentMap; // 4
		//int, GS3DTools::CGUIReplaySystem::vtable 779F80, GS3DTools::CGUIReplaySystem.BB::IPostEvent event 0x518C5B
		// GS3DTools::CMapData::vtable???
		PADDINGI(63);
		shok::String SavegameToLoad; // 90
		PADDINGI(14);
		shok::String Language; // 111
		shok::String OldGameVersion; // 118 unused, SHOKPC1.05
		shok::String UbiComGameName; // 125
		shok::String SoundtrackLanguage; // 132
		PADDINGI(4);
		CLuaDebuggerPort* LuaDebuggerPort;
		lua_State* MainmenuState; // 144
		bool MainmenuInitialized; // just call to reinit instead of init, lua scripts are loaded from winmain
		PADDINGI(3);
	private:
		int one;
	public:
		GGlue::CGluePropsMgr* GluePropsManager; // 150
		PADDINGI(18);
		AGameModeBase* GameModeBase; // 169
		PADDINGI(1);
		NextMode ToDo; // 171
		PADDINGI(2);
		struct CIH {
			shok::Vector<ActualCampagnInfo> Campagns; // access with -1 + name
			CampagnInfo Infos[4];
			struct {
				shok::Vector<int> Keys;
				// set to extra num 51BA6A __thiscall
				// check normal 51B9F7 __thiscall(shok::vector<int>*)
				// check sp 51BA3C __thiscall(shok::vector<int>*) for some reason also checks if vectors sizes match
			} Keys;

			Framework::CampagnInfo* GetCampagnInfo(int i, const char* n);
			Framework::CampagnInfo* GetCampagnInfo(GS3DTools::CMapData* d);

			// load maps 51908F __thiscall
			// load campagns 51A11A __thiscall(path, shok::Vector<int>* Keys)

		} CampagnInfoHandler;
		PADDINGI(1);
		GDB::CList GDB; // 227

		static inline constexpr int vtp = 0x76293C;

		void SaveGDB();

		static inline Framework::CMain** const GlobalObj = reinterpret_cast<Framework::CMain**>(0x84EF60);
		static inline const int* const ExtraNum = reinterpret_cast<int*>(0x886BA8);

		static void HookModeChange();
		static void (*OnModeChange)(NextMode mode);
		static void (*OnSaveLoaded)();

	private:
		void __thiscall CheckToDoOverride();
	};
	static_assert(offsetof(Framework::CMain, CurrentMode) == 3 * 4);
	static_assert(offsetof(Framework::CMain, GDB) == 227 * 4);
	static_assert(offsetof(Framework::CMain, ToDo) == 171 * 4);
	static_assert(offsetof(Framework::CMain, GameModeBase) == 169 * 4);
	static_assert(offsetof(Framework::CMain, SavegameToLoad) == 90 * 4);
	static_assert(offsetof(Framework::CMain, GluePropsManager) == 150 * 4);
	static_assert(offsetof(Framework::CMain, CampagnInfoHandler) == 174 * 4);
	//constexpr int i = offsetof(Framework::CMain, MainmenuState) / 4;
}


