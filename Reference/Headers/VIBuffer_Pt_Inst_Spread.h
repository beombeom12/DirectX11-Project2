#pragma once

#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Pt_Inst_Spread final : public CVIBuffer_Instancing
{
public:
	typedef struct tagVIBufPtInstSpreadDesc
	{
		_uint iNumInstance;
		_uint iMinSpeed, iMaxSpeed;
		_float2 vSize;
		_bool bUseDownY;
		_float fDownRangeMaxY, fDownRangeMinY;
		_float fDownValueMaxY, fDownValueMinY;
	} DESC;

private:
	struct SpreadData
	{
		_bool bDownFlag;
		_float fSpeed;
		_float3 vDirection;
		_float fDownRangeY, fDownValueY;
	};

public:
	CVIBuffer_Pt_Inst_Spread(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Pt_Inst_Spread(const CVIBuffer_Pt_Inst_Spread& rhs);
	virtual ~CVIBuffer_Pt_Inst_Spread() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Tick(_double TimeDelta) override;
	virtual HRESULT Render();
	void Set_RandomData();

private:
	SpreadData* m_pSpreadDatas = nullptr;
	SpreadData* m_pSpreadDatasOriginal = nullptr;

	DESC m_Desc;

public:
	static CVIBuffer_Pt_Inst_Spread* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
	void Reset(_bool bSpeedChange = false);
};

END