#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CNavigation;
class CState;
END

BEGIN(Client)

class CSubMo final : public CGameObject
{
public:
	enum COLLIDER { COLLIDER_HIT, COLLIDER_SWORD, COLLIDER_END };
	enum ATTACKCOLLIDER {ATTACK_A, ATTACK_END};

private:
	CSubMo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSubMo(const CSubMo& rhs);
	virtual ~CSubMo() = default;
	friend class CSubeMo_State;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pModelCom = nullptr;
	CCollider*					m_pColliderCom[COLLTYPE_END] = { nullptr };
	CCollider*					m_pHit[COLLIDER_END] = { nullptr };
	CCollider*					m_pAttack[ATTACK_END] = { nullptr };
	CNavigation*				m_pNavigationCom = nullptr;
	CState*						m_pStateCom = nullptr;
	class CPlayer*				m_pPlayer = nullptr;
	class CSubeMo_State*		m_pSubMoState = nullptr;

	_bool						m_TakeDamge = false;
private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_ShaderResources();
	HRESULT				SetUp_State();
	HRESULT				Set_OnCell();

public:
	_bool				Collision_Player();
	_bool				Collision_ChaseRange();
	_bool				Collision_AttackedRange();
	_bool				isDead();


	void				Player_ToDamaged(_float fDamage);
	void				Set_Collider(void);

	MONSTERABILITY*				m_pMonsterAbility = nullptr;


	CCollider*			Get_AttackCollier() { return m_pAttack[ATTACK_A]; }
private:
	_bool			m_AnimFinish = false;
	_bool			m_TestFinish = true;
	_float			m_fAnimTimDelta = 0.f;

	vector<CGameObject*>	m_pMonsterParts;

	_float			m_fAdjustTime = 0.3f;
	_float			m_fLerfTime = 0.2f;


public:
	static CSubMo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END