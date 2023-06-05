#pragma once

#include "ImGui_Console.h"

BEGIN(Engine)

class ENGINE_DLL CImguiObject : public CImGui_Console
{
private:
	CImguiObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImguiObject() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Show_Window();

private:
	void					Release_Info();
	class CGameObject*		Find_Object(const _tchar* pMyName);
	void					Control_Component(const char* pComponentName);
	void					Peeking_Object();
	void					ImGuizmo_Setting(void);
	void					SaveLoad_Object();
	void					Navi_Peeking();
private:
	char					m_szSaveDateName[MAX_PATH] = "GAME_PLAY";

	_uint					m_iNumLevels = 0;

private:
	map<const _tchar*, class CLayer*>*		m_mapLayer;		
	list<class CGameObject*>*				m_ObjectList;
	map<const _tchar*, class CComponent*>*	m_mapComponent; 

private:
	const _tchar*	m_PeekedLayerName = TEXT("");
	const _tchar*	m_PeekedObjectName = TEXT("");
	const _tchar*	m_PeekedComponent = TEXT("");

private:
	class CGameObject*	m_CurrentObject = nullptr;		
	class CComponent*	m_CurrentComponent = nullptr;	

public:
	static CImguiObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END