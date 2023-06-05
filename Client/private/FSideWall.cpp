#include "stdafx.h"
#include "..\public\FSideWall.h"

CFSideWall::CFSideWall(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CInstallation(pDevice, pContext)
{
}

CFSideWall::CFSideWall(const CFSideWall & rhs)
	: CInstallation(rhs)
{
}

HRESULT CFSideWall::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype((eID))))
		return E_FAIL;
	return S_OK;
}

HRESULT CFSideWall::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;



	return S_OK;
}

_uint CFSideWall::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CFSideWall::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CFSideWall::Render()
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

HRESULT CFSideWall::SetUp_Model()
{
	ENGINE_OPEN;
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FSideWall"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CFSideWall * CFSideWall::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CFSideWall* pInstance = new CFSideWall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : FSideWall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFSideWall::Clone(void * pArg)
{
	CFSideWall* pInstance = new CFSideWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : FSideWall");
		Safe_Release(pInstance);
	}



	return pInstance;
}

void CFSideWall::Free()
{
	__super::Free();
}
