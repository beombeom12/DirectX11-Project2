#pragma once
#include "Base.h"

#include <io.h>
#include "Fmod/fmod.h"
#include "Fmod/fmod.hpp"
// #pragma comment(lib, "fmodL_vc.lib")

BEGIN(Engine)

class CSound : public CBase
{
public:			
	enum VOLUME_TYPE { VOLUME_DEFAULT, VOLUME_3D_RANGE, VOLUME_USE, VOLUME_END };

	typedef struct ENGINE_DLL tag3DSoundDesc
	{
		// 3D 사운드로 사용 시 true로 설정해야 한다
		_bool bIs3D; 
		// 사운드의 시작 위치 정보 Transform 컴포넌트
		class CTransform* pStartTransform;
		// 볼륨은 TargetTransform에 위치를 기준으로 변경 된다
		class CTransform* pTargetTransform;
		// 사운드가 들리는 거리 pTargetTransform에 위치와 pStartTransform에 위치가 Range 값보다 멀어지면 볼륨이 0
		_float fRange;
	} SOUND_DESC;
	
private:
	FMOD_SOUND* m_pSound = nullptr;	
	SOUND_DESC m_SoundDesc;

	static _float m_fMasterVolume;
	_float m_fVolume[VOLUME_END] = { 0.5f, 1.f, 0.5f };
		
protected:
	CSound();
	virtual ~CSound() = default;

public:	
	virtual void Free() override;
	static CSound* Create(FMOD_SOUND* pSound);
	HRESULT Initialize(FMOD_SOUND* pSound);
	void Tick(double TimeDelta, FMOD_CHANNEL* pChannel);
	FMOD_SOUND* GetSoundPtr() { return m_pSound; }

	void Set_SoundDesc(SOUND_DESC& SoundDesc);
	_float GetVolume(VOLUME_TYPE eType) { return m_fVolume[eType]; }
	void Set_Volume(FMOD_CHANNEL* pChannel, _float fVolume);
	static void Set_MasterVolume(_float fVolume) { m_fMasterVolume = fVolume; }
	void UpdateUseVolume(FMOD_CHANNEL* pChannel);
	void Clear_SoundDesc();
};
END
