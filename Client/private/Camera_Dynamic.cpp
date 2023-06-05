#include "stdafx.h"
#include "..\public\Camera_Dynamic.h"
#include "GameInstance.h"
#include "Bone.h"
#include "Weapon.h"
#include "Model.h"
CCamera_Dynamic::CCamera_Dynamic(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::Initialize(void * pArg)
{	


	ZeroMemory(&CameraDesc, sizeof (CAMERADESC));

	if (nullptr != pArg)
		memcpy(&CameraDesc, pArg, sizeof(CAMERADESC));
	else
	{
		CameraDesc.vEye = _float4(0.f, 10.f, -10.f, 1.f);
		CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
		CameraDesc.vUp = _float4(0.f, 0.5f, 0.f, 0.f);

		CameraDesc.FOV = XMConvertToRadians(60.f);
		CameraDesc.Aspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
		CameraDesc.NearZ = 0.1f;
		CameraDesc.FarZ = 300.f;



		CameraDesc.m_MouseMovement = 0.1f;

		CameraDesc.TransformDesc.fSpeedPerSec = 60.f;
		CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	}



	if (FAILED(CCamera::Initialize(&CameraDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	
	return S_OK;
}

_uint CCamera_Dynamic::Tick(_double TimeDelta)
{
	

	ENGINE_OPEN;

	if (ENGINE->Key_Pressing(DIK_UP))
		m_pTransformCom->Camera_GoStraight(TimeDelta);

	if (ENGINE->Key_Pressing(DIK_DOWN))
		m_pTransformCom->Camera_GoBackWard(TimeDelta);

	if (ENGINE->Key_Pressing(DIK_LEFT))
		m_pTransformCom->Camera_GoLeft(TimeDelta);

	if (ENGINE->Key_Pressing(DIK_RIGHT))
		m_pTransformCom->Camera_GoRight(TimeDelta);

	if (ENGINE->Mouse_Pressing(DIM_RB))
	{
		_long	MoveX = 0, MoveY = 0;

		if (MoveX = ENGINE->Get_DIMouseMove(DIMS_X))
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)TimeDelta * (_float)MoveX * m_CameraDesc.m_MouseMovement);

		if (MoveY = ENGINE->Get_DIMouseMove(DIMS_Y))
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), (_float)TimeDelta * (_float)MoveY * m_CameraDesc.m_MouseMovement);

	}


	Player_Chase(TimeDelta);



	ENGINE_CLOSE;

	return __super::Tick(TimeDelta);
}

void CCamera_Dynamic::Late_Tick(_double TimeDelta)
{

	//Player_BoneChase(TimeDelta);

	__super::Late_Tick(TimeDelta);

	

}

HRESULT CCamera_Dynamic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	return S_OK;
}

void CCamera_Dynamic::Player_Chase(_double TimeDelta)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector pPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);


	_vector pPlayer = XMVector3Normalize(pPlayerPos);

	_long	MoveX = 0, MoveY = 0;

	if (MoveX  = pGameInstance->Get_DIMouseMove(DIMS_X))
	 	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)TimeDelta * (_float)MoveX * m_CameraDesc.m_MouseMovement);


	_vector pCamPos = pPlayerPos;
	_float4 vPos;
	XMStoreFloat4(&vPos, pCamPos);

	_float4 vPlay = { vPos.x - 30.f, vPos.y + 30.f, vPos.z ,1.f };
	_vector V = XMLoadFloat4(&vPlay);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, V);

	m_pTransformCom->LookAt(pPlayerPos + XMVectorSet(0.f, 0.5f, 0.f, 0.f));




	//_vector pPlayerLook = pTargetPos->Get_State(CTransform::STATE_LOOK);

	//_vector pPlayer = XMVector3Normalize(pPlayerLook);

	//_vector pCameraPos = pPlayerPos + (pPlayer  * -30.f);

	//_float4 vPos;
	//XMStoreFloat4(&vPos, pCameraPos);

	//_float4 vPlay = { vPos.x, vPos.y + 30.f, vPos.z, 1.f };

	//_vector V = XMLoadFloat4(&vPlay);

	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, V);

	////m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta);
	//m_pTransformCom->LookAt(pPlayerPos + XMVectorSet(0.f, 0.5f, 0.f, 0.f));


	RELEASE_INSTANCE(CGameInstance);



}

void CCamera_Dynamic::Player_BoneChase(_double TimeDelta)
{

	_matrix	SocketMatrix = CameraDesc.pSocket->Get_OffsetMatrix() *
		CameraDesc.pSocket->Get_CombindMatrix() * XMLoadFloat4x4(CameraDesc.pPivotMatrix);
	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	SocketMatrix= SocketMatrix * CameraDesc.pTransform->Get_tWorldMatrix();
	
	XMStoreFloat4x4(&m_SocketMatrix, SocketMatrix);
	
}



HRESULT CCamera_Dynamic::SetUp_Components()
{
	return S_OK;
}

HRESULT CCamera_Dynamic::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_SocketMatrix", &m_SocketMatrix)))
		return E_FAIL;

	
	return S_OK;
}

CCamera_Dynamic * CCamera_Dynamic::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCamera_Dynamic*		pInstance = new CCamera_Dynamic(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void * pArg)
{
	CCamera_Dynamic*		pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Dynamic::Free()
{
	__super::Free();

	for (auto& pPart : m_pPlayerParts)
		Safe_Release(pPart);

	if (true == m_isClone)
	{
		Safe_Release(CameraDesc.pSocket);
		Safe_Release(CameraDesc.pTransform);
	}
	m_pPlayerParts.clear();
}
