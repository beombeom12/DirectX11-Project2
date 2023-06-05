#include "..\public\VIBuffer_Pt_Inst_Explosion.h"

CVIBuffer_Pt_Inst_Explosion::CVIBuffer_Pt_Inst_Explosion(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instancing(pDevice, pContext)
{
}

CVIBuffer_Pt_Inst_Explosion::CVIBuffer_Pt_Inst_Explosion(const CVIBuffer_Pt_Inst_Explosion & rhs)
	: CVIBuffer_Instancing(rhs)
{

}

HRESULT CVIBuffer_Pt_Inst_Explosion::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Pt_Inst_Explosion::Initialize(void * pArg)
{
	ZeroMemory(&m_Desc, sizeof(DESC));

	if(pArg) memcpy(&m_Desc, pArg, sizeof(DESC));
	else return E_FAIL;

	m_pInstanceData = new InstanceData[m_Desc.iNumInstances];
	ZeroMemory(m_pInstanceData, sizeof(InstanceData) * m_Desc.iNumInstances);

	for (_uint i = 0; i < m_Desc.iNumInstances; ++i)
	{
		m_pInstanceData[i].fTime = Rand_FloatRange(m_Desc.fMinTime, m_Desc.fMaxTime);		
		m_pInstanceData[i].fFrame = 0.f;
		m_pInstanceData[i].index = 0;
	}	

	m_pInstanceDataOriginal = new InstanceData[m_Desc.iNumInstances];
	ZeroMemory(m_pInstanceDataOriginal, sizeof(InstanceData) * m_Desc.iNumInstances);
	memcpy(m_pInstanceDataOriginal, m_pInstanceData, sizeof(InstanceData) * m_Desc.iNumInstances);

	if (m_Desc.iNumSpreadInstances > 0)
	{
		m_pSpreadData = new SpreadData[m_Desc.iNumSpreadInstances];
		ZeroMemory(m_pSpreadData, sizeof(SpreadData) * m_Desc.iNumSpreadInstances);
		for (_uint i = 0; i < m_Desc.iNumSpreadInstances; ++i)
		{
			XMStoreFloat4(&m_pSpreadData[i].vDir,
				XMVector4Normalize(XMVectorSet(Rand_FloatRange(-5.f, 5.f), Rand_FloatRange(-5.f, 5.f), Rand_FloatRange(-5.f, 5.f), 0.f)));
			m_pSpreadData[i].fSpeed = Rand_FloatRange(-2.f, 2.f);
		}
	}
	
	
	m_iNumInstance = m_Desc.iNumInstances + m_Desc.iNumSpreadInstances;
	m_iIndexCountPerInstance = 1;
	m_iNumVertexBuffers = 2;
	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;
	m_iNumPrimitive = m_iNumInstance;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_iIndicesSizePerPrimitive = sizeof(_ushort);
	m_iNumIndicesPerPrimitive = 1;
	m_iNumIndices = m_iNumIndicesPerPrimitive * m_iNumPrimitive;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXPOINT*			pVertices = new VTXPOINT;
	ZeroMemory(pVertices, sizeof(VTXPOINT));

	pVertices->vPosition = _float3(0.0f, 0.0f, 0.0f);
	pVertices->vPSize = m_Desc.vMaxSize;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iIndicesSizePerPrimitive * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_ushort*		pIndices = new _ushort[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitive);

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion


#pragma region INSTANCE_BUFFER

	m_iInstanceStride = sizeof(VTXMATRIX);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	_float fRange = m_Desc.fRange;
	_float fRangeOffset = m_Desc.fRange / (_float)m_Desc.iNumInstances;

	_float fDegree = rand() % (360 * 3);
	_float fDegreeOffset = (360.f * 3.f) / (_float)m_Desc.iNumInstances;

	_float fSizeRate = 1.f;
	_float fSizeRateOffset = 1.f / (_float)m_Desc.iNumInstances;

	VTXMATRIX*			pInstanceVertices = new VTXMATRIX[m_iNumInstance];
	ZeroMemory(pInstanceVertices, sizeof(VTXMATRIX));

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		if (i < m_Desc.iNumInstances)
		{
			pInstanceVertices[i].vRight = _float4(1.0f, 0.f, 0.f, fSizeRate);
			pInstanceVertices[i].vUp = _float4(0.0f, 1.f, 0.f, 0.f);
			pInstanceVertices[i].vLook = _float4(0.0f, 0.f, 1.f, 0.f);

			pInstanceVertices[i].vPosition.x = cos(XMConvertToRadians(fDegree)) * fRange;
			pInstanceVertices[i].vPosition.y = sin(XMConvertToRadians(fDegree)) * fRange;
			pInstanceVertices[i].vPosition.z = Rand_FloatRange(m_Desc.fRange * -0.25f, m_Desc.fRange * 0.25f);
			pInstanceVertices[i].vPosition.w = 1.f;

			fDegree += fDegreeOffset;
			fRange = (fRange - fRangeOffset) < 0 ? fRange : (fRange - fRangeOffset);
			fSizeRate = (fSizeRate - fSizeRateOffset) < 0 ? 1.f : (fSizeRate - fSizeRateOffset);
		}
		else
		{
			pInstanceVertices[i].vRight = _float4(1.0f, 0.f, 0.f, Rand_FloatRange(m_Desc.fMinSpreadSizeOffset, m_Desc.fMaxSpreadSizeOffset));
			pInstanceVertices[i].vUp = _float4(0.0f, 1.f, 0.f, (_float)(rand() % m_Desc.iNumSprite));
			pInstanceVertices[i].vLook = _float4(0.0f, 0.f, 1.f, 0.f);
			pInstanceVertices[i].vPosition = _float4(
				Rand_FloatRange(m_Desc.fRange * -m_Desc.fRangeOffset, m_Desc.fRange * m_Desc.fRangeOffset),
				Rand_FloatRange(m_Desc.fRange * -m_Desc.fRangeOffset, m_Desc.fRange * m_Desc.fRangeOffset), 
				Rand_FloatRange(m_Desc.fRange * -m_Desc.fRangeOffset, m_Desc.fRange * m_Desc.fRangeOffset), 1.f);
		}
	}

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pInstanceVertices;

	m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pInstanceBuffer);

	Safe_Delete_Array(pInstanceVertices);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Pt_Inst_Explosion::Tick(_double TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE			SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	_float4 *pPos = nullptr;
	_vector vMovePos;
	_float fDistance = 0.f;
	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i <  m_Desc.iNumInstances; ++i)
	{	
		pPos = &((VTXMATRIX*)SubResource.pData)[i].vUp;

		m_pInstanceData[i].fFrame += TimeDelta;
		if (m_pInstanceData[i].fFrame > m_pInstanceData[i].fTime)
		{
			m_pInstanceData[i].fFrame = 0.f;
			(m_pInstanceData[i].index)++;
			m_pInstanceData[i].index %= m_Desc.iNumSprite;
		}

		pPos->w = m_pInstanceData[i].index;
	}	

	for (_uint i = 0; i < m_Desc.iNumSpreadInstances; ++i)
	{
		pPos = &((VTXMATRIX*)SubResource.pData)[i + m_Desc.iNumInstances].vPosition;
		vMovePos = XMLoadFloat4(pPos) + XMLoadFloat4(&m_pSpreadData[i].vDir) * m_pSpreadData[i].fSpeed;		
		fDistance = XMVectorGetX(XMVector3Length(vMovePos));
		fDistance > m_Desc.fRange ? XMStoreFloat4(pPos, XMVectorSet(0.f, 0.f, 0.f, 1.f)) : XMStoreFloat4(pPos, vMovePos);
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);

	return S_OK;
}

HRESULT CVIBuffer_Pt_Inst_Explosion::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	/* 정점버퍼들을 장치에 바인딩한다.(복수를 바인딩한다.)  */

	ID3D11Buffer*			pVertexBuffers[] = {
		m_pVB,
		m_pInstanceBuffer
	};

	_uint					iStrides[] = {
		m_iStride,
		m_iInstanceStride,
	};

	_uint					iOffsets[] = {
		0,
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);

	/* 인덱스버퍼를 장치에 바인딩한다.(단일로 바인딩한다.)  */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/*m_pContext->DrawIndexed(m_iNumIndices, 0, 0);*/
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

CVIBuffer_Pt_Inst_Explosion * CVIBuffer_Pt_Inst_Explosion::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Pt_Inst_Explosion*		pInstance = new CVIBuffer_Pt_Inst_Explosion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Pt_Inst_Explosion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Pt_Inst_Explosion::Clone(void * pArg)
{
	CVIBuffer_Pt_Inst_Explosion*		pInstance = new CVIBuffer_Pt_Inst_Explosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Pt_Inst_Explosion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Pt_Inst_Explosion::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pInstanceData);
	Safe_Delete_Array(m_pInstanceDataOriginal);
	Safe_Delete_Array(m_pSpreadData);
}

void CVIBuffer_Pt_Inst_Explosion::Reset()
{
	memcpy(m_pInstanceData, m_pInstanceDataOriginal, sizeof(InstanceData) * m_Desc.iNumInstances);
}

