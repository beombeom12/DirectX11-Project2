#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CState;
END

BEGIN(Client)

class CMonster final : public CGameObject
{
//public:
//	enum COLLIDERTYPE { COLLTYPE_AABB, COLLTYPE_OBB, COLLTYPE_SPHERE, COLLTYPE_SPHERE_TARGET, COLLTYPE_END };

private:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;
	friend class CMonster_State;

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
	CState*						m_pStateCom = nullptr;

	class CMonster_State*		m_MonsterState = nullptr;	
private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_ShaderResources();
	HRESULT				SetUp_State();

private:
	_bool				Collision_ToPlayer();
	_bool				Collision_Sword();

	_bool				Collision_ChaseRange();
private:
	_bool			m_AnimFinish = false;
	vector<CGameObject*>	m_pMonsterParts;

public:
	static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END