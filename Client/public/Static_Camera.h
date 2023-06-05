#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CStatic_Camera final : public CCamera
{
private:
	CStatic_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStatic_Camera(const CStatic_Camera& rhs);
	virtual ~CStatic_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype()		override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual _uint Tick(_double TimeDelta)		override;
	virtual void Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()					override;

private:
	HRESULT SetUp_Components();

private:
	CCamera::CAMERADESC				m_CameraDesc;
	_bool							m_CameraMove = true;

public:
	static CStatic_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END