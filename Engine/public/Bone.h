#pragma once

#include "Base.h"

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const char* Get_Name() const {
		return m_szName;
	}

	_matrix Get_OffsetMatrix() {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_matrix Get_CombindMatrix() {
		return XMLoadFloat4x4(&m_CombindTransformMatrix);
	}

	void Set_OffsetMatrix(_float4x4 OffsetMatrix) {
		m_OffsetMatrix = OffsetMatrix;
	}

	void Set_TransformMatrix(_fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix);
	}

public:
	HRESULT Initialize(aiNode* pAINode, CBone* pParent);
	void Compute_CombindTransformMatrix();

private:
	char			m_szName[MAX_PATH];
	_float4x4		m_OffsetMatrix;
	_float4x4		m_TransformMatrix;
	_float4x4		m_CombindTransformMatrix;
	CBone*			m_pParent = nullptr;

private:
	_bool			m_isCloned = false;

public:
	static CBone* Create(aiNode* pAINode, CBone* pParent);
	virtual void Free() override;
};

END