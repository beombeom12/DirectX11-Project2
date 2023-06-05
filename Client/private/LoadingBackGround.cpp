#include "stdafx.h"
#include "..\public\LoadingBackGround.h"

CLoadingBackGround::CLoadingBackGround(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CUI(pDevice, pContext)
{
}

CLoadingBackGround::CLoadingBackGround(const CLoadingBackGround & rhs)
	:CUI(rhs)
{
}

HRESULT CLoadingBackGround::Initialize_Prototype(OBJECT_ID eID, _uint eUIType)
{
	if (FAILED(__super::Initialize_Prototype(eID, eUIType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingBackGround::Initialize(void * pArg)
{
	ZeroMemory(&m_UIDesc, sizeof(CUI::UIDESC));

	if (nullptr != pArg)
		memcpy(&m_UIDesc, pArg, sizeof(CUI::UIDESC));
	else
	{
		m_UIDesc.Size = _float4((_float)10.f, (_float)10, 0.f, 1.f);
		m_UIDesc.Position = _float4(0.f, 0.f, 800.f, 0.99f);
		m_UIDesc.TransformDesc.fSpeedPerSec = 0.f;
		m_UIDesc.TransformDesc.fRotationPerSec = 0.f;
	}

	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_UIDesc)))
		return E_FAIL;

	return S_OK;
}

_uint CLoadingBackGround::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CLoadingBackGround::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CLoadingBackGround::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pShader->Begin(0);

	m_pBuffer->Render();

	return S_OK;
}

HRESULT CLoadingBackGround::Setup_Component(void)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Shader_UI_BackGround"), TEXT("Com_Shader"),
		(CComponent**)&m_pShader)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Texture_Loading"), TEXT("Com_Texture"),
		(CComponent**)&m_pTexture)))
		return E_FAIL;
	return S_OK;
}


CLoadingBackGround * CLoadingBackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID, UITYPE eUIType)
{
	CLoadingBackGround* pInstance = new CLoadingBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID, eUIType)))
	{
		MSG_BOX("FAILED to Create : CLoadingBackGround");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CLoadingBackGround::Clone(void * pArg)
{
	CLoadingBackGround* pInstance = new CLoadingBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("FAILED to Create Clone : CLoadingBackGround");
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CLoadingBackGround::Free(void)
{
	__super::Free();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
}
