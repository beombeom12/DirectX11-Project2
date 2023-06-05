#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CLoadingBackGround : public CUI
{
public:
	CLoadingBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoadingBackGround(const CLoadingBackGround& rhs);
	virtual~CLoadingBackGround() = default;


public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID, _uint eUIType);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render(void);
private:
	HRESULT					Setup_Component(void);
private:
	CUI::UIDESC			m_UIDesc;
public:
	static	CLoadingBackGround*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID, UITYPE eUIType);
	virtual CGameObject*			Clone(void* pArg = nullptr);
	virtual void					Free(void);

};
END
