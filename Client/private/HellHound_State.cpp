#include "stdafx.h"
#include "..\public\HellHound_State.h"
#include "HellHound.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Effect.h"
#include "Effect_Point_Instancing.h"
#include "Effect_PlayerDust.h"
CHellHound_State::CHellHound_State()
{
}

HRESULT CHellHound_State::Initialize(CHellHound * pHellHound)
{
	m_pHellHound = pHellHound;
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

HRESULT CHellHound_State::SetUp_State_Idle()
{
	if (nullptr == m_pHellHound->m_pStateCom)
		return E_FAIL;


	m_pHellHound->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CHellHound_State::Start_Idle)
		.Init_Tick(this, &CHellHound_State::Tick_Idle)
		.Init_End(this, &CHellHound_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK"), this, &CHellHound_State::isChase)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CHellHound_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CHellHound_State::isAttakedFlinch)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CHellHound_State::isDead)
		.Finish_Setting();
	return S_OK;
}

HRESULT CHellHound_State::SetUp_State_Walk()
{
	m_pHellHound->m_pStateCom->
		Add_State(TEXT("STATE::WALK"))
		.Init_Start(this, &CHellHound_State::Start_Walk)
		.Init_Tick(this, &CHellHound_State::Tick_Walk)
		.Init_End(this, &CHellHound_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CHellHound_State::isNone)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CHellHound_State::isAttackRange)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CHellHound_State::isAttakedFlinch)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CHellHound_State::isDead)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHellHound_State::SetUp_State_Attack()
{
	m_pHellHound->m_pStateCom->
		Add_State(TEXT("STATE::ATTACKA"))
		.Init_Start(this, &CHellHound_State::Start_Attack)
		.Init_Tick(this, &CHellHound_State::Tick_Attack)
		.Init_End(this, &CHellHound_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CHellHound_State::isFinish)
		.Init_Changer(TEXT("STATE::ATTACKB"), this, &CHellHound_State::isNextAttackB)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CHellHound_State::isAttakedFlinch)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CHellHound_State::isDead)
		
		.Add_State(TEXT("STATE::ATTACKB"))
		.Init_Start(this, &CHellHound_State::Start_AttackB)
		.Init_Tick(this, &CHellHound_State::Tick_AttackB)
		.Init_End(this, &CHellHound_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CHellHound_State::isFinish)
		.Init_Changer(TEXT("STATE::ATTACKA"), this, &CHellHound_State::isNextIdle)
		.Init_Changer(TEXT("STATE::FLINCH"), this, &CHellHound_State::isAttakedFlinch)
		.Init_Changer(TEXT("STATE::DEAD"), this, &CHellHound_State::isDead)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHellHound_State::SetUp_State_Flinch()
{
	m_pHellHound->m_pStateCom->
		Add_State(TEXT("STATE::FLINCH"))
		.Init_Start(this, &CHellHound_State::Start_Flinch)
		.Init_Tick(this, &CHellHound_State::Tick_Flinch)
		.Init_End(this, &CHellHound_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CHellHound_State::isAttackedFinish)
		//.Init_Changer(TEXT("STATE::DEAD"), this, &CHellHound_State::isDead)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHellHound_State::SetUp_State_Dead()
{
	m_pHellHound->m_pStateCom->
		Add_State(TEXT("STATE::DEAD"))
		.Init_Start(this, &CHellHound_State::Start_Dead)
		.Init_Tick(this, &CHellHound_State::Tick_Dead)
		.Init_End(this, &CHellHound_State::End_Common)
		.Finish_Setting();

	return S_OK;
}

void CHellHound_State::Start_Idle(_double TimeDelta)
{

	m_pHellHound->m_fHellHoundTime = 0.05;
	m_pHellHound->m_pModelCom->Set_AnimIndex(TideHunter_Idle);
}

void CHellHound_State::Start_Walk(_double TimeDelta)
{
	m_pHellHound->m_fHellHoundTime = 0.05;
	m_pHellHound->m_pModelCom->Set_AnimIndex(TideHunter_Run_F);
}

void CHellHound_State::Start_Attack(_double TimeDelta)
{
	m_pHellHound->m_fHellHoundTime = 0.07;
	m_pHellHound->m_pModelCom->Set_AnimIndex(TideHunter_Atk_Slash_01_R);
}

void CHellHound_State::Start_AttackB(_double TimeDelta)
{
	m_pHellHound->m_fHellHoundTime = 0.07;
	m_pHellHound->m_pModelCom->Set_AnimIndex(TideHunter_Atk_Slash_01_L);
}

void CHellHound_State::Start_Flinch(_double TimeDelta)
{
	m_pHellHound->m_fHellHoundTime = 0.08;
	m_pHellHound->m_pModelCom->Set_AnimIndex(TideHunter_Impact_F);
}

void CHellHound_State::Start_Dead(_double TimeDelta)
{
	m_pHellHound->m_fHellHoundTime = 0.03;
	m_pHellHound->m_pModelCom->Set_AnimIndex(TideHunter_Death);
	m_pHellHound->m_pTransformCom->Go_Backward(TimeDelta, m_pHellHound->m_pNavigationCom);
}

void CHellHound_State::Tick_Idle(_double TimeDelta)
{
	static_cast<CCollider*>(m_pHellHound->Get_LeftSwordCollider())->Set_CheckAble(false);
	static_cast<CCollider*>(m_pHellHound->Get_RightSwordCollider())->Set_CheckAble(false);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	m_pHellHound->m_pTransformCom->LookAt(vPlayerPos);

	RELEASE_INSTANCE(CGameInstance);

}

void CHellHound_State::Tick_Walk(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);


	m_pHellHound->m_pTransformCom->Chase(vPlayerPos, TimeDelta * 0.07f);
	m_pHellHound->m_pTransformCom->LookAt(vPlayerPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CHellHound_State::Tick_Attack(_double TimeDelta)
{

	AnimationIntervalCheck(TideHunter_Atk_Slash_01_R, 0.5, 0.8);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);



	m_pHellHound->m_pTransformCom->LookAt(vPlayerPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CHellHound_State::Tick_AttackB(_double TimeDelta)
{

	AnimationIntervalCheck(TideHunter_Atk_Slash_01_L, 0.5, 0.8);
}

void CHellHound_State::Tick_Flinch(_double TimeDelta)
{
	/*AnimFinishChecker(TideHunter_Impact_F, 0.99);*/
}

void CHellHound_State::Tick_Dead(_double TimeDelta)
{
	static_cast<CCollider*>(m_pHellHound->Get_LeftSwordCollider())->Set_CheckAble(false);
	static_cast<CCollider*>(m_pHellHound->Get_RightSwordCollider())->Set_CheckAble(false);


	m_pHellHound->m_pMonsterAbility->fCurrentHP = 0.f;

		if (m_pHellHound->m_pModelCom->Get_Animation(TideHunter_Death)->Get_PlayRate() >= 0.99)
	{
		m_pHellHound->m_pModelCom->Set_AnimIndex(TideHunter_DeadPose);

	}


}

void CHellHound_State::End_Common(_double TimeDelta)
{
}




_bool CHellHound_State::isNone(void)
{
	if (m_pHellHound->Collision_ChaseRange())
	{
		m_pHellHound->m_AnimFinish = false;
	}
	else
	{
		m_pHellHound->m_AnimFinish = true;
	}



	return m_pHellHound->m_AnimFinish;
}

_bool CHellHound_State::isChase(void)
{
	_bool bResult = m_pHellHound->Collision_ChaseRange();


	return bResult;
}

_bool CHellHound_State::isFinish(void)
{
	if (m_pHellHound->Collision_Player())
	{
		m_pHellHound->m_AnimFinish = false;
	}
	else
	{
		m_pHellHound->m_AnimFinish = true;

	}

	return m_pHellHound->m_AnimFinish;


}

_bool CHellHound_State::isAttackedFinish(void)
{
	if (m_pHellHound->Collision_AttackedRange())
	{
		m_pHellHound->m_AnimFinish = false;
	}
	else
	{
		m_pHellHound->m_AnimFinish = true;

	}

	return m_pHellHound->m_AnimFinish;
}

_bool CHellHound_State::isAttackRange(void)
{
	_bool bResult = m_pHellHound->Collision_Player();

	return bResult;
}

_bool CHellHound_State::isAttakedFlinch(void)
{
	_bool bResult = m_pHellHound->Collision_AttackedRange();

	return bResult;
}

_bool CHellHound_State::isNextAttackB(void)
{
	if (m_pHellHound->m_pModelCom->Get_Animation(TideHunter_Atk_Slash_01_R)->Get_PlayRate() >= 0.9)
	{
		if (m_pHellHound->Collision_Player())
		{
			m_pHellHound->m_pModelCom->Set_AnimIndex(TideHunter_Atk_Slash_01_L);

			return true;
		}
	}
	else
	{
	return false;
	}

	return m_pHellHound->Collision_Player();
}

_bool CHellHound_State::isNextIdle(void)
{
	if (m_pHellHound->m_pModelCom->Get_Animation(TideHunter_Atk_Slash_01_L)->Get_PlayRate() >= 0.9)
	{
		if (m_pHellHound->Collision_Player())
		{
			m_pHellHound->m_pModelCom->Set_AnimIndex(TideHunter_Idle);

			return true;
		}
	}
	else
	{
		return false;
	}

	return m_pHellHound->Collision_Player();
}

_bool CHellHound_State::isDead(void)
{
	_bool bResult = m_pHellHound->isDead();

	return bResult;
}

_bool CHellHound_State::AnimationIntervalCheck(ANIMATION eAnim, _double StartRate, _double FinishRate)
{

	ENGINE_OPEN;

	if (m_pHellHound->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() >= StartRate &&
		m_pHellHound->m_pModelCom->Get_Animation(eAnim)->Get_PlayRate() <= FinishRate)
	{
		static_cast<CCollider*>(m_pHellHound->Get_RightSwordCollider())->Set_CheckAble(true);
		static_cast<CCollider*>(m_pHellHound->Get_LeftSwordCollider())->Set_CheckAble(true);	
	}
	else
	{
		static_cast<CCollider*>(m_pHellHound->Get_RightSwordCollider())->Set_CheckAble(false);
		static_cast<CCollider*>(m_pHellHound->Get_LeftSwordCollider())->Set_CheckAble(false);

	}


	ENGINE_CLOSE;
	return true;
}

CHellHound_State * CHellHound_State::Create(CHellHound * pHellHound)
{
	CHellHound_State* pInstnace = new CHellHound_State();
	if (FAILED(pInstnace->Initialize(pHellHound)))
	{
		MSG_BOX("BOOM");
		Safe_Release(pInstnace);
	}
	return pInstnace;
}

void CHellHound_State::Free()
{
	Safe_Release(m_pHellHound);
}
