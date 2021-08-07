#include "pch.h"
#include "s5classfactory.h"

struct shok_vtable_BB_CXmlSerializer {
    void(__stdcall* Destroy)(shok_BB_CXmlSerializer* th);
    void(__stdcall* Deserialize)(shok_BB_CXmlSerializer* th, shok_BB_CFileStreamEx* f, shok_object* ob);
};

struct shok_vtable_BB_CClassFactory {
    PADDINGI(4);
    shok_object* (__stdcall* Create1)(shok_BB_CClassFactory* th, unsigned int id);
    PADDINGI(1);
    int(__stdcall* Create2)(shok_BB_CClassFactory* th, unsigned int id);
    PADDINGI(2);
    int(__stdcall* AddClassData)(shok_BB_CClassFactory* th, unsigned int id, const char* name, shok_object* (__stdcall* NewObj)(), int data);
};
//constexpr int i = offsetof(shok_vtable_BB_CClassFactory, AddLoadData) / 4;

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
    vt->Create2(this, identifier);
    return vt->Create1(this, identifier);
}
