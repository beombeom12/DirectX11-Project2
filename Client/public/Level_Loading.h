#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVEL eID, LEVEL eNextLevelID);
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CLoader*	m_pLoader = nullptr;
	LEVEL			m_eNextLevelID = LEVEL_END;

private:
	HRESULT	Ready_Layer_Background(const _tchar* pLayerTag);
	HRESULT Ready_Layer_LoadingImage(const _tchar* pLayerTag);


public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID, LEVEL eNextLevel);
	virtual void Free() override;
};

END