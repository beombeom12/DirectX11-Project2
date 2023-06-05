#include "stdafx.h"
#include "..\public\LoadingImage.h"


CLoadingImage::CLoadingImage(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CUI(pDevice, pContext)
{
}

CLoadingImage::CLoadingImage(const CLoadingImage & rhs)
	:CUI(rhs)
{
}

HRESULT CLoadingImage::Initialize_Prototype(OBJECT_ID eID, _uint eUIType)
{
	if (FAILED(__super::Initialize_Prototype(eID, eUIType)))
		return E_FAIL;

	return S_OK;

}

HRESULT CLoadingImage::Initialize(void * pArg)
{
	ZeroMemory(&m_UIDesc, sizeof(CUI::UIDESC));

	if (nullptr != pArg)
		memcpy(&m_UIDesc, pArg, sizeof(UIDESC));
	else
	{
		m_UIDesc.Size = _float4((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f);
		m_UIDesc.Position = _float4(0.f, 0.f, 800.f, 1.f);
		m_UIDesc.TransformDesc.fSpeedPerSec = 0.f;
		m_UIDesc.TransformDesc.fRotationPerSec = 0.f;
	}

	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_UIDesc)))
		return E_FAIL;

	return S_OK;
}

_uint CLoadingImage::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CLoadingImage::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CLoadingImage::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pShader->Begin(0);

	m_pBuffer->Render();

	return S_OK;
}

HRESULT CLoadingImage::Setup_Component(void)
{
	ENGINE_OPEN
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Shader_UI_BackGroundImage"), TEXT("Com_Shader"),
		(CComponent**)&m_pShader)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pBuffer)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Texture_LoadingImage"), TEXT("Com_Texture"),
		(CComponent**)&m_pTexture)))
		return E_FAIL;


	ENGINE_CLOSE
		return S_OK;
}

CLoadingImage * CLoadingImage::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID, UITYPE eUIType)
{
	CLoadingImage* pInstance = new CLoadingImage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID, eUIType)))
	{
		MSG_BOX("FAILED to Create : CLoadingImage");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CLoadingImage::Clone(void * pArg)
{
	CLoadingImage* pInstance = new CLoadingImage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("FAILED to Create Clone : CLoadingImage");
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CLoadingImage::Free(void)
{
	__super::Free();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
}
