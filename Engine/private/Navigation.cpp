#include "..\public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "GameInstance.h"
#include "PipeLine.h"
CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
#endif // _DEBUG
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif // _DEBUG

}

HRESULT CNavigation::Initialize_Prototype(const _tchar * pNavigationDataFilePath)
{
	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pNavigationDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3		vPoints[CCell::POINT_END];

	while (true)
	{
		ReadFile(hFile, vPoints, sizeof(_float3) * CCell::POINT_END, &dwByte, nullptr);
		if (0 == dwByte)
			break;


		CCell*		pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_int)m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;
		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(Ready_Neighbor()))
		return E_FAIL;

#ifdef _DEBUG

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CNavigation::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_NaviDesc, pArg, sizeof(NAVIDESC));
	}


	return S_OK;
}

HRESULT CNavigation::iGo(const _tchar* pNavigationDataFilePath)
{

	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pNavigationDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3		vPoints[CCell::POINT_END];

	while (true)
	{
		for (_uint i = 0; i < 3; ++i)
		{
			ReadFile(hFile, &vPoints[i], sizeof(_float3), &dwByte, nullptr);
		}

		if (0 == dwByte)
			break;

		CCell*		pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_int)m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);
	
	if (FAILED(Ready_Neighbor()))
		return E_FAIL;

	return S_OK;

}

_bool CNavigation::isMove_OnNavigation(_fvector TargetPos)
{
	if (-1 == m_NaviDesc.iCurrentIndex)
		return false;

	_int		iNeighborIndex = -1;

	
	if (true == m_Cells[m_NaviDesc.iCurrentIndex]->isIn(TargetPos, &iNeighborIndex))
		return true; 
			
	else
	{


	
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
				{
					return false;
				}

				if (true == m_Cells[iNeighborIndex]->isIn(TargetPos, &iNeighborIndex))
				{
					m_NaviDesc.iCurrentIndex = iNeighborIndex;
					return true;
				}
			}
		}

		else
		{
			return false;
		}
	}

}

_int CNavigation::isPeeking_NaviCell(HWND hWnd, _uint iWinsizeX, _uint iWinsizeY)
{
	POINT      ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_float3         vMousePos;

	vMousePos.x = _float(ptMouse.x / (iWinsizeX * 0.5f) - 1);
	vMousePos.y = _float(ptMouse.y / (iWinsizeY * -0.5f) + 1);
	vMousePos.z = 0.f;

	_vector   vecMousePos = XMLoadFloat3(&vMousePos);
	vecMousePos = XMVectorSetW(vecMousePos, 1.f);

	ENGINE_OPEN;

	_matrix      ProjMatrixInv;
	ProjMatrixInv = ENGINE->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrixInv);
	vecMousePos = XMVector3TransformCoord(vecMousePos, ProjMatrixInv);

	_vector   vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f , 1.f };
	vRayDir = vecMousePos - vRayPos;

	_matrix      ViewMatrixInv;
	ViewMatrixInv = ENGINE->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);
	vRayPos = XMVector3TransformCoord(vRayPos, ViewMatrixInv);
	vRayDir = XMVector3TransformNormal(vRayDir, ViewMatrixInv);
	vRayDir = XMVector3Normalize(vRayDir);

	ENGINE_CLOSE;

	_float fDist = 0.f;

	_uint iSize = static_cast<_uint>(m_Cells.size());

	for (_uint i = 0; i < iSize; ++i)
	{
		if (TriangleTests::Intersects(vRayPos, vRayDir,
			XMLoadFloat4(&_float4(m_Cells[i]->Get_Point(CCell::POINT_A).x, m_Cells[i]->Get_Point(CCell::POINT_A).y, m_Cells[i]->Get_Point(CCell::POINT_A).z, 1.f)),
			XMLoadFloat4(&_float4(m_Cells[i]->Get_Point(CCell::POINT_B).x, m_Cells[i]->Get_Point(CCell::POINT_B).y, m_Cells[i]->Get_Point(CCell::POINT_B).z, 1.f)),
			XMLoadFloat4(&_float4(m_Cells[i]->Get_Point(CCell::POINT_C).x, m_Cells[i]->Get_Point(CCell::POINT_C).y, m_Cells[i]->Get_Point(CCell::POINT_C).z, 1.f)),
			fDist))
		{
			return i;
		}
	}
	return -1;
}

HRESULT CNavigation::DeleteCell(_uint DeleteIndex)
{
	if (DeleteIndex == m_Cells.size())
		m_Cells.pop_back();
	else
		m_Cells.erase(m_Cells.begin() + DeleteIndex);

	if (FAILED(Ready_Neighbor()))
		return E_FAIL;

	return S_OK;
}

#ifdef _DEBUG

HRESULT CNavigation::Render()
{
	_float		fHeight = 0.0f;

	if (-1 == m_NaviDesc.iCurrentIndex)
	{
		fHeight = 0.0f;
		HRESULT hr = m_pShader->Set_RawValue("g_fHeight", &fHeight, sizeof(_float));
		m_pShader->Set_RawValue("g_vColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4));
	}
	else
	{
		fHeight = 0.1f;
		HRESULT hr = m_pShader->Set_RawValue("g_fHeight", &fHeight, sizeof(_float));
		m_pShader->Set_RawValue("g_vColor", &_float4(1.f, 0.f, 0.f, 1.f), sizeof(_float4));

		m_Cells[m_NaviDesc.iCurrentIndex]->Render(m_pShader);

		return S_OK;
	}

	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
			pCell->Render(m_pShader);
	}
	if (m_iPickingIndex != -1)
	{
		fHeight = 0.1f;
		HRESULT hr = m_pShader->Set_RawValue("g_fHeight", &fHeight, sizeof(_float));
		m_pShader->Set_RawValue("g_vColor", &_float4(1.f, 1.f, 1.f, 1.f), sizeof(_float4));

		if (m_Cells.size() > m_iPickingIndex)
			m_Cells[m_iPickingIndex]->Render(m_pShader);
		return S_OK;
	}
	return S_OK;
}


void CNavigation::SetPickingIndex(_int iIndex)
{
	m_iPickingIndex= iIndex;
}

#endif 

_float CNavigation::HeightOnTerrain(_fvector vPos)
{

	_vector         Plane;

	Plane = XMPlaneFromPoints(
		XMVectorSetW(XMLoadFloat3(&m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_A)), 1.f),
		XMVectorSetW(XMLoadFloat3(&m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_B)), 1.f),
		XMVectorSetW(XMLoadFloat3(&m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_C)), 1.f));

	_float         fHeight = (-XMVectorGetX(Plane) * XMVectorGetX(vPos) - XMVectorGetZ(Plane) * XMVectorGetZ(vPos) - XMVectorGetW(Plane)) / XMVectorGetY(Plane);

	return fHeight;
}

/* 네비게이션을 구성하는 쎌들의 이웃을 설정한다. */
HRESULT CNavigation::Ready_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::NEIGHBOR_AB, pDestCell);
			}

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::NEIGHBOR_BC, pDestCell);
			}

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::NEIGHBOR_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

//#endif // _DEBUG

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pNavigationDataFilePath)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationDataFilePath)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*		pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // _DEBUG
}
