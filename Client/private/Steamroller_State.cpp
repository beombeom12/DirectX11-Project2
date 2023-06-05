#include "stdafx.h"
#include "..\public\Steamroller_State.h"
#include "Steamroller.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Model.h"
#include "Animation.h"

CSteamroller_State::CSteamroller_State()
{
}

HRESULT CSteamroller_State::Initialize(CSteamroller * pSteamroller)
{

	m_pSteamroller = pSteamroller;

	if (FAILED(SetUp_State_Idle()))
		return E_FAIL;
	if (FAILED(SetUp_State_Walk()))
		return E_FAIL;
	if (FAILED(SetUp_State_Atttack()))
		return E_FAIL;
	if (FAILED(SetUp_State_Flinch()))
		return E_FAIL;
	if (FAILED(SetUp_State_Skill()))
		return E_FAIL;

	if (FAILED(SetUp_Staate_Dead()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSteamroller_State::SetUp_State_Idle()
{
	

	m_pSteamroller->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CSteamroller_State::Start_Idle)
		.Init_Tick(this, &CSteamroller_State::Tick_Idle)
		.Init_End(this, &CSteamroller_State::End_Common)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CSteamroller_State::isChase)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CSteamroller_State::isAttackRange)
		.Init_Changer(TEXT("STATE::SKILLA"), this, &CSteamroller_State::isSkillRange)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSteamroller_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSteamroller_State::isDead)
		.Finish_Setting();
	return S_OK;
}

HRESULT CSteamroller_State::SetUp_State_Walk()
{



	m_pSteamroller->m_pStateCom->
		Add_State(TEXT("STATE::WALKF"))
		.Init_Start(this, &CSteamroller_State::Start_Walk)
		.Init_Tick(this, &CSteamroller_State::Tick_Chase)
		.Init_End(this, &CSteamroller_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSteamroller_State::isNone)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CSteamroller_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSteamroller_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::SKILLA"), this, &CSteamroller_State::isSkillRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSteamroller_State::isDead)
		.Finish_Setting();

	return S_OK;
}

HRESULT CSteamroller_State::SetUp_State_Atttack()
{



	m_pSteamroller->m_pStateCom->
		Add_State(TEXT("STATE::ATTACK"))
		.Init_Start(this, &CSteamroller_State::Start_Attack)
		.Init_Tick(this, &CSteamroller_State::Tick_Attack)
		.Init_End(this, &CSteamroller_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSteamroller_State::isFinish)
		.Init_Changer(TEXT("STATE::ATTACKB"), this, &CSteamroller_State::isNextAttackB)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSteamroller_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSteamroller_State::isDead)
		
		.Add_State(TEXT("STATE::ATTACKB"))
		.Init_Start(this, &CSteamroller_State::Start_AttackB)
		.Init_Tick(this, &CSteamroller_State::Tick_AttackB)
		.Init_End(this, &CSteamroller_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSteamroller_State::isFinish)
		.Init_Changer(TEXT("STATE::ATTACKC"), this, &CSteamroller_State::isNextAttackC)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSteamroller_State::isAttackedRange)

		.Init_Changer(TEXT("STATE::DEAD"), this, &CSteamroller_State::isDead)


		.Add_State(TEXT("STATE::ATTACKC"))
		.Init_Start(this, &CSteamroller_State::Start_AttackC)
		.Init_Tick(this, &CSteamroller_State::Tick_AttackC)
		.Init_End(this, &CSteamroller_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSteamroller_State::isFinish)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSteamroller_State::isAttackedRange)

		.Init_Changer(TEXT("STATE::DEAD"), this, &CSteamroller_State::isDead)
		.Init_Changer(TEXT("STATE::SKILLA"), this, &CSteamroller_State::isSkillRange)
		.Finish_Setting();

	return S_OK;
}

HRESULT CSteamroller_State::SetUp_State_Skill()
{




	m_pSteamroller->m_pStateCom->
		Add_State(TEXT("STATE::SKILLA"))
		.Init_Start(this, &CSteamroller_State::Start_SkillAttack)
		.Init_Tick(this, &CSteamroller_State::Tick_SkillAttackA)
		.Init_End(this, &CSteamroller_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSteamroller_State::isSkillFinish)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSteamroller_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSteamroller_State::isDead)
		.Init_Changer(TEXT("STATE::SKILLB"), this, &CSteamroller_State::isNextSkillB)

		
		.Add_State(TEXT("STATE::SKILLB"))
		.Init_Start(this, &CSteamroller_State::Start_SkillAttackB)
		.Init_Tick(this, &CSteamroller_State::Tick_SkillAttackB)
		.Init_End(this, &CSteamroller_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSteamroller_State::isSkillFinish)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSteamroller_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSteamroller_State::isDead)
		.Init_Changer(TEXT("STATE::SKILLC"), this, &CSteamroller_State::isNextSkillC)


		.Add_State(TEXT("STATE::SKILLC"))
		.Init_Start(this, &CSteamroller_State::Start_SkillAttackC)
		.Init_Tick(this, &CSteamroller_State::Tick_SkillAttackC)
		.Init_End(this, &CSteamroller_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSteamroller_State::isSkillFinish)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSteamroller_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSteamroller_State::isDead)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CSteamroller_State::isAttackRange)

		.Finish_Setting();

	return S_OK;
}

HRESULT CSteamroller_State::SetUp_State_Flinch()
{

	


	m_pSteamroller->m_pStateCom->
		Add_State(TEXT("STATE::FLINCH"))
		.Init_Start(this, &CSteamroller_State::Start_Flinch)
		.Init_Tick(this, &CSteamroller_State::Tick_Flinch)
		.Init_End(this, &CSteamroller_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSteamroller_State::isAttackedFinish)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSteamroller_State::isDead)
		.Finish_Setting();


	return S_OK;
}

HRESULT CSteamroller_State::SetUp_Staate_Dead()
{



	m_pSteamroller->m_pStateCom->
		Add_State(TEXT("STATE::DEAD"))
		.Init_Start(this, &CSteamroller_State::Start_Dead)
		.Init_Tick(this, &CSteamroller_State::Tick_Dead)
		.Init_End(this, &CSteamroller_State::End_Common)
		.Finish_Setting();

	return S_OK;
}

void CSteamroller_State::Start_Idle(_double TimeDelta)
{
	m_pSteamroller->m_fSteamAnimationTime = 0.1;
	m_pSteamroller->m_pModelCom->Set_AnimIndex(Steamroller_Taunt_01);
}

void CSteamroller_State::Start_Walk(_double TimeDelta)
{
	m_pSteamroller->m_fSteamAnimationTime = 0.1;
	m_pSteamroller->m_pModelCom->Set_AnimIndex(SteamRoller_Run_F);
}


void CSteamroller_State::Start_Attack(_double TimeDelta)
{
	m_pSteamroller->m_fSteamAnimationTime = 0.1;
	m_pSteamroller->m_pModelCom->Set_AnimIndex(Steamroller_Attack_Slam);
}

void CSteamroller_State::Start_AttackB(_double TimeDelta)
{
	m_pSteamroller->m_fSteamAnimationTime = 0.1;
	m_pSteamroller->m_pModelCom->Set_AnimIndex(Steamroller_Attack_Swing_L);
}

void CSteamroller_State::Start_AttackC(_double TimeDelta)
{
	m_pSteamroller->m_fSteamAnimationTime = 0.1;
	m_pSteamroller->m_pModelCom->Set_AnimIndex(Steamroller_Attack_Swing_R);
}

void CSteamroller_State::Start_SkillAttack(_double TimeDelta)
{
	m_pSteamroller->m_fSteamAnimationTime = 0.1;
	m_pSteamroller->m_pModelCom->Set_AnimIndex(SteamRoller_Roll_Start);
}

void CSteamroller_State::Start_SkillAttackB(_double TimeDelta)
{
	m_pSteamroller->m_fSteamAnimationTime = 0.1;
	m_pSteamroller->m_pModelCom->Set_AnimIndex(SteamRoller_Roll_Start_Fast);
}

void CSteamroller_State::Start_SkillAttackC(_double TimeDelta)
{
	m_pSteamroller->m_fSteamAnimationTime = 0.1;
	m_pSteamroller->m_pModelCom->Set_AnimIndex(SteamRoller_Roll_Stop);

}

void CSteamroller_State::Start_Flinch(_double TimeDelta)
{

	m_pSteamroller->m_fSteamAnimationTime = 0.06;
	m_pSteamroller->m_pModelCom->Set_AnimIndex(Steamroller_Impact_F);



}

void CSteamroller_State::Start_Dead(_double TimeDelta)
{

	m_pSteamroller->m_fSteamAnimationTime = 0.09f;
	m_pSteamroller->m_pModelCom->Set_AnimIndex(Steamroller_Death);
	m_pSteamroller->m_pTransformCom->Camera_GoBackWard(TimeDelta * 10.f);
}

void CSteamroller_State::Tick_Idle(_double TimeDelta)
{
	static_cast<CCollider*>(m_pSteamroller->Get_WeaponCollier())->Set_CheckAble(false);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pSteamroller->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CSteamroller_State::Tick_Walk(_double TimeDelta)
{

}

void CSteamroller_State::Tick_Chase(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);


	m_pSteamroller->m_pTransformCom->Chase(vPlayerPos, TimeDelta * 0.05);
	m_pSteamroller->m_pTransformCom->LookAt(vPlayerPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CSteamroller_State::Tick_Attack(_double TimeDelta)
{


	AnimationChecker(Steamroller_Attack_Slam, 0.4, 0.6);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);



	m_pSteamroller->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CSteamroller_State::Tick_AttackB(_double TimeDelta)
{
	AnimationChecker(Steamroller_Attack_Swing_L, 0.4, 0.6);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);



	m_pSteamroller->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CSteamroller_State::Tick_AttackC(_double TimeDelta)
{
	AnimationChecker(Steamroller_Attack_Swing_R, 0.4, 0.6);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);



	m_pSteamroller->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CSteamroller_State::Tick_SkillAttackA(_double TimeDelta)
{
	
}

void CSteamroller_State::Tick_SkillAttackB(_double TimeDelta)
{
	AnimationChecker(SteamRoller_Roll_Start_Fast, 0.5, 0.6);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);


	m_pSteamroller->m_pTransformCom->Chase(vPlayerPos, TimeDelta * 0.1);

	m_pSteamroller->m_pTransformCom->LookAt(vPlayerPos);


	RELEASE_INSTANCE(CGameInstance);
}

void CSteamroller_State::Tick_SkillAttackC(_double TimeDelta)
{
	
}

void CSteamroller_State::Tick_Flinch(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);



	m_pSteamroller->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CSteamroller_State::Tick_Dead(_double TimeDelta)
{
	m_pSteamroller->m_pMonsterAbility->fCurrentHP = 0.f;

	if (m_pSteamroller->m_pModelCom->Get_Animation(Steamroller_Death)->Get_PlayRate() >= 0.99)
	{
		static_cast<CCollider*>(m_pSteamroller->Get_WeaponCollier())->Set_CheckAble(false);
		m_pSteamroller->m_pModelCom->Set_AnimIndex(Steamroller_Deadpose);
	}
}

void CSteamroller_State::End_Common(_double TimeDelta)
{
}

_bool CSteamroller_State::isNone(void)
{
	if (m_pSteamroller->Collision_ChaseRange())
	{
		m_pSteamroller->m_AnimFinish = false;
	}
	else
	{
		m_pSteamroller->m_AnimFinish = true;
	}
	return m_pSteamroller->m_AnimFinish;
}

_bool CSteamroller_State::isFinish(void)
{
	if (m_pSteamroller->Collision_Player())
	{
		m_pSteamroller->m_AnimFinish = false;
	
	}
	else
	{
		m_pSteamroller->m_AnimFinish = true;

	}

	return m_pSteamroller->m_AnimFinish;
}

_bool CSteamroller_State::isSkillFinish(void)
{

	if (m_pSteamroller->Collision_SkillRanage())
	{
		m_pSteamroller->m_AnimFinish = false;
	}
	else
	{
		m_pSteamroller->m_AnimFinish = true;
		
	}

	return m_pSteamroller->m_AnimFinish;
}

_bool CSteamroller_State::isAttackedFinish(void)
{
	if (m_pSteamroller->Collision_AttackedRange())
	{
		m_pSteamroller->m_AnimFinish = false;

	}
	else
	{
		m_pSteamroller->m_AnimFinish = true;
	}

	return m_pSteamroller->m_AnimFinish;
}

_bool CSteamroller_State::isChase(void)
{

	_bool bResult = m_pSteamroller->Collision_ChaseRange();

	return bResult;
}

_bool CSteamroller_State::isAttackRange(void)
{
	_bool bResult = m_pSteamroller->Collision_Player();

	return bResult;
}

_bool CSteamroller_State::isAttackedRange(void)
{
	_bool bResult = m_pSteamroller->Collision_AttackedRange();
	return bResult;
}

_bool CSteamroller_State::isSkillRange(void)
{
	_bool bResult = m_pSteamroller->Collision_SkillRanage();
	return bResult;
}

_bool CSteamroller_State::isFlinch(void)
{
	return _bool();
}

_bool CSteamroller_State::isNextAttackB(void)
{
	if (m_pSteamroller->m_pModelCom->Get_Animation(Steamroller_Attack_Slam)->Get_PlayRate() >= 0.9)
	{

		if (m_pSteamroller->Collision_Player())
		{

			m_pSteamroller->m_pModelCom->Set_AnimIndex(Steamroller_Attack_Swing_L);
			return true;
		}



	}
	else
	{
		return false;
	}


	return m_pSteamroller->Collision_Player();
}

_bool CSteamroller_State::isNextAttackC(void)
{
	if (m_pSteamroller->m_pModelCom->Get_Animation(Steamroller_Attack_Swing_L)->Get_PlayRate() >= 0.9)
	{

		if (m_pSteamroller->Collision_Player())
		{
			m_pSteamroller->m_pModelCom->Set_AnimIndex(Steamroller_Attack_Swing_R);
			return true;
		}



	}
	else
	{
		return false;
	}


	return m_pSteamroller->Collision_Player();
}

_bool CSteamroller_State::isNextSkillB(void)
{

	if (m_pSteamroller->m_pModelCom->Get_Animation(SteamRoller_Roll_Start)->Get_PlayRate() >= 0.9)
	{
		if (m_pSteamroller->Collision_SkillRanage())
		{
			m_pSteamroller->m_pModelCom->Set_AnimIndex(SteamRoller_Roll_Start_Fast);
			return true;

		}
		return false;
	}
	else
	{
		return false;
	}
}

_bool CSteamroller_State::isNextSkillC(void)
{
	if (m_pSteamroller->m_pModelCom->Get_Animation(SteamRoller_Roll_Start_Fast)->Get_PlayRate() >= 0.9)
	{
		if (m_pSteamroller->Collision_SkillRanage())
		{

			m_pSteamroller->m_pModelCom->Set_AnimIndex(SteamRoller_Roll_Stop);
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}
}

_bool CSteamroller_State::isDead(void)
{
	_bool bResult = m_pSteamroller->isDead();

	return bResult; 
}

_bool CSteamroller_State::AnimationChecker(ANIMATION eAnim, _double StartRate, _double FinalRate)
{
	if (m_pSteamroller->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() >= StartRate&&
		m_pSteamroller->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() <= FinalRate)
	{

		static_cast<CCollider*>(m_pSteamroller->Get_WeaponCollier())->Set_CheckAble(true);


	}
	else
	{
		static_cast<CCollider*>(m_pSteamroller->Get_WeaponCollier())->Set_CheckAble(false);
	}

	return true;
}

CSteamroller_State * CSteamroller_State::Create(CSteamroller * pSteamroller)
{
	CSteamroller_State* pInstnace = new CSteamroller_State();
	if (FAILED(pInstnace->Initialize(pSteamroller)))
	{
		MSG_BOX("BOOM");
		Safe_Release(pInstnace);
	}
	return pInstnace;
}

void CSteamroller_State::Free()
{
	Safe_Release(m_pSteamroller);
}
