#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CMoloch_State : public CBase
{

	enum ANIMATION
	{
		Moloch_Atk_Dash_Hold,
		Moloch_Atk_Dash_Start,
		Moloch_Atk_Dash_Strike,
		Moloch_Atk_Full_Dash,
		Moloch_Atk_Full_Dash_Start,
		Moloch_Atk_Full_Dash_Strike,
		Moloch_Atk_Full_Dash_Strike_02,
		Moloch_Atk_Full_Geyser,
		Moloch_Atk_Full_Geyser_02,
		Moloch_Atk_Full_Geyser_Start,
		Moloch_Atk_Full_Swing_01,
		Moloch_Atk_Full_Swing_02,
		Moloch_Atk_Full_Swing_03,
		Moloch_Atk_Full_TremorPulse,
		Moloch_Atk_Swipe_01,
		Moloch_Atk_Swipe_02,
		Moloch_Atk_Swipe_03,
		Moloch_Full_Idle,
		Moloch_Full_Impact_F,
		Moloch_Full_Impact_Stun,
		Moloch_Full_Impact_Stun_Idle,
		Moloch_Full_Run_F,
		Moloch_Idle,
		Moloch_Intro,
		Moloch_Run_F,
		Moloch_Walk_F,
		Moloch_END
	};

public:
	CMoloch_State();
	virtual ~CMoloch_State() = default;
public:
	HRESULT		Initialize(class CMoloch* pMoloch);
private:
	HRESULT		    SetUp_State_Idle();

	HRESULT			SetUp_State_Chase();

	HRESULT			SetUp_State_Attack();


	HRESULT			SetUp_State_Flinch();

	HRESULT			SetUp_State_Dead();



	//Skill1

	HRESULT			SetUp_State_Skill();

	HRESULT			SetUp_State_Skill2();

private:
	void			Start_Idle(_double TimeDelta);

	void			Start_Chase(_double TimeDelta);

	void			Start_Flinch(_double TimeDelta);

	void			Start_Attack(_double TimeDelta);
	void			Start_AttackB(_double TimeDelta);
	void			Start_AttackC(_double TimeDelta);


	//SKill1
	void			Start_Skill1(_double TimeDelta);
	void			Start_Skill2(_double TimeDelta);
	void			Start_Skill3(_double TimeDelta);
	void			Start_Skill4(_double TimeDelta);


	//Skill2		Start
	void			Start_SkillTwo1(_double TimeDelta);
	void			Start_SkillTwo2(_double TimeDelta);
	void			Start_SkillTwo3(_double TimeDelta);


	//Dead
	void			Start_Dead(_double TimeDelta);

private:
	void			Tick_Idle(_double TimeDelta);
	
	void			Tick_Chase(_double TimeDelta);

	void			Tick_Flinch(_double TimeDelta);
	//for Boss Base Attack
	void			Tick_Attack(_double TimeDelta);
	void			Tick_AttackB(_double TimeDelta);
	void			Tick_AttackC(_double TimeDelta);

	//for Boss Skill1
	void			Tick_Skill1(_double TimeDelta);
	void			Tick_Skill2(_double TimeDelta);
	void			Tick_Skill3(_double TImeDelta);
	void			Tick_Skill4(_double TimeDelta);

	//for Boss Skill2

	void			Tick_TwoSkill1(_double TimeDelta);
	void			Tick_TwoSkill2(_double TimeDelta);
	void			Tick_TwoSkill3(_double TimeDelta);



	void			Tick_Dead(_double TimeDelta);
private:
	void			End_Common(_double TimeDelta);


private:
	_bool			isNone(void);

	_bool			isFinish(void);
	//for Skill1
	_bool			isSkillFinish(void);
	//for Skill2
	_bool			isSkill2Finish(void);

	_bool			isChase(void);




	_bool			isAttackRange(void);

	_bool			isAttackedFinish(void);
	
	_bool			isAttackedRange(void);


	//for Boss Skill
	_bool			isSkillRange(void);

	_bool			isSkill2Range(void);
	//for Boss Base Attack
	_bool			isNextAttackB(void);

	_bool			isNextAttackC(void);


	_bool			isNextIdle(void);


	//Skill
	_bool			isSkillNextB(void);

	_bool			isSkillNextC(void);

	_bool			isSkillNextD(void);

	_bool			isSkillNextIdle(void);


	//Skill2 

	_bool			isTwoSkillNextB(void);

	_bool			isTwoSkillNextC(void);

	_bool			isTwoSkillIDLE(void);


	_bool			isDead(void);

	//State Á¦¾î
	_bool			AnimationIntervalChecker(ANIMATION eAnim, _double StartRate, _double FinishRate);
	
private:
	class CMoloch*		m_pMoloch;

public:
	static CMoloch_State*	Create(class CMoloch* pMoloch);
	virtual void Free();

};

END