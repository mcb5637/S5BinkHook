#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_sound.h"
#include "s5_glue.h"
#include "s5_scriptsystem.h"
#include "s5_filesystem.h"
#include "EnumIdManagerMagic.h"

namespace ECore {
	class IReplayStreamExtension {
		virtual void unknown0();
	};

	class CReplayHandler : public BB::IPostEvent {
		// 7 BB::CBinarySerializer* of CReplayMgr
	};

	class IReplayMgr {
		virtual void unknown0() = 0;
	};
	class CReplayMgr : public IReplayMgr {
		CReplayHandler* ReplayHandler; // probably unique ptr
		PADDINGI(2);
		BB::CBinarySerializer* Serializer;
	};
}

namespace GS3DTools {
	class CMapData : public ECore::IReplayStreamExtension {
	public:
		shok::String MapName;
		shok::MapType MapType;
		shok::String MapCampagnName;
		shok::String MapGUID; // 16 theoretically a struct with only a string as member Data

		CMapData& operator=(const CMapData& o);

		static inline constexpr int vtp = 0x761D34;
		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x84E7D0);
	};
	static_assert(offsetof(CMapData, MapGUID) == 16 * 4);

	class CGUIReplaySystem : public ECore::IReplayStreamExtension, public BB::IPostEvent {
	public:
		virtual void __stdcall PostEvent(BB::CEvent* ev) override;

		class CPlayingReplay : public BB::IPostEvent {
			virtual void __stdcall PostEvent(BB::CEvent* ev) override;
		};

		CMapData MapData;
		int DisplaySizeX, DisplaySizeY; // 25
		PADDINGI(1);
		PADDINGI(2); // 28 double -1
		PADDINGI(3); // last float 0.001
		shok::String SomeString; // 33
		PADDINGI(1);
		PADDINGI(2);
		CPlayingReplay PlayingReplay; // 43

		static inline constexpr int vtp = 0x779F80;

		// ctor 518EBB
		// set displ size 518BA1
	};
	static_assert(offsetof(CGUIReplaySystem, SomeString) == 33 * 4);
	static_assert(offsetof(CGUIReplaySystem, PlayingReplay) == 43 * 4);
	//constexpr int i = offsetof(CGUIReplaySystem, PlayingReplay) / 4;
}

namespace Framework {
	struct SKeys {
		shok::Vector<int> Keys;

		bool Check(const SKeys& map) const;
		bool CheckSP(const SKeys& map) const;
		// set to extra num 51BA6A __thiscall
	};

	struct MapInfo {
		shok::String NameKey, DescKey, Name, Desc;
		int SizeX = 0, SizeY = 0;
		bool MPFlag = false; // 30
		PADDING(3);
		int MPPlayerCount = 0;
		int MPGameOptionFlagSet = 0;
		shok::String MiniMapTextureName;
		bool IsExternalmap = false; // ? 40
		PADDING(3);
		SKeys Keys; // check goes for every map key has to be in shok keys
		struct {
			shok::String Data;
		} GUID;
		shok::String MapFileName, MapFilePath;

		static inline BB::SerializationData* const SerializationData = reinterpret_cast<BB::SerializationData*>(0x87EA10);
	};
	static_assert(sizeof(Framework::MapInfo) == 66 * 4);
	//constexpr int i = offsetof(MapInfo, unknown) / 4;

	struct CampagnInfo {
		shok::Vector<Framework::MapInfo> Maps;
		shok::String CampagnName;

		int GetMapIndexByName(const char* s);
		Framework::MapInfo* GetMapInfoByName(const char* n);

		// load 51AA04 __thiscall(path, SKeys* Keys, bool isSp) memoryleaks BB::CBBArchiveFile objects, no point in using the original
		static void HookLoad();
		void __thiscall LoadOverride(const char* path, const SKeys* keys, bool isSp = false);
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
		static void (*OnGameSavedTo2)(const char* folder, const char* savename);
		static void (*PreGameSavedTo)(const char* path, GGL::CGLGameLogic* gl, GS3DTools::CMapData* mapdata, const char* name);
		static void (*PostGameSavedTo)(const char* path, GGL::CGLGameLogic* gl, GS3DTools::CMapData* mapdata, const char* name);
		static void HookSaveGame();
		static bool(*IsSaveValidOverride)(std::string_view save);
		static void HookSavegameValid();
	private:
		static int SavegameValidOverride(lua::State L);
	};
}

namespace GDB {
	class CValue : public BB::IObject {
	public:
		float Data;

		static inline constexpr int vtp = 0x76300C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x93BCC6F3);
	};

	class CString : public BB::IObject {
	public:
		shok::String Data;

		static inline constexpr int vtp = 0x76302C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xAA9646F3);
	};

	class CList : public BB::IObject {
	public:
		shok::Map<shok::String, BB::IObject*> Entries; // GDB::CValue, GDB::CString, or GDB::CList

		bool IsKeyValid(const char* key);
		const char* GetString(const char* key);
		void SetString(const char* key, const char* value);
		float GetValue(const char* key);
		void SetValue(const char* key, float value);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static inline constexpr int vtp = 0x76289C;
		static inline constexpr int TypeDesc = 0x80040C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x5070AAC3);

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
		BB::IPostEvent* ReplayHandler; // ECore::CReplayHandler
		lua_State* LuaState; // 17

		static inline constexpr int vtp = 0x7630CC;

		virtual void __stdcall PostEvent(BB::CEvent* ev) override; // handles time event directly, everyting else goes to replayhandler
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

	class AGameModeBase : public BB::IPostEvent { // postevent does ui events (partial), to GGUI::CManager (GGUI::CMouseEffect) and EScr::CInputHandler
	public:
		virtual ~AGameModeBase() = default;
		virtual void Update(int) = 0; // todo param? p to one of CMain, embedded struct?
		virtual BB::IPostEvent* GetCNetworkEvent() = 0;

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
		static void __stdcall FireMapStartTrigger(lua_State* L, const char* f);
		static void __stdcall FireSaveLoadTrigger(lua_State* L, const char* f);
		bool __thiscall LoadSaveAddS5xOverride(GameModeStartMapData* data, GS3DTools::CMapData* map, const char* path);
		static bool __fastcall LoadSaveCheckIsSaveValid(GameModeStartMapData* data);
	};
	static_assert(offsetof(AGameModeBase, IsExternalMap) == 5704);
	static_assert(offsetof(AGameModeBase, EscapeHandler) == 1423 * 4);
	static_assert(offsetof(AGameModeBase, CheckSumCalc) == 1418 * 4);
	//constexpr int i = offsetof(AGameModeBase, TimeManager.RealTimeMS)/4;
	class CSinglePlayerMode : public AGameModeBase {
	public:
		class CNetworkEvent : public BB::IPostEvent {
			CEventTimeManager* TimeManager1;
			CEventTimeManager* TimeManager2; // same pointer
			BB::CBinarySerializer* BinarySerializer;
			BB::CMemoryStream MemoryStream; // stores GUI events between recieving and executing, then goes to TimeManager2

			static inline constexpr int vtp = 0x7632C0;

			virtual void __stdcall PostEvent(BB::CEvent* ev) override; // speed event to TimeManager1, otherwise stored to MemoryStream

			// update 40F9B5 __thiscall()
		};
		CNetworkEvent NetworkEvent; //1428

		static inline constexpr int vtp = 0x7632C8;

		static void (*PreUpdate)();
		static void HookUpdate();
	private:
		int __thiscall UpdateOverride(int u);
	};
	static_assert(sizeof(CSinglePlayerMode) == 1438 * 4);
	static_assert(offsetof(CSinglePlayerMode, NetworkEvent) == 1428 * 4);
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
		PADDINGI(1);
		GS3DTools::CGUIReplaySystem ReplaySystem; // 28
		PADDINGI(4);
		shok::String ReplayToLoad; // 76 from commandline, not sure if it does anything
		shok::String GUIReplay; // 83 from commandline, not sure it does anything
		shok::String SavegameToLoad; // 90
		shok::String SaveAfterXSeconds; // 97 from commandline, not sure it does anything
		PADDINGI(2);
		bool UseExeDir, MP, GrabScreenshots; // 106 from commandline, not sure if they do anything
		PADDINGI(1);
		double Tickrate; // 108
		bool HideGUI, RecordGUI; // 110
		shok::String Language; // 111
		shok::String OldGameVersion; // 118 unused, SHOKPC1.05
		shok::String UbiComGameName; // 125
		shok::String SoundtrackLanguage; // 132
		int ExtraCmdOnly; // 139
		bool DebugScript;
		PADDINGI(2);
		CLuaDebuggerPort* LuaDebuggerPort;
		lua_State* MainmenuState; // 144
		bool MainmenuInitialized; // just call to reinit instead of init, lua scripts are loaded from winmain
		BBRw::CEngine* Engine; // 146
		ED::CGUIScene* GUIScene;
		ESnd::CSoESound* Sound;
	private:
		int one; // some update value?
	public:
		GGlue::CGluePropsMgr* GluePropsManager; // 150
		PADDINGI(9);
		struct SWindowData {
			HWND MainWindow;
			void* someFunc;
			PADDINGI(1);
			bool Windowed;
			int Width;
			int Height;
			int ColorDepth;
			PADDINGI(1); // some bool that never seems to be able to be true
			int TextureResolution;
		} WindowData; // 160
		AGameModeBase* GameModeBase; // 169
		PADDINGI(1);
		NextMode ToDo; // 171
		PADDINGI(2);
		struct CIH {
			shok::Vector<ActualCampagnInfo> Campagns; // access with -1 + name
			CampagnInfo Infos[4];
			SKeys Keys;

			Framework::CampagnInfo* GetCampagnInfo(shok::MapType i, const char* n);
			Framework::CampagnInfo* GetCampagnInfo(GS3DTools::CMapData* d);

			// load maps 51908F __thiscall
			// load campagns 51A11A __thiscall(path, shok::Vector<int>* Keys)

		} CampagnInfoHandler;
		struct SUserPaths {
			shok::String SaveGames, // 0
				Temp_DebugSaves, // 7
				Temp_Replays, // 14
				Data, // 21
				Temp_Logs_TinCat, // 28
				Data2, // 35
				Temp_UbiCom, // 42
				Script, // 49
				Screenshots, // 56
				Temp_Grab, // 63
				Temp_Logs_Game, // 70
				Temp_MiniDump; // 77
			shok::String Empties[7];
		}*UserPaths;
		GDB::CList GDB; // 227

		static inline constexpr int vtp = 0x76293C;

		// parse cmd args 0x4082F3 uknownObj->__thiscall(const char* str, void* CMainp4)

		void SaveGDB();
		ED::CGUICamera* GetCamera(); // gets mainmenu or ingame camera

		static inline Framework::CMain** const GlobalObj = reinterpret_cast<Framework::CMain**>(0x84EF60);
		static inline const int* const ExtraNum = reinterpret_cast<int*>(0x886BA8);

		static void HookModeChange();
		static void (*OnModeChange)(NextMode mode);
		static void (*OnSaveLoaded)();
		static void (*MainmenuUpdate)();

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
	static_assert(sizeof(Framework::CMain::SWindowData) == 4 * 9);
	static_assert(sizeof(Framework::CMain::SUserPaths) == 133 * 4);
	//constexpr int i = sizeof(Framework::CMain::SUserPaths) / 4;
	//constexpr int i = offsetof(Framework::CMain, ReplayToLoad) / 4;
}

namespace CppLogic {
	template<>
	inline auto GetIdManager<shok::MapType>() {
		return CppLogic::MagicEnum::EnumIdManager<shok::MapType>{};
	}
}
