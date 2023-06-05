#include "stdafx.h"
#include "..\public\RoadCliffRockB.h"


CRoadCliffRockB::CRoadCliffRockB(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CRoadCliffRockB::CRoadCliffRockB(const CRoadCliffRockB & rhs)
	: CInstallation(rhs)
{
}

HRESULT CRoadCliffRockB::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRoadCliffRockB::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;



	return S_OK;
}

_uint CRoadCliffRockB::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CRoadCliffRockB::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CRoadCliffRockB::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRoadCliffRockB::SetUp_Model()
{

	ENGINE_OPEN;
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_RoadCliffRockB"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CRoadCliffRockB * CRoadCliffRockB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CRoadCliffRockB* pInstance = new CRoadCliffRockB(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CRoadCliffRockB");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CRoadCliffRockB::Clone(void * pArg)
{
	CRoadCliffRockB* pInstance = new CRoadCliffRockB(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : CRoadCliffRockB");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CRoadCliffRockB::Free()
{
	__super::Free();
}
