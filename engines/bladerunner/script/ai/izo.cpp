/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "bladerunner/script/ai_script.h"

namespace BladeRunner {

AIScriptIzo::AIScriptIzo(BladeRunnerEngine *vm) : AIScriptBase(vm) {
	_resumeIdleAfterFramesetCompletesFlag = false;
	_var1 = 6;
	_var2 = 1;
	_varNumOfTimesToHoldCurrentFrame = 0;
	// _varChooseIdleAnimation can have valid values: 0, 1
	_varChooseIdleAnimation = 0;
}

void AIScriptIzo::Initialize() {
	_animationFrame = 0;
	_animationState = 0;
	_animationStateNext = 0;
	_animationNext = 0;

	_resumeIdleAfterFramesetCompletesFlag = false;
	_var1 = 6;
	_var2 = 1;
	_varNumOfTimesToHoldCurrentFrame = 0;
	_varChooseIdleAnimation = 0;

	Actor_Set_Goal_Number(kActorIzo, 0);
	Actor_Put_In_Set(kActorIzo, kSetHC01_HC02_HC03_HC04);
	Actor_Set_At_XYZ(kActorIzo, 591.0f, 0.14f, 25.0f, 540);
	World_Waypoint_Set(349, kSetRC03, -14.7f, -4.01f, 224.5f);
}

bool AIScriptIzo::Update() {
	if (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoStopRunning
	 && Player_Query_Current_Scene() == kSceneUG02
	) {
		Actor_Set_Targetable(kActorIzo, true);
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoRunToRC03);
		return true;
	}

	if (Global_Variable_Query(kVariableChapter) == 3
	 && Actor_Query_Goal_Number(kActorIzo) == kGoalIzoGone
	 && Actor_Query_Which_Set_In(kActorIzo) == kSetRC03
	) {
		Actor_Put_In_Set(kActorIzo, kSetFreeSlotI);
		Actor_Set_At_Waypoint(kActorIzo, 41, 0);
	}

	if (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoDefault
	 && Player_Query_Current_Scene() == kSceneTB02
	) {
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoGoToHC03);
	}

	if (Global_Variable_Query(kVariableChapter) == 4
	 && Actor_Query_Goal_Number(kActorIzo) < kGoalIzoGone
	 && Actor_Query_Goal_Number(kActorIzo) < 300
	 && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoGotArrested
	) {
		Actor_Set_Goal_Number(kActorIzo, 300);
	}

	if (Global_Variable_Query(kVariableChapter) == 5
	 && Actor_Query_Goal_Number(kActorIzo) < 400
	) {
		Actor_Set_Goal_Number(kActorIzo, 400);
	}

	return false;
}

void AIScriptIzo::TimerExpired(int timer) {
	//return false;
}

void AIScriptIzo::CompletedMovementTrack() {
	switch (Actor_Query_Goal_Number(kActorIzo)) {
	case kGoalIzoRunToUG02:
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoRunToRC03);
		Player_Gains_Control();
		return; //true;

	case kGoalIzoRunToRC03:
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoWaitingAtRC03);
		return; //true;

	case kGoalIzoRC03Walk:
		// fall through
	case kGoalIzoRC03Run:
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoRC03RunAway);
#if BLADERUNNER_ORIGINAL_BUGS
		Actor_Set_Goal_Number(kActorSteele, kGoalSteeleApprehendIzo);
		// causes unwanted repetition of the apprehend
		// also may cause a freeze if McCoy exits the scene
		Scene_Exits_Enable();
#else
		// prevent re-apprehending of Izo
		if (Actor_Query_Goal_Number(kActorIzo) != kGoalIzoGetArrested
		    && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoGotArrested
		    && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoRC03RanAwayDone
		    && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoDie
		    && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoDieHidden
		    && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoEscape
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleApprehendIzo
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleArrestIzo
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleIzoBlockedByMcCoy
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleShootIzo
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleLeaveRC03
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleGoToPoliceStation
		) {
			Actor_Set_Goal_Number(kActorSteele, kGoalSteeleApprehendIzo);
		}
#endif // BLADERUNNER_ORIGINAL_BUGS
		return; //true;

	case kGoalIzoRC03RunAway:
#if BLADERUNNER_ORIGINAL_BUGS
		// Enabling exits here will cause in some cases
		// McCoy to be able to exit the scene if player clicks fast
		// in which case the apprehending of Izo will take place off-screen
		// and the player will listen to it as if it happened in the current scene
		Scene_Exits_Enable();
#else
		// In some occasions Izo will be block and won't reach exactly the 174 waypoint
		// but he'll stand still at a distance from it (this would still trigger the CompletedMovementTrack() )
		// In this case, Izo would remain stuck standing still in RC03 forever (in Act 2)
		// This makes him teleport elsewhere (behavior similar to when he's arrested)
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoRC03RanAwayDone);
#endif // BLADERUNNER_ORIGINAL_BUGS
		return; //true;

	case kGoalIzoGetArrested:
		Game_Flag_Set(kFlagIzoArrested);
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoGotArrested);
		Scene_Exits_Enable();
		return; //true;

	case kGoalIzoGoToHC01:
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoGoToHC03);
		return; //true;

	case kGoalIzoGoToHC03:
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoGoToHC01);
		return; //true;

	case 300:
		Actor_Set_Goal_Number(kActorIzo, 301);
		return; //true;

	case 301:
		Actor_Set_Goal_Number(kActorIzo, 300);
		return; //true;
	}

	return; //false;
}

void AIScriptIzo::ReceivedClue(int clueId, int fromActorId) {
	//return false;
}

void AIScriptIzo::ClickedByPlayer() {
	if (Actor_Query_Goal_Number(kActorIzo) > 500) { // Dead
		Actor_Face_Actor(kActorMcCoy, kActorIzo, true);
		Actor_Says(kActorMcCoy, 8585, 13);
		return; //true;
	}

	if (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoRunToRC03
	 && Player_Query_Current_Set() == kSetUG02
	) {
		Player_Loses_Control();
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoStopRunning);
		Actor_Face_Actor(kActorMcCoy, kActorIzo, true);
		Actor_Says(kActorMcCoy, 5460, 16);
		Actor_Face_Actor(kActorIzo, kActorMcCoy, true);
		Actor_Says(kActorIzo, 700, 17);
		Actor_Says(kActorMcCoy, 5465, 14);
		dialogueWithIzo();
	}

	if (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoRC03Walk) {
		Actor_Face_Actor(kActorMcCoy, kActorIzo, true);
		Actor_Says(kActorMcCoy, 2715, 14);
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoRC03Run);
		Actor_Says(kActorMcCoy, 1800, 14);
#if BLADERUNNER_ORIGINAL_BUGS
		Actor_Set_Goal_Number(kActorSteele, kGoalSteeleApprehendIzo);
#else
		// prevent re-apprehending of Izo
		if (Actor_Query_Goal_Number(kActorIzo) != kGoalIzoGetArrested
		    && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoGotArrested
		    && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoRC03RanAwayDone
		    && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoDie
		    && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoDieHidden
		    && Actor_Query_Goal_Number(kActorIzo) != kGoalIzoEscape
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleApprehendIzo
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleArrestIzo
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleIzoBlockedByMcCoy
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleShootIzo
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleLeaveRC03
		    && Actor_Query_Goal_Number(kActorSteele) != kGoalSteeleGoToPoliceStation
		) {
			Actor_Set_Goal_Number(kActorSteele, kGoalSteeleApprehendIzo);
		}
#endif // BLADERUNNER_ORIGINAL_BUGS
		return; //true;
	}

	return; //false;
}

void AIScriptIzo::EnteredSet(int setId) {
	// return false;
}

void AIScriptIzo::OtherAgentEnteredThisSet(int otherActorId) {
	// return false;
}

void AIScriptIzo::OtherAgentExitedThisSet(int otherActorId) {
	// return false;
}

void AIScriptIzo::OtherAgentEnteredCombatMode(int otherActorId, int combatMode) {
	if (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoRC03Walk) {
		Game_Flag_Query(kFlagIzoIsReplicant); // bug in the game?
	}
	return; //false;
}

void AIScriptIzo::ShotAtAndMissed() {
	if (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoRC03Walk) {
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoRC03Run);
		return; //true;
	}

	return; //false;
}

bool AIScriptIzo::ShotAtAndHit() {
	if (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoRC03Walk
	 || Actor_Query_Goal_Number(kActorIzo) == kGoalIzoRC03Run
	 || Actor_Query_Goal_Number(kActorIzo) == kGoalIzoRC03RunAway
	) {
		Actor_Set_Health(kActorIzo, 50, 50);

		if (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoRC03Walk) {
			Actor_Set_Goal_Number(kActorIzo, kGoalIzoRC03Run);
		}
		return true;
	} else {
		AI_Movement_Track_Flush(kActorIzo);
		Global_Variable_Increment(kVariableIzoShot, 1);
		if (!Game_Flag_Query(kFlagIzoShot)
		 &&  Global_Variable_Query(kVariableIzoShot) == 1
		) {
			Game_Flag_Set(kFlagIzoShot);
			_animationFrame = 0;
			_animationState = 19;
			Actor_Retired_Here(kActorIzo, 36, 12, true, -1);
			Actor_Set_Goal_Number(kActorIzo, kGoalIzoDie);
		}
		return false;
	}
}

void AIScriptIzo::Retired(int byActorId) {
	if (!Actor_Query_In_Set(kActorIzo, kSetKP07)) {
		return; //false;
	}

	Global_Variable_Decrement(kVariableReplicantsSurvivorsAtMoonbus, 1);
	Actor_Set_Goal_Number(kActorIzo, kGoalIzoGone);

	if (Global_Variable_Query(kVariableReplicantsSurvivorsAtMoonbus) == 0) {
		Player_Loses_Control();
		Delay(2000);
		Player_Set_Combat_Mode(false);
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -12.0f, -41.58f, 72.0f, 0, true, false, false);
		Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
		Ambient_Sounds_Remove_All_Looping_Sounds(1u);
		Game_Flag_Set(kFlagKP07toKP06);
		Game_Flag_Reset(kFlagMcCoyIsHelpingReplicants);
		Set_Enter(kSetKP05_KP06, kSceneKP06);
		return; //true;
	}

	return; //false;
}

int AIScriptIzo::GetFriendlinessModifierIfGetsClue(int otherActorId, int clueId) {
	return 0;
}

bool AIScriptIzo::GoalChanged(int currentGoalNumber, int newGoalNumber) {
	if (newGoalNumber == kGoalIzoEscapedSteeleKnows
	 || newGoalNumber == kGoalIzoDie
	 || newGoalNumber == kGoalIzoDieHidden
	 || newGoalNumber == kGoalIzoGotArrested
	 || newGoalNumber == kGoalIzoRC03RanAwayDone
	 || newGoalNumber == kGoalIzoEscape
	) {
		Spinner_Set_Selectable_Destination_Flag(kSpinnerDestinationDNARow, true);
	}

	switch (newGoalNumber) {
	case kGoalIzoDefault:
		AI_Movement_Track_Flush(kActorIzo);
		return true;

	case kGoalIzoPrepareCamera:
		AI_Movement_Track_Flush(kActorIzo);
		Actor_Face_Heading(kActorIzo, 520, false);
		_animationState = 32;
		_animationFrame = -1;
		return true;

	case kGoalIzoTakePhoto:
		_animationState = 34;
		_animationFrame = -1;
		return true;

	case kGoalIzoRunToUG02:
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append_Run(kActorIzo, 149, 0);
		AI_Movement_Track_Append_Run(kActorIzo, 152, 0);
		AI_Movement_Track_Repeat(kActorIzo);
		Scene_Exit_Add_2D_Exit(1, 394, 229, 485, 371, 1);
		Game_Flag_Set(kFlagHC03Available);
		return true;

	case kGoalIzoStopRunning:
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case kGoalIzoRunToRC03:
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append_Run(kActorIzo, 153, 0); // kSetUG02
		AI_Movement_Track_Append(kActorIzo, 154, 0);     // kSetRC03
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case kGoalIzoWaitingAtRC03:
		return true;

	case kGoalIzoEscape:
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append(kActorIzo, 153, 0);  // kSetUG02
		AI_Movement_Track_Append(kActorIzo, 39, 120); // kSetFreeSlotG
		AI_Movement_Track_Append(kActorIzo, 33, 0);   // kSetFreeSlotA
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case kGoalIzoRC03Walk:
		Game_Flag_Set(kFlagDNARowAvailable);
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append(kActorIzo, 349, 0); // kSetRC03
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case kGoalIzoRC03Run:
		Game_Flag_Set(kFlagDNARowAvailable);
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append_Run(kActorIzo, 349, 0); // kSetRC03
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case kGoalIzoRC03RunAway:
		Game_Flag_Set(kFlagDNARowAvailable);
		AI_Movement_Track_Flush(kActorIzo);
#if BLADERUNNER_ORIGINAL_BUGS
		AI_Movement_Track_Append_Run(kActorIzo, 174, 0); // kSetRC03
#else
		// set a waypoint near 174 but not exactly same coordinates
		// to make colliding with Steele or McCoy less likely
		World_Waypoint_Set(553, kSetRC03, 346.96f, -4.01f, 419.16f);

		// prevent Izo from getting stuck in-scene RC03
		// Note: this alone won't work as a fix;
		// it still requires the additional fix in CompletedMovementTrack()
		AI_Movement_Track_Append_Run(kActorIzo, 553, 0); // kSetRC03
		AI_Movement_Track_Append_Run(kActorIzo, 33, 0);  // kSetFreeSlotA
#endif // BLADERUNNER_ORIGINAL_BUGS
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case 115:
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append(kActorIzo, 39, 60);     // kSetFreeSlotG
		if (Game_Flag_Query(kFlagIzoIsReplicant)) {
			AI_Movement_Track_Append(kActorIzo, 33, 0);  // kSetFreeSlotA
		} else {
			AI_Movement_Track_Append(kActorIzo, 34, 0);  // kSetFreeSlotB
		}
		AI_Movement_Track_Repeat(kActorIzo);
		Game_Flag_Set(kFlagDNARowAvailable);
		return true;

	case kGoalIzoGetArrested:
		Actor_Set_Targetable(kActorIzo, false);
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append(kActorIzo, 174, 0); // kSetRC03
		AI_Movement_Track_Append(kActorIzo, 33, 0);  // kSetFreeSlotA
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case kGoalIzoGoToHC01:
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append(kActorIzo, 39, Random_Query(15, 30));   // kSetFreeSlotG
		AI_Movement_Track_Append(kActorIzo, 149, 0);                     // kSetHC01_HC02_HC03_HC04
		AI_Movement_Track_Append(kActorIzo, 323, Random_Query(90, 120)); // kSetHC01_HC02_HC03_HC04
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case kGoalIzoGoToHC03:
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append(kActorIzo, 149, 0);                       // kSetHC01_HC02_HC03_HC04
		if (Game_Flag_Query(kFlagIzoIsReplicant)) {
			AI_Movement_Track_Append(kActorIzo, 39, 5);                    // kSetFreeSlotG
			AI_Movement_Track_Append(kActorIzo, 34, Random_Query(10, 20)); // kSetFreeSlotB
			AI_Movement_Track_Append(kActorIzo, 39, 5);                    // kSetFreeSlotG
			AI_Movement_Track_Append(kActorIzo, 33, Random_Query(10, 20)); // kSetFreeSlotA
		} else {
			AI_Movement_Track_Append(kActorIzo, 39, Random_Query(5, 15));  // kSetFreeSlotG
			AI_Movement_Track_Append(kActorIzo, 34, Random_Query(20, 40)); // kSetFreeSlotB
		}
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case 160:
		AI_Movement_Track_Flush(kActorIzo);
		return true;

	case kGoalIzoRC03RanAwayDone:
		// fall through
	case kGoalIzoGotArrested:
		Game_Flag_Set(kFlagDNARowAvailable);
		Actor_Put_In_Set(kActorIzo, kSetFreeSlotA);
		Actor_Set_At_Waypoint(kActorIzo, 33, 0); // kSetFreeSlotA
		return true;

	case kGoalIzoDieHidden:
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append(kActorIzo, 41, 0); // kSetFreeSlotI
		AI_Movement_Track_Repeat(kActorIzo);
		Actor_Set_Goal_Number(kActorIzo, 999);
		return true;

	case kGoalIzoDie:
		Game_Flag_Set(kFlagDNARowAvailable);
		AI_Movement_Track_Flush(kActorIzo);
		Ambient_Sounds_Play_Speech_Sound(kActorIzo, 9000, 100, 0, 0, 0);
		Actor_Change_Animation_Mode(kActorIzo, kAnimationModeDie);
		Actor_Set_Goal_Number(kActorIzo, 999);
#if BLADERUNNER_ORIGINAL_BUGS
		Scene_Exits_Enable();
#else
		Actor_Set_Targetable(kActorIzo, false);
		if (!Actor_Query_In_Set(kActorIzo, kSetKP07)) {
			Scene_Exits_Enable();
			Actor_Retired_Here(kActorIzo, 36, 12, true, -1);
		}
#endif // BLADERUNNER_ORIGINAL_BUGS
		return true;

	case kGoalIzoEscapedSteeleKnows:
		Game_Flag_Set(kFlagDNARowAvailable);
		return true;

	case 300:
		AI_Movement_Track_Flush(kActorIzo);
		modifyWaypoints();
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case 301:
		AI_Movement_Track_Flush(kActorIzo);
		AI_Movement_Track_Append(kActorIzo, 34, 1); // kSetFreeSlotB
		AI_Movement_Track_Repeat(kActorIzo);
		return true;

	case 400:
		AI_Movement_Track_Flush(kActorIzo);
		Actor_Put_In_Set(kActorIzo, kSetFreeSlotA);
		Actor_Set_At_Waypoint(kActorIzo, 33, 0); // kSetFreeSlotA
		return true;

	case 999:
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoGone);
		return true;

	case 9999:
		return true;

	}

	return false;
}

bool AIScriptIzo::UpdateAnimation(int *animation, int *frame) {
	switch (_animationState) {
	case 0:
		if (_varChooseIdleAnimation == 1) {
			*animation = kModelAnimationIzoAwkwardPlayWithHands;
			++_animationFrame;
			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoAwkwardPlayWithHands)) {
				_animationFrame = 0;
				_varChooseIdleAnimation = 0;
			}
		} else if (_varChooseIdleAnimation == 0) {
			*animation = kModelAnimationIzoIdle;
			if (_varNumOfTimesToHoldCurrentFrame > 0) {
				--_varNumOfTimesToHoldCurrentFrame;
			} else {
				_animationFrame += _var2;
				if (_animationFrame < 0) {
					_animationFrame = Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoIdle) - 1;
				} else if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoIdle)) {
					_animationFrame = 0;
				}
				--_var1;
				if (_var1 == 0) {
					_var2 = 2 * Random_Query(0, 1) - 1;
					_var1 = Random_Query(6, 14);
					_varNumOfTimesToHoldCurrentFrame = Random_Query(0, 2);
				}
				if (_animationFrame == 0) {
					if (!Random_Query(0, 5)) {
						_varChooseIdleAnimation = 1;
					}
				}
			}
		}
		break;

	case 1:
		if (_animationFrame <= 2 && _resumeIdleAfterFramesetCompletesFlag) {
			*animation = kModelAnimationIzoIdle;
			_animationFrame = 0;
			_animationState = 0;
			_resumeIdleAfterFramesetCompletesFlag = false;
		} else {
			*animation = kModelAnimationIzoCalmTalk;
			++_animationFrame;
			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoCalmTalk)) {
				_animationFrame = 0;
			}
		}
		break;

	case 2:
		// fall through
	case 3:
		// fall through
	case 4:
		// fall through
	case 5:
		// fall through
	case 6:
		// fall through
	case 7:
		// TODO why calculate current animation by adding animationState to kModelAnimationIzoAwkwardPlayWithHands (298)?
		//      seems prone to error.
		//      This (based on the switch cases), results in "talking" animation framesets 300 - 305,
		//      excepting the kModelAnimationIzoCalmTalk (299) which is used as the default or "ending" talking animation.
		*animation = _animationState + kModelAnimationIzoAwkwardPlayWithHands;
		++_animationFrame;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 1;
			*animation = kModelAnimationIzoCalmTalk;
		}
		break;

	case 8:
		*animation = kModelAnimationIzoCombatIdle;
		++_animationFrame;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoCombatIdle)) {
			_animationFrame = 0;
		}
		break;

	case 9:
		*animation = kModelAnimationIzoCombatUnseatheSword;
		++_animationFrame;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoCombatUnseatheSword)) {
			_animationFrame = 0;
			_animationState = 8;
			*animation = kModelAnimationIzoCombatIdle;
		}
		break;

	case 10:
		*animation = kModelAnimationIzoCombatSeatheSword;
		++_animationFrame;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoCombatSeatheSword)) {
			*animation = kModelAnimationIzoIdle;
			_animationFrame = 0;
			_animationState = 0;
			_varChooseIdleAnimation = 0;
		}
		break;

	case 11:
		*animation = kModelAnimationIzoCombatSwordAttack;
		++_animationFrame;
		if (_animationFrame == 6) {
			int snd;
			if (Random_Query(1, 2) == 1) {
				snd = 9010;
			} else {
				snd = 9015;
			}
			Sound_Play_Speech_Line(kActorIzo, snd, 75, 0, 99);
		}
		if (_animationFrame == 9) {
			Actor_Combat_AI_Hit_Attempt(kActorIzo);
		}
		if (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoWaitingAtRC03
		 && _animationFrame == 6
		) {
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeDodge);
		}
		Actor_Query_Goal_Number(kActorIzo);
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 8;
			*animation = kModelAnimationIzoCombatIdle;
			Actor_Change_Animation_Mode(kActorIzo, kAnimationModeCombatIdle);
		}
		break;

	case 12:
		// fall through
	case 13:
		// fall through
	case 16:
		// fall through
	case 17:
		if (_animationState == 12) {
			*animation = kModelAnimationIzoCombatTurnRight;
		}
		if (_animationState == 13) {
			*animation = kModelAnimationIzoCombatTurnLeft;
		}
		if (_animationState == 16) {
			*animation = kModelAnimationIzoCombatGotHitRight;
		}
		if (_animationState == 17) {
			*animation = kModelAnimationIzoCombatGotHitLeft;
		}
		++_animationFrame;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 8;
			*animation = kModelAnimationIzoCombatIdle;
			Actor_Change_Animation_Mode(kActorIzo, kAnimationModeCombatIdle);
		}
		break;

	case 14:
		// fall through
	case 15:
		// TODO A bug? Shouldn't this be _animationState + kModelAnimationIzoCombatGotHitRight?
		*animation = _animationFrame + kModelAnimationIzoCombatGotHitRight;
		++_animationFrame;
		if (_animationFrame >= 2
		 && (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoRC03Walk
		  || Actor_Query_Goal_Number(kActorIzo) == kGoalIzoRC03Run
		 )
		) {
			*animation = kModelAnimationIzoIdle;
			_animationFrame = 0;
			_animationState = 0;
			_varChooseIdleAnimation = 0;
			Actor_Change_Animation_Mode(kActorIzo, kAnimationModeIdle);
			Actor_Set_Goal_Number(kActorIzo, kGoalIzoRC03Run);
		} else {
			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
				*animation = kModelAnimationIzoIdle;
				_animationFrame = 0;
				_animationState = 0;
				_varChooseIdleAnimation = 0;
				Actor_Change_Animation_Mode(kActorIzo, kAnimationModeIdle);
			}
		}
		break;

	case 18:
		// fall through
	case 19:
		if (_animationState == 18) {
			*animation = kModelAnimationIzoCombatShotDead;
		}
		if (_animationState == 19) {
			*animation = kModelAnimationIzoShotDead;
		}
		++_animationFrame;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(*animation) - 1) {
			_animationFrame = Slice_Animation_Query_Number_Of_Frames(*animation) - 1;
			_animationState = 30;
		}
		break;

	case 20:
		// fall through
	case 21:
		// fall through
	case 22:
		// fall through
	case 23:
		// fall through
	case 24:
		// fall through
	case 25:
		// fall through
	case 26:
		// fall through
	case 27:
		// fall through
	case 28:
		// fall through
	case 29:
		if (_animationState == 20) {
			*animation = kModelAnimationIzoWalking;
		}
		if (_animationState == 21) {
			*animation = kModelAnimationIzoRunning;
		}
		if (_animationState == 22) {
			*animation = kModelAnimationIzoCombatWalking;
		}
		if (_animationState == 23) {
			*animation = kModelAnimationIzoCombatRunning;
		}
		if (_animationState == 24) {
			*animation = kModelAnimationIzoCombatClimbStairsUp;
		}
		if (_animationState == 25) {
			*animation = kModelAnimationIzoCombatClimbStairsDown;
		}
		if (_animationState == 26) {
			*animation = kModelAnimationIzoClimbStairsUp;
		}
		if (_animationState == 27) {
			*animation = kModelAnimationIzoClimbStairsDown;
		}
		if (_animationState == 29) {
			*animation = kModelAnimationIzoClimbLadderUp;
		}
		if (_animationState == 28) {
			*animation = kModelAnimationIzoClimbLadderDown;
		}
		++_animationFrame;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		break;

	case 30:
		*animation = kModelAnimationIzoShotDead;
		_animationFrame = Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoShotDead) - 1;
		break;

	case 31:
		*animation = kModelAnimationIzoHoldingCameraIdle;
		++_animationFrame;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoHoldingCameraIdle)) {
			_animationFrame = 0;
		}
		break;

	case 32:
		*animation = kModelAnimationIzoPicksCameraFromShop;
		++_animationFrame;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoPicksCameraFromShop)) {
			_animationFrame = 0;
			_animationState = 31;
			*animation = kModelAnimationIzoHoldingCameraIdle;
		}
		break;

	case 33:
		if (_animationFrame == 0 && _resumeIdleAfterFramesetCompletesFlag) {
			_resumeIdleAfterFramesetCompletesFlag = false;
			_animationState = 31;
			*animation = kModelAnimationIzoHoldingCameraIdle;
		} else {
			*animation = kModelAnimationIzoHoldingCameraTalk;
			++_animationFrame;
			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoHoldingCameraTalk)) {
				_animationFrame = 0;
			}
		}
		break;

	case 34:
		*animation = kModelAnimationIzoHoldingCameraUsesFlash;
		++_animationFrame;
		if (_animationFrame == 6) {
			Scene_Loop_Set_Default(0); // // HC01 - MainLoop
			Scene_Loop_Start_Special(kSceneLoopModeOnce, 2, true); // HC01 - IzoFlashLoop
			Player_Set_Combat_Mode(true);
		}
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoHoldingCameraUsesFlash)) {
			*animation = kModelAnimationIzoIdle;
			_animationFrame = 0;
			_animationState = 0;
			Game_Flag_Set(kFlagUnused407);
			Item_Add_To_World(kItemCamera, kModelAnimationIzoCamera, kSetHC01_HC02_HC03_HC04, 597.46f, 0.14f, 49.92f, 0, 12, 12, false, true, false, false);
			Actor_Set_Goal_Number(kActorIzo, kGoalIzoRunToUG02);
		}
		break;

	case 35:
		if (_varChooseIdleAnimation == 0) {
			*animation = kModelAnimationIzoIdle;
		}
		if (_varChooseIdleAnimation == 1) {
			*animation = kModelAnimationIzoAwkwardPlayWithHands;
		}
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoIdle)) {
			// TODO a bug? Adding 3 to animationFrame when it is >= the num of frames of the frameset
			//             will still keep it >= the num of frame of the frameset.
			//             so why check again in the if clause below?
			_animationFrame += 3;
			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(kModelAnimationIzoIdle)) {
				_animationFrame = 0;
				*animation = _animationNext;
				_animationState = _animationStateNext;
			}
		} else {
			_animationFrame -= 3;
			if (_animationFrame <= 0) {
				_animationFrame = 0;
				*animation = _animationNext;
				_animationState = _animationStateNext;
			}
		}
		break;

	default:
		// Dummy placeholder, kModelAnimationZubenIdle (406) is a Zuben animation
		*animation = kModelAnimationZubenIdle;
		_animationFrame = 0;
		debugC(6, kDebugAnimation, "AIScriptIzo::UpdateAnimation() - Current _animationState (%d) is a placeholder", _animationState);
		break;
	}

	*frame = _animationFrame;

	return true;
}

bool AIScriptIzo::ChangeAnimationMode(int mode) {
	switch (mode) {
	case 0:
		switch (_animationState) {
		case 1:
			// fall through
		case 2:
			// fall through
		case 3:
			// fall through
		case 4:
			// fall through
		case 5:
			// fall through
		case 6:
			// fall through
		case 7:
			// fall through
		case 33:
			_resumeIdleAfterFramesetCompletesFlag = true;
			break;

		case 8:
			// fall through
		case 22:
			// fall through
		case 23:
			_animationState = 10;
			_animationFrame = 0;
			break;

		case 31:
			// fall through
		case 32:
			// fall through
		case 34:
			return true;

		default:
			_animationState = 0;
			_animationFrame = 0;
			break;
		}
		return true;

	case 1:
		if (Actor_Query_Goal_Number(kActorIzo) == kGoalIzoWaitingAtRC03) {
			_animationState = 22;
		} else {
			_animationState = 20;
		}
		_animationFrame = 0;
		return true;

	case 2:
		_animationState = 21;
		_animationFrame = 0;
		return true;

	case 3:
		// fall through
	case 9:
		if (_animationState != 32) {
			if (Actor_Query_Goal_Number(kActorIzo) == 1) {
				_animationState = 33;
				_animationFrame = 0;
			} else if (_animationState <= 0 || _animationState > 7) {
				_animationState = 35;
				_animationStateNext = 1;
				_animationNext = kModelAnimationIzoCalmTalk;
			}
			_resumeIdleAfterFramesetCompletesFlag = false;
		}
		return true;

	case 4:
		if (_animationState > 0) {
			if (_animationState != 8 || _animationState > 8) {
				_animationState = 8;
				_animationFrame = 0;
			}
		} else {
			_animationState = 9;
			_animationFrame = 0;
		}
		break;

	case 5:
		// fall through
	case 18:
		// fall through
	case 19:
		// fall through
	case 20:
		return true;

	case 6:
		_animationState = 11;
		_animationFrame = 0;
		break;

	case 7:
		_animationState = 22;
		_animationFrame = 0;
		break;

	case 8:
		_animationState = 23;
		_animationFrame = 0;
		break;

	case 10:
		// fall through
	case 12:
		if (_animationState != 32) {
			if (_animationState <= 0 || _animationState > 7) {
				_animationState = 35;
				_animationStateNext = 1;
				_animationNext = kModelAnimationIzoMoreCalmTalk;
			}
			_resumeIdleAfterFramesetCompletesFlag = false;
		}
		break;

	case 11:
		// fall through
	case 14:
		if (_animationState != 32) {
			if (_animationState <= 0 || _animationState > 7) {
				_animationState = 35;
				_animationStateNext = 1;
				_animationNext = kModelAnimationIzoHeadNodsAgreeingTalk;
			}
			_resumeIdleAfterFramesetCompletesFlag = false;
		}
		break;

	case 13:
		if (_animationState != 32) {
			if (_animationState <= 0 || _animationState > 7) {
				_animationState = 35;
				_animationStateNext = 1;
				_animationNext = kModelAnimationIzoExplainingTalk;
			}
			_resumeIdleAfterFramesetCompletesFlag = false;
		}
		break;

	case 15:
		if (_animationState != 32) {
			if (_animationState <= 0 || _animationState > 7) {
				_animationState = 35;
				_animationStateNext = 1;
				_animationNext = kModelAnimationIzoOffensiveTalk;
			}
			_resumeIdleAfterFramesetCompletesFlag = false;
		}
		break;

	case 16:
		if (_animationState != 32) {
			if (_animationState <= 0 || _animationState > 7) {
				_animationState = 35;
				_animationStateNext = 1;
				_animationNext = kModelAnimationIzoHeadNodsDisagreeingTalk;
			}
			_resumeIdleAfterFramesetCompletesFlag = false;
		}
		break;

	case 17:
		if (_animationState != 32) {
			if (_animationState <= 0 || _animationState > 7) {
				_animationState = 35;
				_animationStateNext = 1;
				_animationNext = kModelAnimationIzoUnderstandingTalk;
			}
			_resumeIdleAfterFramesetCompletesFlag = false;
		}
		break;

	case 21:
		switch (_animationState) {
		case 8:
			// fall through
		case 9:
			// fall through
		case 10:
			// fall through
		case 11:
			// fall through
		case 12:
			// fall through
		case 13:
			if (Random_Query(0, 1)) {
				_animationState = 17;
			} else {
				_animationState = 16;
			}
			_animationFrame = 0;
			break;

		case 14:
			// fall through
		case 15:
			// fall through
		case 16:
			// fall through
		case 17:
			return true;

		default:
			if (Random_Query(0, 1)) {
				_animationState = 15;
			} else {
				_animationState = 14;
			}
			_animationFrame = 0;
			break;
		}
		break;

	case 22:
		if (Random_Query(0, 1)) {
			_animationState = 16;
		} else {
			_animationState = 17;
		}
		_animationFrame = 0;
		break;

	case kAnimationModeDie:
		_animationState = 19;
		_animationFrame = 0;
		break;

	default:
		debugC(6, kDebugAnimation, "AIScriptIzo::ChangeAnimationMode(%d) - Target mode is not supported", mode);
		break;
	}

	return true;
}

void AIScriptIzo::QueryAnimationState(int *animationState, int *animationFrame, int *animationStateNext, int *animationNext) {
	*animationState     = _animationState;
	*animationFrame     = _animationFrame;
	*animationStateNext = _animationStateNext;
	*animationNext      = _animationNext;
}

void AIScriptIzo::SetAnimationState(int animationState, int animationFrame, int animationStateNext, int animationNext) {
	_animationState     = animationState;
	_animationFrame     = animationFrame;
	_animationStateNext = animationStateNext;
	_animationNext      = animationNext;
}

bool AIScriptIzo::ReachedMovementTrackWaypoint(int waypointId) {
	return true;
}

void AIScriptIzo::FledCombat() {
	// return false;
}

void AIScriptIzo::dialogueWithIzo() {
	Dialogue_Menu_Clear_List();
	DM_Add_To_List_Never_Repeat_Once_Selected(0, 7, 4, -1); // MOTIVES // A bug? This is a wrong option

	if (Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewA)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(10, 8, -1, -1); // LUCY  // A bug? This is a wrong option
	}

	DM_Add_To_List_Never_Repeat_Once_Selected(20, 3, 7, 4); // REFERENCE  // A bug? This is a wrong option
	DM_Add_To_List_Never_Repeat_Once_Selected(30, -1, 3, 7); // DONE // A bug? why not Dialogue_Menu_Add_DONE_To_List?

	Dialogue_Menu_Appear(320, 240);
	int input = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (input) {
	case 0: // MOTIVES -> Should be "LET GO"?
		Actor_Says(kActorMcCoy, 5470, 15); // Get lost, Izo. Take off.
		Actor_Says(kActorIzo, 710, 13);    //
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoEscape);
		Player_Gains_Control();
		break;

	case 10: // LUCY -> Should be split to "WEAPONS" AND "CRYSTAL" (if Replicant)?
		if (Game_Flag_Query(kFlagIzoIsReplicant)) {
			Actor_Says(kActorMcCoy, 5475, 18); // Listen, there's another Blade Runner after you and she won't stop to talk.
			Actor_Says(kActorIzo, 720, 12);
			Actor_Says(kActorMcCoy, 5485, 13); // Where did you get the hardware, Izo?
			Actor_Says(kActorIzo, 740, 14);
			Actor_Says(kActorMcCoy, 5495, 12);
			Actor_Says(kActorIzo, 750, 15);
			Actor_Says(kActorIzo, 760, 17);
			Actor_Says(kActorMcCoy, 5500, 12);
			Actor_Says(kActorIzo, 770, 15);
			Actor_Says(kActorIzo, 780, 15);
			Actor_Says(kActorMcCoy, 5505, 12); // I need to talk to Clovis.
			Actor_Says(kActorIzo, 790, 15);
		} else {
			Actor_Says(kActorMcCoy, 5510, 15); // Where did you get the hardware, Izo?
			Actor_Says(kActorIzo, 820, 13);
			Actor_Says(kActorMcCoy, 5520, 13);
			Actor_Says(kActorIzo, 830, 13);
			Actor_Says(kActorIzo, 840, 13);
		}
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoEscape);
		Player_Gains_Control();
		break;

	case 20: // REFERENCE -> Should be VOIGT-KAMPFF
		Actor_Says(kActorMcCoy, 5480, 18); // Look, just come along with me. You're gonna have to take a little personality test.
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoEscape);
		Player_Gains_Control();
		break;

	case 30: // DONE
		Player_Set_Combat_Mode(true);
		Actor_Set_Goal_Number(kActorIzo, kGoalIzoEscape);
		Player_Gains_Control();
		break;
	}
}

void AIScriptIzo::modifyWaypoints() {
	switch (Random_Query(1, 10) - 1) {
	case 0:
		World_Waypoint_Set(484, kSetNR01, -212.58f, 23.38f, -1859.45f);
		World_Waypoint_Set(485, kSetNR01, 355.49f, 31.66f, -859.81f);
		World_Waypoint_Set(486, kSetNR02, -323.89f, -24.0f, 35.58f);
		World_Waypoint_Set(487, kSetNR02, -211.89f, -24.0f, 35.58f);
		AI_Movement_Track_Append(kActorIzo, 484, 1);
		AI_Movement_Track_Append(kActorIzo, 485, 10);
		AI_Movement_Track_Append(kActorIzo, 486, 1);
		AI_Movement_Track_Append(kActorIzo, 487, 15);
		AI_Movement_Track_Append(kActorIzo, 486, 1);
		AI_Movement_Track_Append(kActorIzo, 485, 1);
		AI_Movement_Track_Append(kActorIzo, 484, 1);
		break;

	case 1:
		World_Waypoint_Set(484, kSetNR05_NR08, -1335.0f, 0.0f, -542.0f);
		World_Waypoint_Set(485, kSetNR05_NR08, -1027.0f, 0.0f, -542.0f);
		AI_Movement_Track_Append(kActorIzo, 484, 1);
		AI_Movement_Track_Append(kActorIzo, 485, 20);
		AI_Movement_Track_Append(kActorIzo, 484, 1);
		break;

	case 2:
		World_Waypoint_Set(484, kSetHF01, -352.16f, 8.0f, -379.24f);
		World_Waypoint_Set(485, kSetHF01, 108.2f, 8.0f, -934.80f);
		AI_Movement_Track_Append(kActorIzo, 484, 1);
		AI_Movement_Track_Append(kActorIzo, 485, 1);
		break;

	case 3:
		World_Waypoint_Set(484, kSetHF03, 589.59f, 47.76f, -1153.76f);
		World_Waypoint_Set(485, kSetHF03, 481.59f, 47.76f, -429.76f);
		World_Waypoint_Set(486, kSetHF02, 524.0f, 47.76f, -562.0f);
		World_Waypoint_Set(487, kSetHF02, -10.0f, 47.76f, -327.0f);
		AI_Movement_Track_Append(kActorIzo, 484, 1);
		AI_Movement_Track_Append(kActorIzo, 485, 1);
		AI_Movement_Track_Append(kActorIzo, 486, 1);
		AI_Movement_Track_Append(kActorIzo, 487, 1);
		break;

	default:
		AI_Movement_Track_Append(kActorIzo, 34, 60); // kSetFreeSlotB
		break;
	}
}

} // End of namespace BladeRunner
