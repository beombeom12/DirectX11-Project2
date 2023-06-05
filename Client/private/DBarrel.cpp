#include "stdafx.h"
#include "..\public\DBarrel.h"
#include "Player.h"
#include "Effect.h"
CDBarrel::CDBarrel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CDBarrel::CDBarrel(const CDBarrel & rhs)
	: CInstallation(rhs)
{
}

HRESULT CDBarrel::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDBarrel::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;

	m_pInteractable = new INTERACTABLE;
	m_pInteractable->fCurrentHP = 1.f;
	m_pInteractable->fMaxHP = 1.f;
	m_pInteractable->fCurrentHP = m_pInteractable->fMaxHP;

	
	return S_OK;
}

_uint CDBarrel::Tick(_double TimeDelta)
{

	
	m_pColliderCom->Update(m_pTransformCom->Get_tWorldMatrix());

	
	return __super::Tick(TimeDelta);


}

void CDBarrel::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect*	pEffectTarget = static_cast<CEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect")));

	if (nullptr != m_pRendererCom)
	{
		if (!m_bDead)
		{ 
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			//m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);
			if (m_pInteractable->fCurrentHP == 0)
			{
				 m_bDead = true;
			}
		}
		if (m_bDead && m_pInteractable->fCurrentHP == 0)
		{
			Kill_Obj();
			pEffectTarget->Late_Tick(TimeDelta);
			
		}
	}
	Collision_Attacked();

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CDBarrel::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect*	pEffectTarget = static_cast<CEffect*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect")));

	pEffectTarget->Render();


	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		m_pModelCom->Render(m_pShaderCom, i, "g_BoneMatrices");
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;

}

HRESULT CDBarrel::SetUp_Model()
{

	ENGINE_OPEN;

	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DBarrel"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	CCollider::COLLIDERDESC						ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

_bool CDBarrel::Collision_Attacked()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* TargetPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player")));
	if (TargetPlayer->Get_WeaponCollier()->Get_CheckAble())
	{
		if (m_pColliderCom->Collision(TargetPlayer->Get_WeaponCollier()))
		{
			RELEASE_INSTANCE(CGameInstance);
			Player_ToDamaged(1.f);
			return true;
		}

		RELEASE_INSTANCE(CGameInstance);
		return false;
	
	}
	else
	{
		RELEASE_INSTANCE(CGameInstance);
		return false; 
	}
}

HRESULT CDBarrel::Player_ToDamaged(_float fDamage)
{


	m_pInteractable->fCurrentHP -= fDamage;
	if (m_pInteractable->fCurrentHP <= 0.f)
		m_pInteractable->fCurrentHP = 0.f;

	

	return S_OK;
}

CDBarrel * CDBarrel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CDBarrel* pInstance = new CDBarrel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CDBarrel");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CDBarrel::Clone(void * pArg)
{
	CDBarrel* pInstance = new CDBarrel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : CDBarrel");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CDBarrel::Free()
{
	__super::Free();

	Safe_Delete<INTERACTABLE*>(m_pInteractable);
}
