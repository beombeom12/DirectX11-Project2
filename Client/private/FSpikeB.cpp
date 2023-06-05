#include "stdafx.h"
#include "..\public\FSpikeB.h"


CFSpikeB::CFSpikeB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInstallation(pDevice, pContext)
{
}

CFSpikeB::CFSpikeB(const CFSpikeB & rhs)
	: CInstallation(rhs)
{
}

HRESULT CFSpikeB::Initialize_Prototype(OBJECT_ID eID)
{

	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;
	return S_OK;
}

HRESULT CFSpikeB::Initialize(void * pArg)
{


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;



	return S_OK;
}

_uint CFSpikeB::Tick(_double TimeDelta)
{



	return __super::Tick(TimeDelta);
}

void CFSpikeB::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CFSpikeB::Render()
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

HRESULT CFSpikeB::SetUp_Model()
{

	ENGINE_OPEN;
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FSpikeB"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CFSpikeB * CFSpikeB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CFSpikeB* pInstance = new CFSpikeB(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CFSpikeB");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CFSpikeB::Clone(void * pArg)
{
	CFSpikeB* pInstance = new CFSpikeB(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : CFSpikeB");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CFSpikeB::Free()
{
	__super::Free();
}
