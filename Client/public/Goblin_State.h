#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CGoblin_State : public CBase
{
	enum ANIMATION {
		Goblin_Attack_01,
		Goblin_Attack_02,
		Goblin_Attack_03,
		Goblin_DeadPose, 
		Goblin_Death_01,
		Goblin_Flinch_F,
		Goblin_Idle,
		Goblin_Impact_F,
		Goblin_Run_F,
		Goblin_Spawn_Lava,
		Goblin_Worship_Idle,
		Goblin_END
	};


public:
	CGoblin_State();
	virtual ~CGoblin_State() = default;


public:
	HRESULT			Initialize(class CGoblin* pGoblin);

private:
	HRESULT		    SetUp_State_Idle();
	HRESULT			SetUp_State_Walk();
	HRESULT			SetUp_State_Attack();
	HRESULT			SetUp_State_Flinch();
	HRESULT			SetUp_Satee_Dead();
	//Start
private:
	void			Start_Idle(_double TimeDelta);

	void			Start_Walk(_double TimeDelta);
	
	void			Start_Attack(_double TimeDelta);
	
	void			Start_Flinch(_double TimeDelta);

	void			Start_Dead(_double TimeDelta);
	//Tick
private:
	void			Tick_Idle(_double TimeDelta);

	void			Tick_Walk(_double TimeDelta);

	void			Tick_Attack(_double TimeDelta);

	void			Tick_Flinch(_double TimeDelta);

	void			Tick_Dead(_double Timedelta);
	//End
private:
	void			End_Common(_double TimeDelta);



private:
	_bool			isNone(void);

	_bool			isFinish(void);
	
	_bool			isAttackedFinish(void);


	//
	_bool			isChase(void);

	_bool			isAttackRange(void);

	_bool			isAttackedRange(void);

	_bool			isDead(void);

	_bool			AnimationChecker(ANIMATION eAnim, _double StartRate, _double FinishRate);
private:
	class CGoblin*		m_pGoblin;


public:
	static CGoblin_State* Create(class CGoblin* pGoblin);
	virtual void Free();
};

END