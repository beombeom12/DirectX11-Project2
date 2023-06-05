#include "stdafx.h"
#include "..\public\DBarricade.h"
#include "Player.h"

CDBarricade::CDBarricade(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CDBarricade::CDBarricade(const CDBarricade & rhs)
	: CInstallation(rhs)
{
}

HRESULT CDBarricade::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;
	return S_OK;
}

HRESULT CDBarricade::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;

	m_pInteractable = new INTERACTABLE;
	m_pInteractable->fCurrentHP = 25.f;
	m_pInteractable->fMaxHP = 25.f;
	m_pInteractable->fCurrentHP = m_pInteractable->fMaxHP;




	return S_OK;
}

_uint CDBarricade::Tick(_double TimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_tWorldMatrix());


	return __super::Tick(TimeDelta);
}

void CDBarricade::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	if (nullptr != m_pRendererCom)
	{
		if (!m_bDead)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		//	m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);
			if (m_pInteractable->fCurrentHP == 0)
			{
				m_bDead = true;
			}

		}
		if (m_bDead && m_pInteractable->fCurrentHP == 0)
		{
			Kill_Obj();
		}
		
	}

	Collision_Attacked();
}

HRESULT CDBarricade::Render()
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

HRESULT CDBarricade::SetUp_Model()
{
	ENGINE_OPEN;
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DBarricade"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC						ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;
	ENGINE_CLOSE;

	return S_OK;
}

HRESULT CDBarricade::Player_Damaged(_float fDamage)
{

	m_pInteractable->fCurrentHP -= fDamage;
	if (m_pInteractable->fCurrentHP <= 0.f)
		m_pInteractable->fCurrentHP = 0.f;
	return S_OK;
}

_bool CDBarricade::Collision_Attacked()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* TargetPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player")));
	if (TargetPlayer->Get_WeaponCollier()->Get_CheckAble())
	{
		if (m_pColliderCom->Collision(TargetPlayer->Get_WeaponCollier()))
		{
			
			RELEASE_INSTANCE(CGameInstance);
			Player_Damaged(1.f);
			return true;
		}

		RELEASE_INSTANCE(CGameInstance);
		return false;

	}
}

CDBarricade * CDBarricade::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CDBarricade* pInstance = new CDBarricade(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CDBarricade");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CDBarricade::Clone(void * pArg)
{
	CDBarricade* pInstance = new CDBarricade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Clone : CDBarricade");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CDBarricade::Free()
{
	__super::Free();
}
