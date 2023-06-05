#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Player.h"
BEGIN(Client)
class CSubeMo_State : public CBase
{

	enum ANIMATION
	{
		Grub_Atk_Bite,
		Grub_Atk_BlowUp,
		Grub_Atk_Burrow,
		Grub_Dead_Pose,
		Grub_Death,
		Grub_Idle,
		Grub_Impact_F,
		Grub_Impact_Flinch_F,
		Grub_Run_F,
		Grub_Walk_F,
		Grub_END
	};


public:
	CSubeMo_State();
	virtual ~CSubeMo_State() = default;


public:
	HRESULT Initialize(class CSubMo* pSubMo);


private:
	HRESULT SetUp_State_Idle();
	HRESULT SetUp_State_Walk();
	HRESULT SetUp_State_Attack();
	HRESULT SetUp_State_Flinch();
	HRESULT SetUp_State_Dead();
private:
	void Start_Idle(_double TimeDelta);

	void Start_Walk(_double TimeDelta);


	void Start_AttackA(_double TimeDelta);

	void Start_Flinch(_double TimeDelta);

	void Start_Dead(_double TimeDelta);
private:
	void Tick_Idle(_double TimeDelta);



	void Tick_Walk(_double TimeDelta);

	void Tick_AttackA(_double TimeDelta);

	void Tick_Flinch(_double TimeDelta);

	void Tick_Dead(_double TimeDelta);
private:
	void	End_Common(_double TimeDelta);

private:
	_bool	isNone(void);

	_bool	isWalk(void);

	_bool	isFinish(void);

	_bool	isAttackRange(void);

	_bool	isAttackedRange(void);


	_bool	isAttackedFinish(void);

	_bool	isDead(void);


	_bool	AnimIntervalChecker(ANIMATION eAnim, _double StartRate, _double FinishRate);
private:
	class CSubMo*		m_pSubMo;
	class CPlayer*		m_pPlayer;

public:
	static CSubeMo_State* Create(class CSubMo* pSubMo);
	virtual void Free();
};

END