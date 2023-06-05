#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Player.h"

BEGIN(Engine)
class CNavigation;

END
BEGIN(Client)
class CGrinner_State : public CBase
{

	enum ANIMATION {
		Grinner_Atk_BarfinRainbows,
		Grinner_Atk_Flip,
		Grinner_Atk_Lunge,
		Grinner_Atk_Swipe_Combo,
		Grinner_DeadPos,
		Grinner_Death,
		Grinner_IA_Death_War,
		Grinner_IA_DeathPose_War,
		Grinner_Idle,
		Grinner_Impact_F,
		Grinner_Impact_Flinch_F,
		Grinner_Impact_L,
		Grinner_Impact_R,
		Grinner_Run_F,
		Grinner_Walk_F,
		Grinner_END
	};

private:
	CGrinner_State();
	virtual	~CGrinner_State() = default;

public:
	HRESULT Initialize(class CGrinner* pGrinner);
	


private:
	HRESULT SetUp_State_Idle();
	HRESULT SetUp_State_Walk();
	HRESULT SetUp_State_Attack();
	HRESULT SetUp_State_Flinch();
	HRESULT SetUp_State_Dead();
	//Start
private:
	void Start_Idle(_double TimeDelta);
	
	void Start_Walk(_double TimeDelta);


	void Start_AttackA(_double TimeDelta);
	void Start_AttackB(_double TimeDelta);
	void Start_AttackC(_double TimeDelta);
	
	//Flinch
	void Start_FlinchA(_double TimeDelta);



	//Dead
	void Start_Dead(_double TimeDelta);

	//Tick
private:
	void Tick_Idle(_double TimeDelta);
	
	
	
	void Tick_Walk(_double TimeDelta);


	void Tick_AttackA(_double TimeDelta);
	void Tick_AttackB(_double TimeDelta);
	void Tick_AttackC(_double TimeDelta);
	//Flinch
	void Tick_FlinchA(_double TimeDelta);

	//Dead
	void Tick_Dead(_double TimeDelta);

	//End
private:
	void	End_Common(_double TimeDelta);



private:
	_bool	isNone(void);
	
	_bool	isFinish(void);

	_bool	isAttackedFinish(void);





	_bool	isWalk(void);
	
	_bool	isAttackRange(void);

	_bool	isAttackedRange(void);

	_bool	isNextAttackB(void);
	_bool	isNextAttackC(void);
	_bool	isNextIdle(void);


	_bool	isDead(void);


//test
	_bool	AnimIntervalChecker(ANIMATION eAnim, _double StartRate, _double FinishRate);
	_bool	AnimFinishChecker(ANIMATION eAnim, _double FinishRate);
private:
	class CGrinner*		m_pGrinner;
	class CPlayer*		m_pPlayer;

	class CNavigation*	m_pNavi;
public:
	_bool		m_bNext;

public:
	static CGrinner_State* Create(class CGrinner* pGrinner);
	virtual void Free();

};

END