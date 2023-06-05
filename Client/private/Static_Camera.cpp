#include "stdafx.h"
#include "..\public\Static_Camera.h"
#include "GameInstance.h"

CStatic_Camera::CStatic_Camera(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera(pDevice, pContext)
{
}

CStatic_Camera::CStatic_Camera(const CStatic_Camera & rhs)
	: CCamera(rhs)
{
}

HRESULT CStatic_Camera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatic_Camera::Initialize(void * pArg)
{
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof CameraDesc);

	if (nullptr != pArg)
		memcpy(&CameraDesc, pArg, sizeof(CAMERADESC));
	else
	{
		CameraDesc.vEye = _float4(0.f, 10.f, -10.f, 1.f);
		CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
		CameraDesc.vUp = _float4(0.f, 1.f, 0.f, 0.f);

		m_CameraDesc.FOV = XMConvertToRadians(90.f);
		m_CameraDesc.Aspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
		m_CameraDesc.NearZ = 0.1f;
		m_CameraDesc.FarZ = 300.f;

		CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
		CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	}

	if (FAILED(CCamera::Initialize(&CameraDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_uint CStatic_Camera::Tick(_double TimeDelta)
{	

	return __super::Tick(TimeDelta);
}

void CStatic_Camera::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CStatic_Camera::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatic_Camera::SetUp_Components()
{
	return S_OK;
}

CStatic_Camera * CStatic_Camera::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CStatic_Camera*		pInstance = new CStatic_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failled to Create : CStatic_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStatic_Camera::Clone(void * pArg)
{
	CStatic_Camera*		pInstance = new CStatic_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CStatic_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatic_Camera::Free()
{
	__super::Free();
}
