#pragma once
#include "s5data.h"

namespace EGL {
	class CGLEBehaviorProps : public BB::IObject, public ECore::ICheckData {
	public:
		int BehaviorIndex, BehaviorClass;

		static inline constexpr int vtp = 0x772A2C;
		static inline constexpr int TypeDesc = 0x813728;

		static BB::SerializationData* SerializationData;
	};
	static_assert(offsetof(EGL::CGLEBehaviorProps, BehaviorIndex) == 2 * 4);


	class CMovementBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		float MovementSpeed, TurningSpeed;
		int MoveTaskList, MoveIdleAnim;

		static inline constexpr int vtp = 0x784938;
		static inline constexpr int TypeDesc = 0x8373E4;
	};
	static_assert(offsetof(EGL::CMovementBehaviorProps, MovementSpeed) == 4 * 4);

	class CAnimationBehaviorProps : public EGL::CGLEBehaviorProps {
	public:

		static inline constexpr int vtp = 0x776BB4;
		static inline constexpr int TypeDesc = 0x820DBC;
	};
}

namespace GGL {
	class CHeroAbilityProps : public EGL::CGLEBehaviorProps {
	public:
		int RechargeTimeSeconds; // 4

		static inline constexpr int vtp = 0x773774;
		static inline constexpr int TypeDesc = 0x816EA8;
	};
	static_assert(offsetof(GGL::CHeroAbilityProps, RechargeTimeSeconds) == 4 * 4);

	class CCamouflageBehaviorProps : public GGL::CHeroAbilityProps {
	public:
		int DurationSeconds; // 5
		float DiscoveryRange;

		static inline constexpr int vtp = 0x7778D8;
		static inline constexpr int TypeDesc = 0x823D28;
	};

	class CHeroHawkBehaviorProps : public GGL::CHeroAbilityProps {
	public:
		int HawkType;
		float HawkMaxRange;

		static inline constexpr int vtp = 0x77672C;
		static inline constexpr int TypeDesc = 0x81FE58;
	};
	class CHawkBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int ExploreTaskList; //4
		float MaxCirclingDistance;

		static inline constexpr int vtp = 0x776A7C;
		static inline constexpr int TypeDesc = 0x82079C;
	};

	class CInflictFearAbilityProps : public GGL::CHeroAbilityProps {
	public:
		int TaskList, Animation, FlightDuration;
		float Range, FlightRange;

		static inline constexpr int vtp = 0x776674;
		static inline constexpr int TypeDesc = 0x81FBAC;
	};

	class CCannonBuilderBehaviorProps : public GGL::CHeroAbilityProps {
	public:
		int TaskList;

		static inline constexpr int vtp = 0x777510;
		static inline constexpr int TypeDesc = 0x823224;
	};

	class CRangedEffectAbilityProps : public GGL::CHeroAbilityProps {
	public:
		bool AffectsOwn, AffectsFriends, AffectsNeutrals, AffectsHostiles, AffectsMilitaryOnly, AffectsLongRangeOnly;
		PADDING(2);
		float Range;
		int DurationSeconds; // 8
		float DamageFactor, ArmorFactor, HealthRecoveryFactor;
		int Effect, HealEffect;

		static inline constexpr int vtp = 0x774E9C;
		static inline constexpr int TypeDesc = 0x81B780;

		bool IsDefensive() const;
		bool IsAggressive() const;
		bool IsHeal() const;
	};
	static_assert(offsetof(GGL::CRangedEffectAbilityProps, DurationSeconds) == 8 * 4);

	class CCircularAttackProps : public GGL::CHeroAbilityProps {
	public:
		int TaskList, Animation, DamageClass, Damage;
		float Range;

		static inline constexpr int vtp = 0x7774A0;
		static inline constexpr int TypeDesc = 0x82305C;
	};


	class CSummonBehaviorProps : public GGL::CHeroAbilityProps {
	public:
		int SummonedEntityType, NumberOfSummonedEntities, SummonTaskList;

		static inline constexpr int vtp = 0x773C50;
		static inline constexpr int TypeDesc = 0x817DE4;
	};
	class CSummonedBehaviorProps : public EGL::CGLEBehaviorProps {
	public:

		static inline constexpr int vtp = 0x773BF0;
		static inline constexpr int TypeDesc = 0x817D58;
	};

	class CConvertSettlerAbilityProps : public GGL::CHeroAbilityProps {
	public:
		int ConversionTaskList;
		float HPToMSFactor, ConversionStartRange, ConversionMaxRange;

		static inline constexpr int vtp = 0x7772D0;
		static inline constexpr int TypeDesc = 0x8227A8;
	};

	class CConvertBuildingAbilityProps : public GGL::CHeroAbilityProps {
	public:
		int ConversionTaskList;
		float HPToMSFactor;

		static inline constexpr int vtp = 0x7773D8;
		static inline constexpr int TypeDesc = 0x822B68;
	};

	class CSniperAbilityProps : public GGL::CHeroAbilityProps {
	public:
		int TaskList, Animation;
		float Range, DamageFactor; // 7

		static inline constexpr int vtp = 0x7745E8;
		static inline constexpr int TypeDesc = 0x81901C;
	};

	class CMotivateWorkersAbilityProps : public GGL::CHeroAbilityProps {
	public:
		int TaskList, Animation;
		float Range; // 7
		int WorkTimeBonus, Effect;

		static inline constexpr int vtp = 0x775788;
		static inline constexpr int TypeDesc = 0x81C3D8;
	};

	class CShurikenAbilityProps : public GGL::CHeroAbilityProps {
	public:
		int TaskList, Animation;
		float Range, MaxArcDegree; // 7
		int NumberShuriken, ProjectileType;
		float ProjectileOffsetHeight, ProjectileOffsetFront, ProjectileOffsetRight;
		int DamageClass, DamageAmount;

		static inline constexpr int vtp = 0x7746B4;
		static inline constexpr int TypeDesc = 0x8192E8;
	};

	class CKegPlacerBehaviorProperties : public GGL::CHeroAbilityProps {
	public:
		int SecondsNeededToArm; // 5
		int SecondsNeededToDisarm;
		int PlaceKegTaskList, DisarmKegTaskList, KegEntityType;

		static inline constexpr int vtp = 0x7763B4;
		static inline constexpr int TypeDesc = 0x81F0AC;
	};

	class CAbilityScoutBinocularProps : public GGL::CHeroAbilityProps {
	public:
		int NumberOfExplorationEntities; // 5
		float SpacingOfExplorationEntities; // int/float?
		int ExplorationRangeOfExplorationEntities; // int/float?
		int ExplorationEntityType, ExplorationTaskList;

		static inline constexpr int vtp = 0x779254;
		static inline constexpr int TypeDesc = 0x829218;
	};

	class CTorchPlacerBehaviorProperties : public GGL::CHeroAbilityProps {
	public:
		int TorchEntity; // 5
		int TaskList;

		static inline constexpr int vtp = 0x773790;
		static inline constexpr int TypeDesc = 0x816E74;
	};
	class CTorchBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:

		static inline constexpr int vtp = 0x773880;
		static inline constexpr int TypeDesc = 0x817148;
	};

	class CPointToResourceBehaviorProperties : public GGL::CHeroAbilityProps {
	public:
		int TaskList; // 5
		float SearchRadius;

		static inline constexpr int vtp = 0x774FEC;
		static inline constexpr int TypeDesc = 0x81BBB0;
	};

	class CSentinelBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		float Range;

		static inline constexpr int vtp = 0x774BAC;
		static inline constexpr int TypeDesc = 0x81AB94;
	};

	class CGLAnimationBehaviorExProps : public EGL::CAnimationBehaviorProps {
	public:
		int SuspensionAnimation, AnimSet; // 4 todo what belongs to ggl, and what to egl
		// list/set of {int Model, AnimSet; } ExtraSet

		static inline constexpr int vtp = 0x776C48;
		static inline constexpr int TypeDesc = 0x820B90;
	};

	class CWorkerBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int WorkTaskList, WorkIdleTaskList; // 4
		int WorkLeaveTaskList;
		int WorkWaitUntil, EatTaskList, EatIdleTaskList, EatWait, RestTaskList, RestIdleTaskList, RestWait;
		int IdleTaskList;
		int LeaveTaskList; // 15
		float AmountResearched;
		float WorkTimeChangeWork;
		float WorkTimeChangeFarm, WorkTimeChangeResidence, WorkTimeChangeCamp; // la20
		int WorkTimeMaxCangeFarm, WorkTimeMaxChangeResidence;
		float ExhaustedWorkMotivationMalus;
		int TransportAmount, TransportModel, TransportAnim, ResourceToRefine;
		float BridgeProgress, MaximumDistanceToBridge; // la 29

		static inline constexpr int vtp = 0x772B90;
		static inline constexpr int TypeDesc = 0x813B40;
	};

	class CBattleBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int BattleTaskList, NormalAttackAnim1, NormalAttackAnim2, CounterAttackAnim, FinishingMoveAnim, MissAttackAnim, BattleIdleAnim, BattleWalkAnim; // 4
		int HitAnim, DamageClass, DamageAmount, MaxDamageRandomBonus; // 12
		float DamageRange;
		int ProjectileEffectID;
		float ProjectileOffsetFront, ProjectileOffsetRight, ProjectileOffsetHeight;
		int BattleWaitUntil, MissChance;
		float MaxRange, MinRange;

		static inline constexpr int vtp = 0x7731C0;
		static inline constexpr int TypeDesc = 0x815F68;
	};

	class CLeaderBehaviorProps : public GGL::CBattleBehaviorProps {
	public:
		int SoldierType, BarrackUpgradeCategory;
		float HomeRadius;
		int HealingPoints, HealingSeconds; // 28
		float AutoAttackRange, UpkeepCosts;

		static inline constexpr int vtp = 0x775FA4;
		static inline constexpr int TypeDesc = 0x81D970;
	};

	class CSoldierBehaviorProps : public GGL::CBattleBehaviorProps {
	public:
		static inline constexpr int vtp = 0x773D10;
		static inline constexpr int TypeDesc = 0x817FE8;
	};

	class CSerfBattleBehaviorProps : public GGL::CBattleBehaviorProps {
	public:
		static inline constexpr int vtp = 0x774B50;
		static inline constexpr int TypeDesc = 0x81A4D0;
	};

	class CBattleSerfBehaviorProps : public GGL::CLeaderBehaviorProps {
	public:
		int TurnIntoSerfTaskList; //32

		static inline constexpr int vtp = 0x77889C;
		static inline constexpr int TypeDesc = 0x8268F0;
	};

	class CSerfBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		class ExtractionInfo {
		public:
			int ResourceEntityType;
			float Delay;
			int Amount;
		};

		int ResourceSearchRadius, ApproachConbstructionSiteTaskList, TurnIntoBattleSerfTaskList; //4
		shok::Vector<ExtractionInfo> ExtractionInfo;

		static inline constexpr int vtp = 0x774A14;
		static inline constexpr int TypeDesc = 0x819B1C;
	};

	class CLimitedAttachmentBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		class LimitedAttachmentProps {
		public:
			shok::String Type;
			int Limit;
			bool IsActive;
			PADDING(3);
		};
		shok::Vector<LimitedAttachmentProps> Attachments;

		static inline constexpr int vtp = 0x775EB4;
		static inline constexpr int TypeDesc = 0x81D70C;
	};

	class CFormationBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		struct IdleAnimProps {
			int AnimID, Frequency;
		};
		shok::Vector<IdleAnimProps> IdleAnims;

		static inline constexpr int vtp = 0x776DE4;
		static inline constexpr int TypeDesc = 0x8212CC;
	};

	class CCamperBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		float Range;

		static inline constexpr int vtp = 0x7777D4;
		static inline constexpr int TypeDesc = 0x8237CC;
	};
	class CCampBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		vector_padding;
		std::vector<shok::Position> Slot;
		int RemoveDelay;

		static inline constexpr int vtp = 0x777854;
		static inline constexpr int TypeDesc = 0x823770;
	};

	class CGLBehaviorPropsDying : public EGL::CGLEBehaviorProps {
	public:
		int DyingTaskList;

		static inline constexpr int vtp = 0x778634;
		static inline constexpr int TypeDesc = 0x825F38;
	};

	class CHeroBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		static inline constexpr int vtp = 0x7767F4;
		static inline constexpr int TypeDesc = 0x820048;
	};

	class CBombBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		float Radius, Delay;
		int Damage, ExplosionEffectID;
		vector_padding;
		std::vector<int, shok::Allocator<int>> AffectedEntityTypes;

		static inline constexpr int vtp = 0x7784A0;
		static inline constexpr int TypeDesc = 0x82587C;
	};
	static_assert(offsetof(GGL::CBombBehaviorProperties, Damage) == 4 * 6);

	class CKegBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		float Radius; //4
		int Damage;
		float Delay;
		int DamagePercent, ExplosionEffectID;

		static inline constexpr int vtp = 0x776558;
		static inline constexpr int TypeDesc = 0x81F6DC;
	};

	class CThiefBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		int SecondsNeededToSteal, MinimumAmountToSteal, MaximumAmountToSteal, CarryingModelID, StealGoodsTaskList, SecureGoodsTaskList;

		static inline constexpr int vtp = 0x7739E0;
		static inline constexpr int TypeDesc = 0x817400;
	};

	class CAutoCannonBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int NumberOfShots; //4
		float RotationSpeed;
		int CannonBallEffectType; // 6
		float ProjectileOffsetFront, ProjectileOffsetRight;
		int ImpactEffectType; // 9
		int ReloadTime;
		float MaxAttackRange, MinRange;
		int DamageClass, DamageAmount;
		float DamageRange;
		int BattleTaskList, SelfDestructTaskList; // 16


		static inline constexpr int vtp = 0x778CD4;
		static inline constexpr int TypeDesc = 0x828088;
	};

	class CFoundationBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int TopEntityType; //4
		float XOffset, YOffset; //int?

		static inline constexpr int vtp = 0x776D08;
		static inline constexpr int TypeDesc = 0x820F18;
	};

	class CResourceRefinerBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		class EfficencyUpgrade {
		public:
			int Technology;
			float Factor;
		};
		int ResourceType; // 4
		float InitialFactor;
		shok::Vector<EfficencyUpgrade> Efficiency;
		int SupplierCategory;

		static inline constexpr int vtp = 0x774C24;
		static inline constexpr int TypeDesc = 0x81AD48;
	};

	class CAffectMotivationBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		float MotivationEffect; //4

		static inline constexpr int vtp = 0x7791D4;
		static inline constexpr int TypeDesc = 0x828FF0;
	};

	class CLimitedLifespanBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int LifespanSeconds;

		static inline constexpr int vtp = 0x775DE4;
		static inline constexpr int TypeDesc = 0x81D080;
	};

	class CBarrackBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		int TrainingTaskList1, TrainingTaskList2, TrainingTaskList3; //4
	private:
		int MaxTrainingNumber;
	public:
		int LeaveTaskList;
		float TrainingTime; //9

		static inline constexpr int vtp = 0x778B34;
		static inline constexpr int TypeDesc = 0x827F48;
	};

	class CFoundryBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		class CannonInfoData {
		public:
			int Cannon;
			int TaskList;
		};
		shok::Vector<CannonInfoData> CannonInfo;

		static inline constexpr int vtp = 0x778B80;
		static inline constexpr int TypeDesc = 0x827F78;
	};

	class CBuildingMerchantBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		static inline constexpr int vtp = 0x7783B8;
		static inline constexpr int TypeDesc = 0x8251B4;
	};

	class CSettlerMerchantBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int InitTaskList, MoveOutTaskList, MoveInTaskList; //4

		static inline constexpr int vtp = 0x7747DC;
		static inline constexpr int TypeDesc = 0x8196E0;
	};

	class CServiceBuildingBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		class WorkModifier {
		public:
			int Technology;
			float WorkAmount;
		};
		float InitialWorkAmount; // int?
		shok::Vector<WorkModifier> Work;

		static inline constexpr int vtp = 0x774830;
		static inline constexpr int TypeDesc = 0x816C3C;
	};

	class CWorkerFleeBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int FlightTaskList; //4

		static inline constexpr int vtp = 0x772A60;
		static inline constexpr int TypeDesc = 0x81374C;
	};

	class CWorkerAlarmModeBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		int GoToDefendableBuildingTaskList; //4

		static inline constexpr int vtp = 0x773544;
		static inline constexpr int TypeDesc = 0x8164B4;
	};

	class CDefendableBuildingBehaviorProps : public EGL::CGLEBehaviorProps {
	public:

		static inline constexpr int vtp = 0x777234;
		static inline constexpr int TypeDesc = 0x8223F4;
	};

	class CGLResourceDoodadBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		struct {
			PADDINGI(1); // vtable?
			int Amount; //float?
			shok::ResourceType Good;
		} Resource; // if im unlucky this is a list or set or something similar stupid

		static inline constexpr int vtp = 0x774D40;
		static inline constexpr int TypeDesc = 0x81B170;
	};

	class CResourceDependentBuildingBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		class TypeData {
		public:
			int Resource, Type;
		};
		shok::Vector<TypeData> Building;

		static inline constexpr int vtp = 0x774DCC;
		static inline constexpr int TypeDesc = 0x81B4FC;
	};

	class CReplaceableEntityBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		int EntityType;

		static inline constexpr int vtp = 0x774E24;
		static inline constexpr int TypeDesc = 0x81B6A8;
	};

	class CMineBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		int AmountToMine;

		static inline constexpr int vtp = 0x77581C;
		static inline constexpr int TypeDesc = 0x81C77C;
	};

	class CKeepBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		int NumSerfs; // used anywhere?

		static inline constexpr int vtp = 0x776600;
		static inline constexpr int TypeDesc = 0x81FA34;
	};

	class CNeutralBridgeBehaviorProperties : public EGL::CGLEBehaviorProps {
	public:
		//shok_constructionInfo ConstructionInfo;

		static inline constexpr int vtp = 0x779B9C;
		static inline constexpr int TypeDesc = 0x8298A4;
	};
}
