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
class CSteamroller	final : public CGameObject
{
	enum COLLIDER { COLLIER_HIT, COLLIDER_SWORD, COLLIER_SKILL1, COLLIDER_END };

public:
	CSteamroller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSteamroller(const CSteamroller& rhs);
	virtual ~CSteamroller() = default;
	friend class CSteamroller_State;


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
	CCollider*						m_pHit[COLLIDER_END] = { nullptr };
	CNavigation*					m_pNavigationCom = nullptr;

	CState*							m_pStateCom = nullptr;
	class CPlayer*					m_pPlayer = nullptr;
	class CSteamroller_State*		m_pSteamState = nullptr;


	_bool							m_TakeDamage = false;


private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_ShaderResources();
	HRESULT				SetUp_State();
	HRESULT				Set_OnCell();


public:
	_bool				Collision_Player();
	_bool				Collision_ChaseRange();
	_bool				Collision_AttackedRange();
	_bool				Collision_SkillRanage();


	_bool				isDead();

	void			Player_To_Damaged(_float fDamage);


	void				Set_Collider();

	CCollider*			Get_WeaponCollier() { return m_pHit[COLLIDER_SWORD]; }
private:
	_bool			m_AnimFinish = false;
	_bool			m_TestFinish = true;
	_float			m_fAnimationTime = 0.f;

	vector<CGameObject*>	m_pMonsterParts;
	MONSTERABILITY*	m_pMonsterAbility;

	_double 			m_fSteamAnimationTime = 1.0;
	_double				m_fLerpTime = 0.2;



public:
	static CSteamroller* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END