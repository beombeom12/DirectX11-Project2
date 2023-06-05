#pragma once

#include "GameInstance.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CInstallation : public CGameObject
{
protected:
	CInstallation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInstallation(const CInstallation& rhs);
	virtual ~CInstallation() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	HRESULT			SetUp_ShaderResources();

protected:
	HRESULT					SetUp_Component();
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CVIBuffer_Square*		m_pSquareCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

protected:
	CTransform::TRANSFORMDESC	m_TransDesc;

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;
	virtual void Free() override;
};

END