#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCalculator : public CComponent
{
private:
	CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCalculator(const CCalculator& rhs);
	virtual ~CCalculator() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void	Calculate_MousePos(HWND hWnd);

public:
	_float4 Ray_Peeking(const class CVIBuffer_Terrain* pBuffer, const class CTransform* pTransform);
	_bool	Ray_CubePeeking(const class CVIBuffer_Square* pBuffer, const class CTransform* pTransform);

	// FilterMap 나중에 쓸수도있음
	void	Ray_Peeking_FilterMap(const class CVIBuffer_Terrain* pBuffer, const class CTransform* pTransform, _bool bOption);
	HRESULT Create_FilterMap(void);

private:
	POINT	m_MousePos = { 0, 0 };

private:
	_ulong	m_VertexColor[FILTERSIZE * FILTERSIZE] = { 0, };

public:
	static CCalculator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free();
};

END