#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Terrain;
class CCalculator;
class CNavigation;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_BRUSH, TYPE_FILTER, TYPE_PICKING, TYPE_END };

public:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID)		override;
	virtual HRESULT Initialize(void* pArg)					override;
	virtual _uint Tick(_double TimeDelta)					override;
	virtual void Late_Tick(_double TimeDelta)				override;
	virtual HRESULT Render()								override;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTexture*				m_pTextureCom[TYPE_END] = { nullptr };
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();

public:
	_float4					Get_Pick(void) const { return m_vPickingPos; }
	CNavigation*			Get_NaviCom(void) { return m_pNavigationCom; }
private:
	_float4					m_vPickingPos = _float4(0.f, 0.f, 0.f, 1.f);

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END