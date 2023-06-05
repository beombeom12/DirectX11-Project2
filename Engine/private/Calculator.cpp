#include "..\public\Calculator.h"
#include "GameInstance.h"

CCalculator::CCalculator(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CCalculator::CCalculator(const CCalculator & rhs)
	: CComponent(rhs)
{
}

HRESULT CCalculator::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCalculator::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCalculator::Calculate_MousePos(HWND hWnd)
{
	GetCursorPos(&m_MousePos);
	ScreenToClient(hWnd, &m_MousePos);
}

_float4 CCalculator::Ray_Peeking(const CVIBuffer_Terrain * pBuffer, const CTransform * pTransform)
{
	ENGINE_OPEN;

	// 1. 마우스 포인터 위치를 가져옴
	_float4 CalPt = _float4(0.f, 0.f, 1.f, 1.f);

	// 2. 마우스 포인터 위치를 투영스페이스로 옮김

	CalPt.x = m_MousePos.x / (WINCX * 0.5f) - 1.f;
	CalPt.y = m_MousePos.y / -(WINCY * 0.5f) + 1.f;

	// 3. 마우스 포인터 위치를 뷰 스페이스로 옮김
	_float4x4		Proj_Matrix;
	XMStoreFloat4x4(&Proj_Matrix, XMMatrixIdentity());
	Proj_Matrix = ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	XMStoreFloat4x4(&Proj_Matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&Proj_Matrix)));
	XMStoreFloat4(&CalPt, XMVector3TransformCoord(XMLoadFloat4(&CalPt), XMLoadFloat4x4(&Proj_Matrix)));

	// 4. 마우스 포인터 위치를 월드스페이스로 옮김 이거 
	_float4 vRayPos, vRayDir;

	_float4x4	View_Matrix;
	XMStoreFloat4x4(&View_Matrix, XMMatrixIdentity());
	View_Matrix = ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	XMStoreFloat4x4(&View_Matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&View_Matrix)));
	memcpy(&vRayPos, &View_Matrix.m[3][0], sizeof(_float4));
	XMStoreFloat4(&CalPt, XMVector3TransformCoord(XMLoadFloat4(&CalPt), XMLoadFloat4x4(&View_Matrix)));
	XMStoreFloat4(&vRayDir, XMLoadFloat4(&CalPt) - XMLoadFloat4(&vRayPos));

	// 5. 마우스 포인터 위치를 로컬스페이스로 옮김
	_float4x4		World_Matrix;
	XMStoreFloat4x4(&World_Matrix, XMMatrixIdentity());
	World_Matrix = pTransform->Get_WorldMatrix();
	XMStoreFloat4x4(&World_Matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&World_Matrix)));
	XMStoreFloat4(&vRayPos, XMVector3TransformCoord(XMLoadFloat4(&vRayPos), XMLoadFloat4x4(&World_Matrix)));
	XMStoreFloat4(&vRayDir, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat4(&vRayDir), XMLoadFloat4x4(&World_Matrix))));

	const _float4*	pVertex = pBuffer->Get_Vertex();
	const _uint		iNumX = pBuffer->Get_Vertex_X();
	const _uint		iNumZ = pBuffer->Get_Vertex_Z();
	const _uint		iNumPrimitive = pBuffer->Get_Primitive();

	_ulong	dwIndex[3]{};
	_float	fDist = 0.f;

	for (_ulong z = 0; z < iNumZ - 1; ++z)
	{
		for (_ulong x = 0; x < iNumX - 1; ++x)
		{
			_ulong Index = z * iNumX + x;

			dwIndex[0] = Index + iNumX;
			dwIndex[1] = Index + iNumX + 1;
			dwIndex[2] = Index + 1;

			if (TriangleTests::Intersects(
				XMLoadFloat4(&vRayPos),
				XMLoadFloat4(&vRayDir),
				XMLoadFloat4(&pVertex[dwIndex[1]]),
				XMLoadFloat4(&pVertex[dwIndex[0]]),
				XMLoadFloat4(&pVertex[dwIndex[2]]),
				fDist))
			{
				ENGINE_CLOSE;

				_float4      NumSam;

				XMStoreFloat4(&NumSam, (XMLoadFloat4(&vRayPos) + (XMLoadFloat4(&vRayDir)) * fDist));

				return NumSam;
			}

			dwIndex[0] = Index + iNumX;
			dwIndex[1] = Index + 1;
			dwIndex[2] = Index;

			if (TriangleTests::Intersects(
				XMLoadFloat4(&vRayPos),
				XMLoadFloat4(&vRayDir),
				XMLoadFloat4(&pVertex[dwIndex[2]]),
				XMLoadFloat4(&pVertex[dwIndex[1]]),
				XMLoadFloat4(&pVertex[dwIndex[0]]),
				fDist))
			{
				ENGINE_CLOSE;
			
				_float4      NumSam;

				XMStoreFloat4(&NumSam, (XMLoadFloat4(&vRayPos) + (XMLoadFloat4(&vRayDir)) * fDist));

				return NumSam;
			}

		}
	}

	ENGINE_CLOSE;

	return _float4(0.f, 0.f, 0.f, 1.f);
}

_bool CCalculator::Ray_CubePeeking(const CVIBuffer_Square * pBuffer, const CTransform * pTransform)
{
	ENGINE_OPEN;

	_float4	CalPt = _float4(0.f, 0.f, 1.f, 1.f);
	D3D11_VIEWPORT	Viewport;
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));
	CalPt.x = m_MousePos.x / (WINCX * 0.5f) - 1.f;
	CalPt.y = m_MousePos.y / -(WINCY * 0.5f) + 1.f;

	_float4x4		Proj_Matrix;
	XMStoreFloat4x4(&Proj_Matrix, XMMatrixIdentity());
	Proj_Matrix = ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	XMStoreFloat4x4(&Proj_Matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&Proj_Matrix)));
	XMStoreFloat4(&CalPt, XMVector3TransformCoord(XMLoadFloat4(&CalPt), XMLoadFloat4x4(&Proj_Matrix)));

	_float4	vRayPos, vRayDir;

	_float4x4		View_Matrix;
	XMStoreFloat4x4(&View_Matrix, XMMatrixIdentity());
	View_Matrix = ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	XMStoreFloat4x4(&View_Matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&View_Matrix)));
	memcpy(&vRayPos, &View_Matrix.m[3][0], sizeof(_float4));
	XMStoreFloat4(&CalPt, XMVector3TransformCoord(XMLoadFloat4(&CalPt), XMLoadFloat4x4(&View_Matrix)));
	XMStoreFloat4(&vRayDir, XMLoadFloat4(&CalPt) - XMLoadFloat4(&vRayPos));

	_float4x4		World_Matrix;
	XMStoreFloat4x4(&World_Matrix, XMMatrixIdentity());
	World_Matrix = pTransform->Get_WorldMatrix();
	XMStoreFloat4x4(&World_Matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&World_Matrix)));
	XMStoreFloat4(&vRayPos, XMVector3TransformCoord(XMLoadFloat4(&vRayPos), XMLoadFloat4x4(&World_Matrix)));
	XMStoreFloat4(&vRayDir, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat4(&vRayDir), XMLoadFloat4x4(&World_Matrix))));

	const _float4*	pVertex = pBuffer->Get_Vertex();

	_ulong	dwIndex[3]{};
	_float	fDist = 0.f;

	dwIndex[0] = 0;
	dwIndex[1] = 1;
	dwIndex[2] = 2;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	dwIndex[0] = 0;
	dwIndex[1] = 2;
	dwIndex[2] = 3;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}
	// +Z
	dwIndex[0] = 5;
	dwIndex[1] = 4;
	dwIndex[2] = 7;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	dwIndex[0] = 5;
	dwIndex[1] = 7;
	dwIndex[2] = 6;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	// +X
	dwIndex[0] = 1;
	dwIndex[1] = 5;
	dwIndex[2] = 6;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	dwIndex[0] = 1;
	dwIndex[1] = 6;
	dwIndex[2] = 2;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	// -X
	dwIndex[0] = 4;
	dwIndex[1] = 0;
	dwIndex[2] = 3;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	dwIndex[0] = 4;
	dwIndex[1] = 3;
	dwIndex[2] = 7;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	// +Y
	dwIndex[0] = 4;
	dwIndex[1] = 5;
	dwIndex[2] = 1;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	dwIndex[0] = 4;
	dwIndex[1] = 1;
	dwIndex[2] = 0;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	// -Y
	dwIndex[0] = 3;
	dwIndex[1] = 2;
	dwIndex[2] = 6;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	dwIndex[0] = 3;
	dwIndex[1] = 6;
	dwIndex[2] = 7;
	if (TriangleTests::Intersects(
		XMLoadFloat4(&vRayPos),
		XMLoadFloat4(&vRayDir),
		XMLoadFloat4(&pVertex[dwIndex[1]]),
		XMLoadFloat4(&pVertex[dwIndex[0]]),
		XMLoadFloat4(&pVertex[dwIndex[2]]),
		fDist))
	{
		ENGINE_CLOSE;
		return true;
	}

	ENGINE_CLOSE;
	return false;
}

void CCalculator::Ray_Peeking_FilterMap(const CVIBuffer_Terrain * pBuffer, const CTransform * pTransform, _bool bOption)
{
	ENGINE_OPEN;


	_float4	CalPt = _float4(0.f, 0.f, 1.f, 1.f);

	CalPt.x = m_MousePos.x / (WINCX * 0.5f) - 1.f;
	CalPt.y = m_MousePos.y / -(WINCY * 0.5f) + 1.f;


	_float4x4		Proj_Matrix;
	XMStoreFloat4x4(&Proj_Matrix, XMMatrixIdentity());
	Proj_Matrix = ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	XMStoreFloat4x4(&Proj_Matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&Proj_Matrix)));
	XMStoreFloat4(&CalPt, XMVector3TransformCoord(XMLoadFloat4(&CalPt), XMLoadFloat4x4(&Proj_Matrix)));

	_float4 vRayPos, vRayDir;

	_float4x4	View_Matrix;
	XMStoreFloat4x4(&Proj_Matrix, XMMatrixIdentity());
	View_Matrix = ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	XMStoreFloat4x4(&View_Matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&Proj_Matrix)));
	memcpy(&vRayPos, &View_Matrix.m[3][0], sizeof(_float4));
	XMStoreFloat4(&CalPt, XMVector3TransformCoord(XMLoadFloat4(&CalPt), XMLoadFloat4x4(&View_Matrix)));
	XMStoreFloat4(&vRayDir, XMLoadFloat4(&CalPt) - XMLoadFloat4(&vRayPos));

	_float4x4		World_Matrix;
	XMStoreFloat4x4(&World_Matrix, XMMatrixIdentity());
	World_Matrix = pTransform->Get_WorldMatrix();
	XMStoreFloat4x4(&View_Matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&World_Matrix)));
	XMStoreFloat4(&vRayPos, XMVector3TransformCoord(XMLoadFloat4(&vRayPos), XMLoadFloat4x4(&World_Matrix)));
	XMStoreFloat4(&vRayDir, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat4(&vRayDir), XMLoadFloat4x4(&World_Matrix))));

	const _float4*	pVertex = pBuffer->Get_Vertex();
	const _uint		iNumX = pBuffer->Get_Vertex_X();
	const _uint		iNumZ = pBuffer->Get_Vertex_Z();
	const _uint		iNumPrimitive = pBuffer->Get_Primitive();

	_ulong dwIndex[3]{};
	_float fDist = 0.f;

	for (_ulong z = 0; z < iNumZ - 1; ++z)
	{
		for (_ulong x = 0; x < iNumX - 1; ++x)
		{
			_ulong Index = z * iNumX + x;

			dwIndex[0] = Index + iNumX;
			dwIndex[1] = Index + iNumX + 1;
			dwIndex[2] = Index + 1;

			if (TriangleTests::Intersects(
				XMLoadFloat4(&vRayPos),
				XMLoadFloat4(&vRayDir),
				XMLoadFloat4(&pVertex[dwIndex[1]]),
				XMLoadFloat4(&pVertex[dwIndex[0]]),
				XMLoadFloat4(&pVertex[dwIndex[2]]),
				fDist))
			{
				ENGINE_CLOSE;
				if (bOption)
				{
					m_VertexColor[dwIndex[0]] = 255;
					m_VertexColor[dwIndex[1]] = 255;
					m_VertexColor[dwIndex[2]] = 255;
				}
				else
				{
					m_VertexColor[dwIndex[0]] = 0;
					m_VertexColor[dwIndex[1]] = 0;
					m_VertexColor[dwIndex[2]] = 0;
				}

				Create_FilterMap();

				return;

			}

			dwIndex[0] = Index + iNumX;
			dwIndex[1] = Index + 1;
			dwIndex[2] = Index;

			if (TriangleTests::Intersects(
				XMLoadFloat4(&vRayPos),
				XMLoadFloat4(&vRayDir),
				XMLoadFloat4(&pVertex[dwIndex[2]]),
				XMLoadFloat4(&pVertex[dwIndex[1]]),
				XMLoadFloat4(&pVertex[dwIndex[0]]),
				fDist))
			{
				ENGINE_CLOSE;
				
				if (bOption)
				{
					m_VertexColor[dwIndex[0]] = 255;
					m_VertexColor[dwIndex[0]] = 255;
					m_VertexColor[dwIndex[0]] = 255;
				}
				else
				{
					m_VertexColor[dwIndex[0]] = 0;
					m_VertexColor[dwIndex[1]] = 0;
					m_VertexColor[dwIndex[2]] = 0;
				}

				Create_FilterMap();

				return;
			}
		}
	}

	ENGINE_CLOSE;
	
	return;
}

HRESULT CCalculator::Create_FilterMap(void)
{
	ID3D11Texture2D*		pTexture2D = nullptr;

	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = FILTERSIZE;
	TextureDesc.Height = FILTERSIZE;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pTexture2D)))
		return E_FAIL;

	_ulong*	pPixel = new _ulong[FILTERSIZE * FILTERSIZE];

	for (_ulong y = 0; y < FILTERSIZE; ++y)
	{	
		for (_ulong x = 0; x < FILTERSIZE; ++x)
		{
			_ulong dwIndex = y * FILTERSIZE + x;

			pPixel[dwIndex] = D3DCOLOR_ARGB(255, m_VertexColor[dwIndex], m_VertexColor[dwIndex], m_VertexColor[dwIndex]);
		}
	}

	D3D11_MAPPED_SUBRESOURCE	SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	memcpy(SubResource.pData, pPixel, sizeof(_ulong) * FILTERSIZE * FILTERSIZE);

	m_pContext->Unmap(pTexture2D, 0);

	if (FAILED(DirectX::SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../../Data/Default.dds"))))
		return E_FAIL;

	Safe_Release(pTexture2D);
	Safe_Delete_Array(pPixel);

	return S_OK;
}

CCalculator * CCalculator::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCalculator*	pInstance = new CCalculator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CCalcuator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCalculator::Clone(void * pArg)
{
	CCalculator*	pInstance = new CCalculator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCalculator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCalculator::Free()
{
	__super::Free();
}
