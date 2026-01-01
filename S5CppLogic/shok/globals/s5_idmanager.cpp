#include "s5_idmanager.h"
#include <shok/s5_mem.h>
#include <shok/ui/s5_ui.h>

static inline int(__thiscall* const shok_getAnimIdByName)(const BB::IIDManager* th, const char* name) = reinterpret_cast<int(__thiscall*)(const BB::IIDManager * th, const char* name)>(0x54F19E);
static inline int(__thiscall* const shok_BB_CIDManager_getidbyname)(void* th, const char* name, int nid) = reinterpret_cast<int(__thiscall*)(void*, const char*, int)>(0x54F656);
int BB::IIDManager::GetIdByName(const char* name) const {
	return shok_getAnimIdByName(this, name);
}
const char* BB::IIDManager::GetNameByID(int id) const
{
	if (id > 0 && id < static_cast<int>(TypeNames.size()))
		return TypeNames[id].Name;
	return nullptr;
}
int BB::IIDManager::GetIDByNameOrCreate(const char* name, int newid)
{
	return shok_BB_CIDManager_getidbyname(this, name, newid);
}
int BB::IIDManager::GetIDByNameOrCreate(const char* name)
{
	return GetIDByNameOrCreate(name, 0);
}

void BB::IIDManager::RemoveID(int id)
{
	auto v = TypeNames.SaveVector();
	shok::Free(v.Vector.at(id).Name);
	v.Vector[id].Name = nullptr;
	v.Vector[id].Hash = 0;
	if (id == static_cast<int>(v.Vector.size() - 1))
		v.Vector.pop_back();
}
static inline void(__stdcall* const idmanager_writemngtoglobal)(lua_State* L, const char* global, BB::IIDManager* mng) = reinterpret_cast<void(__stdcall*)(lua_State*, const char*, BB::IIDManager*)>(0x59C128);
void BB::IIDManager::DumpManagerToLuaGlobal(lua_State* L, const char* global)
{
	idmanager_writemngtoglobal(L, global, this);
}

size_t BB::IIDManager::size() const
{
	return TypeNames.size();
}

void BB::IIDManager::clear()
{
	auto v = TypeNames.SaveVector();
	{
		for (auto& i : v.Vector) {
			shok::Free(i.Name);
			i.Name = nullptr;
		}
	}
	v.Vector.clear();
	v.Vector.push_back({ nullptr, 0 });
}

BB::IIDManager::Iter::Iter(const IIDManager* mng, int id) : Mng(mng), Id(id)
{
}
int BB::IIDManager::Iter::operator*() const noexcept
{
	return Id;
}
BB::IIDManager::Iter& BB::IIDManager::Iter::operator++()
{
	do {
		++Id;
	} while (Mng->GetNameByID(Id) == nullptr && *this != Mng->end());
	return *this;
}
BB::IIDManager::Iter BB::IIDManager::Iter::operator++(int)
{
	Iter r = *this;
	++(*this);
	return r;
}
BB::IIDManager::Iter BB::IIDManager::begin() const
{
	Iter i{ this, 0 };
	++i;
	return i;
}
BB::IIDManager::Iter BB::IIDManager::end() const
{
	return Iter{ this, static_cast<int>(TypeNames.size())};
}

BB::CIDManagerEx * BB::CIDManagerEx::ShortenedFeedBackEventManager() {
	GGUI::SoundFeedback::GlobalObj(); // create it, if it does not exist
	return *reinterpret_cast<BB::CIDManagerEx**>(0x880BA0);
}
