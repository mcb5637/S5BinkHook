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
#define PADDING(size) private: char TOKENPASTE2(padding_, __LINE__) [size]={}; public:
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
// 0x57D6CA entity ctor jmp to add TASK_LUA_FUNC handlers
// 0x588408 behavioranim_addhandlers jmp to add TASK_WAIT_FOR_ANIM_NON_CANCELABLE handlers
// 0x4ADB16 on building init blocking check jmp to fix move tasks (seems not to conflict with CUtil.ActivateTaskIncrementBugfix)
// 0x496677 start music jmp
// AIL_closestream 0x7614A4 dllimport redirect
// 0x57B223 entity settasklist jmp
// 0x55A62A button check shortcut sign extension opcode fixed OR
// 0x55A597 jmp button check shortcut logic override
// 
// only without SCELoader
// EGL::CGLEEntity::EntityHurtEntity 0x49F358 jmp patched, func override
// EntityDealAoEDamage 0x49F82A func override
// 0x5113C2 arrow onhit jmp
// 0x4DBA20 projectile creator ctor more zeroing out
// 0x511634 projectile from creator copy more data
// 0x50CA59 melee hit jmp
// 0x4FE722 circular attack jmp
// 0x4FF4EB cannonball onhit jmp
// 0x506B28 bomb explode jmp
// 0x4F1E77 keg explode damage jmp
// 0x4DC6D9 throw shuriken projecile jmp
// 0x5062C6 place bomb jmp
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
// place building pos check 0x5389FB jmp
// place building set model 0x538B01 jmp
// place building calc preview pos 0x538BDB jmp
// place building update model 0x538C8D call redirct
// window message handler 0x40744B jmp
// activate ranged effect ability heal 0x4E3C78 call redirect
// ED_CLandscape vtable 0x76A410 get water height func override (hires bridge area)
// waterregion unknown func 0x47D301 jmp (hires bridge area)
// bridgeentity apply bridgeheight 0x503C50 jmp redefined (hires bridge area)
// EGL::CGLETerrainLowRes::BridgeHeights vector modified (hires bridge area)

#include "s5_forwardDecls.h"
#include "s5_mem.h"
#include "s5_datastructures.h"

namespace shok {

	// format i int, f float, x hex int, c char, s const char*
	static inline void (*const LogString)(const char* format, ...) = reinterpret_cast<void (*)(const char* format, ...)>(0x548268);




	// generic structs
	struct Position {
		float X;
		float Y;

		void FloorToBuildingPlacement();
		void RoundToBuildingPlacement();
		float GetDistanceSquaredTo(const Position& p) const;
		bool IsInRange(const Position& p, float range) const;
		// returns deg, this is a hok specific angle, useful for a look at by settlers
		float GetAngleBetween(const Position& p) const;
		// requires rad
		Position Rotate(float r) const;
		// requires rad
		Position RotateAround(float r, const Position& center) const;

		Position operator+(const Position& other) const;
		Position& operator+=(const Position& other);
		Position operator-(const Position& other) const;
		Position& operator-=(const Position& other);
		Position operator*(float f) const;
		Position& operator*=(float f);
		float Dot(const Position& o) const;
		auto operator<=>(const Position& o) const = default;

		static shok_BB_CClassFactory_serializationData* SerializationData;
	};
	struct PositionRot : Position {
		float r;
	};


	struct AARect {
		shok::Position low, high;
		auto operator<=>(const AARect& o) const = default;
		AARect operator+(const AARect& other) const;
		AARect& operator+=(const AARect& other);
		AARect operator-(const AARect& other) const;
		AARect& operator-=(const AARect& other);

		// requires rad
		AARect Rotate(float r) const;
		AARect Sort() const;

		// uses Blocked1 and Blocked2 for the 2 members
		static shok_BB_CClassFactory_serializationData* SerializationData;
	};

	struct CostInfo { // size 18
		PADDINGI(1);
		float Gold = 0, GoldRaw = 0;
		float Silver = 0, SilverRaw = 0;
		float Stone = 0, StoneRaw = 0;
		float Iron = 0, IronRaw = 0;
		float Sulfur = 0, SulfurRaw = 0;
		float Clay = 0, ClayRaw = 0;
		float Wood = 0, WoodRaw = 0;
		float WeatherEnergy = 0, Knowledge = 0, Faith = 0;

		float GetResourceAmountFromType(shok::ResourceType ty, bool addRaw) const;
		void AddToType(shok::ResourceType ty, float toadd);
		void SubFromType(shok::ResourceType ty, float tosub);
		bool HasResources(const CostInfo* has) const;

		static inline shok_BB_CClassFactory_serializationData* SerializationData = reinterpret_cast<shok_BB_CClassFactory_serializationData*>(0x85D668);
	};
	static_assert(sizeof(CostInfo) == 18 * 4);


	struct Attachment {
		shok::AttachmentType AttachmentType;
		int EntityId, EventID; // does event id exist, or is it just in attachmentinfo loader?

		auto operator<=>(const Attachment& o) const = default;
	};
}

namespace BB {
	class IObject {
	public:
		virtual ~IObject() = default;
		virtual unsigned int __stdcall GetClassIdentifier() const = 0;
		virtual void* __stdcall CastToIdentifier(unsigned int id) = 0;

		static constexpr int TypeDesc = 0x7FFE08;
		static constexpr int vtp = 0x7620F0;
	};

	class IPostEvent {
	public:
		virtual void __stdcall PostEvent(BB::CEvent* ev) = 0;
	};
}

namespace ECore {
	class ICheckData {
	public:
		virtual ~ICheckData() = default;
	private:
		virtual bool unknownFunc1(int) = 0;
	};
}

namespace EGL {
	class CGLEAttachableBase {
	public:
		virtual ~CGLEAttachableBase() = default;
		// 8 more funcs

		shok::Set<shok::Attachment> ObserverEntities; // 8
		shok::Set<shok::Attachment> ObserverEffects; // 11
		shok::Set<shok::Attachment> ObservedEntities; // 14
		shok::Set<shok::Attachment> ObservedEffects; // 17
		byte SendEvent; // 20
		PADDING(3);

		static constexpr int vtp = 0x783D18;
		static constexpr int TypeDesc = 0x810B98;
	};
	class CEntityAttachmentProxy {
	};
	class CEffectAttachmentProxy {
	};
	template<class T, class V>
	class TGLEAttachable : public CGLEAttachableBase {

	};


	template<class T, class V>
	class IGLEHandler {
	public:
		virtual void Handle(T* t) = 0;
	};
	template<int HandlerID, class EventBase, class HandlerEvent, class T, class V>
	class THandler : public IGLEHandler<EventBase, V> {
	public:
		typedef void (T::*HandlerType)(HandlerEvent* ev);

		T* Object;
		HandlerType HandlerFunc;

		virtual void Handle(EventBase* ev) override {
			std::invoke(HandlerFunc, Object, static_cast<HandlerEvent*>(ev));
		}
	};

	using EventHandler = EGL::IGLEHandler<BB::CEvent, void>;


	class ISlot {
	public:
		static inline constexpr int vtp = 0x7738CC;
		virtual ~ISlot() = default;
	};
	template<class T, unsigned int id>
	class TSlot : public ISlot {

	};
}

template<class T>
inline void shok_saveVector(std::vector<T, shok::Allocator<T>>* vec, std::function<void(std::vector<T, shok::Allocator<T>> &s)> func) {
#ifdef _DEBUG
	std::vector<T, shok::Allocator<T>> save{};
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
inline void shok_saveList(std::list<T, shok::Allocator<T>>* vec, std::function<void(std::list<T, shok::Allocator<T>>& s)> func) {
#ifdef _DEBUG
	std::list<T, shok::Allocator<T>> save{};
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

// casts shok objects with runtime type chek. you can only cast objects that have a vtable (does not have to be known) and a known RTTI TypeDesc set in the class.
// works almost the same way as dynamic_cast, just you can only cast pointers and you have to specify both current type and target type (without pointer).
// example:
// EGL::CGLEEntity* e = ...;
// GGL::CSettler* sett = shok_DynamicCast<EGL::CGLEEntity, GGL::CSettler>(e);
// if (sett) { ...
template<class CastFrom, class CastTo>
inline CastTo* shok_DynamicCast(CastFrom* i) {
	void* (__cdecl* const shok_dyncastFunc)(void* i, int off, int TypeDescSource, int TypeDescOut, bool isref) = reinterpret_cast<void* (__cdecl* const)(void*, int, int, int, bool)>(0x5C36EE);
	return static_cast<CastTo*>(shok_dyncastFunc(i, 0, CastFrom::TypeDesc, CastTo::TypeDesc, false));
}

struct shok_object {
	int vtable;
};
struct shok_BB_IObject : shok_object {

	void Destructor(bool free); // not everything should be destroyed with this destructor, make sure there is not somenting extra for your type in question
	unsigned int GetIdentifier() const;

	static constexpr int TypeDesc = 0x7FFE08;
	static constexpr int vtp = 0x7620F0;
	// no identifier
};

struct shok_vtable_BB_IObject {
	void(__thiscall* dtor)(shok_BB_IObject* th, bool free);
	unsigned int(__thiscall* GetClassIdentifier)(const shok_BB_IObject* th);
	PADDINGI(1); // unknown, thiscall (th, x,x), cast to identifier?
};

struct shok_BB_IPostEvent : shok_object {

};

template<class T>
bool contains(const T* data, const T search, int num) {
	for (int i = 0; i < num; i++)
		if (data[i] == search)
			return true;
	return false;
}

const char* (__cdecl* const shok_GetStringTableText)(const char* key) = reinterpret_cast<const char* (__cdecl* const)(const char*)>(0x556D2E);


enum class win_mouseEvents : int {
	KeyDown = 0x100,
	KeyUp = 0x101,
	Char = 0x102,
	SysKeyDown = 0x104,
	SysKeyUp = 0x105,
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


#include "s5_defines.h"
#include "s5_exception.h"
#include "s5tech.h"
#include "s5_behaviorProps.h"
#include "s5_entitytype.h"
#include "s5_behaviors.h"
#include "s5_entity.h"
#include "s5_entitydisplay.h"
#include "s5_events.h"
//#include "s5_netevents.h"
#include "s5effects.h"
#include "s5player.h"
#include "s5RWEngine.h"
#include "s5_idmanager.h"
#include "s5_cutscene.h"
#include "s5_entityandeffectmanager.h"
#include "s5_maplogic.h"
#include "s5_mapdisplay.h"
#include "s5_config.h"
#include "s5globalObjects.h"
#include "s5ui.h"
#include "s5widget.h"
#include "s5framework.h"
#include "s5tasklist.h"
#include "s5classfactory.h"
#include "s5sound.h"

static inline void(_stdcall* const shok_SetHighPrecFPU)() = reinterpret_cast<void(_stdcall*)()>(0x5C8451);

static inline int(__stdcall* const shok_loadBuffer)(lua_State* L, const char* buff, size_t bufflen, const char* name) = reinterpret_cast<int(__stdcall*)(lua_State*, const char*, size_t, const char*)>(0x59BE57);


static inline lua_State** const shok_luastate_game = reinterpret_cast<lua_State**>(0x853A9C);
extern lua_State* mainmenu_state;
static inline HWND* shok_mainWindowHandle = reinterpret_cast<HWND*>(0x84ECC4);

struct shok_BB_CFileStreamEx : shok_object {
private:
	int x = 0;

public:
	static constexpr int vtp = 0x761C60;


	shok_BB_CFileStreamEx();
	~shok_BB_CFileStreamEx();
	bool OpenFile(const char* filename, int u);
	size_t GetSize();
	int ReadToBuffer(void* buff, size_t s);
	void Close();
};
const char* ReadFileToString(const char* name, size_t* size);
bool DoesFileExist(const char* name);

void RedirectCall(void* call, void* redirect);
void RedirectCallVP(void* call, void* redirect);
long long WriteJump(void* adr, void* toJump);
static_assert(sizeof(long long) == 8);

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
