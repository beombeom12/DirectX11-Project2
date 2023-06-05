#pragma once
#include "Base.h"

BEGIN(Engine)
class CCustomFont final : public CBase
{
private:
	CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCustomFont() = default;

public:
	HRESULT Initialize(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pText, const _float2& vPosition, _float fRadian, _float2 vScale, _fvector vColor);


private:
	ID3D11Device*		m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	//스프라이트폰트로 그리는것이다.
	SpriteFont*			m_pFont = nullptr;
	//실제 그리기 위해서는 SpriteBatch라는것이 필요하다.
	SpriteBatch*		m_pBatch = nullptr;

public:
	static CCustomFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath);
	virtual void Free() override;

};

END