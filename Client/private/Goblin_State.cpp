#include "stdafx.h"
#include "..\public\Goblin_State.h"
#include "Goblin.h"
#include "Model.h"
#include "Animation.h"

CGoblin_State::CGoblin_State()
{
}

HRESULT CGoblin_State::Initialize(CGoblin * pGoblin)
{

	m_pGoblin = pGoblin;

	if (FAILED(SetUp_State_Idle()))
		return E_FAIL;

	if (FAILED(SetUp_State_Walk()))
		return E_FAIL;
	
	if (FAILED(SetUp_State_Attack()))
		return E_FAIL;

	if (FAILED(SetUp_State_Flinch()))
		return E_FAIL;
	
	if (FAILED(SetUp_Satee_Dead()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGoblin_State::SetUp_State_Idle()
{


	if (nullptr == m_pGoblin->m_pStateCom)
		return E_FAIL;


	m_pGoblin->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CGoblin_State::Start_Idle)
		.Init_Tick(this, &CGoblin_State::Tick_Idle)
		.Init_End(this, &CGoblin_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK"), this, &CGoblin_State::isChase)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CGoblin_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CGoblin_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CGoblin_State::isDead)
		.Finish_Setting();
	return S_OK;

}

HRESULT CGoblin_State::SetUp_State_Walk()
{

	m_pGoblin->m_pStateCom->
		Add_State(TEXT("STATE::WALK"))
		.Init_Start(this, &CGoblin_State::Start_Walk)
		.Init_Tick(this, &CGoblin_State::Tick_Walk)
		.Init_End(this, &CGoblin_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGoblin_State::isNone)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CGoblin_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CGoblin_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CGoblin_State::isDead)
		.Finish_Setting();

	return S_OK;
}

HRESULT CGoblin_State::SetUp_State_Attack()
{
	m_pGoblin->m_pStateCom->
		Add_State(TEXT("STATE::ATTACK"))
		.Init_Start(this, &CGoblin_State::Start_Attack)
		.Init_Tick(this, &CGoblin_State::Tick_Attack)
		.Init_End(this, &CGoblin_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGoblin_State::isFinish)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CGoblin_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CGoblin_State::isDead)

		.Finish_Setting();

	return S_OK;
}

HRESULT CGoblin_State::SetUp_State_Flinch()
{
	m_pGoblin->m_pStateCom->
		Add_State(TEXT("STATE::FLINCH"))
		.Init_Start(this, &CGoblin_State::Start_Flinch)
		.Init_Tick(this, &CGoblin_State::Tick_Flinch)
		.Init_End(this, &CGoblin_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGoblin_State::isAttackedFinish)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CGoblin_State::isDead)


		.Finish_Setting();

	return S_OK;
}

HRESULT CGoblin_State::SetUp_Satee_Dead()
{
	m_pGoblin->m_pStateCom->
		Add_State(TEXT("STATE::DEAD"))
		.Init_Start(this, &CGoblin_State::Start_Dead)
		.Init_Tick(this, &CGoblin_State::Tick_Dead)
		.Init_End(this, &CGoblin_State::End_Common)
		.Finish_Setting();

	return S_OK;
}


void CGoblin_State::Start_Idle(_double TimeDelta)
{
	m_pGoblin->m_fGoblinTime = 0.07f;
	m_pGoblin->m_pModelCom->Set_AnimIndex(Goblin_Idle);
}

void CGoblin_State::Start_Walk(_double TimeDelta)
{
	m_pGoblin->m_fGoblinTime = 0.04f;
	m_pGoblin->m_pModelCom->Set_AnimIndex(Goblin_Run_F);
}

void CGoblin_State::Start_Attack(_double TimeDelta)
{
	m_pGoblin->m_fGoblinTime = 0.05f;
	m_pGoblin->m_pModelCom->Set_AnimIndex(Goblin_Attack_01);
}

void CGoblin_State::Start_Flinch(_double TimeDelta)
{
	m_pGoblin->m_fGoblinTime = 0.07f;
	m_pGoblin->m_pModelCom->Set_AnimIndex(Goblin_Impact_F);
}

void CGoblin_State::Start_Dead(_double TimeDelta)
{
	m_pGoblin->m_fGoblinTime = 0.04f;
	m_pGoblin->m_pModelCom->Set_AnimIndex(Goblin_Death_01);
}

void CGoblin_State::Tick_Idle(_double TimeDelta)
{
	static_cast<CCollider*>(m_pGoblin->Get_SwordCollider())->Set_CheckAble(false);
	//m_pGoblin->m_pHit[GOBLIN_HIT]->Update(m_pGoblin->m_pTransformCom->Get_tWorldMatrix());

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pGoblin->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);

}

void CGoblin_State::Tick_Walk(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);


	m_pGoblin->m_pTransformCom->Chase(vPlayerPos, TimeDelta * 0.06);
	m_pGoblin->m_pTransformCom->LookAt(vPlayerPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CGoblin_State::Tick_Attack(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);

	AnimationChecker(Goblin_Attack_01, 0.7, 0.9);

	m_pGoblin->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CGoblin_State::Tick_Flinch(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pGoblin->m_pTransformCom->LookAt(vPlayerPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CGoblin_State::Tick_Dead(_double Timedelta)
{
	m_pGoblin->m_pMonsterAbility->fCurrentHP = 0.f;
	static_cast<CCollider*>(m_pGoblin->Get_SwordCollider())->Set_CheckAble(false);
	if (m_pGoblin->m_pModelCom->Get_Animation(Goblin_Death_01)->Get_PlayRate() >= 0.9)
	{
		static_cast<CCollider*>(m_pGoblin->Get_SwordCollider())->Set_CheckAble(false);
		m_pGoblin->m_pModelCom->Set_AnimIndex(Goblin_DeadPose);
		
	}

}

void CGoblin_State::End_Common(_double TimeDelta)
{
}

_bool CGoblin_State::isNone(void)
{
	if (m_pGoblin->Collision_ChaseRange())
	{
		m_pGoblin->m_AnimFinish = false;
	}
	else
	{
		m_pGoblin->m_AnimFinish = true;
	}



	return m_pGoblin->m_AnimFinish;
}

_bool CGoblin_State::isFinish(void)
{

	if (m_pGoblin->Collision_Player())
	{
		m_pGoblin->m_AnimFinish = false;
	}
	else
	{
		m_pGoblin->m_AnimFinish = true;
	}

	return m_pGoblin->m_AnimFinish;
}

_bool CGoblin_State::isAttackedFinish(void)
{
	if (m_pGoblin->Collision_AttackedRange())
	{
		m_pGoblin->m_AnimFinish = false;

	}
	else
	{
		m_pGoblin->m_AnimFinish = true;

	}

	return m_pGoblin->m_AnimFinish;
}

_bool CGoblin_State::isChase(void)
{
	_bool bResult = m_pGoblin->Collision_ChaseRange();


	return bResult;
}

_bool CGoblin_State::isAttackRange(void)
{
	_bool	bResult = m_pGoblin->Collision_Player();
	return bResult;
}

_bool CGoblin_State::isAttackedRange(void)
{
	_bool bResult = m_pGoblin->Collision_AttackedRange();
	
	return bResult;
}

_bool CGoblin_State::isDead(void)
{
	_bool bResult = m_pGoblin->isDead();
	return  bResult;
}

_bool CGoblin_State::AnimationChecker(ANIMATION eAnim, _double StartRate, _double FinishRate)
{
	ENGINE_OPEN;

	if (m_pGoblin->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() >= StartRate &&
		m_pGoblin->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() <= FinishRate)
	{
		static_cast<CCollider*>(m_pGoblin->Get_SwordCollider())->Set_CheckAble(true);
	
	
	}
	else
	{
		static_cast<CCollider*>(m_pGoblin->Get_SwordCollider())->Set_CheckAble(false);
	}


	ENGINE_CLOSE;


	return true;
}

CGoblin_State * CGoblin_State::Create(CGoblin * pGoblin)
{
	CGoblin_State* pInstnace = new CGoblin_State();
	if (FAILED(pInstnace->Initialize(pGoblin)))
	{
		MSG_BOX("BOOM");
		Safe_Release(pInstnace);
	}
	return pInstnace;
}

void CGoblin_State::Free()
{
	Safe_Release(m_pGoblin);
}


