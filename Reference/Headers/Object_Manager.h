#pragma once

#include "Base.h"
#include "GameObject.h"

/* ���ӳ��� �ʿ��� ��ü���� �� ����(CLayer)�� ���� ������ �����Ѵ�. */
/* ��ü���� ������ �����ϰ� �ִ� CLayer���� �����ϴ� Ŭ�����̴�. */
/* ��� ��ü���� ����(Tick, Late_Tick)�� ����Ѵ�. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager);
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_ComponentPtr(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex);

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Clear(_uint iLevelIndex);
public:
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Clone_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, const _tchar* pMyName = nullptr, void* pArg = nullptr, class CGameObject** ppGameObject = nullptr);
	CGameObject* Clone_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
	void Tick(_double TimeDelta);
	void Late_Tick(_double TimeDelta);

public:
	map<const _tchar*, class CLayer*>*					Get_MapLayer(_uint iLevelIndex);
	list<CGameObject*>*									Get_ObjectList(_uint iLevelIndex, const _tchar* pListTag);

private: /* ������ü���� ��Ƴ��´�. */
	map<const _tchar*, class CGameObject*>*			m_Prototypes = nullptr;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;

private: /* �纻��ü���� �����ϱ����� �����̳�. */
	map<const _tchar*, class CLayer*>*			m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>	LAYERS;

	_uint										m_iNumLevels = 0;


private:
	class CGameObject* Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END