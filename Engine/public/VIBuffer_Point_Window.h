#pragma once
#include "VIBuffer_Instancing.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Point_Window final : public  CVIBuffer_Instancing
{



public:
	typedef struct PointWindow
	{
		_uint iNumInstance;
		_uint iMinSpeed, iMaxSpeed;
		_float2 fSize;
		_bool	bUse;
		_float fRangeMaxY, fRangeMinY;
		_float fValueMaxY, fValueMinY;
	}WINDOW;

private:
	struct Data
	{
		_bool bDonwFlag;
		_float fSpeed;
		_float3 vDirection;
		_float fDownRangeY, fDownValueY;
	};


public:
	CVIBuffer_Point_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Window(const CVIBuffer_Point_Window& rhs);
	virtual ~CVIBuffer_Point_Window() = default;


private:
	WINDOW 		m_DustDesc;
	
	Data*		m_pData = nullptr;
	Data*		m_pDataReal = nullptr;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Tick(_double TimeDelta) override;
	virtual HRESULT Render();

	void Reset(_bool bSpeedChange = false);




public:
	static CVIBuffer_Point_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END