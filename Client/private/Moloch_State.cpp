#include "stdafx.h"
#include "..\public\Moloch_State.h"
#include "Moloch.h"
#include "Model.h"
#include "Animation.h"
#include "Mo_Stone.h"
#include "Transform.h"

CMoloch_State::CMoloch_State()
{
}

HRESULT CMoloch_State::Initialize(CMoloch * pMoloch)
{
	m_pMoloch = pMoloch;


	if (FAILED(SetUp_State_Idle()))
		return E_FAIL;
	
	if (FAILED(SetUp_State_Chase()))
		return E_FAIL;

	if (FAILED(SetUp_State_Attack()))
		return E_FAIL;

	if (FAILED(SetUp_State_Flinch()))
		return E_FAIL;
		
//For Boss Skill	
	if (FAILED(SetUp_State_Skill()))
		return E_FAIL;
	if (FAILED(SetUp_State_Skill2()))
		return E_FAIL;
	if (FAILED(SetUp_State_Dead()))
		return E_FAIL;

	return S_OK;
}


HRESULT CMoloch_State::SetUp_State_Idle()
{
	if (nullptr == m_pMoloch->m_pStateCom)
		return E_FAIL;

	m_pMoloch->m_pStateCom->
		Set_Root(TEXT("STATE::IDLE"))
		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CMoloch_State::Start_Idle)
		.Init_Tick(this, &CMoloch_State::Tick_Idle)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::CHASE"), this, &CMoloch_State::isChase)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CMoloch_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::SKILL1"), this, &CMoloch_State::isSkillRange)
		.Init_Changer(TEXT("STATE::TWOSKILL1"), this, &CMoloch_State::isSkill2Range)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)
		.Finish_Setting();
	return S_OK;
}

HRESULT CMoloch_State::SetUp_State_Chase()
{
	m_pMoloch->m_pStateCom->
		Add_State(TEXT("STATE::CHASE"))
		.Init_Start(this, &CMoloch_State::Start_Chase)
		.Init_Tick(this, &CMoloch_State::Tick_Chase)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isNone)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CMoloch_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::SKILL1"), this, &CMoloch_State::isSkillRange)
		.Init_Changer(TEXT("STATE::TWOSKILL1"), this, &CMoloch_State::isSkill2Range)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)
		
		.Finish_Setting();
	return S_OK;
}

HRESULT CMoloch_State::SetUp_State_Attack()
{
	m_pMoloch->m_pStateCom->
		Add_State(TEXT("STATE::ATTACKA"))
		.Init_Start(this, &CMoloch_State::Start_Attack)
		.Init_Tick(this, &CMoloch_State::Tick_Attack)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isFinish)
		.Init_Changer(TEXT("STATE::ATTACKB"), this, &CMoloch_State::isNextAttackB)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)

		.Add_State(TEXT("STATE::ATTACKB"))
		.Init_Start(this, &CMoloch_State::Start_AttackB)
		.Init_Tick(this, &CMoloch_State::Tick_AttackB)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isFinish)
		.Init_Changer(TEXT("STATE::ATTACKC"), this, &CMoloch_State::isNextAttackC)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)



		.Add_State(TEXT("STATE::ATTACKC"))
		.Init_Start(this, &CMoloch_State::Start_AttackC)
		.Init_Tick(this, &CMoloch_State::Tick_AttackC)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isFinish)
	//	.Init_Changer(TEXT("STATE::ATTACKA"), this, &CMoloch_State::isNextIdle)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::SKILL1"), this, &CMoloch_State::isSkillRange)
		.Init_Changer(TEXT("STATE::TWOSKILL1"), this, &CMoloch_State::isSkill2Range)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)
		.Finish_Setting();

	return S_OK;
}

HRESULT CMoloch_State::SetUp_State_Flinch()
{

	m_pMoloch->m_pStateCom->
		Add_State(TEXT("STATE::FLINCH"))
		.Init_Start(this, &CMoloch_State::Start_Flinch)
		.Init_Tick(this, &CMoloch_State::Tick_Flinch)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isAttackedFinish)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)
		.Finish_Setting();
	return S_OK;
}

HRESULT CMoloch_State::SetUp_State_Skill()
{
	m_pMoloch->m_pStateCom->
		Add_State(TEXT("STATE::SKILL1"))
		.Init_Start(this, &CMoloch_State::Start_Skill1)
		.Init_Tick(this, &CMoloch_State::Tick_Skill1)
		.Init_End(this, &CMoloch_State::End_Common)	
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isSkillFinish)
		.Init_Changer(TEXT("STATE::SKILL2"), this, &CMoloch_State::isSkillNextB)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)
		
		.Add_State(TEXT("STATE::SKILL2"))
		.Init_Start(this, &CMoloch_State::Start_Skill2)
		.Init_Tick(this, &CMoloch_State::Tick_Skill2)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isSkillFinish)
		.Init_Changer(TEXT("STATE::SKILL3"), this, &CMoloch_State::isSkillNextC)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)
		
		.Add_State(TEXT("STATE::SKILL3"))
		.Init_Start(this, &CMoloch_State::Start_Skill3)
		.Init_Tick(this, &CMoloch_State::Tick_Skill3)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isSkillFinish)
		.Init_Changer(TEXT("STATE::SKILL4"), this, &CMoloch_State::isSkillNextD)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)
		
		.Add_State(TEXT("STATE::SKILL4"))
		.Init_Start(this, &CMoloch_State::Start_Skill4)
		.Init_Tick(this, &CMoloch_State::Tick_Skill4)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isSkillFinish)
		//.Init_Changer(TEXT("STATE::SKILL1"), this, &CMoloch_State::isSkillNextIdle)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CMoloch_State::isAttackRange)
		.Init_Changer(TEXT("STATE::TWOSKILL1"), this, &CMoloch_State::isSkill2Range)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)
		.Finish_Setting();
	return S_OK;
}

HRESULT CMoloch_State::SetUp_State_Skill2()
{


	m_pMoloch->m_pStateCom->
		Add_State(TEXT("STATE::TWOSKILL1"))
		.Init_Start(this, &CMoloch_State::Start_SkillTwo1)
		.Init_Tick(this, &CMoloch_State::Tick_TwoSkill1)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isSkill2Finish)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::TWOSKILL2"),this, &CMoloch_State::isTwoSkillNextB)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)

		.Add_State(TEXT("STATE::TWOSKILL2"))
		.Init_Start(this, &CMoloch_State::Start_SkillTwo2)
		.Init_Tick(this, &CMoloch_State::Tick_TwoSkill2)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isSkill2Finish)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::TWOSKILL3"), this, &CMoloch_State::isTwoSkillNextC)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)

		.Add_State(TEXT("STATE::TWOSKILL3"))
		.Init_Start(this, &CMoloch_State::Start_SkillTwo3)
		.Init_Tick(this, &CMoloch_State::Tick_TwoSkill3)
		.Init_End(this, &CMoloch_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMoloch_State::isSkill2Finish)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CMoloch_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::TWOSKILL1"), this, &CMoloch_State::isTwoSkillIDLE)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CMoloch_State::isAttackRange)
		.Init_Changer(TEXT("STATE::SKILL1"), this, &CMoloch_State::isSkillRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CMoloch_State::isDead)
		.Finish_Setting();
	return S_OK;
}

HRESULT CMoloch_State::SetUp_State_Dead()
{

	m_pMoloch->m_pStateCom->
		Add_State(TEXT("STATE::DEAD"))
		.Init_Start(this, &CMoloch_State::Start_Dead)
		.Init_Tick(this, &CMoloch_State::Tick_Dead)
		.Init_End(this, &CMoloch_State::End_Common)
		.Finish_Setting();
	return S_OK;
}


void CMoloch_State::Start_Idle(_double TimeDelta)
{
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Full_Idle);
}

void CMoloch_State::Start_Chase(_double TimeDelta)
{
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Full_Run_F);
}

void CMoloch_State::Start_Flinch(_double TimeDelta)
{
	m_pMoloch->m_fMolochPlayTime = 0.08f;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Full_Impact_F);
}

void CMoloch_State::Start_Attack(_double TimeDelta)
{


	m_pMoloch->m_fMolochPlayTime = 0.05;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Swipe_01);
}

void CMoloch_State::Start_AttackB(_double TimeDelta)
{
	
	m_pMoloch->m_fMolochPlayTime = 0.05;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Swipe_02);
}

void CMoloch_State::Start_AttackC(_double TimeDelta)
{



	m_pMoloch->m_fMolochPlayTime = 0.05;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Swipe_03);
}

void CMoloch_State::Start_Skill1(_double TimeDelta)
{
	m_pMoloch->m_fMolochPlayTime = 0.1;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Dash_Start);
}

void CMoloch_State::Start_Skill2(_double TimeDelta)
{
	m_pMoloch->m_fMolochPlayTime = 0.1;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Dash);
}

void CMoloch_State::Start_Skill3(_double TimeDelta)
{
	m_pMoloch->m_fMolochPlayTime = 0.05;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Dash_Strike);
}

void CMoloch_State::Start_Skill4(_double TimeDelta)
{
	m_pMoloch->m_fMolochPlayTime = 0.05;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Dash_Strike_02);
}

void CMoloch_State::Start_SkillTwo1(_double TimeDelta)
{
	m_pMoloch->m_fMolochPlayTime = 0.05f;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Geyser_Start);

}

void CMoloch_State::Start_SkillTwo2(_double TimeDelta)
{
	m_pMoloch->m_fMolochPlayTime = 0.05f;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Geyser);


	m_pMoloch->m_pMoStone->m_bFirst = false;

}

void CMoloch_State::Start_SkillTwo3(_double TimeDelta)
{
	m_pMoloch->m_fMolochPlayTime = 0.05f;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Geyser_02);
}



void CMoloch_State::Start_Dead(_double TimeDelta)
{

	m_pMoloch->m_fMolochPlayTime = 0.05f;
	m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Full_Impact_Stun);

}

void CMoloch_State::Tick_Idle(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->Off_Switch();
	static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(false);
	static_cast<CCollider*>(m_pMoloch->Get_StoneCollider())->Set_CheckAble(false);


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pMoloch->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CMoloch_State::Tick_Chase(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(false);
	//static_cast<CCollider*>(m_pMoloch->Get_StoneCollider())->Set_CheckAble(false);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);

	
	m_pMoloch->m_pTransformCom->Chase(vPlayerPos, TimeDelta * 0.08);
	m_pMoloch->m_pTransformCom->LookAt(vPlayerPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CMoloch_State::Tick_Flinch(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
}

void CMoloch_State::Tick_Attack(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	static_cast<CCollider*>(m_pMoloch->Get_StoneCollider())->Set_CheckAble(false);
	AnimationIntervalChecker(Moloch_Atk_Swipe_01, 0.2, 0.6);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pMoloch->m_pTransformCom->LookAt(vPlayerPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CMoloch_State::Tick_AttackB(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	static_cast<CCollider*>(m_pMoloch->Get_StoneCollider())->Set_CheckAble(false);
	AnimationIntervalChecker(Moloch_Atk_Swipe_02, 0.2, 0.9);
}

void CMoloch_State::Tick_AttackC(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	static_cast<CCollider*>(m_pMoloch->Get_StoneCollider())->Set_CheckAble(false);
	AnimationIntervalChecker(Moloch_Atk_Swipe_03, 0.2, 0.9);
}

void CMoloch_State::Tick_Skill1(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	static_cast<CCollider*>(m_pMoloch->Get_StoneCollider())->Set_CheckAble(false);
	static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(false);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*	pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pMoloch->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);

}

void CMoloch_State::Tick_Skill2(_double TimeDelta)
{

	m_pMoloch->m_pMonsterHPUI->On_Switch();
	//static_cast<CCollider*>(m_pMoloch->Get_StoneCollider())->Set_CheckAble(false);
	static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(false);

}

void CMoloch_State::Tick_Skill3(_double TImeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	AnimationIntervalChecker(Moloch_Atk_Full_Dash_Strike, 0.1, 0.9);
	m_pMoloch->m_pTransformCom->Camera_GoStraight(TImeDelta * 0.1f);

}

void CMoloch_State::Tick_Skill4(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	AnimationIntervalChecker(Moloch_Atk_Full_Dash_Strike_02, 0.1, 0.9);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*	pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pMoloch->m_pTransformCom->Camera_GoStraight(TimeDelta * 0.07f);
	m_pMoloch->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CMoloch_State::Tick_TwoSkill1(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(false);
	AnimationIntervalChecker(Moloch_Atk_Full_Geyser_Start, 0.1, 0.9);
	

}

void CMoloch_State::Tick_TwoSkill2(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	m_pMoloch->m_pMoStone->Shoot_Stone(TimeDelta);
	static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(false);
	
}

void CMoloch_State::Tick_TwoSkill3(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(false);
	static_cast<CCollider*>(m_pMoloch->Get_StoneCollider())->Set_CheckAble(false);
}

void CMoloch_State::Tick_Dead(_double TimeDelta)
{
	m_pMoloch->m_pMonsterHPUI->On_Switch();
	m_pMoloch->m_pMonsterAbility->fCurrentHP = 0.f;
	static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(false);
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Full_Impact_Stun)->Get_PlayRate() >= 0.99)
	{
		static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(false);
		m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Full_Impact_Stun_Idle);
	}
}

void CMoloch_State::End_Common(_double TimeDelta)
{
}

_bool CMoloch_State::isNone(void)
{
	if (m_pMoloch->Collision_ChaseRange())
	{
		m_pMoloch->m_AnimFinish = false;
	}
	else
	{
		m_pMoloch->m_AnimFinish = true;
	}



	return m_pMoloch->m_AnimFinish;
}

_bool CMoloch_State::isFinish(void)
{
	if (m_pMoloch->Collision_Player())
	{
		m_pMoloch->m_AnimFinish = false;
	}
	else
	{
		m_pMoloch->m_AnimFinish = true;
	}

	return m_pMoloch->m_AnimFinish;
}

_bool CMoloch_State::isSkillFinish(void)
{

	if (m_pMoloch->Collision_Skill1Range())
	{
		m_pMoloch->m_AnimFinish = false;
	}
	else
	{
		m_pMoloch->m_AnimFinish = true;
	}

	return m_pMoloch->m_AnimFinish;
}

_bool CMoloch_State::isSkill2Finish(void)
{
	if (m_pMoloch->Collision_Skill2Range())
	{
		m_pMoloch->m_AnimFinish = false;
	}
	else
	{
		m_pMoloch->m_AnimFinish = true;
	}

	return m_pMoloch->m_AnimFinish;
}

_bool CMoloch_State::isChase(void)
{
	_bool bResult = m_pMoloch->Collision_ChaseRange();

	return bResult;
}

_bool CMoloch_State::isAttackRange(void)
{
	_bool bResult = m_pMoloch->Collision_Player();

	return bResult;
}

_bool CMoloch_State::isAttackedFinish(void)
{
	if (m_pMoloch->Collision_AttackedRange())
	{
		m_pMoloch->m_AnimFinish = false;

	}
	else
	{
		m_pMoloch->m_AnimFinish = true;
	}

	return m_pMoloch->m_AnimFinish;
}

_bool CMoloch_State::isAttackedRange(void)
{
	_bool bResult = m_pMoloch->Collision_AttackedRange();


	return bResult;
}

_bool CMoloch_State::isSkillRange(void)
{
	_bool bResult = m_pMoloch->Collision_Skill1Range();



	return bResult;

}

_bool CMoloch_State::isSkill2Range(void)
{
	_bool bResult = m_pMoloch->Collision_Skill2Range();
	return bResult;
}



_bool CMoloch_State::isNextAttackB(void)
{
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Atk_Swipe_01)->Get_PlayRate() >= 0.9)
	{


		ENGINE_OPEN;

		ENGINE->Play_Sound(TEXT("MolochAttack,mp3"), 1, false, 1);

		ENGINE_CLOSE;

		if (m_pMoloch->Collision_Player())
		{

	
			m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Swipe_02);
			return true;
		}
		
	}
	else
	{
		return false;
	}
	return m_pMoloch->Collision_Player();
}

_bool CMoloch_State::isNextAttackC(void)
{
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Atk_Swipe_02)->Get_PlayRate() >= 0.9)
	{

		if (m_pMoloch->Collision_Player())
		{

			ENGINE_OPEN;

			ENGINE->Play_Sound(TEXT("MolochAttack2.mp3"), 1, false, 1);

			ENGINE_CLOSE;

			m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Swipe_03);
			return true;
		}

	}
	else
	{
		return false;
	}
	return m_pMoloch->Collision_Player();
}


_bool CMoloch_State::isNextIdle(void)
{
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Atk_Swipe_03)->Get_PlayRate() >= 0.9)
	{

		if (m_pMoloch->Collision_Player())
		{
			m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Full_Idle);
			return true;
		}

	}
	else
	{
		return false;
	}
	return m_pMoloch->Collision_Player();
}



//Skill1

_bool CMoloch_State::isSkillNextB(void)
{
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Atk_Full_Dash_Start)->Get_PlayRate() >= 0.9)
	{
		if (m_pMoloch->Collision_Skill1Range())
		{
			m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Dash);
			return true;
		}
	}
	else
	{
		return false;
	}


	return m_pMoloch->Collision_Skill1Range();
}

_bool CMoloch_State::isSkillNextC(void)
{
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Atk_Full_Dash)->Get_PlayRate() >= 0.9)
	{
		if (m_pMoloch->Collision_Skill1Range())
		{
			m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Dash_Strike);
			return true;
		}
	}
	else
	{
		return false;
	}

	return m_pMoloch->Collision_Skill1Range();
}

_bool CMoloch_State::isSkillNextD(void)
{
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Atk_Full_Dash_Strike)->Get_PlayRate() >= 0.9)
	{
		if (m_pMoloch->Collision_Skill1Range())
		{
			m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Dash_Strike_02);
			return true;
		}
	}
	else
	{
		return false;
	}

	return m_pMoloch->Collision_Skill1Range();
}

_bool CMoloch_State::isSkillNextIdle(void)
{
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Atk_Full_Dash_Strike_02)->Get_PlayRate() >= 0.9)
	{
		if (m_pMoloch->Collision_Skill1Range())
		{
			m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Full_Idle);
			return true;
		}
	}
	else
	{
		return false;
	}

	return m_pMoloch->Collision_Skill1Range();
}

_bool CMoloch_State::isTwoSkillNextB(void)
{
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Atk_Full_Geyser_Start)->Get_PlayRate() >= 0.9)
	{
		if (m_pMoloch->Collision_Skill2Range())
		{
			m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Geyser);
			return true;
		}
	}
	else
	{
		return false;
	}

	return m_pMoloch->Collision_Skill2Range();
}

_bool CMoloch_State::isTwoSkillNextC(void)
{
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Atk_Full_Geyser)->Get_PlayRate() >= 0.9)
	{
		if (m_pMoloch->Collision_Skill2Range())
		{
			m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Atk_Full_Geyser_02);
			return true;
		}
	}
	else
	{
		return false;
	}

	return m_pMoloch->Collision_Skill2Range();
}

_bool CMoloch_State::isTwoSkillIDLE(void)
{
	if (m_pMoloch->m_pModelCom->Get_Animation(Moloch_Atk_Full_Geyser_02)->Get_PlayRate() >= 0.9)
	{
		if (m_pMoloch->Collision_Skill2Range())
		{
			m_pMoloch->m_pModelCom->Set_AnimIndex(Moloch_Full_Idle);
			return true;
		}
	}
	else
	{
		return false;
	}

	return m_pMoloch->Collision_Skill2Range();
}

_bool CMoloch_State::isDead(void)
{
	_bool bResult = m_pMoloch->isDead();
	return  bResult;
}

_bool CMoloch_State::AnimationIntervalChecker(ANIMATION eAnim, _double StartRate, _double FinishRate)
{
	ENGINE_OPEN;

	if (m_pMoloch->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() >= StartRate &&
		m_pMoloch->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() <= FinishRate)
	{
		static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(true);
		static_cast<CCollider*>(m_pMoloch->Get_StoneCollider())->Set_CheckAble(true);
	}
	else
	{
		static_cast<CCollider*>(m_pMoloch->Get_SwordCollider())->Set_CheckAble(false);
		static_cast<CCollider*>(m_pMoloch->Get_StoneCollider())->Set_CheckAble(false);
	}

		
	ENGINE_CLOSE;
	return true;
}

CMoloch_State * CMoloch_State::Create(CMoloch * pMoloch)
{
	CMoloch_State* pInstance = new CMoloch_State();

	if (FAILED(pInstance->Initialize(pMoloch)))
	{
		MSG_BOX("BOOOOMING : CMoloch_State");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMoloch_State::Free()
{
	Safe_Release(m_pMoloch);
	
}



