#pragma once

#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagGameObjectDesc
	{
		CTransform::TRANSFORMDESC	TransformDesc;

	}GAMEOBJECTDESC;

public:
	class CComponent*	Find_Component(const _tchar* ComponentTag);

protected:
	CGameObject(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	static const _tchar*		m_pTransformComTag;	

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

public:
	class CComponent*						Get_Component(const _tchar* ComponentTag);
	map<const _tchar*, class CComponent*>*	Get_MapCompoent() { return &m_mapComponent; }

	OBJECT_ID	Get_ID() const { return m_eObjectID; }

protected:	
	ID3D11Device*			m_pDevice = nullptr;	
	ID3D11DeviceContext*	m_pContext = nullptr;	

	OBJECT_ID				m_eObjectID = ID_END;
	GAMEOBJECTDESC			m_GameObjectDesc;

	_bool					m_isClone = false;

protected:
	const _tchar*	m_pMyName = TEXT("");	

public:
	void			Set_MyName(const _tchar* pMyName) { m_pMyName = pMyName; }
	const _tchar*	Get_MyName(void) { return m_pMyName; }
	class CTransform*	Get_Transform() { return m_pTransformCom; }
protected:
	const _tchar*	m_pProtoName = TEXT("");		

public:
	void			Set_ProtoName(const _tchar* pProtoName) { m_pProtoName = pProtoName; }
	const _tchar*	Get_ProtoName() { return m_pProtoName; }
	_float					m_fCamDistance = { 0.0 };
protected:
	map<const _tchar*, class CComponent*>	m_mapComponent;		
	class CComponent*	Find_Proto(const _tchar* pProtoTag);


public:
	void				Kill_Obj() { m_bDead = true; }

protected:
	class CTransform*								m_pTransformCom = nullptr;	
	_bool				m_bDead = false;

protected:	
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, class CComponent** ppOut, void* pArg = nullptr);

	void Compute_CamDistance();
public:	
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END