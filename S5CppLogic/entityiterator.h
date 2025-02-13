#pragma once

#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <concepts>
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"
#include "s5_entity.h"
#include "s5_effects.h"
#include "s5_entityandeffectmanager.h"
#include "s5_player.h"
#include "luaext.h"

namespace CppLogic::Iterator {
	template<class T>
	class Predicate {
	public:
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const = 0;

		using BaseClass = Predicate<T>;
	};

	struct EntityIteratorStatus {
		int EntityIndex = -1, X = -1, Y = -1;
		shok::AccessCategory ac = shok::AccessCategory::AccessCategoryNone;

		auto operator<=>(const EntityIteratorStatus&) const = default;
	};
	template<class T>
	class ManagedIterator {
	public:
	private:
		EntityIteratorStatus current;
	public:
		const Predicate<T>* const Pred;
		ManagedIterator(const Predicate<T>* const p) : Pred(p) {
		}
	protected:
		// return nullptr on end
		virtual T* GetNextBase(EntityIteratorStatus& c) const = 0;
		virtual T* GetCurrentBase(const EntityIteratorStatus& c) const = 0;
	public:
		void Reset() {
			current = EntityIteratorStatus{};
		}
		T* GetNext(EntityIteratorStatus& curr, float* rangeOut, int* prio) const {
			while (true) {
				T* c = GetNextBase(curr);
				if (!c)
					return nullptr;
				if (Pred->Matches(c, rangeOut, prio))
					return c;
			}
		}
		T* GetNext(float* rangeOut, int* prio) {
			return GetNext(current, rangeOut, prio);
		}

		class Iter {
			friend class ManagedIterator<T>;
			const ManagedIterator& it;
			EntityIteratorStatus curr;

			Iter(const ManagedIterator& i, EntityIteratorStatus curr) : it(i) {
				this->curr = curr;
			}
		public:
			bool operator==(const Iter& o) const {
				return curr == o.curr;
			}
			T* operator*() const {
				return it.GetCurrentBase(curr);
			}
			Iter& operator++() {
				if (!it.GetNext(curr, nullptr, nullptr))
					curr = EntityIteratorStatus{};
				return *this;
			}
			Iter operator++(int) {
				Iter r = *this;
				++(*this);
				return r;
			}
		};
		Iter begin() {
			Iter i{ *this, EntityIteratorStatus{} };
			++i;
			return i;
		}
		Iter end() {
			return { *this, EntityIteratorStatus{} };
		}

		struct ExtendedData {
			T* Object = nullptr;;
			float Range = -1;
			int Priority = -1;
		};
		class EIter {
			friend class ManagedIterator<T>;
			const ManagedIterator& it;
			EntityIteratorStatus curr;
			ExtendedData Data;

			EIter(const ManagedIterator& i, EntityIteratorStatus curr) : it(i) {
				this->curr = curr;
			}
		public:
			bool operator==(const EIter& o) const {
				return curr == o.curr;
			}
			ExtendedData& operator*() {
				return Data;
			}
			EIter& operator++() {
				Data.Range = -1;
				Data.Priority = -1;
				Data.Object = it.GetNext(curr, &Data.Range, &Data.Priority);
				if (!Data.Object)
					curr = EntityIteratorStatus{};
				return *this;
			}
			EIter operator++(int) {
				EIter r = *this;
				++(*this);
				return r;
			}
		};
		EIter Ebegin() {
			EIter i{ *this, EntityIteratorStatus{} };
			++i;
			return i;
		}
		EIter Eend() {
			return { *this, EntityIteratorStatus{} };
		}
		class EIterCreator {
			friend class ManagedIterator<T>;
			ManagedIterator& it;
			EIterCreator(ManagedIterator& i) : it(i) {};
		public:
			EIter begin() {
				return it.Ebegin();
			}
			EIter end() {
				return it.Eend();
			}
		};
		EIterCreator ExtendedIterate() {
			return { *this };
		}

		T* GetNearest(float* rangeOut) {
			T* r = nullptr;
			int p = -1;
			float ra = -1;
			for (ExtendedData& e : ExtendedIterate()) {
				if (e.Priority > p || r == nullptr || (e.Range < ra && e.Priority == p)) {
					p = e.Priority;
					ra = e.Range;
					r = e.Object;
				}
			}
			if (rangeOut)
				*rangeOut = ra;
			return r;
		}
		T* GetFurthest(float* rangeOut) {
			T* r = nullptr;
			int p = -1;
			float ra = -1;
			for (ExtendedData& e : ExtendedIterate()) {
				if (e.Priority > p || r == nullptr || (e.Range > ra && e.Priority == p)) {
					p = e.Priority;
					ra = e.Range;
					r = e.Object;
				}
			}
			if (rangeOut)
				*rangeOut = ra;
			return r;
		}
		int Count() {
			int i = 0;
			for (T* e : *this) {
				++i;
			}
			return i;
		}
	};

	template<class T>
	class GlobalManagedIterator : public ManagedIterator<T> {
		EGL::TGLEItemManager<T, 65535>* manager;
	protected:
		GlobalManagedIterator(EGL::TGLEItemManager<T, 65535>* mng, const Predicate<T>* const p)
		: ManagedIterator<T>(p) {
			manager = mng;
		}

		virtual T* GetNextBase(EntityIteratorStatus& c) const override {
			while (true) {
				c.EntityIndex++;
				if (c.EntityIndex > manager->HighestUsedSlot)
					return nullptr;
				T* r = manager->GetInSlot(c.EntityIndex);
				if (r)
					return r;
			}
		}
		virtual T* GetCurrentBase(const EntityIteratorStatus& c) const override {
			return manager->GetInSlot(c.EntityIndex);
		}
	};

	// you may create/destroy entities while iterating, even the current one (just dont try to get the current one from the iterator after destroying)
	class GlobalEntityIterator : public GlobalManagedIterator<EGL::CGLEEntity> {
	public:
		GlobalEntityIterator(const Predicate<EGL::CGLEEntity>* const p);
	};

	// you may create/destroy effects while iterating, even the current one (just dont try to get the current one from the iterator after destroying)
	class GlobalEffectIterator : public GlobalManagedIterator<EGL::CEffect> {
	public:
		GlobalEffectIterator(const Predicate<EGL::CEffect>* const p);
	};

	// you may not create/destroy entities while iterating
	class PlayerEntityIterator : public ManagedIterator<EGL::CGLEEntity> {
		const GGL::CPlayerAttractionHandler& ah;
	protected:
		virtual EGL::CGLEEntity* GetNextBase(EntityIteratorStatus& c) const override;
		virtual EGL::CGLEEntity* GetCurrentBase(const EntityIteratorStatus& c) const override;
	public:
		PlayerEntityIterator(shok::PlayerId player, const Predicate<EGL::CGLEEntity>* const p);
	};
	// you may not create/destroy entities while iterating
	class MultiPlayerEntityIterator : public ManagedIterator<EGL::CGLEEntity> {
	public:
		std::array<shok::PlayerId, 9> Players;
	protected:
		virtual EGL::CGLEEntity* GetNextBase(EntityIteratorStatus& c) const override;
		virtual EGL::CGLEEntity* GetCurrentBase(const EntityIteratorStatus& c) const override;
	public:
		MultiPlayerEntityIterator(const Predicate<EGL::CGLEEntity>* const p);
		MultiPlayerEntityIterator(const Predicate<EGL::CGLEEntity>* const p, std::initializer_list<shok::PlayerId> pls);
	};
	// you may not create/destroy entities while iterating
	// area defines only the regions to search, you need a predicate for an exact area check
	// the actually searched area may (and likely is) bigger than the constructors area, but never smaller
	class MultiRegionEntityIterator : public ManagedIterator<EGL::CGLEEntity> {
		const int BaseX, BaseY;
		const int EndX, EndY;
		const shok::AccessCategoryFlags ac;

		bool NextAccessCategory(EntityIteratorStatus& c) const;
		bool NextRegion(EntityIteratorStatus& c) const;
	protected:
		virtual EGL::CGLEEntity* GetNextBase(EntityIteratorStatus& c) const override;
		virtual EGL::CGLEEntity* GetCurrentBase(const EntityIteratorStatus& c) const override;
	public:
		MultiRegionEntityIterator(float x1, float y1, float x2, float y2, shok::AccessCategoryFlags accessCategories, const Predicate<EGL::CGLEEntity>* const p);
		MultiRegionEntityIterator(const shok::AARect& area, shok::AccessCategoryFlags accessCategories, const Predicate<EGL::CGLEEntity>* const p);
		MultiRegionEntityIterator(const shok::Position& center, float range, shok::AccessCategoryFlags accessCategories, const Predicate<EGL::CGLEEntity>* const p);
	};

	template<class T, size_t s>
	class PredicateStaticAnd : public Predicate<T> {
	public:
		const Predicate<T>* preds[s];
		PredicateStaticAnd(std::initializer_list<const Predicate<T>*> l) {
			if (l.size() != s)
				throw std::out_of_range("invalid initializer size");
			std::copy(l.begin(), l.end(), preds);
		}
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const override {
			for (const auto& p : preds) {
				if (!p->Matches(e, rangeOut, prio))
					return false;
			}
			return true;
		}
	};
	template<class T>
	class PredicateDynamicAnd : public Predicate<T> {
	public:
		std::vector<const Predicate<T>*> preds;
		lua_State* L = nullptr;
		lua::Reference r = lua::State::NoRef;

		PredicateDynamicAnd() {}
		PredicateDynamicAnd(std::initializer_list<const Predicate<T>*> l) {
			preds.resize(l.size());
			std::copy(l.begin(), l.end(), preds.begin());
		}
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const override {
			for (const auto& p : preds) {
				if (!p->Matches(e, rangeOut, prio))
					return false;
			}
			return true;
		}
		~PredicateDynamicAnd() {
			if (r != lua::State::NoRef)
				lua::State(L).UnRef(r);
		}
	};

	template<class T, size_t s>
	class PredicateStaticOr : public Predicate<T> {
	public:
		const Predicate<T>* preds[s];
		PredicateStaticOr(std::initializer_list<const Predicate<T>*> l) {
			if (l.size() != s)
				throw std::out_of_range("invalid initializer size");
			std::copy(l.begin(), l.end(), preds);
		}
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const override {
			for (const auto& p : preds) {
				if (p->Matches(e, rangeOut, prio))
					return true;
			}
			return false;
		}
	};
	template<class T>
	class PredicateDynamicOr : public Predicate<T> {
	public:
		std::vector<const Predicate<T>*> preds;
		lua_State* L = nullptr;
		lua::Reference r = lua::State::NoRef;


		PredicateDynamicOr() {}
		PredicateDynamicOr(std::initializer_list<const Predicate<T>*> l) {
			preds.resize(l.size());
			std::copy(l.begin(), l.end(), preds.begin());
		}
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const override {
			for (const auto& p : preds) {
				if (p->Matches(e, rangeOut, prio))
					return true;
			}
			return false;
		}
		~PredicateDynamicOr() {
			if (r != lua::State::NoRef)
				lua::State(L).UnRef(r);
		}
	};

	template<class T>
	class PredicateNot : public Predicate<T> {
		const Predicate<T>* pred;
	public:
		lua_State* L = nullptr;
		lua::Reference r = lua::State::NoRef;


		PredicateNot(const Predicate<T>* p) {
			pred = p;
		}
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const override {
			return !pred->Matches(e, rangeOut, prio);
		}
		~PredicateNot() {
			if (r != lua::State::NoRef)
				lua::State(L).UnRef(r);
		}
	};

	template<class T>
	class PredicatePriority : public Predicate<T> {
		const Predicate<T>* pred;
		const int priori;
	public:
		lua_State* L = nullptr;
		lua::Reference r = lua::State::NoRef;


		PredicatePriority(const Predicate<T>* predicate, int priority) : pred(predicate), priori(priority) {
		}
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const override {
			if (prio && pred->Matches(e, rangeOut, prio))
				*prio = priori;
			return true;
		}
		~PredicatePriority() {
			if (r != lua::State::NoRef)
				lua::State(L).UnRef(r);
		}
	};


	class EntityPredicateOfType : public Predicate<EGL::CGLEEntity> {
		shok::EntityTypeId ety;
	public:
		EntityPredicateOfType(shok::EntityTypeId ety);
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateOfPlayer : public Predicate<EGL::CGLEEntity> {
		shok::PlayerId player;
	public:
		EntityPredicateOfPlayer(shok::PlayerId pl);
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateOfEntityCategory : public Predicate<EGL::CGLEEntity> {
		shok::EntityCategory ecat;
	public:
		EntityPredicateOfEntityCategory(shok::EntityCategory category);
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateProvidesResource : public Predicate<EGL::CGLEEntity> {
		shok::ResourceType res;
	public:
		EntityPredicateProvidesResource(shok::ResourceType resType);
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateOfUpgradeCategory : public Predicate<EGL::CGLEEntity> {
		shok::UpgradeCategoryId ucat;
	public:
		EntityPredicateOfUpgradeCategory(shok::UpgradeCategoryId category);
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateIsNotFleeingFrom : public Predicate<EGL::CGLEEntity> {
		const shok::Position pos;
		const float range;
	public:
		EntityPredicateIsNotFleeingFrom(const shok::Position& center, float r);
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};

	template<class T>
	class PredicateInCircle : public Predicate<T> {
		const shok::Position pos;
		const float rangeSquared;
	public:
		PredicateInCircle(const shok::Position& p, float rsqu) : pos(p), rangeSquared(rsqu) {
		}
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const override {
			float r = e->Position.GetDistanceSquaredTo(pos);
			if (rangeOut)
				*rangeOut = r;
			return r <= rangeSquared;
		}
	};
	template<class T>
	class PredicateInRect : public Predicate<T> {
		const shok::AARect rect;
	public:
		PredicateInRect(const shok::AARect& r) : rect{ std::min(r.low.X, r.high.X), std::min(r.low.Y, r.high.Y), std::max(r.low.X, r.high.X), std::max(r.low.Y, r.high.Y) } {
		}
		PredicateInRect(float x1, float y1, float x2, float y2) : rect{ std::min(x1, x2), std::min(y1, y2), std::max(x1, x2), std::max(y1, y2) } {
		}
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const override {
			return rect.low.X <= e->Position.X && e->Position.X <= rect.high.X && rect.low.Y <= e->Position.Y && e->Position.Y <= rect.high.Y;
		}
	};
	template<class T>
	class PredicateInArbitrayRect : public Predicate<T> {
		const CppLogic::Rect rect;
	public:
		PredicateInArbitrayRect(const CppLogic::Rect& r) : rect{r } {
		}
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const override {
			return rect.IsPosInRect(e->Position);
		}
	};
	template<class T>
	class PredicateFunc : public Predicate<T> {
		const std::function<bool(const T*, float*, int*)> func;
	public:
		PredicateFunc(const std::function<bool(const T*, float*, int*)>& f) : func(f) {
		}
		virtual bool Matches(const T* e, float* rangeOut, int* prio) const override {
			return std::invoke(func, e, rangeOut, prio);
		}
	};

	class EntityPredicateIsSettler : public Predicate<EGL::CGLEEntity> {
	public:
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateIsBuilding : public Predicate<EGL::CGLEEntity> {
	public:
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateIsBuildingAndNotConstructionSite : public Predicate<EGL::CGLEEntity> {
	public:
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateIsCombatRelevant : public Predicate<EGL::CGLEEntity> {
	public:
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateIsNotSoldier : public Predicate<EGL::CGLEEntity> {
	public:
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateIsVisible : public Predicate<EGL::CGLEEntity> {
	public:
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateIsAlive : public Predicate<EGL::CGLEEntity> {
	public:
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};
	class EntityPredicateIsNotInBuilding : public Predicate<EGL::CGLEEntity> {
	public:
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};

	class EntityPredicateOfAnyPlayer : public Predicate<EGL::CGLEEntity> {
	public:
		std::array<shok::PlayerId, 9> players;
		EntityPredicateOfAnyPlayer();
		EntityPredicateOfAnyPlayer(std::initializer_list<shok::PlayerId> pl);
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
		static int FillHostilePlayers(std::array<shok::PlayerId, 9>& players, shok::PlayerId pl);
		static int FillFriendlyPlayers(std::array<shok::PlayerId, 9>& players, shok::PlayerId pl, bool addSelf);
	};
	class EntityPredicateOfAnyType : public Predicate<EGL::CGLEEntity> {
	public:
		std::vector<shok::EntityTypeId> entityTypes;
		EntityPredicateOfAnyType() = default;
		EntityPredicateOfAnyType(std::initializer_list<shok::EntityTypeId> ety);
		virtual bool Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const override;
	};

	class EffectPredicateOfType : public Predicate<EGL::CEffect> {
		shok::EffectTypeId Type;
	public:
		EffectPredicateOfType(shok::EffectTypeId ty);
		virtual bool Matches(const EGL::CEffect* e, float* rangeOut, int* prio) const override;
	};
	class EffectPredicateOfPlayer : public Predicate<EGL::CEffect> {
		shok::PlayerId Player;
	public:
		EffectPredicateOfPlayer(shok::PlayerId pl);
		virtual bool Matches(const EGL::CEffect* e, float* rangeOut, int* prio) const override;
	};
	class EffectPredicateIsArrow : public Predicate<EGL::CEffect> {
	public:
		virtual bool Matches(const EGL::CEffect* e, float* rangeOut, int* prio) const override;
	};
	class EffectPredicateIsCannonBall : public Predicate<EGL::CEffect> {
	public:
		virtual bool Matches(const EGL::CEffect* e, float* rangeOut, int* prio) const override;
	};
	class EffectPredicateIsArrowOrCannonBall : public Predicate<EGL::CEffect> {
	public:
		virtual bool Matches(const EGL::CEffect* e, float* rangeOut, int* prio) const override;
	};
}
