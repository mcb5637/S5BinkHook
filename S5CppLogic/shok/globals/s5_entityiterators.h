#pragma once

#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/globals/s5_maplogic.h>

// note that i recommend using CppLogic::Iterator

namespace EGL {
	class IGLEEntityPredicate {
	public:
		virtual ~IGLEEntityPredicate() = default;
		virtual bool Check(EGL::CGLEEntity* e) = 0;

		static inline constexpr int vtp = 0x766B60;
	};
	class CGLEEntityTypePredicate : public IGLEEntityPredicate {
	public:
		static inline constexpr int vtp = 0x766CA0;

		shok::EntityTypeId EntityType;
	};
	class CEntityTypesPredicate : public IGLEEntityPredicate {
	public:
		static inline constexpr int vtp = 0x76EB14;

		shok::Vector<shok::EntityTypeId> EntityTypes;

		// ctor 57A58A
	};
	class CGLEEntityPredicateArea : public IGLEEntityPredicate {
	public:
		static inline constexpr int vtp = 0x783C6C;

		shok::AARect Area;

		// ctor 579EF8 (this, shok::position*, shok::position*)
	};
	class CGLEEntityDistancePredicate : public IGLEEntityPredicate {
	public:
		static inline constexpr int vtp = 0x783C78;

		shok::Position Center;
		float RangeSquared;

		// ctor 579F9A (this, shok::position*, float range)
	};

	class CDisplayInterfaceFlagsPredicate : public IGLEEntityPredicate {
	public:
		bool RenderInvisible;

		static inline constexpr int vtp = 0x7837A4;
	};


	class IGLEEntityIDIterator {
	public:
		virtual ~IGLEEntityIDIterator() = default;
		virtual bool __stdcall MoveNext() = 0;
		virtual void __stdcall Reset() = 0;
		virtual void __stdcall Destroy() = 0;

		shok::EntityId CurrentId;
		IGLEEntityPredicate* Predicate;

		static inline constexpr int vtp = 0x76D738;
	};

	class CGLEEntityIterator : public IGLEEntityIDIterator {
	public:

		static inline constexpr int vtp = 0x783C84;
		// ctor 57A049 (this, predicate)
	};

	class CGLEEntityAreaIterator : public CGLEEntityIterator { // just uses a second predicate EGL::CGLEEntityPredicateArea
	public:
		CGLEEntityPredicateArea* Area;

		static inline constexpr int vtp = 0x7777e4;
		// ctor 57A086 (this, shok::position*, shok::position*, predicate)
	};

	class CGLEEntityDistanceIterator : public CGLEEntityIterator { // just uses a second predicate EGL::CGLEEntityDistancePredicate
	public:
		CGLEEntityDistancePredicate* Area;

		static inline constexpr int vtp = 0x783C98;
		// ctor 57A0DC (this, shok::position* center, float range, predicate)
	};

	class CPlayerEntityIterator {
	public:
		virtual ~CPlayerEntityIterator() = default;

		static inline constexpr int vtp = 0x766CD4;
		// ctor 57E0B2 (this, player, 0, 0)
		// go next 57DF7D -> bool has next
		// get current 57DF13
		// dtor sometimes inlined, vector at [2] (dtor 4A1357)
	};

	class CRegionEntityIterator {
	public:
		virtual ~CRegionEntityIterator() = default;

		EGL::RegionDataEntity::Entry* Region;
		shok::AccessCategoryFlags CategoriesRequested, CategoriesLeft;
		shok::AccessCategory CurrentCat;
		EGL::CGLEEntity** Current;
		EGL::CGLEEntity** End;
		IGLEEntityPredicate* Predicate;

		static inline constexpr int vtp = 0x76F834;
		// ctor 57A3A7 (this, x, y, accessCategoryBits, predicate)
		// go next 57A4D1 -> bool has next
		// get current 57A3F7
		// dtor basically empty
		// 57a3fd skip by predicate
	};

	class MultiRegionEntityIterator {
		// ctor 582FB7 (this, pos, range, accessCategoryBits, 0, predicate)
		// go next 5830BD -> bool has next
		// get current 583027
		// dtor 58300C
	};

	class IEntityDisplayIterator {
	public:
		virtual ~IEntityDisplayIterator() = default;
		virtual EGL::IEntityDisplay* __stdcall GetCurrent() = 0;
		virtual bool __stdcall GoNext() = 0;
		virtual void __stdcall Reset() = 0;
		// deletes predicate, then this
		virtual void __stdcall Delete() = 0;

		static inline constexpr int vtp = 0x783768;
	};
	class CEntityDisplayIterator : public IEntityDisplayIterator {
	public:
		CRegionEntityIterator RegionIter;
		IGLEEntityPredicate* Predicate;


		static inline constexpr int vtp = 0x783CC4;

		//ctor 57a426 (x, y, flags, predicate)
	};

}

namespace GGL {
	class CResourceProviderPredicate : EGL::IGLEEntityPredicate {
	public:
		static inline constexpr int vtp = 0x76FF44;
	};
}
