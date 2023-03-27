#pragma once

#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_maplogic.h"

// note that i recomment using CppLogic::Iterator

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

		int EntityType;
	};
	class CEntityTypesPredicate : public IGLEEntityPredicate {
	public:
		static inline constexpr int vtp = 0x76EB14;

		shok::Vector<int> EntityTypes;

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


	class IGLEEntityIDIterator {
	public:
		virtual ~IGLEEntityIDIterator() = default;
		virtual bool __stdcall MoveNext() = 0;
		virtual void __stdcall Reset() = 0;
		virtual void __stdcall Destroy() = 0;

		int CurrentId;
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

		static inline constexpr int vtp = 0x783C84;
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
		shok::AccessCategoryFlags flags1, flags2;
		PADDINGI(3);
		IGLEEntityPredicate* Predicate;

		static inline constexpr int vtp = 0x76F834;
		// ctor 57A3A7 (this, x, y, accessCategoryBits, predicate)
		// go next 57A4D1 -> bool has next
		// get current 57A3F7
		// dtor basically empty
	};

	class MultiRegionEntityIterator {
		// ctor 582FB7 (this, pos, range, accessCategoryBits, 0, predicate)
		// go next 5830BD -> bool has next
		// get current 583027
		// dtor 58300C
	};
}

namespace GGL {
	class CResourceProviderPredicate : EGL::IGLEEntityPredicate {
	public:
		static inline constexpr int vtp = 0x76FF44;
	};
}
