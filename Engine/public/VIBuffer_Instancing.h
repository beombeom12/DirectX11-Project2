#pragma once
#include "VIBuffer.h"

//�ν��Ͻ��� �θ�Ŭ������ �߻�Ŭ������ ������ٰ��̴�.
//��ü�θ� ���� -> �߻�ȭ

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
	//Tick�� ���������Լ��� ����� �ش�.
	virtual  HRESULT Tick(_double TimeDelta) = 0;
	virtual HRESULT Render();

protected:
	_uint			m_iNumInstance = 0;				//�ϳ��� �ν��Ͻ��� ��� ������ ����� �����̳�.
	_uint			m_iInstanceStride = 0;			//�ν��Ͻ� ����
	_uint			m_iIndexCountPerInstance = 0;	//�׸��� �׷��������� �ʿ��� �ν��Ͻ� ->DrawIndex�� �Ҷ� ����Ѵ�.
	ID3D11Buffer*	m_pInstanceBuffer = nullptr;	//�ν��Ͻ� ����� ������ ���Ѵ�. 


public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

};

END