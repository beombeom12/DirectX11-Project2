#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "Moloch.h"
BEGIN(Client)
class CUI_MonsterHP : public CUI
{
public:
	CUI_MonsterHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MonsterHP(const CUI_MonsterHP& rhs);
	virtual ~CUI_MonsterHP() = default;


public:
	virtual HRESULT	Initialize_Prototype(OBJECT_ID eID, _uint eUIType);
	virtual HRESULT	Initialize_Clone(void* pArg = nullptr);
	virtual _uint	Tick(_double TimeDelta);
	virtual void	Late_Tick(_double TimeDelta);
	virtual	HRESULT	Render(void);


private:
	HRESULT					Setup_Component(void);


private:
	CUI::UIDESC			m_UIDesc;
	class	CMoloch*			m_pMonster = nullptr;



public:
	_bool		On_Switch() { return m_bSwitch = true; }
	_bool		Off_Switch() { return m_bSwitch = false; }


	_bool			m_bSwitch = false;




public:
	static CUI_MonsterHP*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID, UITYPE eType);
	virtual CGameObject*			Clone(void* pArg = nullptr);
	virtual void					Free(void);

};

END