#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CBone;
class CTransform;
END

BEGIN(Client)

class CCamera_Dynamic final : public CCamera
{


private:
	CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Dynamic(const CCamera_Dynamic& rhs);
	virtual ~CCamera_Dynamic() = default;

public:
	virtual HRESULT Initialize_Prototype()		override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual _uint Tick(_double TimeDelta)		override;
	virtual void Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()					override;


private:
	void Player_Chase(_double TimeDelta);
	void Player_BoneChase(_double TimeDelta);
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();
private:
	_bool						m_CameraMove = true;
	_float3						m_fDistance;
	_float4x4					m_SocketMatrix;
	vector<CGameObject*>		m_pPlayerParts;
	_float4						m_vPlayerLook = _float4(0.f, 0.f, 0.f, -1.f);
protected:
	CCamera::CAMERADESC			CameraDesc;


	_float4x4					m_CamPosition;


	CModel*						m_pModelCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
public:
	static CCamera_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END