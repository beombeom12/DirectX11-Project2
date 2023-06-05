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
class CGoblin	final : public CGameObject
{

public:
	enum GOBLINCOLLIER { GOBLIN_HIT, GOBLIN_SWORD, GOBLIN_END };

public:
	CGoblin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGoblin(const CGoblin& rhs);
	virtual ~CGoblin() = default;
	friend class CGoblin_State;


public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();


private:
	CShader*						m_pShaderCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CModel*							m_pModelCom = nullptr;
	CCollider*						m_pColliderCom[COLLTYPE_END] = { nullptr };
	CCollider*						m_pHit[GOBLIN_END] = { nullptr };
	CNavigation*					m_pNavigationCom = nullptr;
	CState*							m_pStateCom = nullptr;
	class CPlayer*					m_pPlayer = nullptr;
	class CGoblin_State*			m_pGoblinState = nullptr;
	MONSTERABILITY*					m_pMonsterAbility = nullptr;

	_bool	m_TakeDamage = false;

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

	CCollider*			Get_SwordCollider();

	void				Player_ToDamaged(_float fDamage);


private:
	_bool			m_AnimFinish = false;
	_bool			m_TestFinish = true;
	_float			m_fAnimationTime = 0.f;
	vector<CGameObject*>	m_pMonsterParts;

	_float			m_fGoblinTime = 0.3f;
	_float			m_fLerpAnimationTime = 0.2f;
public:
	static CGoblin* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END