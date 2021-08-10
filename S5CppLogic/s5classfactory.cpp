#include "pch.h"
#include "s5classfactory.h"

struct shok_vtable_BB_CXmlSerializer {
    void(__stdcall* Destroy)(shok_BB_CXmlSerializer* th);
    void(__stdcall* Deserialize)(shok_BB_CXmlSerializer* th, shok_BB_CFileStreamEx* f, shok_object* ob); // open file in mode 0x113
    PADDINGI(1);
    void(__stdcall* Serialize)(shok_BB_CXmlSerializer* th, shok_BB_CFileStreamEx* f, shok_object* ob); // open file in mode 0x121
    PADDINGI(1); // dtor, use destroy
    void(__stdcall* DeserializeByData)(shok_BB_CXmlSerializer* th, shok_BB_CFileStreamEx* f, shok_object* ob, shok_BB_CClassFactory_serializationData* d);
    // serialize by data, (shok_BB_CXmlSerializer* th, shok_BB_CFileStreamEx* f, shok_object* ob, shok_BB_CClassFactory_serializationData* d, char* buff)?
};

struct shok_vtable_BB_CClassFactory {
    PADDINGI(4);
    shok_object* (__stdcall* Create)(shok_BB_CClassFactory* th, unsigned int id);
    PADDINGI(1);
    const char*(__stdcall* GetClassDemangledName)(shok_BB_CClassFactory* th, unsigned int id);
    shok_BB_CClassFactory_serializationData* (__stdcall* GetClassSerializationData)(shok_BB_CClassFactory* th, unsigned int id);
    PADDINGI(1);
    int(__stdcall* AddClassData)(shok_BB_CClassFactory* th, unsigned int id, const char* name, shok_object* (__stdcall* NewObj)(), shok_BB_CClassFactory_serializationData* data); // 9
};
//constexpr int i = offsetof(shok_vtable_BB_CClassFactory, GetClassSerializationData) / 4;

static inline shok_BB_CXmlSerializer*(__stdcall* const xmlserializer_new)(int d) = reinterpret_cast<shok_BB_CXmlSerializer*(__stdcall* const)(int)>(0x550731);
shok_BB_CXmlSerializer* shok_BB_CXmlSerializer::Create()
{
    return xmlserializer_new(0);
}
void shok_BB_CXmlSerializer::Destroy()
{
    reinterpret_cast<shok_vtable_BB_CXmlSerializer*>(vtable)->Destroy(this);
}
void shok_BB_CXmlSerializer::Deserialize(shok_BB_CFileStreamEx* f, shok_object* ob)
{
    reinterpret_cast<shok_vtable_BB_CXmlSerializer*>(vtable)->Deserialize(this, f, ob);
}
void shok_BB_CXmlSerializer::Deserialize(const char* filename, shok_object* ob)
{
    shok_BB_CFileStreamEx filestr{};
    if (filestr.OpenFile(filename, 0x113)) {
        Deserialize(&filestr, ob);
        filestr.Close();
    }
}

shok_object* shok_BB_CClassFactory::CreateObject(unsigned int identifier)
{
    shok_vtable_BB_CClassFactory* vt = reinterpret_cast<shok_vtable_BB_CClassFactory*>(vtable);
    //vt->Create2(this, identifier);
    return vt->Create(this, identifier);
}
void shok_BB_CClassFactory::LoadObject(shok_object* ob, const char* filename)
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
void shok_BB_CClassFactory::AddClassToFactory(unsigned int identifier, const char* name, shok_object* (__stdcall* createObj)(), shok_BB_CClassFactory_serializationData* serializationData)
{
    reinterpret_cast<shok_vtable_BB_CClassFactory*>(vtable)->AddClassData(this, identifier, name, createObj, serializationData);
}



shok_BB_CClassFactory_serializationData seridata[] = {
    {
        2, "Data", 4, 4, shok_BB_CClassFactory_serializationData_FieldSerilaizer::TypeInt, nullptr, 0,0,0
    },
    {}
};


struct factorytest_s : shok_object {
    int data;

    static inline constexpr int Identifier = 0x500;
    static const int TypeDesc;
};
int factorytestvt[];
RTTI_TypeDescriptor factorytypedesc{ };
RTTI_BaseClassDescriptor factorybaseclass1{ &factorytypedesc, 1, 0, -1, 0, 0 };
RTTI_BaseClassDescriptor factorybaseclass2{ reinterpret_cast<RTTI_TypeDescriptor*>(0x7FFE08), 0, 0, -1, 0, 0 };
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

shok_object* __stdcall newfactorytest_s() {
    factorytest_s* r = new factorytest_s();
    r->vtable = reinterpret_cast<int>(&(factorytestvt[1]));
    return r;
}

void factorytest()
{
    shok_BB_CClassFactory* f = *shok_BB_CClassFactory::GlobalObj;
    f->AddClassToFactory(0x500, "class GGL::FactoryTest", &newfactorytest_s, seridata);
    factorytest_s* o = shok_DynamicCastFromObject<factorytest_s>((*shok_BB_CClassFactory::GlobalObj)->CreateObject(0x500));
    (*shok_BB_CClassFactory::GlobalObj)->LoadObject(o, "data\\test.xml");
    DEBUGGER_BREAK;
}
