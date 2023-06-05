#pragma once

#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Pt_Inst_Explosion final : public CVIBuffer_Instancing
{
public:
	typedef struct tagVIBufPtInstSnowDesc
	{
		_uint iNumInstances;
		_float fRange;
		_float fMinTime, fMaxTime;
		_float2 vMaxSize, vMinSize;
		_uint iNumSprite;

		_uint iNumSpreadInstances;
		_float fMaxSpreadSizeOffset, fMinSpreadSizeOffset;
		_float fRangeOffset;
	} DESC;

private:
	struct InstanceData
	{
		_float fTime, fFrame;		
		_float2 vSize;
		_uint index;
	};

	struct SpreadData
	{
		_float4 vDir;
		_float fSpeed;
	};

private:
	InstanceData* m_pInstanceData = nullptr;
	InstanceData* m_pInstanceDataOriginal = nullptr;

	SpreadData* m_pSpreadData = nullptr;

	DESC m_Desc;

public:
	CVIBuffer_Pt_Inst_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Pt_Inst_Explosion(const CVIBuffer_Pt_Inst_Explosion& rhs);
	virtual ~CVIBuffer_Pt_Inst_Explosion() = default;

	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Tick(_double TimeDelta) override;
	virtual HRESULT Render();

	static CVIBuffer_Pt_Inst_Explosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	void Reset();
};

END