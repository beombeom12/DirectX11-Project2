#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CUI_Player	: public CUI
{
public:
	CUI_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Player(const CUI_Player& rhs);
	virtual ~CUI_Player() = default;


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

public:
	static CUI_Player*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID, UITYPE eType);
	virtual CGameObject*			Clone(void* pArg = nullptr);
	virtual void					Free(void);

};

END