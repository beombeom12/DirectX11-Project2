#include "..\public\Channel.h"
#include "Model.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(aiNodeAnim * pAIChannel, CModel * pModel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_pBone = pModel->Get_BonePtr(m_szName);	
	Safe_AddRef(m_pBone);

	m_iNumKeyframes = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyframes = max(m_iNumKeyframes, pAIChannel->mNumPositionKeys);

	XMFLOAT3	vScale;
	XMFLOAT4	vRotation;
	XMFLOAT3	vPosition;

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME		KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(XMFLOAT3));
			KeyFrame.Time = pAIChannel->mScalingKeys[i].mTime;
		}

		if (i < pAIChannel->mNumRotationKeys)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.Time = pAIChannel->mRotationKeys[i].mTime;
		}

		if (i < pAIChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(XMFLOAT3));
			KeyFrame.Time = pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

void CChannel::Update_TransformMatrix(_double PlayTime)
{
	_vector		vScale;
	_vector		vRotation;
	_vector		vPosition;

	_matrix		TransformMatrix;

	if (PlayTime >= m_KeyFrames.back().Time)
	{
		vScale = XMLoadFloat3(&m_KeyFrames.back().vScale);
		vRotation = XMLoadFloat4(&m_KeyFrames.back().vRotation);
		vPosition = XMLoadFloat3(&m_KeyFrames.back().vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	else
	{
		while (PlayTime >= m_KeyFrames[m_iCurrentKeyFrameIndex + 1].Time)
		{
			++m_iCurrentKeyFrameIndex;
		}
		
		_double		Ratio = (PlayTime - m_KeyFrames[m_iCurrentKeyFrameIndex].Time) /
			(m_KeyFrames[m_iCurrentKeyFrameIndex + 1].Time - m_KeyFrames[m_iCurrentKeyFrameIndex].Time);

		_vector		vSourScale, vDestScale;
		_vector		vSourRotation, vDestRotation;
		_vector		vSourPosition, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vScale);
		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex].vRotation);
		vSourPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vPosition);

		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, static_cast<_float>(Ratio));
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, static_cast<_float>(Ratio));
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, static_cast<_float>(Ratio));
		vPosition = XMVectorSetW(vPosition, 1.f);
		if (!strcmp(m_szName, "Bone_War_Root"))
		{
			vPosition = XMVectorSet(0.f, 0.f, XMVectorGetZ(vPosition), 1.f);
		}
		//Grinner
		if (!strcmp(m_szName, "Bone_G_Root"))
		{
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}

		if (!strcmp(m_szName, "Bone_HellHound_Root"))
		{
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}
		if (!strcmp(m_szName, "Bone_Roller_Root"))
		{
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}
		if (!strcmp(m_szName, "Bone_Goblin_Root"))
		{
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}
		if (!strcmp(m_szName, "Bone_Moloch_Root"))
		{
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}
		if (!strcmp(m_szName, "Bone_Tidehunter_Root"))
		{
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}

	}
	
	TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBone->Set_TransformMatrix(TransformMatrix);
}

_bool CChannel::Update_LerbTransformMatrix(_double PlayTime, CChannel * Sour, CChannel * Dest, _double LerpSpeed, _bool bFinish)
{
	_vector vScale;
	_vector vPosition;
	_vector vRotation;

	_matrix TransformMatrix;

	if (-1 == m_iLerpFrameIndex)
	{
		while (PlayTime >= Sour->m_KeyFrames[m_iLerpFrameIndex + 1].Time)
		{
			m_iLerpFrameIndex++;



			if (m_iNumKeyframes <= m_iLerpFrameIndex + 1)
			{
				m_iLerpFrameIndex = -1;
				break;
			}
		}
	}
	_uint iSour;

	if (bFinish)
		iSour = m_iNumKeyframes - 1;
	else
		iSour = m_iLerpFrameIndex;

	_uint iDest = 0;					// 지금 실행하는 애니메이션은 무조건 0부터 시작

	if (Sour->m_KeyFrames.size() <= iSour)
		iSour = (_uint)Sour->m_KeyFrames.size() - 1;

	m_LerpRatio += LerpSpeed;			// 위쪽에있는 PlayTime에 맞게 증가해야하기떄문에 +를 해준다

										// 크기, 위치, 회전을 맞게끔 만들어준다
	_vector vSourScale, vDestScale;
	_vector vSourRotation, vDestRotation;
	_vector vSourPosition, vDestPosition;

	// 현재 애니메이션과 다음 애니메이션의 크기, 회전, 위치를 맞춰준다
	vSourScale = XMLoadFloat3(&Sour->m_KeyFrames[iSour].vScale);
	vSourRotation = XMLoadFloat4(&Sour->m_KeyFrames[iSour].vRotation);
	vSourPosition = XMLoadFloat3(&Sour->m_KeyFrames[iSour].vPosition);

	vDestScale = XMLoadFloat3(&Dest->m_KeyFrames[iDest].vScale);
	vDestRotation = XMLoadFloat4(&Dest->m_KeyFrames[iDest].vRotation);
	vDestPosition = XMLoadFloat3(&Dest->m_KeyFrames[iDest].vPosition);

	vScale = XMVectorLerp(vSourScale, vDestScale, static_cast<_float>(m_LerpRatio));
	vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, static_cast<_float>(m_LerpRatio));
	vPosition = XMVectorLerp(vSourPosition, vDestPosition, static_cast<_float>(m_LerpRatio));
	vPosition = XMVectorSetW(vPosition, 1.f);

	if (!strcmp(m_szName, "Bone_War_Root"))
	{
		vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	//Grinner
	if (!strcmp(m_szName, "Bone_G_Root"))
	{
		vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}
	//HellHound
	if (!strcmp(m_szName, "Bone_HellHound_Root"))
	{
		vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}
	//SteamRoller
	if (!strcmp(m_szName, "Bone_Roller_Root"))
	{
		vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}
	//Moloch
	if (!strcmp(m_szName, "Bone_Moloch_Root"))
	{
		vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	if (!strcmp(m_szName, "Bone_Tidehunter_Root"))
	{
		vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}



	TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBone->Set_TransformMatrix(TransformMatrix);

	
	if (1.f <= m_LerpRatio)
	{
		m_LerpRatio = 0.f;
		m_iLerpFrameIndex = -1;
		return true;
	}

	return false;
}

CChannel * CChannel::Create(aiNodeAnim * pAIChannel, CModel * pModel)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("Failed to Created : Channel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	Safe_Release(m_pBone);

	m_KeyFrames.clear();
}
