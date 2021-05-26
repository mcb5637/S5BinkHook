#pragma once
#include "s5data.h"

struct shok_framework_mapinfo {
	shok_string NameKey, DescKey, Name, Desc;
	int SizeX, SizeY;
	byte MPFlag;
	PADDING(3);
	int MPPlayerCount;
	int MPGameOptionFlagSet;
	shok_string MiniMapTextureName;
	PADDINGI(1); // 1?
	vector_padding;
	std::vector<int, shok_allocator<int>> Keys;
	struct {
		shok_string Data;
	} GUID;
	shok_string MapFileName, MapFilePath;
};
static_assert(sizeof(shok_framework_mapinfo) == 66 * 4);

struct shok_framework_campagnInfo {
	vector_padding;
	std::vector<shok_framework_mapinfo, shok_allocator<shok_framework_mapinfo>> Maps;

	int GetMapIndexByName(const char* s);
	shok_framework_mapinfo* GetMapInfoByName(const char* n);
};

struct shok_GS3DTools_CMapData : shok_object {
	shok_string MapName;
	int MapType;
	shok_string MapCampagnName, MapGUID;

	static inline constexpr int vtp = 0x761D34;
};

struct shok_framework_saveData {
	struct {
		shok_string SavePath;
		shok_GS3DTools_CMapData MapData;
	}* CurrentSave;
	char* SaveDir;
	char* DebugSaveDir;

	bool LoadSaveData(const char* name);
};
static inline shok_framework_saveData* (* const shok_getframework_saveDataObj)() = (shok_framework_saveData * (* const)()) 0x403158;

struct shok_Framework_CMain : shok_object {
	int vtable_Framework_IGameCallBacks, vtable_ESnd_IAmbientSoundInfo;
	PADDINGI(1); // 2
	shok_GS3DTools_CMapData CurrentMap;
	//int, GS3DTools::CGUIReplaySystem::vtable 779F80, GS3DTools::CGUIReplaySystem.BB::IPostEvent
	// GS3DTools::CMapData::vtable???


	static inline constexpr int vtp = 0x76293C;

	shok_framework_campagnInfo* GetCampagnInfo(int i, const char* n);
	shok_framework_campagnInfo* GetCampagnInfo(shok_GS3DTools_CMapData* d);
};

static inline shok_Framework_CMain** const shok_Framework_CMainObj = (shok_Framework_CMain**)0x84EF60;
