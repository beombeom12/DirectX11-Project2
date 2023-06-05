#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CNavigation;
class CCollider;
class CState;
END



BEGIN(Client)
class CHellHound	final : public CGameObject
{
public:
	CHellHound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHellHound(const CHellHound& rhs);
	virtual ~CHellHound() = default;
	friend class CHellHound_State;
public:
	enum HELLHOUNDCOLLIDER { HELLHOUND_HIT, HELLHOUND_SWORD, HELLHOUND_END };
	enum HELLHOUNDATTACK {HELLHOUND_ATTACK_A, HELLHOUND_ATTACK_B, HELLHOUND_ATTACK_END};

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();


private:
	CState*							m_pStateCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CModel*							m_pModelCom = nullptr;
	CCollider*						m_pColliderCom[COLLTYPE_END] = { nullptr };
	CCollider*						m_pHit[HELLHOUND_END] = { nullptr };
	CCollider*						m_pAttack[HELLHOUND_ATTACK_END] = { nullptr };
	CNavigation*					m_pNavigationCom = nullptr;
	class CPlayer*					m_pPlayer = nullptr;
	class CHellHound_State*			m_pHellHoundState = nullptr;
	MONSTERABILITY*					m_pMonsterAbility = nullptr;
	class CEffect*					m_pEffect = nullptr;
	class CEffect_Point_Instancing* m_pPointeffect = nullptr;
	class CEffect_PlayerDust*		m_pDustEffect = nullptr;



	_bool							m_TakeDamage = false;
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
	_bool				isDead();



	void				Player_ToDamaged(_float fDamage);
	void				Set_Collider(void);

	CCollider*			Get_RightSwordCollider() { return m_pAttack[HELLHOUND_ATTACK_A]; }
	CCollider*			Get_LeftSwordCollider() { return m_pAttack[HELLHOUND_ATTACK_B]; }

private:
	_bool			m_AnimFinish = false;
	_bool			m_TestFinish = true;
	_float			m_fAnimationTime = 0.f;
	vector<CGameObject*>	m_pMonsterParts;




	_float			m_fHellHoundTime = 0.3f;
	_float			m_fLerpTime = 0.2f;


public:
	static CHellHound* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END