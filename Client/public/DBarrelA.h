#pragma once
#include "Client_Defines.h"
#include "Installation.h"


class CDBarrelA : public CInstallation
{
private:
	CDBarrelA(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDBarrelA(const CDBarrelA& rhs);
	virtual ~CDBarrelA() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();


private:
	HRESULT SetUp_Model();


	INTERACTABLE*			m_pInteractable = nullptr;
public:
	void	Player_ToDamaged(_float fDamage);
	_bool	Collision_Attacked();
public:
	static  CDBarrelA* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);

	virtual void Free();
};

