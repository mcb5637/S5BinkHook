#pragma once
#include <string>
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_behaviors.h"

namespace CppLogic::EntityAddon {
	class EntityAddonData : public EGL::CGLEBehavior {
	public:
		int HealthOverride = -1;
		bool HealthUseBoni = true;
		int DamageOverride = -1;
		int ArmorOverride = -1;
		float ExplorationOverride = -1.0f;
		int RegenHPOverride = -1;
		int RegenSecondsOverride = -1;
		float MaxRangeOverride = -1.0f;
		std::string NameOverride = "";
		int FakeTaskValue = 0;

		void operator =(const EntityAddonData& other);

		static inline constexpr int Identifier = 0x1004;
		static BB::SerializationData SerializationData[];

		virtual ~EntityAddonData() override = default;
		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(int id) override;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) override;

		void* operator new(size_t s);
		void operator delete(void* p);

		static void Init();
	};
}
