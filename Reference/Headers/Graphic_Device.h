#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;
public:
	HRESULT Ready_Graphic_Device(HWND hWnd, GRAPHIC_DESC::WINMODE WinMode, _uint iWinCX, _uint iWinCY, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

	HRESULT Update_SwapChain(HWND hWnd, _uint iWinCX, _uint iWinCY, _bool bIsFullScreen, _bool bNeedUpdate);

private:	

	/* �޸� �Ҵ�. (��������, �ε�������, �ؽ��ķε�) */
	ID3D11Device*			m_pDevice = nullptr;

	/* ���ε��۾�. ��ɽ��� (�������۸� SetStreamSource(), SetIndices(), SetTransform(), SetTexture() */
	ID3D11DeviceContext*	m_pContext = nullptr;

	IDXGISwapChain*			m_pSwapChain = nullptr;	

	// ID3D11ShaderResourceView*
	ID3D11RenderTargetView*		m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;

private:
	HRESULT Ready_SwapChain(HWND hWnd, GRAPHIC_DESC::WINMODE WinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

public:
	virtual void Free() override;
};

END