#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CBone;
class CNavigation;
class CState;
class CCollider;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public: 
	enum PLAYERCOLLIDER {PCOLLIDER_HIT, PCOLLIDER_SWORD, PCOLLIDER_SWORD2, PCOLLIDER_END };

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
	friend class CPlayer_State;
public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;



private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pColliderCom[COLLTYPE_END] = { nullptr };
	CCollider*				m_pHit[PCOLLIDER_END] = { nullptr };
	CBone*					m_pBone = nullptr;
	CNavigation*			m_pNavigation = nullptr;
	CState*					m_pState = nullptr;
	PLAYERABILITY*			m_pAbility = nullptr;

	class CEffect_Point_PlayerDust*	m_pPointPlayerDust = nullptr;
	class CEffect_PlayerDust*		m_pEffect_PlayerDust = nullptr;
	class CEffect_Point_PlayerWindow* m_pEffect_Window = nullptr;


	class CWarSkill*				m_pWarSkill = nullptr;


private:
	vector<CGameObject*>	m_pPlayerParts;
	_uint					m_iAnimIndex = 0;


	_bool					m_bTakeDamage = false;
	_bool					m_bGrinnerTakeDamage = false;
	_bool					m_bSubMoTakeDamage = false;
	_bool					m_bMolochTakeDamage = false;
	_bool					m_bMolochStoneTakeDamage = false;
	_bool					m_bSteamTakeDamage = false;
	_bool					m_bHellHoundTakeDamage = false;
public:
	HRESULT	Ready_Parts();
	HRESULT	Ready_WarSkill();
	HRESULT Ready_BoneChase();
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();
	HRESULT	SetUp_State();
	HRESULT Set_OnCell();


	//FSM
	class CWeapon*				m_pWeapon = nullptr;
	class CCollider*			m_pCollier = nullptr;
	class CPlayer_State*		m_pPlayer_StateCom = nullptr;
	_float		Get_HPAbility() { return m_pAbility->fHP; }
	_float		Get_MaxAbility() { return m_pAbility->fMaxHP; }
	void		DisOverlap(CCollider * TargetOverlapCollider);
public:
	CCollider*	Get_WeaponCollier();
	_bool		Collision_ToMonster();
	_bool		Collision_AttackedRange();

	void		Set_Damaged(_float fDamage)
	{
		m_pAbility->fHP = fDamage;
		if (m_pAbility->fHP <= 0.f)
			m_pAbility->fHP = 0.f;
	}

	void		Damaged(_float fDamage);


public:
	char			szName[MAX_PATH];




	_float			m_fAnimationTime = 0.f;
	_bool			m_AnimFinish = false;



	_double			m_fAnimAdjust = 1.6;
	_double			m_fLerpAnimTime = 0.2;

	CCollider*		m_pOverlapCollider;

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END