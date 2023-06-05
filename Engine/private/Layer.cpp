#include "..\public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Initialize()
{
	return S_OK;
}

void CLayer::Tick(_double TimeDelta)
{
	/*for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Tick(TimeDelta);
	}*/

	for (auto& iter : m_ObjectList)
	{
		_uint iResult = 0;

		list<CGameObject*>::iterator iter;
		
		for (iter = m_ObjectList.begin(); iter != m_ObjectList.end();)
		{
			iResult = (*iter)->Tick(TimeDelta);
			if (iResult == -1)
			{
				int iA = 0;
			}

			if (iResult & 0x80000000)
			{
				Safe_Release(*iter);
				iter = m_ObjectList.erase(iter);
			}
			else
				++(iter);
		}
	}

}

void CLayer::Late_Tick(_double TimeDelta)
{
	for (auto& iter : m_ObjectList)
	{
		if (nullptr != iter)
			iter->Late_Tick(TimeDelta);
	}
}

CComponent * CLayer::Get_ComponentPtr(const _tchar * pComponentTag, _uint iIndex)
{
	if (iIndex >= m_ObjectList.size())
		return nullptr;

	auto	iter = m_ObjectList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	if (iter == m_ObjectList.end())
		return nullptr;

	return (*iter)->Find_Component(pComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjectList.push_back(pGameObject);

	return S_OK;
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLayer::Free()
{
	/*for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();*/

	for_each(m_ObjectList.begin(), m_ObjectList.end(), CDeleteObj());
	m_ObjectList.clear();
}
