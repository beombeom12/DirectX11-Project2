#include "stdafx.h"
#include "..\public\HellHound.h"

#include "HellHound_State.h"
#include "GameInstance.h"
#include "Model.h"
#include "Bone.h"
#include "Player.h"
#include "Effect.h"
#include "Effect_Point_Instancing.h"
#include "Effect_PlayerDust.h"
CHellHound::CHellHound(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CHellHound::CHellHound(const CHellHound & rhs)
	:CGameObject(rhs)
{
}

HRESULT CHellHound::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;
	return S_OK;
}

HRESULT CHellHound::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECTDESC			GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof GameObjectDesc);

	GameObjectDesc.TransformDesc.fSpeedPerSec = 4.0f;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	if (FAILED(SetUp_State()))
		return E_FAIL;

	m_pModelCom->Set_AnimIndex(9);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(20.f, 0.f, 150.f, 1.f));


	m_pMonsterAbility = new MONSTERABILITY;
	m_pMonsterAbility->fCurrentHP = 100.f;
	m_pMonsterAbility->fMaxHP = 100.f;
	m_pMonsterAbility->fCurrentHP = m_pMonsterAbility->fMaxHP;

	ENGINE_OPEN;
	//m_pEffect =	(CEffect*)ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect"));

	//m_pPointeffect = (CEffect_Point_Instancing*)ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_Point_Instancing"));

	//m_pDustEffect = (CEffect_PlayerDust*)ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Effect_Point_Instancing"));
	ENGINE_CLOSE;
	return S_OK;
}

_uint CHellHound::Tick(_double TimeDelta)
{

	if (nullptr != m_pStateCom)
		m_pStateCom->Tick(TimeDelta);

	m_fAnimationTime += TimeDelta;

	m_pModelCom->Play_Animation(TimeDelta, m_fLerpTime, m_fHellHoundTime);
	//for (_uint i = 0; i < m_pMonsterParts.size(); ++i)
	//{
	//	m_pMonsterParts[i]->Tick(TimeDelta);
	//}
	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pHit[HELLHOUND_HIT]->Update(m_pTransformCom->Get_tWorldMatrix());

	return __super::Tick(TimeDelta);
}

void CHellHound::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);


	//for (_uint i = 0; i < m_pMonsterParts.size(); ++i)
	//{
	//	m_pMonsterParts[i]->Late_Tick(TimeDelta);
	//}

	if (nullptr != m_pRendererCom)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		for (auto& pCollider : m_pColliderCom)
		{
		m_pRendererCom->Add_DebugRenderGroup(pCollider);
		}
		for (auto& pHit : m_pHit)
		{
		m_pRendererCom->Add_DebugRenderGroup(pHit);
		}
		for (auto& pAttack : m_pAttack)
		{
			m_pRendererCom->Add_DebugRenderGroup(pAttack);
		}

		//m_pRendererCom->Add_DebugRenderGroup(m_pNavigationCom);
	}

	Set_Collider();
	//SetOn_Cell();

}

HRESULT CHellHound::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		m_pModelCom->Render(m_pShaderCom, i, "g_BoneMatrices");
	}

#ifdef _DEBUG


#endif

	return S_OK;
}

HRESULT CHellHound::SetUp_Components()
{
	// For.Com_Renderer
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com.Model
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_HellHound"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	//STATE
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pStateCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;


	// For.Com_SPHERE 
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(20.0f, 1.9f, 20.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;



	//Hit 
	ColliderDesc.vSize = _float3(7.0f, 1.9f, 7.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.2f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_GOBLIN_HIT"),
		(CComponent**)&m_pHit[HELLHOUND_HIT], &ColliderDesc)))
		return E_FAIL;



	ColliderDesc.vSize = _float3(7.0f, 1.9f, 7.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_GOBLIN_Sword"),
		(CComponent**)&m_pAttack[HELLHOUND_ATTACK_A], &ColliderDesc)))
		return E_FAIL;


	ColliderDesc.vSize = _float3(7.0f, 1.9f, 7.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_GOBLIN_Sword2"),
		(CComponent**)&m_pAttack[HELLHOUND_ATTACK_B], &ColliderDesc)))
		return E_FAIL;



	// For.Com_SPHERE_TARGET
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(50.f, 1.f, 50.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y /** 1.f*/, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"), TEXT("Com_SPHERE_TARGET"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE_TARGET], &ColliderDesc)))
		return E_FAIL;

	//Navigation
	//CNavigation::NAVIDESC			NaviDesc;
	//ZeroMemory(&NaviDesc, sizeof(CNavigation::tagNavigationDesc));

	//NaviDesc.iCurrentIndex = 0;

	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"),
	//	(CComponent**)&m_pNavigationCom, &NaviDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CHellHound::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	// For.Light
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CHellHound::SetUp_State()
{
	if (nullptr == m_pStateCom)
		return E_FAIL;

	m_pHellHoundState = CHellHound_State::Create(this);
	if (nullptr == m_pHellHoundState)
		return E_FAIL;

	return S_OK;
}

HRESULT CHellHound::SetOn_Cell()
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);


	_float fHeight = m_pNavigationCom->HeightOnTerrain(vPos);

	m_pTransformCom->Set_Height(fHeight);

	return S_OK;
}

_bool CHellHound::Collision_Player()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider*			pTargetCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_SPHERE"));


	if (m_pColliderCom[COLLTYPE_SPHERE]->Collision(pTargetCollider))
	{
		RELEASE_INSTANCE(CGameInstance);
		return true;
	}
	else
	{
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}

}

_bool CHellHound::Collision_ChaseRange()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CCollider*			pTargetColliderTarget = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_SPHERE_TARGET"));

	if (m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Collision(pTargetColliderTarget))
	{
		RELEASE_INSTANCE(CGameInstance);
		return true;
	}
	else
	{
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}
}

_bool CHellHound::Collision_AttackedRange()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* TargetPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player")));


	if (TargetPlayer->Get_WeaponCollier()->Get_CheckAble())
	{
		_bool IsColl = m_pHit[HELLHOUND_HIT]->Collision(TargetPlayer->Get_WeaponCollier());
	
			if(IsColl == true)
			{
				if (m_TakeDamage == false)
				{
				
					Player_ToDamaged(15.f);
					m_TakeDamage = true;
				}
				return true;
			}
			else
			{
				m_TakeDamage = false;
				return false;
			}
	
	}
	else
	{
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}

}

_bool CHellHound::isDead()
{

	if (m_pMonsterAbility->fCurrentHP == 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void CHellHound::Player_ToDamaged(_float fDamage)
{
	m_pMonsterAbility->fCurrentHP -= fDamage;
	if (m_pMonsterAbility->fCurrentHP <= 0.f)
		m_pMonsterAbility->fCurrentHP = 0.f;
}

void CHellHound::Set_Collider(void)
{

	_matrix HeadMatrix = m_pModelCom->Get_BonePtr("Bone_Tidehunter_Hand_R")->Get_OffsetMatrix()
		* m_pModelCom->Get_BonePtr("Bone_Tidehunter_Hand_R")->Get_CombindMatrix()
		* m_pModelCom->Get_PivotMatrix();
	//Bone_TideHunter_Head

	_matrix LeftMatrix = m_pModelCom->Get_BonePtr("Bone_Tidehunter_Hand_L")->Get_OffsetMatrix()
		* m_pModelCom->Get_BonePtr("Bone_Tidehunter_Hand_L")->Get_CombindMatrix()
		* m_pModelCom->Get_PivotMatrix();


	HeadMatrix.r[0] = XMVector3Normalize(HeadMatrix.r[0]);
	HeadMatrix.r[1] = XMVector3Normalize(HeadMatrix.r[1]);
	HeadMatrix.r[2] = XMVector3Normalize(HeadMatrix.r[2]);

	LeftMatrix.r[0] = XMVector3Normalize(LeftMatrix.r[0]);
	LeftMatrix.r[1] = XMVector3Normalize(LeftMatrix.r[1]);
	LeftMatrix.r[2] = XMVector3Normalize(LeftMatrix.r[2]);
	HeadMatrix = HeadMatrix * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
	LeftMatrix = LeftMatrix * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
	m_pAttack[HELLHOUND_ATTACK_A]->Update(HeadMatrix);
	m_pAttack[HELLHOUND_ATTACK_B]->Update(LeftMatrix);
}

CHellHound * CHellHound::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CHellHound*		pInstance = new CHellHound(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Created : CHellHound");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CHellHound::Clone(void * pArg)
{
	CHellHound*		pInstance = new CHellHound(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Cloned : CHellHound");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHellHound::Free()
{

	__super::Free();
	//for (auto& pPart : m_pMonsterParts)
	//	Safe_Release(pPart);
/*
	m_pMonsterParts.clear();
*/	Safe_Release(m_pStateCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pHellHoundState);
}
