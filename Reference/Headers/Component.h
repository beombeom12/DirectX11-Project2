#pragma once

#include "Base.h"

/* 우리가 앞으로 사용할 컴포넌트(텍스쳐, 트랜스폼, 정점,인덱스버퍼, 모델) 들의 부모가 되는 클래스이다. */
/* 클래스 다형성을 이용하여 하나의 템플릿컨테이너안에 담아두도록 저으이한다. */

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