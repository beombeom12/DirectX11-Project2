#pragma once

#include "Client_Defines.h"
#include "ImGui_Console.h"
#include <sstream>

BEGIN(Client)

class CObject_Builder : public CImGui_Console
{
public:
	enum OBJECTTYPE {	OBJECT_TERRAINA, OBJECT_LANDING,  OBJECT_DROCKA, OBJECT_DROCKB, OBJECT_CLIFFROCK,
		OBJECT_DESERTROADA, OBJECT_DESERTROADB,
		//DesertRoad
		OBJECT_ROCKPILLAR, DESERT_ROCKTALL, DESERT_TALLCOLLISION, DESERT_TALLLODA, DESERT_TALLLODB, DESERT_TERRAINRUBBLE, DESERT_DSTAIR, DESERT_DSTAIRTWO, DESERT_DSTAIRCASE,
		//DesertRoadCliff
		DESERT_ROADCLIFF, DESERT_ROADCLIFFA, DESERT_ROADCLIFFB, DESERT_ROADCLIFFC,
		//Fortress
		OBJECT_DROAD, OBJECT_ROCKA, OBJECT_CUBE, OBJECT_FSIDEWALL, OBJECT_FORTRESSGATE, OBEJCT_FSIDECLIFF,
		OBJECT_FSPIKEA, OBJECT_FSPIKEAHALF, OBJECT_FSPIKEB, OBJECT_FWALLA, OBJECT_FWALLB, OBJECT_FSTAIRUP,
		OBJECT_FROADMAIN,
		//Enviroments
		OBJECT_DTREE, OBJECT_DBARREL, OBJECT_DBARRELA, OBJECT_DBARRICADE,
		OBJECT_DBIRCHA, OBJECT_DBIRCHB, OBJECT_DBIRCHC,
		OBJECT_FORLIAGEA,
		OBJECT_END
	};

public:
	CObject_Builder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CObject_Builder() = default;

public:
	virtual HRESULT Initialize(void);
	virtual void	Show_Window(void);

private:
	void			Create_Cube(_uint eType);

private:
	_float4			m_vPickingPos = _float4(0.f, 0.f, 0.f, 1.f);
	_float3			m_vScale = _float3(0.f, 0.f, 0.f);
	_bool			m_bCreate;
	char			m_szObjectName[MAX_PATH] = "";

public:
	static CObject_Builder* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END