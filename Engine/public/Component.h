#pragma once

#include "Base.h"

/* �츮�� ������ ����� ������Ʈ(�ؽ���, Ʈ������, ����,�ε�������, ��) ���� �θ� �Ǵ� Ŭ�����̴�. */
/* Ŭ���� �������� �̿��Ͽ� �ϳ��� ���ø������̳ʾȿ� ��Ƶε��� �������Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

#ifdef _DEBUG
public:
	virtual HRESULT Render() { return S_OK; };
#endif //
protected:
	_bool						m_isCloned = false;

public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

};

END