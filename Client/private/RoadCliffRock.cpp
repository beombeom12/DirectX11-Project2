#include "stdafx.h"
#include "..\public\RoadCliffRock.h"


CRoadCliffRock::CRoadCliffRock(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CRoadCliffRock::CRoadCliffRock(const CRoadCliffRock & rhs)
	: CInstallation(rhs)
{
}

HRESULT CRoadCliffRock::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRoadCliffRock::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;



	return S_OK;
}

_uint CRoadCliffRock::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CRoadCliffRock::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);


}

HRESULT CRoadCliffRock::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRoadCliffRock::SetUp_Model()
{
	ENGINE_OPEN;
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_RoadCliffRock"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

CRoadCliffRock * CRoadCliffRock::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CRoadCliffRock* pInstance = new CRoadCliffRock(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CRoadCliffRock");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CRoadCliffRock::Clone(void * pArg)
{
	CRoadCliffRock* pInstance = new CRoadCliffRock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : CRoadCliffRock");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CRoadCliffRock::Free()
{
	__super::Free();
}
