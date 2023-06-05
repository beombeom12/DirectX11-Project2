#include "stdafx.h"
#include "..\public\FWallA.h"



CFWallA::CFWallA(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInstallation(pDevice, pContext)
{
}

CFWallA::CFWallA(const CFWallA & rhs)
	: CInstallation(rhs)
{
}

HRESULT CFWallA::Initialize_Prototype(OBJECT_ID eID)
{

	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;
	return S_OK;
}

HRESULT CFWallA::Initialize(void * pArg)
{


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;



	return S_OK;
}

_uint CFWallA::Tick(_double TimeDelta)
{



	return __super::Tick(TimeDelta);
}

void CFWallA::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CFWallA::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		m_pModelCom->Render(m_pShaderCom, i, "g_BoneMatrices");
	}
	return S_OK;
}

HRESULT CFWallA::SetUp_Model()
{

	ENGINE_OPEN;
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FWalLA"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CFWallA * CFWallA::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CFWallA* pInstance = new CFWallA(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CFWallA");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CFWallA::Clone(void * pArg)
{
	CFWallA* pInstance = new CFWallA(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : CStone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CFWallA::Free()
{
	__super::Free();
}
