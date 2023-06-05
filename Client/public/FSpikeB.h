#pragma once
#include "Client_Defines.h"
#include "Installation.h"


class CFSpikeB : public CInstallation
{
private:
	CFSpikeB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSpikeB(const CFSpikeB& rhs);
	virtual ~CFSpikeB() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();


private:
	HRESULT SetUp_Model();

public:
	static  CFSpikeB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);

	virtual void Free();
};

