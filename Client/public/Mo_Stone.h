#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CTransform;
END

BEGIN(Client)

class CMo_Stone final : public CGameObject
{
private:
	CMo_Stone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMo_Stone(const CMo_Stone& rhs);
	virtual ~CMo_Stone() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID) ;
	virtual HRESULT Initialize(void* pArg) ;
	virtual _uint Tick(_double TimeDelta) ;
	virtual void Late_Tick(_double TimeDelta) ;
	virtual HRESULT Render() ;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();


public:
	void	Shoot_Stone(_double TimeDelta);
	_float4	V = { 0.f, 0.f, 0.f, 0.f };

	_bool			m_bFirst = false;
	CCollider*			Get_StoneCollider() { return m_pColliderCom; }
public:
	static CMo_Stone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END