#include "stdafx.h"
#include "..\public\RoadCliffRockC.h"



CRoadCliffRockC::CRoadCliffRockC(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CRoadCliffRockC::CRoadCliffRockC(const CRoadCliffRockC & rhs)
	: CInstallation(rhs)
{
}

HRESULT CRoadCliffRockC::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRoadCliffRockC::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;



	return S_OK;
}

_uint CRoadCliffRockC::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CRoadCliffRockC::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CRoadCliffRockC::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRoadCliffRockC::SetUp_Model()
{
	ENGINE_OPEN;
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_RoadCliffRockC"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CRoadCliffRockC * CRoadCliffRockC::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CRoadCliffRockC* pInstance = new CRoadCliffRockC(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CRoadCliffRockC");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CRoadCliffRockC::Clone(void * pArg)
{
	CRoadCliffRockC* pInstance = new CRoadCliffRockC(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : CRoadCliffRockC");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CRoadCliffRockC::Free()
{
	__super::Free();
}
