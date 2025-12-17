#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/entity/s5_baseprops.h>

namespace EGL {
	class CMovementBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		float MovementSpeed = 0.0f, TurningSpeed = 0.0f;
		shok::TaskListId MoveTaskList{};
		shok::AnimationId MoveIdleAnim{};

		static inline constexpr int vtp = 0x784938;
		static inline constexpr int TypeDesc = 0x8373E4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2FCFDA17);
	};
	static_assert(offsetof(EGL::CMovementBehaviorProps, MovementSpeed) == 4 * 4);

	class CAnimationBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		shok::AnimationId SuspensionAnimation{};

		static inline constexpr int vtp = 0x776BB4;
		static inline constexpr int TypeDesc = 0x820DBC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7B4A2BED);
	};
}

namespace GGL {
	class CHeroAbilityProps : public EGL::CGLEBehaviorProps {
	public:
		int RechargeTimeSeconds = 0; // 4

		static inline constexpr int vtp = 0x773774;
		static inline constexpr int TypeDesc = 0x816EA8;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xA7450C5D);


		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x86CB78);
	};
	static_assert(offsetof(GGL::CHeroAbilityProps, RechargeTimeSeconds) == 4 * 4);

	class CCamouflageBehaviorProps : public GGL::CHeroAbilityProps {
	public:
		int DurationSeconds = 0; // 5
		float DiscoveryRange = 0.0f;

		static inline constexpr int vtp = 0x7778D8;
		static inline constexpr int TypeDesc = 0x823D28;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD7898E5D);
	};

	class CHeroHawkBehaviorProps : public GGL::CHeroAbilityProps {
	public:
		shok::EntityTypeId HawkType{};
		float HawkMaxRange = 0.0f;

		static inline constexpr int vtp = 0x77672C;
		static inline constexpr int TypeDesc = 0x81FE58;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9E9325AD);
	};
	class CHawkBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		shok::TaskListId ExploreTaskList{}; //4
		float MaxCirclingDistance = 0.0f;

		static inline constexpr int vtp = 0x776A7C;
		static inline constexpr int TypeDesc = 0x82079C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2AD269CD);
	};

	class CInflictFearAbilityProps : public GGL::CHeroAbilityProps {
	public:
		shok::TaskListId TaskList{};
		shok::AnimationId Animation{};
		int FlightDuration = 0;
		float Range = 0.0f, FlightRange = 0.0f;

		static inline constexpr int vtp = 0x776674;
		static inline constexpr int TypeDesc = 0x81FBAC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x829758BD);
	};

	class CCannonBuilderBehaviorProps : public GGL::CHeroAbilityProps {
	public:
		shok::TaskListId TaskList = {};

		static inline constexpr int vtp = 0x777510;
		static inline constexpr int TypeDesc = 0x823224;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x512F3EED);
		static inline BB::SerializationData* const SerializationData = reinterpret_cast<BB::SerializationData*>(0x876E98);
	};

	class CRangedEffectAbilityProps : public GGL::CHeroAbilityProps {
	public:
		bool AffectsOwn = false, AffectsFriends = false, AffectsNeutrals = false, AffectsHostiles = false, AffectsMilitaryOnly = false, AffectsLongRangeOnly = false;
		PADDING(2);
		float Range = 0.0f;
		int DurationSeconds = 0; // 8
		float DamageFactor = 1.0f, ArmorFactor = 1.0f, HealthRecoveryFactor = 0.0f;
		shok::EffectTypeId Effect{}, HealEffect{};

		static inline constexpr int vtp = 0x774E9C;
		static inline constexpr int TypeDesc = 0x81B780;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xC025B92D);

		[[nodiscard]] bool IsDefensive() const;
		[[nodiscard]] bool IsAggressive() const;
		[[nodiscard]] bool IsHeal() const;
	};
	static_assert(offsetof(GGL::CRangedEffectAbilityProps, DurationSeconds) == 8 * 4);

	class CCircularAttackProps : public GGL::CHeroAbilityProps {
	public:
		shok::TaskListId TaskList{};
		shok::AnimationId Animation{};
		shok::DamageClassId DamageClass{};
		int Damage = 0;
		float Range = 0.0f;

		static inline constexpr int vtp = 0x7774A0;
		static inline constexpr int TypeDesc = 0x82305C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xE3FB1C6D);
	};


	class CSummonBehaviorProps : public GGL::CHeroAbilityProps {
	public:
		shok::EntityTypeId SummonedEntityType = {};
		int NumberOfSummonedEntities = 0;
		shok::TaskListId SummonTaskList = {};

		static inline constexpr int vtp = 0x773C50;
		static inline constexpr int TypeDesc = 0x817DE4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x15C89FFD);

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x86D400);
	};
	class CSummonedBehaviorProps : public EGL::CGLEBehaviorProps {
	public:

		static inline constexpr int vtp = 0x773BF0;
		static inline constexpr int TypeDesc = 0x817D58;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x6E025CDD);
	};

	class CConvertSettlerAbilityProps : public GGL::CHeroAbilityProps {
	public:
		shok::TaskListId ConversionTaskList{};
		float HPToMSFactor = 0.0f, ConversionStartRange = 0.0f, ConversionMaxRange = 0.0f;

		static inline constexpr int vtp = 0x7772D0;
		static inline constexpr int TypeDesc = 0x8227A8;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD7F5781D);
	};

	class CConvertBuildingAbilityProps : public GGL::CHeroAbilityProps {
	public:
		shok::TaskListId ConversionTaskList{};
		float HPToMSFactor = 0.0f;

		static inline constexpr int vtp = 0x7773D8;
		static inline constexpr int TypeDesc = 0x822B68;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xFE86F10D);
	};

	class CSniperAbilityProps : public GGL::CHeroAbilityProps {
	public:
		shok::TaskListId TaskList{};
		shok::AnimationId Animation{};
		float Range = 0.0f, DamageFactor = 0.0f; // 7

		static inline constexpr int vtp = 0x7745E8;
		static inline constexpr int TypeDesc = 0x81901C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x4BE9AED);
	};

	class CMotivateWorkersAbilityProps : public GGL::CHeroAbilityProps {
	public:
		shok::TaskListId TaskList{};
		shok::AnimationId Animation{};
		float Range = 0.0f; // 7
		int WorkTimeBonus = 0;
		shok::EffectTypeId Effect{};

		static inline constexpr int vtp = 0x775788;
		static inline constexpr int TypeDesc = 0x81C3D8;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x69BEA76D);
	};

	class CShurikenAbilityProps : public GGL::CHeroAbilityProps {
	public:
		shok::TaskListId TaskList{};
		shok::AnimationId Animation{};
		float Range = 0.0f, MaxArcDegree = 0.0f; // 7
		int NumberShuriken = 0;
		shok::EffectTypeId ProjectileType{};
		float ProjectileOffsetHeight = 0.0f, ProjectileOffsetFront = 0.0f, ProjectileOffsetRight = 0.0f;
		shok::DamageClassId DamageClass{};
		int DamageAmount = 0;

		static inline constexpr int vtp = 0x7746B4;
		static inline constexpr int TypeDesc = 0x8192E8;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xEC73CADD);
	};

	class CKegPlacerBehaviorProperties : public GGL::CHeroAbilityProps {
	public:
		float SecondsNeededToArm = 5.0f; // 5
		float SecondsNeededToDisarm = 5.0f;
		shok::TaskListId PlaceKegTaskList{}, DisarmKegTaskList{};
		shok::EntityTypeId KegEntityType{};

		static inline constexpr int vtp = 0x7763B4;
		static inline constexpr int TypeDesc = 0x81F0AC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x4C080E87);
	};

	class CAbilityScoutBinocularProps : public GGL::CHeroAbilityProps {
	public:
		int NumberOfExplorationEntities = 1; // 5
		float SpacingOfExplorationEntities = 1.0f; // int/float?
		float ExplorationRangeOfExplorationEntities = 1.0f; // int/float?
		shok::EntityTypeId ExplorationEntityType{};
		shok::TaskListId ExplorationTaskList{};

		static inline constexpr int vtp = 0x779254;
		static inline constexpr int TypeDesc = 0x829218;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xED8ED4BD);
	};

	class CTorchPlacerBehaviorProperties : public GGL::CHeroAbilityProps {
	public:
		shok::EntityTypeId TorchEntity{}; // 5
		shok::TaskListId TaskList{};

		static inline constexpr int vtp = 0x773790;
		static inline constexpr int TypeDesc = 0x816E74;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x4D4BE597);
	};
	class CTorchBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		float Duration = 0.0f;

		static inline constexpr int vtp = 0x773880;
		static inline constexpr int TypeDesc = 0x817148;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2EDC3C97);
	};

	class CPointToResourceBehaviorProperties : public GGL::CHeroAbilityProps {
	public:
		shok::TaskListId TaskList{}; // 5
		float SearchRadius = 0.0f; // not initialized by ctor?

		static inline constexpr int vtp = 0x774FEC;
		static inline constexpr int TypeDesc = 0x81BBB0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x55EFCD07);
	};

	class CSentinelBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		float Range = 0.0f;

		static inline constexpr int vtp = 0x774BAC;
		static inline constexpr int TypeDesc = 0x81AB94;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x134118ED);
	};

	class CGLAnimationBehaviorExProps : public EGL::CAnimationBehaviorProps {
	public:
		shok::AnimSetId AnimSet{};
		shok::Map<shok::ModelId, shok::AnimSetId> ExtraSet; // Model -> AnimSet

		static inline constexpr int vtp = 0x776C48;
		static inline constexpr int TypeDesc = 0x820B90;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2AE74258);

		// looks up ExtraSet, if not found, returns AnimSet
		shok::AnimSetId GetAnimSet(shok::ModelId modelOverride);
	};

	class CWorkerBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		shok::TaskListId WorkTaskList{}, WorkIdleTaskList{}; // 4
		shok::TaskListId WorkLeaveTaskList{};
		int WorkWaitUntil = 0;
		shok::TaskListId EatTaskList{}, EatIdleTaskList{};
		int EatWait = 0;
		shok::TaskListId RestTaskList{}, RestIdleTaskList{};
		int RestWait = 0;
		shok::TaskListId IdleTaskList{};
		shok::TaskListId LeaveTaskList{}; // 15
		float AmountResearched = 1.0f;
		float WorkTimeChangeWork = 0.0f;
		float WorkTimeChangeFarm = 0.1f, WorkTimeChangeResidence = 0.1f, WorkTimeChangeCamp = 0.1f; // la20
		int WorkTimeMaxCangeFarm = 0, WorkTimeMaxChangeResidence = 0;
		float ExhaustedWorkMotivationMalus = 0.0f;
		int TransportAmount = 1;
		shok::ModelId TransportModel{}; //25
		shok::AnimationId TransportAnim{};
		shok::ResourceType ResourceToRefine{};
		float BridgeProgress = 0.1f, MaximumDistanceToBridge = 10000.0f; // la 29

		static inline constexpr int vtp = 0x772B90;
		static inline constexpr int TypeDesc = 0x813B40;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x4E64A87D);
	};

	class CBattleBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		shok::TaskListId BattleTaskList{}; // 4
		shok::AnimationId NormalAttackAnim1{}, NormalAttackAnim2{}, // 5 randomly selected if damage factor <= 1
			CounterAttackAnim{}, FinishingMoveAnim{}, // 7 counterattack if damage factor > 1, finishing seems unused
			MissAttackAnim{}, BattleIdleAnim{}, BattleWalkAnim{}; // 9
		shok::AnimationId HitAnim{};
		shok::DamageClassId DamageClass{}; // 13
		int DamageAmount = 0, MaxDamageRandomBonus = 0;
		float DamageRange = -1.0f;
		shok::EffectTypeId ProjectileEffectID{};
		float ProjectileOffsetFront = 0.0f, ProjectileOffsetRight = 0.0f, ProjectileOffsetHeight = 0.0f;
		int BattleWaitUntil = 0, MissChance = 0; // 21
		float MaxRange = 1.5f, MinRange = 0.0f;

		static inline constexpr int vtp = 0x7731C0;
		static inline constexpr int TypeDesc = 0x815F68;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x866ADB5D);
	};
	//constexpr int i = offsetof(CBattleBehaviorProps, BattleWaitUntil) / 4;

	class CLeaderBehaviorProps : public GGL::CBattleBehaviorProps {
	public:
		shok::EntityTypeId SoldierType{};
		shok::UpgradeCategoryId BarrackUpgradeCategory{};
		float HomeRadius = 3000.0f;
		int HealingPoints = 0, HealingSeconds = 0; // 28
		float AutoAttackRange = -1.0f, UpkeepCosts = 0.0f;

		static inline constexpr int vtp = 0x775FA4;
		static inline constexpr int TypeDesc = 0x81D970;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xFA0AC28D);
	};

	class CSoldierBehaviorProps : public GGL::CBattleBehaviorProps {
	public:
		static inline constexpr int vtp = 0x773D10;
		static inline constexpr int TypeDesc = 0x817FE8;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x186DA78D);
	};

	class CSerfBattleBehaviorProps : public GGL::CBattleBehaviorProps {
	public:
		static inline constexpr int vtp = 0x774B50;
		static inline constexpr int TypeDesc = 0x81A4D0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x86A8AADD);
	};

	class CBattleSerfBehaviorProps : public GGL::CLeaderBehaviorProps {
	public:
		shok::TaskListId TurnIntoSerfTaskList{}; //32 not initialized by ctor

		static inline constexpr int vtp = 0x77889C;
		static inline constexpr int TypeDesc = 0x8268F0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xFA2E053D);
	};

	class CSerfBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		class ExtractionInfoData {
		public:
			shok::EntityTypeId ResourceEntityType;
			float Delay;
			int Amount;
		};

		int ResourceSearchRadius = 0;
		shok::TaskListId ApproachConstructionSiteTaskList{}, TurnIntoBattleSerfTaskList{}; //4
		shok::Vector<ExtractionInfoData> ExtractionInfo;

		static inline constexpr int vtp = 0x774A14;
		static inline constexpr int TypeDesc = 0x819B1C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xCC7A655D);
	};

	class CLimitedAttachmentBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		class LimitedAttachmentProps {
		public:
			shok::String Type;
			int Limit = 0;
			bool IsActive = true;
			PADDING(3);
		};
		shok::Vector<LimitedAttachmentProps> Attachments;

		static inline constexpr int vtp = 0x775EB4;
		static inline constexpr int TypeDesc = 0x81D70C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xE4461E77);
	};

	class CFormationBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		struct IdleAnimProps {
			shok::AnimationId AnimID{};
			int Frequency = 0;
		};
		shok::Vector<IdleAnimProps> IdleAnims;

		static inline constexpr int vtp = 0x776DE4;
		static inline constexpr int TypeDesc = 0x8212CC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB55A9DE7);

		static inline const BB::SerializationData* SerializationData = reinterpret_cast<const BB::SerializationData*>(0x875A40);
	};
	static_assert(sizeof(CFormationBehaviorProperties) == 8 * 4);

	class CCamperBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		float Range = 50000.0f;

		static inline constexpr int vtp = 0x7777D4;
		static inline constexpr int TypeDesc = 0x8237CC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xBB31E947);
	};
	class CCampBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		shok::Vector<shok::Position> Slot;
		int RemoveDelay = 5.0f;

		static inline constexpr int vtp = 0x777854;
		static inline constexpr int TypeDesc = 0x823770;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x22B29E27);
	};

	class CGLBehaviorPropsDying : public EGL::CGLEBehaviorProps {
	public:
		shok::TaskListId DyingTaskList{};

		static inline constexpr int vtp = 0x778634;
		static inline constexpr int TypeDesc = 0x825F38;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7653C1D);
	};

	class CHeroBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		static inline constexpr int vtp = 0x7767F4;
		static inline constexpr int TypeDesc = 0x820048;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD8C7955D);
	};

	class CBombBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		float Radius = 0.0f, Delay = 0.0f;
		int Damage = 100;
		shok::EffectTypeId ExplosionEffectID{};
		shok::Vector<int> AffectedEntityTypes;

		static inline constexpr int vtp = 0x7784A0;
		static inline constexpr int TypeDesc = 0x82587C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x483AEA17);
	};
	static_assert(offsetof(GGL::CBombBehaviorProperties, Damage) == 4 * 6);

	class CKegBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		float Radius = 0.0f; //4
		int Damage = 0;
		float Delay = 0.0f;
		int DamagePercent = 50;
		shok::EffectTypeId ExplosionEffectID{};

		static inline constexpr int vtp = 0x776558;
		static inline constexpr int TypeDesc = 0x81F6DC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9D419467);
	};

	class CThiefBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		int SecondsNeededToSteal = 5, MinimumAmountToSteal = 1, MaximumAmountToSteal = 1;
		shok::ModelId CarryingModelID{};
		shok::TaskListId StealGoodsTaskList{}, SecureGoodsTaskList{};

		static inline constexpr int vtp = 0x7739E0;
		static inline constexpr int TypeDesc = 0x817400;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xF06ABAB7);
	};

	class CAutoCannonBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int NumberOfShots = 0; //4
		float RotationSpeed = 0.0f; // rad, string seri as deg
		shok::EffectTypeId CannonBallEffectType{}; // 6
		float ProjectileOffsetFront = 0.0f, ProjectileOffsetRight = 0.0f;
		shok::EffectTypeId ImpactEffectType{}; // 9
		int ReloadTime = 0;
		float MaxAttackRange = 0.0f, MinRange = 0.0f;
		shok::DamageClassId DamageClass{};
		int DamageAmount = 0;
		float DamageRange = -1.0f;
		shok::TaskListId BattleTaskList{}, SelfDestructTaskList{}; // 16


		static inline constexpr int vtp = 0x778CD4;
		static inline constexpr int TypeDesc = 0x828088;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x5B6BD62D);
	};

	class CFoundationBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		shok::EntityTypeId TopEntityType{}; //4
		float XOffset = 0.0f, YOffset = 0.0f; //int?

		static inline constexpr int vtp = 0x776D08;
		static inline constexpr int TypeDesc = 0x820F18;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x449A39AD);
	};

	class CResourceRefinerBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		class EfficiencyUpgrade {
		public:
			shok::TechnologyId Technology{};
			float Factor = 2.0f;
		};
		shok::ResourceType ResourceType{}; // 4
		float InitialFactor = 1.0f;
		shok::Vector<EfficiencyUpgrade> Efficiency;
		shok::EntityCategory SupplierCategory{};

		static inline constexpr int vtp = 0x774C24;
		static inline constexpr int TypeDesc = 0x81AD48;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB4889547);
	};

	class CAffectMotivationBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		float MotivationEffect = 0.0f; //4

		static inline constexpr int vtp = 0x7791D4;
		static inline constexpr int TypeDesc = 0x828FF0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB31BF57D);
	};

	class CLimitedLifespanBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int LifespanSeconds = 0;

		static inline constexpr int vtp = 0x775DE4;
		static inline constexpr int TypeDesc = 0x81D080;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9296D76D);
	};

	class CBarrackBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		shok::Array<shok::TaskListId, 3> TrainingTaskList; //4
		shok::TaskListId LeaveTaskList{};
		float TrainingTime = 1.0f; //9

		static inline constexpr int vtp = 0x778B34;
		static inline constexpr int TypeDesc = 0x827F48;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xA8D28667);
	};

	class CFoundryBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		class CannonInfoData {
		public:
			shok::EntityTypeId Cannon{};
			shok::TaskListId TaskList{};
		};
		shok::Vector<CannonInfoData> CannonInfo;

		static inline constexpr int vtp = 0x778B80;
		static inline constexpr int TypeDesc = 0x827F78;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x852B75D7);

		shok::TaskListId GetTaskList(shok::EntityTypeId cannonType);
	};

	class CBuildingMerchantBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		static inline constexpr int vtp = 0x7783B8;
		static inline constexpr int TypeDesc = 0x8251B4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD3302443);
	};

	class CSettlerMerchantBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		shok::TaskListId InitTaskList{}, MoveOutTaskList{}, MoveInTaskList{}; //4

		static inline constexpr int vtp = 0x7747DC;
		static inline constexpr int TypeDesc = 0x8196E0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xDF6E2BB3);
	};

	class CServiceBuildingBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		class WorkModifier {
		public:
			shok::TechnologyId Technology{};
			float WorkAmount = 1.0f;
		};
		float InitialWorkAmount = 0.0f;
		shok::Vector<WorkModifier> Work;

		static inline constexpr int vtp = 0x774830;
		static inline constexpr int TypeDesc = 0x816C3C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xAF007567);

		[[nodiscard]] float GetProgressAmount(shok::PlayerId p) const;

		virtual ~CServiceBuildingBehaviorProperties() override;
		CServiceBuildingBehaviorProperties();

		static inline const BB::SerializationData* SerializationData = reinterpret_cast<const BB::SerializationData*>(0x86F2D8);
	};

	class CWorkerFleeBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		shok::TaskListId FlightTaskList{}; //4

		static inline constexpr int vtp = 0x772A60;
		static inline constexpr int TypeDesc = 0x81374C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x91415DD);
	};

	class CWorkerAlarmModeBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		shok::TaskListId GoToDefendableBuildingTaskList{}; //4

		static inline constexpr int vtp = 0x773544;
		static inline constexpr int TypeDesc = 0x8164B4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3F0C405D);
	};

	class CDefendableBuildingBehaviorProps : public EGL::CGLEBehaviorProps {
	public:

		static inline constexpr int vtp = 0x777234;
		static inline constexpr int TypeDesc = 0x8223F4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9D1FA07D);
	};

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CGLResourceProps {
	public:
        [[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const;

		int Amount = 0;
		shok::ResourceType Good = shok::ResourceType::Gold;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9a4d90d7);
	};
	class CGLResourceDoodadBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		CGLResourceProps Resource;

		static inline constexpr int vtp = 0x774D40;
		static inline constexpr int TypeDesc = 0x81B170;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB3C8F597);
	};

	class CResourceDependentBuildingBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		class TypeData {
		public:
			shok::EntityTypeId Resource{}, Type{};
		};
		shok::Vector<TypeData> Building;

		static inline constexpr int vtp = 0x774DCC;
		static inline constexpr int TypeDesc = 0x81B4FC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x73BE4B47);
	};

	class CReplaceableEntityBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		shok::EntityTypeId EntityType{};

		static inline constexpr int vtp = 0x774E24;
		static inline constexpr int TypeDesc = 0x81B6A8;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x65E392E7);
	};

	class CMineBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		int AmountToMine = 1;

		static inline constexpr int vtp = 0x77581C;
		static inline constexpr int TypeDesc = 0x81C77C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xAFD62C97);
	};

	class CKeepBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		int NumSerfs = 0; // used anywhere?

		static inline constexpr int vtp = 0x776600;
		static inline constexpr int TypeDesc = 0x81FA34;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x37D0B437);
	};

	class CNeutralBridgeBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		CGLBuildingConstructionProps ConstructionInfo;

		static inline constexpr int vtp = 0x779B9C;
		static inline constexpr int TypeDesc = 0x8298A4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD915E0E7);
	};

	class CGLBehaviorFieldDoodadProps : public EGL::CGLEBehaviorProps {
	public:
		shok::EntityTypeId Crop{};
		PADDINGI(1);
		shok::Goods Good{};
		int NumCropsInRow = 0, NumberOfRows = 0;
		float CropSpacing = 0.0f, RowSpacing = 0.0f;
		int NumFlowers = 0;
		shok::Vector<shok::EntityTypeId> Flower;

		static inline constexpr int vtp = 0x77856C;
		static inline constexpr int TypeDesc = 0x8259C0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9819CF67);
	};
	static_assert(offsetof(CGLBehaviorFieldDoodadProps, Crop) == 4 * 4);
	static_assert(offsetof(CGLBehaviorFieldDoodadProps, NumFlowers) == 4 * 11);
	static_assert(sizeof(CGLBehaviorFieldDoodadProps) == 16 * 4);

	class CGLBehaviorCropDoodadProps : public EGL::CGLEBehaviorProps {
	public:
		shok::EntityTypeId NextPhaseCrop{};
		int PhaseTime = 0;
		bool RequiresSowing = false;

		static inline constexpr int vtp = 0x778774;
		static inline constexpr int TypeDesc = 0x826438;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x59979187);
	};
	static_assert(sizeof(CGLBehaviorCropDoodadProps) == 7 * 4);
}
