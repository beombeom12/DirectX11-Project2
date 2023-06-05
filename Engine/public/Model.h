#pragma once

#include "Component.h"

BEGIN(Engine)

// Model을 로드 할 때 텍스쳐도 같이 로드할거다 Model Component가 Texture Component를 가진 상태로 만들 것 이다
class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_matrix Get_PivotMatrix() const {
		return XMLoadFloat4x4(&m_PivotMatrix);
	}

	_float4x4 Get_PivotFloat4x4() const {
		return m_PivotMatrix;
	}
	_float4x4* Get_PivotFloat4x4s()  {
		return &m_PivotMatrix;
	}
	// 특정 뼈를 검색해서 찾는 함수
	class CBone* Get_BonePtr(const char* pBoneName);

public:
	void Set_AnimIndex(_uint iAnimIndex);
	_bool	isFinish(void);

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	void Play_Animation(_double TimeDelta, _double LerpSpeed = 0.2f, _double AnimSpeed = 1.3f, _bool bFinish = false);
	HRESULT Bind_Material(class CShader* pShader, _uint iMeshIndex, aiTextureType eType, const char* pConstantName);
	HRESULT Render(CShader* pShader, _uint iMeshIndex, const char* pBoneConstantName = nullptr, _uint iPassIndex = 0);

	class CAnimation* Get_Animation(_uint iIndex);

	_bool	Get_AnimFinish() {
		return m_bAnimFinish;
	}
public:
	const aiScene*				m_pAIScene = nullptr;

	Assimp::Importer			m_Importer;
	TYPE						m_eType = TYPE_END;

	// 하나의 모델은 교체가 가능한 여러개의 메시로 구성되어있다
	_uint						m_iNumMeshes = 0;
	vector<class CMesh*>		m_Meshes;

	_uint						m_iNumMaterials = 0;
	vector<MODELMATERIAL>		m_Materials;

	// 전체 뼈의 갯수
	_uint						m_iNumBones = 0;
	vector<class CBone*>		m_Bones;			// 이 뼈는 상속구조를 받는다

	_uint						m_iCurrentAnimIndex = 0;		// 클라이언트가 사용할 것 이다
	_uint						m_iNumAnimations = 0;
	vector<class CAnimation*>	m_Animations;

	_float4x4					m_PivotMatrix;
	////Lerp
	_bool	m_bChanged = false;
	_int	m_iNextAnimation = -1;
	_bool	m_bAnimFinish = false;
public:
	HRESULT Ready_Bones(aiNode* pNode, class CBone* pParent);
	HRESULT Ready_MeshContainers();
	HRESULT	Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_Animation();			

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END