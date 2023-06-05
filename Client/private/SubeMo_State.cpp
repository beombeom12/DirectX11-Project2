#include "stdafx.h"
#include "..\public\SubeMo_State.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Collider.h"
#include "SubMo.h"

CSubeMo_State::CSubeMo_State()
{
}

HRESULT CSubeMo_State::Initialize(CSubMo * pSubMo)
{
	m_pSubMo = pSubMo;
	if (FAILED(SetUp_State_Idle()))
		return E_FAIL;

	if (FAILED(SetUp_State_Walk()))
		return E_FAIL;

	if (FAILED(SetUp_State_Attack()))
		return E_FAIL;

	if (FAILED(SetUp_State_Flinch()))
		return E_FAIL;

	if (FAILED(SetUp_State_Dead()))
		return E_FAIL;
	return S_OK;
}

HRESULT CSubeMo_State::SetUp_State_Idle()
{
	if (nullptr == m_pSubMo->m_pStateCom)
		return E_FAIL;


	m_pSubMo->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CSubeMo_State::Start_Idle)
		.Init_Tick(this, &CSubeMo_State::Tick_Idle)
		.Init_End(this, &CSubeMo_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK"), this, &CSubeMo_State::isWalk)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CSubeMo_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSubeMo_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSubeMo_State::isDead)
		.Finish_Setting();
	return S_OK;
}

HRESULT CSubeMo_State::SetUp_State_Walk()
{
	m_pSubMo->m_pStateCom
		->Set_Root(TEXT("STATE::WALK"))

		.Add_State(TEXT("STATE::WALK"))
		.Init_Start(this, &CSubeMo_State::Start_Walk)
		.Init_Tick(this, &CSubeMo_State::Tick_Walk)
		.Init_End(this, &CSubeMo_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSubeMo_State::isNone)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CSubeMo_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSubeMo_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSubeMo_State::isDead)
		.Finish_Setting();
	return S_OK;
}

HRESULT CSubeMo_State::SetUp_State_Attack()
{
	m_pSubMo->m_pStateCom->
		Add_State(TEXT("STATE::ATTACKA"))
		.Init_Start(this, &CSubeMo_State::Start_AttackA)
		.Init_Tick(this, &CSubeMo_State::Tick_AttackA)
		.Init_End(this, &CSubeMo_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSubeMo_State::isFinish)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CSubeMo_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSubeMo_State::isDead)
		.Finish_Setting();

	return S_OK;
}
HRESULT CSubeMo_State::SetUp_State_Flinch()
{
	m_pSubMo->m_pStateCom->
		Add_State(TEXT("STATE::FLINCH"))
		.Init_Start(this, &CSubeMo_State::Start_Flinch)
		.Init_Tick(this, &CSubeMo_State::Tick_Flinch)
		.Init_End(this, &CSubeMo_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CSubeMo_State::isAttackedFinish)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CSubeMo_State::isDead)
		.Finish_Setting();

	return S_OK;
}
HRESULT CSubeMo_State::SetUp_State_Dead()
{
	m_pSubMo->m_pStateCom->
		Add_State(TEXT("STATE::DEAD"))
		.Init_Start(this, &CSubeMo_State::Start_Dead)
		.Init_Tick(this, &CSubeMo_State::Tick_Dead)
		.Init_End(this, &CSubeMo_State::End_Common)
		.Finish_Setting();
	return S_OK;
}
void CSubeMo_State::Start_Idle(_double TimeDelta)
{
	m_pSubMo->m_fAdjustTime = 0.1f;
	m_pSubMo->m_pModelCom->Set_AnimIndex(Grub_Idle);
}

void CSubeMo_State::Start_Walk(_double TimeDelta)
{
	m_pSubMo->m_fAdjustTime = 0.1f;
	m_pSubMo->m_pModelCom->Set_AnimIndex(Grub_Run_F);
}

void CSubeMo_State::Start_AttackA(_double TimeDelta)
{
	m_pSubMo->m_fAdjustTime = 0.03f;
	m_pSubMo->m_pModelCom->Set_AnimIndex(Grub_Atk_Bite);
}

void CSubeMo_State::Start_Flinch(_double TimeDelta)
{
	m_pSubMo->m_fAdjustTime = 0.05f;
	m_pSubMo->m_pModelCom->Set_AnimIndex(Grub_Impact_F);
}

void CSubeMo_State::Start_Dead(_double TimeDelta)
{
	m_pSubMo->m_fAdjustTime = 0.06f;
	m_pSubMo->m_pModelCom->Set_AnimIndex(Grub_Death);
}

void CSubeMo_State::Tick_Idle(_double TimeDelta)
{

	static_cast<CCollider*>(m_pSubMo->Get_AttackCollier())->Set_CheckAble(false);


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pSubMo->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CSubeMo_State::Tick_Walk(_double TimeDelta)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);


	m_pSubMo->m_pTransformCom->Chase(vPlayerPos, TimeDelta  * 0.05);

	m_pSubMo->m_pTransformCom->LookAt(vPlayerPos);


	RELEASE_INSTANCE(CGameInstance);
}

void CSubeMo_State::Tick_AttackA(_double TimeDelta)
{

	AnimIntervalChecker(Grub_Atk_Bite, 0.5, 0.9);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pSubMo->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CSubeMo_State::Tick_Flinch(_double TimeDelta)
{
}

void CSubeMo_State::Tick_Dead(_double TimeDelta)
{

	m_pSubMo->m_pMonsterAbility->fCurrentHP = 0.f;
	static_cast<CCollider*>(m_pSubMo->Get_AttackCollier())->Set_CheckAble(false);
	if (m_pSubMo->m_pModelCom->Get_Animation(Grub_Death)->Get_PlayRate() >= 0.99 && m_pSubMo->m_pMonsterAbility->fCurrentHP == 0)
	{
		m_pSubMo->m_pModelCom->Set_AnimIndex(Grub_Dead_Pose);
	}

}

void CSubeMo_State::End_Common(_double TimeDelta)
{
}

_bool CSubeMo_State::isNone(void)
{
	if (m_pSubMo->Collision_ChaseRange())
	{
		m_pSubMo->m_AnimFinish = false;
	}
	else
	{
		m_pSubMo->m_AnimFinish = true;
	}
	return m_pSubMo->m_AnimFinish;
}

_bool CSubeMo_State::isWalk(void)
{
	_bool bResult = m_pSubMo->Collision_ChaseRange();

	return bResult;
}

_bool CSubeMo_State::isFinish(void)
{
	if (m_pSubMo->Collision_Player())
	{
		m_pSubMo->m_AnimFinish = false;
	}
	else
	{
		m_pSubMo->m_AnimFinish = true;

	}

	return m_pSubMo->m_AnimFinish;

}

_bool CSubeMo_State::isAttackRange(void)
{
	_bool bResult = m_pSubMo->Collision_Player();

	return bResult;
}

_bool CSubeMo_State::isAttackedRange(void)
{
	_bool bResult = m_pSubMo->Collision_AttackedRange();



	return bResult;
}

_bool CSubeMo_State::isAttackedFinish(void)
{

	if (m_pSubMo->Collision_AttackedRange())
	{
		m_pSubMo->m_AnimFinish = false;

	}
	else
	{
		m_pSubMo->m_AnimFinish = true;
	}

	return m_pSubMo->m_AnimFinish;
}

_bool CSubeMo_State::isDead(void)
{
	_bool  bResult = m_pSubMo->isDead();

	return bResult;
}

_bool CSubeMo_State::AnimIntervalChecker(ANIMATION eAnim, _double StartRate, _double FinishRate)
{

	if (m_pSubMo->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() >= StartRate &&
		m_pSubMo->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() <= FinishRate)
	{
		static_cast<CCollider*>(m_pSubMo->Get_AttackCollier())->Set_CheckAble(true);
	}
	else
	{
		static_cast<CCollider*>(m_pSubMo->Get_AttackCollier())->Set_CheckAble(false);
	}


	return true;
}

CSubeMo_State * CSubeMo_State::Create(CSubMo * pSubMo)
{
	CSubeMo_State* pInstnace = new CSubeMo_State();
	if (FAILED(pInstnace->Initialize(pSubMo)))
	{
		MSG_BOX("BOOM");
		Safe_Release(pInstnace);
	}
	return pInstnace;
}

void CSubeMo_State::Free()
{
	Safe_Release(m_pSubMo);
}


