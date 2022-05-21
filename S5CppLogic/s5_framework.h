#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_sound.h"
#include "s5_glue.h"

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
		shok::String MapCampagnName, MapGUID;

		static inline constexpr int vtp = 0x761D34;
	};
}

namespace Framework {
	struct MapInfo {
		shok::String NameKey, DescKey, Name, Desc;
		int SizeX, SizeY;
		byte MPFlag;
		PADDING(3);
		int MPPlayerCount;
		int MPGameOptionFlagSet;
		shok::String MiniMapTextureName;
		PADDINGI(1); // 1?
		shok::Vector<int> Keys;
		struct {
			shok::String Data;
		} GUID;
		shok::String MapFileName, MapFilePath;
	};
	static_assert(sizeof(Framework::MapInfo) == 66 * 4);

	struct CampagnInfo {
		shok::Vector<Framework::MapInfo> Maps;

		int GetMapIndexByName(const char* s);
		Framework::MapInfo* GetMapInfoByName(const char* n);
	};

	struct SaveData {
		struct {
			shok::String SavePath;
			GS3DTools::CMapData MapData;
		}*CurrentSave;
		char* SaveDir;
		char* DebugSaveDir;

		bool LoadSaveData(const char* name);

		static inline Framework::SaveData* (* const GlobalObj)() = reinterpret_cast<Framework::SaveData * (* const)()>(0x403158);
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
		struct GDBEntry {
			shok::String Key;
			BB::IObject* Data; // GDB::CValue, GDB::CString, or GDB::CList
		};


		shok::Set<GDBEntry> Entries;

		bool IsKeyValid(const char* key);
		const char* GetString(const char* key);
		void SetString(const char* key, const char* value);
		float GetValue(const char* key);
		void SetValue(const char* key, float value);

		virtual unsigned int __stdcall GetClassIdentifier() const override;

		static inline constexpr int vtp = 0x76289C;
		static inline constexpr int TypeDesc = 0x80040C;
		static inline constexpr unsigned int Identifier = 0x5070AAC3;
	};
}

namespace Framework {
	class AGameModeBase : public BB::IPostEvent {
	public:
		virtual ~AGameModeBase() = default;
		// 2 more pure virt funcs

		static inline constexpr int vtp = 0x76307C;
	};
	class CSinglePlayerMode : public AGameModeBase {
	public:
		static inline constexpr int vtp = 0x7632C8;
	};
	class CMultiPlayerMode : public AGameModeBase {
	public:
		static inline constexpr int vtp = 0x7631BC;
	};


	class IGameCallBacks {
		virtual void unknown0() = 0;
	};

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
		//int, GS3DTools::CGUIReplaySystem::vtable 779F80, GS3DTools::CGUIReplaySystem.BB::IPostEvent
		// GS3DTools::CMapData::vtable???
		PADDINGI(63);
		shok::String SavegameToLoad; // 90

		PADDINGI(47);
		lua_State* MainmenuState; // 144
		bool MainmenuInitialized; // just call to reinit instead of init, lua scripts are loaded from winmain
		PADDINGI(4);
		GGlue::CGluePropsMgr* GluePropsManager; // 150
		PADDINGI(18);
		AGameModeBase* GameModeBase; // 169
		PADDINGI(1);
		NextMode ToDo; // 171
		PADDINGI(55);
		GDB::CList GDB; // 227

		static inline constexpr int vtp = 0x76293C;

		Framework::CampagnInfo* GetCampagnInfo(int i, const char* n);
		Framework::CampagnInfo* GetCampagnInfo(GS3DTools::CMapData* d);
		void SaveGDB();

		static inline Framework::CMain** const GlobalObj = reinterpret_cast<Framework::CMain**>(0x84EF60);

		static void HookModeChange();
		static void (*OnModeChange)(NextMode mode);
	};
	static_assert(offsetof(Framework::CMain, CurrentMode) == 3 * 4);
	static_assert(offsetof(Framework::CMain, GDB) == 227 * 4);
	static_assert(offsetof(Framework::CMain, ToDo) == 171 * 4);
	static_assert(offsetof(Framework::CMain, GameModeBase) == 169 * 4);
	static_assert(offsetof(Framework::CMain, SavegameToLoad) == 90 * 4);
	static_assert(offsetof(Framework::CMain, GluePropsManager) == 150 * 4);
	//constexpr int i = offsetof(Framework::CMain, GluePropsManager) / 4;
}


