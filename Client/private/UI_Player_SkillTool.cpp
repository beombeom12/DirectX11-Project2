#include "stdafx.h"
#include "..\public\UI_Player_SkillTool.h"

CUI_Player_SkillTool::CUI_Player_SkillTool(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CUI(pDevice, pContext)
{
}

CUI_Player_SkillTool::CUI_Player_SkillTool(const CUI_Player_SkillTool & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_SkillTool::Initialize_Prototype(OBJECT_ID eID, _uint eUIType)
{
	if (FAILED(__super::Initialize_Prototype(eID, eUIType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Player_SkillTool::Initialize_Clone(void * pArg)
{
	ZeroMemory(&m_UIDesc, sizeof(CUI::UIDESC));

	if (nullptr != pArg)
		memcpy(&m_UIDesc, pArg, sizeof(CUI::UIDESC));
	else
	{
		m_UIDesc.Size = _float4((_float)300.f, (_float)150.f, 0.f, 1.f);
		m_UIDesc.Position = _float4(-390.f, -280.f, 800.f, 0.98f);
		m_UIDesc.TransformDesc.fSpeedPerSec = 0.f;
		m_UIDesc.TransformDesc.fRotationPerSec = 0.f;
	}

	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_UIDesc)))
		return E_FAIL;

	return S_OK;
}

_uint CUI_Player_SkillTool::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CUI_Player_SkillTool::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Player_SkillTool::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pShader->Begin(0);

	m_pBuffer->Render();

	return S_OK;
}

HRESULT CUI_Player_SkillTool::Setup_Component(void)
{
	ENGINE_OPEN;
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Shader_SkillTool"), TEXT("Com_Shader"),
		(CComponent**)&m_pShader)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_SkillTool"), TEXT("Com_Texture"),
		(CComponent**)&m_pTexture)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CUI_Player_SkillTool * CUI_Player_SkillTool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID, UITYPE eType)
{
	CUI_Player_SkillTool* pInstance = new CUI_Player_SkillTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID, eType)))
	{
		MSG_BOX("Failed to Create : CUI_Player_SkillTool");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CUI_Player_SkillTool::Clone(void * pArg)
{
	CUI_Player_SkillTool* pInstance = new CUI_Player_SkillTool(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Crate Clone : CUI_Player_SkillTool");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CUI_Player_SkillTool::Free(void)
{

	__super::Free();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
}
