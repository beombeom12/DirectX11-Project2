#include "stdafx.h"
#include "..\public\Grinner_State.h"
#include "Grinner.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Collider.h"
CGrinner_State::CGrinner_State()
{
}

HRESULT CGrinner_State::Initialize(CGrinner * pGrinner)
{
	m_pGrinner = pGrinner;
	
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

	m_bNext = false;
	
	return S_OK;
}

HRESULT CGrinner_State::SetUp_State_Idle()
{
	if (nullptr == m_pGrinner->m_pStateCom)
		return E_FAIL;


	m_pGrinner->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CGrinner_State::Start_Idle)
		.Init_Tick(this, &CGrinner_State::Tick_Idle)
		.Init_End(this, &CGrinner_State::End_Common)
		.Init_Changer(TEXT("STATE::WALKF"), this, &CGrinner_State::isWalk)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CGrinner_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCHA"), this, &CGrinner_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CGrinner_State::isDead)
		.Finish_Setting();
	return S_OK;
}

HRESULT CGrinner_State::SetUp_State_Walk()
{
	if (nullptr == m_pGrinner->m_pStateCom)
		return E_FAIL;


	m_pGrinner->m_pStateCom->
		Add_State(TEXT("STATE::WALKF"))
		.Init_Start(this, &CGrinner_State::Start_Walk)
		.Init_Tick(this, &CGrinner_State::Tick_Walk)
		.Init_End(this, &CGrinner_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGrinner_State::isNone)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CGrinner_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCHA"), this, &CGrinner_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CGrinner_State::isDead)
		.Finish_Setting();

	return S_OK;
}

HRESULT CGrinner_State::SetUp_State_Attack()
{
	m_pGrinner->m_pStateCom->
		Add_State(TEXT("STATE::ATTACKA"))
		.Init_Start(this, &CGrinner_State::Start_AttackA)
		.Init_Tick(this, &CGrinner_State::Tick_AttackA)
		.Init_End(this, &CGrinner_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGrinner_State::isFinish)
		.Init_Changer(TEXT("STATE::FLINCHA"), this, &CGrinner_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CGrinner_State::isDead)
		.Init_Changer(TEXT("STATE::ATTACKB"), this, &CGrinner_State::isNextAttackB)



		.Add_State(TEXT("STATE::ATTACKB"))
		.Init_Start(this, &CGrinner_State::Start_AttackB)
		.Init_Tick(this, &CGrinner_State::Tick_AttackB)
		.Init_End(this, &CGrinner_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGrinner_State::isFinish)
		.Init_Changer(TEXT("STATE::FLINCHA"), this, &CGrinner_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::ATTACKC"), this, &CGrinner_State::isNextAttackC)

		.Add_State(TEXT("STATE::ATTACKC"))
		.Init_Start(this, &CGrinner_State::Start_AttackC)
		.Init_Tick(this, &CGrinner_State::Tick_AttackC)
		.Init_End(this, &CGrinner_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGrinner_State::isFinish)
		.Init_Changer(TEXT("STATE::FLINCHA"), this, &CGrinner_State::isAttackedRange)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CGrinner_State::isNextIdle)
		.Finish_Setting();



	return S_OK;
}

HRESULT CGrinner_State::SetUp_State_Flinch()
{

	if (nullptr == m_pGrinner->m_pStateCom)
		return E_FAIL;


	m_pGrinner->m_pStateCom->
		Add_State(TEXT("STATE::FLINCHA"))
		.Init_Start(this, &CGrinner_State::Start_FlinchA)
		.Init_Tick(this, &CGrinner_State::Tick_FlinchA)
		.Init_End(this, &CGrinner_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGrinner_State::isAttackedFinish)
		//.Init_Changer(TEXT("STATE::ATTACKA"), this, &CGrinner_State::isAttackRange)
		//.Init_Changer(TEXT("STATE::DEAD"), this, &CGrinner_State::isDead)
		.Finish_Setting();

	return S_OK;
}

HRESULT CGrinner_State::SetUp_State_Dead()
{

	if (nullptr == m_pGrinner->m_pStateCom)
		return E_FAIL;


	m_pGrinner->m_pStateCom->
		Add_State(TEXT("STATE::DEAD"))
		.Init_Start(this, &CGrinner_State::Start_Dead)
		.Init_Tick(this, &CGrinner_State::Tick_Dead)
		.Init_End(this, &CGrinner_State::End_Common)

		.Finish_Setting();
		return S_OK;
}

void CGrinner_State::Start_Idle(_double TimeDelta)
{

	m_pGrinner->m_fGrinnerAnim = 0.1f;
	m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_Idle);

}

void CGrinner_State::Start_Walk(_double TimeDelta)
{
	m_pGrinner->m_fGrinnerAnim = 0.05f;
	m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_Walk_F);
}


void CGrinner_State::Start_AttackA(_double TimeDelta)
{
	m_pGrinner->m_fGrinnerAnim = 0.05;
	m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_Atk_Lunge);


}

void CGrinner_State::Start_AttackB(_double TimeDelta)
{
	m_pGrinner->m_fGrinnerAnim = 0.05;
	m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_Atk_Swipe_Combo);
}

void CGrinner_State::Start_AttackC(_double TimeDelta)
{
	m_pGrinner->m_fGrinnerAnim = 0.05;
	m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_Atk_Flip);
}

void CGrinner_State::Start_FlinchA(_double TimeDelta)
{
	m_pGrinner->m_fGrinnerAnim = 0.05f;
	m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_Impact_F);
}

void CGrinner_State::Start_Dead(_double TimeDelta)
{
	m_pGrinner->m_fGrinnerAnim = 0.02f;
	m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_Death);
	m_pGrinner->m_pTransformCom->Camera_GoBackWard(TimeDelta * 10.f);
	
}

void CGrinner_State::Tick_Idle(_double TimeDelta)
{
	static_cast<CCollider*>(m_pGrinner->Get_RightSwordCollider())->Set_CheckAble(false);
	//static_cast<CCollider*>(m_pGrinner->Get_LeftSwordCollider())->Set_CheckAble(false);


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pGrinner->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CGrinner_State::Tick_Walk(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pGrinner->m_pTransformCom->Chase(vPlayerPos, TimeDelta * 0.07f);

	m_pGrinner->m_pTransformCom->LookAt(vPlayerPos);


	RELEASE_INSTANCE(CGameInstance);
}



void CGrinner_State::Tick_AttackA(_double TimeDelta)
{
	AnimIntervalChecker(Grinner_Atk_Lunge, 0.4, 0.8);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pGrinner->m_pTransformCom->LookAt(vPlayerPos);
	
	RELEASE_INSTANCE(CGameInstance);
}

void CGrinner_State::Tick_AttackB(_double TimeDelta)
{
	AnimIntervalChecker(Grinner_Atk_Swipe_Combo, 0.4, 0.8);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);	
	m_pGrinner->m_pTransformCom->LookAt(vPlayerPos);



	RELEASE_INSTANCE(CGameInstance);
}

void CGrinner_State::Tick_AttackC(_double TimeDelta)
{
	AnimIntervalChecker(Grinner_Atk_Flip, 0.5, 0.9);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pGrinner->m_pTransformCom->LookAt(vPlayerPos);

		



	RELEASE_INSTANCE(CGameInstance);
}


void CGrinner_State::Tick_FlinchA(_double TimeDelta)
{
	AnimIntervalChecker(Grinner_Impact_F, 0.0, 0.0);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pGrinner->m_pTransformCom->LookAt(vPlayerPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CGrinner_State::Tick_Dead(_double TimeDelta)
{

	m_pGrinner->m_pMonsterAbility->fCurrentHP = 0.f;


	if (m_pGrinner->m_pModelCom->Get_Animation(Grinner_Death)->Get_PlayRate() >= 0.99 && m_pGrinner->m_pMonsterAbility->fCurrentHP == 0.f)
	{
			m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_DeadPos);
	}

}

void CGrinner_State::End_Common(_double TimeDelta)
{
}

_bool CGrinner_State::isNone(void)
{

	if (m_pGrinner->Collision_ChaseRange())
	{
		m_pGrinner->m_AnimFinish = false;
	}
	else
	{
		m_pGrinner->m_AnimFinish = true;
	}
	return m_pGrinner->m_AnimFinish;
}

_bool CGrinner_State::isFinish(void)
{



	if (m_pGrinner->Collision_Player())
	{
		m_pGrinner->m_AnimFinish = false;
	}
	else
	{
		m_pGrinner->m_AnimFinish = true;
		
	}

	return m_pGrinner->m_AnimFinish;



}

_bool CGrinner_State::isAttackedFinish(void)
{
	if (m_pGrinner->Collision_AttackedRange())
	{
		m_pGrinner->m_AnimFinish = false;
	}
	else
	{
		m_pGrinner->m_AnimFinish = true;

	}
	return m_pGrinner->m_AnimFinish;
}


_bool CGrinner_State::AnimFinishChecker(ANIMATION eAnim, _double FinishRate)
{
	return m_pGrinner->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() >= FinishRate;
	
}

_bool CGrinner_State::isWalk(void)
{

	 _bool bResult = m_pGrinner->Collision_ChaseRange();

	 return bResult;

}

_bool CGrinner_State::isAttackRange(void)
{
	_bool bResult = m_pGrinner->Collision_Player();

	return bResult;
}

_bool CGrinner_State::isAttackedRange(void)
{

	_bool bResult = m_pGrinner->Collision_AttackedRange();



	return bResult;
}

_bool CGrinner_State::isNextAttackB(void)
{
   

	if (m_pGrinner->m_pModelCom->Get_Animation(Grinner_Atk_Lunge)->Get_PlayRate() >= 0.8)
	{
		if (m_pGrinner->Collision_Player())
		{
			m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_Atk_Swipe_Combo);

			return true; 
		}
	}
	else
	{
		return false;
	}

	return m_pGrinner->Collision_Player();
}

_bool CGrinner_State::isNextAttackC(void)
{
	if (m_pGrinner->m_pModelCom->Get_Animation(Grinner_Atk_Swipe_Combo)->Get_PlayRate() >= 0.8)
	{
		if (m_pGrinner->Collision_Player())
		{
			m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_Atk_Lunge);

			return true;
		}
	}
	else
	{
		return false; 
	}

	return m_pGrinner->Collision_Player();

}

_bool CGrinner_State::isNextIdle(void)
{
	if (m_pGrinner->m_pModelCom->Get_Animation(Grinner_Atk_Flip)->Get_PlayRate() >= 0.8)
	{
		if (m_pGrinner->Collision_Player())
		{
			m_pGrinner->m_pModelCom->Set_AnimIndex(Grinner_Atk_Flip);

			return true;
		}
	}
	else
	{
		return false;
	}

	return m_pGrinner->Collision_Player();
}

_bool CGrinner_State::isDead(void)
{

	_bool bResult = m_pGrinner->isDead();

	return bResult;

}

_bool CGrinner_State::AnimIntervalChecker(ANIMATION eAnim, _double StartRate, _double FinishRate)
{
	ENGINE_OPEN;


	if (m_pGrinner->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() >= StartRate &&
		m_pGrinner->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() <= FinishRate)
	{
		static_cast<CCollider*>(m_pGrinner->Get_RightSwordCollider())->Set_CheckAble(true);
	//	static_cast<CCollider*>(m_pGrinner->Get_LeftSwordCollider())->Set_CheckAble(true);
	}
	else
	{                 
		static_cast<CCollider*>(m_pGrinner->Get_RightSwordCollider())->Set_CheckAble(false);
	//	static_cast<CCollider*>(m_pGrinner->Get_LeftSwordCollider())->Set_CheckAble(false);
	}


	ENGINE_CLOSE;
	return true;
}


CGrinner_State * CGrinner_State::Create(CGrinner * pGrinner)
{

	CGrinner_State* pInstnace = new CGrinner_State();
	if (FAILED(pInstnace->Initialize(pGrinner)))
	{
		MSG_BOX("BOOM");
		Safe_Release(pInstnace);
	}
	return pInstnace;
}

void CGrinner_State::Free()
{


	Safe_Release(m_pGrinner);

}
