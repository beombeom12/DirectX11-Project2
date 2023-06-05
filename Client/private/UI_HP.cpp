#include "stdafx.h"
#include "..\public\UI_HP.h"
#include "Player.h"


CUI_HP::CUI_HP(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CUI(pDevice, pContext)
{
}

CUI_HP::CUI_HP(const CUI_HP & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_HP::Initialize_Prototype(OBJECT_ID eID, _uint eUIType)
{
	if (FAILED(__super::Initialize_Prototype(eID, eUIType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_HP::Initialize_Clone(void * pArg)
{
	ZeroMemory(&m_UIDesc, sizeof(CUI::UIDESC));

	if (nullptr != pArg)
		memcpy(&m_UIDesc, pArg, sizeof(CUI::UIDESC));
	else
	{
		m_UIDesc.Size = _float4((_float)250.f, (_float)30.f, 0.f, 1.f);
		m_UIDesc.Position = _float4(-360.f, 338.f, 800.f, 1.f);
		m_UIDesc.TransformDesc.fSpeedPerSec = 0.f;
		m_UIDesc.TransformDesc.fRotationPerSec = 0.f;
		
	}

	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_UIDesc)))
		return E_FAIL;


	ENGINE_OPEN;

	m_pPlayer = (CPlayer*)ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player"));

	ENGINE_CLOSE;
	return S_OK;
}

_uint CUI_HP::Tick(_double TimeDelta)
{
	// m_UIDesc.Size = _float4((_float)280.f, (_float)40.f, 0.f, 1.f);
	// m_UIDesc.Position = _float4(-360.f, 338.f, 800.f, 1.f);
	
	_float fCurrentHP = m_pPlayer->Get_HPAbility();
	_float fMaxHP = m_pPlayer->Get_MaxAbility();
	_float fRateHP = fCurrentHP / fMaxHP;

	m_UIDesc.Size.x = 250.f * fRateHP;
	m_UIDesc.Position.x = -360.f - (250.f * (1.f - fRateHP) * 0.5f);

	m_pTransformCom->Set_Scaled(_float3(m_UIDesc.Size.x, m_UIDesc.Size.y, m_UIDesc.Size.z));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
		XMVectorSet(m_UIDesc.Position.x, m_UIDesc.Position.y, m_UIDesc.Position.z, m_UIDesc.Position.w));
	
	return __super::Tick(TimeDelta);
}

void CUI_HP::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_HP::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_float fCurrentHP = m_pPlayer->Get_HPAbility();
	_float fMaxHP = m_pPlayer->Get_MaxAbility();
	_float fRateHP = fCurrentHP / fMaxHP;

	m_pShader->Set_RawValue("g_vGuage", &fRateHP, sizeof(_float));
	_float fLenght = m_pPlayer->Get_MaxAbility();
	m_pShader->Set_RawValue("g_vHPLenght", &fLenght, sizeof(_float));	
	m_pShader->Begin(0);

	m_pBuffer->Render();

	return S_OK;
}


HRESULT CUI_HP::Setup_Component(void)
{
	ENGINE_OPEN;
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Shader_HP"), TEXT("Com_Shader"),
		(CComponent**)&m_pShader)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_PlayerHP"), TEXT("Com_Texture"),
		(CComponent**)&m_pTexture)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CUI_HP * CUI_HP::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID, UITYPE eType)
{
	CUI_HP* pInstance = new CUI_HP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID, eType)))
	{
		MSG_BOX("Failed to Create : CUI_HP");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CUI_HP::Clone(void * pArg)
{
	CUI_HP* pInstance = new CUI_HP(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Crate Clone : CUI_HP");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CUI_HP::Free(void)
{

	__super::Free();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
}
