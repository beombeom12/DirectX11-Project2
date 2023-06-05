#include "stdafx.h"
#include "..\public\Steamroller.h"
#include "Steamroller_State.h"
#include "Model.h"
#include "Bone.h"
#include "Player.h"
CSteamroller::CSteamroller(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CSteamroller::CSteamroller(const CSteamroller & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSteamroller::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSteamroller::Initialize(void * pArg)
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


	m_pModelCom->Set_AnimIndex(11);



	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(40.f, 0.f, 150.f, 1.f));


	m_pMonsterAbility = new MONSTERABILITY;

	m_pMonsterAbility->fCurrentHP = 100.f;
	m_pMonsterAbility->fMaxHP = 100.f;
	m_pMonsterAbility->fCurrentHP = m_pMonsterAbility->fMaxHP;




	return S_OK;
}

_uint CSteamroller::Tick(_double TimeDelta)
{
	if (nullptr != m_pStateCom)
		m_pStateCom->Tick(TimeDelta);



	m_fAnimationTime += TimeDelta;

	m_pModelCom->Play_Animation(TimeDelta, m_fLerpTime, m_fSteamAnimationTime);

	for (_uint i = 0; i < m_pMonsterParts.size(); ++i)
	{
		m_pMonsterParts[i]->Tick(TimeDelta);
	}


	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());



	m_pHit[COLLIDER_HIT]->Update(m_pTransformCom->Get_tWorldMatrix());

	m_pHit[COLLIDER_SWORD]->Update(m_pTransformCom->Get_tWorldMatrix());

	m_pHit[COLLIER_SKILL1]->Update(m_pTransformCom->Get_tWorldMatrix());


	return __super::Tick(TimeDelta);
}

void CSteamroller::Late_Tick(_double TimeDelta)
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
		}*/


		//m_pRendererCom->Add_DebugRenderGroup(m_pNavigationCom);
	}


	Set_Collider();
	//Set_OnCell();
}

HRESULT CSteamroller::Render()
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

HRESULT CSteamroller::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Steamroller"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	//STATE
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pStateCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;

	// For.Com_SPHERE 
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(30.0f, 1.9f, 30.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;


	//HIT
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(7.0f, 1.9f, 7.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.2f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE_HIT"),
		(CComponent**)&m_pHit[COLLIDER_HIT], &ColliderDesc)))
		return E_FAIL;

	//SWORD
	ColliderDesc.vSize = _float3(0.0f, 1.9f, 7.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE_SWORD"),
		(CComponent**)&m_pHit[COLLIDER_SWORD], &ColliderDesc)))
		return E_FAIL;



	//Skill 
	ColliderDesc.vSize = _float3(35.f, 1.9f, 35.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE_SKILL"),
		(CComponent**)&m_pHit[COLLIER_SKILL1], &ColliderDesc)))
		return E_FAIL;


	// For.Com_SPHERE_TARGET
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(40.f, 1.f, 40.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y /** 1.f*/, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"), TEXT("Com_SPHERE_TARGET"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE_TARGET], &ColliderDesc)))
		return E_FAIL;

	////Navigation
	//CNavigation::NAVIDESC			NaviDesc;
	//ZeroMemory(&NaviDesc, sizeof(CNavigation::tagNavigationDesc));

	//NaviDesc.iCurrentIndex = 0;

	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"),
	//	(CComponent**)&m_pNavigationCom, &NaviDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CSteamroller::SetUp_ShaderResources()
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

HRESULT CSteamroller::SetUp_State()
{
	if (nullptr == m_pStateCom)
		return E_FAIL;

	m_pSteamState = CSteamroller_State::Create(this);
	if (nullptr == m_pSteamState)
		return E_FAIL;

	return S_OK;
}

HRESULT CSteamroller::Set_OnCell()
{

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);


	_float fHeight = m_pNavigationCom->HeightOnTerrain(vPos);

	m_pTransformCom->Set_Height(fHeight);

	return S_OK;
}

_bool CSteamroller::Collision_Player()
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

_bool CSteamroller::Collision_ChaseRange()
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

_bool CSteamroller::Collision_AttackedRange()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* TargetPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player")));


	if (TargetPlayer->Get_WeaponCollier()->Get_CheckAble())
	{
		_bool IsColl = m_pHit[COLLIDER_HIT]->Collision(TargetPlayer->Get_WeaponCollier());

		if (IsColl == true)
		{
			if (m_TakeDamage == false)
			{

				Player_To_Damaged(15.f);
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

_bool CSteamroller::Collision_SkillRanage()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CCollider*			pTargetColliderTarget = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_SPHERE"));

	if (m_pHit[COLLIER_SKILL1]->Collision(pTargetColliderTarget))
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

_bool CSteamroller::isDead()
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

void  CSteamroller::Player_To_Damaged(_float fDamage)
{
	m_pMonsterAbility->fCurrentHP -= fDamage;
	if (m_pMonsterAbility->fCurrentHP <= 0.f)
		m_pMonsterAbility->fCurrentHP = 0.f;

}

void CSteamroller::Set_Collider()
{
	_matrix   RightMatrix =
		m_pModelCom->Get_BonePtr("Bone_Roller_Weapon_Steamroller")->Get_OffsetMatrix()
		* m_pModelCom->Get_BonePtr("Bone_Roller_Weapon_Steamroller")->Get_CombindMatrix()
		* m_pModelCom->Get_PivotMatrix();



	RightMatrix.r[0] = XMVector3Normalize(RightMatrix.r[0]);
	RightMatrix.r[1] = XMVector3Normalize(RightMatrix.r[1]);
	RightMatrix.r[2] = XMVector3Normalize(RightMatrix.r[2]);


	RightMatrix = RightMatrix * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());


	m_pHit[1]->Update(RightMatrix);

}

CSteamroller * CSteamroller::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{

	CSteamroller*		pInstance = new CSteamroller(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Created : CSteamroller");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSteamroller::Clone(void * pArg)
{
	CSteamroller*		pInstance = new CSteamroller(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Cloned : CSteamroller");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSteamroller::Free()
{

	__super::Free();

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	for (_uint i = 0; i < COLLIDER_END; ++i)
		Safe_Release(m_pHit[i]);


	for (auto& pPart : m_pMonsterParts)
		Safe_Release(pPart);

	m_pMonsterParts.clear();

	Safe_Delete<MONSTERABILITY*>(m_pMonsterAbility);

	Safe_Release(m_pStateCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pSteamState);
}
