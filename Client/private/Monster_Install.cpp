#include "stdafx.h"
#include "..\public\Monster_Install.h"
#include "GameObject.h"
#include "Terrain.h"
CMonster_Install::CMonster_Install(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CImGui_Console(pDevice, pContext)
{
}

HRESULT CMonster_Install::Initialize(void)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_bCreate = false;

	return S_OK;
}

void CMonster_Install::Create_Cube(_uint eType)
{

	ENGINE_OPEN;

	_float fCollision = m_vPickingPos.x + m_vPickingPos.y + m_vPickingPos.z;

	if (m_bCreate && (fCollision > 3.f) && ENGINE->Mouse_Down(DIM_LB))
	{
		if (!strcmp(m_szMonsterName, ""))
		{
			ImGui::Checkbox("Monster_Create", &m_bCreate);
			ENGINE_CLOSE;
			ImGui::End();
			return;
		}

		_tchar* szName = new _tchar[MAX_PATH];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szMonsterName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));
		TcharList.push_back(szName);

		_float4 fPos = _float4(m_vPickingPos.x, m_vPickingPos.y, m_vPickingPos.z, 1.f);



		switch (eType)
		{

		case MONSTER_GOBLINE:
			{
				ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Install_Monster"), TEXT("Prototype_GameObject_Goblin"), szName);
				break;
			}
		case MONSTER_END:
			{	
			break;
			}
		}
		if (nullptr != ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Install_Monster"), szName))
		{
			static_cast<CTransform*>(
				ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Install_Monster"), szName)
				->Get_Component(CGameObject::m_pTransformComTag))
				->Set_tScaling(m_vScale).Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&fPos));
		}

		m_bCreate = false;
		strcpy_s(m_szMonsterName, "");


	}
	ENGINE_CLOSE;

}

void CMonster_Install::Show_Window(void)
{

	ImGui::Begin("Monster_Create");
	void BeginFrame();

	ImGui::PushItemWidth(200);


	ENGINE_OPEN;
	if (ENGINE->Get_NowLevel() == LEVEL_GAMEPLAY)
	{
		m_vPickingPos = static_cast<CTerrain*>(ENGINE->Get_GameObject(
			LEVEL_GAMEPLAY,
			TEXT("Layer_BackGround"),
			TEXT("Prototype_GameObject_Terrain")))
			->Get_Pick();

		ImGui::InputFloat4("MousePos", &m_vPickingPos.x);
		ImGui::InputFloat3("Scale", &m_vScale.x);
		ImGui::InputText("Name", m_szMonsterName, MAX_PATH);


		static int Object = MONSTER_END;
		if (ImGui::CollapsingHeader("Monster", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("Monster_Gobline", &Object, MONSTER_GOBLINE);
		}



		if (ImGui::CollapsingHeader("Create_A", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("NO", &Object, MONSTER_END);
		}


		Create_Cube(Object);
	}
	else
	{
		ImGui::Text("FAILING");
	}

	ImGui::Checkbox("Create", &m_bCreate);

	ENGINE_CLOSE;

	ImGui::End();

}

CMonster_Install * CMonster_Install::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMonster_Install* pInstance = new CMonster_Install(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create : CMonster_Install");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Install::Free()
{
	__super::Free();
}
