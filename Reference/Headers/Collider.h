#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{


public:
	typedef struct tagColliderDesc
	{
		_float3		vCenter;
		_float3		vSize;
		_float3		vRotation;

	}COLLIDERDESC;

public:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eType);
	virtual HRESULT Initialize(void* pArg);

public:
	void Update(_fmatrix TransformMatrix);
	
public:
	_bool Collision(class CCollider* pTargetCollider);
	_vector Get_Center(void);
	_float	Get_Length(void);
#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

private:
	TYPE						m_eType = TYPE_END;
	BoundingBox*				m_pAABB_Original = nullptr;
	BoundingBox*				m_pAABB = nullptr;
	BoundingOrientedBox*		m_pOBB_Original = nullptr;
	BoundingOrientedBox*		m_pOBB = nullptr;
	BoundingSphere*				m_pSphere_Original = nullptr;
	BoundingSphere*				m_pSphere = nullptr;
	BoundingSphere*				m_pSphere_TargrtOriginal = nullptr;
	BoundingSphere*				m_pSpher_Target = nullptr;
	

public:
	_bool						m_isColl = false;

public:
	_bool	Get_CheckAble(void) { return m_bCheckAble; }
	void	Set_CheckAble(_bool Able) { m_bCheckAble = Able; }

private:
	_bool						m_bCheckAble = true;

#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>*			m_pBatch = nullptr;
	BasicEffect*									m_pEffect = nullptr;
	ID3D11InputLayout*								m_pInputLayout = nullptr;
	_float4											m_vColor;
#endif // _DEBUG

private:
	_matrix Remove_Rotation(_fmatrix TransformMatrix);

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType);
	virtual CComponent* Clone(void* pArg = nullptr) ;
	virtual void Free() override;
};

END