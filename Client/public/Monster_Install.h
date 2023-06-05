#pragma once
#include "Client_Defines.h"
#include "ImGui_Console.h"
#include <sstream>
BEGIN(Client)


class CMonster_Install : public CImGui_Console
{

	enum MonsterTYPE {MONSTER_GOBLINE, MONSTER_END};

public:
	CMonster_Install(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CMonster_Install() = default;

public:
	virtual HRESULT Initialize(void);
	virtual void	Show_Window(void);

private:
	void			Create_Cube(_uint eType);

private:
	_float4			m_vPickingPos = _float4(0.f, 0.f, 0.f, 1.f);
	_float3			m_vScale = _float3(0.f, 0.f, 0.f);
	_bool			m_bCreate;
	char			m_szMonsterName[MAX_PATH] = "";


public:
	static CMonster_Install* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END