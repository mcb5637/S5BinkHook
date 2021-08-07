#pragma once
#include "s5data.h"

struct shok_BB_CFileStreamEx;
struct shok_BB_CXmlSerializer : shok_object {
	PADDINGI(45); // 43 + a bit of extra space

	static inline constexpr int vtp = 0x77FA2C;
	static inline constexpr int TypeDesc = 0x8311F0;


	void Destroy();
	static shok_BB_CXmlSerializer* Create();
	void Deserialize(shok_BB_CFileStreamEx* f, shok_object* ob);
	void Deserialize(const char* filename, shok_object* ob);

private:
	shok_BB_CXmlSerializer()=default;
};

struct shok_BB_CClassFactory : shok_object {

	static inline constexpr int vtp = 0x77F74C;
	static inline constexpr int TypeDesc = 0x830C80;


	shok_object* CreateObject(unsigned int identifier);
	template<class T> T* CreateObject()
	{
		return shok_DynamicCastFromObject<T>(CreateObject(T::Identifier));
	}
	template<class T> T* LoadObject(const char* filename)
	{
		T* ob = CreateObject<T>();
		shok_BB_CXmlSerializer* s = shok_BB_CXmlSerializer::Create();
		s->Deserialize(filename, ob);
		s->Destroy();
		return ob;
	}


	static inline shok_BB_CClassFactory** const GlobalObj = reinterpret_cast<shok_BB_CClassFactory**>(0x88F044);
};
