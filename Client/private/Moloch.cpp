#include "stdafx.h"
#include "..\public\Moloch.h"
#include "GameInstance.h"
#include "Bone.h"
#include "Moloch_Sword.h"
#include "Moloch_State.h"
#include "Player.h"
CMoloch::CMoloch(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CMoloch::CMoloch(const CMoloch & rhs)
	:CGameObject(rhs)
{
}

HRESULT CMoloch::Initialize_Prototype(OBJECT_ID eID)
{

	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;

}

HRESULT CMoloch::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECTDESC			GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof GameObjectDesc);

	GameObjectDesc.TransformDesc.fSpeedPerSec = 4.0f;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if(FAILED(Ready_Parts()))
		return E_FAIL;
	
	if (FAILED(SetUp_State()))
		return E_FAIL;

	m_pModelCom->Set_AnimIndex(17);




	m_pMonsterAbility = new MONSTERABILITY;
	m_pMonsterAbility->fCurrentHP = 100.f;
	m_pMonsterAbility->fMaxHP = 100.f;
	m_pMonsterAbility->fCurrentHP = m_pMonsterAbility->fMaxHP;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(699.879f, 64.070f, 382.716f, 1.f));
	ENGINE_OPEN;

	m_pMoStone = (CMo_Stone*)ENGINE->Clone_NoGameObject(LEVEL_GAMEPLAY,TEXT("Prototype_GameObject_StoneMo"));
	ENGINE_CLOSE;
	return S_OK;
}

_uint CMoloch::Tick(_double TimeDelta)
{

	
	ENGINE_OPEN;

	m_pMonsterHPUI = (CUI_MonsterHP*)ENGINE->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_UI_MonsterHP"));

	ENGINE_CLOSE;
	if (nullptr != m_pStateCom)
		m_pStateCom->Tick(TimeDelta);
	
	m_pModelCom->Play_Animation(TimeDelta,m_fLerpTime, m_fMolochPlayTime);

	for (_uint i = 0; i < m_pMonsterParts.size(); ++i)
	{
		m_pMonsterParts[i]->Tick(TimeDelta);
	}
	//CHASE, ATTACK
	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());
	//Hit
	m_pHit[MOLOCH_HIT]->Update(m_pTransformCom->Get_tWorldMatrix());
	//Skill
	m_pAttack[MOLOCH_SKILL1]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pAttack[MOLOCH_SKILL2]->Update(m_pTransformCom->Get_tWorldMatrix());

	if (m_pMoStone != nullptr)
		m_pMoStone->Tick(TimeDelta);

	if (nullptr != m_pMonsterHPUI)
		m_pMonsterHPUI->Tick(TimeDelta);


	return __super::Tick(TimeDelta);
}

void CMoloch::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	for (_uint i = 0; i < m_pMonsterParts.size(); ++i)
	{
		m_pMonsterParts[i]->Late_Tick(TimeDelta);
	}
	if (nullptr != m_pRendererCom)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	/*	for (auto& pCollider : m_pColliderCom)
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
		}*/
		//m_pRendererCom->Add_DebugRenderGroup(m_pNavigationCom);
	}



	//SetOn_Cell();

	if (m_pMoStone != nullptr)
		m_pMoStone->Late_Tick(TimeDelta);
	if (m_pMonsterHPUI != nullptr)
		m_pMonsterHPUI->Late_Tick(TimeDelta);

}

HRESULT CMoloch::Render()
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
	return S_OK;
}

HRESULT CMoloch::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Moloch"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	//STATE
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pStateCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;



	//Base Attack
	// For.Com_SPHERE 
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(20.0f, 1.9f, 20.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;



	//Hit 
	ColliderDesc.vSize = _float3(7.0f, 2.9f, 7.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.9f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_MOLOCH_HIT"),
		(CComponent**)&m_pHit[MOLOCH_HIT], &ColliderDesc)))
		return E_FAIL;



	//Attack (Skill)
	ColliderDesc.vSize = _float3(45.0f, 1.9f, 45.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_MOLOCH_Skill1"),
		(CComponent**)&m_pAttack[MOLOCH_SKILL1], &ColliderDesc)))
		return E_FAIL;
	
	//Attack2(SKill2)
	ColliderDesc.vSize = _float3(70.0f, 1.9f, 70.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_MOLOCH_Skill2"),
		(CComponent**)&m_pAttack[MOLOCH_SKILL2], &ColliderDesc)))
		return E_FAIL;



	// For.Com_SPHERE_TARGET
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(73.0f, 1.f, 73.0f);
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

HRESULT CMoloch::SetUp_ShaderResources()
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

HRESULT CMoloch::SetUp_State()
{
	if (nullptr == m_pStateCom)
		return E_FAIL;

	m_pMolochState = CMoloch_State::Create(this);
	if (nullptr == m_pMolochState)
		return E_FAIL;

	return S_OK;
}

HRESULT CMoloch::Ready_Parts()
{
	//Bone_Weapon_R_HandLink


	CGameObject* pPartObject = nullptr;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CMoloch_Sword::WEAPONDESC			WeaponDesc;
	ZeroMemory(&WeaponDesc, sizeof(CMoloch_Sword::WEAPONDESC));

	WeaponDesc.PivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pSocket = m_pModelCom->Get_BonePtr("Bone_Weapon_R_HandLink");
	WeaponDesc.pTargetTransform = m_pTransformCom;
	Safe_AddRef(WeaponDesc.pSocket);
	Safe_AddRef(m_pTransformCom);

	pPartObject = pGameInstance->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MolochSword"), &WeaponDesc);
	if (nullptr == pPartObject)
		return E_FAIL;

	m_pMonsterParts.push_back(pPartObject);

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMoloch::SetOn_Cell()
{

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);


	_float fHeight = m_pNavigationCom->HeightOnTerrain(vPos);


	m_pTransformCom->Set_Height(fHeight);
	
	
	return S_OK;
}





_bool CMoloch::Collision_Player()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider*		pTargetCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_SPHERE"));

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

_bool CMoloch::Collision_ChaseRange()
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

_bool CMoloch::Collision_AttackedRange()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CPlayer* TargetPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player")));

	if (TargetPlayer->Get_WeaponCollier()->Get_CheckAble())
	{
		_bool IsColl = m_pHit[MOLOCH_HIT]->Collision(TargetPlayer->Get_WeaponCollier());

		if (IsColl == true)
		{
			if (m_bTakeDamage == false)
			{
				Player_ToDamaged(1.f);
				m_bTakeDamage = true;
			}

			return true;
		}
		else
		{
			m_bTakeDamage = false;
			return false;

		}

	}
	else
	{
		RELEASE_INSTANCE(CGameInstance);
		return false; 
	}




}

_bool CMoloch::Collision_Skill1Range()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CCollider*			pTargetColliderTarget = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_PLAYER_ATTACKED_SKILL"));

	if (m_pAttack[MOLOCH_SKILL1]->Collision(pTargetColliderTarget))
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

_bool CMoloch::Collision_Skill2Range()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_PLAYER_ATTACKED_SKILL2"));


	if (m_pAttack[MOLOCH_SKILL2]->Collision(pTargetCollider))
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

_bool CMoloch::isDead()
{
	if (m_pMonsterAbility->fCurrentHP == 0.f)
	{
		m_pMonsterHPUI->Off_Switch();
		return true;
	}
	else
	{
		return false;

	}
}

CCollider * CMoloch::Get_SwordCollider()
{
	return static_cast<CMoloch_Sword*>(m_pMonsterParts.front())->Get_ColliderWeapon();
}

CCollider * CMoloch::Get_StoneCollider()
{
	return 	m_pMoStone->Get_StoneCollider();
}



void CMoloch::Player_ToDamaged(_float fDamage)
{




	m_pMonsterAbility->fCurrentHP -= fDamage;
	
	if (m_pMonsterAbility->fCurrentHP -= fDamage)
	{
		m_pMonsterHPUI->On_Switch();
	}

	
	if (m_pMonsterAbility->fCurrentHP <= 0.f)
		m_pMonsterAbility->fCurrentHP = 0.f;

}


CMoloch * CMoloch::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CMoloch*	pInstance = new CMoloch(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to created : CMoloch");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMoloch::Clone(void * pArg)
{
	CMoloch* pInstance = new CMoloch(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to created clone : CMoloch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoloch::Free()
{
	__super::Free();
	
	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);
	
	for (auto& pPart : m_pMonsterParts)
		Safe_Release(pPart);
	m_pMonsterParts.clear();

	Safe_Release(m_pStateCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMolochState);
	Safe_Release(m_pMonsterHPUI);
	Safe_Release(m_pMoStone);
}

