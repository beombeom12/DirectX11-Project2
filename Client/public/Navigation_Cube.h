#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRenderer;
class CShader;
class CVIBuffer_Square;

END

BEGIN(Client)
class CNavigation_Cube final : public CGameObject
{
public:
	CNavigation_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation_Cube(const CNavigation_Cube& rhs);
	virtual ~CNavigation_Cube() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg) override;
	virtual _uint Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT		SetUp_Component(void);
	HRESULT		SetUp_ShaderResource(void);
public:
	_bool		Peeking();
private:
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_Square*		m_pVIBufferCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;


private:
	_float4					m_vPickingPos = _float4(0.f, 0.f, 1.f, 1.f);

public:
	static CNavigation_Cube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END