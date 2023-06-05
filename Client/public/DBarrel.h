#pragma once
#include "Client_Defines.h"
#include "Installation.h"
#include "GameObject.h"


BEGIN(Client)


class CDBarrel : public CInstallation
{

	enum DBARREL {DBARREL_HIT, DBARREL_EXPLORE, DBARREL_END};
private:
	CDBarrel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDBarrel(const CDBarrel& rhs);
	virtual ~CDBarrel() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();



private:
	HRESULT SetUp_Model();
	_bool	Collision_Attacked();
	HRESULT Player_ToDamaged(_float fDamage);
public:
	INTERACTABLE*		m_pInteractable = nullptr;
public:
	static  CDBarrel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);

	virtual void Free();
};
END