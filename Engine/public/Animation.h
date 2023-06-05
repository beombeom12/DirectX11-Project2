#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
public:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(aiAnimation* pAIAnimation, class CModel* pModel);
	_bool Update_Bones(_double TimeDelta, _double AnimSpeed);
public:
	//선형보관
	_bool	LerpTime(_double TimeDelta, CAnimation* NextAnim, _double LerpSpeed, _bool bFinish = false);

	_double Get_PlayRate() { return m_PlayTime / m_Duration; }

public:
	_bool	isFinish(void);

private:
	char						m_szName[MAX_PATH];
	_double						m_Duration = 0.f;
	_double						m_TickPerSecond;
	_double						m_PlayTime = 0.0;

	_bool						m_isFinished = false;
	_bool						m_isLooping = false;

	_uint						m_iNumChannels = 0;
	vector<class CChannel*>		m_Channels;

	_bool						m_AnimFinish = false;
	_bool						m_LerpFinish = false;
public:
	static CAnimation* Create(aiAnimation* pAIAnimation, class CModel* pModel);
	virtual void Free() override;
};

END