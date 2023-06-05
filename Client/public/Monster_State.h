#pragma once

#include "Client_Defines.h"
#include "Base.h"




BEGIN(Client)

class CMonster_State : public CBase
{
public:
	enum ANIMATION {
		Scarab_Hulk_GroundSpawn,
		ScarabHulk_Attack_L,
		ScarabHulk_Attack_R,
		ScarabHulk_Idle,
		ScarabHulk_Walk_B,
		ScarabHulk_Walk_F,
		ScarabHulk_END
	};

private:
	CMonster_State();
	virtual ~CMonster_State() = default;

public:
	HRESULT Initialize(class CMonster* Monster);

private:
	HRESULT SetUp_State_Idle();
	HRESULT SetUp_State_Walk();

	HRESULT SetUp_State_Attack();
	HRESULT SetUp_State_Damaged();
	HRESULT SetUp_State_Deth();

private: // Start //
	void Start_Idle(_double TimeDelta);

	void Start_Walk(_double TimeDelta);

	void Start_Attack(_double TimeDelta);

	void Start_Damaged(_double TimeDelta);





private: // Tick //
	void Tick_Idle(_double TimeDelta);

	void Tick_Walk(_double TimeDelta);

	void Tick_Attack(_double TimeDelta);

	void Tick_Damaged(_double TimeDelta);






private: // END //
	void End_Common(_double TimeDleta);


private:
	_bool	isFinish(void);
	_bool	isWalking(void);
	_bool	isAttacking(void);

private:
	class CMonster*		m_pMonster;	
	class CTransform*	m_pPlayerTransformCom;
	class CPlayer*		m_pPlayer;
public:
	static CMonster_State* Create(class CMonster* Monster);
	virtual void Free();
};

END