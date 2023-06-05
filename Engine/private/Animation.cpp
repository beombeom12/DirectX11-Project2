#include "..\public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
	: m_isLooping(true)
{
}

HRESULT CAnimation::Initialize(aiAnimation * pAIAnimation, CModel * pModel)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_Duration = pAIAnimation->mDuration;

	m_TickPerSecond = pAIAnimation->mTicksPerSecond;

	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim*		pAINodeAnim = pAIAnimation->mChannels[i];

		CChannel*		pChannel = CChannel::Create(pAINodeAnim, pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

_bool CAnimation::Update_Bones(_double TimeDelta, _double AnimSpeed)
{
	if (true == m_isFinished &&
		false == m_isLooping)
	{
		return false;
	}

	if (false == m_LerpFinish)
		m_PlayTime += TimeDelta * m_TickPerSecond * AnimSpeed;

	if (m_PlayTime >= m_Duration)
	{
		m_PlayTime = 0.f;
		m_isFinished = true;
		return true;
	}

	if (m_isLooping && m_isFinished)
	{
		for (auto iter : m_Channels)
			iter->Reset_FrameIndex();
	}

	for (auto iter : m_Channels)
	{
		iter->Update_TransformMatrix(m_PlayTime);
	}

	if (m_isFinished = true)
		m_isFinished = false;

	return false;
}

_bool CAnimation::LerpTime(_double TimeDelta, CAnimation * NextAnim, _double LerpSpeed, _bool bFinish)
{
	if (m_LerpFinish)
	{
		m_PlayTime = 0.f;

		for (auto iter : m_Channels)
		{
			iter->Reset_FrameIndex();
			iter->Reset_LerpIndex();
		}

		m_LerpFinish = false;

		return false;
	}
	else
	{
		for (auto Current : m_Channels)
		{
			for (auto Next : NextAnim->m_Channels)
			{

				if (!strcmp(Current->Get_ChannelName(), Next->Get_ChannelName()))
				{
					m_LerpFinish = Current->Update_LerbTransformMatrix(m_PlayTime, Current, Next, LerpSpeed, m_isFinished);
					break;
				}
			}
		}

		return true;
	}

	return true;
}

_bool CAnimation::isFinish(void)
{
	_bool bResult = m_AnimFinish;

	m_AnimFinish = false;

	return bResult;
}

CAnimation * CAnimation::Create(aiAnimation * pAIAnimation, CModel* pModel)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation::Free()
{
	for (auto iter : m_Channels)
		Safe_Release(iter);

	m_Channels.clear();
}