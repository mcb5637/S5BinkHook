#include <Win.h>
#include <string_view>
#include <array>
#include <processthreadsapi.h>
#include <ctime>
#include <filesystem>
#include "l_api.h"
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/globals/s5_filesystem.h>
#include <shok/globals/s5_framework.h>
#include <shok/s5_scriptsystem.h>
#include <shok/globals/s5_idmanager.h>
#include <shok/globals/s5_config.h>
#include <shok/s5_exception.h>
#include <shok/globals/s5_cutscene.h>
#include <shok/events/s5_netevents.h>
#include <shok/entity/s5_entity.h>
#include <utility/hooks.h>
#include <luaext.h>
#include <utility/SchemaGenerator.h>
#include <utility/savegame_extra.h>

namespace CppLogic::API {
	void CheckEvalEnabled(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException{ "Loading lua code disabled for Kimichura" };
	}

	bool IsExternalmap(const char* s) {
		std::string_view str{ s };
		return str.rfind(R"(data\maps\externalmap\)", 0) != std::string_view::npos;
	}

	int Eval(luaext::State L) {
		CheckEvalEnabled(L);
		size_t strlen = 0;
		const char* s = L.CheckString(1, &strlen);
		lua::ErrorCode r = L.LoadBuffer(s, strlen, "Eval");
		L.Push(r == lua::ErrorCode::Success);
		return 2;
	}

	int Log(luaext::State L) {
		const char* s = L.CheckString(1);
		shok::LogString("%s\n", s);
		return 0;
	}

	int StackTrace(luaext::State L) {
		L.Push(L.GenerateStackTrace(L.OptInt(1, 0), L.OptInt(2, -1), L.OptBool(3, false), L.OptBool(4, false)));
		return 1;
	}

	int GetFuncDebug(luaext::State L) {
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

	int ReadFileAsString(luaext::State L) {
		const char* s = L.CheckString(1);
#ifndef DEBUG_FUNCS
		if (!IsExternalmap(s))
			throw lua::LuaException{ "not a map file" };
#endif
		auto data = BB::CFileSystemMgr::ReadFileToString(s);
		L.Push(data);
		return 1;
	}

	int GetFilesInDirectory(luaext::State L) {
		shok::Set<shok::String> m{};
		(*BB::CFileSystemMgr::GlobalObj)->FillFilesInDirectory(&m, L.CheckString(1), BB::IFileSystem::SearchOptions::SkipDirectories);
		L.NewTable();
		int i = 1;
		for (const auto& f : m) {
			L.Push(f);
			L.SetTableRaw(-2, i);
			++i;
		}
		return 1;
	}

	int LDoesFileExist(luaext::State L) {
		const char* s = L.CheckString(1);
		L.Push(BB::CFileSystemMgr::DoesFileExist(s));
		return 1;
	}

	int DoString(luaext::State L) {
		CheckEvalEnabled(L);
		size_t strlen = 0;
		const char* s = L.CheckString(1, &strlen);
		const char* na = L.CheckString(2);
		L.Push(static_cast<int>(L.DoString(s, strlen, na)));
		return L.GetTop() - 2;
	}

	int MapGetDataPath(luaext::State L) {
		const char* n = L.CheckString(1);
		auto ty = L.CheckEnum<shok::MapType>(2);
		const char* cn = L.OptString(3, nullptr); // optional
		bool nt = L.OptBool(4, false);
		Framework::CampagnInfo* ci = (*Framework::CMain::GlobalObj)->CampagnInfoHandler.GetCampagnInfo(ty, cn);
		if (ci == nullptr) {
			if (nt) {
				L.Push("");
				L.Push("");
				L.NewTable();
				L.Push("invalid map type/campagn");
				return 4;
			}
			else {
				throw lua::LuaException("invalid map type/campagn");
			}
		}
		Framework::MapInfo* i = ci->GetMapInfoByName(n);
		if (i == nullptr) {
			if (nt) {
				L.Push("");
				L.Push("");
				L.NewTable();
				L.Push("invalid map");
				return 4;
			}
			else {
				throw lua::LuaException("invalid");
			}
		}
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

	int SaveGetMapInfo(luaext::State L) {
		const char* save = L.CheckString(1);
		bool nt = L.OptBool(2, false);
		Framework::SavegameSystem* sdata = Framework::SavegameSystem::GlobalObj();
		if (!sdata->LoadSaveData(save)) {
			if (nt) {
				L.Push("");
				L.Push(0);
				L.Push("");
				L.Push("");
				L.Push(false);
				return 5;
			}
			else {
				throw lua::LuaException("save doesnt exist");
			}
		}
		L.Push(sdata->CurrentSave->MapData.MapName.c_str());
		L.Push(static_cast<int>(sdata->CurrentSave->MapData.MapType));
		L.Push(sdata->CurrentSave->MapData.MapCampagnName.c_str());
		L.Push(sdata->CurrentSave->MapData.MapGUID.c_str());
		L.Push(true);
		return 5;
	}

	void PushGDBList(luaext::State L, const GDB::CList& list) {
		L.NewTable();
		for (const auto& e : list.Entries) {
			L.Push(e.first.c_str());
			if (const auto* s = dynamic_cast<const GDB::CString*>(e.second))
				L.Push(s->Data.c_str());
			else if (const auto* v = dynamic_cast<const GDB::CValue*>(e.second))
				L.Push(v->Data);
			else if (const auto* l = dynamic_cast<const GDB::CList*>(e.second)) {
				L.CheckStack(4, "stackoverflow");
				PushGDBList(L, *l);
			}
			else
				L.Push(true);
			L.SetTableRaw(-3);
		}
	}

	int GetGDB(luaext::State L) {
		PushGDBList(L, (*Framework::CMain::GlobalObj)->GDB);
		return 1;
	}

	int RemoveGDBKey(luaext::State L) {
		std::string k = L.CheckStdString(1);
		(*Framework::CMain::GlobalObj)->GDB.RemoveKey(k);
		return 0;
	}

	void GetRuntimeStore() {
		luaext::State mm{ shok::LuaStateMainmenu };
		mm.PushLightUserdata(&GetRuntimeStore);
		mm.GetTableRaw(mm.REGISTRYINDEX);
		if (mm.IsNil(-1)) {
			mm.NewTable();
			mm.PushLightUserdata(&GetRuntimeStore);
			mm.PushValue(-2);
			mm.SetTableRaw(mm.REGISTRYINDEX);
		}
	}
	int RuntimeStoreSet(luaext::State L) {
		luaext::State mm{ shok::LuaStateMainmenu };
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
	int RuntimeStoreGet(luaext::State L) {
		luaext::State mm{ shok::LuaStateMainmenu };
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

	int CreateExtraDataTables(luaext::State L) {
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
	void AppendPersistendMapFileName(luaext::State L, std::filesystem::path& p, int indexoff) {
		const char* n = L.CheckString(1 + indexoff);
		auto ty = L.CheckEnum<shok::MapType>(2 + indexoff);
		const char* cn = L.OptString(3 + indexoff, nullptr); // optional
		Framework::CampagnInfo* ci = (*Framework::CMain::GlobalObj)->CampagnInfoHandler.GetCampagnInfo(ty, cn);
		if (!ci)
			throw lua::LuaException("invalid map type/campagn");
		Framework::MapInfo* i = ci->GetMapInfoByName(n);
		if (!i)
			throw lua::LuaException("invalid map");
		p.append(i->GUID.Data.c_str()).replace_extension("bin");
	}

	int SavePersistentMapFile(luaext::State L) {
		std::filesystem::path p = GetPersistentMapFilesDir();
		AppendPersistendMapFileName(L, p, 1);
		auto path = p.string();

		try {
			BB::CFileStreamEx fs{};
			if (!fs.OpenFile(path.c_str(), BB::IStream::Flags::DefaultWrite))
				throw lua::LuaException{ "cannot open file" };
			CppLogic::Serializer::AdvLuaStateSerializer seri{ fs, L.GetState(), L.OptBool(5, false) };
			seri.SerializeVariable(1);
			fs.Close();
		}
		catch (const BB::CFileException& ex) {
			char buff[255]{};
			ex.CopyMessage(buff, 254);
			throw lua::LuaException{ buff };
		}

		L.Push(path);
		return 1;
	}
	int LoadPersistentMapFile(luaext::State L) {
		std::filesystem::path p = GetPersistentMapFilesDir();
		AppendPersistendMapFileName(L, p, 0);
		auto path = p.string();

		try {
			BB::CFileStreamEx fs{};
			if (!fs.OpenFile(path.c_str(), BB::IStream::Flags::DefaultRead))
				throw lua::LuaException{ "cannot open file" };
			CppLogic::Serializer::AdvLuaStateSerializer seri{ fs, L.GetState(),  L.OptBool(4, false) };
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
	int HasPersistentMapFile(luaext::State L) {
		std::filesystem::path p = GetPersistentMapFilesDir();
		AppendPersistendMapFileName(L, p, 0);

		bool direxists = std::filesystem::exists(p) && std::filesystem::is_directory(p);

		L.Push(std::filesystem::exists(p) && std::filesystem::is_regular_file(p));
		L.Push(direxists);
		return 2;
	}

	DWORD MainThreadID = 0;
	int GetMainThreadID(luaext::State L) {
		L.Push(MainThreadID);
		return 1;
	}
	int GetCurrentThreadID(luaext::State L) {
		L.Push(static_cast<int>(GetCurrentThreadId()));
		return 1;
	}

	int LGetCurrentTime(luaext::State L) {
		L.Push(static_cast<double>(std::time(nullptr)));
		return 1;
	}

	int GetCurrentCutscene(luaext::State L) {
		L.Push((**ECS::CManager::GlobalObj)->ActiveCutscene.c_str());
		return 1;
	}

	int EnableScriptTriggerEval(luaext::State L) {
		bool a = L.CheckBool(1);
		EScr::CLuaFuncRefGlobal::HookFuncAccess(a);
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ScriptTriggerEval = a;
		return 0;
	}

	int ReloadExternalmaps(luaext::State L) {
		auto* m = *Framework::CMain::GlobalObj;
		if (m->CurrentMode != Framework::CMain::Mode::MainMenu)
			throw lua::LuaException{"not in mainmenu"};
		m->CampagnInfoHandler.Infos[3].LoadOverride("maps\\user", &m->CampagnInfoHandler.Keys);
		return 0;
	}

	int GetMonitors(luaext::State L) {
		L.NewTable();
		struct D {
			luaext::State L;
			int i = 1;
		};
		D d{ L };

		EnumDisplayMonitors(nullptr, nullptr, [](HMONITOR m, HDC, LPRECT r, LPARAM vd) -> BOOL {
			D* d = reinterpret_cast<D*>(vd);
			MONITORINFOEX mi{};
			mi.cbSize = sizeof(MONITORINFOEX);
			if (GetMonitorInfo(m, &mi)) {
				d->L.NewTable();

				d->L.Push("Name");
				d->L.Push(mi.szDevice);
				d->L.SetTableRaw(-3);
				d->L.Push("H");
				d->L.Push(static_cast<int>(r->bottom - r->top));
				d->L.SetTableRaw(-3);
				d->L.Push("W");
				d->L.Push(static_cast<int>(r->right - r->left));
				d->L.SetTableRaw(-3);


				d->L.SetTableRaw(-2, d->i);
				++d->i;
			}
			return true;
		}, reinterpret_cast<LPARAM>(&d));

		return 1;
	}

	int RNG::Int(luaext::State L)
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

	int RNG::Number(luaext::State L)
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

	int RNG::Serialize(luaext::State L)
	{
		RNG* th = L.CheckUserClass<RNG>(1);
		L.Push(typename_details::type_name<RNG>());
		static_assert(sizeof(RNG) == 1 * 4);
		L.PushFString("%d", *reinterpret_cast<int*>(th));
		return 2;
	}

	RNG::RNG()
		: Core(std::random_device{}())
	{
	}

	RNG::RNG(unsigned int seed)
		: Core(seed)
	{
	}

	int RNG::Deserialize(luaext::State L)
	{
		const char* s = L.CheckString(1);
		auto* ud = L.NewUserClass<RNG>();
		*reinterpret_cast<int*>(ud) = std::atol(s);
		return 1;
	}

	void RNG::Register(luaext::State L)
	{
		L.PrepareUserClassType<RNG>();
		CppLogic::Serializer::AdvLuaStateSerializer::UserdataDeserializer[std::string{ typename_details::type_name<RNG>() }] = &luaext::State::CppToCFunction<RNG::Deserialize>;
	}

	int CreateRNG(luaext::State L) {
		if (L.IsNoneOrNil(1))
			L.NewUserClass<RNG>();
		else
			L.NewUserClass<RNG>(L.CheckInt(1));
		return 1;
	}

#ifdef DEBUG_FUNCS
	int GenerateClassSchemas(luaext::State L) {
		BB::CFileStreamEx f{};
		f.OpenFile(L.CheckString(1), BB::IStream::Flags::DefaultWrite);
		CppLogic::Serializer::SchemaGenerator::WriteAllClassesSchema(f);
		f.Close();
		return 0;
	}
	int DumpUnknownFieldSerializers(luaext::State L) {
		CppLogic::Serializer::SchemaGenerator::PushUnknownFieldSerializers(L);
		return 1;
	}
	int DumpUnknownListOptions(luaext::State L) {
		CppLogic::Serializer::SchemaGenerator::PushUnknownListOptions(L);
		return 1;
	}
#endif

	void ResizeWindow(Framework::CMain::SWindowData* wd) {
		Framework::CMain* f = *Framework::CMain::GlobalObj;
		static constexpr const char* key = "CppLogic\\BordelessFullscreenOn";
		if (!f->GDB.IsKeyValid(key))
			return;
		std::string_view n{ f->GDB.GetString(key) };
		if (n.empty())
			return;

		wd->OverrideSizeBorderlessFullscreen(n);
	}

	constexpr std::array API{
			luaext::FuncReference::GetRef<Eval>("Eval"),
			luaext::FuncReference::GetRef<Log>("Log"),
			luaext::FuncReference::GetRef<StackTrace>("StackTrace"),
			luaext::FuncReference::GetRef<ReadFileAsString>("ReadFileAsString"),
#ifdef DEBUG_FUNCS
			luaext::FuncReference::GetRef<GetFilesInDirectory>("GetFilesInDirectory"),
#endif
			luaext::FuncReference::GetRef<LDoesFileExist>("DoesFileExist"),
			luaext::FuncReference::GetRef<DoString>("DoString"),
			luaext::FuncReference::GetRef<MapGetDataPath>("MapGetDataPath"),
			luaext::FuncReference::GetRef<SaveGetMapInfo>("SaveGetMapInfo"),
			luaext::FuncReference::GetRef<GetGDB>("GetGDB"),
			luaext::FuncReference::GetRef<RuntimeStoreSet>("RuntimeStoreSet"),
			luaext::FuncReference::GetRef<RuntimeStoreGet>("RuntimeStoreGet"),
			luaext::FuncReference::GetRef<CreateExtraDataTables>("CreateExtraDataTables"),
			luaext::FuncReference::GetRef<GetFuncDebug>("GetFuncDebug"),
			luaext::FuncReference::GetRef<GetMainThreadID>("GetMainThreadID"),
			luaext::FuncReference::GetRef<GetCurrentThreadID>("GetCurrentThreadID"),
			luaext::FuncReference::GetRef<LGetCurrentTime>("GetCurrentTime"),
			luaext::FuncReference::GetRef<RemoveGDBKey>("RemoveGDBKey"),
			luaext::FuncReference::GetRef<SavePersistentMapFile>("SavePersistentMapFile"),
			luaext::FuncReference::GetRef<LoadPersistentMapFile>("LoadPersistentMapFile"),
			luaext::FuncReference::GetRef<HasPersistentMapFile>("HasPersistentMapFile"),
			luaext::FuncReference::GetRef<CreateRNG>("CreateRandomNumberGenerator"),
			luaext::FuncReference::GetRef<GetCurrentCutscene>("GetCurrentCutscene"),
			luaext::FuncReference::GetRef<EnableScriptTriggerEval>("EnableScriptTriggerEval"),
			luaext::FuncReference::GetRef<GetMonitors>("GetMonitors"),
#ifdef DEBUG_FUNCS
			luaext::FuncReference::GetRef<GenerateClassSchemas>("GenerateClassSchemas"),
			luaext::FuncReference::GetRef<DumpUnknownFieldSerializers>("DumpUnknownFieldSerializers"),
			luaext::FuncReference::GetRef<DumpUnknownListOptions>("DumpUnknownListOptions"),
#endif
	};

	void Init(luaext::State L)
	{
		L.RegisterFuncs(API, -3);
		if (L.GetState() == shok::LuaStateMainmenu) {
			L.RegisterFunc<ReloadExternalmaps>("ReloadExternalmaps", -3);

			Framework::CMain::ResizeWindow = &ResizeWindow;
			Framework::CMain::HookOverrideWindowInit();
		}
		MainThreadID = GetCurrentThreadId();
		RNG::Register(L);
	}

	void OnSaveLoaded(luaext::State L)
	{
		EScr::CLuaFuncRefGlobal::HookFuncAccess(CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ScriptTriggerEval);
	}

	void Cleanup(luaext::State L)
	{
		EScr::CLuaFuncRefGlobal::HookFuncAccess(false);
	}
}

// CppLogic.API.Log("string")
// CppLogic.API.ReadFileAsString("data\\maps\\externalmap\\info.xml")