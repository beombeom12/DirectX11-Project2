#pragma once

/* 싱글톤.  */
/* 클라이언트와 엔진의 소통을 위해 만들어진 클래스. */

#include "Base.h"
#include "Input_Device.h"
#include "Component_Manager.h"
#include "PipeLine.h"
#include "VIBuffer_Square.h"
#include "Calculator.h"
#include "Transform.h"
#include "Renderer.h"
#include "State.h"
#include "Sound_Manager.h"
BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	static _uint Get_StaticLevelIndex() {
		return m_iStaticLevelIndex;
	}

public:
	HWND GetHWND() { return m_hWnd; }

public:
	static const _tchar*		m_pPrototypeTransformTag;	//

public: // For.Imgui
	HRESULT Create_MainWindow(void);
	HRESULT Add_Console(const _tchar* pName, class CImGui_Console* pConsole);

public: /* For.GameInstance */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	void Tick_Engine(_double TimeDelta);
	void Late_Tick_Engine(_double TimeDelta);
	void Clear_Level(_uint iLevelIndex);
	HRESULT Render_Engine(void);

public: /* For.Graphic_Device */
	HRESULT Clear_Graphic_Device(const _float4* pColor);
	HRESULT Present();
	HRESULT Update_SwapChain(HWND hWnd, _uint iWinCX, _uint iWInCY, _bool bIsFullScreen, _bool bNeedUpdate);

public: /* For.Input_Device */
	_byte		Get_DIKeyState(_ubyte byKeyID);
	_byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID);
	_long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState);
	_bool		Key_Down(_ubyte KeyState);
	_bool		Key_Up(_ubyte KeyState);
	_bool		Key_Pressing(_ubyte KeyState);
	_bool		Mouse_Down(MOUSEKEYSTATE MouseState);
	_bool		Mouse_Up(MOUSEKEYSTATE MouseState);
	_bool		Mouse_Pressing(MOUSEKEYSTATE MouseState);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	HRESULT Render_Level();
	LEVEL	Get_NowLevel();

public: /* For.Object_Manager */
	class CComponent* Get_ComponentPtr(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Clone_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, const _tchar* pMyName = nullptr, void* pArg = nullptr, CGameObject** ppGameObject = nullptr);
	CGameObject* Clone_NoGameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: // For.PipLine
	_matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE eState);
	_matrix Get_TransformMatrix_Inverse(CPipeLine::TRANSFORMSTATE eState);
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_float4 Get_CamPosition();

public: // For.Timer_Manager
	_double Get_TimeDelta(const _tchar* pTimerTag);
	HRESULT	Ready_Timer(const _tchar* pTimerTag);
	void	Update_Timer(const _tchar* pTimerTag);

public:// For.Light_Manager
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);


public://Font_Manager
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPosition, _float fRadian, _float2 vScale, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f));
public: /* For.Target_Manager */
	ID3D11ShaderResourceView* Get_DepthTargetSRV();


public: /* For.Sound_Manager */
	void Play_Sound(const _tchar *pSoundKey, _float fVolume, _bool bIsBGM = false, _int iManualChannelIndex = -1);
	void Stop_Sound(_uint iManualChannelIndex);
	void Set_Volume(_uint iManualChannelIndex, _float fVolume);
	void Set_MasterVolume(_float fVolume);
	void Set_SoundDesc(const _tchar *pSoundKey, CSound::SOUND_DESC& SoundDesc);


private:
	static _uint				m_iStaticLevelIndex;
	HWND						m_hWnd;

public:
	CGameObject*	Get_GameObject(_uint iLevelIndex, const _tchar* LayerTag, const _tchar* ObjectTag);
	CComponent*		Get_Component(_uint iLevelIndex, const _tchar* LayerTag, const _tchar* ObjectTag, const _tchar* ComponentTag);

private:
	HRESULT	SetUp_Engine_Console(void);

private:
	class CGraphic_Device*			m_pGraphic_Device = nullptr;
	class CInput_Device*			m_pInput_Device = nullptr;
	class CLevel_Manager*			m_pLevel_Manager = nullptr;
	class CObject_Manager*			m_pObject_Manager = nullptr;
	class CComponent_Manager*		m_pComponent_Manager = nullptr;
	class CPipeLine*				m_pPipeLine = nullptr;
	class CTimer_Manager*			m_pTimer_Manager = nullptr;
	class CLight_Manager*			m_pLight_Manager = nullptr;
	class CImGui_Manager*			m_pImgui_Manager = nullptr;
	class CFont_Manager*			m_pFont_Manager = nullptr;
	class CSound_Manager*			m_pSound_Manager = nullptr;
	class CTarget_Manager*			m_pTarget_Manager = nullptr;
private:
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pContext = nullptr;

public:
	static void Release_Engine();

public:
	virtual void Free() override;
};

END