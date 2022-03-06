#include "pch.h"
#include "s5classfactory.h"

static inline BB::CXmlSerializer*(__stdcall* const xmlserializer_new)(int d) = reinterpret_cast<BB::CXmlSerializer*(__stdcall* const)(int)>(0x550731);
BB::CXmlSerializer* BB::CXmlSerializer::Create()
{
    return xmlserializer_new(0);
}
void BB::CXmlSerializer::Deserialize(const char* filename, BB::IObject* ob)
{
    BB::CFileStreamEx filestr{};
    if (filestr.OpenFile(filename, 0x113)) {
        Deserialize(&filestr, ob);
        filestr.Close();
    }
}

void BB::CClassFactory::LoadObject(BB::IObject* ob, const char* filename)
{
    BB::CXmlSerializer* s = BB::CXmlSerializer::Create();
    s->Deserialize(filename, ob);
    s->Destroy();
}
