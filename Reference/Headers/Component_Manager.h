#pragma once

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"



#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Rect_Instancing.h"
#include "VIBuffer_Point_Instancing.h"
#include "VIBuffer_Point_PlayerDust.h"
#include "VIBuffer_Point_Dust.h"
#include "VIBuffer_Point_Window.h"



#include "Collider.h"
#include "State.h"
#include "Navigation.h"


/* 다양한 컴포넌트(원형)들을 담고 있는다. -> 사본컴포넌트들은 사용해야할 객체들이 보유한다. */
/* 사본을 생성하여 리턴한다. */
BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

private:
	_uint											m_iNumLevels = 0;

	/* 레벨별로 원형 컴포넌트(텍스쳐, 메시모델 etc )들을 보관하겠다. */
	map<const _tchar*, class CComponent*>*			m_pPrototypes = nullptr;
	typedef map<const _tchar*, class CComponent*>	PROTOTYPES;

private:
	CComponent* Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag);


public:
	virtual void Free() override;
};

END