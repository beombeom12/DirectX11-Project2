#pragma once
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class ENGINE_DLL CUI : public CGameObject
{
public:
	typedef struct tagUIDescription : public GAMEOBJECTDESC
	{
		_float4		Size, Position;
	}UIDESC;


protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID, _uint eUIType);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	UITYPE					m_eType;
	CUI::UIDESC			m_UIDesc;

protected:
	_float4x4				m_matView, m_matProj;
	HRESULT					Setup_ShaderResources(void);
	CRenderer*				m_pRenderer = nullptr;
	CShader*				m_pShader = nullptr;
	CVIBuffer_Rect*			m_pBuffer = nullptr;
	CTexture*				m_pTexture = nullptr;


public:
	virtual CGameObject*	Clone(void* pArg = nullptr)PURE;
	virtual void			Free(void);

};

END