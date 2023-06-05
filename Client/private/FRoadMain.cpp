#include "stdafx.h"
#include "..\public\FRoadMain.h"



CFRoadMain::CFRoadMain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInstallation(pDevice, pContext)
{
}

CFRoadMain::CFRoadMain(const CFRoadMain & rhs)
	: CInstallation(rhs)
{
}

HRESULT CFRoadMain::Initialize_Prototype(OBJECT_ID eID)
{

	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;
	return S_OK;
}

HRESULT CFRoadMain::Initialize(void * pArg)
{


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;



	return S_OK;
}

_uint CFRoadMain::Tick(_double TimeDelta)
{



	return __super::Tick(TimeDelta);
}

void CFRoadMain::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CFRoadMain::Render()
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

HRESULT CFRoadMain::SetUp_Model()
{
	ENGINE_OPEN;
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FRoadMain"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CFRoadMain * CFRoadMain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CFRoadMain* pInstance = new CFRoadMain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CFRoadMain");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CFRoadMain::Clone(void * pArg)
{
	CFRoadMain* pInstance = new CFRoadMain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : CFRoadMain");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CFRoadMain::Free()
{
	__super::Free();
}
