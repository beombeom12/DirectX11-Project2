#include "stdafx.h"
#include "..\public\Player_State.h"
#include "Player.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "Animation.h"
#include "Weapon.h"


//TEST

#include "WarSkill.h"


#include "Effect_Point_PlayerDust.h"
#include "Effect_PlayerDust.h"
#include "Effect_Point_PlayerWindow.h"
CPlayer_State::CPlayer_State()
{
}

HRESULT CPlayer_State::Initialze(CPlayer * Player)
{

	m_pPlayer = Player;

	if (FAILED(Setup_State_Idle())) return E_FAIL;
	if (FAILED(Setup_State_Walk())) return E_FAIL;
	if (FAILED(Setup_State_Attack())) return E_FAIL;
	if (FAILED(Setup_State_HeavyAttack())) return E_FAIL;
	if (FAILED(Setup_State_Dash())) return E_FAIL;
	if (FAILED(Setup_State_Skill1())) return E_FAIL;
	if (FAILED(Setup_State_Flinch())) return E_FAIL;

	//isHeavyAttack = false;
	return S_OK;
}

HRESULT CPlayer_State::Setup_State_Idle(void)
{
	if (nullptr == m_pPlayer->m_pState)
		return E_FAIL;


	m_pPlayer->m_pState
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CPlayer_State::Start_Idle)
		.Init_Tick(this, &CPlayer_State::Tick_Idle)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)
		.Finish_Setting();
	return S_OK;
}

HRESULT CPlayer_State::Setup_State_Idle_Combat(void)
{
	return S_OK;
}

HRESULT CPlayer_State::Setup_State_Walk(void)
{
	if (nullptr == m_pPlayer->m_pState)
		return E_FAIL;

	m_pPlayer->m_pState->
		//WALKFRONT
		Add_State(TEXT("STATE::WALKF"))
		.Init_Start(this, &CPlayer_State::Start_Walk_F)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_F)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)
		//WALKBACK
		.Add_State(TEXT("STATE::WALKB"))
		.Init_Start(this, &CPlayer_State::Start_Walk_B)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_B)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)


		.Add_State(TEXT("STATE::WALKFL"))
		.Init_Start(this, &CPlayer_State::Start_Walk_FL)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_FL)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)



		.Add_State(TEXT("STATE::WALKFR"))
		.Init_Start(this, &CPlayer_State::Start_Walk_FR)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_FR)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)



		.Add_State(TEXT("STATE::WALKFLEFT"))
		.Init_Start(this, &CPlayer_State::Start_Walk_FLeft)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_FLeft)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)

		.Add_State(TEXT("STATE::WALKFRIGHT"))
		.Init_Start(this, &CPlayer_State::Start_Walk_FRight)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_FRight)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
			.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)

		//Backward Left
		.Add_State(TEXT("STATE::BACKWARDLEFT"))
		.Init_Start(this, &CPlayer_State::Start_Walk_BL)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_BLeft)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
			.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)

		//BackWard Right
			.Add_State(TEXT("STATE::BACKWARDRIGHT"))
			.Init_Start(this, &CPlayer_State::Start_Walk_BR)
			.Init_Tick(this, &CPlayer_State::Tick_Walk_BRight)
			.Init_End(this, &CPlayer_State::End_Common)
			.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
			.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
			.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
			.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
			.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
			.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
			.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
			.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
			.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
			.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
			.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
			.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
			.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
			.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)

		.Add_State(TEXT("STATE::RUNFCOMBAT"))
		.Init_Start(this, &CPlayer_State::Start_Run_CombatF)
		.Init_Tick(this, &CPlayer_State::Tick_Run_CombatF)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
			.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
			.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
			.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)
			.Finish_Setting();


	return S_OK;
}

HRESULT CPlayer_State::Setup_State_Attack(void)
{
	if (nullptr == m_pPlayer->m_pState)
		return E_FAIL;

	m_pPlayer->m_pState->
		//ATTACK_A
		Add_State(TEXT("STATE::ATTACK"))
		.Init_Start(this, &CPlayer_State::Start_Attack_A)							//올려 치기 공격 
		.Init_Tick(this, &CPlayer_State::Tick_Attack_A)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFinish)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACKB"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		//.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)


		.Add_State(TEXT("STATE::ATTACKB"))
		.Init_Start(this, &CPlayer_State::Start_Attack_B)							//올려치기 공격중 B
		.Init_Tick(this, &CPlayer_State::Tick_Attack_B)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFinish)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACKC"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)

		.Add_State(TEXT("STATE::ATTACKC"))
		.Init_Start(this, &CPlayer_State::Start_Attack_C)							//올려치기 공격중 B
		.Init_Tick(this, &CPlayer_State::Tick_Attack_C)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFinish)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACKD"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)







		.Add_State(TEXT("STATE::ATTACKD"))
		.Init_Start(this, &CPlayer_State::Start_Attack_D)							//올려치기 공격중 B
		.Init_Tick(this, &CPlayer_State::Tick_Attack_D)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFinish)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACKC_A"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)

		.Add_State(TEXT("STATE::ATTACKC_A"))
		.Init_Start(this, &CPlayer_State::Start_Attack_C_A)							//올려치기 공격중 B
		.Init_Tick(this, &CPlayer_State::Tick_Attack_C_A)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFinish)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)



		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::Setup_State_HeavyAttack(void)
{
	if (nullptr == m_pPlayer->m_pState)
		return E_FAIL;

	m_pPlayer->m_pState->
		//HeavyAttack_A
		Add_State(TEXT("STATE::HEAVYATTACK"))
		.Init_Start(this, &CPlayer_State::Start_HeavyAttack_A)							//올려 치기 공격 
		.Init_Tick(this, &CPlayer_State::Tick_HeavyAttack_A)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFinish)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACKB"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)

		.Add_State(TEXT("STATE::HEAVYATTACKB"))
		.Init_Start(this, &CPlayer_State::Start_HeavyAttack_B)							//올려 치기 공격 
		.Init_Tick(this, &CPlayer_State::Tick_HeavyAttack_B)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFinish)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACKC"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)


	


		.Add_State(TEXT("STATE::HEAVYATTACKC"))
		.Init_Start(this, &CPlayer_State::Start_HeavyAttack_C)							//올려 치기 공격 
		.Init_Tick(this, &CPlayer_State::Tick_HeavyAttack_C)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFinish)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::HEAVYATTACK"), this, &CPlayer_State::KeyInput_J)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)
		
		
		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::Setup_State_Dash(void)
{
	if (nullptr == m_pPlayer->m_pState)
		return E_FAIL;

	m_pPlayer->m_pState->
		Add_State(TEXT("STATE::Dash"))
		.Init_Start(this, &CPlayer_State::Start_Dash_F)
		.Init_Tick(this, &CPlayer_State::Tick_Dash_F)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFinish)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::RUNFCOMBAT"), this, &CPlayer_State::KeyInput_R)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::KeyInput_K)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::Skill1"), this, &CPlayer_State::KeyInput_L)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)
		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::Setup_State_Skill1(void)
{
	if (nullptr == m_pPlayer->m_pState)
		return E_FAIL;

	m_pPlayer->m_pState->
		Add_State(TEXT("STATE::Skill1"))
		.Init_Start(this, &CPlayer_State::Start_Skill1_A)
		.Init_Tick(this, &CPlayer_State::Tick_Skill1_A)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::Skill2"), this, &CPlayer_State::KeyInput_LS)
		

		.Add_State(TEXT("STATE::Skill2"))
		.Init_Start(this, &CPlayer_State::Start_Skill2_A)
		.Init_Tick(this, &CPlayer_State::Tick_Skill2_A)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFinish)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CPlayer_State::isAttackedRange)

		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::Setup_State_Flinch(void)
{
	if (nullptr == m_pPlayer->m_pState)
		return E_FAIL;

	m_pPlayer->m_pState->
		Add_State(TEXT("STATE::FLINCH"))
		.Init_Start(this, &CPlayer_State::Start_Flinch_F)
		.Init_Tick(this, &CPlayer_State::Tick_Flinch_A)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::isFlinch)
	/*	.Init_Changer(TEXT("STATE::WALKF"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALKB"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALKFL"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALKFR"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALKFLEFT"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALKFRIGHT"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::BACKWARDLEFT"), this, &CPlayer_State::KeyInput_AS)
		.Init_Changer(TEXT("STATE::BACKWARDRIGHT"), this, &CPlayer_State::KeyInput_DS)
		.Init_Changer(TEXT("STATE::Dash"), this, &CPlayer_State::KeyInput_Space)*/
		.Finish_Setting();


	return S_OK;

}

void CPlayer_State::Start_Idle(_double TimeDelta)
{
 	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Idle_Combat);

}

void CPlayer_State::Start_Idle_To_Combat(_double TimeDelta)
{
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Idle_to_Idle_Combat);
}

void CPlayer_State::Start_Combat_To_Idle(_double TimeDelta)
{
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Idle_Combat_to_Idle);
}

void CPlayer_State::Start_Walk_F(_double TimeDelta)
{
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Run_F_Combat);
}

void CPlayer_State::Start_Walk_B(_double TimeDelta)
{
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Run_F_Combat);
}

void CPlayer_State::Start_Walk_FL(_double TimeDelta)
{
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Run_F_Combat);
}

void CPlayer_State::Start_Walk_FR(_double TimeDelta)
{
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Run_F_Combat);
}

void CPlayer_State::Start_Walk_BL(_double TimeDelta)
{
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Run_F_Combat);
}

void CPlayer_State::Start_Walk_BR(_double TimeDelta)
{
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Run_F_Combat);
}

void CPlayer_State::Start_Run_CombatF(_double TimeDelta)
{

	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Run_F);
}

void CPlayer_State::Start_Walk_FLeft(_double TimeDelta)
{
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Run_F_Combat);
}

void CPlayer_State::Start_Walk_FRight(_double TimeDelta)
{
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Run_F_Combat);
}

void CPlayer_State::Start_Attack_A(_double TimeDelta)
{
	ENGINE_OPEN;
	ENGINE->Play_Sound(TEXT("AttackAHigh.mp3"), 1, false, 1);

	ENGINE_CLOSE;
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Atk_Light_01);

}

void CPlayer_State::Start_Attack_B(_double TimeDelta)
{
	ENGINE_OPEN;
	ENGINE->Play_Sound(TEXT("AttackAHigh.mp3"), 1, false, 1);

	ENGINE_CLOSE;
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Atk_Light_02);
}

void CPlayer_State::Start_Attack_C(_double TimeDelta)
{
	ENGINE_OPEN;
	ENGINE->Play_Sound(TEXT("AttackBHight.mp3"), 1, false, 1);

	ENGINE_CLOSE;
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Atk_Light_03);
}

void CPlayer_State::Start_Attack_C_A(_double TimeDelta)
{
	ENGINE_OPEN;
	ENGINE->Play_Sound(TEXT("AttackBHight.mp3"), 1, false, 1);
	ENGINE_CLOSE;
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Atk_Light_04_Single);
}

void CPlayer_State::Start_Attack_D(_double TimeDelta)
{
	ENGINE_OPEN;
	ENGINE->Play_Sound(TEXT("AttackBHight.mp3"), 1, false, 1);
	ENGINE_CLOSE;
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Atk_Light_04);
}

void CPlayer_State::Start_HeavyAttack_A(_double TimeDelta)
{
	ENGINE_OPEN;

	ENGINE->Play_Sound(TEXT("HeavyAttackBHigh.mp3"), 1, false, 1);

	ENGINE_CLOSE;


	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Atk_Heavy_01);
}

void CPlayer_State::Start_HeavyAttack_B(_double TimeDelta)
{
	ENGINE_OPEN;

	ENGINE->Play_Sound(TEXT("HeavyAttackCHigh.mp3"), 1, false, 1);

	ENGINE_CLOSE;


	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Atk_Heavy_02);
}



void CPlayer_State::Start_HeavyAttack_C(_double TimeDelta)
{
	ENGINE_OPEN;

	ENGINE->Play_Sound(TEXT("HeavyAttackDHigh.mp3"), 1, false, 1);

	ENGINE_CLOSE;
	m_pPlayer->m_fAnimAdjust = 2.f;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Atk_Heavy_03);
}

void CPlayer_State::Start_Skill1_A(_double TimeDelta)
{
	ENGINE_OPEN;
	ENGINE->Play_Sound(TEXT("GoShot3.mp3"), 1, true, 1);
	ENGINE_CLOSE;
	m_pPlayer->m_fAnimAdjust = 1.9;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Atk_Flamebrand);

}

void CPlayer_State::Start_Skill2_A(_double TimeDelta)
{
	ENGINE_OPEN;
	ENGINE->Play_Sound(TEXT("FinishEffect.mp3"), 1, true, 1);
	ENGINE_CLOSE;

	m_pPlayer->m_fAnimAdjust = 1.9;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Atk_Flamebrand_End);
}


void CPlayer_State::Start_Dash_F(_double TimeDelta)
{

	ENGINE_OPEN;

	ENGINE->Play_Sound(TEXT("DashSoundHigh.mp3"), 1, false, 1);

	ENGINE_CLOSE;

	m_pPlayer->m_fAnimAdjust = 1.7;
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Dash_F);

}

void CPlayer_State::Start_Dash_B(_double TimeDelta)
{
	m_pPlayer->m_pModelCom->Set_AnimIndex(War_DashTo_Back);
}

void CPlayer_State::Start_Flinch_F(_double TimeDelta)
{
	m_pPlayer->m_fAnimAdjust = 1.f;

	m_pPlayer->m_pModelCom->Set_AnimIndex(War_Impact_From_Front_01);
}


void CPlayer_State::Tick_Idle(_double TimeDelta)
{

	static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
}

void CPlayer_State::Tick_Walk_F(_double TimeDelta)
{
	static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
	m_pNavi = m_pPlayer->m_pNavigation;
m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-270.f));
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.3f, m_pNavi);
	//m_pPlayer->m_pPointPlayerDust->Shoot_Point();


}

void CPlayer_State::Tick_Walk_B(_double TimeDelta)
{
	static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
	m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.3f, m_pNavi);
	//m_pPlayer->m_pPointPlayerDust->Shoot_Point();
}

void CPlayer_State::Tick_Walk_FL(_double TimeDelta)
{
	static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
	m_pNavi = m_pPlayer->m_pNavigation;
	m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.3f, m_pNavi);
	//m_pPlayer->m_pPointPlayerDust->Shoot_Point();
}

void CPlayer_State::Tick_Walk_FR(_double TimeDelta)
{
	m_pNavi = m_pPlayer->m_pNavigation;
	m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.3f, m_pNavi);
//	m_pPlayer->m_pPointPlayerDust->Shoot_Point();
	//m_pPlayer->m_pTransformCom->Turn(XMVectorSet(0.f, 0.000001f, 0.f, 0.f), XMConvertToRadians(90.f));
}

void CPlayer_State::Tick_Walk_FLeft(_double TimeDelta)
{
	static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
	m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-315.f));
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.3f, m_pNavi);
}

void CPlayer_State::Tick_Walk_FRight(_double TimeDelta)
{
	static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
	m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-225.f));
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.3f, m_pNavi);
}

void CPlayer_State::Tick_Walk_BLeft(_double TimeDelta)
{
	static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
	m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.3f, m_pNavi);
}

void CPlayer_State::Tick_Walk_BRight(_double TimeDelta)
{
	static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
	m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-135.f));
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.3f, m_pNavi);
}


void CPlayer_State::Tick_Run_CombatF(_double TimeDelta)
{
	static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.6, m_pNavi);

}

//Heavy
void CPlayer_State::Tick_Attack_A(_double TimeDelta)
{
	AnimIntervalChecker(War_Atk_Light_01, 0.3, 0.7);
//	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 0.05, m_pNavi);

}

void CPlayer_State::Tick_Attack_B(_double TimeDelta)
{
	AnimIntervalChecker(War_Atk_Light_02, 0.3, 0.7);
	//m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 0.05, m_pNavi);
}

void CPlayer_State::Tick_Attack_C(_double TimeDelta)
{
	AnimIntervalChecker(War_Atk_Light_03, 0.1, 0.9);
	//m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 0.05, m_pNavi);
}

void CPlayer_State::Tick_Attack_C_A(_double TimeDelta)
{
	AnimIntervalChecker(War_Atk_Light_04_Single, 0.1, 0.9);
	//m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 0.08, m_pNavi);
}

void CPlayer_State::Tick_Attack_D(_double TimeDelta)
{
	AnimIntervalChecker(War_Atk_Light_04, 0.1, 0.9);
//	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 0.01, m_pNavi);

	//m_pPlayer->m_pEffect_Window->Shoot_Point();

}

void CPlayer_State::Tick_HeavyAttack_A(_double TimeDelta)
{


	AnimIntervalChecker(War_Atk_Heavy_01, 0.1, 0.9);
	m_pPlayer->m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pPlayer->m_pTransformCom->Get_tWorldMatrix());
	//m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 0.05, m_pNavi);
}

void CPlayer_State::Tick_HeavyAttack_B(_double TimeDelta)
{
	AnimIntervalChecker(War_Atk_Heavy_02, 0.1, 0.9);
	//m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 0.07, m_pNavi);

}


void CPlayer_State::Tick_HeavyAttack_C(_double TimeDelta)
{
	AnimIntervalChecker(War_Atk_Heavy_03, 0.1, 0.9);
	//m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 0.07, m_pNavi);
}

void CPlayer_State::Tick_Skill1_A(_double TimeDelta)
{




	AnimIntervalChecker(War_Atk_Flamebrand, 0.1, 0.99);

	ENGINE_OPEN;
	
	//ENGINE->Play_Sound(TEXT("Test3.mp3"), 1, false, 1);

	if(ENGINE->Get_DIKeyState(DIK_A))
	{
		m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
		m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_pNavi);
	}
	else if (ENGINE->Get_DIKeyState(DIK_W))
	{
		m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-270.f));
		m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_pNavi);
	}
	else if (ENGINE->Get_DIKeyState(DIK_D))
	{
		m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
		m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_pNavi);
	}
	else if (ENGINE->Get_DIKeyState(DIK_S))
	{
		m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
		m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_pNavi);
	}
	else if (ENGINE->Get_DIKeyState(DIK_Q))
	{
		m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-315.f));
		m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_pNavi);

	}
	else if (ENGINE->Get_DIKeyState(DIK_E))
	{
		m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-225.f));
		m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_pNavi);

	}
	else if (ENGINE->Get_DIKeyState(DIK_Z))
	{
		m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));
		m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_pNavi);

	}
	else if (ENGINE->Get_DIKeyState(DIK_C))
	{
		m_pPlayer->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-135.f));
		m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_pNavi);

	}


	
	ENGINE_CLOSE;
}

void CPlayer_State::Tick_Skill2_A(_double TimeDelta)
{
}

void CPlayer_State::Tick_Dash_F(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 3.f, m_pNavi);
	m_pPlayer->m_pEffect_PlayerDust->Shoot_Point();


}

void CPlayer_State::Tick_Dash_B(_double TimeDelta)
{
}


void CPlayer_State::Tick_Flinch_A(_double TimeDelta)
{
	static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
	m_pPlayer->m_pTransformCom->Go_Backward(TimeDelta * 0.03, m_pNavi);	
}

void CPlayer_State::End_Common(_double TimeDelta)
{

	ENGINE_OPEN;
	ENGINE->Stop_Sound(1);
	ENGINE_CLOSE;

}

_bool CPlayer_State::KeyInput_None(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		!ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_W(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_A(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		!ENGINE->Get_DIKeyState(DIK_W) &&
		ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_S(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		!ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_D(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		!ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		ENGINE->Get_DIKeyState(DIK_D);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_WA(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		ENGINE->Get_DIKeyState(DIK_W) &&
		ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_WD(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		ENGINE->Get_DIKeyState(DIK_D);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_WS(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_AS(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		!ENGINE->Get_DIKeyState(DIK_W) &&
		ENGINE->Get_DIKeyState(DIK_A) &&
		ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_DS(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		!ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		ENGINE->Get_DIKeyState(DIK_S) &&
		ENGINE->Get_DIKeyState(DIK_D);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_H(void)
{
	ENGINE_OPEN;

	_bool	bResult =
		!ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D) &&
		ENGINE->Key_Down(DIK_H);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_K(void)
{

	ENGINE_OPEN;

	_bool bResult =
		!ENGINE->Key_Down(DIK_W) &&
		!ENGINE->Key_Down(DIK_A) &&
		!ENGINE->Key_Down(DIK_S) &&
		!ENGINE->Key_Down(DIK_D) &&
		ENGINE->Key_Down(DIK_K);
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_J(void)
{


	ENGINE_OPEN;

	_bool bResult =
		!ENGINE->Key_Down(DIK_W)  &&
		!ENGINE->Key_Down(DIK_A) &&
		!ENGINE->Key_Down(DIK_S) &&
		!ENGINE->Key_Down(DIK_D) &&
		ENGINE->Key_Down(DIK_J);
	
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_L(void)
{

	ENGINE_OPEN;

	_bool bResult =
		ENGINE->Get_DIKeyState(DIK_W) &&
		ENGINE->Get_DIKeyState(DIK_A) &&
		ENGINE->Get_DIKeyState(DIK_S) &&
		ENGINE->Get_DIKeyState(DIK_D) ||
		ENGINE->Key_Down(DIK_L);
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_LS(void)
{
	ENGINE_OPEN;

	_bool bResult =
		!ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D) &&
		ENGINE->Key_Up(DIK_L);
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_R(void)
{
	ENGINE_OPEN
		_bool bResult =
		ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D) &&
		ENGINE->Get_DIKeyState(DIK_R);
	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_Shift(void)
{
	//ENGINE_OPEN;

	//_bool bResult =
	//	ENGINE->Get_DIKeyState(DIK_LSHIFT);

	//ENGINE_CLOSE;
	//return bResult;

	return _bool();
}

_bool CPlayer_State::KeyInput_Space(void)
{
	ENGINE_OPEN;

	_bool bResult =
		!ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D) &&
		ENGINE->Key_Down(DIK_SPACE);

	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_ShiftW(void)
{
	ENGINE_OPEN;

	_bool bResult =

		ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D) &&
		ENGINE->Get_DIKeyState(DIK_LSHIFT);

	ENGINE_CLOSE;
	return bResult;
}

_bool CPlayer_State::KeyInput_ShiftA(void)
{
	ENGINE_OPEN;

	_bool bResult =

		!ENGINE->Get_DIKeyState(DIK_W) &&
		ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		!ENGINE->Get_DIKeyState(DIK_D) &&
		ENGINE->Get_DIKeyState(DIK_LSHIFT);

	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_ShiftD(void)
{
	ENGINE_OPEN;

	_bool bResult =

		!ENGINE->Get_DIKeyState(DIK_W) &&
		!ENGINE->Get_DIKeyState(DIK_A) &&
		!ENGINE->Get_DIKeyState(DIK_S) &&
		ENGINE->Get_DIKeyState(DIK_D) &&
		ENGINE->Get_DIKeyState(DIK_LSHIFT);

	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::isFinish(void)
{
	_bool bResult = m_pPlayer->m_pModelCom->Get_AnimFinish();
	m_pPlayer->m_pModelCom->isFinish();
	m_pPlayer->m_AnimFinish = bResult;

	return bResult;
}

_bool CPlayer_State::isFlinch(void)
{

	if (m_pPlayer->Collision_AttackedRange())
	{
		m_pPlayer->m_AnimFinish = false;
	}
	else
	{
		m_pPlayer->m_AnimFinish = true;
	}
	return m_pPlayer->m_AnimFinish;
}

_bool CPlayer_State::isAttackedRange(void)
{
	_bool bResult = m_pPlayer->Collision_AttackedRange();

	return bResult;
}

_bool CPlayer_State::AnimIntervalChecker(ANIMATION eAnim, _double StartRate, _double FinishRate)
{
	ENGINE_OPEN;
	if (m_pPlayer->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() >= StartRate
		&& m_pPlayer->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() <= FinishRate)
	{

		static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(true);
	}
	else
	{
		static_cast<CCollider*>(m_pPlayer->Get_WeaponCollier())->Set_CheckAble(false);
	}
	ENGINE_CLOSE;

	return true;
}

_bool CPlayer_State::AnimFinishChecker(ANIMATION eAnim, _double FinishRate)
{
	return m_pPlayer->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() >= FinishRate;
}// Transition

CPlayer_State * CPlayer_State::Create(CPlayer * Player)
{
	CPlayer_State* pInstance = new CPlayer_State();

	if (FAILED(pInstance->Initialze(Player)))
	{
		MSG_BOX("BOOOOOOOOOOOOOOMING : CPlayer_State");
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CPlayer_State::Free(void)
{
	Safe_Release(m_pPlayer->m_pNavigation);
	Safe_Release(m_pPlayer);

}


