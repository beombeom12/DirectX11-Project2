#include "stdafx.h"
#include "..\public\Level_Loading.h"

#include "Loader.h"
#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"

#include "LoadingBackGround.h"
#include "LoadingImage.h"

CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eID, LEVEL eNextLevelID)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Background(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_LoadingImage(TEXT("Layer_BackGroundImage"))))
		return E_FAIL;

	m_eNextLevelID = eNextLevelID;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;



	return S_OK;
}

void CLevel_Loading::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

}

void CLevel_Loading::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);


	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (true == m_pLoader->isFinished())
	{

			CLevel*		pLevel = nullptr;

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(pGameInstance->Open_Level(m_eNextLevelID, pLevel)))
				return;
	
	}

	Safe_Release(pGameInstance);
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Background(const _tchar * pLayerTag)
{
	ENGINE_OPEN;


	if (FAILED(ENGINE->Clone_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_UIBackGround"), TEXT("Prototype_GameObject_UIBackGround"))))
		return E_FAIL;
	ENGINE_CLOSE;
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_LoadingImage(const _tchar * pLayerTag)
{

	ENGINE_OPEN;




	if (FAILED(ENGINE->Clone_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_LoadingImage"), TEXT("Loading_Image"))))
		return E_FAIL;

	ENGINE_CLOSE;



	return S_OK;
}



CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eID, LEVEL eNextLevelID)
{
	CLevel_Loading*		pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eID, eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

}
