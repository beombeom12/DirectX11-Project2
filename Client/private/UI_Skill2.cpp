#pragma once
#include "stdafx.h"
#include "..\public\UI_Skill2.h"


CUI_Skill2::CUI_Skill2(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CUI(pDevice, pContext)
{
}

CUI_Skill2::CUI_Skill2(const CUI_Skill2 & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Skill2::Initialize_Prototype(OBJECT_ID eID, _uint eUIType)
{
	if (FAILED(__super::Initialize_Prototype(eID, eUIType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Skill2::Initialize_Clone(void * pArg)
{
	ZeroMemory(&m_UIDesc, sizeof(CUI::UIDESC));

	if (nullptr != pArg)
		memcpy(&m_UIDesc, pArg, sizeof(CUI::UIDESC));
	else
	{
		m_UIDesc.Size = _float4((_float)150.f, (_float)150.f, 0.f, 1.f);
		m_UIDesc.Position = _float4(-470.f, -260.f, 800.f, 0.99f);
		m_UIDesc.TransformDesc.fSpeedPerSec = 0.f;
		m_UIDesc.TransformDesc.fRotationPerSec = 0.f;
	}

	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_UIDesc)))
		return E_FAIL;

	return S_OK;
}

_uint CUI_Skill2::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CUI_Skill2::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Skill2::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pShader->Begin(0);

	m_pBuffer->Render();

	return S_OK;
}

HRESULT CUI_Skill2::Setup_Component(void)
{
	ENGINE_OPEN;
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Shader_Skill2"), TEXT("Com_Shader"),
		(CComponent**)&m_pShader)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Skill2"), TEXT("Com_Texture"),
		(CComponent**)&m_pTexture)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CUI_Skill2 * CUI_Skill2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID, UITYPE eType)
{
	CUI_Skill2* pInstance = new CUI_Skill2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID, eType)))
	{
		MSG_BOX("Failed to Create : CUI_Skill2");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CUI_Skill2::Clone(void * pArg)
{
	CUI_Skill2* pInstance = new CUI_Skill2(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Crate Clone : CUI_Skill2");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CUI_Skill2::Free(void)
{

	__super::Free();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
}
