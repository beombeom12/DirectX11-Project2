#include "stdafx.h"
#include "..\public\RoadCliffRockA.h"


CRoadCliffRockA::CRoadCliffRockA(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CRoadCliffRockA::CRoadCliffRockA(const CRoadCliffRockA & rhs)
	: CInstallation(rhs)
{
}

HRESULT CRoadCliffRockA::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRoadCliffRockA::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;



	return S_OK;
}

_uint CRoadCliffRockA::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CRoadCliffRockA::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);


}

HRESULT CRoadCliffRockA::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRoadCliffRockA::SetUp_Model()
{

	ENGINE_OPEN;
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_RoadCliffRockA"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CRoadCliffRockA * CRoadCliffRockA::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CRoadCliffRockA* pInstance = new CRoadCliffRockA(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CRoadCliffRockA");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CRoadCliffRockA::Clone(void * pArg)
{
	CRoadCliffRockA* pInstance = new CRoadCliffRockA(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : CRoadCliffRockA");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CRoadCliffRockA::Free()
{
	__super::Free();
}
