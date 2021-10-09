#include "pch.h"
#include "s5classfactory.h"

struct shok_vtable_BB_CXmlSerializer {
    void(__stdcall* Destroy)(shok_BB_CXmlSerializer* th);
    void(__stdcall* Deserialize)(shok_BB_CXmlSerializer* th, shok_BB_CFileStreamEx* f, shok_BB_IObject* ob); // open file in mode 0x113
    PADDINGI(1);
    void(__stdcall* Serialize)(shok_BB_CXmlSerializer* th, shok_BB_CFileStreamEx* f, shok_BB_IObject* ob); // open file in mode 0x121
    PADDINGI(1); // dtor, use destroy
    void(__stdcall* DeserializeByData)(shok_BB_CXmlSerializer* th, shok_BB_CFileStreamEx* f, shok_BB_IObject* ob, shok_BB_CClassFactory_serializationData* d);
    // serialize by data, (shok_BB_CXmlSerializer* th, shok_BB_CFileStreamEx* f, shok_object* ob, shok_BB_CClassFactory_serializationData* d, char* buff)?
};

struct shok_vtable_BB_CClassFactory {
    PADDINGI(4);
    shok_BB_IObject* (__stdcall* Create)(shok_BB_CClassFactory* th, unsigned int id);
    unsigned int(__stdcall* GetIdentifierByName)(shok_BB_CClassFactory* th, const char*);
    const char*(__stdcall* GetClassDemangledName)(shok_BB_CClassFactory* th, unsigned int id);
    shok_BB_CClassFactory_serializationData* (__stdcall* GetClassSerializationData)(shok_BB_CClassFactory* th, unsigned int id);
    PADDINGI(1);
    int(__stdcall* AddClassData)(shok_BB_CClassFactory* th, unsigned int id, const char* name, shok_BB_IObject* (__stdcall* NewObj)(), shok_BB_CClassFactory_serializationData* data); // 9, data can be nullptr
};
//constexpr int i = offsetof(shok_vtable_BB_CClassFactory, AddClassData) / 4;

static inline shok_BB_CXmlSerializer*(__stdcall* const xmlserializer_new)(int d) = reinterpret_cast<shok_BB_CXmlSerializer*(__stdcall* const)(int)>(0x550731);
shok_BB_CXmlSerializer* shok_BB_CXmlSerializer::Create()
{
    return xmlserializer_new(0);
}
void shok_BB_CXmlSerializer::Destroy()
{
    reinterpret_cast<shok_vtable_BB_CXmlSerializer*>(vtable)->Destroy(this);
}
void shok_BB_CXmlSerializer::Deserialize(shok_BB_CFileStreamEx* f, shok_BB_IObject* ob)
{
    reinterpret_cast<shok_vtable_BB_CXmlSerializer*>(vtable)->Deserialize(this, f, ob);
}
void shok_BB_CXmlSerializer::Deserialize(const char* filename, shok_BB_IObject* ob)
{
    shok_BB_CFileStreamEx filestr{};
    if (filestr.OpenFile(filename, 0x113)) {
        Deserialize(&filestr, ob);
        filestr.Close();
    }
}

shok_BB_IObject* shok_BB_CClassFactory::CreateObject(unsigned int identifier)
{
    shok_vtable_BB_CClassFactory* vt = reinterpret_cast<shok_vtable_BB_CClassFactory*>(vtable);
    //vt->Create2(this, identifier);
    return vt->Create(this, identifier);
}
void shok_BB_CClassFactory::LoadObject(shok_BB_IObject* ob, const char* filename)
{
    shok_BB_CXmlSerializer* s = shok_BB_CXmlSerializer::Create();
    s->Deserialize(filename, ob);
    s->Destroy();
}
shok_BB_CClassFactory_serializationData* shok_BB_CClassFactory::GetSerializationDataForClass(unsigned int identifier)
{
    return reinterpret_cast<shok_vtable_BB_CClassFactory*>(vtable)->GetClassSerializationData(this, identifier);
}
const char* shok_BB_CClassFactory::GetClassDemangledName(unsigned int identifier)
{
    return reinterpret_cast<shok_vtable_BB_CClassFactory*>(vtable)->GetClassDemangledName(this, identifier);
}
unsigned int shok_BB_CClassFactory::GetIdentifierByName(const char* name)
{
    return reinterpret_cast<shok_vtable_BB_CClassFactory*>(vtable)->GetIdentifierByName(this, name);
}
void shok_BB_CClassFactory::AddClassToFactory(unsigned int identifier, const char* name, shok_BB_IObject* (__stdcall* createObj)(), shok_BB_CClassFactory_serializationData* serializationData)
{
    reinterpret_cast<shok_vtable_BB_CClassFactory*>(vtable)->AddClassData(this, identifier, name, createObj, serializationData);
}

static inline const char* (__thiscall* const typedesc_name)(const RTTI_TypeDescriptor* th) = reinterpret_cast<const char* (__thiscall*)(const RTTI_TypeDescriptor*)>(0x5C32F8);
const char* RTTI_TypeDescriptor::name() const
{
    return typedesc_name(this);
}

struct factorytest_s : shok_BB_IObject {
    std::vector<int> data;

    static inline constexpr int Identifier = 0x500;
    static const int TypeDesc;
};
shok_BB_CClassFactory_serializationData_ListOptions_ForVector<int> intvect{};
shok_BB_CClassFactory_serializationData seridata[] = {
    {
        2, "Data", offsetof(factorytest_s, data), sizeof(factorytest_s::data), shok_BB_CClassFactory_serializationData_FieldSerilaizer::TypeInt, nullptr, 0, &intvect,0
    },
    {}
};


int factorytestvt[];
RTTI_TypeDescriptor factorytypedesc{ };
RTTI_BaseClassDescriptor factorybaseclass1{ &factorytypedesc, 1, 0, -1, 0, 0 };
RTTI_BaseClassDescriptor factorybaseclass2{ RTTI_TypeDescriptor::TypeID<shok_BB_IObject>(), 0, 0, -1, 0, 0 };
RTTI_BaseClassDescriptor* factorybaseclasses[] = { &factorybaseclass1 , &factorybaseclass2 };
RTTI_ClassHierarchyDescriptor factoryclasshiera{ 0, 0, 2, factorybaseclasses };
RTTI_CompleteObjectLocator  factoryobjectloc{ 0,0,0, &factorytypedesc , &factoryclasshiera };

const int factorytest_s::TypeDesc = reinterpret_cast<int>(&factorytypedesc);

int __stdcall getfactoryid(int _) {
    return 0x500;
}
int factorytestvt[] = {
    reinterpret_cast<int>(&factoryobjectloc), 0, reinterpret_cast<int>(&getfactoryid), 0x55336A
};

shok_BB_IObject* __stdcall newfactorytest_s() {
    factorytest_s* r = new factorytest_s();
    r->vtable = reinterpret_cast<int>(&(factorytestvt[1]));
    return r;
}

void factorytest()
{
    shok_BB_CClassFactory* f = *shok_BB_CClassFactory::GlobalObj;
    f->AddClassToFactory(0x500, "class GGL::FactoryTest", &newfactorytest_s, seridata);
    factorytest_s* o = shok_DynamicCast<shok_BB_IObject, factorytest_s>((*shok_BB_CClassFactory::GlobalObj)->CreateObject(0x500));
    (*shok_BB_CClassFactory::GlobalObj)->LoadObject(o, "data\\test.xml");
    DEBUGGER_BREAK;
}
