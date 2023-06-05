#include "stdafx.h"
#include "..\public\Effect_Point_PlayerDust.h"




CEffect_Point_PlayerDust::CEffect_Point_PlayerDust(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CEffect_Point_PlayerDust::CEffect_Point_PlayerDust(const CEffect_Point_PlayerDust & rhs)
	:CGameObject(rhs)
{
}

HRESULT CEffect_Point_PlayerDust::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Point_PlayerDust::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GameObjectDesc));

	GameObjectDesc.TransformDesc.fSpeedPerSec = 5.f;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(CGameObject::Initialize(&GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	m_iTextureX = 128 / 64;
	m_iTextureY = 128 / 64;

	_float fX = m_iTextureIndex % m_iTextureX;
	_float fY = m_iTextureIndex % m_iTextureY;

	m_pTransformCom->Set_Scaled(_float3(1.2f, 1.2f, 1.2f));

	return S_OK;
}

_uint CEffect_Point_PlayerDust::Tick(_double TimeDelta)
{
	m_pVIBufferCom->Tick(TimeDelta);

	m_fFrame += TimeDelta;

	if (m_fFrame > 0.8f)
	{
		m_fFrame = 0.f;
		m_iTextureIndex++;
		m_iTextureIndex %= 4;
	}


	return __super::Tick(TimeDelta);
}

void CEffect_Point_PlayerDust::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);



	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CEffect_Point_PlayerDust::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CEffect_Point_PlayerDust::Shoot_Point()
{
	ENGINE_OPEN;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTargetPos = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vMonsterPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMonsterPos);

	ENGINE_CLOSE;
}

HRESULT CEffect_Point_PlayerDust::SetUp_Components()
{	
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointPlayerDust"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_PlayerDust_Instancing"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_PlayerDust"), TEXT("Com_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_Point_PlayerDust::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	// m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_WorldMatrix);
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", (_uint)m_fFrame)))
		return E_FAIL;

	return S_OK;
}

CEffect_Point_PlayerDust * CEffect_Point_PlayerDust::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CEffect_Point_PlayerDust*		pInstance = new CEffect_Point_PlayerDust(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Created : CEffect_Point_PlayerDust");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Point_PlayerDust::Clone(void * pArg)
{
	CEffect_Point_PlayerDust*		pInstance = new CEffect_Point_PlayerDust(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Point_PlayerDust");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Point_PlayerDust::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
