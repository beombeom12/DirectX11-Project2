#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Square : public CVIBuffer
{
private:
	CVIBuffer_Square(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Square(const CVIBuffer_Square& rhs);
	virtual ~CVIBuffer_Square() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	const _float4* Get_Vertex(void) const { return m_VertexPos; }

private:
	_float4*		m_VertexPos = nullptr;

public:
	static CVIBuffer_Square* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free();
};

END