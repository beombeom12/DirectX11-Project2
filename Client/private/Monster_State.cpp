#include "stdafx.h"
#include "..\public\Monster_State.h"
#include "Monster.h"
#include "GameInstance.h"


CMonster_State::CMonster_State()
{
}

HRESULT CMonster_State::Initialize(CMonster * Monster)
{
	m_pMonster = Monster;

	if (FAILED(SetUp_State_Idle()))
		return E_FAIL;

	if (FAILED(SetUp_State_Walk()))
		return E_FAIL;
	//if (FAILED(SetUp_State_Attack()))
	//	return E_FAIL;

	//if (FAILED(SetUp_State_Damaged()))
	//	return E_FAIL;

	//if (FAILED(SetUp_State_Deth()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CMonster_State::SetUp_State_Idle()
{
	if (nullptr == m_pMonster->m_pStateCom)
		return E_FAIL;


	m_pMonster->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CMonster_State::Start_Idle)
		.Init_Tick(this, &CMonster_State::Tick_Idle)
		.Init_End(this, &CMonster_State::End_Common)
		//.Init_Changer(TEXT("STATE::ATTACKL"), this, &CMonster_State::isAttacking)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CMonster_State::isWalking)
		.Finish_Setting();
	return S_OK;
}

HRESULT CMonster_State::SetUp_State_Walk()
{
	if (nullptr == m_pMonster->m_pStateCom)
		return E_FAIL;


	m_pMonster->m_pStateCom->
		Add_State(TEXT("STATE::WALKF"))
		.Init_Start(this, &CMonster_State::Start_Walk)
		.Init_Tick(this, &CMonster_State::Tick_Walk)
		.Init_End(this, &CMonster_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMonster_State::isFinish)
	//	.Init_Changer(TEXT("STATE::ATTACKL"), this, &CMonster_State::isAttacking)
		.Finish_Setting();
	return S_OK;
}

HRESULT CMonster_State::SetUp_State_Attack()
{
	if (nullptr == m_pMonster->m_pStateCom)
		return E_FAIL;


	m_pMonster->m_pStateCom->
		Add_State(TEXT("STATE::ATTACKL"))
		.Init_Start(this, &CMonster_State::Start_Attack)
		.Init_Tick(this, &CMonster_State::Tick_Attack)
		.Init_End(this, &CMonster_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMonster_State::isFinish)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CMonster_State::isWalking)
		.Finish_Setting();
	return S_OK;
}

HRESULT CMonster_State::SetUp_State_Damaged()
{
	return S_OK;
}

HRESULT CMonster_State::SetUp_State_Deth()
{
	return S_OK;
}

void CMonster_State::Start_Idle(_double TimeDelta)
{
	m_pMonster->m_pModelCom->Set_AnimIndex(ScarabHulk_Idle);
}



void CMonster_State::Start_Walk(_double TimeDelta)
{
	m_pMonster->m_pModelCom->Set_AnimIndex(ScarabHulk_Walk_F);
}



void CMonster_State::Start_Attack(_double TimeDelta)
{ 
	m_pMonster->m_pModelCom->Set_AnimIndex(ScarabHulk_Attack_L);
}



void CMonster_State::Start_Damaged(_double TimeDelta)
{
}




void CMonster_State::Tick_Idle(_double TimeDelta)
{
}

void CMonster_State::Tick_Walk(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	
	m_pMonster->m_pTransformCom->LookAt(vPlayerPos);
	m_pMonster->m_pTransformCom->Chase(vPlayerPos, TimeDelta);
	
	
	
	RELEASE_INSTANCE(CGameInstance);

}

void CMonster_State::Tick_Attack(_double TimeDelta)
{
}

void CMonster_State::Tick_Damaged(_double TimeDelta)
{
}

void CMonster_State::End_Common(_double TimeDleta)
{
}

_bool CMonster_State::isFinish(void)
{


	if (m_pMonster->Collision_ChaseRange())
	{
		m_pMonster->m_AnimFinish = false;
	}
	else
	{
		m_pMonster->m_AnimFinish = true;
	}
	return m_pMonster->m_AnimFinish;
}

_bool CMonster_State::isWalking(void)
{
	//만약 플레이어의 콜라이더와 부딪히면

	_bool bResult = m_pMonster->Collision_ToPlayer();

	return bResult;
}

_bool CMonster_State::isAttacking(void)
{
	_bool bResult = m_pMonster->Collision_Sword();

	return bResult;
}

CMonster_State * CMonster_State::Create(CMonster * Monster)
{
	CMonster_State*		pInstance = new CMonster_State();

	if (FAILED(pInstance->Initialize(Monster)))
	{
		MSG_BOX("Failed to Create : CMonster_State");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_State::Free()
{

	Safe_Release(m_pMonster);
}
