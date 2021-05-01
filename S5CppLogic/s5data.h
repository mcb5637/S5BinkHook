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

#define SHOK_Import_LUA_OPEN 0x761284
#define SHOK_SEGMENTSTART 0x401000
#define SHOK_SEGMENTLENGTH 0x64B000

typedef uint8_t byte;

// list of code changes in exe
// 0x761284 redirected (lua open dll import)
// EGL_CGLEGameLogic.CreateEffect vtable redirect
// GGL_CCannonBallEffect & GGL_CArrowEffect . OnHit vtable redirect
// GGUI_CManager.PostEvent vtable redirect
// convert task 0x4FCD27 call redirect
// 0x005011DF reset camo func override
// 0x4D51A4 & 0x50163A activate camo call redirect
// 
// only without SCELoader
// shok_entityHurtEntity 0x49F358 jmp patched
// entity get max hp 0x57B798 jmp
// 0x4BDED8 ui settler overhead hp bar maxhp jmp

// allocator
static inline void* (__cdecl* shok_malloc)(size_t t) = (void* (__cdecl*)(size_t)) 0x5C4181;
static inline void* (__cdecl* shok_new)(size_t t) = (void* (__cdecl*)(size_t)) 0x5C04FB;
static inline void(__cdecl* shok_free)(void* p) = (void(__cdecl*)(void* p)) 0x5C2E2D;
static inline void (*shok_logString)(const char* format, ...) = (void (*)(const char* format, ...)) 0x548268;

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
struct shok_set {
	PADDINGI(1)
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

typedef std::basic_string<char, std::char_traits<char>, shok_allocator<char>> shok_string;

#ifdef _DEBUG
static_assert(sizeof(shok_string) == 7 * 4);
#elif
static_assert(sizeof(shok_string) == 6 * 4);
#endif

template<class T>
inline void shok_saveVector(std::vector<T, shok_allocator<T>>* vec, std::function<void(std::vector<T, shok_allocator<T>> &s)> func) {
#ifdef _DEBUG
	std::vector<T, shok_allocator<T>> save = std::vector<T, shok_allocator<T>>();
	int* vecPoint = (int*)vec;
	int* savePoint = (int*)&save;
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

struct shok_costInfo {
private:
	int u;
public:
	float Gold, GoldRaw, Silver, SilverRaw, Stone, StoneRaw, Iron, IronRaw, Sulfur, SulfurRaw, Clay, ClayRaw, Wood, WoodRaw, WeatherEnergy, Knowledge, Faith;

	float GetResourceAmountFromType(int ty, bool addRaw);
	void AddToType(int ty, float toadd);
	void SubFromType(int ty, float tosub);
	bool HasResources(shok_costInfo* has);
};

struct shok_object {
public:
	void* vtable;
};

struct shok_attachment {
	int AttachmentType, EntityId;
};
bool operator<(shok_attachment a, shok_attachment b);

struct shok_event_data {

};

template<class T>
bool contains(T* data, T search, int num) {
	for (int i = 0; i < num; i++)
		if (data[i] == search)
			return true;
	return false;
}

#include "s5behaviorProps.h"
#include "s5entitytype.h"
#include "s5behaviors.h"
#include "s5entity.h"
#include "s5defines.h"
#include "s5events.h"
#include "s5effects.h"
#include "s5player.h"
#include "s5tech.h"
#include "s5globalObjects.h"
#include "s5widget.h"

// xml loader vars:
// v2 type?
// v3 xml element
// v4 offset
// v5 size


static inline void(_stdcall* shok_SetHighPrecFPU)() = (void(_stdcall*)()) 0x5C8451;

static inline int(__stdcall* shok_loadBuffer)(lua_State* L, const char* buff, size_t bufflen, const char* name) = (int(__stdcall*)(lua_State*, const char*, size_t, const char*)) 0x59BE57;


static inline lua_State** shok_luastate_game = (lua_State**)0x853A9C;


const char* ReadFileToString(const char* name, size_t* size);
bool DoesFileExist(const char* name);

void RedirectCall(void* call, void* redirect);
void WriteJump(void* adr, void* toJump);

bool HasSCELoader();

template<class T>
constexpr inline T rad2deg(T r) {
	return r * 180 / M_PI;
}
template<class T>
constexpr inline T deg2rad(T d) {
	return d * M_PI / 180;
}
