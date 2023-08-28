#include "pch.h"
#include <stdexcept>
#include "s5_config.h"
#include "s5_defines.h"
#include "s5_classfactory.h"
#include "ModConfig.h"

EGL::CGLEAnimProps::CGLEAnimProps()
{
    SetVT(vtp);
}
EGL::CGLEAnimProps::CGLEAnimProps(const CGLEAnimProps& o)
{
    SetVT(vtp);
    Duration = o.Duration;
    IsWalkOrRun = o.IsWalkOrRun;
}
EGL::CGLEAnimProps::CGLEAnimProps(CGLEAnimProps&& o) noexcept
{
    SetVT(vtp);
    Duration = o.Duration;
    IsWalkOrRun = o.IsWalkOrRun;
}

void* EGL::CGLEAnimProps::operator new(size_t s)
{
    return shok::Malloc(s);
}
void EGL::CGLEAnimProps::operator delete(void* p) {
    shok::Free(p);
}

shok::ClassId __stdcall EGL::CGLEAnimProps::GetClassIdentifier() const
{
    return Identifier;
}

void EGL::CGLEAnimProps::SetVT(int vt)
{
    *reinterpret_cast<int*>(this) = vt;
}

float& GGL::CDamageClassProps::GetBonusVsArmorClass(shok::ArmorClassId ac)
{
    int i = static_cast<int>(ac);
    if (i > 7) {
        if (auto ex = BB::IdentifierCast<CppLogic::Mod::Config::DamageClassExt>(this)) {
            auto f = ex->ExtraArmorClasses.find(ac);
            if (f != ex->ExtraArmorClasses.end())
                return f->second;
        }
    }
    --i;
    if (i < 0 || i > 6)
        throw std::out_of_range{ "invalid armorclass" };
    return (&ArmorClassNoneFactor)[i];
}

GGL::CDamageClassProps* GGL::DamageClassesHolder::TryGet(shok::DamageClassId id)
{
    int i = static_cast<int>(id);
    if ( i > 0 && i < static_cast<int>(DamageClassList.size()))
        return DamageClassList[i];
    return nullptr;
}

GGL::CLogicProperties::SSettlerUpgradeCategory::SSettlerUpgradeCategory(shok::UpgradeCategoryId cat, shok::EntityTypeId first)
{
    *reinterpret_cast<int*>(this) = vtp;
    Category = cat;
    FirstSettler = first;
}
GGL::CLogicProperties::SSettlerUpgradeCategory::SSettlerUpgradeCategory(SSettlerUpgradeCategory&& o) noexcept
{
    *reinterpret_cast<int*>(this) = vtp;
    Category = o.Category;
    FirstSettler = o.FirstSettler;
}
shok::ClassId GGL::CLogicProperties::SSettlerUpgradeCategory::GetClassIdentifier() const
{
    return Identifier;
}

GGL::CLogicProperties::SBuildingUpgradeCategory::SBuildingUpgradeCategory(shok::UpgradeCategoryId cat, shok::EntityTypeId first)
{
    *reinterpret_cast<int*>(this) = vtp;
    Category = cat;
    FirstBuilding = first;
    NumScholars = 0;
}
GGL::CLogicProperties::SBuildingUpgradeCategory::SBuildingUpgradeCategory(SBuildingUpgradeCategory&& o) noexcept
{
    *reinterpret_cast<int*>(this) = vtp;
    Category = o.Category;
    FirstBuilding = o.FirstBuilding;
    NumScholars = o.NumScholars;
}
shok::ClassId GGL::CLogicProperties::SBuildingUpgradeCategory::GetClassIdentifier() const
{
    return Identifier;
}

static inline GGL::CLogicProperties::STradeResource* (__thiscall* const logicprops_gettraderes)(GGL::CLogicProperties* th, shok::ResourceType rt) = reinterpret_cast<GGL::CLogicProperties::STradeResource * (__thiscall*)(GGL::CLogicProperties*, shok::ResourceType)>(0x4B015F);
GGL::CLogicProperties::STradeResource* GGL::CLogicProperties::GetResource(shok::ResourceType rt)
{
    return logicprops_gettraderes(this, rt);
}

static inline GGL::ExperienceClass::LevelData* (__thiscall* const xpclass_getlevel)(GGL::ExperienceClass* th, int l) = reinterpret_cast<GGL::ExperienceClass::LevelData * (__thiscall*)(GGL::ExperienceClass*, int)>(0x515270);
GGL::ExperienceClass::LevelData* GGL::ExperienceClass::GetLevel(int lvl)
{
    return xpclass_getlevel(this, lvl);
}

static inline GGL::ExperienceClass* (__thiscall* const xpclassholder_getclass)(GGL::ExperienceClassHolder* th, shok::ExperienceClass cl) = reinterpret_cast<GGL::ExperienceClass * (__thiscall*)(GGL::ExperienceClassHolder*, shok::ExperienceClass)>(0x515299);
GGL::ExperienceClass* GGL::ExperienceClassHolder::GetClass(shok::ExperienceClass cl)
{
    return xpclassholder_getclass(this, cl);
}

void* GGL::ExperienceClass::operator new(size_t s)
{
    return shok::Malloc(s);
}
void GGL::ExperienceClass::operator delete(void* p) {
    shok::Free(p);
}

std::vector<GGL::ExperienceClassHolder::EntityCategoryToExperienceClassData> GGL::ExperienceClassHolder::EntityCategoryToExperienceClass{ {
    {shok::EntityCategory::Bow, shok::ExperienceClass::Bow},
    {shok::EntityCategory::Sword, shok::ExperienceClass::Sword},

    {shok::EntityCategory::Rifle, shok::ExperienceClass::Rifle},
    {shok::EntityCategory::Scout, shok::ExperienceClass::Rifle},
    {shok::EntityCategory::Thief, shok::ExperienceClass::Rifle},

    {shok::EntityCategory::Spear, shok::ExperienceClass::Spear},
    {shok::EntityCategory::CavalryLight, shok::ExperienceClass::LightCavalry},
    {shok::EntityCategory::CavalryHeavy, shok::ExperienceClass::HeavyCavalry},
    {shok::EntityCategory::Cannon, shok::ExperienceClass::Cannon},
} };

static inline void(__thiscall* const xpclassholder_load)(GGL::ExperienceClassHolder* th) = reinterpret_cast<void(__thiscall*)(GGL::ExperienceClassHolder*)>(0x515520);
GGL::ExperienceClassHolder* GGL::ExperienceClassHolder::GlobalObj()
{
    auto* r = reinterpret_cast<ExperienceClassHolder*>(0x868ED4);
    if (!r->Loaded)
        xpclassholder_load(r);
    return r;
}

void GGL::ExperienceClassHolder::LoadExperienceClass(shok::ExperienceClass c)
{
    int id = static_cast<int>(c);
    ExperienceClass* cl = Classes.at(id);
    std::string file{ "Data\\Config\\Experience" };
    file.append(cl->Table.c_str());
    file.append(".xml");
    {
        auto v = cl->Level.SaveVector();
        v.Vector.clear();
    }
    (*BB::CClassFactory::GlobalObj)->LoadObject(cl, file.c_str(), ExperienceClass::SerializationData);
}

shok::ExperienceClass GGL::ExperienceClassHolder::AddExperienceClass(const char* name, shok::EntityCategory cat)
{
    shok::ExperienceClass id = static_cast<shok::ExperienceClass>(Classes.size());
    {
        auto v = Classes.SaveVector();
        ExperienceClass* c = new ExperienceClass{};
        c->Table.assign(name);
        v.Vector.push_back(c);
    }
    LoadExperienceClass(id);

    EntityCategoryToExperienceClass.emplace_back(cat, id);

    return id;
}

void GGL::ExperienceClassHolder::PopExpeienceClass(shok::ExperienceClass c)
{
    if (c == shok::ExperienceClass::Rifle)
        throw std::out_of_range{ "cannot pop vanilla experience class" };
    int id = static_cast<int>(c);
    if (id + 1 != static_cast<int>(Classes.size()))
        throw std::out_of_range{ "somehow the id is wrong" };
    auto v = Classes.SaveVector();
    delete v.Vector[id];
    v.Vector.pop_back();

    EntityCategoryToExperienceClass.erase(std::remove_if(EntityCategoryToExperienceClass.begin(), EntityCategoryToExperienceClass.end(),
        [c](const EntityCategoryToExperienceClassData& d) {
            return d.ExperienceClass == c;
        }), EntityCategoryToExperienceClass.end());
}
