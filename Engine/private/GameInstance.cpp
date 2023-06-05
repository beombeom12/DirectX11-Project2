#include "..\public\GameInstance.h"

#include "Graphic_Device.h"
#include "ImGui_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Timer_Manager.h"
#include "Light_Manager.h"
#include "Component_Manager.h"
#include "Target_Manager.h"
#include "Font_Manager.h"
#include "ImguiObject.h"

IMPLEMENT_SINGLETON(CGameInstance)

_uint CGameInstance::m_iStaticLevelIndex = 0;
const _tchar* CGameInstance::m_pPrototypeTransformTag = TEXT("Prototype_Component_Transform");

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pImgui_Manager(CImGui_Manager::GetInstance())
	, m_pFont_Manager(CFont_Manager::GetInstance())
	, m_pSound_Manager(CSound_Manager::GetInstance())
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pSound_Manager);
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pImgui_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CGameInstance::Create_MainWindow(void)
{
	if (nullptr == m_pImgui_Manager)
		return E_FAIL;

	m_pImgui_Manager->CreateMainWindow();

	return S_OK;
}

HRESULT CGameInstance::Add_Console(const _tchar * pName, CImGui_Console * pConsole)
{
	if (nullptr == m_pImgui_Manager)
		return E_FAIL;

	return m_pImgui_Manager->Add_Console(pName, pConsole);

}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pInput_Device ||
		nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager ||
		nullptr == m_pImgui_Manager)
		return E_FAIL;

	/* 그래픽 디바이스 초기화. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWindowMode, GraphicDesc.iViewportSizeX, GraphicDesc.iViewportSizeY, ppDeviceOut, ppContextOut)))
		return E_FAIL;

	m_hWnd = GraphicDesc.hWnd;
	m_pDevice = *ppDeviceOut;
	m_pContext = *ppContextOut;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (FAILED(m_pInput_Device->Ready_Input_Device(hInst, GraphicDesc.hWnd)))
		return E_FAIL;


	CoInitializeEx(nullptr, COINIT_MULTITHREADED);


	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels + 1)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels + 1)))
		return E_FAIL;
	
	m_iStaticLevelIndex = iNumLevels;
	

	if (FAILED(m_pImgui_Manager->Ready_Imgui(GraphicDesc.hWnd, *ppDeviceOut, *ppContextOut)))
		return E_FAIL;


	if (FAILED(m_pComponent_Manager->Add_Prototype(m_iStaticLevelIndex, m_pPrototypeTransformTag, CTransform::Create(*ppDeviceOut, *ppContextOut))))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Initialize(*ppDeviceOut, *ppContextOut)))
	//	return E_FAIL;

	if (FAILED(SetUp_Engine_Console()))
		return E_FAIL;

	if (FAILED(m_pSound_Manager->Reserve_Manager("../Bin/Resources/Sound/", 10)))
		return E_FAIL;
	return S_OK;
}

void CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pInput_Device ||
		nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager ||
		nullptr == m_pImgui_Manager)
		return;

	m_pInput_Device->Invalidate_Input_Device();

	m_pObject_Manager->Tick(TimeDelta);

	m_pLevel_Manager->Tick(TimeDelta);

	m_pSound_Manager->Tick(TimeDelta);

	m_pImgui_Manager->Tick(TimeDelta);

	m_pPipeLine->Tick();
}

void CGameInstance::Late_Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pImgui_Manager ||
		nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Late_Tick(TimeDelta);

	m_pLevel_Manager->Late_Tick(TimeDelta);

	m_pImgui_Manager->Late_Tick(TimeDelta);

	m_pInput_Device->Reset_All();
}

void CGameInstance::Clear_Level(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Clear(iLevelIndex);
}

HRESULT CGameInstance::Render_Engine(void)
{
	if (nullptr == m_pImgui_Manager ||
		nullptr == m_pLevel_Manager)
		return E_FAIL;

	if(
	nullptr == m_pLevel_Manager)
	return E_FAIL;

	if (FAILED(m_pImgui_Manager->Render()))
		return E_FAIL;

	if (FAILED(m_pLevel_Manager->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_Graphic_Device(const _float4 * pColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	HRESULT			hr = 0;

	hr = m_pGraphic_Device->Clear_BackBuffer_View(*pColor);
	hr = m_pGraphic_Device->Clear_DepthStencil_View();

	return hr;
}


HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

HRESULT CGameInstance::Update_SwapChain(HWND hWnd, _uint iWinCX, _uint iWInCY, _bool bIsFullScreen, _bool bNeedUpdate)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Update_SwapChain(hWnd, iWinCX, iWInCY, bIsFullScreen, bNeedUpdate);
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE byMouseID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseState(byMouseID);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMoveState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMove(eMoveState);
}

_bool CGameInstance::Key_Down(_ubyte KeyState)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Key_Down(KeyState);
}

_bool CGameInstance::Key_Up(_ubyte KeyState)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Key_Up(KeyState);
}

_bool CGameInstance::Key_Pressing(_ubyte KeyState)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Key_Pressing(KeyState);
}

_bool CGameInstance::Mouse_Down(MOUSEKEYSTATE MouseState)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Mouse_Down(MouseState);
}

_bool CGameInstance::Mouse_Up(MOUSEKEYSTATE MouseState)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Mouse_Up(MouseState);
}

_bool CGameInstance::Mouse_Pressing(MOUSEKEYSTATE MouseState)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Mouse_Pressing(MouseState);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

HRESULT CGameInstance::Render_Level()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Render();
}

LEVEL CGameInstance::Get_NowLevel()
{
	if (nullptr == m_pLevel_Manager)
		return LEVEL_END;

	return m_pLevel_Manager->Get_LevelID();
}

CComponent * CGameInstance::Get_ComponentPtr(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_ComponentPtr(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Clone_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, const _tchar* pMyName, void * pArg, CGameObject** ppGameObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Clone_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pMyName, pArg, ppGameObject);
}

CGameObject * CGameInstance::Clone_NoGameObject(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Clone_GameObject(iLevelIndex, pPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4(eState);
}

_matrix CGameInstance::Get_TransformMatrix_Inverse(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix_Inverse(eState);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

_float4 CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return _float4(0.0f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_CamPosition();
}

_double CGameInstance::Get_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0;

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Ready_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Ready_Timer(pTimerTag);
}

void CGameInstance::Update_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	m_pTimer_Manager->Update_Timer(pTimerTag);
}

const LIGHTDESC * CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pContext, LightDesc);
}

HRESULT CGameInstance::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;
	return m_pFont_Manager->Add_Font(pDevice, pContext, pFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _tchar * pFontTag, const _tchar * pText, const _float2 & vPosition, _float fRadian, _float2 vScale, _fvector vColor)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;
	return m_pFont_Manager->Render_Font(pFontTag, pText, vPosition, fRadian, vScale, vColor);
}

ID3D11ShaderResourceView * CGameInstance::Get_DepthTargetSRV()
{
	if (nullptr == m_pTarget_Manager)
		return nullptr;

	return m_pTarget_Manager->Get_SRV(TEXT("Target_Depth"));
}

void CGameInstance::Play_Sound(const _tchar * pSoundKey, _float fVolume, _bool bIsBGM, _int iManualChannelIndex)
{
	if (m_pSound_Manager == nullptr) return;
	m_pSound_Manager->Play_Sound(pSoundKey, fVolume, bIsBGM, iManualChannelIndex);
}

void CGameInstance::Stop_Sound(_uint iManualChannelIndex)
{
	if (m_pSound_Manager == nullptr) return;
	m_pSound_Manager->Stop_Sound(iManualChannelIndex);
}

void CGameInstance::Set_Volume(_uint iManualChannelIndex, _float fVolume)
{
	if (m_pSound_Manager == nullptr) return;
	m_pSound_Manager->Set_Volume(iManualChannelIndex, fVolume);
}

void CGameInstance::Set_MasterVolume(_float fVolume)
{
	if (m_pSound_Manager == nullptr) return;
	m_pSound_Manager->Set_MasterVolume(fVolume);
}

void CGameInstance::Set_SoundDesc(const _tchar * pSoundKey, CSound::SOUND_DESC & SoundDesc)
{
	if (m_pSound_Manager == nullptr) return;
	m_pSound_Manager->Set_SoundDesc(pSoundKey, SoundDesc);
}

CGameObject * CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar * LayerTag, const _tchar * ObjectTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	for (auto& iter : *(m_pObject_Manager->Get_ObjectList(iLevelIndex, LayerTag)))
	{
		if (!lstrcmp(iter->Get_MyName(), ObjectTag))
		{
			return iter;
		}
	}

	return nullptr;
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * LayerTag, const _tchar * ObjectTag, const _tchar * ComponentTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	for (auto& iter : *(m_pObject_Manager->Get_ObjectList(iLevelIndex, LayerTag)))
	{
		if (!lstrcmp(iter->Get_MyName(), ObjectTag))
		{
			return iter->Get_Component(ComponentTag);
		}
	}

	return nullptr;
}

HRESULT CGameInstance::SetUp_Engine_Console(void)
{
	if (nullptr == m_pImgui_Manager)
		return E_FAIL;

	if (FAILED(m_pImgui_Manager->Add_Console(TEXT("IMGUI_OBJECT"), CImguiObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->DestroyInstance();

	CObject_Manager::GetInstance()->DestroyInstance();

	CComponent_Manager::GetInstance()->DestroyInstance();

	CLevel_Manager::GetInstance()->DestroyInstance();

	CInput_Device::GetInstance()->DestroyInstance();

	CPipeLine::GetInstance()->DestroyInstance();

	CFont_Manager::GetInstance()->DestroyInstance();

	CLight_Manager::GetInstance()->DestroyInstance();

	CTarget_Manager::GetInstance()->DestroyInstance();


	CGraphic_Device::GetInstance()->DestroyInstance();
	CSound_Manager::GetInstance()->DestroyInstance();

	CTimer_Manager::GetInstance()->DestroyInstance();

	CImGui_Manager::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);

	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pImgui_Manager);
}

