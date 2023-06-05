#include "..\public\VIBuffer_Square.h"

CVIBuffer_Square::CVIBuffer_Square(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Square::CVIBuffer_Square(const CVIBuffer_Square & rhs)
	: CVIBuffer(rhs)
	, m_VertexPos(rhs.m_VertexPos)
{
}

HRESULT CVIBuffer_Square::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_iNumVertexBuffers = 1;
	m_iStride = sizeof(VTXSQUARE);
	m_iNumVertices = 8;
	m_iNumPrimitive = 12;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_iIndicesSizePerPrimitive = sizeof(FACEINDICES16);
	m_iNumIndicesPerPrimitive = 3;
	m_iNumIndices = m_iNumIndicesPerPrimitive * m_iNumPrimitive;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	m_VertexPos = new _float4[m_iNumVertices];
	ZeroMemory(m_VertexPos, sizeof(_float4) * m_iNumVertices);

	VTXSQUARE* pVertex = new VTXSQUARE[m_iNumVertices];
	ZeroMemory(pVertex, sizeof(VTXSQUARE) * m_iNumVertices);

	pVertex[0].vPosition = _float3(-0.5, 0.5, 0.5f);
	pVertex[0].vTextureUV = _float3(0.f, 0.f, 0.f);
	m_VertexPos[0] = _float4(
		pVertex[0].vPosition.x,
		pVertex[0].vPosition.y,
		pVertex[0].vPosition.z, 1.f);

	pVertex[1].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertex[1].vTextureUV = _float3(1.f, 0.f, 0.f);
	m_VertexPos[1] = _float4(
		pVertex[1].vPosition.x,
		pVertex[1].vPosition.y,
		pVertex[1].vPosition.z, 1.f);

	pVertex[2].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertex[2].vTextureUV = _float3(1.f, 1.f, 0.f);
	m_VertexPos[2] = _float4(
		pVertex[2].vPosition.x,
		pVertex[2].vPosition.y,
		pVertex[2].vPosition.z, 1.f);

	pVertex[3].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertex[3].vTextureUV = _float3(0.5f, 1.f, 0.f);
	m_VertexPos[3] = _float4(
		pVertex[3].vPosition.x,
		pVertex[3].vPosition.y,
		pVertex[3].vPosition.z, 1.f);

	pVertex[4].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertex[4].vTextureUV = _float3(0.f, 0.f, 1.f);
	m_VertexPos[4] = _float4(
		pVertex[4].vPosition.x,
		pVertex[4].vPosition.y,
		pVertex[4].vPosition.z, 1.f);

	pVertex[5].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertex[5].vTextureUV = _float3(1.f, 0.f, 1.f);
	m_VertexPos[5] = _float4(
		pVertex[5].vPosition.x,
		pVertex[5].vPosition.y,
		pVertex[5].vPosition.z, 1.f);

	pVertex[6].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertex[6].vTextureUV = _float3(1.f, 1.f, 1.f);
	m_VertexPos[6] = _float4(
		pVertex[6].vPosition.x,
		pVertex[6].vPosition.y,
		pVertex[6].vPosition.z, 1.f);

	pVertex[7].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertex[7].vTextureUV = _float3(0.f, 1.f, 1.f);
	m_VertexPos[7] = _float4(
		pVertex[7].vPosition.x,
		pVertex[7].vPosition.y,
		pVertex[7].vPosition.z, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertex;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertex);
	
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_BufferDesc.ByteWidth = m_iIndicesSizePerPrimitive * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	FACEINDICES16* pIndex = new FACEINDICES16[m_iNumIndices];
	ZeroMemory(pIndex, sizeof(FACEINDICES16) * m_iNumIndices);

	// -Z
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	// +Z
	pIndex[2]._0 = 5;
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 7;

	pIndex[3]._0 = 5;
	pIndex[3]._1 = 7;
	pIndex[3]._2 = 6;

	// +X
	pIndex[4]._0 = 1;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 6;

	pIndex[5]._0 = 1;
	pIndex[5]._1 = 6;
	pIndex[5]._2 = 2;

	// -X
	pIndex[6]._0 = 4;
	pIndex[6]._1 = 0;
	pIndex[6]._2 = 3;

	pIndex[7]._0 = 4;
	pIndex[7]._1 = 3;
	pIndex[7]._2 = 7;

	// +Y
	pIndex[8]._0 = 4;
	pIndex[8]._1 = 5;
	pIndex[8]._2 = 1;

	pIndex[9]._0 = 4;
	pIndex[9]._1 = 1;
	pIndex[9]._2 = 0;

	// -Y
	pIndex[10]._0 = 3;
	pIndex[10]._1 = 2;
	pIndex[10]._2 = 6;

	pIndex[11]._0 = 3;
	pIndex[11]._1 = 6;
	pIndex[11]._2 = 7;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndex;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndex);

	return S_OK;
}

HRESULT CVIBuffer_Square::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Square * CVIBuffer_Square::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Square*	pInstance = new CVIBuffer_Square(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CVIBuffer_Square");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Square::Clone(void * pArg)
{
	CVIBuffer_Square*	pInstance = new CVIBuffer_Square(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create : CVIBuffer_Square");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Square::Free()
{
	__super::Free();

	if (false == m_bClone)
		Safe_Delete_Array(m_VertexPos);
}
