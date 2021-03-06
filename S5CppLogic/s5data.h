#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <functional>
#include "luaimport.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define PADDING(size) private: char TOKENPASTE2(padding_, __LINE__) [size]; public:
#define PADDINGI(size) PADDING(size*4)

#ifdef _DEBUG
#define vector_padding public:
static_assert(sizeof(std::vector<int>) == 16);
static_assert(sizeof(std::list<int>) == 12);
#else
#define vector_padding PADDING(4) public:
static_assert(sizeof(std::vector<int>) == 12);
static_assert(sizeof(std::list<int>) == 8);
#endif

#define DEBUGGER_BREAK _asm int 3

constexpr int SHOK_Import_LUA_OPEN = 0x761284;
constexpr int SHOK_Import_LUA_CLOSE = 0x76126C;
constexpr int SHOK_SEGMENTSTART = 0x401000;
constexpr int SHOK_SEGMENTLENGTH = 0x64B000;

typedef uint8_t byte;

// list of code changes in exe
// 0x761284 redirected (lua open dll import)
// EGL_CGLEGameLogic.CreateEffect vtable redirect
// GGL_CCannonBallEffect & GGL_CArrowEffect . OnHit vtable redirect
// cannonball effect from creator 0x4FF942 jmp
// GGUI_CManager.PostEvent vtable redirect
// convert task 0x4FCD26 call redirect
// 0x5011DF reset camo func override
// 0x4D51A4 & 0x50163A activate camo call redirect
// 0x49A6A7 settlerchangeplayer override func to fix it not returning new id
// 0x4B8EAD constructionsite_getprogresspertick func override
// string render 0x557E47 to 0x557DAA jmp, string render getlen 0x708F60 jmp
// entity execute task get task handler 0x57BF7A call redirect
// destroy entity 0x57C94A jmp
// snipe task 0x4DB5B8 jmp (func override)
// 0x529067 feedback event handler res mined show floatie
// 
// only without SCELoader
// shok_EGL_CGLEEntity::EntityHurtEntity 0x49F358 jmp patched, afew instructions noped
// entity get max hp 0x57B798 jmp, settler overhead hp bar 0x4BDED8 jmp, createentity 0x571B93 jmp set hp
// create entity 0x571B93 jmp to fix max hp
// 0x4BDED8 ui settler overhead hp bar maxhp jmp
// shok_playerattractionhandler_checkpayday 0x4C25FB jmp at 0x4C2754
// entity get damage battle event 0x50C785 jmp, autocannon event 0x50F5ED jmp, battle singletargetcalcdmg 0x50C235 jmp, battle proj 0x50C3E7 jmp, autocannon proj 0x51076C jmp
// entity get armor event settler 0x4A6B15 jmp, building 0x4AB160 jmp
// entity get exploration settler 0x4A4AC3 jmp, building 0x4AB199 jmp
// leader behavior regen 0x4EAE92 jmp, leader behavior ontick? 0x4EFC29 jmp
// battle behavior get max range 0x50AB48 jmp, autocannon behavior get max range 0x50F50D jmp
// GetStringTableText 0x556D2E jmp
// display entity name overhead 0x53F911 jmp
// check building placement 0x4B45B9 jmp
// place building create net event 0x538FF4 jmp
// place building preview pos check 0x5389FB jmp
// place building set model 0x538C8D jmp
// win main char event 0x40754C call redirect
// win main key event 0x40757D call redirect
// win main mouse event check 0x40747E call redirect
// activate ranged effect ability heal 0x4E3C78 call redirect

// allocator
static inline void* (__cdecl* const shok_malloc)(size_t t) = reinterpret_cast<void* (__cdecl*)(size_t)>(0x5C4181);
static inline void* (__cdecl* const shok_new)(size_t t) = reinterpret_cast<void* (__cdecl*)(size_t)>(0x5C04FB);
static inline void(__cdecl* const shok_free)(void* p) = reinterpret_cast<void(__cdecl*)(void* p)>(0x5C2E2D);
static inline void (*const shok_logString)(const char* format, ...) = reinterpret_cast<void (*)(const char* format, ...)>(0x548268);

template <class T>
struct shok_allocator {
	typedef T value_type;
	shok_allocator() = default;
	template <class U> constexpr shok_allocator(const shok_allocator <U>&) noexcept {}

	[[nodiscard]] T* allocate(size_t n) noexcept
	{
		void* p = shok_malloc(n * sizeof(T));
		return (T*)p;
	}
	void deallocate(T* p, size_t n) noexcept
	{
		shok_free(p);
	}
};
template <class T, class U>
bool operator==(const shok_allocator <T>&, const shok_allocator <U>&) { return true; }
template <class T, class U>
bool operator!=(const shok_allocator <T>&, const shok_allocator <U>&) { return false; }

template <class T>
struct shok_treeNode {
	shok_treeNode<T>* left, * parent, * right;
	T data;
	bool redblack;
};
template <class T>
struct shok_set { // todo iterators
	PADDINGI(1);
	shok_treeNode<T>* root;
	int size;

private:
	void ForAllRec(shok_treeNode<T>* d, std::function<void(T*)> lambda) {
		if (d == root)
			return;
		ForAllRec(d->left, lambda);
		lambda(&d->data);
		ForAllRec(d->right, lambda);
	}
public:
	void ForAll(std::function<void(T*)> lambda) {
		ForAllRec(root->parent, lambda);
	}
	T* GetFirstMatch(std::function<bool(T*)> lambda) {
		T* r = nullptr;
		ForAll([&r, lambda](T* d) {
			if (r == nullptr && lambda(d))
				r = d;
			});
		return r;
	}
};

struct shok_string {
private:
	int u1 = 0;
	int data;
	int u[4] = { 0 };
	int size = 0;

public:
	shok_string(const char* s);
	shok_string(const shok_string& c);
	void assign(const char* s);
	const char* c_str();
	~shok_string();
	shok_string();
};

static_assert(sizeof(shok_string) == 7 * 4);

template<class T>
inline void shok_saveVector(std::vector<T, shok_allocator<T>>* vec, std::function<void(std::vector<T, shok_allocator<T>> &s)> func) {
#ifdef _DEBUG
	std::vector<T, shok_allocator<T>> save{};
	int* vecPoint = reinterpret_cast<int*>(vec);
	int* savePoint = reinterpret_cast<int*>(&save);
	int backu[3] = {};
	for (int i = 1; i < 4; i++) {
		backu[i - 1] = savePoint[i];
		savePoint[i] = vecPoint[i];
	}
	func(save);
	for (int i = 1; i < 4; i++) {
		vecPoint[i] = savePoint[i];
		savePoint[i] = backu[i - 1];
	}
#else
	func(*vec);
#endif
}
template<class T>
inline void shok_saveList(std::list<T, shok_allocator<T>>* vec, std::function<void(std::list<T, shok_allocator<T>>& s)> func) {
#ifdef _DEBUG
	std::list<T, shok_allocator<T>> save{};
	int* vecPoint = reinterpret_cast<int*>(vec);
	int* savePoint = reinterpret_cast<int*>(&save);
	int backu[2] = {};
	for (int i = 1; i < 3; i++) {
		backu[i - 1] = savePoint[i];
		savePoint[i] = vecPoint[i];
	}
	func(save);
	for (int i = 1; i < 3; i++) {
		vecPoint[i] = savePoint[i];
		savePoint[i] = backu[i - 1];
	}
#else
	func(*vec);
#endif
}

// allows read/write/execute of the memory location pointed to until it goes out of scope.
// using more than one at the same time works as expected, cause the destructors are called in reverse order.
// use always as stack variable!
struct shok_saveVirtualProtect {
	shok_saveVirtualProtect();
	shok_saveVirtualProtect(void* adr, size_t size);
	~shok_saveVirtualProtect();
private:
	void* Adr;
	size_t Size;
	unsigned long Prev;
};

// casts shok objects with runtime type chek. you can only cast objects that have a vtable (does not have to be known) and a known RTTI TypeDesc set in the class.
// works almost the same way as dynamic_cast, just you can only cast pointers and you have to specify both current type and target type (without pointer).
// example:
// shok_EGL_CGLEEntity* e = ...;
// shok_GGL_CSettler* sett = shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e);
// if (sett) { ...
template<class CastFrom, class CastTo>
inline CastTo* shok_DynamicCast(CastFrom* i) {
	void* (__cdecl* const shok_dyncastFunc)(void* i, int off, int TypeDescSource, int TypeDescOut, bool isref) = reinterpret_cast<void* (__cdecl* const)(void*, int, int, int, bool)>(0x5C36EE);
	return static_cast<CastTo*>(shok_dyncastFunc(i, 0, CastFrom::TypeDesc, CastTo::TypeDesc, false));
}

// generic structs
struct shok_position {
	float X;
	float Y;

	void FloorToBuildingPlacement();
	float GetDistanceSquaredTo(shok_position& p);
	bool IsInRange(shok_position& p, float range);
	// returns deg
	float GetAngleBetween(shok_position& p);
};

struct shok_positionRot : shok_position {
	float r;
};

struct shok_AARect {
	shok_position low, high;
};

enum class shok_ResourceType;
struct shok_costInfo { // size 18
	PADDINGI(1);
	float Gold, GoldRaw, Silver, SilverRaw, Stone, StoneRaw, Iron, IronRaw, Sulfur, SulfurRaw, Clay, ClayRaw, Wood, WoodRaw, WeatherEnergy, Knowledge, Faith;

	float GetResourceAmountFromType(shok_ResourceType ty, bool addRaw);
	void AddToType(shok_ResourceType ty, float toadd);
	void SubFromType(shok_ResourceType ty, float tosub);
	bool HasResources(shok_costInfo* has);
};
static_assert(sizeof(shok_costInfo) == 18 * 4);

struct shok_object {
	int vtable;
};

enum class shok_AttachmentType;
struct shok_attachment {
	shok_AttachmentType AttachmentType;
	int EntityId, EventID; // does event id exist, or is it just in attachmentinfo loader?
};
bool operator<(shok_attachment a, shok_attachment b);

struct shok_BB_CEvent;
enum class shok_EventIDs : int;
enum class shok_NetEventIds : int;
struct shok_EGL_CGLETaskArgs;


template<class T>
bool contains(T* data, T search, int num) {
	for (int i = 0; i < num; i++)
		if (data[i] == search)
			return true;
	return false;
}

const char* (__cdecl* const shok_GetStringTableText)(const char* key) = reinterpret_cast<const char* (__cdecl* const)(const char*)>(0x556D2E);


enum class win_mouseEvents : int {
	MouseMove = 0x200,
	LButtonDown = 0x201,
	LButtonUp = 0x202,
	LButtonDBl = 0x203,
	RButtonDown = 0x204,
	RButtonUp = 0x205,
	RButtonDBl = 0x206,
	MButtonDown = 0x207,
	MButtonUp = 0x208,
	MButtonDBl = 0x209,
	MouseWheel = 0x20A,
	XButtonDown = 0x20B,
	XButtonUp = 0x20C,
	XButtonDBl = 0x20D,
};


#include "s5defines.h"
#include "s5behaviorProps.h"
#include "s5entitytype.h"
#include "s5behaviors.h"
#include "s5entity.h"
#include "s5events.h"
#include "s5effects.h"
#include "s5player.h"
#include "s5tech.h"
#include "s5globalObjects.h"
#include "s5widget.h"
#include "s5framework.h"
#include "s5tasklist.h"

// xml loader vars:
// v2 type?
// v3 xml element
// v4 offset
// v5 size


static inline void(_stdcall* const shok_SetHighPrecFPU)() = reinterpret_cast<void(_stdcall*)()>(0x5C8451);

static inline int(__stdcall* const shok_loadBuffer)(lua_State* L, const char* buff, size_t bufflen, const char* name) = reinterpret_cast<int(__stdcall*)(lua_State*, const char*, size_t, const char*)>(0x59BE57);


static inline lua_State** const shok_luastate_game = reinterpret_cast<lua_State**>(0x853A9C);
extern lua_State* mainmenu_state;


const char* ReadFileToString(const char* name, size_t* size);
bool DoesFileExist(const char* name);

void RedirectCall(void* call, void* redirect);
void WriteJump(void* adr, void* toJump);

bool HasSCELoader();

extern const char* (*GetStringTableTextOverride)(const char* s);
void HookGetStringTableText();

void HookTextPrinting();

template<class T>
constexpr inline T rad2deg(T r) {
	return static_cast<T>(static_cast<double>(r) * 180.0 / M_PI);
}
template<class T>
constexpr inline T deg2rad(T d) {
	return static_cast<T>(static_cast<double>(d) * M_PI / 180);
}
