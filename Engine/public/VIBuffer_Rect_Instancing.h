#pragma once
#include "VIBuffer_Instancing.h"
BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instancing final : public  CVIBuffer_Instancing
{
public:
	CVIBuffer_Rect_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Rect_Instancing(const CVIBuffer_Rect_Instancing& rhs);
	virtual ~CVIBuffer_Rect_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT	Tick(_double TimeDelta) override;
	virtual HRESULT Render();


protected:
	_double*		m_pSpeed = nullptr;


public:
	static CVIBuffer_Rect_Instancing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;


};

END