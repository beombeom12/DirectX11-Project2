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

class CGrinner final : public CGameObject
{
public:
	enum COLLIDER { COLLIDER_HIT, COLLIDER_SWORD, COLLIDER_END };
	enum ATTACKCOLLIDER {ATTACKCOLLIDER_A, ATTACKCOLLIDER_B, ATTACKCOLLIER_END};

private:
	CGrinner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGrinner(const CGrinner& rhs);
	virtual ~CGrinner() = default;
	friend class CGrinner_State;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	 CModel*					m_pModelCom = nullptr;
	CCollider*					m_pColliderCom[COLLTYPE_END] = { nullptr };
	CCollider*					m_pHit[COLLIDER_END] = { nullptr };
	CCollider*					m_pAttack[ATTACKCOLLIER_END] = { nullptr };
	CNavigation*				m_pNavigationCom = nullptr;
	CState*						m_pStateCom = nullptr;
	class CPlayer*				m_pPlayer = nullptr;
	class CGrinner_State*		m_GrinnerState = nullptr;


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

	CCollider*			Get_RightSwordCollider() { return m_pAttack[ATTACKCOLLIDER_A]; }
	CCollider*			Get_LeftSwordCollider()	 {  return m_pAttack[ATTACKCOLLIDER_B]; }

	MONSTERABILITY*				m_pMonsterAbility = nullptr;


	_bool						m_TakeDamage = false;
private:
	_bool			m_AnimFinish = false;
	_bool			m_TestFinish = true;
	_float			m_fAnimTimDelta = 0.f;

	vector<CGameObject*>	m_pMonsterParts;


	_float			m_fGrinnerAnim = 0.3f;
	_float			m_fLerpAnimationTime = 0.2f;

public:
	static CGrinner* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END