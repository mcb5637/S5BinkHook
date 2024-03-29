#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"

namespace BB {
	class CEvent : public BB::IObject {
	public:
		int EventTypeId;

		CEvent(int eventid);
		CEvent(shok::EventIDs eventid);
		CEvent(shok::NetEventIds eventid);
		CEvent(shok::FeedbackEventIds eventid);
		CEvent(shok::InputEventIds eventid);
		CEvent(BB::CEvent&&) = default;
		CEvent(const BB::CEvent&) = default;
		CEvent& operator=(BB::CEvent&&) = default;
		CEvent& operator=(const BB::CEvent&) = default;

		bool IsEvent(shok::EventIDs id);
		bool IsEvent(shok::NetEventIds id);
		bool IsEvent(shok::FeedbackEventIds id);
		bool IsEvent(shok::InputEventIds id);

		static inline constexpr int vtp = 0x762114;
		static inline constexpr int TypeDesc = 0x7FFE24;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x472E2780);
		static inline const BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x84EE58);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

	protected:
		void SetVT(int vt);
	};
}

namespace EGL {
	// input events
	template<class T, unsigned int id>
	requires (std::same_as<T, int> && id == 0xFE5B4097) || (std::same_as<T, bool> && id == 0x29EC0467) || (std::same_as<T, float> && id == 0x4C324467)
	class CEventValue : public BB::CEvent {
	public:
		T Data = {};

		static constexpr int vtp() {
			if constexpr (std::same_as<T, int> && id == 0xFE5B4097) {
				return 0x763130;
			}
			else if constexpr (std::same_as<T, bool> && id == 0x29EC0467) {
				return 0x76E220;
			}
			else if constexpr (std::same_as<T, float> && id == 0x4C324467) {
				return 0x76F8F4;
			}
			else {
				//static_assert(false, "template initialization faled for CEventValue");
			}
		}
		static constexpr int TypeDesc() {
			if constexpr (std::same_as<T, int> && id == 0xFE5B4097) {
				return 0x801270;
			}
			else if constexpr (std::same_as<T, bool> && id == 0x29EC0467) {
				return 0x810ADC;
			}
			else if constexpr (std::same_as<T, float> && id == 0x4C324467) {
				return 0x8117B0;
			}
			else {
				//static_assert(false, "template initialization faled for CEventValue");
			}
		}

		CEventValue(shok::EventIDs e, T d) : BB::CEvent(e) {
			SetVT(vtp());
			Data = d;
		}
		CEventValue(CEventValue&&) = default;
		CEventValue(const CEventValue&) = default;
		CEventValue& operator=(CEventValue&&) = default;
		CEventValue& operator=(const CEventValue&) = default;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(id);
	};
	using CEventValue_Int = CEventValue<int, 0xFE5B4097>;
	using CEventValue_Bool = CEventValue<bool, 0x29EC0467>;
	using CEventValue_Float = CEventValue<float, 0x4C324467>;


	template<class T, unsigned int id>
	requires (std::same_as<T, bool> && id == 0x65DE8317) || (std::same_as<T, int> && id == 0x483040E7) || (std::same_as<T, float> && id == 0x578EE8F7)
	class CEventGetValue : public BB::CEvent {
	public:
		T Data = {};

		static constexpr int vtp() {
			if constexpr (std::same_as<T, bool> && id == 0x65DE8317) {
				return 0x766CB4;
			}
			else if constexpr (std::same_as<T, int> && id == 0x483040E7) {
				return 0x766CC4;
			}
			else if constexpr (std::same_as<T, float> && id == 0x578EE8F7) {
				return 0x76E210;
			}
			else {
				//static_assert(false, "template initialization faled for CEventGetValue");
			}
		}
		static constexpr int TypeDesc() {
			if constexpr (std::same_as<T, bool> && id == 0x65DE8317) {
				return 0x807A70;
			}
			else if constexpr (std::same_as<T, int> && id == 0x483040E7) {
				return 0x807AA4;
			}
			else if constexpr (std::same_as<T, float> && id == 0x578EE8F7) {
				return 0x810AAC;
			}
			else {
				//static_assert(false, "template initialization faled for CEventGetValue");
			}
		}

		CEventGetValue(shok::EventIDs e) : BB::CEvent(e) {
			SetVT(vtp());
		}
		CEventGetValue(CEventGetValue&&) = default;
		CEventGetValue(const CEventGetValue&) = default;
		CEventGetValue& operator=(CEventGetValue&&) = default;
		CEventGetValue& operator=(const CEventGetValue&) = default;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(id);
	};
	// EGL::CEventGetValue<GGL::CBuildingMerchantBehaviorProps const *,-1872076829> 7704A4
	// EGL::CEventGetValue<GGL::CNeutralBridgeBehavior *,1150290935>
	// EGL::CEventGetValue<GGL::CBattleBehavior *,-646837913>
	// EGL::CEventGetValue<GGL::CSoldierBehavior *,-806903129>
	// EGL::CEventGetValue<GGL::CLeaderBehavior *,-1190255961>
	// EGL::CEventGetValue<GGL::CBehaviorFollow *,-1301899769>
	// EGL::CEventGetValue<EGL::CMovementBehavior *,212523703>

	class CEventEntityGetBool : public CEventGetValue_Bool {
	public:
		shok::EntityId Entity;

		CEventEntityGetBool(shok::EventIDs e, shok::EntityId eid);

		static inline constexpr int vtp = 0x7704B4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB5641427);
	};


	class IEventEntityID {
	public:
		virtual shok::EntityId GetEntityID() const = 0;
		static inline constexpr int vtp = 0x766B34;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x4E0C8853);
	};
	class CEvent1Entity : public BB::CEvent, public IEventEntityID {
	public:
		shok::EntityId EntityID;

		CEvent1Entity(shok::EventIDs e, shok::EntityId ent);
		CEvent1Entity(CEvent1Entity&&) = default;
		CEvent1Entity(const CEvent1Entity&) = default;
		CEvent1Entity& operator=(CEvent1Entity&&) = default;
		CEvent1Entity& operator=(const CEvent1Entity&) = default;

		virtual shok::EntityId GetEntityID() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

		static inline constexpr int vtp = 0x766C60;
		static inline constexpr int TypeDesc = 0x8079A8;
		static inline constexpr int vtp_IEventEntityID = 0x766C58;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3B55826D);
	};

	class CEventPosition : public BB::CEvent {
	public:
		shok::Position Position;

		CEventPosition(shok::EventIDs e, const shok::Position& p);
		CEventPosition(CEventPosition&&) = default;
		CEventPosition(const CEventPosition&) = default;
		CEventPosition& operator=(CEventPosition&&) = default;
		CEventPosition& operator=(const CEventPosition&) = default;

		static inline constexpr int vtp = 0x766C70;
		static inline constexpr int TypeDesc = 0x8079C8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x175247D);
	};

	class CEventPositionAndTaskState : public CEventPosition {
	public:
		shok::TaskState State;

		CEventPositionAndTaskState(shok::EventIDs e, const shok::Position& p, shok::TaskState s);
		CEventPositionAndTaskState(CEventPositionAndTaskState&&) = default;
		CEventPositionAndTaskState(const CEventPositionAndTaskState&) = default;
		CEventPositionAndTaskState& operator=(CEventPositionAndTaskState&&) = default;
		CEventPositionAndTaskState& operator=(const CEventPositionAndTaskState&) = default;

		static inline constexpr int vtp = 0x775F78;
		static inline constexpr int TypeDesc = 0x81D9C8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x5ADDA397);
	};

	class IEventPlayerID {
	public:
		virtual shok::PlayerId GetPlayerID() const = 0;
		static inline constexpr int vtp = 0x76D788;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7B28CFC3);
	};
	class CEventPlayerID : public BB::CEvent, public IEventPlayerID {
	public:
		shok::PlayerId PlayerID;

		CEventPlayerID(shok::EventIDs e, shok::PlayerId p);
		CEventPlayerID(CEventPlayerID&&) = default;
		CEventPlayerID(const CEventPlayerID&) = default;
		CEventPlayerID& operator=(CEventPlayerID&&) = default;
		CEventPlayerID& operator=(const CEventPlayerID&) = default;

		virtual shok::PlayerId GetPlayerID() const override;

		static inline constexpr int vtp = 0x76D900;
		static inline constexpr int TypeDesc = 0x80E078;
		static inline constexpr int vtp_IEventPlayerID = 0x76D8F8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xE2540573);
	};

	class CEventSubAnim : public BB::CEvent {
	public:
		shok::AnimationId AnimId;
		int SubAnimIndex;
		bool PlayBackwards, IsLooped;

		CEventSubAnim(shok::EventIDs e, shok::AnimationId an, int subind, bool back, bool loop);
		CEventSubAnim(CEventSubAnim&&) = default;
		CEventSubAnim(const CEventSubAnim&) = default;
		CEventSubAnim& operator=(CEventSubAnim&&) = default;
		CEventSubAnim& operator=(const CEventSubAnim&) = default;

		static inline constexpr int vtp = 0x76D910;
		static inline constexpr int TypeDesc = 0x80E09C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x175247D);
	};

	class IEvent2Entities {
	public:
		virtual shok::EntityId GetActorID() const = 0;
		virtual shok::EntityId GetTargetID() const = 0;

		static inline constexpr int vtp = 0x76D790;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x84682AC3);
	};
	class CEvent2Entities : public BB::CEvent, public IEvent2Entities {
	public:
		shok::EntityId ActorId, TargetId;

		CEvent2Entities(shok::EventIDs e, shok::EntityId aid, shok::EntityId tid);
		CEvent2Entities(CEvent2Entities&&) = default;
		CEvent2Entities(const CEvent2Entities&) = default;
		CEvent2Entities& operator=(CEvent2Entities&&) = default;
		CEvent2Entities& operator=(const CEvent2Entities&) = default;

		virtual shok::EntityId GetActorID() const override;
		virtual shok::EntityId GetTargetID() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

		static inline constexpr int vtp = 0x76D92C;
		static inline constexpr int TypeDesc = 0x80E0BC;
		static inline constexpr int vtp_IEvent2Entities = 0x76D920;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x472E2780);
	};

	class CEventThousandthsGetInteger : public BB::CEvent {
	public:
		int Thousands;
		int Data = 0;

		CEventThousandthsGetInteger(shok::EventIDs e, int thousands);
		CEventThousandthsGetInteger(CEventThousandthsGetInteger&&) = default;
		CEventThousandthsGetInteger(const CEventThousandthsGetInteger&) = default;
		CEventThousandthsGetInteger& operator=(CEventThousandthsGetInteger&&) = default;
		CEventThousandthsGetInteger& operator=(const CEventThousandthsGetInteger&) = default;

		static inline constexpr int vtp = 0x775F68;
		static inline constexpr int TypeDesc = 0x81D998;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x202DA887);
	};

	class CEventSoundPositionAndID : public BB::CEvent {
	public:
		shok::SoundId SoundId;
		bool HasPosition;
		float X, Y, Z;
		int Volume;
		bool Looped;

		CEventSoundPositionAndID(shok::EventIDs e, shok::SoundId soundid, bool hasPos, const shok::Position& p, float z, int vol, bool looped);
		CEventSoundPositionAndID(CEventSoundPositionAndID&&) = default;
		CEventSoundPositionAndID(const CEventSoundPositionAndID&) = default;
		CEventSoundPositionAndID& operator=(CEventSoundPositionAndID&&) = default;
		CEventSoundPositionAndID& operator=(const CEventSoundPositionAndID&) = default;

		static inline constexpr int vtp = 0x76D4B0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x956FCE37);
	};

	class CEventGetPosition : public BB::CEvent {
	public:
		shok::Position Position;

		CEventGetPosition(shok::EventIDs e);
		CEventGetPosition(CEventGetPosition&&) = default;
		CEventGetPosition(const CEventGetPosition&) = default;
		CEventGetPosition& operator=(CEventGetPosition&&) = default;
		CEventGetPosition& operator=(const CEventGetPosition&) = default;

		static inline constexpr int vtp = 0x7743CC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x89C73967);
	};

	class CEventIndexAndEffectType : public BB::CEvent {
	public:
		int Index;
		shok::EffectTypeId EffectType = shok::EffectTypeId::Invalid;


		static inline constexpr int vtp = 0x78554C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x929AD507);
	};
}

namespace GGL {
	class CEventAttachmentTypeGetInteger : public BB::CEvent {
	public:
		shok::AttachmentType AttachmentType;
		int Data = 0;

		CEventAttachmentTypeGetInteger(shok::EventIDs e, shok::AttachmentType t);
		CEventAttachmentTypeGetInteger(CEventAttachmentTypeGetInteger&&) = default;
		CEventAttachmentTypeGetInteger(const CEventAttachmentTypeGetInteger&) = default;
		CEventAttachmentTypeGetInteger& operator=(CEventAttachmentTypeGetInteger&&) = default;
		CEventAttachmentTypeGetInteger& operator=(const CEventAttachmentTypeGetInteger&) = default;

		static inline constexpr int vtp = 0x766C80;
		static inline constexpr int TypeDesc = 0x8079EC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8745CA77);
	};
	class CEventAttachmentTypeInteger : public BB::CEvent {
	public:
		shok::AttachmentType AttachmentType;
		int Data = 0;

		CEventAttachmentTypeInteger(shok::EventIDs e, shok::AttachmentType t, int d);
		CEventAttachmentTypeInteger(CEventAttachmentTypeInteger&&) = default;
		CEventAttachmentTypeInteger(const CEventAttachmentTypeInteger&) = default;
		CEventAttachmentTypeInteger& operator=(CEventAttachmentTypeInteger&&) = default;
		CEventAttachmentTypeInteger& operator=(const CEventAttachmentTypeInteger&) = default;

		// no vtable/identifier/typedesc?
		static constexpr shok::ClassId Identifier = BB::InvalidIdentifier;
	};
	class CEventAttachmentTypeGetBool : public BB::CEvent {
	public:
		shok::AttachmentType AttachmentType;
		bool Data = 0;

		CEventAttachmentTypeGetBool(shok::EventIDs e, shok::AttachmentType t);

		static inline constexpr int vtp = 0x776EF0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8745CA77);
	};

	class CEventEntityIndex : public BB::CEvent {
	public:
		shok::EntityId EntityId;
		int Index;

		CEventEntityIndex(shok::EventIDs e, shok::EntityId eid, int ind);
		CEventEntityIndex(CEventEntityIndex&&) = default;
		CEventEntityIndex(const CEventEntityIndex&) = default;
		CEventEntityIndex& operator=(CEventEntityIndex&&) = default;
		CEventEntityIndex& operator=(const CEventEntityIndex&) = default;

		static inline constexpr int vtp = 0x766C90;
		static inline constexpr int TypeDesc = 0x807A20;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x472E2780);
	};

	class CEventTransaction : public BB::CEvent {
	public:
		shok::ResourceType SellType, BuyType;
		float BuyAmount; // 4

		CEventTransaction(shok::EventIDs e, shok::ResourceType sell, shok::ResourceType buy, float buyAm);
		CEventTransaction(CEventTransaction&&) = default;
		CEventTransaction(const CEventTransaction&) = default;
		CEventTransaction& operator=(CEventTransaction&&) = default;
		CEventTransaction& operator=(const CEventTransaction&) = default;

		static inline constexpr int vtp = 0x76D93C;
		static inline constexpr int TypeDesc = 0x80E0E0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x86EF8F47);
	};

	class CEventGoodsTraded : public CEventTransaction, public EGL::IEventEntityID {
	public:
		shok::EntityId Entity;
		float SellAmount; //7

		CEventGoodsTraded(shok::EventIDs e, shok::ResourceType sell, shok::ResourceType buy, float buyAm, shok::EntityId en, float sellam);
		virtual shok::EntityId GetEntityID() const override;

		static inline constexpr int vtp = 0x775CF8;
		static inline constexpr int vtp_IEventEntityID = 0x775CF0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x90CC5403);
	};

	class CEventPositionAnd2EntityTypes : public EGL::CEventPosition {
	public:
		shok::EntityTypeId Type1, Type2;

		CEventPositionAnd2EntityTypes(shok::EventIDs e, const shok::Position& p, shok::EntityTypeId t1, shok::EntityTypeId t2);
		CEventPositionAnd2EntityTypes(CEventPositionAnd2EntityTypes&&) = default;
		CEventPositionAnd2EntityTypes(const CEventPositionAnd2EntityTypes&) = default;
		CEventPositionAnd2EntityTypes& operator=(CEventPositionAnd2EntityTypes&&) = default;
		CEventPositionAnd2EntityTypes& operator=(const CEventPositionAnd2EntityTypes&) = default;

		static inline constexpr int vtp = 0x76D94C;
		static inline constexpr int TypeDesc = 0x80E104;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x5FF4D28D);
	};

	class CEventEntityAttachment : public BB::CEvent {
	public:
		shok::AttachmentType Type;
		shok::EntityId EntityId;
		shok::EventIDs DetachEvent;

		CEventEntityAttachment(shok::EventIDs e, shok::AttachmentType ty, shok::EntityId eid, shok::EventIDs detach);
		CEventEntityAttachment(CEventEntityAttachment&&) = default;
		CEventEntityAttachment(const CEventEntityAttachment&) = default;
		CEventEntityAttachment& operator=(CEventEntityAttachment&&) = default;
		CEventEntityAttachment& operator=(const CEventEntityAttachment&) = default;

		static inline constexpr int vtp = 0x770844;
		static inline constexpr int TypeDesc = 0x8129E0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB17BC067);
	};

	class CEventChangeMotivation : public EGL::CEventValue_Float {
	public:
		shok::WorkerReason Reason;

		CEventChangeMotivation(shok::EventIDs e, float d, shok::WorkerReason r);

		static inline constexpr int vtp = 0x76F904;
		static inline constexpr int TypeDesc = 0x8117E0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x992A8BCD);
	};

	class CEventStartAlphaBlending : public BB::CEvent {
	public:
		int Alpha;
		float BlendingTime;

		static inline constexpr int vtp = 0x773594;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x199C7D73);
	};

	class CEventHeroAbilityInteger : public EGL::CEventGetValue_Int {
	public:
		shok::AbilityId Ability;

		CEventHeroAbilityInteger(shok::EventIDs id, shok::AbilityId ab);

		static inline constexpr int vtp = 0x773DF8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8A68CFDD);
	};
}

// GGL::CEventHeroAbilityInteger -> valueint
// GGL::CEventHeroAbilityGetInteger -> getvalueint

// ENetworkX::CEventBase probably never used, since servers are down

// EGL::CEventSoundPositionAndID
// EGL::CEventAnimation : BB::CEvent

// GGL::CEventPlayerIDInteger -> EGL::CEventPlayerID 76D964
// EGL::CEventPositionAndEntity 76E150
// GGL::CEventSourceTargetPlayerID 76F8BC
// GGL::CEventDiplomacyChanged -> GGL::CEventSourceTargetPlayerID 76F8E4
// GGL::CEventChangeMotivation -> EGL::CEventValue<float,1278362727> 76F904
// GGL::CEventWeatherStateChanged 770130
// GGL::CEventBridgeProgress -> EGL::CEventPlayerID 770494
// EGL::CEventEntityGetBool
// EGL::CEventPlayerGetBool
// GGL::CEventTributePaid
// GGL::CEventOfferBase
// GGL::CEventMercenaryOffer GGL::CEventGetMercenaryOffer
// GGL::CEventTechOffer GGL::CEventGetTechOffer
// EGL::CEventRowColumn GGL::CEventGetRowColumn
// ECore::CECoreEventInteger
// EGUIX::CEventUpdateTime
// EGUIX::CGroupEvent
// EGL::CEvent1Effect

// GGL::CEventEntityAttachment
// GGL::CEventAttachmentType
// GGL::CEventAttachmentTypeGetBool
// EGL::CEventUVAnim
// EGL::CEventGetPosition
// GGL::CEventFollowInfo
// GGL::CEventKegInfo
// GGL::CEventGetPositionFromID
// GGL::CEventIndex
// EGL::CEventAnimation
// EGL::CEventEntityInRangeOfEntity


namespace BB {
	class CInputEvent : public BB::CEvent {
	public:
		bool EventHandeled; // needs confimration, no seridata
		PADDING(3);

		CInputEvent(shok::InputEventIds id);
		CInputEvent(CInputEvent&&) = default;
		CInputEvent(const CInputEvent&) = default;
		CInputEvent& operator=(CInputEvent&&) = default;
		CInputEvent& operator=(const CInputEvent&) = default;

		static inline constexpr int TypeDesc = 0x7FFE3C;
		static inline constexpr int vtp = 0x762124;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x232A806D);
	};

	class CMouseEvent : public BB::CInputEvent {
	public:
		shok::Keys KeyData; // 3 Keys.XXX modifiers bitfield
		int Clicks; // might be intended to differentiate single and double klicks, but double clicks do not get generated by win
		int X, Y, Delta; // mousepos, delta is parameter of mousewheel

		BB::CMouseEvent(shok::InputEventIds id, shok::Keys keydata, int clicks, int x, int y, int delta);
		BB::CMouseEvent(BB::CMouseEvent&&) = default;
		BB::CMouseEvent(const BB::CMouseEvent&) = default;
		BB::CMouseEvent& operator=(BB::CMouseEvent&&) = default;
		BB::CMouseEvent& operator=(const BB::CMouseEvent&) = default;

		bool IsKey(shok::Keys key);
		bool IsModifier(shok::Keys mod);

		static inline constexpr int TypeDesc = 0x7FFE98;
		static inline constexpr int vtp = 0x762154;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7B2E4D60);
	};

	class CKeyEvent : public BB::CInputEvent {
	public:
		shok::Keys KeyData;

		CKeyEvent(shok::InputEventIds id, shok::Keys keydata);
		CKeyEvent(CKeyEvent&&) = default;
		CKeyEvent(const CKeyEvent&) = default;
		CKeyEvent& operator=(CKeyEvent&&) = default;
		CKeyEvent& operator=(const CKeyEvent&) = default;

		bool IsKey(shok::Keys key);
		bool IsModifier(shok::Keys mod);

		static inline constexpr int TypeDesc = 0x7FFE5C;
		static inline constexpr int vtp = 0x762134;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x6FC080E0);
	};

	class CKeyPressEvent : public BB::CInputEvent {
	public:
		int KeyChar; // as char
		shok::Keys KeyModifier;

		CKeyPressEvent(shok::InputEventIds id, int keychar, shok::Keys keymodif);
		CKeyPressEvent(CKeyPressEvent&&) = default;
		CKeyPressEvent(const CKeyPressEvent&) = default;
		CKeyPressEvent& operator=(CKeyPressEvent&&) = default;
		CKeyPressEvent& operator=(const CKeyPressEvent&) = default;

		static inline constexpr int TypeDesc = 0x7FFE78;
		static inline constexpr int vtp = 0x762144;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x77699AE0);
	};
}

//constexpr int i = offsetof(BB::CMouseEvent, KeyData) / 4;

//struct EGL::EventHandler : shok_object {
//	void* Object;
//	void(__thiscall* Func)(void* th, BB::CEvent* ev);
//};

namespace CppLogic::Events {
	class AdvHurtEvent : public EGL::CEvent2Entities {
	public:
		int Damage;
		shok::AdvancedDealDamageSource Source;
		shok::PlayerId AttackerPlayer;

		AdvHurtEvent(shok::EventIDs e, shok::EntityId aid, shok::EntityId tid, int dmg, shok::AdvancedDealDamageSource sou, shok::PlayerId attpl);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1001);
	};

	class ResourceEvent : public EGL::CEvent1Entity {
	public:
		shok::ResourceType Type;
		int ResourceAmount;

		ResourceEvent(shok::EventIDs e, shok::EntityId id, shok::ResourceType rt, int am);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1002);
	};

	class ConversionEvent : public BB::CEvent, public EGL::IEventEntityID, public EGL::IEvent2Entities {
	public:
		shok::EntityId ConverterId;
		shok::EntityId TargetIDOld, TargetIDNew;

		ConversionEvent(shok::EventIDs e, shok::EntityId id, shok::EntityId told, shok::EntityId tnew);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

		virtual shok::EntityId GetEntityID() const override;

		virtual shok::EntityId GetActorID() const override;
		virtual shok::EntityId GetTargetID() const override;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1003);
	};

	class AdvHurtByEvent : public EGL::CEvent1Entity {
	public:
		int Damage;
		shok::AdvancedDealDamageSource Source;
		shok::PlayerId AttackerPlayer;

		AdvHurtByEvent(shok::EventIDs e, shok::EntityId aid, int dmg, shok::AdvancedDealDamageSource sou, shok::PlayerId attpl);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1007);
	};
}

namespace shok {
	enum class EventIDs : int {
		NoDetachEvent = 0, // only use for attachment detach event

		ResourceDoodad_RemoveOneRes = 0x10002, //BB::CEvent unused
		OnResourceDoodadDetach = 0x10003, //EGL::CEvent1Entity, also bserf
		Mine_DoWorkStep = 0x10004, //BB::CEvent
		Follow_GetFollowStatus = 0x10008, //EGL::CEventGetValue<int,1211121895>
		Mine_GetResourcesRemaining = 0x10007, //EGL::CEventGetValue<int,1211121895>
		Serf_ExtractResource = 0x10009, //EGL::CEvent1Entity, serfbattle same as 14003
		Serf_CanPathToResourceEntity = 0x1000A, //EGL::CEventEntityGetBool
		ResourceRefiner_Refine = 0x1000B, //BB::CEvent
		Tree_Init = 0x1000C, //EGL::CEventValue<int,-27574121>
		Tree_UpdateModel = 0x1000D, //BB::CEvent
		Tree_ApplyBlocking = 0x1000E, // EGL::CEventGetValue<bool,1709081367> called from GGL::CResourceDoodad::OnUpdateBlocking, if true skips resdoodad blocking

		Thief_StealFromCommand = 0x10102, //EGL::CEvent1Entity
		Thief_SecureGoodsCommand = 0x10103, //EGL::CEvent1Entity
		Thief_OnStealTargetDetach = 0x10104, //EGL::CEvent1Entity
		Thief_OnSecureTargetDetach = 0x10105, //EGL::CEvent1Entity
		Thief_IsCarryingSomething = 0x10106, //EGL::CEventGetValue<bool,1709081367>
		KegPlacer_SabotageCommand = 0x10107, //EGL::CEvent1Entity
		Keg_Setup = 0x10108, //GGL::CEventKegInfo {vt,evid,targetid}
		Keg_OnThiefDetach = 0x10109, //EGL::CEvent1Entity
		Keg_Arm = 0x1010A, //BB::CEvent
		Keg_Disarm = 0x1010B, //BB::CEvent
		KegPlacer_OnTargetDetach = 0x1010C, //EGL::CEvent1Entity
		Keg_CanBeDisarmed = 0x1010D, //EGL::CEventGetValue<bool,1709081367>
		KegPlacer_DefuseCommand = 0x1010E, //EGL::CEvent1Entity
		KegPlacer_OnDisarmDetach = 0x1010F, //EGL::CEvent1Entity
		Keg_GetTimeToExplode = 0x10110, //EGL::CEventGetValue<int,1211121895>
		Thief_GetStealProgress = 0x10111, //EGL::CEventGetValue<float,1468983543>
		KegPlacer_GetSabotageProgress = 0x10112, //EGL::CEventGetValue<float,1468983543>
		KegPlacer_GetDisarmProgress = 0x10113, //EGL::CEventGetValue<float,1468983543>
		Thief_GetAmountOfStolenResource = 0x10114, //EGL::CEventGetValue<int,1211121895>
		Thief_GetStolenResourceType = 0x10115, //EGL::CEventGetValue<int,1211121895>


		MoveCommand_Move = 0x11002, //EGL::CEventPosition
		Leader_SetTerritory = 0x11003, //EGL::CEventPosition
		Leader_SetTerritoryRange = 0x11004, //EGL::CEventValue<float,1278362727>

		Follow_TargetDestroyed = 0x11009, //EGL::CEvent1Entity
		Follow_StartFollowing = 0x1100A, //GGL::CEventFollowInfo
		Follow_CheckTargetStillAlive = 0x1100B, //BB::CEvent
		Follow_GetFollowBehavior = 0x1100C, //EGL::CEventGetValue<GGL::CBehaviorFollow *,-1301899769>

		Movement_GetNextFineWaypoint = 0x1100E, //EGL::CEventGetPosition
		Movement_MoveTo = 0x1100D, //EGL::CEventPosition better not use this, use MoveCommand
		Movement_OnSetMoveTo = 0x11017, //BB::CEvent better not use this, use MoveCommand
		// 0x1100F evade behavior? BB::CEvent
		Leader_ApproachPos = 0x1101A, //EGL::CEventPosition
		// 1101B follow semms to return -1, lots of calls
		Battle_SetBattleWalkAnim = 0x1101C, //BB::CEvent

		Formation_GetPosInFormation = 0x1101E, //EGL::CEventPosition

		// 0x11013 evadingent
		// 0x11014 evadingent to 0x11011 cevent
		// 0x11015 evadingent

		Binocular_ExploreCommand = 0x11102, //EGL::CEventPosition
		TorchPlacer_PlaceTorch = 0x11103, //EGL::CEventPosition
		PointToResources_Activate = 0x11104, //BB::CEvent

		Settler_SetDefaultTaskList = 0x12002, //BB::CEvent
		Camp_OnWorkerDetach = 0x12003, //EGL::CEvent1Entity
		Settler_TaskWait = 0x12004, // EGL::CEventValue<int,-27574121>
		Animation_SetAnimSet = 0x12007, //EGL::CEventValue<int,-27574121>
		Leader_ChangePlayer = 0x12008, //EGL::CEventValue<int,-27574121> (better use EGL::CGLEEntity::AdvChangePlayer)

		Worker_OnWorkPlaceBuildingDestroyed = 0x13002, //EGL::CEvent1Entity serf probably construction/repair destr
		Worker_FarmBuildingDestroyed = 0x13003, //EGL::CEvent1Entity
		Worker_ResidencePlaceBuildingDestroyed = 0x13004, //EGL::CEvent1Entity
		FeedbackEvent_BattleOnDamageDealt = 0x13005, //GGL::CFeedbackEventBattling
		Worker_GetMotivation = 0x13007, //EGL::CEventGetValue<float,1468983543>
		Worker_GetWorkWaitUntil = 0x13008, //EGL::CEventGetValue<int,1211121895>
		Worker_GetWorkTaskList = 0x13009, //EGL::CEventGetValue<int,1211121895> checks building, then props
		Worker_LevyTaxes = 0x1300A, //BB::CEvent
		Worker_ForceToWork = 0x1300B, //BB::CEvent
		Worker_OnGoToDefendableBuildingFailed = 0x1300C, //BB::CEvent
		Camp_GetNextFreeSlot = 0x1300D, //EGL::CEventGetValue<int,1211121895> returns slot id, on error?
		Camp_GetPositionOfSlot = 0x1300E, //GGL::CEventGetPositionFromID
		Camp_AttachCamper = 0x1300F, //GGL::CEventEntityIndex
		Camp_HasFreeSlot = 0x13010, //EGL::CEventGetValue<bool,1709081367>
		Camper_OnCampDetach = 0x13011, //EGL::CEvent1Entity
		Camper_LeaveCamp = 0x13012, //BB::CEvent
		Worker_GetNextCycleAction = 0x13013, //EGL::CEventValue<int,-27574121>, gets used by playerattractionhandler
		Worker_SetWorkTaskListsPerCycle = 0x13014, //EGL::CEventValue<int,-27574121> empty
		Worker_Bless = 0x13015, //BB::CEvent (checks and subs per settler BlessingCost, default 0, then adds BlessingBonus)
		Worker_AdvanceInCycles = 0x13016, //BB::CEvent
		Worker_ToJoinedCycle = 0x13017, //BB::CEvent
		Worker_OnLeaveBuildingDetach = 0x13018, //EGL::CEvent1Entity
		Worker_ChangeMoti = 0x13019, //GGL::CEventChangeMotivation
		Worker_GetMaxWorkTime = 0x1301A, //EGL::CEventGetValue<int, 1211121895>
		Worker_GetWorkTimeRemaining = 0x1301B, //EGL::CEventGetValue<int, 1211121895>
		Foundry_GetTaskListForCurrentCannon = 0x1301C, //EGL::CEventGetValue<int, 1211121895>
		Worker_OnBuildingCatchFire = 0x1301D, //EGL::CEvent1Entity worker something entity
		Worker_GetResourceToRefine = 0x1301E, //EGL::CEventGetValue<int, 1211121895>
		WorkerFlee_IsFleeing = 0x1301F, //EGL::CEventGetValue<bool,1709081367>
		Worker_IsEnteredBuldingRecentlyAttacked = 0x13020, //EGL::CEventGetValue<bool,1709081367>
		Worker_IsLeaving = 0x13021, //EGL::CEventGetValue<bool,1709081367>
		Worker_GetTransportModel = 0x13022, //EGL::CEventGetValue<int, 1211121895>
		SettlerMerchant_MoveInCommand = 0x13023, //BB::CEvent
		SettlerMerchant_MoveOutCommand = 0x13024, //BB::CEvent
		Worker_OnApproachingBridgeDetach = 0x13025, //EGL::CEvent1Entity empty
		NeutralBridge_IncreaseProgress = 0x13026, //GGL::CEventBridgeProgress (destroys if reaches 100)
		NeutralBridge_GetNeutralBridgeBehavior = 0x13027, //EGL::CEventGetValue<GGL::CNeutralBridgeBehavior *,1150290935>
		NeutralBrifge_GetProgress = 0x13028, //GGL::CEventBridgeProgress
		Worker_SetWorkTimeRemaining = 0x13029, //EGL::CEventValue<int,-27574121>
		SettlerMerchant_GetBuildingId = 0x1302A, //EGL::CEventGetValue<int, 1211121895>

		BuildingBeh_OnSerfDetach = 0x14002, //EGL::CEvent1Entity construction & repairing
		Serf_Construct = 0x14003, // GGL::CEventEntityIndex serfbattle?
		Keep_BuySerfCommand = 0x14004, //BB::CEvent
		Serf_CommandTurnToBattleSerf = 0x14005, //BB::CEvent battleserf stop if toserf tl
		BattleSerf_CommandTurnToSerf = 0x14006, //BB::CEvent serf stop if toserf tl
		Serf_GetResourceAttachmentOffset = 0x14007, //EGL::CEventGetValue<float,1468983543> used for resouce finder
		BattleSerf_GetTimeToChangeBack = 0x14008, //EGL::CEventGetValue<int, 1211121895>

		Leader_AttackEntity = 0x15004, //EGL::CEvent1Entity
		Leader_OnAttackCommandTargetDetach = 0x15005, //EGL::CEvent1Entity
		Battle_OnAttackAnimStartsAgainst = 0x15006, //EGL::CEvent1Entity
		Soldier_OnLeaderDetach = 0x15007, //EGL::CEvent1Entity
		Leader_OnSoldierDetach = 0x15008, //EGL::CEvent1Entity
		Leader_AttachSoldier = 0x15009, //EGL::CEvent1Entity
		Leader_GetHealthPlusTroopHealth = 0x1500A, //EGL::CEventGetValue<int, 1211121895>
		Leader_GetMaxHealthPlusTroopHealth = 0x1500B, //EGL::CEventGetValue<int, 1211121895>
		Battle_InternalEngageTarget = 0x1500C, //EGL::CEvent1Entity attaches as ATTACKER_TARGET
		Leader_OnAttackTargetDetached = 0x1500D, //EGL::CEvent1Entity
		OnAttackedBy = 0x1500E, //EGL::CEvent1Entity, CppLogic::Events::AdvHurtByEvent (subclass of EGL::CEvent1Entity) with EGL::CGLEEntity::HookHurtEntity
		Battle_GetBattleStatus = 0x15011, //EGL::CEventGetValue<int, 1211121895>
		Battle_SetBattleStatus = 0x15012, //EGL::CEventValue<int,-27574121>
		Battle_GetDamageClass = 0x15013, //EGL::CEventGetValue<int, 1211121895>
		Leader_GetCurrentNumSoldier = 0x15016, //EGL::CEventGetValue<int, 1211121895>
		Formation_SetFormationTaskList = 0x15017, //BB::CEvent
		Leader_SetFormation = 0x15018, //EGL::CEventValue<int,-27574121>
		Leader_GetFormation = 0x15019, //EGL::CEventGetValue<int, 1211121895>
		Soldier_OnSomethingTargetDetaches = 0x1501C, //EGL::CEvent1Entity probably better to not use, seems to conditionaly forward to detach, Battle_InternalEngageTarget, Leader_Defend
		Battle_OnTargetDetached = 0x1501D, //EGL::CEvent1Entity
		Leader_GetSoldierType = 0x1501E, //EGL::CEventGetValue<int,1211121895>
		Leader_GetMaxNumSoldier = 0x1501F, //EGL::CEventGetValue<int, 1211121895>
		GetArmorClass = 0x15020, //EGL::CEventGetValue<int,1211121895>
		Leader_GetAttackTarget = 0x15021, //EGL::CEventGetValue<int, 1211121895> soldier forwards to leader
		Leader_Hurt = 0x15023, //EGL::CEventValue<int,-27574121>
		Battle_GetLatestAttackerID = 0x15024, //EGL::CEventGetValue<int, 1211121895>
		Battle_GetLatestHitTurn = 0x15025, //EGL::CEventGetValue<int, 1211121895>
		Battle_GetBattleBehavior = 0x15026, //EGL::CEventGetValue<GGL::CBattleBehavior *,-646837913>
		Leader_GetLeaderBehavior = 0x15027, //EGL::CEventGetValue<GGL::CLeaderBehavior *,-1190255961>
		Soldier_GetSoldierBehavior = 0x15028, //EGL::CEventGetValue<GGL::CSoldierBehavior *,-806903129>
		SoldierGetLeaderID = 0x15029, //EGL::CEventGetValue<int, 1211121895>
		Leader_GetXP = 0x1502A, //EGL::CEventGetValue<int, 1211121895>
		Leader_SetXP = 0x1502B, //EGL::CEventValue<int,-27574121>
		Leader_OnAttackedDied = 0x1502C, //BB::CEvent based on leaderBeh->LatestAttackerID, empty
		Leader_GetCommand = 0x1502D, //EGL::CEventGetValue<int, 1211121895>
		Leader_AttackMove = 0x1502E, //EGL::CEventPosition
		Leader_HoldPosition = 0x1502F, //BB::CEvent
		Leader_Aggressive = 0x15030, //BB::CEvent deprecated, leader seems to do the same as defend
		Leader_Guard = 0x15031, //EGL::CEvent1Entity
		Leader_Defend = 0x15032, //BB::CEvent
		BombPlacer_CommandPlaceBomb = 0x15033, //EGL::CEventPosition
		LeaderPatrol = 0x15034, //EGL::CEventPosition
		Barracks_BuyLeader = 0x15035, //EGL::CEventValue<int,-27574121> ucat
		Leader_GetNearbyBarracks = 0x15036, //EGL::CEventGetValue<int, 1211121895>
		Barracks_BuySoldierForLeader = 0x15037, //EGL::CEvent1Entity
		Leader_OnCCEnd = 0x15038, //EGL::CEvent1Entity
		Leader_OnGuardedAttackedBy = 15039, //EGL::CEvent1Entity
		LeaderPatrolAddPoint = 0x1503A, //EGL::CEventPosition
		Leader_SetIsUsingTargetOrientation = 0x1503B, //EGL::CEventGetValue<bool,1709081367>
		SerfBattle_OnTurnToSerfCommand = 0x1503C, //BB::CEvent
		GetArmor = 0x1503E, //EGL::CEventGetValue<int, 1211121895>
		Barracks_GetTrainingTaskList = 0x1503F, //EGL::CEventGetValue<int, 1211121895>
		Barracks_GetTrainingTime = 0x15040, //EGL::CEventGetValue<float, 1468983543>
		Barracks_HasSpaceForOneMoreLeader = 0x15041, //EGL::CEventGetValue<bool,1709081367> (unused)
		Leader_SetTrainingTL = 0x15042, //EGL::CEventValue<int,-27574121>
		Barracks_GetLeaveTaskList = 0x15043, //EGL::CEventGetValue<int, 1211121895>
		Leader_OnBarracksDetach = 0x15044, //EGL::CEvent1Entity
		Leader_ExpellSoldier = 0x15046, //EGL::CEventValue<bool,703333479> bool seems to be use effect
		WorkerAlarmMode_Enable = 0x15047, //BB::CEvent
		WorkerAlarmMode_Disable = 0x15048, //BB::CEvent
		Battle_IsInBattleTL = 0x15049, //EGL::CEventGetValue<bool,1709081367>
		Battle_DisableAutoAttack = 0x1504A, //EGL::CEventGetValue<bool,1709081367> only used by camo
		Leader_GetUpkeepCost = 0x1504C, //EGL::CEventGetValue<float, 1468983543>

		GetDamage = 0x1503D, //EGL::CEventGetValue<int, 1211121895>
		Battle_CheckHelp = 0x15045, //EGL::CEvent1Entity unused, probably not working (does some range checnking, then attacking?)

		HeroHawk_SendHawk = 0x16002, //EGL::CEventPosition

		Hero_GetActionPoints = 0x16003, //EGL::CEventGetValue<int,1211121895> unused
		Hero_GetActionPointsMaximum = 0x16004, //EGL::CEventGetValue<int,1211121895> unused
		Hero_SetActionPoints = 0x16006, //EGL::CEventValue<int,-27574121> unused
		Hero_GetActionPointsForAbility = 0x16007, //GGL::CEventHeroAbilityGetInteger unused

		HeroAbility_IsAbilitySupported = 0x16008, //GGL::CEventHeroAbilityInteger
		HeroAbility_StandUp = 0x16009, //BB::CEvent
		CannonBuilder_BuildCannonCommand = 0x1600A, //GGL::CEventPositionAnd2EntityTypes
		AutoCannon_OnFoundationDetach = 0x1600B, //EGL::CEvent1Entity
		Foundation_OnAutoCannonDetach = 0x1600C, //EGL::CEvent1Entity
		HeroAbility_Cancel = 0x1600D, //BB::CEvent
		OnBuilderDetaches = 0x1600E, //EGL::CEvent1Entity foundation detach builder & top then die, neutralbridge detach architects
		Hawk_OnHeroDetach = 0x1600F, //BB::CEvent
		HeroAbility_GetChargeCurrent = 0x16010, //GGL::CEventHeroAbilityInteger
		HeroAbility_GetChargeMax = 0x16011, //GGL::CEventHeroAbilityInteger
		HeroAbility_SetChargeCurrent = 0x16012, //GGL::CEventHeroAbilityInteger

		Camouflage_IsInvisible = 0x16013, //EGL::CEventGetValue<bool,1709081367>
		ThiefCamouflage_Reset = 0x16014, //BB::CEvent fired from sabotage and steal goods
		Camouflage_Activate = 0x16015, //BB::CEvent
		Camouflage_GetDurationMax = 0x16016, //EGL::CEventGetValue<int,1211121895>
		Camouflage_GetDurationCurrent = 0x16017, //EGL::CEventGetValue<int,1211121895>

		HeroBehavior_GetResurrectionTimePassed = 0x16018, //EGL::CEventGetValue<int,1211121895>

		Sentinel_GetUrgency = 0x16019, //EGL::CEventGetValue<int,1211121895>

		Summon_ActivateCommand = 0x1601A, //BB::CEvent
		Summoned_OnSummonerDetach = 0x1601B, //BB::CEvent
		RangedEffect_Activate = 0x1601C, //BB::CEvent
		RangedEffect_GetDamageFactor = 0x1601D, //EGL::CEventGetValue<float,1468983543>
		RangedEffect_GetArmorFactor = 0x1601E, //EGL::CEventGetValue<float,1468983543>
		RangedEffect_GetDurationMax = 0x1601F, //EGL::CEventGetValue<int,1211121895>
		RangedEffect_GetDurationCurrent = 0x16020, //EGL::CEventGetValue<int,1211121895>

		HeroBehavior_CommandNPCInteraction = 0x16021, //EGL::CEvent1Entity

		CircularAttack_ActivateCommand = 0x16022, //BB::CEvent
		ConvertBuilding_ActivateCommand = 0x16023, //EGL::CEvent1Entity
		ConvertBuilding_OnTargetDetach = 0x16024, //EGL::CEvent1Entity
		Settler_IsFearless = 0x16025, //EGL::CEventGetValue<bool,1709081367>
		ConvertSettler_ActivateCommand = 0x16027, //EGL::CEvent1Entity
		ConvertSettler_OnTargetDetach = 0x16028, //EGL::CEvent1Entity
		LimitedLifespan_GetTimeRemaining = 0x16029, //EGL::CEventGetValue<int,1211121895>
		LimitedLifespan_GetTimeMax = 0x1602A, //EGL::CEventGetValue<int,1211121895>
		HeroBehavior_GetSpawnTurn = 0x1602B, //EGL::CEventGetValue<int,1211121895>

		MotivateVorkers_ActivateCommand = 0x1602C, //BB::CEvent
		Sniper_SnipeCommand = 0x1602D, //EGL::CEvent1Entity
		Sniper_GetRange = 0x1602E, //EGL::CEventGetValue<float,1468983543>
		Shuriken_ActivateCommand = 0x1602F, //EGL::CEvent1Entity
		Shuriken_GetRange = 0x16030, //EGL::CEventGetValue<float,1468983543>

		InflictFear_Activate = 0x16026, //BB::CEvent

		WorkPlace_OnWorkerAttach = 0x17003, //EGL::CEvent1Entity
		WorkPlace_OnWorkerDetach = 0x17004, //EGL::CEvent1Entity
		Building_OnConstructionComplete = 0x17005, //BB::CEvent affectmoti affect moti, foundation init, defendablebuil activate defendmode construction complete
		BuildingBeh_OnWorkerDetach = 0x17006, //EGL::CEvent1Entity forward to WorkPlace_OnWorkerDetach
		Market_WorkStep = 0x17007, //BB::CEvent
		University_ResearchStep = 0x17008, //BB::CEvent
		Market_GetProgress = 0x17009, //EGL::CEventGetValue<float,1468983543>
		Market_StartTrade = 0x1700A, //EGL::CEventGetValue<float,1468983543>
		Market_CancelTrade = 0x1700B, //BB::CEvent
		University_GetTechResearchedProgress = 0x1700C, //EGL::CEventGetValue<float,1468983543> 0-100, seems to be unused
		ConstructionSite_OnBuildingDetach = 0x1700D, //EGL::CEvent1Entity
		Settler_MoveToBuildingApproachPos = 0x1700E, //EGL::CEvent1Entity
		WorkerAlarmMode_OnBuildingDetach = 0x1700F, //EGL::CEvent1Entity
		AffectMotivation_GetEffect = 0x17010, //EGL::CEventGetValue<float,1468983543>
		ResourceRefiner_GetNearestSupplier = 0x17011, //EGL::CEventGetValue<int,1211121895> returns entity id
		Worker_ResetTaskList = 0x17012, //BB::CEvent
		Foundry_GetProgress = 0x17014, //EGL::CEventGetValue<int,1211121895>
		Foundry_SetProgress = 0x17015, //EGL::CEventValue<int,-27574121>
		Foundry_BuildCannonCommand = 0x17016, //EGL::CEventValue<int,-27574121>
		Foundry_GetCannonType = 0x17017, //EGL::CEventGetValue<int,1211121895>
		Foundry_WorkStep = 0x17018, //BB::CEvent
		Unused_Init = 0x17019, //BB::CEvent deprecated, never called
		Building_OnUpgradeStart = 0x1701A, //BB::CEvent
		Building_OnBuildOnDetached = 0x1701B, //BB::CEvent used by mine
		Building_OnUpgradeCancel = 0x1701C, //BB::CEvent
		IsConvertible = 0x1701D, //EGL::CEventGetValue<bool,1709081367>

		Barracks_ActivateAutoFill = 0x1701E, //BB::CEvent
		Barracks_DeActivateAutoFill = 0x1701F, //BB::CEvent
		Barracks_IsAutoFillActive = 0x17020, //EGL::CEventGetValue<bool,1709081367>
		BuildingMerchant_IsBlocked = 0x17021, //EGL::CEventPlayerGetBool
		BuildingMerchant_GetBlockingPlayer = 0x17022, //EGL::CEventGetValue<int,1211121895>
		BuildingMerchant_GetBuildingMerchantProps = 0x17023, //EGL::CEventGetValue<GGL::CBuildingMerchantBehaviorProps const *,-1872076829>
		BuildingMerchant_IsNotEmpty = 0x17024, //EGL::CEventGetValue<bool,1709081367>
		BuildingMercenary_AddOffer = 0x17025, //GGL::CEventMercenaryOffer
		BuildingMercenary_GetOffer = 0x17026, //GGL::CEventGetMercenaryOffer
		BuildingMercenary_BuyOffer = 0x17027, //GGL::CEventPlayerIDInteger
		IsMercenaryBuilding = 0x17028, //EGL::CEventGetValue<bool, 1709081367> GGL::CBuildingMercenaryBehavior
		BuildingMerchant_GetNumberOfOffers = 0x17029, //EGL::CEventGetValue<int,1211121895>
		IsMerchantBuilding = 0x1702A,  //EGL::CEventGetValue<bool, 1709081367> GGL::CBuildingMerchantBehavior
		BuildingTechTrader_AddOffer = 0x1702B, //GGL::CEventTechOffer
		BuildingTechTrader_GetOffer = 0x1702C, //GGL::CEventGetTechOffer
		IsTechTraderBuilding = 0x1702D, //EGL::CEventGetValue<bool, 1709081367>

		IsSerf = 0x18002, //EGL::CEventGetValue<bool, 1709081367>
		IsWorker = 0x18003, //EGL::CEventGetValue<bool, 1709081367>
		IsLeader = 0x18004, //EGL::CEventGetValue<bool, 1709081367>
		IsSoldier = 0x18005, //EGL::CEventGetValue<bool, 1709081367>
		IsHero = 0x18006, //EGL::CEventGetValue<bool, 1709081367>
		IsBattleOrAutocannon = 0x18007, //EGL::CEventGetValue<bool, 1709081367>
		IsSettlerOrBuilding = 0x18008, //EGL::CEventGetValue<bool, 1709081367>
		IsSerfOrWorker = 0x18009, //EGL::CEventGetValue<bool, 1709081367>
		DefendableBuilding_CanAlarmModeBeActivated = 0x1800A, // EGL::CEventGetValue<bool, 1709081367>
		IsMilitaryBuilding =  0x1800B, //EGL::CEventGetValue<bool,1709081367> building & (ofCategory Military | MilitaryBuilding)
		IsRefiner = 0x1800C, //EGL::CEventGetValue<bool, 1709081367>
		IsThief = 0x1800D, //EGL::CEventGetValue<bool, 1709081367>

		LimitedAttachment_Attach = 0x1A002, //GGL::CEventEntityAttachment
		LimitedAttachment_Dettach = 0x1A003, //GGL::CEventEntityAttachment
		LimitedAttacgment_IsActiveAndNotFull = 0x1A004, //GGL::CEventAttachmentTypeGetBool, is unused
		LimitedAttachment_IsActive = 0x1A005, //GGL::CEventAttachmentTypeGetBool
		LimitedAttachment_SetLimit = 0x1A006, //GGL::CEventAttachmentTypeInteger
		LimitedAttachment_GetMax = 0x1A007, //GGL::CEventAttachmentTypeGetInteger
		LimitedAttachment_GetFree = 0x1A008, //GGL::CEventAttachmentTypeGetInteger
		LimitedAttachment_Activate = 0x1A009, //GGL::CEventAttachmentType
		LimitedAttachment_DeActivate = 0x1A00A, //GGL::CEventAttachmentType

		LogicEvent_DiplomacyChanged = 0x1C002,
		LogicEvent_TributePaid = 0x1C003,
		LogicEvent_ResearchDone = 0x1C004,
		LogicEvent_TradeCompleted = 0x1C005,
		LogicEvent_WeatherChanged = 0x1C006,
		LogicEvent_HurtEntity = 0x1C007, // original EGL::CEvent2Entities, now CppLogic::Events::AdvHurtEvent (subclass)

		Animal_FleeFrom = 0x1E002, // EGL::CEventPosition

		AlphaBlending_Start = 0x1D003, // GGL::CEventStartAlphaBlending

		Movement_TaskMoveToApproachPosOf = 0x20001, //EGL::CEvent1Entity
		Movement_TaskMoveToPosRealtiveToEntity = 0x20002, //EGL::CEventPositionAndEntity
		Movement_TaskMoveToPos = 0x20003, //EGL::CEventPosition
		MultiSubAnim_SetSubAnim = 0x20004, //EGL::CEventSubAnim
		Behavior_Tick = 0x20005, //BB::CEvent ticks every second, also LogicEvent_Second
		LogicEvent_OnTick = 0x20006, // 10 per sec
		Movement_StatePerformMovementStep = 0x20008, //EGL::CEventThousandthsGetInteger pass state arg, returns state return
		Movement_IsMoving = 0x20009, //EGL::CEventGetValue<bool,1709081367>
		Die = 0x2000A, //BB::CEvent
		Movement_StopMove = 0x2000D, // BB::CEvent
		Movement_SetSpeedFactor = 0x2000E, //EGL::CEventValue<float,1278362727>
		//EGL::CEventPositionAndTaskState pass pos and state, call Movement_StatePerformMovementStep from state
		Movement_TaskMoveToPosAndSetState = 0x2000F,
		Movement_RotateTo = 0x20010, // EGL::CEventValue<float,1278362727>
		Movement_GetMovementBehavior = 0x20012, //EGL::CEventGetValue<EGL::CMovementBehavior *,212523703>

		Animation_GetAnim = 0x20013, //EGL::CEventGetValue<int,1211121895>
		Animation_UnSuspend = 0x20014, //EGL::CEventValue<int,-27574121> argument is ticks spent suspended
		Animation_Suspend = 0x20015, //EGL::CEventValue<int,-27574121> argument is current tick
		LogicEvent_EntityCreated = 0x20016,
		LogicEvent_EntityDestroyed = 0x20017,
		LogicEvent_PlayerDied = 0x20018,
		LogicEvent_InRange = 0x20019,
		ParticleEffectSwitch_Activate = 0x2001A, //EGL::CEventValue<int,-27574121>
		ParticleEffectSwitch_Deactivate = 0x2001B, //EGL::CEventValue<int,-27574121>
		ParticleEffectAttachment_CreateEffect = 0x2001C, //EGL::CEventIndexAndEffectType
		Animation_SetAnim = 0x2001D, //EGL::CEventAnimation
		Animation_ResetTaskType = 0x2001E, //BB::CEvent
		// 0x2001F BB::CEvent, movement cancel?
		// 0x20020 BB::CEvent, movement resume?
		Movement_GetSpeedFactor = 0x20022, //EGL::CEventGetValue<float,1468983543>
		Movement_IsPathingUsed = 0x20023, //EGL::CEventGetValue<bool,1709081367>
		//EGL::CEventValue<int,-27574121> settler only?, only works for specific circumstances, gets called by GetSector methods
		InternalGetSectorIfSomething = 0x20024,
		ReplaceableEntity_Disable = 0x20025, //BB::CEvent
		UVAnim_SetStatus = 0x20026, //EGL::CEventUVAnim
		Animation_SetSuspendedAnim = 0x20027, //BB::CEvent

		Sound_Start = 0x22001, // EGL::CEventSoundPositionAndID
		Sound_Stop = 0x22002, // EGL::CEventSoundPositionAndID

		// entity events
		CppL_OnEntityDestroy = 0x50000, // BB::CEvent deprectated
		CppL_OnEntityKilled = 0x50001, // GGL::CEventEntityIndex index is attacker player
		CppL_OnDamageDealt = 0x50002, // GGL::CEventEntityIndex entity is damaged, index is damage
		CppL_AffectedExperienceGained = 0x50003, // GGL::CEventEntityIndex, index id xp, can be modified, id is attacker

		// script events
		CppLogicEvent_OnEntityKilled = 0x50003, // CppLogic::Events::AdvHurtEvent
		CppLogicEvent_OnPayday = 0x50004, // GGL::CEventGoodsTraded
		CppLogicEvent_OnConvert = 0x50005, // CppLogic::Events::ConversionEvent
		CppLogicEvent_OnEffectCreated = 0x50006, // EGL::CEvent1Entity
		CppLogicEvent_OnFlyingEffectHit = 0x50007, // EGL::CEvent1Entity
		CppLogicEvent_OnEffectDestroyed = 0x50008, // EGL::CEvent1Entity
		CppLogicEvent_OnResourceRefined = 0x50009, // GGL::CEventGoodsTraded (also fired on entities worker and workplace, entity is worker, BuyAmount can be changed)
		CppLogicEvent_OnRefinerSupplyTaken = 0x5000A, // GGL::CEventGoodsTraded (also fired on entities worker and workplace, entity is worker)
		CppLogicEvent_OnResourceMined = 0x5000B, // GGL::CEventGoodsTraded (also fired on entities worker and workplace, entity is worker/serf)
	};

	enum class InputEventIds : int {
		KeyDown = 0x1001,
		KeyUp = 0x1002,
		KeyPressed = 0x1003,
		MouseMove = 0x1004,
		MouseButtonDown = 0x1005,
		MouseButtonUp = 0x1006,
		MouseWheel = 0x1007, // if you want to use just mousewheel without modifiers, use ERwTools::CDefCameraBehaviour::HookEnableZoom to disable zoom

		MouseEnter = 0x50001,
		MouseLeave = 0x50002,
		AutoUpdate = 0x50003,
		ManualUpdate = 0x50004,
		UnHighlightGroup = 0x50005, // EGUIX::CGroupEvent
		WidgetShow = 0x50006,
		WidgetHide = 0x50007,

		ClearFocus = 0x42000, // BB::CEvent, only used by CppLogic::Mod::UI::InputFocusWidget
	};
}
