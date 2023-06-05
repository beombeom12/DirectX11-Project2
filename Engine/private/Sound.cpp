#include "..\public\Sound.h"
#include "..\public\Transform.h"

_float CSound::m_fMasterVolume = 0.5f;

CSound::CSound()
{
}

void CSound::Free()
{
	Clear_SoundDesc();
	FMOD_Sound_Release(m_pSound);
}

CSound* CSound::Create(FMOD_SOUND* pSound)
{
	CSound* pInstance = new CSound;
	if (pInstance->Initialize(pSound))
	{
		MSG_BOX("Failed to Created : CSound");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CSound::Initialize(FMOD_SOUND* pSound)
{
	m_pSound = pSound;	
	ZeroMemory(&m_SoundDesc, sizeof(SOUND_DESC));

	return S_OK;
}

void CSound::Tick(double TimeDelta, FMOD_CHANNEL* pChannel)
{
	if (m_SoundDesc.bIs3D == false) 
		return;

	_vector vStart = m_SoundDesc.pStartTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vEnd = m_SoundDesc.pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
	_float fLength = XMVectorGetX(XMVector4Length(vStart - vEnd));

	m_fVolume[VOLUME_3D_RANGE] = (1.f - (fLength / m_SoundDesc.fRange)) * (fLength < m_SoundDesc.fRange);
		
	UpdateUseVolume(pChannel);
}

void CSound::Set_Volume(FMOD_CHANNEL* pChannel, _float fVolume)
{
	if (pChannel == nullptr) return;

	m_fVolume[VOLUME_DEFAULT] = fVolume > 1.f ? 1.f : fVolume < 0.f ? 0.f : fVolume;
	
	UpdateUseVolume(pChannel);
}

void CSound::UpdateUseVolume(FMOD_CHANNEL* pChannel)
{	
	if (pChannel)
	{
		m_fVolume[VOLUME_USE] = m_fVolume[VOLUME_DEFAULT] * m_fMasterVolume * m_fVolume[VOLUME_3D_RANGE];
		FMOD_Channel_SetVolume(pChannel, m_fVolume[VOLUME_USE]);
	}	
}

void CSound::Set_SoundDesc(SOUND_DESC& SoundDesc)
{
	Clear_SoundDesc();

	memcpy(&m_SoundDesc, &SoundDesc, sizeof(SOUND_DESC));
	
	_vector vStart = m_SoundDesc.pStartTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vEnd = m_SoundDesc.pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
	_float fLength = XMVectorGetX(XMVector4Length(vStart - vEnd));

	m_fVolume[VOLUME_3D_RANGE] = (1.f - (fLength / m_SoundDesc.fRange)) * (fLength < m_SoundDesc.fRange);

	Safe_AddRef(m_SoundDesc.pStartTransform);
	Safe_AddRef(m_SoundDesc.pTargetTransform);
}

void CSound::Clear_SoundDesc()
{
	Safe_Release(m_SoundDesc.pStartTransform);
	Safe_Release(m_SoundDesc.pTargetTransform);

	ZeroMemory(&m_SoundDesc, sizeof(SOUND_DESC));
}