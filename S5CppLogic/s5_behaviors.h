#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_player.h"

namespace EGL {
	class CGLEBehavior : public BB::IObject { // no vtable
	public:
		int SlotIndex = 0, EntityId = 0;
		EGL::CGLEBehaviorProps* PropPointer = nullptr; // 3, warning this may not be set

	protected:
		virtual void AddHandlers(int id) = 0;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) = 0;
		virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) = 0;
	private:
		virtual void unknownFuncBeh1(EGL::CGLEEntity* e); // on movement seems to copy a lot of data, maybe change behavior?
		virtual void unknownFuncBeh2(int uk);

	public:
		static inline constexpr int TypeDesc = 0x813778;
		static BB::SerializationData* SerializationData;
	};

	class CCoarsePath {
	public:
		int vtable;
		struct {
			int Goal;
			int GoalDistance;
			int WaypointsCount;
			int CacheCount;
			int CacheTail;
			int CacheItem[33];
			bool GoalCached;
			PADDING(3);
		} WayPoints;
	};
	class CMovementBehavior : public EGL::CGLEBehavior {
	public:
		int CurrentTurn;
		float MovementSpeed, TurningSpeed, SpeedFactor; // la7
		PADDINGI(1);
		shok::PositionRot NextWayPoint, LastTurnPos;
		bool IsPathingUsed, IsMoveFinished; // 15
		PADDING(2);
		PADDINGI(1); // p to EGL::CMovementBehavior::CSlotMovingEntity
		int ResumeModelID; // 17
		bool PauseMode, WasMoving, IsObstructed;
		PADDING(1);

	private:
		virtual void unknownFuncMove1() = 0; // seems to reste some behavior variables
	public:
		virtual float GetMovementSpeed() = 0;
		virtual float GetTurningSpeed() = 0;
	private:
		virtual void unknownFuncMove3(float*) = 0; // check some blocking data?
		virtual void unknownFuncMove4(float*) = 0;
		virtual void unknownFuncMove5(float*, float*, float*) = 0; // pos in rect?
		virtual void unknownFuncMove6() = 0; // seems to be always empty

	public:
		static inline constexpr int vtp = 0x7848DC;
	};

	class GLEBehaviorMultiSubAnims : public EGL::CGLEBehavior {
	public:
		PADDINGI(1);
		int LastUpdateTurn;
		struct {
			byte Active, PlayBackwards, IsLooped;
			PADDING(1);
			int AnimID, StartTurn, Duration;
			float Speed;
		} AnimSlot[4];

		// defined tasks: TASK_SET_SUB_ANIM
		// defined events: MultiSubAnim_SetSubAnim

		static inline constexpr int vtp = 0x785EEC;
		static inline constexpr int TypeDesc = 0x83AE4C;
		static inline constexpr unsigned int Identifier = 0x0FCFC0D;
	};

	struct SSlotArgsAnimation {
		PADDING(1);
		bool PlayBackwards;
		int AnimId;
		int StartTurn;
		int Duration;
		float SpeedModifier;
	};
	class CBehaviorAnimation : public CGLEBehavior {
	public:
		class CSlotAnimation : public EGL::TSlot<EGL::SSlotArgsAnimation, 796165920> {
		public:
			CBehaviorAnimation* Behavior;

			static constexpr int vtp = 0x784A84;
		};

		// all fields are in GGL::CGLBehaviorAnimationEx

	protected:
		shok::TaskStateExecutionResult StateWaitForAnim(int i);
	};
}

namespace GGL {
	class CBehaviorDefaultMovement : public EGL::CMovementBehavior {
	public:
		int FinePath; // 19 p to GGL::CPath/GGL::CLeaderPath
		EGL::CCoarsePath CoarsePath;
		int FineWayPointIndex; // 60
		int MovedFineWaypoints;

		static inline constexpr int vtp = 0x7786AC;
		static inline constexpr int TypeDesc = 0x818DF0;
		static inline constexpr unsigned int Identifier = 0x3B4EFD17;

		// defined states: Move, Move_NonCancelable, Rotate
		// defined events: Movement_XXX, 1100D, 1100E, 11017
		// defined tasks: TASK_TURN_TO_TARGET_ORIENTATION, TASK_SET_TARGET_ORIENTATION_RELATIVE
	};
	class CSettlerMovement : public GGL::CBehaviorDefaultMovement {
	public:
		static inline constexpr int vtp = 0x77471C;
		static inline constexpr int TypeDesc = 0x81961C;
		static inline constexpr unsigned int Identifier = 0x786A407;
	};
	class CLeaderMovement : public GGL::CBehaviorDefaultMovement {
	public:
		static inline constexpr int vtp = 0x775ED4;
		static inline constexpr int TypeDesc = 0x81D7E4;
		static inline constexpr unsigned int Identifier = 0x0E66510C3;
	};
	class CSoldierMovement : public GGL::CBehaviorDefaultMovement {
	public:
		static inline constexpr int vtp = 0x77438C;
		static inline constexpr int TypeDesc = 0x818E1C;
		static inline constexpr unsigned int Identifier = 0x19BC8A93;
	};
	class CBehaviorAnimalMovement : public GGL::CBehaviorDefaultMovement {
	public:
		static inline constexpr int vtp = 0x77880C;
		static inline constexpr int TypeDesc = 0x826844;
		static inline constexpr unsigned int Identifier = 0x29093B37;
	};


	class CHeroAbility : public EGL::CGLEBehavior { // no vtable
	public:
		GGL::CHeroAbilityProps* AbilityProps = nullptr;
		int SecondsCharged = 0; // 5


		virtual void AddHandlers(int id) override;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) override;
		virtual bool IsAbility(int ability) = 0; // 8
		virtual bool CanUseAbility();
		virtual bool CheckAndResetCooldown();

		// defined events: HeroAbility_XXX, Behavior_Tick, Die

		static inline constexpr int TypeDesc = 0x816E2C;
		static inline BB::SerializationData* const SerializationData = reinterpret_cast<BB::SerializationData*>(0x86CBE8);
	};

	struct SSlotArgsCamouflage {
		int LocalPlayer;
		bool IsLocalPlayer, IsNotAlliedToLocalPlayer;
	};
	class CCamouflageBehavior : public GGL::CHeroAbility, public EGL::TSlot<GGL::SSlotArgsCamouflage, 0x3AA0169D> {
	public:
		GGL::CCamouflageBehaviorProps* CamoProps;
		int InvisibilityRemaining; // 8

		virtual void UpdateCamo() = 0;

		// defined events: Behavior_Tick, Camouflage_XXX, 1504A and 16013 return isinvisible, AttackEntity 1500E camo reset

		static inline constexpr int vtp = 0x7738F4;
		static inline constexpr int TypeDesc = 0x8172E8;
		static inline constexpr unsigned int Identifier = 0x0E2D7A5DD;
	};
	static_assert(offsetof(CCamouflageBehavior, InvisibilityRemaining) == 8 * 4);

	class CThiefCamouflageBehavior : public GGL::CCamouflageBehavior {
	public:
		int TimeToInvisibility;

		// defined events: ThiefCamouflage_Reset

		static inline constexpr int vtp = 0x773934;
		static inline constexpr int TypeDesc = 0x817310;
		static inline constexpr unsigned int Identifier = 0x51D6A267;
	};

	class CHeroHawkBehavior : public GGL::CHeroAbility {
	public:
		// defined events: HeroHawk_XXX, Die, HeroAbility_StandUpOrInit

		static inline constexpr int vtp = 0x7766F0;
		static inline constexpr int TypeDesc = 0x81FE84;
		static inline constexpr unsigned int Identifier = 0x0FE5FCD9D;
	};
	class CHawkBehavior : public EGL::CGLEBehavior {
	public:
		PADDINGI(1);
		shok::Position ExplorePosition;
		int CreationTurn;

		// defined tasks: TASK_GO_TO_POS, TASK_MOVE_TO_EXPLORATION_POINT, TASK_CHECK_HERO_POSITION
		// defined events: HeroHawk_SendHawk

		static inline constexpr int vtp = 0x776A24;
		static inline constexpr int TypeDesc = 0x8207C4;
		static inline constexpr unsigned int Identifier = 0x0CB26FC4D;
	};


	class CInflictFearAbility : public GGL::CHeroAbility {
	public:
		PADDINGI(1);
		int SecondsFearRemaining; // 7

		// defined events: InflictFear_XXX, Behavior_Tick, Die
		// defined tasks: TASK_SET_SPECIAL_ATTACK_ANIM, TASK_PERFORM_SPECIAL_ATTACK

		static inline constexpr int vtp = 0x776638;
		static inline constexpr int TypeDesc = 0x81FBD8;
		static inline constexpr unsigned int Identifier = 0x400E6C6D;
	};

	class CBombPlacerBehavior : public GGL::CHeroAbility {
	public:
		shok::Position StartPosition, TargetPosition;
		byte PlacedBomb; // 10
		PADDING(3);

		// defined events: BombPlacer_XXX, HeroAbility_Cancel
		// defined tasks: TASK_PLACE_BOMB, TASK_GO_TO_BOMB_TARGET, TASK_RETREAT_FROM_BOMB_TARGET

		static inline constexpr int vtp = 0x7783D8;
		static inline constexpr int TypeDesc = 0x8255D0;
		static inline constexpr unsigned int Identifier = 0x29AF8F97;

		static void HookFixBombAttachment();
	};
	class CBombBehavior : public EGL::CGLEBehavior {
	public:
		int TimeToExplode; //4

		// defined states: Default

		static inline constexpr int vtp = 0x778468;
		static inline constexpr int TypeDesc = 0x8258A8;
		static inline constexpr unsigned int Identifier = 0x76AE8807;
	};

	class CCannonBuilderBehavior : public GGL::CHeroAbility {
	public:
		PADDINGI(1);
		shok::Position StartPosition;
		int CannonType, FoundationType;
		byte PlacedCannon; // 11
		PADDING(3);

		// defined events: CannonBuilder_XXX, HeroAbility_Cancel, Die, 1600B some form of stop?->12002
		// defined tasks: TASK_GO_TO_CANNON_POSITION, TASK_BUILD_CANNON

		static inline constexpr int vtp = 0x7774D4;
		static inline constexpr int TypeDesc = 0x823254;
		static inline constexpr unsigned int Identifier = 0x259E7B1D;
	};

	class CRangedEffectAbility : public GGL::CHeroAbility {
	public:
		PADDINGI(1);
		int SecondsRemaining; // 7

		// defined events: HeroAbility_Reset, Behavior_Tick, RangedEffect_XXX

		static inline constexpr int vtp = 0x774E54;
		static inline constexpr int TypeDesc = 0x81B7AC;
		static inline constexpr unsigned int Identifier = 0x0F622BC1D;

		void __thiscall AdvHealAffected(); // heals all attached with HERO_AFFECTED

		static void HookHealAffected(bool active);
	};

	class CCircularAttack : public GGL::CHeroAbility {
	public:
		GGL::CCircularAttackProps* CAProps;

		// defined events: CircularAttack_ActivateCommand
		// defined tasks: TASK_SET_SPECIAL_ATTACK_ANIM, TASK_PERFORM_SPECIAL_ATTACK

		static inline constexpr int vtp = 0x777464;
		static inline constexpr int TypeDesc = 0x823038;
		static inline constexpr unsigned int Identifier = 0x522330D;
	};

	class CSummonBehavior : public GGL::CHeroAbility {
	public:
		GGL::CSummonBehaviorProps* SUProps = nullptr;

		// defined events: HeroAbility_Reset, Summon_ActivateCommand
		// defined tasks: TASK_SUMMON_ENTITIES

		static inline constexpr int vtp = 0x773C10;
		static inline constexpr int TypeDesc = 0x817E0C;
		static inline constexpr unsigned int Identifier = 0x2BA19F1D;
		static inline BB::SerializationData* const SerializationData = reinterpret_cast<BB::SerializationData*>(0x86D4B8);


		virtual void AddHandlers(int id) override;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) override;
		virtual bool IsAbility(int ability) override;

	protected:
		void EventDie(BB::CEvent* ev);
		void EventActivate(BB::CEvent* ev);
		int TaskSummon(EGL::CGLETaskArgs* a);
	};
	class CSummonedBehavior : public EGL::CGLEBehavior {
	public:
		// defined events: Summoned_OnSummonerDetach

		static inline constexpr int vtp = 0x773BB8;
		static inline constexpr int TypeDesc = 0x817CD4;
		static inline constexpr unsigned int Identifier = 0x83CAFB3D;
	};

	class CConvertSettlerAbility : public GGL::CHeroAbility {
	public:
		PADDINGI(1);
		int TimeToConvert;

		// defined states: ComvertSettler
		// defined tasks: TASK_MOVE_TO_SETTLER_TO_CONVERT, TASK_CONVERT_SETTLER
		// defined events: ComvertSettler_XXX, OnAttackedBy, HeroAbility_Cancel

		static inline constexpr int vtp = 0x777294;
		static inline constexpr int TypeDesc = 0x8227D8;
		static inline constexpr unsigned int Identifier = 0x0E8276B7D;

		static void HookConvertEvent();
		void __thiscall PerformConversion();
	};

	class CConvertBuildingAbility : public GGL::CHeroAbility {
	public:
		PADDINGI(1);
		int TimeToConvert;

		// defined states: ConvertBuilding
		// defined tasks: TASK_MOVE_TO_BUILDING_TO_CONVERT, TASK_CONVERT_BUILDING
		// defined events: ConvertBuilding_XXX, OnAttackedBy

		static inline constexpr int vtp = 0x77739C;
		static inline constexpr int TypeDesc = 0x822B98;
		static inline constexpr unsigned int Identifier = 0x0FF68C96D;
	};

	class CSniperAbility : public GGL::CHeroAbility {
	public:
		PADDINGI(1);
		int TargetId; // 7

		// defined tasks: TASK_SET_SNIPE_ANIM, TASK_SNIPE, TASK_TURN_TO_SNIPER_TARGET
		// defined events: Sniper_XXX

		static inline constexpr int vtp = 0x7745AC;
		static inline constexpr int TypeDesc = 0x819044;
		static inline constexpr unsigned int Identifier = 0x0D167211D;

		static void OverrideSnipeTask();
		static int (*SnipeDamageOverride)(EGL::CGLEEntity* sniper, EGL::CGLEEntity* tar, int dmg);
	private:
		int __thiscall TaskOverrideSnipe(EGL::CGLETaskArgs* a);
	};

	class CMotivateWorkersAbility : public GGL::CHeroAbility {
	public:

		// defined tasks: TASK_SET_MOTIVATE_ANIM, TASK_PERFORM_MOTIVATION
		// defined events: MotivateVorkers_ActivateCommand

		static inline constexpr int vtp = 0x77574C;
		static inline constexpr int TypeDesc = 0x81C408;
		static inline constexpr unsigned int Identifier = 0x33A2FBFD;
	};

	class CShurikenAbility : public GGL::CHeroAbility {
	public:
		PADDINGI(1);
		int TargetId;

		// defined tasks: TASK_SET_THROW_SHURIKEN_ANIM, TASK_THROW_SHURIKEN, TASK_TURN_TO_SHURIKEN_TARGET
		// defined events: Shuriken_XXX

		static inline constexpr int vtp = 0x774658;
		static inline constexpr int TypeDesc = 0x819310;
		static inline constexpr unsigned int Identifier = 0x3A0D7DAD;
	};

	class CKegPlacerBehavior : public GGL::CHeroAbility {
	public:
		shok::Position StartPosition, TargetPosition;
		byte PlacedKeg; // 10
		PADDING(3);
		int TurnsUntilArmed, TurnsUntilDisarmed; // 11

		// defined states: ThiefSabotage, ThiefDisarm
		// defined tasks: TASK_PLACE_KEG, TASK_GO_TO_KEG_TARGET, TASK_GO_TO_KEG, TASK_CHECK_GO_TO_KEG_TARGET_SUCCESS, TASK_RETREAT_FROM_KEG_TARGET, TASK_DISARM_KEG
		// defined events: KegPlacer_XXX, HeroAbility_Cancel

		static inline constexpr int vtp = 0x776368;
		static inline constexpr int TypeDesc = 0x81F084;
		static inline constexpr unsigned int Identifier = 0x0BF115057;
	};
	class CKegBehavior : public EGL::CGLEBehavior {
	public:
		byte IsArmed;
		PADDING(3);
		int TimeToExplode; //5

		// defined events: Keg_XXX
		// defined states: Default

		static inline constexpr int vtp = 0x7764D8;
		static inline constexpr int TypeDesc = 0x81F6BC;
		static inline constexpr unsigned int Identifier = 0x14A85A47;

		void AdvancedDealDamage();
	};

	class CAbilityScoutBinocular : public GGL::CHeroAbility {
	public:
		PADDINGI(1);
		byte Active; // 7
		PADDING(3);
		shok::Position ExlorationPosition;

		// defined tasks: TASK_EXPLORE, TASK_TURN_TO_EXPLORE_POSITION
		// defined events: Binocular_XXX, OnAttackedBy, 11002

		static inline constexpr int vtp = 0x779218;
		static inline constexpr int TypeDesc = 0x829248;
		static inline constexpr unsigned int Identifier = 0x0E7661C1D;
	};

	class CTorchPlacerBehavior : public GGL::CHeroAbility {
	public:
		shok::Position StartPosition, TargetPosition; // 5
		byte PlacedTorch;
		PADDING(3);

		// defined tasks: TASK_GO_TO_TORCH_DESTINATION, TASK_PLACE_TORCH
		// defined events: HeroAbility_Cancel, TorchPlacer_PlaceTorch

		static inline constexpr int vtp = 0x773738;
		static inline constexpr int TypeDesc = 0x816E4C;
		static inline constexpr unsigned int Identifier = 0x0A5AB3F17;
	};
	class CTorchBehavior : public EGL::CGLEBehavior {
	public:
		int RemainingTurns, TotalTurns, InitialExplorationRange; //4

		// defined states: Default

		static inline constexpr int vtp = 0x773848;
		static inline constexpr int TypeDesc = 0x817174;
		static inline constexpr unsigned int Identifier = 0x0A27E4FF7;
	};

	class CPointToResourceBehavior : public GGL::CHeroAbility {
	public:

		// defined states: ScoutPointToRes (maybe unused)
		// defined tasks: TASK_POINT_TO_RESOURCE
		// defined events: HeroAbility_Cancel (null), PointToResources_Activate

		static inline constexpr int vtp = 0x774FB0;
		static inline constexpr int TypeDesc = 0x81BB84;
		static inline constexpr unsigned int Identifier = 0x0EC055D7;
	};

	class CThiefBehavior : public EGL::CGLEBehavior {
	public:
		int Amount, ResourceType, StolenFromPlayer, TimeToSteal; //4

		// defined tasks: TASK_GO_TO_TARGET_BUILDING, TASK_CHECK_GO_TO_TARGET_BUILDING_SUCCESS, TASK_GO_TO_SECURE_BUILDING, TASK_CHECK_GO_TO_SECURE_BUILDING_SUCCESS,
		//		TASK_STEAL_GOODS, TASK_SECURE_STOLEN_GOODS, TASK_SET_THIEF_MODEL, TASK_THIEF_IDLE
		// defined events: Thief_XXX, HeroAbility_Cancel
		// defined states: ThiefStealGoods

		static inline constexpr int vtp = 0x7739B0;
		static inline constexpr int TypeDesc = 0x8173DC;
		static inline constexpr unsigned int Identifier = 0x7AE64E17;
	};

	class CSentinelBehavior : public EGL::CGLEBehavior {
	public:
		PADDINGI(1);
		int Urgency; // 5 ???

		// defined events: Behavior_Tick, Sentinel_GetUrgency

		static inline constexpr int vtp = 0x774B6C;
		static inline constexpr int TypeDesc = 0x81ABC0;
		static inline constexpr unsigned int Identifier = 0x5F1FFAD;
	};

	class CGLBehaviorAnimationEx : public EGL::CBehaviorAnimation {
	public:
		int Animation, AnimCategory, SuspendedAnimation, StartTurn, Duration; // 4
		bool PlayBackwards; // 9
		PADDING(3);
		int TurnToWaitFor;
		float SpeedModifier; // 11
		PADDINGI(1); // 12 p to EGL::CBehaviorAnimation::CSlotAnimation
		PADDINGI(1); // p to props
		struct {
			PADDINGI(1);
			int TaskType; // 15
			int AnimID;
			int Category;
			byte PlayBackwards;
			PADDING(3);
		} AnimToRestore;
		int AnimSet; // 19

		// defined states: WaitForAnim
		// defined tasks: TASK_SET_ANIM (x2), TASK_WAIT_FOR_ANIM, TASK_RANDOM_WAIT_FOR_ANIM
		// defined events: Animation_XXX

		EGL::CGLEAnimSet* GetAnimSet();

		static inline constexpr int vtp = 0x776B64;
		static inline constexpr int TypeDesc = 0x820BE8;
		static inline constexpr unsigned int Identifier = 0x0A4947238;

		static void HookNonCancelableAnim();
	private:
		int TaskWaitForAnimNonCancelable(EGL::CGLETaskArgsThousandths* a);
		void __thiscall AddNonCancelableHandlers();
	};

	class CBehaviorWalkCommand : public EGL::CGLEBehavior {
	public:
		shok::Position WalkTarget;

		// defined tasks: TASK_WALK -> goes to ev 20003
		// defined events: MoveCommand_Move

		static inline constexpr int vtp = 0x7736A4;
		static inline constexpr int TypeDesc = 0x816A78;
		static inline constexpr unsigned int Identifier = 0x0E23732C7;
	};

	class CWorkerBehavior : public EGL::CGLEBehavior {
	public:
		int WorkTimeRemaining, TargetWorkTime; // 4
		float Motivation;
	private:
		int BehaviorProps2; //7
	public:
		int CycleIndex; // seems to be 0->working, 1->eating, 2->resting, 5->joining settlement
		int TimesWorked, TimesNoWork, TimesNoFood, TimesNoRest, TimesNoPay, JoblessSinceTurn;
		byte CouldConsumeResource, IsLeaving; // 15
		PADDING(2);
		PADDINGI(1);
		float CarriedResourceAmount; // 17

		// defined states: unknown 13,21
		// defined tasks: TASK_LEAVE_SETTLEMENT, TASK_GO_TO_POS, TASK_ADVANCE_IN_CYCLE, TASK_INCREASE_PLAYER_XXX,
		// 	   TASK_CHANGE_WORK_TIME_ABSOLUTE, TASK_CHANGE_WORK_TIME_RELATIVE, TASK_CHANGE_WORK_TIME_XXX, TASK_GO_TO_WORK_BUILDING,
		// 	   TASK_GO_TO_EAT_BUILDING, TASK_GO_TO_REST_BUILDING, TASK_GO_TO_LEAVE_BUILDING,
		// 	   TASK_CHECK_GO_TO_WORK_BUILDING_SUCCESS, TASK_CHECK_GO_TO_EAT_BUILDING_SUCCESS, TASK_CHECK_GO_TO_REST_BUILDING_SUCCESS, TASK_CHECK_GO_TO_VILLAGE_CENTER_SUCCESS,
		// 	   TASK_WORK_WAIT_UNTIL, TASK_EAT_WAIT, TASK_REST_WAIT, TASK_SET_ANIM_AT_WORKPLACE, TASK_SET_ANIM_AT_FARM, TASK_SET_ANIM_AT_RESIDENCE, TASK_SET_ANIM_AT_VILLAGE_CENTER,
		// 	   TASK_MINED_RESOURCE TASK_REFINE_RESOURCE TASK_DO_TRADE_STEP TASK_DO_RESEARCH_STEP (all the same),
		// 	   TASK_RETURN_TO_CYCLE, TASK_CHECK_FEAR, TASK_CREATE_CANNON, TASK_SET_CANNON_PROGRESS, TASK_DO_WORK_AT_FOUNDRY
		// 	   TASK_GO_TO_SUPPLIER, TASK_TAKE_FROM_STOCK, TASK_SET_CARRIER_MODEL, TASK_CHECK_GO_TO_SUPPLIER_SUCCESS, TASK_ACTIVATE_UVANIM,
		// 	   TASK_START_WORK_IF_AT_WORKPLACE, TASK_ACTIVATE_PARTICLE_EFFECT, TASK_DEACTIVATE_PARTICLE_EFFECT,TASK_GO_TO_NEAREST_NEUTRAL_BRIDGE, TASK_CHECK_GO_TO_BRIDGE_SUCCESS,
		// 	   TASK_INCREASE_BRIDGE_PROGRESS
		// defined events: Worker_XXX

		static inline constexpr int vtp = 0x772B30;
		static inline constexpr int TypeDesc = 0x813B1C;
		static inline constexpr unsigned int Identifier = 0x0DCDBAB9D;
	};

	class CBehaviorFollow : public EGL::CGLEBehavior {
	public:
		static inline constexpr int vtp = 0x776E40;
	};

	class CBattleBehavior : public GGL::CBehaviorFollow {
	public:
		float SuccessDistance, FailureDistance; // 4
		int TimeOutTime, StartTurn;
		shok::Position TargetPosition;
		byte StartFollowing, StopFollowing; // 10
		PADDING(2);
		int FollowStatus;
		GGL::CBattleBehaviorProps* BattleProps; // 12 p to behprops
		int LatestHitTurn;
		int LatestAttackerID, BattleStatus; // la15
		byte NoMoveNecessary, NormalRangeCheckNecessary;
		PADDING(2);
		shok::LeaderCommand Command;
		shok::Position AttackMoveTarget;
		byte Helping;
		PADDING(3);
		int MilliSecondsToWait, MSToPlayHitAnimation;
		byte HitPlayed; // 23
		PADDING(3);
		int LatestAttackTurn;

		// defined sates: Follow, MoveToTarget, BattleWait
		// defined events: Follow_XXX, Animation_UnSuspend (x2) (updates startturn), Battle_XXX, OnAttackedBy
		// defined tasks: TASK_MOVE_TO_TARGET, TASK_SET_ORIENTATION_TO_TARGET, TASK_SET_ATTACK_ANIM, TASK_HURT, TASK_CHECK_RANGE, TASK_FIRE_PROJECTILE, TASK_SET_BATTLE_IDLE_ANIM,
		//		TASK_BATTLE_WAIT_UNTIL, TASK_SET_LATEST_ATTACK_TURN, TASK_WAIT_FOR_LATEST_ATTACK, TASK_RESOLVE_BATTLE_COLLISION

		virtual void SetCurrentCommand(shok::LeaderCommand cmd) = 0; // only sets the command int, not the tasklist
		// 5 more virt funcs

		static inline constexpr int vtp = 0x77313C;
		static inline constexpr int TypeDesc = 0x815EEC;
		static inline constexpr unsigned int Identifier = 0x0C4F1C42D;

		float GetMaxRange() const;
		int GetDamage() const;
		int GetMaxRandomDamage() const;
		int GetRandomDamage(); // uses (*EGL::CGLEGameLogic::GlobalObj)->RNG
		int __thiscall GetDamageAgainst(EGL::CGLEEntity* target);
		EGL::CGLEEntity* GetTarget() const;
		float GetMissChance() const;
		bool CheckMiss(); // uses (*EGL::CGLEGameLogic::GlobalObj)->RNG
		float __thiscall GetMaxRangeBase() const;

		static void HookDamageOverride();
		static void HookRangeOverride();
	private:
		void __thiscall EventOverrideGetDamage(EGL::CEventGetValue_Int* ev);
		int __thiscall TaskOverrideFireProjctile(EGL::CGLETaskArgs* a);
	};

	class CLeaderBehavior : public GGL::CBattleBehavior {
	public:
		PADDINGI(2);
		int TroopHealthCurrent, TroopHealthPerSoldier; // 27
		shok::Position TerritoryCenter;
		float TerritoryCenterRange;
		int Experience; // 32
		shok::Vector<shok::Position> PatrolPoints;
		int NextPatrolPointIndex; // 37
		float DefendOrientation;
		int TrainingStartTurn;
		byte PatrolIndexCountingDown, UsingTargetOrientation, UsingTerritory; // 40
		PADDING(1);
		int SecondsSinceHPRefresh, NudgeCount, FormationType;
		shok::Position StartBattlePosition;
		int IndexOfFirstChaoticPosition; //46

		// defined states: Train, LeaderGetCloseTotarget
		// defined events: Leader_XXX, MoveCommand_Move, Behavior_Tick (regen), HeroAbility_Reset, IsLeader
		// defined tasks: TASK_SET_DEFAULT_REACTION_TYPE, TASK_GO_TO_POS, TASK_ACTIVATE_UVANIM, TASK_LEAVE_BARRACKS, TASK_SET_ANIM_AT_BARRACKS, TASK_TRAIN,
		//		TASK_GET_CLOSE_TO_TARGET, TASK_ATTACK, TASK_CHECK_MIN_RADIUS

		static inline constexpr int vtp = 0x7761E0;
		static inline constexpr int TypeDesc = 0x81EF80;
		static inline constexpr unsigned int Identifier = 0x6DACD5ED;

		int GetTroopHealth();
		int GetTroopHealthPerSoldier();
		void PerformRegeneration();

		static bool LeaderRegenRegenerateSoldiers;
		static void HookLeaderRegen();
	private:
		void __thiscall CheckRegen();
	};

	class CSoldierBehavior : public GGL::CBattleBehavior {
	public:

		// defined states: 1, 24
		// defined events: Leader_Hurt, Leader_GetAttackTarget (forwards to leader), HeroAbility_Reset, Soldier_XXX, IsSoldier
		// defined tasks: TASK_GO_TO_POS, TASK_LEAVE_BARRACKS

		static inline constexpr int vtp = 0x773CC8;
		static inline constexpr int TypeDesc = 0x817FC4;
		static inline constexpr unsigned int Identifier = 0x0B1DACA7D;
	};

	class CBattleSerfBehavior : public GGL::CLeaderBehavior {
	public:
		PADDINGI(1); // 47 p to props
		int JobMemoryResourceID, TimeBeforeChangeback;

		// defined events: BattleSerf_XXX, Behavior_Tick (ticks down time if >0, reverts back if 0), Serf_CommandTurnToBattleSerf,
		//		Serf_Construct?, Serf_ExtractResource?, OnResourceDoodadDetach?
		// defined tasks: TASK_GO_TO_MAIN_HOUSE, TASK_TURN_INTO_SERF

		static inline constexpr int vtp = 0x7788C4;
		static inline constexpr int TypeDesc = 0x826BB4;
		static inline constexpr unsigned int Identifier = 0x738D051D;
	};

	class CSerfBattleBehavior : public GGL::CBattleBehavior {
	public:
		// padding 2, shok_pos territory?

		// defined events: MoveCommand_Move, Leader_AttackEntity, Leader_GetAttackTarget, Behavior_Tick, OnAttackedBy, Leader_OnAttackCommandTargetDetach
		// defined tasks: TASK_SET_DEFAULT_REACTION_TYPE

		static inline constexpr int vtp = 0x774A98;
		static inline constexpr int TypeDesc = 0x81A4FC;
		static inline constexpr unsigned int Identifier = 0x0AAB009AD;
	};
	// GGL::CWorkerBattleBehavior unused

	class CAutoCannonBehavior : public EGL::CGLEBehavior {
	public:
		PADDINGI(1); //4
		GGL::CAutoCannonBehaviorProps* ACProps;
		int LatestHitTurn, LatestAttackerID, WaitMS, LastTargetCheckTurn; //6
		int ShotsLeft; //10
		float LastTurnOrientation;
		int LatestAttackTurn, MilliSecondsToWait;

		// defined tasks: TASK_SET_ORIENTATION_TO_TARGET, TASK_CHECK_RANGE, TASK_FIRE_PROJECTILE, TASK_BATTLE_WAIT_UNTIL, TASK_SET_LATEST_ATTACK_TURN, TASK_WAIT_FOR_LATEST_ATTACK
		// defined states: Rotate, BattleWaitUntilAutoCannon, BattleWait (TASK_WAIT_FOR_LATEST_ATTACK), Default
		// defined events: Leader_AttackEntity, Leader_OnAttackTargetDetached, Battle_GetDamageClass, Battle_GetLatestAttackerID, Battle_GetLatestHitTurn, GetDamage, OnAttackedBy,
		//		AutoCannon_XXX

		float GetMaxRange() const;
		int GetDamage() const;
		float __thiscall GetMaxRangeBase() const;

		static inline constexpr int vtp = 0x778CF0;
		static inline constexpr int TypeDesc = 0x8288A0;
		static inline constexpr unsigned int Identifier = 0x143C5EFD;

		static void HookDamageOverride();
		static void HookRangeOverride();
	private:
		int __thiscall TaskFireProjectileOverride(EGL::CGLETaskArgs* a);
		void __thiscall EventGetDamageOverride(EGL::CEventGetValue_Int* ev);
	};
	class CFoundationBehavior : public EGL::CGLEBehavior {
	public:

		// defined events: Leader_AttackEntity, Foundation_XXX, Building_OnUpgradeStart, Building_OnUpgradeCancel, OnBuilderDetaches

		static inline constexpr int vtp = 0x776CA0;
		static inline constexpr int TypeDesc = 0x820F44;
		static inline constexpr unsigned int Identifier = 0x0B68167D;
	};

	class CSerfBehavior : public EGL::CGLEBehavior {
	public:
		int ConstructionSiteSlotIndex; //4
		int ExtractionDelayCounter; //float?
		int LastResourceType, JobMemoryResourceID;
		int ResourceSlot;
		float ResourceAttachmentOffset; //9

		// defined tasks: TASK_GO_TO_MAIN_HOUSE, TASK_GO_TO_CONSTRUCTION_SITE, TASK_TURN_TO_CONSTRUCTION_SITE, TASK_CHANGE_ATTACHMENT_TO_CONSTRUCTION_SITE,
		//		TASK_GO_TO_CONSTRUCTION_SITE_SLOT, TASK_LEAVE_SETTLEMENT, TASK_ABANDON_CURRENT_JOB, TASK_GO_TO_RESOURCE, TASK_TURN_TO_RESOURCE, TASK_WAIT_EXTRACTION_DELAY
		//		TASK_EXTRACT_RESOURCE, TASK_GO_TO_RESOURCE_SLOT, TASK_TURN_INTO_BATTLE_SERF
		// defined events: Worker_WorkPlaceBuildingDestroyed, Serf_XXX, BattleSerf_CommandTurnToSerf, Leader_AttackEntity, OnResourceDoodadDetach, IsSerf
		// defined states: SerfSearchResource, 1

		static inline constexpr int vtp = 0x774874;
		static inline constexpr int TypeDesc = 0x819AFC;
		static inline constexpr unsigned int Identifier = 0x1A1A688D;
	};

	struct SSlotArgsLimitedAttachment {
	};
	class CLimitedAttachmentBehavior : public EGL::CGLEBehavior, public EGL::TSlot<GGL::SSlotArgsLimitedAttachment, 0x13C4316D> {
	public:
		class AttachmentInfo {
		public:
			int Limit = 0;
			bool IsActive = false;
			PADDING(3);
		};

		shok::Map<shok::AttachmentType, AttachmentInfo> AttachmentMap;

		// defined evets: LimitedAttachment_XXX

		static inline constexpr int vtp = 0x775E84;
		static inline constexpr int TypeDesc = 0x81D6DC;
		static inline constexpr unsigned int Identifier = 0x2776847;
	};
	static_assert(offsetof(CLimitedAttachmentBehavior, AttachmentMap) == 5 * 4);

	class CFormationBehavior : public EGL::CGLEBehavior {
	public:
		int AnimStartTurn, AnimDuration;

		// defined tasks: TASK_ASSUME_POSITION_IN_FORMATION, TASK_IDLE_IN_FORMATION
		// defined states: IdleInFormation, AssumePositionInFormation
		// defined events: Formation_AssumePosition

		static inline constexpr int vtp = 0x776D60;
		static inline constexpr int TypeDesc = 0x8212A4;
		static inline constexpr unsigned int Identifier = 0x0C9C36977;
	};

	class CCamperBehavior : public EGL::CGLEBehavior {
	public:

		// defined tasks: TASK_GO_TO_CAMP, TASK_LEAVE_CAMP

		static inline constexpr int vtp = 0x77777C;
		static inline constexpr int TypeDesc = 0x823720;
		static inline constexpr unsigned int Identifier = 0x1ADA8097;
	};
	class CCampBehavior : public EGL::CGLEBehavior {
	public:
		PADDINGI(1);
		shok::Vector<shok::Position> Slot; // not sure if it is positions in here
		int NumTurnsToDeletion;

		// defined states: Default

		static inline constexpr int vtp = 0x777864;
		static inline constexpr int TypeDesc = 0x823C48;
		static inline constexpr unsigned int Identifier = 0x671CDCE7;
	};

	class CGLBehaviorDying : public EGL::CGLEBehavior {
	public:

		// defined tasks: TASK_DELETE_SELF, TASK_CREATE_ENTITY, TASK_SPAWN_PLAYER_EFFECT

		static inline constexpr int vtp = 0x7785E4;
		static inline constexpr int TypeDesc = 0x825F14;
		static inline constexpr unsigned int Identifier = 0x0C035C4ED;
	};

	class CHeroBehavior : public EGL::CGLEBehavior {
	public:
		PADDINGI(1);
		int ResurrectionTimePassed, SpawnTurn; // 5
		byte FriendNear, EnemyNear;
		PADDING(2);

		// defined events: Die, HeroBehavior_XXX
		// defined tasks: TASK_BECOME_COMATOSE, TASK_GO_TO_NPC, TASK_INTERACT_WITH_NPC
		// defined tasks: HeroGoToNPC, HeroComatose

		static inline constexpr int vtp = 0x77677C;
		static inline constexpr int TypeDesc = 0x820070;
		static inline constexpr unsigned int Identifier = 0x0FCB04BAD;
	};

	class CResourceRefinerBehavior : public EGL::CGLEBehavior {
	public:

		// defined evetns: ResourceRefiner_XXX, Worker_GetResourceToRefine

		static inline constexpr int vtp = 0x774BCC;
		static inline constexpr int TypeDesc = 0x81AD80;
		static inline constexpr unsigned int Identifier = 0x340C4B57;
	};

	class CAffectMotivationBehavior : public EGL::CGLEBehavior {
	public:
		PADDINGI(1); //p to props
		int PlayerID; //5
		byte MotivationAffected;
		PADDING(3);

		// defined events: AffectMotivation_XXX

		static inline constexpr int vtp = 0x77918C;
		static inline constexpr int TypeDesc = 0x829024;
		static inline constexpr unsigned int Identifier = 0x6280C00D;
	};

	class CLimitedLifespanBehavior : public EGL::CGLEBehavior {
	public:
		PADDINGI(1);
		int RemainingLifespanSeconds;

		// defined events: Behavior_Tick, LimitedLifespan_XXX

		static inline constexpr int vtp = 0x775D9C;
		static inline constexpr int TypeDesc = 0x81D0B0;
		static inline constexpr unsigned int Identifier = 0x66243B3D;
	};

	class CBarrackBehavior : public EGL::CGLEBehavior {
	public:
		byte AutoFillActive; //4
		PADDING(3);

		// defined events: Barracks_XXX

		static inline constexpr int vtp = 0x778A68;
		static inline constexpr int TypeDesc = 0x8278DC;
		static inline constexpr unsigned int Identifier = 0x0FE724607;
	};

	class CBuildingMerchantBehavior : public EGL::CGLEBehavior {
	public:
		class COffer : public BB::IObject {
		public:
			shok::CostInfo ResourceCosts;
			int OffersRemaining;

			static inline constexpr int vtp = 0x7781B4;
			static inline constexpr int TypeDesc = 0x824BC8;
			static inline constexpr unsigned int Identifier = 0x1079FB13;
		};

		int CurrentPlayer; //4
		int TraderEntityID;
		shok::Vector<GGL::CBuildingMerchantBehavior::COffer*> Offer;

		// defined events: Behavior_Tick, BuildingMerchant_XXX

		static inline constexpr int vtp = 0x778208;
		static inline constexpr int TypeDesc = 0x824DFC;
		static inline constexpr unsigned int Identifier = 0x8D87CCB3;
	};
	class CBuildingMercenaryBehavior : public GGL::CBuildingMerchantBehavior {
	public:
		class CTechOffer : public GGL::CBuildingMerchantBehavior::COffer {
		public:
			int OfferedTechnologyType;

			static inline constexpr int vtp = 0x7781C4;
			static inline constexpr int TypeDesc = 0x824BFC;
			static inline constexpr unsigned int Identifier = 0x0D9705E63;
		};

		// defined events: BuildingMercenary_XXX

		static inline constexpr int vtp = 0x7782C0;
		static inline constexpr int TypeDesc = 0x8250E4;
		static inline constexpr unsigned int Identifier = 0x2B61C163;
	};
	class CBuildingTechTraderBehavior : public GGL::CBuildingMerchantBehavior {
	public:
		class CMercenaryOffer : public GGL::CBuildingMerchantBehavior::COffer {
		public:
			int OfferedEntityType;

			static inline constexpr int vtp = 0x778284;
			static inline constexpr int TypeDesc = 0x824ED8;
			static inline constexpr unsigned int Identifier = 0x339B693;
		};

		// defined events: BuildingTechTrader_XXX, BuildingMercenary_BuyOffer

		static inline constexpr int vtp = 0x77822C;
		static inline constexpr int TypeDesc = 0x824E28;
		static inline constexpr unsigned int Identifier = 0x41EEB893;
	};
	class CSettlerMerchantBehavior : public EGL::CGLEBehavior {
	public:

		// defined tasks: TASK_GO_TO_POS, TASK_TRADER_OFFER_POS_REACHED
		// defined states: Merchant_WaitForHero
		// defined events: SettlerMerchant_XXX

		static inline constexpr int vtp = 0x77477C;
		static inline constexpr int TypeDesc = 0x819710;
		static inline constexpr unsigned int Identifier = 0x0AB50F013;
	};

	class CMarketBehavior : public EGL::CGLEBehavior {
	public:
		GGL::CTradeManager::TradeOrder CurrentTrade; // 4

		// defined events: Market_XXX

		static inline constexpr int vtp = 0x775CCC;
		static inline constexpr int TypeDesc = 0x81CE24;
		static inline constexpr unsigned int Identifier = 0x90A53E97;
	};

	class CFoundryBehavior : public EGL::CGLEBehavior {
	public:
		int CannonType, CannonProgress; //4

		// defined events: Foundry_XXX

		static inline constexpr int vtp = 0x778A8C;
		static inline constexpr int TypeDesc = 0x827900;
		static inline constexpr unsigned int Identifier = 0x8EB5DC97;
	};

	class CWorkerFleeBehavior : public EGL::CGLEBehavior {
	public:
		shok::Position FlightPosition; //4
		// 6 p to props

		// defined tasks: TASK_FLEE
		// defined events: WorkerFlee_XXX, OnAttackedBy

		static inline constexpr int vtp = 0x7729FC;
		static inline constexpr int TypeDesc = 0x813798;
		static inline constexpr unsigned int Identifier = 0x83A6BCAD;
	};

	class CWorkerAlarmModeBehavior : public EGL::CGLEBehavior {
	public:

		// defined tasks: TASK_CHANGE_DEFENDABLE_BUILDING_ATTACHMENT, TASK_MOVE_TO_DEFENDABLE_BUILDING, TASK_DEFEND, TASK_CHECK_GO_TO_DEFENDABLE_BUILDING_SUCCESS
		// defined events: WorkerAlarmMode_XXX

		static inline constexpr int vtp = 0x7734DC;
		static inline constexpr int TypeDesc = 0x8164E4;
		static inline constexpr unsigned int Identifier = 0x8076034D;
	};

	class CDefendableBuildingBehavior : public EGL::CGLEBehavior {
	public:
		int RemainderMS;
		shok::TaskState OldState;

		// defined events: WorkerAlarmMode_Enable, WorkerAlarmMode_Disable, DefendableBuilding_XXX
		// defined states: BuildingAlarmDefend

		static inline constexpr int vtp = 0x7771DC;
		static inline constexpr int TypeDesc = 0x822428;
		static inline constexpr unsigned int Identifier = 0x0B663EA8D;
	};

	class CUniversityBehavior : public EGL::CGLEBehavior {
	public:

		// defined events: University_XXX

		static inline constexpr int vtp = 0x7736F4;
		static inline constexpr int TypeDesc = 0x816C74;
		static inline constexpr unsigned int Identifier = 0x5F780A67;
	};

	class CResourceDoodadBehavior : public EGL::CGLEBehavior {
	public:
		// defined events: ResourceDoodad_RemoveOneRes

		static inline constexpr int vtp = 0x774CCC;
		static inline constexpr int TypeDesc = 0x81B1A4;
		static inline constexpr unsigned int Identifier = 0x707CD967;
	};

	class CTreeBehavior : public EGL::CGLEBehavior {
	public:
		int OriginalEntityType; // 4
		shok::Position Position;
		float Orientation;

		// defined events: Tree_XXX

		static inline constexpr int vtp = 0x774CF0;
		static inline constexpr int TypeDesc = 0x81B1D0;
		static inline constexpr unsigned int Identifier = 0x74CA1CA7;
	};

	class CResourceDependentBuildingBehavior : public EGL::CGLEBehavior {
	public:

		// defined states: Default

		static inline constexpr int vtp = 0x774D94;
		static inline constexpr int TypeDesc = 0x81B478;
		static inline constexpr unsigned int Identifier = 0x4267D337;
	};

	class CReplaceableEntityBehavior : public EGL::CGLEBehavior {
	public:
		byte IsReplacementActive; // 4
		PADDING(3);

		// defined events: ReplaceableEntity_Disable

		static inline constexpr int vtp = 0x774DEC;
		static inline constexpr int TypeDesc = 0x081B61C;
		static inline constexpr unsigned int Identifier = 0x981BE357;
	};

	class CMineBehavior : public EGL::CGLEBehavior {
	public:

		// defined events: Mine_XXX, OnResourceDoodadDetach

		static inline constexpr int vtp = 0x7757CC;
		static inline constexpr int TypeDesc = 0x81C5EC;
		static inline constexpr unsigned int Identifier = 0x0DE005127;
	};

	class CKeepBehavior : public EGL::CGLEBehavior {
	public:

		// defined events: Keep_BuySerfCommand

		static inline constexpr int vtp = 0x7765C8;
		static inline constexpr int TypeDesc = 0x81FA60;
		static inline constexpr unsigned int Identifier = 0x995E7AF7;
	};

	class CFarmBehavior : public EGL::CGLEBehavior {
	public:

		// defined events: WorkPlace_OnWorkerAttach, WorkPlace_OnWorkerDetach

		static inline constexpr int vtp = 0x776EBC;
		static inline constexpr int TypeDesc = 0x821970;
		static inline constexpr unsigned int Identifier = 0x6EF4A627;
	};

	class CConstructionSiteBehavior : public EGL::CGLEBehavior {
	public:

		// defined events: WorkPlace_OnWorkerAttach, WorkPlace_OnWorkerDetach

		static inline constexpr int vtp = 0x777424;
		static inline constexpr int TypeDesc = 0x822EC4;
		static inline constexpr unsigned int Identifier = 0x0E69FE40;
	};

	class CBuildingBehavior : public EGL::CGLEBehavior {
	public:
		// 5 p to GGL::CBuildingBehavior::CSlotBuilding ?

		// defined states: Default, ?
		// defined events: Die

		static inline constexpr int vtp = 0x777FBC;
		static inline constexpr int TypeDesc = 0x8247AC;
		static inline constexpr unsigned int Identifier = 0x0F3538D37;
	};

	class CBarrierBehavior : public EGL::CGLEBehavior {
	public:

		// defined states: Default, ?

		static inline constexpr int vtp = 0x778A10;
		static inline constexpr int TypeDesc = 0x8277DC;
		static inline constexpr unsigned int Identifier = 0x310288C7;
	};

	class CNeutralBridgeBehavior : public EGL::CGLEBehavior {
	public:
		PADDINGI(9); //Progress
		PADDINGI(3); //SlotShoreA
		PADDINGI(3); //SlotShoreB

		static inline constexpr int vtp = 0x779BC4;
		static inline constexpr int TypeDesc = 0x829AA8;
		static inline constexpr unsigned int Identifier = 0x0D26CD737;
	};


	// these belong to serf behavior
	class CPositionAtResourceFinder {
	public:

		static GGL::CPositionAtResourceFinder* CreateByEntity(int entityid);
		shok::Position CalcPositionFromFloat(float f);

		virtual ~CPositionAtResourceFinder() = default;
		virtual float SearchForPosition(EGL::CGLEEntity* e) = 0;
	private:
		virtual void GetPositionOffset(shok::Position* out, float f) = 0;
	public:

		static inline constexpr int vtp = 0x7729AC;
		static inline constexpr int TypeDesc = 0x8135B4;
	};
	class CPositionAtCircularResourceFinder : public GGL::CPositionAtResourceFinder {
	public:

		static inline constexpr int vtp = 0x7729BC;
		static inline constexpr int TypeDesc = 0x8135E0;
	};
	class CPositionAtLinearResourceFinder : public GGL::CPositionAtResourceFinder {
	public:

		static inline constexpr int vtp = 0x7729CC;
		static inline constexpr int TypeDesc = 0x813614;
	};


}


// GGL::CEvadeBehaviorBase
//GGL::CEvadeExecutionBehavior
//GGL::CBehaviorFieldDoodad unused
//GGL::CStaticCamouflageSlot unused
//GGL::CAlphaBlendingBehavior blendingfog todo
//EGL::CUVAnimBehavior
//EGL::CParticleEffectSwitchBehavior
//EGL::CParticleEffectAttachmentBehavior
