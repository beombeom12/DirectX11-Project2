#include "stdafx.h"
#include "..\public\Player.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Bone.h"
#include "Animation.h"
#include "Player_State.h"
#include "Camera_Dynamic.h"
#include "Collider.h"

//Monster
#include "Goblin.h"
#include "Goblin_Sword.h"

#include "Grinner.h"

#include "Moloch.h"
#include "Mo_Stone.h"


#include "Steamroller.h"
#include "SubMo.h"
#include "HellHound.h"

#include "Effect_Point_PlayerDust.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{

	CGameObject::GAMEOBJECTDESC			GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof GameObjectDesc);

	GameObjectDesc.TransformDesc.fSpeedPerSec = 7.0f;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_State()))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_WarSkill()))
		return E_FAIL;

	//
	//if (FAILED(Ready_BoneChase()))
	//	return E_FAIL;
	
	
	m_pAbility = new PLAYERABILITY;
	m_pAbility->fHP = 100.f;
	m_pAbility->fMaxHP = 100.f;
	m_pAbility->fHP = m_pAbility->fMaxHP;

	m_pModelCom->Set_AnimIndex(22);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1.494f, 0.f, 12.337f, 1.f));
	
	ENGINE_OPEN;

	m_pPointPlayerDust = (CEffect_Point_PlayerDust*)ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_PlayerPoint_Instancing"));
	m_pEffect_PlayerDust = (CEffect_PlayerDust*)ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_TestPoint_Instancing"));
	m_pEffect_Window = (CEffect_Point_PlayerWindow*)ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_WindowTest_Instancing"));


		//MESHTESTING
	m_pWarSkill = (CWarSkill*)ENGINE->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_WarSkill"));
	
	
	
	ENGINE_CLOSE;



	
	return S_OK;
}

_uint CPlayer::Tick(_double TimeDelta)
{


	if (nullptr != m_pState)
		m_pState->Tick(TimeDelta);


	m_pModelCom->Play_Animation(TimeDelta, m_fLerpAnimTime, m_fAnimAdjust);

	for (_uint i = 0; i < m_pPlayerParts.size(); ++i)
	{
		m_pPlayerParts[i]->Tick(TimeDelta);
	}



	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());

	m_pHit[PCOLLIDER_HIT]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pHit[PCOLLIDER_SWORD]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pHit[PCOLLIDER_SWORD2]->Update(m_pTransformCom->Get_tWorldMatrix());


	return __super::Tick(TimeDelta);
}

void CPlayer::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	for (_uint i = 0; i < m_pPlayerParts.size(); ++i)
	{
		m_pPlayerParts[i]->Late_Tick(TimeDelta);
	}

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	/*		for (auto& pCollider : m_pColliderCom)
			{
				m_pRendererCom->Add_DebugRenderGroup(pCollider);
			}
			for (auto & pHit : m_pHit)
			{
				m_pRendererCom->Add_DebugRenderGroup(pHit);
			}*/
	
		m_pRendererCom->Add_DebugRenderGroup(m_pNavigation);
	}

	Set_OnCell();
}

HRESULT CPlayer::Render()
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

HRESULT CPlayer::Ready_Parts()
{
	CGameObject*		pPartObject = nullptr;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CWeapon::WEAPONDESC			WeaponDesc;
	ZeroMemory(&WeaponDesc, sizeof(CWeapon::WEAPONDESC));

	WeaponDesc.PivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pSocket = m_pModelCom->Get_BonePtr("Bone_War_Weapon_Sword");
	WeaponDesc.pTargetTransform = m_pTransformCom;
	Safe_AddRef(WeaponDesc.pSocket);
	Safe_AddRef(m_pTransformCom);

	pPartObject = pGameInstance->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon"), &WeaponDesc);
	if (nullptr == pPartObject)
		return E_FAIL;

	m_pPlayerParts.push_back(pPartObject);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;

}

HRESULT CPlayer::Ready_WarSkill()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pPartObject = nullptr;


	pPartObject = pGameInstance->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_WarSkill"));

	m_pPlayerParts.push_back(pPartObject);


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer::Ready_BoneChase()
{
	CGameObject*		pGameObject = nullptr;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	CCamera::CAMERADESC		CameraDesc;

	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.pPivotMatrix = m_pModelCom->Get_PivotFloat4x4s();
	CameraDesc.pSocket = m_pModelCom->Get_BonePtr("Bone_War_Head");
	CameraDesc.pTransform = m_pTransformCom;
	Safe_AddRef(CameraDesc.pSocket);
	Safe_AddRef(CameraDesc.pTransform);

	pGameObject = pGameInstance->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Dynamic"), &CameraDesc);
	if (nullptr == pGameObject)
		return E_FAIL;

	m_pPlayerParts.push_back(pGameObject);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;}





void CPlayer::DisOverlap(CCollider * TargetOverlapCollider)
{
	//if (nullptr == m_pOverlapCollider || nullptr == TargetOverlapCollider)
	//	return;

	//_bool bOverlap = m_pOverlapCollider->Collision_Check(TargetOverlapCollider, CCollider::POWER_OVERLAP);

	//if (bOverlap)
	//{
	//	_vector vDisOverlapVector = m_pOverlapCollider->Get_Center() - TargetOverlapCollider->Get_Center();
	//	_float fContectDistance = m_pOverlapCollider->Get_Length() + TargetOverlapCollider->Get_Length();
	//	_float fCurDistance = XMVectorGetX(XMVector3Length(vDisOverlapVector));
	//	_float fResultOverlapSize = fContectDistance - fCurDistance;

	//	_float4   fMyPos, fMyDir;
	//	XMStoreFloat4(&fMyPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	//	XMStoreFloat4(&fMyDir, XMVector3Normalize(vDisOverlapVector));

	//	if (m_pNavigation->isMove_OnNavigation(&fMyPos, &fMyDir))
	//		m_pTransformCom->Move_Axis(XMVector3Normalize(vDisOverlapVector), fResultOverlapSize);
	//	else
	//	{
	//		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&fMyPos));
	//		m_pTransform->Move_Axis(XMVector3Normalize(XMLoadFloat4(&fMyDir)), fResultOverlapSize);
	//	}
	//}
}

CCollider * CPlayer::Get_WeaponCollier()
{	

	return static_cast<CWeapon*>(m_pPlayerParts.front())->Get_ColliderWeapon();

}

_bool CPlayer::Collision_ToMonster()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CCollider* pTargetCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Com_SPHERE"));

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

_bool CPlayer::Collision_AttackedRange()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//Goblin


	CGoblin* TargetGoblin = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin")));
	CGoblin* pTargetGoblin2 = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin2")));
	CGoblin* pTargetGoblin3 = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin3")));
	CGoblin* pTargetGoblin4 = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin4")));
	CGoblin* pTargetGoblin5 = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin5")));
	CGoblin* pTargetGoblin6 = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin6")));
	CGoblin* pTargetGoblin7 = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin7")));
	CGoblin* pTargetGoblin8 = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin8")));
	CGoblin* pTargetGoblin9 = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin9")));
	CGoblin* pTargetGoblin10 = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin10")));
	CGoblin* pTargetGoblin11 = static_cast<CGoblin*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Goblin11")));

	CSubMo* pTargetSubMo = static_cast<CSubMo*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_SubMo")));
	CSubMo* pTargetSubMo2 = static_cast<CSubMo*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_SubMo2")));
	CSubMo* pTargetSubMo3 = static_cast<CSubMo*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_SubMo3")));
	CSubMo* pTargetSubMo4 = static_cast<CSubMo*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_SubMo4")));
	CSubMo* pTargetSubMo5 = static_cast<CSubMo*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_SubMo5")));
	CSubMo* pTargetSubMo6 = static_cast<CSubMo*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_SubMo6")));
	CSubMo* pTargetSubMo7 = static_cast<CSubMo*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_SubMo6")));
	CSubMo* pTargetSubMo8 = static_cast<CSubMo*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_SubMo6")));


	CSteamroller* pTargetSteamRoller = static_cast<CSteamroller*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Steamroller")));
	CSteamroller* pTargetSteamRoller2 = static_cast<CSteamroller*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Steamroller2")));
	CSteamroller* pTargetSteamRoller3 = static_cast<CSteamroller*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Steamroller3")));
	CSteamroller* pTargetSteamRoller4 = static_cast<CSteamroller*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Steamroller4")));
	CSteamroller* pTargetSteamRoller5 = static_cast<CSteamroller*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Steamroller5")));
	CSteamroller* pTargetSteamRoller6 = static_cast<CSteamroller*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Steamroller6")));



	CHellHound* pTargetHellHound = static_cast<CHellHound*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_HellHound")));
	CHellHound* pTargetHellHound2 = static_cast<CHellHound*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_HellHound2")));
	CHellHound* pTargetHellHound3 = static_cast<CHellHound*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_HellHound3")));
	CHellHound* pTargetHellHound4 = static_cast<CHellHound*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_HellHound4")));
	CHellHound* pTargetHellHound5 = static_cast<CHellHound*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_HellHound5")));



	CGrinner* TargetGrinner = static_cast<CGrinner*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Grinner")));
	CMoloch*	pTargetMoloch = static_cast<CMoloch*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Moloch")));




	if (TargetGoblin->Get_SwordCollider()->Get_CheckAble()  || 
		pTargetGoblin2->Get_SwordCollider()->Get_CheckAble() ||
		pTargetGoblin3->Get_SwordCollider()->Get_CheckAble() ||
		pTargetGoblin4->Get_SwordCollider()->Get_CheckAble() ||
		pTargetGoblin5->Get_SwordCollider()->Get_CheckAble() ||
		pTargetGoblin6->Get_SwordCollider()->Get_CheckAble() ||
		pTargetGoblin7->Get_SwordCollider()->Get_CheckAble() ||
		pTargetGoblin8->Get_SwordCollider()->Get_CheckAble() || 
		pTargetGoblin9->Get_SwordCollider()->Get_CheckAble() ||
		pTargetGoblin10->Get_SwordCollider()->Get_CheckAble() ||
		pTargetGoblin11->Get_SwordCollider()->Get_CheckAble())
	{
		_bool IsColl = m_pHit[PCOLLIDER_HIT]->Collision(TargetGoblin->Get_SwordCollider());
		_bool IsColl2 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetGoblin2->Get_SwordCollider());
		_bool IsColl3 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetGoblin3->Get_SwordCollider());
		_bool IsColl4 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetGoblin4->Get_SwordCollider());
		_bool IsColl5 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetGoblin5->Get_SwordCollider());
		_bool IsColl6 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetGoblin6->Get_SwordCollider());
		_bool IsColl7 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetGoblin7->Get_SwordCollider());
		_bool IsColl8 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetGoblin8->Get_SwordCollider());
		_bool IsColl9 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetGoblin9->Get_SwordCollider());
		_bool IsColl10 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetGoblin10->Get_SwordCollider());
		_bool IsColl11 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetGoblin11->Get_SwordCollider());
		if (IsColl == true || IsColl2 == true ||
			IsColl3 == true || IsColl4 == true || 
			IsColl5 == true || IsColl6 == true || 
			IsColl7 == true || IsColl8 == true || IsColl9 == true 
			|| IsColl10 == true || IsColl11 == true)
		{
	
			if (m_bTakeDamage == false )
			{
				Damaged(1.f);
				m_bTakeDamage = true;
			
			}
				return true;
		}
	}
	


	//Submo




	if (pTargetSubMo->Get_AttackCollier()->Get_CheckAble() || pTargetSubMo2->Get_AttackCollier()->Get_CheckAble() ||
		pTargetSubMo3->Get_AttackCollier()->Get_CheckAble() || pTargetSubMo4->Get_AttackCollier()->Get_CheckAble() ||
		pTargetSubMo5->Get_AttackCollier()->Get_CheckAble() || pTargetSubMo6->Get_AttackCollier()->Get_CheckAble() ||
		pTargetSubMo7->Get_AttackCollier()->Get_CheckAble() || pTargetSubMo8->Get_AttackCollier()->Get_CheckAble()
		)
	{
		_bool IsSubmoColl = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSubMo->Get_AttackCollier());
		_bool IsSubmoColl2 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSubMo2->Get_AttackCollier());
		_bool IsSubmoColl3 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSubMo3->Get_AttackCollier());
		_bool IsSubmoColl4 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSubMo4->Get_AttackCollier());
		_bool IsSubmoColl5 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSubMo5->Get_AttackCollier());
		_bool IsSubmoColl6 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSubMo6->Get_AttackCollier());
		_bool IsSubmoColl7 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSubMo7->Get_AttackCollier());
		_bool IsSubmoColl8 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSubMo8->Get_AttackCollier());
		if (IsSubmoColl == true || IsSubmoColl2 == true || IsSubmoColl3 == true || IsSubmoColl4 == true || IsSubmoColl5 == true || IsSubmoColl6 == true 
			|| IsSubmoColl7 == true || IsSubmoColl8 ==true)
		{
			if (m_bSubMoTakeDamage == false)
			{
				Damaged(1.f);
				m_bSubMoTakeDamage = true;
			}
			return true;
		}
	}
	

	//SteamRoller




	if (pTargetSteamRoller->Get_WeaponCollier()->Get_CheckAble() || pTargetSteamRoller2->Get_WeaponCollier()->Get_CheckAble() ||
		pTargetSteamRoller3->Get_WeaponCollier()->Get_CheckAble() || pTargetSteamRoller4->Get_WeaponCollier()->Get_CheckAble() ||
		pTargetSteamRoller5->Get_WeaponCollier()->Get_CheckAble() || pTargetSteamRoller6->Get_WeaponCollier()->Get_CheckAble())
	{
		_bool IsSteamColl = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSteamRoller->Get_WeaponCollier());
		_bool IsSteamColl2 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSteamRoller2->Get_WeaponCollier());
		_bool IsSteamColl3 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSteamRoller3->Get_WeaponCollier());
		_bool IsSteamColl4 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSteamRoller4->Get_WeaponCollier());
		_bool IsSteamColl5 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSteamRoller5->Get_WeaponCollier());
		_bool IsSteamColl6 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetSteamRoller6->Get_WeaponCollier());
		if (IsSteamColl == true || IsSteamColl2 == true ||
			IsSteamColl3 == true || IsSteamColl4 == true ||
			IsSteamColl5 == true || IsSteamColl6 == true)
		{

			if (m_bSteamTakeDamage == false)
			{
				Damaged(1.f);
				m_bSteamTakeDamage = true;
			}
			return true;
		}
	}



	//HellHound

	if (pTargetHellHound->Get_RightSwordCollider()->Get_CheckAble() || pTargetHellHound2->Get_RightSwordCollider()->Get_CheckAble() ||
		pTargetHellHound3->Get_RightSwordCollider()->Get_CheckAble() || pTargetHellHound4->Get_RightSwordCollider()->Get_CheckAble() ||
		pTargetHellHound4->Get_RightSwordCollider()->Get_CheckAble())
	{
		_bool IsHellColl = m_pHit[PCOLLIDER_HIT]->Collision(pTargetHellHound->Get_RightSwordCollider());
		_bool IsHellColl2 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetHellHound2->Get_RightSwordCollider());
		_bool IsHellColl3 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetHellHound3->Get_RightSwordCollider());
		_bool IsHellColl4 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetHellHound4->Get_RightSwordCollider());
		_bool IsHellColl5 = m_pHit[PCOLLIDER_HIT]->Collision(pTargetHellHound5->Get_RightSwordCollider());
		if (IsHellColl == true || IsHellColl2 == true ||
			IsHellColl3 == true || IsHellColl4 == true ||
			IsHellColl5 == true)
		
		{

			if (m_bHellHoundTakeDamage == false)
			{
				Damaged(1.f);
				m_bHellHoundTakeDamage = true;

			}
			return true;
		}
	}







	//Grinner

	

	static _bool m_bGrinnerTakeDamage = false;
	if (TargetGrinner->Get_RightSwordCollider()->Get_CheckAble())
	{
		_bool IsRightGrinner = m_pHit[PCOLLIDER_HIT]->Collision(TargetGrinner->Get_RightSwordCollider());

		if (IsRightGrinner == true)
		{
		
			if (m_bGrinnerTakeDamage == false)
			{
				Damaged(1.f);
				m_bGrinnerTakeDamage = true;
			}
			return true;

		}
	
	}
	










	//Moloch



	static _bool					m_bMolochTakeDamage = false;
	if (pTargetMoloch->Get_SwordCollider()->Get_CheckAble())
	{
		_bool IsMolochColl = m_pHit[PCOLLIDER_HIT]->Collision(pTargetMoloch->Get_SwordCollider());

		if (IsMolochColl == true)
		{

			if (m_bMolochTakeDamage == false)
			{
				Damaged(3.f);
				m_bMolochTakeDamage = true;
			}
			return true;

		}

	}





	//MolochStone

	 if (pTargetMoloch->Get_StoneCollider()->Get_CheckAble())
	{
		_bool IsMoStoneColl = m_pHit[PCOLLIDER_HIT]->Collision(pTargetMoloch->Get_StoneCollider());

		if (IsMoStoneColl == true)
		{

			if (m_bMolochStoneTakeDamage == false)
			{
			Damaged(5.f);
				m_bMolochStoneTakeDamage = true;
			}
			return true;
		}
	}
	













	m_bTakeDamage = false;
	m_bSubMoTakeDamage = false;
	m_bSteamTakeDamage = false;
	m_bMolochTakeDamage = false;
	m_bMolochStoneTakeDamage = false;
	m_bHellHoundTakeDamage = false;
	m_bGrinnerTakeDamage = false;
	return false;




	RELEASE_INSTANCE(CGameInstance);

}
void CPlayer::Damaged(_float fDamage)
{
	m_pAbility->fHP -= fDamage;
	if (m_pAbility->fHP <= 0.f)
		m_pAbility->fHP = 0.f;
}


HRESULT CPlayer::SetUp_Components()
{
	//Renderer
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	//Shader
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;
	
	//Model
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RunPerson"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	//STATE
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pState)))
		return E_FAIL;



	//Collider

	CCollider::COLLIDERDESC						ColliderDesc;


	//Sphere
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;
	
	
	//Hit

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(15.f, 5.f, 15.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_PLAYER_HIT"),
		(CComponent**)&m_pHit[PCOLLIDER_HIT], &ColliderDesc)))
		return E_FAIL;


	//for Boss Skill

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(30.f, 5.f, 30.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_PLAYER_ATTACKED_SKILL"),
		(CComponent**)&m_pHit[PCOLLIDER_SWORD], &ColliderDesc)))
		return E_FAIL;

	//for Final Boss SKill2

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(60.f, 5.f, 60.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_PLAYER_ATTACKED_SKILL2"),
		(CComponent**)&m_pHit[PCOLLIDER_SWORD2], &ColliderDesc)))
		return E_FAIL;


	//Target

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(10.0f, 1.f, 10.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y /** 1.f*/, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"), TEXT("Com_SPHERE_TARGET"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE_TARGET], &ColliderDesc)))
		return E_FAIL;





	//Navigation
	CNavigation::NAVIDESC			NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::tagNavigationDesc));

	NaviDesc.iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"),
		(CComponent**)&m_pNavigation, &NaviDesc))) 
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderResources()
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


	/* For.Lights */
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CPlayer::SetUp_State()
{
	if (nullptr == m_pState)
		return E_FAIL;

	m_pPlayer_StateCom = CPlayer_State::Create(this);
	if (nullptr == m_pPlayer_StateCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Set_OnCell()
{

	ENGINE_OPEN;

	_vector      vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float      fHeight = m_pNavigation->HeightOnTerrain(vPos);

	m_pTransformCom->Set_Height(fHeight);

	ENGINE_CLOSE;

	return S_OK;


}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CPlayer*		pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("BOOOOOOOOOOOMING : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("BOOOOOOOOOOMING Clone : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	for (_uint i = 0; i < PCOLLIDER_END; ++i)
		Safe_Release(m_pHit[i]);

	for (auto& pPart : m_pPlayerParts)
		Safe_Release(pPart);

	m_pPlayerParts.clear();


	Safe_Delete<PLAYERABILITY*>(m_pAbility);
	Safe_Release(m_pNavigation);
	Safe_Release(m_pState);
	Safe_Release(m_pPlayer_StateCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}


