#pragma once

#include "Component.h"


BEGIN(Engine)

class ENGINE_DLL CState : public CComponent
{
private:
	typedef struct tagState
	{
		std::function<void(_double)>		State_Start = nullptr;
		std::function<void(_double)>		State_Tick = nullptr;
		std::function<void(_double)>		State_End = nullptr;

	}STATE;

	typedef struct tagChanger
	{
		const _tchar*						szNextState = TEXT("");
		std::function<_bool(void)>			Changer_Func = nullptr;

	}CHANGER;

private:
	CState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CState(const CState& rhs);
	virtual ~CState() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg = nullptr);
	void	Tick(_double TimeDelta);

public:
	CState& Set_Root(const _tchar* szStateName);
	CState& Add_State(const _tchar* szStateName);
	CState& Finish_Setting(void) { m_CurStateName = m_RootStateName; return *this; }

public:
	template<typename T>
	CState& Init_Start(T* obj, void (T::*memFunc)(_double))
	{
		const auto itr = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_CurStateName));
		assert(itr != m_mapState.end());

		itr->second.State_Start = [obj, memFunc](_double TimeDelta)
		{
			(obj->*memFunc)(TimeDelta);
		};
		
		return *this;
	}

	template<typename T>
	CState& Init_Tick(T* obj, void (T::*memFunc)(_double))
	{
		const auto iter = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_CurStateName));
		assert(iter != m_mapState.end());

		iter->second.State_Tick = [obj, memFunc](_double TimeDelta)
		{
			(obj->*memFunc)(TimeDelta);
		};

		return *this;
	}

	template<typename T>
	CState& Init_End(T* obj, void (T::*memFunc)(_double))
	{
		const auto iter = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_CurStateName));
		assert(iter != m_mapState.end());

		iter->second.State_End = [obj, memFunc](_double TimeDelta)
		{
			(obj->*memFunc)(TimeDelta);
		};

		return *this;
	}

	template<typename T>
	CState& Init_Changer(const _tchar* szNextState, T* obj, _bool(T::*memFunc)())
	{
		const auto iter = find_if(m_mapChanger.begin(), m_mapChanger.end(), CTag_Finder(m_CurStateName));

		CHANGER Changer;
		ZeroMemory(&Changer, sizeof(CHANGER));

		Changer.szNextState = szNextState;
		Changer.Changer_Func = [obj, memFunc]()
		{
			return (obj->*memFunc)();
		};

		if (iter == m_mapChanger.end())
			m_mapChanger.emplace(m_CurStateName, list<CHANGER>{Changer});
		else
			iter->second.push_back(Changer);

		return *this;
	}

private:
	map<const _tchar*, STATE>	m_mapState;
	const _tchar*				m_RootStateName = TEXT("");
	const _tchar*				m_CurStateName = TEXT("");
	const _tchar*				m_NextStateName = TEXT("");

private:
	map<const _tchar*, list<CHANGER>>		m_mapChanger;	//

public:
	static CState* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free();
};

END