#include "stdafx.h"
#include "..\public\SubMo.h"
#include "GameInstance.h"
#include "SubeMo_State.h"
#include "Player.h"
#include "Bone.h"
CSubMo::CSubMo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CSubMo::CSubMo(const CSubMo & rhs)
	:CGameObject(rhs)
{
}

HRESULT CSubMo::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSubMo::Initialize(void * pArg)
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

	m_pModelCom->Set_AnimIndex(5);

	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(60.f, 0.f, 150.f, 1.f));

	m_pMonsterAbility = new MONSTERABILITY;
	m_pMonsterAbility->fCurrentHP = 100.f;
	m_pMonsterAbility->fMaxHP = 100.f;
	m_pMonsterAbility->fCurrentHP = m_pMonsterAbility->fMaxHP;

	return S_OK;
}

_uint CSubMo::Tick(_double TimeDelta)
{


	if (nullptr != m_pStateCom)
		m_pStateCom->Tick(TimeDelta);


	m_fAnimTimDelta += TimeDelta;


	m_pModelCom->Play_Animation(TimeDelta, m_fLerfTime, m_fAdjustTime);

	for (_uint i = 0; i < m_pMonsterParts.size(); ++i)
	{
		m_pMonsterParts[i]->Tick(TimeDelta);
	}

	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pHit[COLLIDER_HIT]->Update(m_pTransformCom->Get_tWorldMatrix());


	return __super::Tick(TimeDelta);
}

void CSubMo::Late_Tick(_double TimeDelta)
{

	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{


		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		for (auto& pCollider : m_pColliderCom)
		{
			m_pRendererCom->Add_DebugRenderGroup(pCollider);
		}
		for (auto & pHit : m_pHit)
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
	//Set_OnCell();
}

HRESULT CSubMo::Render()
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

HRESULT CSubMo::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SubMo"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	//STATE
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pStateCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;


	// For.Com_SPHERE 
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(12.0f, 1.9f, 12.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;



	//HIT SPHERE
	ColliderDesc.vSize = _float3(6.f, 1.9f, 6.0f);
	ColliderDesc.vCenter = _float3(1.f, ColliderDesc.vSize.y * 1.2f, 1.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE_HIT"),
		(CComponent**)&m_pHit[COLLIDER_HIT], &ColliderDesc)))
		return E_FAIL;



	ColliderDesc.vSize = _float3(7.f, 1.9f, 7.0f);
	ColliderDesc.vCenter = _float3(1.f, -2.f, -2.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE_Attack"),
		(CComponent**)&m_pAttack[ATTACK_A], &ColliderDesc)))
		return E_FAIL;


	// For.Com_SPHERE_TARGET
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(40.0f, 1.f, 40.0f);
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

HRESULT CSubMo::SetUp_ShaderResources()
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

HRESULT CSubMo::SetUp_State()
{
	if (nullptr == m_pStateCom)
		return E_FAIL;

	m_pSubMoState = CSubeMo_State::Create(this);
	if (nullptr == m_pSubMoState)
		return E_FAIL;

	return S_OK;
}

HRESULT CSubMo::Set_OnCell()
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float fHeight = m_pNavigationCom->HeightOnTerrain(vPos);

	m_pTransformCom->Set_Height(fHeight);


	return S_OK;
}

_bool CSubMo::Collision_Player()
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

_bool CSubMo::Collision_ChaseRange()
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

_bool CSubMo::Collision_AttackedRange()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pTargetPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player")));

	if (pTargetPlayer->Get_WeaponCollier()->Get_CheckAble())
	{
		_bool IsColl = m_pHit[COLLIDER_HIT]->Collision(pTargetPlayer->Get_WeaponCollier());
	
		if (IsColl == true)
		{
			if (m_TakeDamge == false)
			{
				Player_ToDamaged(15.f);
				m_TakeDamge = true;
			}
			return true;
		}
		else
		{
			m_TakeDamge = false;
			return false;
		}

	}
	else
	{
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}	

	
}

_bool CSubMo::isDead()
{

	if (m_pMonsterAbility->fCurrentHP == 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void CSubMo::Player_ToDamaged(_float fDamage)
{
	m_pMonsterAbility->fCurrentHP -= fDamage;

	if (m_pMonsterAbility->fCurrentHP <= 0.f)
		m_pMonsterAbility->fCurrentHP = 0.f;

	

}

void CSubMo::Set_Collider(void)
{
	_matrix   RightMatrix =
		m_pModelCom->Get_BonePtr("Bone_G_Claw_R")->Get_OffsetMatrix()
		* m_pModelCom->Get_BonePtr("Bone_G_Claw_R")->Get_CombindMatrix()
		* m_pModelCom->Get_PivotMatrix();

	RightMatrix.r[0] = XMVector3Normalize(RightMatrix.r[0]);
	RightMatrix.r[1] = XMVector3Normalize(RightMatrix.r[1]);
	RightMatrix.r[2] = XMVector3Normalize(RightMatrix.r[2]);
	RightMatrix = RightMatrix * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());

	m_pAttack[0]->Update(RightMatrix);
}

CSubMo * CSubMo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CSubMo*		pInstance = new CSubMo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Created : CSubMo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSubMo::Clone(void * pArg)
{
	CSubMo*		pInstance = new CSubMo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create Cloned : CSubMo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSubMo::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);



	//for (auto& pPart : m_pMonsterParts)
	//	Safe_Release(pPart);


	Safe_Delete<MONSTERABILITY*>(m_pMonsterAbility);

	Safe_Release(m_pStateCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pSubMoState);
}
