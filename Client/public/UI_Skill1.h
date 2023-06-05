#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CUI_Skill : public CUI
{
public:
	CUI_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Skill(const CUI_Skill& rhs);
	virtual ~CUI_Skill() = default;


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
	static CUI_Skill*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID, UITYPE eType);
	virtual CGameObject*			Clone(void* pArg = nullptr);
	virtual void					Free(void);

};

END