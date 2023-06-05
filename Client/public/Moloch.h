#pragma once


#include "Client_Defines.h"
#include "GameObject.h"
#include "Mo_Stone.h"
#include "UI_MonsterHP.h"


BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CNavigation;
class CState;
END

BEGIN(Client)
class CMoloch	final : public CGameObject
{

public:
	enum MOLOCHCOLLIDER { MOLOCH_HIT, MOLOCH_SWORD, MOLOCH_END };
	enum MOLOCHATTACK {MOLOCH_SKILL1, MOLOCH_SKILL2,MOLOCH_SKILLEND};
public:
	CMoloch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMoloch(const CMoloch& rhs);
	virtual ~CMoloch() = default;



public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();
	friend class CMoloch_State;

private:
	CShader*						m_pShaderCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CModel*							m_pModelCom = nullptr;
	CCollider*						m_pColliderCom[COLLTYPE_END] = { nullptr };
	CCollider*						m_pHit[MOLOCH_END] = { nullptr };

	CCollider*						m_pAttack[MOLOCH_SKILLEND] = { nullptr };
	CNavigation*					m_pNavigationCom = nullptr;
	CState*							m_pStateCom = nullptr;
	class CPlayer*					m_pPlayer = nullptr;
	class CMoloch_State*			m_pMolochState = nullptr;

	class CMo_Stone*				m_pMoStone = nullptr;
	class CUI_MonsterHP*			m_pMonsterHPUI = nullptr;

	_bool							m_bTakeDamage = false;
private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_ShaderResources();
	HRESULT				SetUp_State();
	HRESULT				Ready_Parts();
	HRESULT				SetOn_Cell();


public:
	_bool				Collision_Player();
	
	_bool				Collision_ChaseRange();
	
	_bool				Collision_AttackedRange();
	//For Boss Skill
	_bool				Collision_Skill1Range();

	_bool				Collision_Skill2Range();

	
	_bool				isDead();

	CCollider*			Get_SwordCollider();
	CCollider*			Get_StoneCollider();


	void				Player_ToDamaged(_float fDamage);



	MONSTERABILITY*					m_pMonsterAbility = nullptr;
	_float		Get_HPAbility() { return m_pMonsterAbility->fCurrentHP; }
	_float		Get_MaxAbility() { return m_pMonsterAbility->fMaxHP; }
private:
	_bool			m_AnimFinish = false;
	_bool			m_TestFinish = true;
	_float			m_fAnimationTime = 0.f;
	vector<CGameObject*>	m_pMonsterParts;

	_float			m_fMolochPlayTime = 0.5f;
	_float			m_fLerpTime = 0.2f;


public:
	static CMoloch* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END