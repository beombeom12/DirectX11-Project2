#pragma once
#include "Client_Defines.h"
#include "Installation.h"

BEGIN(Engine)
class CCalculator;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CDesertTerrain : public CInstallation
{
private:
	CDesertTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDesertTerrain(const CDesertTerrain & rhs);
	virtual ~CDesertTerrain()  = default;


public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();


private:
	HRESULT SetUp_Model();

public:
	_float4					Get_Pick(void) const { return m_vPickingPos; }

private:
	_float4					m_vPickingPos = _float4(0.f, 0.f, 0.f, 1.f);
	CVIBuffer_Terrain*			m_pVIBufferCom;
public:
	static CDesertTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);

	virtual void Free();

};

END