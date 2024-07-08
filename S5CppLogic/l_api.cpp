#include "pch.h"
#include <libloaderapi.h>
#include <sstream>
#include <string_view>
#include <array>
#include <processthreadsapi.h>
#include <ctime>
#include <filesystem>
#include "l_api.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_filesystem.h"
#include "s5_framework.h"
#include "s5_scriptsystem.h"
#include "s5_idmanager.h"
#include "s5_config.h"
#include "s5_exception.h"
#include "s5_cutscene.h"
#include "s5_netevents.h"
#include "s5_entity.h"
#include "hooks.h"
#include "luaext.h"
#include "SchemaGenerator.h"

namespace CppLogic::API {
	void CheckEvalEnabled(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException{ "Loading lua code disabled for Kimichura" };
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
		L.Push(L.GenerateStackTrace(L.OptInt(1, 0), L.OptInt(2, -1), L.OptBool(3, false), L.OptBool(4, false)));
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
			throw lua::LuaException{ "not a map file" };
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
		Framework::CampagnInfo* ci = (*Framework::CMain::GlobalObj)->CampagnInfoHandler.GetCampagnInfo(static_cast<shok::MapType>(ty), cn);
		if (!ci)
			throw lua::LuaException("invalid map type/campagn");
		Framework::MapInfo* i = ci->GetMapInfoByName(n);
		if (!i)
			throw lua::LuaException("invalid map");
		L.Push(i->MapFilePath.c_str());
		L.Push(i->GUID.Data.c_str());
		L.NewTable();
		int j = 1;
		for (int k : i->Keys.Keys) {
			L.Push(k);
			L.SetTableRaw(-2, j);
			++j;
		}
		return 3;
	}

	int SaveGetMapInfo(lua::State L) {
		const char* save = L.CheckString(1);
		Framework::SavegameSystem* sdata = Framework::SavegameSystem::GlobalObj();
		if (!sdata->LoadSaveData(save))
			throw lua::LuaException("save doesnt exist");
		L.Push(sdata->CurrentSave->MapData.MapName.c_str());
		L.Push(static_cast<int>(sdata->CurrentSave->MapData.MapType));
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
			di = *L.ToNumber(2);
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
			di = *mm.ToNumber(-1);
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

			L.SetGlobal();
		}

		L.Push("NetEvents");
		CppLogic::GetIdManager<shok::NetEventIds>().PushToState(L);
		L.SetGlobal();

		L.Push("AdvancedDealDamageSource");
		CppLogic::GetIdManager<shok::AdvancedDealDamageSource>().PushToState(L);
		L.SetGlobal();

		return 0;
	}

	std::filesystem::path GetPersistentMapFilesDir() {
		std::filesystem::path p{ Framework::SavegameSystem::GlobalObj()->SaveDir };
		p.replace_filename("PersistentMapFiles");
		return p;
	}
	void AppendPersistendMapFileName(lua::State L, std::filesystem::path& p, int indexoff) {
		int ty = L.CheckInt(2 + indexoff);
		const char* cn = L.OptString(3 + indexoff, nullptr); // optional
		const char* n = L.CheckString(1 + indexoff);
		Framework::CampagnInfo* ci = (*Framework::CMain::GlobalObj)->CampagnInfoHandler.GetCampagnInfo(static_cast<shok::MapType>(ty), cn);
		if (!ci)
			throw lua::LuaException("invalid map type/campagn");
		Framework::MapInfo* i = ci->GetMapInfoByName(n);
		if (!i)
			throw lua::LuaException("invalid map");
		p.append(i->GUID.Data.c_str()).replace_extension("bin");
	}

	int SavePersistentMapFile(lua::State L) {
		std::filesystem::path p = GetPersistentMapFilesDir();
		AppendPersistendMapFileName(L, p, 1);

		try {
			BB::CFileStreamEx fs{};
			if (!fs.OpenFile(p.string().c_str(), BB::IStream::Flags::DefaultWrite))
				throw lua::LuaException{ "cannot open file" };
			CppLogic::Serializer::AdvLuaStateSerializer seri{ fs, L.GetState() };
			seri.SerializeVariable(1);
			fs.Close();
		}
		catch (const BB::CFileException& ex) {
			char buff[255]{};
			ex.CopyMessage(buff, 254);
			throw lua::LuaException{ buff };
		}

		L.Push(p.string().c_str());
		return 1;
	}
	int LoadPersistentMapFile(lua::State L) {
		std::filesystem::path p = GetPersistentMapFilesDir();
		AppendPersistendMapFileName(L, p, 0);

		try {
			BB::CFileStreamEx fs{};
			if (!fs.OpenFile(p.string().c_str(), BB::IStream::Flags::DefaultRead))
				throw lua::LuaException{ "cannot open file" };
			CppLogic::Serializer::AdvLuaStateSerializer seri{ fs, L.GetState() };
			seri.DeserializeVariable();
			fs.Close();
		}
		catch (const BB::CFileException& ex) {
			char buff[255]{};
			ex.CopyMessage(buff, 254);
			throw lua::LuaException{ buff };
		}

		return 1;
	}
	int HasPersistentMapFile(lua::State L) {
		std::filesystem::path p = GetPersistentMapFilesDir();
		AppendPersistendMapFileName(L, p, 0);

		bool direxists = std::filesystem::exists(p) && std::filesystem::is_directory(p);

		L.Push(std::filesystem::exists(p) && std::filesystem::is_regular_file(p));
		L.Push(direxists);
		return 2;
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

	int GetCurrentCutscene(lua::State L) {
		L.Push((**ECS::CManager::GlobalObj)->ActiveCutscene.c_str());
		return 1;
	}

	int ReloadExternalmaps(lua::State L) {
		auto* m = *Framework::CMain::GlobalObj;
		if (m->CurrentMode != Framework::CMain::Mode::MainMenu)
			throw lua::LuaException{"not in mainmenu"};
		m->CampagnInfoHandler.Infos[3].LoadOverride("maps\\user", &m->CampagnInfoHandler.Keys);
		return 0;
	}

	int RNG::Int(lua::State L)
	{
		RNG* th = L.CheckUserClass<RNG>(1);
		int min = L.CheckInt(2);
		int max;
		if (L.IsNoneOrNil(3)) {
			max = min;
			min = 1;
		}
		else
			max = L.CheckInt(3);
		std::uniform_int_distribution d{ min, max };
		L.Push(d(th->Core));
		return 1;
	}

	int RNG::Number(lua::State L)
	{
		RNG* th = L.CheckUserClass<RNG>(1);
		double min;
		double max;
		if (L.IsNoneOrNil(1)) {
			min = 0;
			max = 1;
		}
		else if (L.IsNoneOrNil(3)) {
			min = 1;
			max = L.CheckNumber(2);
		}
		else {
			min = L.CheckNumber(2);
			max = L.CheckNumber(3);
		}
		std::uniform_real_distribution d{ min, max };
		L.Push(d(th->Core));
		return 1;
	}

	int RNG::Serialize(lua::State L)
	{
		RNG* th = L.CheckUserClass<RNG>(1);
		L.Push(typename_details::type_name<RNG>());
		static_assert(sizeof(RNG) == 1 * 4);
		L.PushFString("%d", *reinterpret_cast<int*>(th));
		return 2;
	}

	RNG::RNG()
		: Core(static_cast<unsigned int>(std::time(nullptr)))
	{
	}

	RNG::RNG(unsigned int seed)
		: Core(seed)
	{
	}

	int RNG::Deserialize(lua::State L)
	{
		const char* s = L.CheckString(1);
		auto* ud = L.NewUserClass<RNG>();
		*reinterpret_cast<int*>(ud) = std::atol(s);
		return 1;
	}

	void RNG::Register(lua::State L)
	{
		L.PrepareUserClassType<RNG>();
		CppLogic::Serializer::AdvLuaStateSerializer::UserdataDeserializer[std::string{ typename_details::type_name<RNG>() }] = &lua::State::CppToCFunction<RNG::Deserialize>;
	}

	int CreateRNG(lua::State L) {
		if (L.IsNoneOrNil(1))
			L.NewUserClass<RNG>();
		else
			L.NewUserClass<RNG>(L.CheckInt(1));
		return 1;
	}

#ifdef _DEBUG
	int GenerateClassSchemas(lua::State L) {
		BB::CFileStreamEx f{};
		f.OpenFile(L.CheckString(1), BB::IStream::Flags::DefaultWrite);
		CppLogic::Serializer::SchemaGenerator::WriteAllClassesSchema(f);
		f.Close();
		return 0;
	}
	int DumpUnknownFieldSerializers(lua::State L) {
		CppLogic::Serializer::SchemaGenerator::PushUnknownFieldSerializers(L);
		return 1;
	}
#endif

	constexpr std::array API{
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
			lua::FuncReference::GetRef<SavePersistentMapFile>("SavePersistentMapFile"),
			lua::FuncReference::GetRef<LoadPersistentMapFile>("LoadPersistentMapFile"),
			lua::FuncReference::GetRef<HasPersistentMapFile>("HasPersistentMapFile"),
			lua::FuncReference::GetRef<CreateRNG>("CreateRandomNumberGenerator"),
			lua::FuncReference::GetRef<GetCurrentCutscene>("GetCurrentCutscene"),
#ifdef _DEBUG
			lua::FuncReference::GetRef<GenerateClassSchemas>("GenerateClassSchemas"),
			lua::FuncReference::GetRef<DumpUnknownFieldSerializers>("DumpUnknownFieldSerializers"),
#endif
	};

	void Init(lua::State L)
	{
		L.RegisterFuncs(API, -3);
		if (L.GetState() == shok::LuaStateMainmenu) {
			L.RegisterFunc<ReloadExternalmaps>("ReloadExternalmaps", -3);
		}
		MainThreadID = GetCurrentThreadId();
		RNG::Register(L);
	}
}

// CppLogic.API.Log("string")
// CppLogic.API.ReadFileAsString("data\\maps\\externalmap\\info.xml")