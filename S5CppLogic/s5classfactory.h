#pragma once
#include "s5data.h"

struct shok_BB_CFileStreamEx;
struct shok_BB_CXmlSerializer : shok_object {
	PADDINGI(45); // 43 + a bit of extra space

	static inline constexpr int vtp = 0x77FA2C;
	static inline constexpr int TypeDesc = 0x8311F0;


	void Destroy();
	static shok_BB_CXmlSerializer* Create();
	void Deserialize(shok_BB_CFileStreamEx* f, shok_BB_IObject* ob);
	void Deserialize(const char* filename, shok_BB_IObject* ob);

private:
	shok_BB_CXmlSerializer() = default;
};

struct shok_BB_CClassFactory_serializationData_FieldSerilaizer {
	int(__stdcall* DeserializeFromString)(void* data, const char* buff);
	int(__stdcall* SerializeToString)(char* buff, size_t s, void* data);
	// probably the same for binary
	// bool
	// another func

	static inline shok_BB_CClassFactory_serializationData_FieldSerilaizer* const TypeInt = reinterpret_cast<shok_BB_CClassFactory_serializationData_FieldSerilaizer*>(0x810C98);
	static inline shok_BB_CClassFactory_serializationData_FieldSerilaizer* const TypeFloat = reinterpret_cast<shok_BB_CClassFactory_serializationData_FieldSerilaizer*>(0x810C78);
	static inline shok_BB_CClassFactory_serializationData_FieldSerilaizer* const TypeBool = reinterpret_cast<shok_BB_CClassFactory_serializationData_FieldSerilaizer*>(0x810C58);
	static inline shok_BB_CClassFactory_serializationData_FieldSerilaizer* const TypeClassIdentifier = reinterpret_cast<shok_BB_CClassFactory_serializationData_FieldSerilaizer*>(0x813978);
	static inline shok_BB_CClassFactory_serializationData_FieldSerilaizer* const TypeTaskList = reinterpret_cast<shok_BB_CClassFactory_serializationData_FieldSerilaizer*>(0x85D4AC);
	// does not add ids
	static inline shok_BB_CClassFactory_serializationData_FieldSerilaizer* const TypeEntityType = reinterpret_cast<shok_BB_CClassFactory_serializationData_FieldSerilaizer*>(0x85D4D0);
	static inline shok_BB_CClassFactory_serializationData_FieldSerilaizer* const TypeModel = reinterpret_cast<shok_BB_CClassFactory_serializationData_FieldSerilaizer*>(0x8585C0);

};
struct shok_BB_CClassFactory_serializationData_ListOptions {
	struct iter {
		void* List;
		unsigned int data; // usually a pointer
		bool first = true;
	};

	void* (__stdcall* AddToList)(void* List) = nullptr; // gets list, allocates, then returns p to new object
	int z = 0;
	iter* (__stdcall* AllocIter)(void* List) = nullptr;
	bool(__stdcall* IterNext)(iter* i) = nullptr;
	void* (__stdcall* IterCurrent)(iter* i) = nullptr;
	void(__stdcall* FreeIter)(iter* i) = nullptr;
	int nullsub = 0x52B509;
	size_t(__stdcall* GetSize)(void* List) = nullptr;
};
template<class T>
struct shok_BB_CClassFactory_serializationData_ListOptions_ForVector : shok_BB_CClassFactory_serializationData_ListOptions { // do not use with shok::Allocator vector!

	shok_BB_CClassFactory_serializationData_ListOptions_ForVector() {
		AddToList = [](void* l) {
			std::vector<T>* v = static_cast<std::vector<T>*>(l);
			v->push_back(T{});
			return static_cast<void*>(v->data() + (v->size() - 1));
		};
		AllocIter = [](void* l) {
			return new iter{ l, 0, true };
		};
		IterNext = [](iter* i) {
			std::vector<T>* v = static_cast<std::vector<T>*>(i->List);
			if (i->first) {
				i->first = false;
				i->data = 0;
			}
			else {
				i->data++;
			}
			return i->data < v->size();
		};
		IterCurrent = [](iter* i) {
			std::vector<T>* v = static_cast<std::vector<T>*>(i->List);
			return static_cast<void*>(v->data() + i->data);
		};
		FreeIter = [](iter* i) {
			delete i;
		};
		GetSize = [](void* l) {
			std::vector<T>* v = static_cast<std::vector<T>*>(l);;
			return v->size();
		};
	}
};
struct shok_BB_CClassFactory_serializationData { // use a 0-terminated array (default constructed is 0)
	int Type = 0; // 2 direct val, 3 embedded
	const char* SerializationName = nullptr;
	int Position = 0;
	int Size = 0;
	shok_BB_CClassFactory_serializationData_FieldSerilaizer* DataConverter = 0;
	shok_BB_CClassFactory_serializationData* SubElementData = nullptr; //5
	int Unknown2 = 0;
	shok_BB_CClassFactory_serializationData_ListOptions* ListOptions = nullptr;
	int Unknown3 = 0;


};
static_assert(sizeof(shok_BB_CClassFactory_serializationData) == 4 * 9);

struct shok_BB_CClassFactory : shok_object {

	static inline constexpr int vtp = 0x77F74C;
	static inline constexpr int TypeDesc = 0x830C80;


	shok_BB_IObject* CreateObject(unsigned int identifier);
	template<class T> T* CreateObject()
	{
		return shok_DynamicCast<shok_BB_IObject, T>(CreateObject(T::Identifier));
	}
	void LoadObject(shok_BB_IObject* ob, const char* filename);
	template<class T> T* LoadObject(const char* filename)
	{
		T* ob = CreateObject<T>();
		if (!ob)
			return nullptr;
		LoadObject(ob, filename);
		return ob;
	}
	shok_BB_CClassFactory_serializationData* GetSerializationDataForClass(unsigned int identifier); // returns a 0-terminated array
	template<class T> shok_BB_CClassFactory_serializationData* GetSerializationDataForClass()
	{
		return GetSerializationDataForClass(T::Identifier);
	}
	const char* GetClassDemangledName(unsigned int identifier);
	unsigned int GetIdentifierByName(const char* name);
	void AddClassToFactory(unsigned int identifier, const char* name, shok_BB_IObject* (__stdcall* createObj)(), shok_BB_CClassFactory_serializationData* serializationData);


	static inline shok_BB_CClassFactory** const GlobalObj = reinterpret_cast<shok_BB_CClassFactory**>(0x88F044);
};

// to create a dynamic_cast able type, provide a RTTI typedesc and a vtable for it.
// remember to set vtable[-1] to &RTTI_CompleteObjectLocator
// and set up all base classes as RTTI_BaseClassDescriptor
// look at existing classes for what values you do need to put into the RTTI fields
// look at factorytest_s in s5classfactory.cpp for an easy example
struct RTTI_TypeDescriptor {
	int typedesc_vtable = 0x0788E9C;
	int runtime_reference = 0;
	char MangledTypeName[25] = ".?AVFakeClass@CppLogic@@";

	const char* name() const;
	static inline const RTTI_TypeDescriptor* (__cdecl* const __RTTypeid)(const void* ob) = reinterpret_cast<const RTTI_TypeDescriptor* (__cdecl*)(const void*)>(0x5C339A);
	template<class T>
	static const RTTI_TypeDescriptor* TypeID() {
		return reinterpret_cast<RTTI_TypeDescriptor*>(T::TypeDesc);
	}
};
struct RTTI_BaseClassDescriptor {
	const RTTI_TypeDescriptor* TypeDesc;
	int NumOfSubElementsInBaseClassArray;
	int MemberDisplacement = 0;
	int vtableDisplacement = 0;
	int DisplacementWithinVtable = 0;
	int BaseClassAttributes = 0;
};
struct RTTI_ClassHierarchyDescriptor {
	unsigned int Signature = 0;
	int Attributes;
	int NumberOfBaseClassEntries;
	RTTI_BaseClassDescriptor** BaseClassArray;
};
struct RTTI_CompleteObjectLocator {
	unsigned int Signature = 0;
	unsigned int Offset = 0;
	unsigned int ConstructorDisplaycementOffset = 0;
	RTTI_TypeDescriptor* TypeDesc;
	RTTI_ClassHierarchyDescriptor* ClassHierarchyDesc;
};


void factorytest();
