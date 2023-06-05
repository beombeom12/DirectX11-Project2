#pragma once

#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_PlayerDust final : public CVIBuffer_Instancing
{
public:
	CVIBuffer_Point_PlayerDust(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_PlayerDust(const CVIBuffer_Point_PlayerDust& rhs);
	virtual ~CVIBuffer_Point_PlayerDust() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Tick(_double TimeDelta) override;
	virtual HRESULT Render();

private:
	_double*				m_pSpeeds = nullptr;

public:
	static CVIBuffer_Point_PlayerDust* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance = 10);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END