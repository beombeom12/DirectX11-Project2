#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CSteamroller_State : public CBase
{
	enum ANIMATION {

		Steamroller_Attack_Slam,
		Steamroller_Attack_Swing_L,
		Steamroller_Attack_Swing_R,
		Steamroller_Deadpose,
		Steamroller_Death,
		Steamroller_Idle,
		Steamroller_Impact_F,
		SteamRoller_Roll_Start,
		SteamRoller_Roll_Start_Fast,
		SteamRoller_Roll_Stop,
		SteamRoller_Run_F,
		Steamroller_Taunt_01,
		Steamroller_END,
	};


public:
	CSteamroller_State();
	virtual ~CSteamroller_State() = default;


public:
	HRESULT			Initialize(class CSteamroller* pSteamroller);

private:
	HRESULT		    SetUp_State_Idle();
	HRESULT			SetUp_State_Walk();

	HRESULT			SetUp_State_Atttack();

	//Skill1 
	HRESULT			SetUp_State_Skill();

	HRESULT			SetUp_State_Flinch();


	HRESULT			SetUp_Staate_Dead();

	//Start
private:
	void			Start_Idle(_double TimeDelta);

	void			Start_Walk(_double TimeDelta);


	void			Start_Attack(_double TimeDelta);
	void			Start_AttackB(_double TimeDelta);
	void			Start_AttackC(_double TimeDelta);





	void			Start_SkillAttack(_double TimeDelta);
	void			Start_SkillAttackB(_double TimeDelta);
	void			Start_SkillAttackC(_double TimeDelta);

	void			Start_Flinch(_double TimeDelta);

	void			Start_Dead(_double TimeDelta);
	//Tick
private:
	void			Tick_Idle(_double TimeDelta);

	void			Tick_Walk(_double TimeDelta);

	void			Tick_Chase(_double TimeDelta);

	void			Tick_Attack(_double TimeDelta);
	void			Tick_AttackB(_double TimeDelta);
	void			Tick_AttackC(_double TimeDelta);

	void			Tick_SkillAttackA(_double TimeDelta);
	void			Tick_SkillAttackB(_double TimeDelta);
	void			Tick_SkillAttackC(_double TimeDelta);


	void			Tick_Flinch(_double TimeDelta);

	void			Tick_Dead(_double TimeDelta);
	//End
private:
	void			End_Common(_double TimeDelta);



private:
	_bool			isNone(void);

	_bool			isFinish(void);

	_bool			isSkillFinish(void);

	_bool			isAttackedFinish(void);

	//
	_bool			isChase(void);

	_bool			isAttackRange(void);
	
	_bool			isAttackedRange(void);


	_bool			isSkillRange(void);

	_bool			isFlinch(void);

	_bool			isNextAttackB(void);

	_bool			isNextAttackC(void);

	_bool			isNextSkillB(void);

	_bool			isNextSkillC(void);



	_bool			isDead(void);


	_bool			AnimationChecker(ANIMATION eAnim, _double StartRate, _double FinalRate);

private:
	class CSteamroller*		m_pSteamroller;


public:
	static CSteamroller_State* Create(class CSteamroller* pSteamroller);
	virtual void Free();
};

END