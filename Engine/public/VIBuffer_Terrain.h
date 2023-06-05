#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer 
{
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMapFilePath);
	virtual HRESULT Initialize(void* pArg);

private:
	_uint		m_iNumVerticesX = 0;
	_uint		m_iNumVerticesZ = 0;

	_float4*	m_VertexPos = nullptr;

public:
	const _float4*	Get_Vertex(void) const { return m_VertexPos; }
	const _uint		Get_Primitive(void) const { return m_iNumPrimitive; }
	const _uint		Get_Vertex_X(void) const { return m_iNumVerticesX; }
	const _uint		Get_Vertex_Z(void) const { return m_iNumVerticesZ; }

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END