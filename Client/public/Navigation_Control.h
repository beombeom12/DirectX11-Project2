#pragma once

#include "Client_Defines.h"
#include "ImGui_Console.h"

BEGIN(Client)

class CNavigation_Control : public CImGui_Console
{
private:
	CNavigation_Control(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CNavigation_Control() = default;

public:
	virtual HRESULT Initialize(void);
	virtual void	Show_Window(void);
	void			ImGui_Setting(void);
	void			CreateImgui();
private:
	void			Picking_Cube();
	void			Ready_Layer_Cube(const _tchar* pLayerTag);
	void			Init();



private: 

	vector<class CGameObject*>		m_SquareCube;
	list<_float3>					m_PointList;
	class	CNavigation*			m_NaviCom;

	_float4		m_fCellPoint = _float4{ 0.f, 0.f, 0.f, 1.f };

	_bool		m_bCreate;
	_bool		m_bCellCreate;
	_bool		m_bDelete;

	//
	_uint		m_iCurIndex = 0;
	_int		m_iCurrentIndex = -1;
	_float3		m_vPoint[3];
	//
	char		m_szCellName[MAX_PATH] = "Navigation";
	//
	char		m_szCubeName[MAX_PATH] = "Cube";

	_int				m_iDeleteCellIndex = -1;
public:
	static CNavigation_Control* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END