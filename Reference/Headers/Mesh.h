#pragma once

#include "VIBuffer.h"
#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eType, aiMesh* pAIMesh, class CModel* pModel);
	virtual HRESULT Initialize(void* pArg) override;

public:
	void SetUp_MeshBone(class CModel* pModel);
	void SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);	

private:
	aiMesh*					m_pAIMesh = nullptr;
	CModel::TYPE			m_eType;
	
	_uint					m_iMaterialIndex = 0;
	_uint					m_iNumBones = 0;
	vector<class CBone*>	m_Bones;

private:
	HRESULT Ready_VertexBuffer_NonAnimModel(aiMesh* pAIMesh, class CModel* pModel);
	HRESULT Ready_VertexBuffer_AnimModel(aiMesh* pAIMesh, class CModel* pModel);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, aiMesh* pAIMesh, class CModel* pModel);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free();
};

END
