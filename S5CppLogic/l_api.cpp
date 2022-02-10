#include "pch.h"

#include "l_api.h"

#include <libloaderapi.h>
#include <sstream>
#include <string_view>
#include <array>

#include "luaext.h"

namespace CppLogic::API {

	void CheckEvalEnabled(lua::State L) {
		if (HasSCELoader())
			L.Error("Loading lua code disabled for Kimichura");
	}

	bool IsExternalmap(const char* s) {
		std::string_view str{ s };
		return str.rfind("data\\maps\\externalmap\\", 0) != std::string_view::npos;
	}

	int Eval(lua::State L) {
		CheckEvalEnabled(L);
		size_t strlen = 0;
		const char* s = L.CheckString(1, &strlen);
		lua::ErrorCode r = L.LoadBuffer(s, strlen, "Eval");
		L.Push(r == lua::ErrorCode::Success);
		return 2;
	}

	int Log(lua::State L) {
		const char* s = L.CheckString(1);
		shok::LogString("%s\n", s);
		return 0;
	}

	int StackTrace(lua::State L) {
		L.Push(L.GenerateStackTrace(L.OptInteger(1, 0), L.OptInteger(2, -1), L.OptBool(3, false), L.OptBool(4, false)));
		return 1;
	}

	int GetFuncDebug(lua::State L) {
		L.Assert(L.IsFunction(1), "no func");
		L.PushValue(1);
		lua::DebugInfo ar = L.Debug_GetInfoForFunc(lua::DebugInfoOptions::Source | lua::DebugInfoOptions::Name | lua::DebugInfoOptions::Line | lua::DebugInfoOptions::Upvalues);

		L.NewTable();
		L.Push("name");
		L.Push(ar.Name);
		L.SetTableRaw(-3);
		L.Push("namewhat");
		L.Push(ar.NameWhat);
		L.SetTableRaw(-3);
		L.Push("nups");
		L.Push(ar.NumUpvalues);
		L.SetTableRaw(-3);
		L.Push("short_src");
		L.Push(ar.ShortSrc);
		L.SetTableRaw(-3);
		L.Push("linedefined");
		L.Push(ar.LineDefined);
		L.SetTableRaw(-3);
		L.Push("what");
		L.Push(ar.What);
		L.SetTableRaw(-3);
		return 1;
	}

	int ReadFileAsString(lua::State L) {
		size_t strlen = 0;
		const char* s = L.CheckString(1);
#ifndef _DEBUG
		if (!IsExternalmap(s))
			L.Error("not a map file");
#endif
		const char* data = ReadFileToString(s, &strlen);
		if (!data)
			return 0;
		L.Push(data, strlen);
		delete[] data;
		return 1;
	}

	int LDoesFileExist(lua::State L) {
		const char* s = L.CheckString(1);
		L.Push(DoesFileExist(s));
		return 1;
	}

	int DoString(lua::State L) {
		CheckEvalEnabled(L);
		size_t strlen = 0;
		const char* s = L.CheckString(1, &strlen);
		const char* na = L.CheckString(2);
		L.Push(static_cast<int>(L.DoString(s, strlen, na)));
		return L.GetTop() - 2;
	}

	int MapGetDataPath(lua::State L) {
		int ty = L.CheckInt(2);
		const char* cn = L.OptString(3, nullptr); // optional
		const char* n = L.CheckString(1);
		shok_framework_campagnInfo* ci = (*shok_Framework_CMain::GlobalObj)->GetCampagnInfo(ty, cn);
		L.Assert(ci, "invalid map type/campagn");
		shok_framework_mapinfo* i = ci->GetMapInfoByName(n);
		L.Assert(i, "invalid map");
		L.Push(i->MapFilePath.c_str());
		return 1;
	}

	int SaveGetMapInfo(lua::State L) {
		const char* save = L.CheckString(1);
		shok_framework_saveData* sdata = shok_framework_saveData::GlobalObj();
		L.Assert(sdata->LoadSaveData(save), "save doesnt exist");
		L.Push(sdata->CurrentSave->MapData.MapName.c_str());
		L.Push(sdata->CurrentSave->MapData.MapType);
		L.Push(sdata->CurrentSave->MapData.MapCampagnName.c_str());
		L.Push(sdata->CurrentSave->MapData.MapGUID.c_str());
		return 4;
	}

	int GetGDB(lua::State L) {
		L.NewTable();
		std::function<void(shok_GDBEntry*)> a = [&L, &a](shok_GDBEntry* e) {
			L.Push(e->Key.c_str());
			if (e->Data->vtable == shok_GDB_CString::vtp)
				L.Push(static_cast<shok_GDB_CString*>(e->Data)->Data.c_str());
			else if (e->Data->vtable == shok_GDB_CValue::vtp)
				L.Push(static_cast<shok_GDB_CValue*>(e->Data)->Data);
			else if (e->Data->vtable == shok_GDB_CList::vtp) {
				L.CheckStack(4, "stackoverflow");
				L.NewTable();
				static_cast<shok_GDB_CList*>(e->Data)->Entries.ForAll(a);
			}
			else
				L.Push(true);
			L.SetTableRaw(-3);
		};
		(*shok_Framework_CMain::GlobalObj)->GDB.Entries.ForAll(a);
		return 1;
	}

	void GetRuntimeStore() {
		lua::State mm{ mainmenu_state };
		mm.PushLightUserdata(&GetRuntimeStore);
		mm.GetTableRaw(mm.REGISTRYINDEX);
		if (mm.IsNil(-1)) {
			mm.NewTable();
			mm.PushLightUserdata(&GetRuntimeStore);
			mm.PushValue(-2);
			mm.SetTableRaw(mm.REGISTRYINDEX);
		}
	}
	int RuntimeStoreSet(lua::State L) {
		lua::State mm{ mainmenu_state };
		int t = mm.GetTop();

		const char* s = L.CheckString(1);
		const char* ds = nullptr;
		lua::Number di = 0;
		lua::LType ty = L.Type(2);
		L.Assert(ty == lua::LType::Number || ty == lua::LType::String || ty == lua::LType::Nil, "not saveable");
		if (ty == lua::LType::Number)
			di = L.ToNumber(2);
		else if (ty == lua::LType::String)
			ds = L.ToString(2);
		GetRuntimeStore();
		mm.Push(s);
		if (ty == lua::LType::Number)
			mm.Push(di);
		else if (ty == lua::LType::String)
			mm.Push(ds);
		else
			mm.Push();
		mm.SetTableRaw(-3);

		mm.SetTop(t);
		return 0;
	}
	int RuntimeStoreGet(lua::State L) {
		lua::State mm{ mainmenu_state };
		int t = mm.GetTop();

		const char* s = L.CheckString(1);
		GetRuntimeStore();
		mm.Push(s);
		mm.GetTableRaw(-2);
		lua::LType ty = mm.Type(-1);
		const char* ds = nullptr;
		lua::Number di = 0;
		if (ty == lua::LType::Number)
			di = mm.ToNumber(-1);
		else if (ty == lua::LType::String)
			ds = mm.ToString(-1);

		mm.SetTop(t);

		if (ty == lua::LType::Number)
			L.Push(di);
		else if (ty == lua::LType::String)
			L.Push(ds);
		else
			L.Push();

		return 1;
	}

	int CreateExtraDataTables(lua_State* L) {
		(*shok_BB_CIDManagerEx::AnimManager)->DumpManagerToLuaGlobal(L, "Animations");
		(*shok_BB_CIDManagerEx::TerrainTypeManager)->DumpManagerToLuaGlobal(L, "TerrainTypes");
		(*shok_BB_CIDManagerEx::WaterTypeManager)->DumpManagerToLuaGlobal(L, "WaterTypes");
		(*shok_BB_CIDManagerEx::ArmorClassManager)->DumpManagerToLuaGlobal(L, "ArmorClasses");
		return 0;
	}

	constexpr std::array<lua::FuncReference, 13> API{ {
			lua::FuncReference::GetRef<Eval>("Eval"),
			lua::FuncReference::GetRef<Log>("Log"),
			lua::FuncReference::GetRef<StackTrace>("StackTrace"),
			lua::FuncReference::GetRef<ReadFileAsString>("ReadFileAsString"),
			lua::FuncReference::GetRef<LDoesFileExist>("DoesFileExist"),
			lua::FuncReference::GetRef<DoString>("DoString"),
			lua::FuncReference::GetRef<MapGetDataPath>("MapGetDataPath"),
			lua::FuncReference::GetRef<SaveGetMapInfo>("SaveGetMapInfo"),
			lua::FuncReference::GetRef<GetGDB>("GetGDB"),
			lua::FuncReference::GetRef<RuntimeStoreSet>("RuntimeStoreSet"),
			lua::FuncReference::GetRef<RuntimeStoreGet>("RuntimeStoreGet"),
			lua::FuncReference::GetRef<CreateExtraDataTables>("CreateExtraDataTables"),
			lua::FuncReference::GetRef<GetFuncDebug>("GetFuncDebug"),
	} };

	void Init(lua::State L)
	{
		L.RegisterFuncs(API, -3);
	}

}

// CppLogic.API.Log("string")
// CppLogic.API.ReadFileAsString("data\\maps\\externalmap\\info.xml")