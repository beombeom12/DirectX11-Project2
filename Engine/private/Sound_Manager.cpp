#include "Sound_Manager.h"

IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager()
{	
}

void CSound_Manager::Free()
{
	Stop_All();

	for (auto& Pair : m_Sounds) 
	{
		delete[] Pair.first;;
		Safe_Release(Pair.second);
	}
	m_Sounds.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

HRESULT CSound_Manager::Reserve_Manager(const char* pFilePath, _uint iNumManualChannels)
{
	if (pFilePath == nullptr) 
		return E_FAIL;

	m_iNumManualChannels = iNumManualChannels;
	for (_uint i = 0; i < MAX_CHANNEL_COUNT; i++) 
		m_Channels[i].second = NO_USE_CHNNEL;
	
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	FMOD_System_Init(m_pSystem, MAX_CHANNEL_COUNT, FMOD_INIT_NORMAL, NULL);
	Load_SoundFile(pFilePath);

	return S_OK;
}

HRESULT CSound_Manager::Load_SoundFile(const char* pFilePath)
{	
	char szPath[MAX_PATH] = "";
	strcat_s(szPath, pFilePath);
	strcat_s(szPath, "*.*");

	_finddata_t FindData;
	intptr_t handle = _findfirst(szPath, &FindData);
	if (handle == -1) 
		return E_FAIL;

	char szFullPath[MAX_PATH] = "";
	char szCurPath[MAX_PATH] = "";	
	strcpy_s(szCurPath, pFilePath);
	
	int iResult = 0;
	while (iResult != -1) 
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, FindData.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT Ret = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);
		if (Ret == FMOD_OK)
		{
			int iStrLen = (int)strlen(FindData.name) + 1;
			_tchar* pSoundKey = new _tchar[iStrLen];
			ZeroMemory(pSoundKey, sizeof(_tchar) * iStrLen);
			
			MultiByteToWideChar(CP_ACP, 0, FindData.name, iStrLen, pSoundKey, iStrLen);
			
			CSound *pSoundObj = CSound::Create(pSound);
			if (pSound == nullptr)
				return E_FAIL;

			m_Sounds.emplace(pSoundKey, pSoundObj);
		}

		iResult = _findnext(handle, &FindData);
	}

	FMOD_System_Update(m_pSystem);
	_findclose(handle);

	return S_OK;
}

void CSound_Manager::Play_Sound(const _tchar *pSoundKey, _float fVolume, _bool bIsBGM, _int iManualChannelIndex)
{
	if (m_iNumManualChannels == MAX_CHANNEL_COUNT)
		return;

	SOUNDS::iterator Pair = find_if(m_Sounds.begin(), m_Sounds.end(), CTag_Finder(pSoundKey));
	if (Pair == m_Sounds.end())
		return;

	FMOD_BOOL bPlayFlag = FALSE;
	_int iPlayIndex = -1;
	if (iManualChannelIndex == -1)
	{
		for (_uint i = m_iNumManualChannels; i < MAX_CHANNEL_COUNT; i++)
		{
			if (FMOD_Channel_IsPlaying(m_Channels[i].first, &bPlayFlag))
			{
				iPlayIndex = i;
				break;
			}
		}
	}
	else
		iPlayIndex = iManualChannelIndex;

	if (iPlayIndex >= MAX_CHANNEL_COUNT || iPlayIndex < 0)
		return;

	FMOD_CHANNEL* pUseChannel;
	CSound* pUseSound = Pair->second;

	FMOD_System_PlaySound(m_pSystem, pUseSound->GetSoundPtr(), nullptr, FALSE, &pUseChannel);
	if (bIsBGM)
		FMOD_Channel_SetMode(pUseChannel, FMOD_LOOP_NORMAL);

	pUseSound->Set_Volume(pUseChannel, fVolume);
	FMOD_System_Update(m_pSystem);

	m_Channels[iPlayIndex] = { pUseChannel, pUseSound };

}

void CSound_Manager::Stop_SoundAuto()
{
	if (m_iNumManualChannels == MAX_CHANNEL_COUNT) 
		return;

	FMOD_BOOL bPlayFlag = FALSE;
	for (_uint i = 0; i < MAX_CHANNEL_COUNT; i++) 
	{
		FMOD_Channel_IsPlaying(m_Channels[i].first, &bPlayFlag);
		if (bPlayFlag == FALSE && m_Channels[i].second != NO_USE_CHNNEL)
		{
			FMOD_Channel_Stop(m_Channels[i].first);
			m_Channels[i].second = NO_USE_CHNNEL;
		}
	}
}

void CSound_Manager::Stop_Sound(_uint iManualChannelIndex)
{
	if (m_iNumManualChannels == 0 || iManualChannelIndex >= MAX_CHANNEL_COUNT)
		return;

	FMOD_Channel_Stop(m_Channels[iManualChannelIndex].first);
	m_Channels[iManualChannelIndex].second = NO_USE_CHNNEL;
}

void CSound_Manager::Set_Volume(_uint iManualChannelIndex, _float fVolume)
{
	if (m_iNumManualChannels == 0 || iManualChannelIndex >= MAX_CHANNEL_COUNT) 
		return;

	if (m_Channels[iManualChannelIndex].second == NO_USE_CHNNEL) 
		return;

	m_Channels[iManualChannelIndex].second->Set_Volume(m_Channels[iManualChannelIndex].first, fVolume);	
}

void CSound_Manager::Set_MasterVolume(_float fVolume)
{
	fVolume = fVolume > 1.f ? 1.f : fVolume < 0.f ? 0.f : fVolume;
	CSound::Set_MasterVolume(fVolume);
	for (int i = 0; i < m_iNumManualChannels; i++)
	{
		if (m_Channels[i].second)
			m_Channels[i].second->UpdateUseVolume(m_Channels[i].first);
	}
}

void CSound_Manager::Set_SoundDesc(const _tchar *pSoundKey, CSound::SOUND_DESC& SoundDesc)
{
	SOUNDS::iterator Pair = find_if(m_Sounds.begin(), m_Sounds.end(), CTag_Finder(pSoundKey));
	if (Pair == m_Sounds.end())
		return;

	Pair->second->Set_SoundDesc(SoundDesc);
}

void CSound_Manager::Stop_All()
{
	for (_uint i = 0; i < MAX_CHANNEL_COUNT; ++i)
	{
		FMOD_Channel_Stop(m_Channels[i].first);
		m_Channels[i].second = NO_USE_CHNNEL;
	}	
}

void CSound_Manager::Tick(double TimeDelta)
{
	for (auto& Pair : m_Sounds)
	{
		Pair.second->Tick(TimeDelta, nullptr);
	}
		
	CHANNEL* pChannel = nullptr;
	for (int i = 0; i < MAX_CHANNEL_COUNT; i++)
	{
		pChannel = &m_Channels[i];
		if (pChannel->second)
		{
			pChannel->second->UpdateUseVolume(m_Channels[i].first);
			
			FMOD_BOOL bPlayFlag = FALSE;
			FMOD_Channel_IsPlaying(m_Channels[i].first, &bPlayFlag);
			if (bPlayFlag == FALSE)
			{
				FMOD_Channel_Stop(pChannel->first);
				pChannel->second = NO_USE_CHNNEL;
			}
		}
	}
}