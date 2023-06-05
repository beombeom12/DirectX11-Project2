#include "stdafx.h"
#include "..\public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_InstallationObject(TEXT("Game_Play_Layer_Object"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_InstallMosnter(TEXT("Install_Monster"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	

	ENGINE_OPEN;

	ENGINE->Play_Sound(TEXT("18 Dredge Works.mp3"), 1, true, 0);

	ENGINE_CLOSE;


	return S_OK;
}

void CLevel_GamePlay::Tick(_double TimeDelta)
{



	__super::Tick(TimeDelta);

}

void CLevel_GamePlay::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Level : GAMEPLAY"));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.isEnable = true;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof LightDesc);

	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.isEnable = true;
	///*LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);*/
	//LightDesc.vPosition = _float4(5.f, 3.f, 5.f, 1.f);
	//LightDesc.fRange = 10.0f;
	//LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.2f, 0.2f, 0.2f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;


	//ZeroMemory(&LightDesc, sizeof LightDesc);

	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.isEnable = true;
	//
	//LightDesc.vPosition = _float4(10.f, 3.f, 5.f, 1.f);
	//LightDesc.fRange = 10.0f;
	//LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	//LightDesc.vAmbient = _float4(0.2f, 0.4f, 0.2f, 0.2f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"), TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;






	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(0.f, 10.f, -10.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.vUp = _float4(0.f, 0.5f, 0.f, 0.f);


	CameraDesc.FOV = XMConvertToRadians(60.0f);
	CameraDesc.Aspect = g_iWinSizeX / _float(g_iWinSizeY);
	CameraDesc.NearZ = 0.2f;
	CameraDesc.FarZ = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 60.0f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.m_MouseMovement = 0.1f;




	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Dynamic"),nullptr, &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"), TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);






		////Goblin


		//F1
		CGameObject* pGameObject = nullptr;
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin"), TEXT("Prototype_GameObject_Goblin"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(91.390f, 0.f, 32.78f, 1.f));



		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin2"), TEXT("Prototype_GameObject_Goblin2"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(107.327f, 0.f, 28.829f, 1.f));


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin3"), TEXT("Prototype_GameObject_Goblin3"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(90.098f, 0.f, 3.176f, 1.f));


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin4"), TEXT("Prototype_GameObject_Goblin4"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(107.27f, 0.f, 8.294f, 1.f));


		////GB  F2

		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin5"), TEXT("Prototype_GameObject_Goblin5"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(293.427f, 0.f, 19.2f, 1.f));


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin6"), TEXT("Prototype_GameObject_Goblin6"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(275.055f, 0.f, 25.647f, 1.f));


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin7"), TEXT("Prototype_GameObject_Goblin7"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(274.33f, 0.f, -5.048f, 1.f));


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin8"), TEXT("Prototype_GameObject_Goblin8"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(310.96f, 0.f, 13.056f, 1.f));
			


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin9"), TEXT("Prototype_GameObject_Goblin9"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(293.427f, 0.f, -1.905f, 1.f));


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin10"), TEXT("Prototype_GameObject_Goblin10"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(800.313f, 65.370f, 168.114f, 1.f));



		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Goblin11"), TEXT("Prototype_GameObject_Goblin11"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(811.488f, 65.370f, 168.114f, 1.f));





		//SubSpot
		CGameObject* pSubmo = nullptr;
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_SubMo"), TEXT("Prototype_GameObject_SubMo"), nullptr, &pSubmo)))
			return E_FAIL;
		if (pSubmo != nullptr)
			pSubmo->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(73.840f, 0.f, 28.010f, 1.f));

		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_SubMo2"), TEXT("Prototype_GameObject_SubMo2"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(76.266f, 0.f, 4.061f, 1.f));


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_SubMo3"), TEXT("Prototype_GameObject_SubMo3"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(261.55f, 0.f, 1.288f, 1.f));

		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_SubMo4"), TEXT("Prototype_GameObject_SubMo4"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(262.184f, 0.f, 20.286f, 1.f));


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_SubMo5"), TEXT("Prototype_GameObject_SubMo5"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(662.221f, 26.186f, 24.193f, 1.f));

		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_SubMo6"), TEXT("Prototype_GameObject_SubMo6"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(621.239f, 26.186f, 41.553f, 1.f));


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_SubMo7"), TEXT("Prototype_GameObject_SubMo7"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(826.839f, 65.003f, 167.868f, 1.f));

		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_SubMo8"), TEXT("Prototype_GameObject_SubMo8"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(784.223f, 65.003f, 175.134f, 1.f));




		////SteamRoller  Spot 
		CGameObject* pSteamRoller = nullptr;
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Steamroller"), TEXT("Prototype_GameObject_Steamroller"), nullptr, &pSteamRoller)))
			return E_FAIL;
		if (pSteamRoller != nullptr)
			pSteamRoller->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(398.1851f, 28.386058f, 6.964f, 1.f));



		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Steamroller2"), TEXT("Prototype_GameObject_Steamroller2"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(412.302f, 28.285823f, -9.07f, 1.f));



		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Steamroller3"), TEXT("Prototype_GameObject_Steamroller3"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(662.345f, 26.3296f, 46.2562f, 1.f));




		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Steamroller4"), TEXT("Prototype_GameObject_Steamroller4"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(635.9954f, 26.6730f, 1.99f, 1.f));

		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Steamroller5"), TEXT("Prototype_GameObject_Steamroller5"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(827.296f, 65.682f, 152.999f, 1.f));


		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Steamroller6"), TEXT("Prototype_GameObject_Steamroller6"), nullptr, &pGameObject)))
			return E_FAIL;
		if (pGameObject != nullptr)
			pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(792.910f, 65.457f, 155.422f, 1.f));




	//	//HellHound Spot
		CGameObject* pHellHound = nullptr;
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_HellHound"), TEXT("Prototype_GameObject_HellHound"), nullptr, &pHellHound)))
	return E_FAIL;
	if (pHellHound != nullptr)
		pHellHound->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(677.456482f, 3.95517203f, 108.48f, 1.f));



	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_HellHound2"), TEXT("Prototype_GameObject_HellHound2"), nullptr, &pGameObject)))
		return E_FAIL;
	if (pGameObject != nullptr)
		pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(677.456f, 3.955f, 82.283f, 1.f));


	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_HellHound3"), TEXT("Prototype_GameObject_HellHound3"), nullptr, &pGameObject)))
		return E_FAIL;
	if (pGameObject != nullptr)
		pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(777.948669f, 66.01710551f, 260.921875f, 1.f));



	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_HellHound4"), TEXT("Prototype_GameObject_HellHound4"), nullptr, &pGameObject)))
		return E_FAIL;
	if (pGameObject != nullptr)
		pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(773.471f, 66.5978622f, 234.511688f, 1.f));


	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_HellHound5"), TEXT("Prototype_GameObject_HellHound5"), nullptr, &pGameObject)))
		return E_FAIL;
	if (pGameObject != nullptr)
		pGameObject->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(637.599f, 25.621f, 30.097f, 1.f));





	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster"), TEXT("Prototype_GameObject_Monster"))))
	//	return E_FAIL;


		//Grinner1
		CGameObject* pGrinner = nullptr;
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Grinner"), TEXT("Prototype_GameObject_Grinner"),nullptr, &pGrinner)))
			return E_FAIL;
		if (pGrinner != nullptr)
			pGrinner->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(706.587036f, 3.307182 , 100.31f, 1.f));






















		//Moloch
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Moloch"), TEXT("Prototype_GameObject_Moloch"))))
			return E_FAIL;

	


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pGameObject = nullptr;

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect"), TEXT("Prototype_GameObject_Effect"))))
		return E_FAIL;
	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect_Point_Instancing"), TEXT("Prototype_GameObject_Effect_Point_Instancing"))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect_PlayerPoint_Instancing"), TEXT("Prototype_GameObject_Effect_PlayerPoint_Instancing"))))
	//	return E_FAIL;
	
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect_TestPoint_Instancing"), TEXT("Prototype_GameObject_Effect_TestPoint_Instancing"))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect_WindowTest_Instancing"), TEXT("Prototype_GameObject_Effect_WindowTest_Instancing"))))
	//	return E_FAIL;
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_InstallationObject(const _tchar * pLayerTag)
{


	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UI_Player"), TEXT("Prototype_UI_Player"))))
		return E_FAIL;
	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UI_Player"))))
	//	return E_FAIL;
	
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UI_PlayerHPBAR"), TEXT("Prototype_UI_PlayerHPBAR"))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UI_PlayerHP"), TEXT("Prototype_UI_PlayerHP"))))
		return E_FAIL;
	
	//SkillTool
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UI_Player_SkillTool"), TEXT("Prototype_UI_Player_SkillTool"))))
		return E_FAIL;

	//Skill1
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UI_Player_Skill1"), TEXT("Prototype_UI_Player_Skill1"))))
		return E_FAIL;

	//Skill2
	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UI_Player_Skill2"), TEXT("Prototype_UI_Player_Skill2"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UI_MonsterHP"), TEXT("Prototype_UI_MonsterHP"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

\
CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();


}
