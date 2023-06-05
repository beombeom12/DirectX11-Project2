#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_TRANSLATION, STATE_END };

	typedef struct tagTransformDesc
	{
		float		fSpeedPerSec;

		float		fRotationPerSec;

	}TRANSFORMDESC;


protected:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_float4x4 Get_WorldMatrix() const { return m_WorldMatrix; }

	_matrix Get_WorldMatrix_Inverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	_matrix Get_tWorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	_vector Get_State(STATE eState) const {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_vector Get_tState(STATE eID) const { return XMLoadFloat4x4(&m_WorldMatrix).r[eID]; }

	_float3 Get_Scaled() const {
		return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
	}

	CTransform& Set_tState(STATE eID, _fvector vSet)
	{
		_float4 vTemp;
		XMStoreFloat4(&vTemp, vSet);
		memcpy(&m_WorldMatrix.m[eID][0], &vTemp, sizeof(_float4));
		return *this;
	}

	CTransform&	Set_WorldMatrix(_float4x4 World)
	{
		m_WorldMatrix = World;
		return *this;
	}

	void Sets_WorldMatrix(_matrix WorldMatrix) {
		XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
	}

	void Set_State(STATE eState, _fvector vState) {
		_float4		vTmp;
		XMStoreFloat4(&vTmp, vState);
		memcpy(&m_WorldMatrix.m[eState][0], &vTmp, sizeof vTmp);
	}
	
	void Set_Scaled(STATE eState, _float fScale);
	void Set_Scaled(_float3 vScale);
	void Scaling(STATE eState, _float fScale);

	CTransform&		Set_tScaled(_float3 vScale);
	CTransform&		Set_tScaling(_float3 vScale);
	CTransform&		Set_Height(_float fHeight);
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void Go_Straight(_double TimeDelta, class CNavigation* pNavigation);
	void Camera_GoStraight(_double TimeDelta);
	void Camera_GoLeft(_double TimeDelta);
	void Camera_GoRight(_double TimeDelta);
	void Camera_GoBackWard(_double TimeDelta);
	void Go_Backward(_double TimeDelta, class CNavigation* pNavigation);
	void Go_Left(_double TimeDelta, class CNavigation* pNavigation);
	void Go_Right(_double TimeDelta, class CNavigation* pNavigation);
	void Go_Jump(_double TimeDelta, class CNavigation* pNavigation);
	void Turn(_fvector vAxis, _double TimeDelta); /* Dynamic */
	void Rotation(_fvector vAxis, _float fRadian); /* Static */

	void LookAt(_fvector vTargetPos);

	void Chase(_fvector vTargetPos, _double TimeDelta, _float fLimit = 0.7f);

	void NaviChase(_fvector vTargetPos, _double TimeDelta, class CNavigation* pNavigation,  _float fLimit = 0.7f);


	void Player_Chase(_fvector vTargetPos);


	//Test

	void Go_Up(_double TimeDelta);

public:
	HRESULT Bind_ShaderResource(class CShader* pShaderCom, const char* pConstantName);

private:
	_float4x4				m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};


END