#pragma once

#include "GameObject.h"
#include "Bone.h"
BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public CGameObject::GAMEOBJECTDESC
	{
		_float4		vEye, vAt, vUp;				// View
		_float		FOV, Aspect, NearZ, FarZ;	// Projection
		_float		m_MouseMovement;			// 마우스 감도


		_float4x4*	pPivotMatrix;
		CBone*		pSocket;
		CTransform*	pTransform;
	}CAMERADESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	class CPipeLine*		m_pPipeLine = nullptr;
protected:

	CAMERADESC				m_CameraDesc;

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END