#include "stdafx.h"
#include "..\public\Navigation_Cube.h"
#include "GameInstance.h"

CNavigation_Cube::CNavigation_Cube(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CNavigation_Cube::CNavigation_Cube(const CNavigation_Cube & rhs)
	:CGameObject(rhs)
{
}

HRESULT CNavigation_Cube::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation_Cube::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Scaled(_float3(1.f, 1.0, 1.f));

	return S_OK;
}

_uint CNavigation_Cube::Tick(_double TimeDelta)
{
	if (nullptr != m_pCalculatorCom)
		m_pCalculatorCom->Calculate_MousePos(g_hWnd);

	
	return __super::Tick(TimeDelta);
}

void CNavigation_Cube::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}
}

HRESULT CNavigation_Cube::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CNavigation_Cube::SetUp_Component(void)
{

	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Square"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Square"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Calculator"), TEXT("Com_Caluator"),
		(CComponent**)&m_pCalculatorCom)))
		return E_FAIL;
	return S_OK;
}

HRESULT CNavigation_Cube::SetUp_ShaderResource(void)
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_matWorld")))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_matView", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_matProj", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	/* For.Lights */
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_bool CNavigation_Cube::Peeking()
{
	return m_pCalculatorCom->Ray_CubePeeking(m_pVIBufferCom, m_pTransformCom);

}

CNavigation_Cube * CNavigation_Cube::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{

	CNavigation_Cube* pInstance = new CNavigation_Cube(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : Navigation_Cube");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CGameObject * CNavigation_Cube::Clone(void * pArg)
{
	CNavigation_Cube* pInstance = new CNavigation_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : Navigation_Cube");
		Safe_Release(pInstance);
	}


	return pInstance;
}

void CNavigation_Cube::Free()
{
	__super::Free();

	Safe_Release(m_pCalculatorCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
