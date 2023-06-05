#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CNavigation;

END


BEGIN(Client)

class CPlayer_State final : public CBase
{
public:
	CPlayer_State();
	virtual~CPlayer_State() = default;


	enum ANIMATION {
		War_Atk_Dash,
		War_Atk_Flamebrand,
		War_Atk_Flamebrand_End,
		War_Atk_Flamebrand_Start,
		War_Atk_Heavy_01,
		War_Atk_Heavy_01b,
		War_Atk_Heavy_02,
		War_Atk_Heavy_03,
		War_Atk_Launch,
		War_Atk_Launch_B,
		War_Atk_Light_01,
		War_Atk_Light_02,
		War_Atk_Light_02_b,
		War_Atk_Light_03,
		War_Atk_Light_03_Single,
		War_Atk_Light_04,
		War_Atk_Light_04_Single,
		War_Dash_F,
		War_DashTo_Back,
		War_DashTo_F,
		War_Death,
		War_Idle,
		War_Idle_Combat,
		War_Idle_Combat_to_Idle,
		War_Idle_to_Idle_Combat,
		War_Impact_From_Back_01,
		War_Impact_From_Front_01,
		War_Impact_From_Left_01,
		War_Impact_From_Right_01,
		War_Jump,
		War_Jump_Combat,
		War_Run_F,
		War_Run_F_Combat,
		War_vorpal_Walk_B,
		War_vorpal_Walk_BL,
		War_vorpal_Walk_BR,
		War_vorpal_Walk_F,
		War_vorpal_Walk_FL,
		War_vorpal_Walk_FR,
		War_Walk_F_Combat,
		WAR_END
	};


public:
	HRESULT Initialze(class CPlayer* Player);

private:
	HRESULT	Setup_State_Idle(void);
	HRESULT Setup_State_Idle_Combat(void);
	HRESULT	Setup_State_Walk(void);
	HRESULT Setup_State_Attack(void);
	HRESULT Setup_State_HeavyAttack(void);
	HRESULT Setup_State_Dash(void);
	HRESULT Setup_State_Skill1(void);

	HRESULT Setup_State_Flinch(void);
	/////////START//////////////////////////
	void	Start_Idle(_double TimeDelta);


	//Idle to Combat
	void	Start_Idle_To_Combat(_double TimeDelta);
	void	Start_Combat_To_Idle(_double TimeDelta);
	//Walk
	void	Start_Walk_F(_double TimeDelta);
	void	Start_Walk_B(_double TimeDelta);
	void	Start_Walk_FL(_double TimeDelta);
	void	Start_Walk_FR(_double TimeDelta);

	void	Start_Walk_BL(_double TimeDelta);
	void	Start_Walk_BR(_double TimeDelta);

	//Run
	void	Start_Run_CombatF(_double TimeDelta);




	//WA WD
	void	Start_Walk_FLeft(_double TimeDelta);
	void	Start_Walk_FRight(_double TimeDelta);

	//Attack
	void	Start_Attack_A(_double TimeDelta);
	void	Start_Attack_B(_double TimeDelta);
	void	Start_Attack_C(_double TimeDelta);
	void	Start_Attack_C_A(_double TimeDelta);

	void	Start_Attack_D(_double TimeDelta);

	//HeavyAttack
	void	Start_HeavyAttack_A(_double TimeDelta);
	void	Start_HeavyAttack_B(_double TimeDelta);

	void	Start_HeavyAttack_C(_double TimeDelta);

	//Skill1
	void	Start_Skill1_A(_double TimeDelta);
	void	Start_Skill2_A(_double TimeDelta);
	//Dash
	void	Start_Dash_F(_double TimeDelta);
	void	Start_Dash_B(_double TimeDelta);


	//Flinch
	void	Start_Flinch_F(_double TimeDelta);


	////////TICK/////////////////////////////
	void	Tick_Idle(_double TimeDelta);
	void	Tick_Idle_To_Combat(_double TimeDelta);
	void	Tick_Combat_To_Idle(_double TimeDelta);


	void	Tick_Walk_F(_double TimeDelta);
	void	Tick_Walk_B(_double TimeDelta);
	void	Tick_Walk_FL(_double TimeDelta);
	void	Tick_Walk_FR(_double TimeDelta);


	void	Tick_Walk_FLeft(_double TimeDelta);
	void	Tick_Walk_FRight(_double TimeDelta);

	void	Tick_Walk_BLeft(_double TimeDelta);
	void	Tick_Walk_BRight(_double TimeDelta);


	//Run

	void	Tick_Run_CombatF(_double TimeDelta);
	//Attack
	void	Tick_Attack_A(_double TimeDelta);
	void	Tick_Attack_B(_double TimeDelta);
	void	Tick_Attack_C(_double TimeDelta);
	void	Tick_Attack_C_A(_double TimeDelta);
	void	Tick_Attack_D(_double TimeDelta);
	//HeavyAttack
	void	Tick_HeavyAttack_A(_double TimeDelta);
	void	Tick_HeavyAttack_B(_double TimeDelta);
	void	Tick_HeavyAttack_BC(_double TimeDelta);
	void	Tick_HeavyAttack_C(_double TimeDelta);

	//Skill
	void	Tick_Skill1_A(_double TimeDelta);
	void	Tick_Skill2_A(_double TimeDelta);
	void	Tick_Skill3_A(_double TimeDelta);


	//Dash
	void	Tick_Dash_F(_double TimeDelta);
	void	Tick_Dash_B(_double TimeDelta);

	//Flinch

	void	Tick_Flinch_A(_double TimeDelta);

	void	End_Common(_double TimeDelta);




	////////KeySTATE///////////
	_bool	KeyInput_None(void);
	_bool	KeyInput_W(void);
	_bool	KeyInput_A(void);
	_bool	KeyInput_S(void);
	_bool	KeyInput_D(void);
	_bool	KeyInput_WA(void);
	_bool	KeyInput_WD(void);
	_bool	KeyInput_WS(void);
	_bool	KeyInput_AS(void);
	_bool	KeyInput_DS(void);
	
	//IDle_to Combat
	_bool	KeyInput_H(void);
	//Attack
	_bool	KeyInput_K(void);
	//HeavyAttack
	_bool	KeyInput_J(void);

	


	//Skill
	_bool	KeyInput_L(void);
	_bool	KeyInput_LS(void);
	//Run
	_bool	KeyInput_R(void);
	_bool	KeyInput_Shift(void);
	
	_bool	KeyInput_Space(void);
	_bool	KeyInput_ShiftW(void);
	_bool	KeyInput_ShiftA(void);
	_bool	KeyInput_ShiftD(void);

	_bool	isFinish(void);


	_bool	isFlinch(void);

	_bool isAttackedRange(void);

private:
	_bool	AnimIntervalChecker(ANIMATION eAnim, _double StartRate, _double FinishRate);
	_bool	AnimFinishChecker(ANIMATION eAnim, _double FinishRate);

private:
	class CPlayer*		m_pPlayer;
	class CNavigation*		m_pNavi;
	_bool					isHeavyAttack = false;


	_bool					isSound = false;
public:
	static CPlayer_State* Create(class CPlayer* Player);
	virtual void Free(void);
};

END