#pragma once
#include "s5data.h"

struct shok_framework_mapinfo {
	shok::String NameKey, DescKey, Name, Desc;
	int SizeX, SizeY;
	byte MPFlag;
	PADDING(3);
	int MPPlayerCount;
	int MPGameOptionFlagSet;
	shok::String MiniMapTextureName;
	PADDINGI(1); // 1?
	vector_padding;
	std::vector<int, shok::Allocator<int>> Keys;
	struct {
		shok::String Data;
	} GUID;
	shok::String MapFileName, MapFilePath;
};
static_assert(sizeof(shok_framework_mapinfo) == 66 * 4);

struct shok_framework_campagnInfo {
	vector_padding;
	std::vector<shok_framework_mapinfo, shok::Allocator<shok_framework_mapinfo>> Maps;

	int GetMapIndexByName(const char* s);
	shok_framework_mapinfo* GetMapInfoByName(const char* n);
};

struct shok_GS3DTools_CMapData : shok_object {
	shok::String MapName;
	int MapType;
	shok::String MapCampagnName, MapGUID;

	static inline constexpr int vtp = 0x761D34;
};

struct shok_framework_saveData {
	struct {
		shok::String SavePath;
		shok_GS3DTools_CMapData MapData;
	}* CurrentSave;
	char* SaveDir;
	char* DebugSaveDir;

	bool LoadSaveData(const char* name);

	static inline shok_framework_saveData* (* const GlobalObj)() = reinterpret_cast<shok_framework_saveData * (* const)()>(0x403158);
};



struct shok_GDB_CValue : shok_BB_IObject {
	float Data;

	static inline constexpr int vtp = 0x76300C;
};

struct shok_GDB_CString : shok_BB_IObject {
	shok::String Data;

	static inline constexpr int vtp = 0x76302C;
};

struct shok_GDBEntry {
	shok::String Key;
	shok_BB_IObject* Data; // shok_GDB_CValue, shok_GDB_CString, or shok_GDB_CList
};

struct shok_GDB_CList : shok_BB_IObject {
	shok::Set<shok_GDBEntry> Entries;

	bool IsKeyValid(const char* key);
	const char* GetString(const char* key);
	void SetString(const char* key, const char* value);
	float GetValue(const char* key);
	void SetValue(const char* key, float value);

	static inline constexpr int vtp = 0x76289C;
	static inline constexpr int TypeDesc = 0x80040C;
};

struct shok_Framework_CMain : shok_object {
	int vtable_Framework_IGameCallBacks, vtable_ESnd_IAmbientSoundInfo;
	int CurrentMode; // 1 mainmenu, 2 ingame (sp?)
	shok_GS3DTools_CMapData CurrentMap; // 4
	//int, GS3DTools::CGUIReplaySystem::vtable 779F80, GS3DTools::CGUIReplaySystem.BB::IPostEvent
	// GS3DTools::CMapData::vtable???

	PADDINGI(200);
	shok_GDB_CList GDB; // 227

	static inline constexpr int vtp = 0x76293C;

	shok_framework_campagnInfo* GetCampagnInfo(int i, const char* n);
	shok_framework_campagnInfo* GetCampagnInfo(shok_GS3DTools_CMapData* d);
	void SaveGDB();

	static inline shok_Framework_CMain** const GlobalObj = reinterpret_cast<shok_Framework_CMain**>(0x84EF60);
};
//constexpr int i = offsetof(shok_Framework_CMain, CurrentMap.MapName.size) / 4;
