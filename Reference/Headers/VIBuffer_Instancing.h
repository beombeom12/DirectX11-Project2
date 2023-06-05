#pragma once
#include "VIBuffer.h"

//인스턴싱의 부모클래스로 추상클래스로 만들어줄것이다.
//자체부모를 생성 -> 추상화

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract : public CVIBuffer
{
public:
	CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs);
	virtual ~CVIBuffer_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	//Tick을 순수가상함수로 만들어 준다.
	virtual  HRESULT Tick(_double TimeDelta) = 0;
	virtual HRESULT Render();

protected:
	_uint			m_iNumInstance = 0;				//하나의 인스턴스로 몇개의 도형을 만들어 낼것이냐.
	_uint			m_iInstanceStride = 0;			//인스턴스 보폭
	_uint			m_iIndexCountPerInstance = 0;	//그림을 그려내기위해 필요한 인스턴스 ->DrawIndex를 할때 사용한다.
	ID3D11Buffer*	m_pInstanceBuffer = nullptr;	//인스턴스 행렬의 집합을 말한다. 


public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

};

END