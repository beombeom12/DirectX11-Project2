#include "..\public\UI.h"

CUI::CUI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CUI::CUI(const CUI & rhs)
	:CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype(OBJECT_ID eID, _uint eUIType)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	m_eType = (UITYPE)eUIType;

	return S_OK;
}

HRESULT CUI::Initialize(void * pArg)
{
	ZeroMemory(&m_UIDesc, sizeof(UIDESC));

	if (nullptr != pArg)
		memcpy(&m_UIDesc, pArg, sizeof(UIDESC));

	if (FAILED(__super::Initialize(&m_UIDesc)))
		return E_FAIL;

	if (FAILED(Setup_ShaderResources()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(m_UIDesc.Size.x, m_UIDesc.Size.y, m_UIDesc.Size.z));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
		XMVectorSet(m_UIDesc.Position.x, m_UIDesc.Position.y, m_UIDesc.Position.z, m_UIDesc.Position.w));

	return S_OK;
}

_uint CUI::Tick(_double TimeDelta)
{
	if (FAILED(Setup_ShaderResources()))
		return 0;

	return __super::Tick(TimeDelta);
}

void CUI::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Setup_ShaderResources(void)
{
	if (nullptr == m_pShader)
		return E_FAIL;

	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH((_float)WINCX, (_float)WINCY, 0.f, 1000.f));

	m_pShader->Set_Matrix("g_matWorld", &(m_pTransformCom->Get_WorldMatrix()));
	m_pShader->Set_Matrix("g_matView", &m_matView);
	m_pShader->Set_Matrix("g_matProj", &m_matProj);
	m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture");

	return S_OK;
}

void CUI::Free(void)
{
	__super::Free();
}
