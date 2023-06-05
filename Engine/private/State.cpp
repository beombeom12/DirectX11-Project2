#include "..\public\State.h"

CState::CState(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CState::CState(const CState & rhs)
	: CComponent(rhs)
{
}

HRESULT CState::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CState::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CState::Tick(_double TimeDelta)
{
	auto CurState = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_CurStateName));

	for (auto Changer : m_mapChanger[m_CurStateName])
	{
		if (true == Changer.Changer_Func())
		{
			m_NextStateName = Changer.szNextState;
			break;
		}
	}

	if (lstrcmp(m_NextStateName, TEXT("")))
	{
		if (nullptr != CurState->second.State_End)
			CurState->second.State_End(TimeDelta);

		m_CurStateName = m_NextStateName;

		if (nullptr != m_mapState[m_CurStateName].State_Start)
			m_mapState[m_CurStateName].State_Start(TimeDelta);

		m_NextStateName = TEXT("");
	}
	else
	{
		if (nullptr != CurState->second.State_Tick)
			CurState->second.State_Tick(TimeDelta);
	}
}

CState & CState::Set_Root(const _tchar * szStateName)
{
	m_RootStateName = szStateName;

	return *this;
}

CState & CState::Add_State(const _tchar * szStateName)
{
	STATE eState;
	ZeroMemory(&eState, sizeof(eState));

	m_mapState.emplace(szStateName, eState);
	m_CurStateName = szStateName;

	return *this;
}

CState * CState::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CState*		pInstance = new CState(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CState::Clone(void * pArg)
{
	CState*		pInstance = new CState(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState::Free()
{
	__super::Free();
}
