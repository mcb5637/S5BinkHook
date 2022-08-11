#include "pch.h"
#include <libloaderapi.h>
#include <sstream>
#include <string_view>
#include <array>
#include <processthreadsapi.h>
#include <ctime>
#include "l_api.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_filesystem.h"
#include "s5_framework.h"
#include "s5_scriptsystem.h"
#include "s5_idmanager.h"
#include "s5_config.h"
#include "hooks.h"
#include "luaext.h"

namespace CppLogic::API {
	void CheckEvalEnabled(lua::State L) {
		if (CppLogic::HasSCELoader())
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
		if (!L.IsFunction(1))
			throw lua::LuaException("no func");
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
		const char* data = BB::CFileSystemMgr::ReadFileToString(s, &strlen);
		if (!data)
			return 0;
		L.Push(data, strlen);
		delete[] data;
		return 1;
	}

	int LDoesFileExist(lua::State L) {
		const char* s = L.CheckString(1);
		L.Push(BB::CFileSystemMgr::DoesFileExist(s));
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
		Framework::CampagnInfo* ci = (*Framework::CMain::GlobalObj)->GetCampagnInfo(ty, cn);
		if (!ci)
			throw lua::LuaException("invalid map type/campagn");
		Framework::MapInfo* i = ci->GetMapInfoByName(n);
		if (!i)
			throw lua::LuaException("invalid map");
		L.Push(i->MapFilePath.c_str());
		return 1;
	}

	int SaveGetMapInfo(lua::State L) {
		const char* save = L.CheckString(1);
		Framework::SaveData* sdata = Framework::SaveData::GlobalObj();
		if (!sdata->LoadSaveData(save))
			throw lua::LuaException("save doesnt exist");
		L.Push(sdata->CurrentSave->MapData.MapName.c_str());
		L.Push(sdata->CurrentSave->MapData.MapType);
		L.Push(sdata->CurrentSave->MapData.MapCampagnName.c_str());
		L.Push(sdata->CurrentSave->MapData.MapGUID.c_str());
		return 4;
	}

	void PushGDBList(lua::State L, const GDB::CList& list) {
		L.NewTable();
		for (const auto& e : list.Entries) {
			L.Push(e.first.c_str());
			if (const GDB::CString* s = dynamic_cast<const GDB::CString*>(e.second))
				L.Push(s->Data.c_str());
			else if (const GDB::CValue* v = dynamic_cast<const GDB::CValue*>(e.second))
				L.Push(v->Data);
			else if (const GDB::CList* l = dynamic_cast<const GDB::CList*>(e.second)) {
				L.CheckStack(4, "stackoverflow");
				PushGDBList(L, *l);
			}
			else
				L.Push(true);
			L.SetTableRaw(-3);
		}
	}

	int GetGDB(lua::State L) {
		PushGDBList(L, (*Framework::CMain::GlobalObj)->GDB);
		return 1;
	}

	int RemoveGDBKey(lua::State L) {
		std::string k = L.CheckStdString(1);
		(*Framework::CMain::GlobalObj)->GDB.RemoveKey(k);
		return 0;
	}

	void GetRuntimeStore() {
		lua::State mm{ shok::LuaStateMainmenu };
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
		lua::State mm{ shok::LuaStateMainmenu };
		int t = mm.GetTop();

		const char* s = L.CheckString(1);
		const char* ds = nullptr;
		lua::Number di = 0;
		lua::LType ty = L.Type(2);
		if (!(ty == lua::LType::Number || ty == lua::LType::String || ty == lua::LType::Nil))
			throw lua::LuaException("not saveable");
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
		lua::State mm{ shok::LuaStateMainmenu };
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

	int CreateExtraDataTables(lua::State L) {
		(*BB::CIDManagerEx::AnimManager)->DumpManagerToLuaGlobal(L.GetState(), "Animations");
		(*BB::CIDManagerEx::TerrainTypeManager)->DumpManagerToLuaGlobal(L.GetState(), "TerrainTypes");
		(*BB::CIDManagerEx::WaterTypeManager)->DumpManagerToLuaGlobal(L.GetState(), "WaterTypes");
		(*BB::CIDManagerEx::ArmorClassManager)->DumpManagerToLuaGlobal(L.GetState(), "ArmorClasses");

		{
			L.Push("ExperienceClasses");
			L.NewTable();

			L.Push("Invalid");
			L.Push(static_cast<int>(shok::ExperienceClass::Invalid));
			L.SetTableRaw(-3);

			auto* mng = GGL::ExperienceClassHolder::GlobalObj();
			for (int i = 0; i < static_cast<int>(mng->Classes.size()); ++i) {
				L.Push(mng->Classes[i]->Table.c_str());
				L.Push(i);
				L.SetTableRaw(-3);
			}

			L.SetTableRaw(L.GLOBALSINDEX);
		}


		return 0;
	}

	int MainThreadID = 0;
	int GetMainThreadID(lua::State L) {
		L.Push(MainThreadID);
		return 1;
	}
	int GetCurrentThreadID(lua::State L) {
		L.Push(static_cast<int>(GetCurrentThreadId()));
		return 1;
	}

	int LGetCurrentTime(lua::State L) {
		L.Push(static_cast<double>(std::time(nullptr)));
		return 1;
	}

	constexpr std::array<lua::FuncReference, 17> API{ {
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
			lua::FuncReference::GetRef<GetMainThreadID>("GetMainThreadID"),
			lua::FuncReference::GetRef<GetCurrentThreadID>("GetCurrentThreadID"),
			lua::FuncReference::GetRef<LGetCurrentTime>("GetCurrentTime"),
			lua::FuncReference::GetRef<RemoveGDBKey>("RemoveGDBKey"),
	} };

	void Init(lua::State L)
	{
		L.RegisterFuncs(API, -3);
		MainThreadID = GetCurrentThreadId();
	}
}

// CppLogic.API.Log("string")
// CppLogic.API.ReadFileAsString("data\\maps\\externalmap\\info.xml")