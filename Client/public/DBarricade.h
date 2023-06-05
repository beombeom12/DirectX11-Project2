#pragma once
#include "Client_Defines.h"
#include "Installation.h"


class CDBarricade : public CInstallation
{
private:
	CDBarricade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDBarricade(const CDBarricade& rhs);
	virtual ~CDBarricade() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();


private:
	HRESULT				SetUp_Model();
	HRESULT				Player_Damaged(_float fDamage);
	_bool				Collision_Attacked();
public:
	INTERACTABLE*			m_pInteractable = nullptr;

public:
	static  CDBarricade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);

	virtual void Free();
};

