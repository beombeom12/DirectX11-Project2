#include "..\public\VIBuffer_Point_Window.h"


CVIBuffer_Point_Window::CVIBuffer_Point_Window(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CVIBuffer_Instancing(pDevice, pContext)
{
}

CVIBuffer_Point_Window::CVIBuffer_Point_Window(const CVIBuffer_Point_Window & rhs)
	:CVIBuffer_Instancing(rhs)
{
}

HRESULT CVIBuffer_Point_Window::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Point_Window::Initialize(void * pArg)
{
	if (pArg == nullptr)
		return E_FAIL;
	ZeroMemory(&m_DustDesc, sizeof(WINDOW));
	memcpy(&m_DustDesc, pArg, sizeof(WINDOW));

	m_iNumInstance = m_DustDesc.iNumInstance;
	m_pData = new Data[m_iNumInstance];
	ZeroMemory(m_pData, sizeof(Data) * m_iNumInstance);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_pData[i].fSpeed = (_float)((rand() % 3) + 2) + ((rand() % 10) + 1) * 0.1f;
		XMStoreFloat3(&m_pData[i].vDirection, XMVector3Normalize(XMVectorSet((rand() % 10) - 4.5f, (rand() % 10) - 4.5f, (rand() % 10) - 4.5f, 0.f)));

		if (m_DustDesc.bUse)
		{
			m_pData[i].fDownRangeY = (_float)(rand() % 2) + ((rand() % 10) + 1) * 0.1f;
			m_pData[i].fDownValueY = ((rand() % 10) + 1) * 0.1f;
		}

	}


	m_pDataReal = new Data[m_iNumInstance];
	ZeroMemory(m_pDataReal, sizeof(Data) * m_iNumInstance);
	memcpy(m_pDataReal, m_pData, sizeof(Data));

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

	pVertices->vPosition = _float3(0.0f, -2.0f, 0.0f);
	pVertices->vPSize = m_DustDesc.fSize;

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

	VTXMATRIX*			pInstanceVertices = new VTXMATRIX[m_iNumInstance];
	ZeroMemory(pInstanceVertices, sizeof(VTXMATRIX));

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceVertices[i].vRight = _float4(1.0f, 0.f, 0.f, 0.f);
		pInstanceVertices[i].vUp = _float4(0.0f, 1.f, 0.f, 0.f);
		pInstanceVertices[i].vLook = _float4(0.0f, 0.f, 1.f, 0.f);
		pInstanceVertices[i].vPosition = _float4(0.0f, 0.f, 0.f, 1.f);
	}

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pInstanceVertices;

	m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pInstanceBuffer);

	Safe_Delete_Array(pInstanceVertices);
#pragma endregion



	return S_OK;
}

HRESULT CVIBuffer_Point_Window::Tick(_double TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE			SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	_float4 *p = nullptr;
	_vector vPos;


	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		p = &((VTXMATRIX*)SubResource.pData)[i].vPosition;

		if (m_pData[i].bDonwFlag)
		{
			vPos = XMLoadFloat4(p) - XMVectorSetZ(XMLoadFloat3(&m_pData[i].vDirection), 0.f) * XMVectorSetX(XMLoadFloat3(&m_pData[i].vDirection), 0.f) * XMVectorSetY(XMLoadFloat3(&m_pData[i].vDirection), 0.f) * (m_pData[i].fSpeed * 0.5f) * TimeDelta;
			XMStoreFloat4(p, XMVectorSetW(vPos, 1.f));
			p->y += m_pData[i].fSpeed * TimeDelta;
			p->x -= m_pData[i].fSpeed * 0.2 * TimeDelta;
			p->z -= m_pData[i].fSpeed * TimeDelta;
		}
		else
		{
			_float fDistance = XMVectorGetX(XMVector3Length(XMVectorSet(0.f, 0.f, 0.f, 1.f) - XMLoadFloat4(p)));
			_float fSpeed = fDistance < m_pData[i].fDownRangeY * 0.2f ? m_pData[i].fSpeed * 0.5f : m_pData[i].fSpeed;

			vPos = XMLoadFloat4(p) + XMLoadFloat3(&m_pData[i].vDirection) * fSpeed * TimeDelta;
			XMStoreFloat4(p, XMVectorSetW(vPos, 1.f));

			m_pData[i].bDonwFlag = m_DustDesc.bUse && (fDistance > m_pData[i].fDownRangeY);
		}
	}
	m_pContext->Unmap(m_pInstanceBuffer, 0);

	return S_OK;
}

HRESULT CVIBuffer_Point_Window::Render()
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

void CVIBuffer_Point_Window::Reset(_bool fSpeedChange)
{
	D3D11_MAPPED_SUBRESOURCE SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXMATRIX*)SubResource.pData)[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);

	memcpy(m_pData, m_pDataReal, sizeof(Data) * m_iNumInstance);
	if (fSpeedChange)
	{
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{

			m_pData[i].fSpeed = (_float)((rand() % 3) + 2) + ((rand() % 10) + 1) * 0.1f;
			XMStoreFloat3(&m_pData[i].vDirection, XMVector3Normalize(XMVectorSet((rand() % 10) - 4.5f, (rand() % 10) - 4.5f, (rand() % 10) - 4.5f, 0.f)));

			if (m_DustDesc.bUse)
			{
				m_pData[i].fDownRangeY = (_float)(rand() % 2) + ((rand() % 10) + 1) * 0.1f;
				m_pData[i].fDownValueY = ((rand() % 10) + 1) * 0.1f;
			}

		}

	}
}

CVIBuffer_Point_Window * CVIBuffer_Point_Window::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Point_Window*		pInstance = new CVIBuffer_Point_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CVIBuffer_Point_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Point_Window::Clone(void * pArg)
{
	CVIBuffer_Point_Window*		pInstance = new CVIBuffer_Point_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create  Clone: CVIBuffer_Point_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Point_Window::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pData);
	Safe_Delete_Array(m_pDataReal);
}
