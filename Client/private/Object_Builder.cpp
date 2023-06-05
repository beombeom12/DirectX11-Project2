#include "stdafx.h"
#include "..\public\Object_Builder.h"
#include "GameObject.h"
#include "Terrain.h"
#include "DesertTerrain.h"
CObject_Builder::CObject_Builder(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CImGui_Console(pDevice, pContext)
{
}

HRESULT CObject_Builder::Initialize(void)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_bCreate = false;

	return S_OK;
}

void CObject_Builder::Show_Window(void)
{
	ImGui::Begin("Object_Create");
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
		ImGui::InputText("Name", m_szObjectName, MAX_PATH);

		static int Object = OBJECT_END;
		if (ImGui::CollapsingHeader("Desert", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("DesertTerrainA", &Object, OBJECT_TERRAINA);
			ImGui::RadioButton("DLanding", &Object, OBJECT_LANDING);
			ImGui::SameLine();
			ImGui::RadioButton("DRoadA", &Object, OBJECT_DESERTROADA);
			ImGui::SameLine();
			ImGui::RadioButton("DRockA", &Object, OBJECT_DROCKA);
			ImGui::SameLine();
			ImGui::RadioButton("DRockB", &Object, OBJECT_DROCKB);
			ImGui::SameLine();
			ImGui::RadioButton("CliffRock", &Object, OBJECT_CLIFFROCK);

	
		}
		if (ImGui::CollapsingHeader("DesertRoad", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("DRPerPoly", &Object, OBJECT_DESERTROADB);
			ImGui::SameLine();
			ImGui::RadioButton("DPillar", &Object, OBJECT_ROCKPILLAR);
			ImGui::SameLine();
			ImGui::RadioButton("DTall", &Object, DESERT_ROCKTALL);
			ImGui::SameLine();
			ImGui::RadioButton("DTallColl", &Object, DESERT_TALLCOLLISION);
			ImGui::SameLine();
			ImGui::RadioButton("LODA", &Object, DESERT_TALLLODA);

			ImGui::RadioButton("LODB", &Object, DESERT_TALLLODB);
			ImGui::SameLine();
			ImGui::RadioButton("Rubble", &Object, DESERT_TERRAINRUBBLE);
			ImGui::SameLine();
			ImGui::RadioButton("Stair", &Object, DESERT_DSTAIR);
			ImGui::SameLine();
			ImGui::RadioButton("StairTwo", &Object, DESERT_DSTAIRTWO);
			ImGui::SameLine();
			ImGui::RadioButton("StairCase", &Object, DESERT_DSTAIRCASE);
			
			
			ImGui::RadioButton("RoadCliff", &Object, DESERT_ROADCLIFF);
			ImGui::SameLine();
			ImGui::RadioButton("RoadCliffA", &Object, DESERT_ROADCLIFFA);
			ImGui::SameLine();
			ImGui::RadioButton("RoadCliffB", &Object, DESERT_ROADCLIFFB);
			ImGui::SameLine();
			ImGui::RadioButton("RoadCliffC", &Object, DESERT_ROADCLIFFC);

		}
		if (ImGui::CollapsingHeader("Fortress", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("FStairUp", &Object, OBJECT_FSTAIRUP);
			ImGui::SameLine();
			ImGui::RadioButton("FRoadMain", &Object, OBJECT_FROADMAIN);
			ImGui::RadioButton("DRoad", &Object, OBJECT_DROAD);
			ImGui::SameLine();
			ImGui::RadioButton("Stone", &Object, OBJECT_ROCKA);
			ImGui::SameLine();
			ImGui::RadioButton("FSideWall", &Object, OBJECT_FSIDEWALL);
			ImGui::SameLine();
			ImGui::RadioButton("FSideCliff", &Object, OBEJCT_FSIDECLIFF);
			ImGui::SameLine();
			ImGui::RadioButton("FortressGate", &Object, OBJECT_FORTRESSGATE);

			ImGui::RadioButton("FSpikeA", &Object, OBJECT_FSPIKEA);
			ImGui::SameLine();
			ImGui::RadioButton("FSpikeHalf", &Object, OBJECT_FSPIKEAHALF);
			ImGui::SameLine();
			ImGui::RadioButton("FSpikeB", &Object, OBJECT_FSPIKEB);
			ImGui::SameLine();
			ImGui::RadioButton("FWallA", &Object, OBJECT_FWALLA);
			ImGui::SameLine();
			ImGui::RadioButton("FWallB", &Object, OBJECT_FWALLB);
		}

		if (ImGui::CollapsingHeader("EnviroMents", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("DTree", &Object, OBJECT_DTREE);
			ImGui::SameLine();
			ImGui::RadioButton("DBirchA", &Object, OBJECT_DBIRCHA);
			ImGui::SameLine();
			ImGui::RadioButton("DBirchB", &Object, OBJECT_DBIRCHB);
			ImGui::SameLine();
			ImGui::RadioButton("DBirchC", &Object, OBJECT_DBIRCHC);
			ImGui::SameLine();
			ImGui::RadioButton("ForliageA", &Object, OBJECT_FORLIAGEA);


			ImGui::RadioButton("DBarrel", &Object, OBJECT_DBARREL);
			ImGui::SameLine();
			ImGui::RadioButton("DBarrelA", &Object, OBJECT_DBARRELA);
			ImGui::SameLine();
			ImGui::RadioButton("DBarricade", &Object, OBJECT_DBARRICADE);
	
		}


		if (ImGui::CollapsingHeader("Create_A", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("NO", &Object, OBJECT_END);
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

void CObject_Builder::Create_Cube(_uint eType)
{
	ENGINE_OPEN;

	_float fCollision = m_vPickingPos.x + m_vPickingPos.y + m_vPickingPos.z;

	if (m_bCreate && (fCollision > 3.f) && ENGINE->Mouse_Down(DIM_LB))
	{
		if (!strcmp(m_szObjectName, ""))
		{
			ImGui::Checkbox("Cube_Create", &m_bCreate);
			ENGINE_CLOSE;
			ImGui::End();
			return;
		}

		_tchar* szName = new _tchar[MAX_PATH];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szObjectName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));
		TcharList.push_back(szName);

		_float4 fPos = _float4(m_vPickingPos.x, m_vPickingPos.y , m_vPickingPos.z, 1.f);

		switch (eType)
		{
		case OBJECT_TERRAINA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DesertTerrainA"), szName);
			break;
		}
		case OBJECT_DROCKA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DesertRockA"), szName);
			break;
		}
		case OBJECT_DROCKB:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DesertRockB"), szName);
			break;
		}
		case OBJECT_DESERTROADA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DesertRoadA"), szName);
			break;
		}
		case OBJECT_DESERTROADB:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DesertRoadB"), szName);
			break;
		}
		case OBJECT_CLIFFROCK:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_CliffRock"), szName);
			break;
		}
		case OBJECT_LANDING:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Landing"), szName);
			break;
		}
		//DesertRoad

		case OBJECT_ROCKPILLAR:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_RockPillar"), szName);
			break;
		}
		case DESERT_ROCKTALL:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_RockTall"), szName);
			break;
		}
		case DESERT_TALLCOLLISION:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_TallCollision"), szName);
			break;
		}
		case DESERT_TALLLODA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_TallLODA"), szName);
			break;
		}
		case DESERT_TALLLODB:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_TALLLODB"), szName);
			break;
		}
		case DESERT_TERRAINRUBBLE:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_TerrainRubble"), szName);
			break;
		}
		case DESERT_DSTAIR:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Stair"), szName);
			break;
		}
		case DESERT_DSTAIRTWO:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_StairTwo"), szName);
			break;
		}
		case DESERT_DSTAIRCASE:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DStairCase"), szName);
			break;
		}
		case DESERT_ROADCLIFF:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_RoadCliffRock"), szName);
			break;
		}
		case DESERT_ROADCLIFFA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_RoadCliffRockA"), szName);
			break;
		}

		case DESERT_ROADCLIFFB:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_RoadCliffRockB"), szName);
			break;
		}

		case DESERT_ROADCLIFFC:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_RoadCliffRockC"), szName);
			break;
		}


		//Fortress
		case OBJECT_ROCKA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Stone"), szName);
			break;
		}
		case OBJECT_DROAD:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DesertRoad"), szName);
			break;
		}
		case OBJECT_FSIDEWALL:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FSideWall"), szName);
			break;
		}
		case OBJECT_FORTRESSGATE:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FortressGate"), szName);
			break;
		}
		case OBEJCT_FSIDECLIFF:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FSideCliff"), szName);
			break;
		}
		case OBJECT_FSPIKEA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FSpikeA"), szName);
			break;
		}
		case OBJECT_FSPIKEAHALF:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FSpikeAHalf"), szName);
			break;
		}
		case OBJECT_FSPIKEB:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FSpikeB"), szName);
			break;
		}
		case OBJECT_FWALLA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FWallA"), szName);
			break;
		}
		case OBJECT_FWALLB:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FWallB"), szName);
			break;
		}
		case OBJECT_FROADMAIN:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FRoadMain"), szName);
			break;
		}
		//Enviroments
		case OBJECT_DTREE:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DTree"), szName);
			break;
		}
		case OBJECT_DBIRCHA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DBirchA"), szName);
			break;
		}
		case OBJECT_DBIRCHB:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DBirchB"), szName);
			break;
		}
		case OBJECT_DBIRCHC:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DBirchC"), szName);
			break;
		}
		case OBJECT_FORLIAGEA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_ForliageA"), szName);
			break;
		}
		case OBJECT_DBARREL:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DBarrel"), szName);
			break;
		}

		case OBJECT_DBARRELA:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DBarrelA"), szName);
			break;
		}
		case OBJECT_DBARRICADE:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_DBarricade"), szName);
			break;
		}
		case OBJECT_FSTAIRUP:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FStairUp"), szName);
			break;
		}
		case OBJECT_END:
			break;
		}

		if (nullptr != ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), szName))
		{
			static_cast<CTransform*>(
				ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), szName)
				->Get_Component(CGameObject::m_pTransformComTag))
				->Set_tScaling(m_vScale).Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&fPos));	
		}

		m_bCreate = false;
		strcpy_s(m_szObjectName, "");
	}

	ENGINE_CLOSE;
}

CObject_Builder * CObject_Builder::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CObject_Builder* pInstance = new CObject_Builder(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create : CObject_Builder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Builder::Free()
{
	__super::Free();
}
