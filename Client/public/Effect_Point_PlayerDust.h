#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Point_PlayerDust;
END

BEGIN(Client)

class CEffect_Point_PlayerDust final : public CGameObject
{
private:
	CEffect_Point_PlayerDust(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Point_PlayerDust(const CEffect_Point_PlayerDust& rhs);
	virtual ~CEffect_Point_PlayerDust() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual _uint Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;


	void	Shoot_Point();

private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Point_PlayerDust*	m_pVIBufferCom = nullptr;

	_float						m_fFrame;


	_uint						m_iTextureIndex = 0;
	_uint						m_iTextureX = 0;

	_uint						m_iTextureY = 0;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();

public:
	static CEffect_Point_PlayerDust* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END