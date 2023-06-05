#pragma once

#include "Client_Defines.h"
#include "Base.h"
BEGIN(Client)
class CHellHound_State : public CBase
{

	enum ANIMATION {
		TideHunter_Atk_Slash_01_L,
		TideHunter_Atk_Slash_01_R,
		TideHunter_Atk_Slash_02_L,
		TideHunter_Atk_Slash_02_R,
		TideHunter_Atk_Spit,
		TideHunter_Tail_L,
		TideHunter_Tail_R,
		TideHunter_DeadPose,
		TideHunter_Death,
		TideHunter_Idle, 
		TideHunter_Impact_F,
		TideHunter_Impact_Flinch_F,
		TideHunter_Run_F,
		TideHunter_Walk_F,
		TideHunter_END
	};

public:
	CHellHound_State();
	virtual ~CHellHound_State() = default;

public:
	HRESULT			Initialize(class CHellHound* pHellHound);
private:
	HRESULT		    SetUp_State_Idle();
	HRESULT			SetUp_State_Walk();
	HRESULT			SetUp_State_Attack();
	HRESULT			SetUp_State_Flinch();
	HRESULT			SetUp_State_Dead();

private:
	void			Start_Idle(_double TimeDelta);

	void			Start_Walk(_double TimeDelta);

	void			Start_Attack(_double TimeDelta);
	void			Start_AttackB(_double TimeDelta);


	void			Start_Flinch(_double TimeDelta);

	void			Start_Dead(_double TimeDelta);
private:
	void			Tick_Idle(_double TimeDelta);

	void			Tick_Walk(_double TimeDelta);

	void			Tick_Attack(_double TimeDelta);
	void			Tick_AttackB(_double TimeDelta);

	void			Tick_Flinch(_double TimeDelta);

	void			Tick_Dead(_double TimeDelta);
private:
	void			End_Common(_double TimeDelta);



private:
	class CHellHound*		m_pHellHound;

	_bool			isNone(void);
	_bool			isChase(void);
	_bool			isFinish(void);
	_bool			isAttackedFinish(void);
	_bool			isAttackRange(void);
	_bool			isAttakedFlinch(void);
	_bool			isNextAttackB(void);
	_bool			isNextIdle(void);
	_bool			isDead(void);



	//
	_bool			AnimationIntervalCheck(ANIMATION eAnim, _double StartRate, _double FinishRate);
public:
	static CHellHound_State* Create(class CHellHound* pHellHound);
	virtual void Free();
};

END