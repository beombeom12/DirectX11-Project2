#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(aiNodeAnim* pAIChannel, class CModel* pModel);
	void Update_TransformMatrix(_double PlayTime);
	void Reset_KeyFrameIndex() {
		m_iCurrentKeyFrameIndex = 0;
	}


	void Reset_LerpTime() { m_LerpRatio = 0.f; };
	void Reset_LerpIndex() { m_iLerpFrameIndex = -1; }
	void Reset_FrameIndex() { m_iCurrentKeyFrameIndex = 0; m_LerpRatio = 0.f; }


public:
	_bool Update_LerbTransformMatrix(_double PlayTime, CChannel* Sour, CChannel* Dest, _double LerpSpeed, _bool bFinish = false);
	char* Get_ChannelName() { return m_szName; }
	_uint Get_KeyFrame() { return m_iNumKeyframes; }
private:
	char				m_szName[MAX_PATH] = "";
	class CBone*		m_pBone = nullptr;

	_uint				m_iNumKeyframes = 0;
	vector<KEYFRAME>	m_KeyFrames;

	_uint				m_iCurrentKeyFrameIndex = 0;



	_uint				m_iLerpFrameIndex = -1;

	_double				m_Ratio = 0.f;
	_double				m_LerpRatio = 0.f;

public:
	static CChannel* Create(aiNodeAnim* pAIChannel, class CModel* pModel);
	virtual void Free() override;
};

END