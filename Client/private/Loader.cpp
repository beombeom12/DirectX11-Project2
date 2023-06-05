#include "stdafx.h"
#include "..\public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "LoadingImage.h"
#include "LoadingBackGround.h"


#include "Terrain.h"
#include "Sky.h"
#include "TestMap.h"
#include "Navigation_Cube.h"

//Player
#include "Player.h"
#include "Weapon.h"
#include "WarSkill.h"
//EffectInstancing
#include "Effect_Rect_Instancing.h"
#include "Effect_Point_Instancing.h"
#include "Effect.h"
#include "Effect_PlayerDust.h"
#include "Effect_Point_PlayerDust.h"
#include "Effect_Point_PlayerWindow.h"


//Object
//Desert
#include "DesertTerrain.h"
#include "DesertRoadA.h"
#include "DesertRoadB.h"
#include "DesertRockA.h"
#include "DesertRockB.h"
#include "CliffRock.h"
#include "LandingCrater.h"


//DesertRoad
#include "DesertRockPillar.h"
#include "DesertRockTall.h"
#include "DesertTallCollision.h"
#include "DesertTallLODA.h"
#include "DesertTallLODB.h"
#include "DesertTerrainRubble.h"
#include "DStair.h"
#include "DStairTwo.h"

#include "RoadCliffRock.h"
#include "RoadCliffRockA.h"
#include "RoadCliffRockB.h"
#include "RoadCliffRockC.h"



#include "DesertRoad.h"



//Enviroment
#include "DTree.h"
#include "DBarrel.h"
#include "DBirchA.h"
#include "DBirchB.h"
#include "DBirchC.h"
#include "ForliageA.h"
#include "DBarricade.h"
#include "DBarrelA.h"
#include "StairCase.h"

//Fortress
#include "Stone.h"
#include "FSideWall.h"
#include "FortressGate.h"
#include "FSideCliff.h"
#include "FSpikeA.h"
#include "FSpikeAHalf.h"
#include "FSpikeB.h"
#include "FWallA.h"
#include "FWallB.h"
#include "StairUp.h"
#include "FRoadMain.h"


//Monster
#include "Monster.h"
#include "Grinner.h"
#include "Steamroller.h"
#include "Goblin.h"
#include "Goblin_Sword.h"

#include "Moloch.h"
#include "Moloch_Sword.h"
#include "Mo_Stone.h"



#include "HellHound.h"
#include "SubMo.h"


//UI
#include "UI_Player.h"
#include "UI_Player_Tool.h"
#include "UI_HPBAR.h"
#include "UI_HP.h"


#include "UI_Player_SkillTool.h"
#include "UI_Skill1.h"
#include "UI_Skill2.h"



#include "UI_MonsterHP.h"



CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY LoadingThread(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_ForLogo();
		break;

	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlay();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	/* 로딩을 하기위한 추가적인 흐름을 만든다 (Thread).*/
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThread, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;





	return S_OK;
}

/* 로고를 위한 원형을 생성한다. */
HRESULT CLoader::Loading_ForLogo()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/so.png")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("버퍼를 로딩중입니다. "));


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));


	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중입니다. "));


	lstrcpy(m_szLoadingText, TEXT("객체원형을 생성중입니다. "));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext, ID_ENVIRONMENT))))
		return E_FAIL;






	lstrcpy(m_szLoadingText, TEXT("로딩끝. "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

/* 게임플레이를 위한 원형을 생성한다. */
HRESULT CLoader::Loading_ForGamePlay()
{
	ENGINE_OPEN;




	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));
	
	////Terrain
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/noise%d.dds"), 2))))
		return E_FAIL;
	////Brush
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_RuneCircle_Belial_Debuff_01.png"), 1))))
		return E_FAIL;
	//Filter
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.dds"), 1))))
		return E_FAIL;
	///* For.Prototype_Component_Texture_Sky */

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Texture7.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Explosion */
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Explosion"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
		return E_FAIL;




	//UI
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_style2_portraitWar.png")))))
		return E_FAIL;





	//UI_HPBAR
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_PlayerHPBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_style2_HealthBase2.png")))))
		return E_FAIL;
	//UI
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_PlayerHP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/HUD_UnitFrame_Sub_HealthMeterFill.png")))))
		return E_FAIL;

	//UISkillTool
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_SkillTool"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/HUD_AbilityWheel_WrathBG.png")))))
		return E_FAIL;

	//UISKILL
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Skill1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_AbilityIcon_HorseMode_WarMelee_128.png")))))
		return E_FAIL;

	//MonsterUI
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_MonsterHP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/theHUDui_13.png")))))
		return E_FAIL;




	//UISKILL2

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Skill2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_WrathPower_War_4Main.png")))))
		return E_FAIL;


	//Effect
	//snow
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_PlayerDust"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Effect/T_Dust_Particle_D_COLOR.png")))))
		return E_FAIL;

	//dust
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Dust"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Effect/T_Dust_Particle_D.png")))))
		return E_FAIL;


	//Window Paritcle
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_WindowParticle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Effect/VFX_gen_simpleSpark_SRGB.png")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("버퍼를 로딩중입니다. "));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	////VIBuffer_Square
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Square"),
		CVIBuffer_Square::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Dust_Instancing"),
		CVIBuffer_Point_Dust::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//EffectInstancing
	//if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Instancing"),
	//	CVIBuffer_Rect_Instancing::Create(m_pDevice, m_pContext, 100))))
	//	return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point_Instancing */
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instancing"),
		CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext, 30))))
		return E_FAIL;




	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_PlayerDust_Instancing"),
		CVIBuffer_Point_PlayerDust::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Window_Instancing"),
	//	CVIBuffer_Point_Window::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));

	_matrix			PivotMatrix = XMMatrixIdentity();

	//Player
	PivotMatrix = XMMatrixScaling(0.03f,0.03f, 0.03f) *  XMMatrixRotationY(XMConvertToRadians(270.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RunPerson"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/War/SoSexy14.fbx", PivotMatrix))))
		return E_FAIL;

	//Sword
	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationX(XMConvertToRadians(270.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sword"),
		(CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Sword/Sword.fbx", PivotMatrix)))))
		return E_FAIL;

	//WarSkill
	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationX(XMConvertToRadians(270.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_WarSkill"),
		(CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Sword/Skill/WarSkill1.fbx", PivotMatrix)))))
		return E_FAIL;

	//scaraHulk
	//PivotMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/ScarabHulk/Testing.fbx", PivotMatrix))))
	//	return E_FAIL;

	//Grinner
	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Grinner"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Grinner/Grinner2.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_HellHound"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/TideHunter/TideHunter.fbx", PivotMatrix))))
		return E_FAIL;



	//SteamRoller
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Steamroller"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/SteamRoller/untitled2.fbx", PivotMatrix))))
		return E_FAIL;



	///////////Goblin
	//Goblin
	PivotMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Gobline"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Goblin/Gobline03.fbx", PivotMatrix))))
		return E_FAIL;

	//Sword
	PivotMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) ;
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Goblin_Sword"),
	(CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Goblin/GoblinSword/GoblineSword.fbx", PivotMatrix)))))
	return E_FAIL;



	//Moloch_Stone
	PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f);
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_StoneMoloch"),
		(CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/MolochStone/MolochMagicA.fbx", PivotMatrix)))))
		return E_FAIL;

	//Moloch_Sword
	PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f)* XMMatrixRotationZ(XMConvertToRadians(230.f)) * XMMatrixRotationX(XMConvertToRadians(-30.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Moloch_Sword"),
		(CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Moloch1/Sword/So.fbx", PivotMatrix)))))
		return E_FAIL;


	//Moloch
	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f);
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Moloch"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Moloch1/Moloch2.fbx", PivotMatrix))))
		return E_FAIL;



	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SubMo"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/SubMo/Grub.fbx", PivotMatrix))))
		return E_FAIL;







	//OBJECT
	//Desert
	//TerrainA
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Terrain"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/TerrainA/DesertTerrain4.fbx", PivotMatrix))))
		return E_FAIL;
	//RockA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DesertRockA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertRockA/DesertRockA.fbx", PivotMatrix))))
		return E_FAIL;
	//RockB
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DesertRockB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertRockB/DesertRockB.fbx", PivotMatrix))))
		return E_FAIL;
	//CliffRock
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_CliffRock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertCliff/DesertCliff.fbx", PivotMatrix))))
		return E_FAIL;
	//LandingCrater
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_LandingCrater"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertLanding/DesertLandingCreater.fbx", PivotMatrix))))
		return E_FAIL;

	//DesertRoadA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DesertRoadA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertRoad/DesertRoadA.fbx", PivotMatrix))))
		return E_FAIL;



	//Desert         Road

	//RockTallPerPoly
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DesertRoadB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertRockPerPoly/RockTallPerPoly.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DesertRockPillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertRockPillar/RockPillar.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DesertRockTall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertRockTall/Tall.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DesertCollision"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertTallCollision/RockTallCollision.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DeserrTallLODA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertTallLODA/RockTallLODA.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DesertTallLODB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertTallLODB/RockTallLODB.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DesertTerrainRubble"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DesertTerrainRubble/DesertTerrainRubble.fbx", PivotMatrix))))
		return E_FAIL;


	//DesertStair

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DStair"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/Stair/Stair.fbx", PivotMatrix))))
		return E_FAIL;
	//DesertStairTwo
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DStairTwo"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/Stair2/Stair2.fbx", PivotMatrix))))
		return E_FAIL;

	//StairCase
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_StairCase"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/DStairCase/StairCase.fbx", PivotMatrix))))
		return E_FAIL;


	//RoadCliff

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_RoadCliffRock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/RoadCliffRock/CliffRock.fbx", PivotMatrix))))
		return E_FAIL;
	//CliffRockA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_RoadCliffRockA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/RoadCliffRockA/CliffRockA.fbx", PivotMatrix))))
		return E_FAIL;
	//CliffRockB
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_RoadCliffRockB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/RoadCliffRockB/CliffRockB.fbx", PivotMatrix))))
		return E_FAIL;
	//CliffRockC
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_RoadCliffRockC"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertFix/RoadCliffRockC/CliffRockC.fbx", PivotMatrix))))
		return E_FAIL;














	//DesertEnviroMent
	
	//Tree
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DTree"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertEnviroment/DTree/DTree.fbx", PivotMatrix))))
		return E_FAIL;
	
	//DBirchA
	
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DBirchA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertEnviroment/DBirchA/BirchTreeA.fbx", PivotMatrix))))
		return E_FAIL;

	//DBirchB
	
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DBirchB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertEnviroment/DBirchB/DBirchB.fbx", PivotMatrix))))
		return E_FAIL;
	
	//DBirchC

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DBirchC"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertEnviroment/DBirchC/DBirchC.fbx", PivotMatrix))))
		return E_FAIL;

	//ForliageA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_ForliageA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertEnviroment/FoliageA/Foliage.fbx", PivotMatrix))))
		return E_FAIL;
	
	//Barrel
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DBarrel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertEnviroment/DBarrel/DBarrel.fbx", PivotMatrix))))
		return E_FAIL;

	//BarrelA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DBarrelA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertEnviroment/DBarrelA/Barrel.fbx", PivotMatrix))))
		return E_FAIL;

	//Barricade
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DBarricade"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertEnviroment/DBarricade/BarricadeA.fbx", PivotMatrix))))
		return E_FAIL;




	//DesertRoad
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_DesertRoad"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertMap/DesertRoad/DesertRoad.fbx", PivotMatrix))))
		return E_FAIL;



	//Fortress

	//Stone
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Stone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DesertMap/RockA/RockA.fbx", PivotMatrix))))
		return E_FAIL;
	//FSideWall
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FSideWall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fortress/FSideWall/FSideWall.fbx", PivotMatrix))))
		return E_FAIL;
	//FortressGate
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FortressGate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fortress/FWallGate/FWallGate.fbx", PivotMatrix))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FSideCliff"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fortress/FSideCliff/FSideCliff.fbx", PivotMatrix))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FSpikeA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fortress/FSpikeA/FSpikeA.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FSpikeHalf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fortress/FSpikeAHalf/FSpikeAHalf.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FSpikeB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fortress/FSpikeB/FSpikeBHalf.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FWalLA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fortress/FWallA/FWallA.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FWallB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fortress/FWallB/FWallB.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FStairUp"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fortress/FStairUp/StairUp.fbx", PivotMatrix))))
		return E_FAIL;
	
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FRoadMain"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fortress/FRoadMain/FStoneRoad.fbx", PivotMatrix))))
		return E_FAIL;
	
	
	lstrcpy(m_szLoadingText, TEXT("콜라이더를 로딩중입니다. "));
	// For.Prototype_Component_Collider_AABB
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, TYPE_AABB))))
		return E_FAIL;

	// For.Prototype_Component_Collider_OBB
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, TYPE_OBB))))
		return E_FAIL;

	// For.Prototype_Component_Collider_SPHERE
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, TYPE_SPHERE))))
		return E_FAIL;




	// For.Prototype_Component_Collider_SPHERE_TARGET
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"),
		CCollider::Create(m_pDevice, m_pContext, TYPE_SPHERE_TARGET))))
		return E_FAIL;

	//For Sword

	//if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB_TARGET"),
	//	CCollider::Create(m_pDevice, m_pContext, TYPE_SWORD))))
	//	return E_FAIL;





	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중입니다. "));
	//Terrain
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	//VtxModel
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	//State
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
		CState::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//Cube
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	//Square
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Square"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Vertex_Square.hlsl"), VTXSQUARE_DECLARATION::Elements, VTXSQUARE_DECLARATION::iNumElements))))
		return E_FAIL;


	//AnimModel
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;
	//Effect
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxRectInstance.hlsl"), VTXRECTINSTANCE_DECLARATION::Elements, VTXRECTINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl"), VTXPOINT_DECLARATION::Elements, VTXPOINT_DECLARATION::iNumElements))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxDustPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxDustPointInstance.hlsl"), VTXPOINT_DECLARATION::Elements, VTXPOINT_DECLARATION::iNumElements))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxWindowPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxWindowPointInstance.hlsl"), VTXPOINT_DECLARATION::Elements, VTXPOINT_DECLARATION::iNumElements))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointPlayerDust"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointPlayerDust.hlsl"), VTXPOINT_DECLARATION::Elements, VTXPOINT_DECLARATION::iNumElements))))
		return E_FAIL;


	//Navigation
	lstrcpy(m_szLoadingText, TEXT("네비게이션을 로딩중입니다. "));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation.dat")))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("객체원형을 생성중입니다. "));

	//Terrain
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext, ID_ENVIRONMENT))))
		return E_FAIL;

	//Sky
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext, ID_SKY))))
		return E_FAIL;

	//Effect
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect"),
		CEffect::Create(m_pDevice, m_pContext, ID_EFFECT))))
		return E_FAIL;
	
	
	//TestMap

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TestMap"),
		CTestMap::Create(m_pDevice, m_pContext, ID_ENVIRONMENT))))
		return E_FAIL;

	//Square
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Square"),
		CNavigation_Cube::Create(m_pDevice, m_pContext, ID_ENVIRONMENT))))
		return E_FAIL;



	// For.Prototype_GameObject_Player
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext, ID_PLAYER))))
		return E_FAIL;






	//Monster
	//ScarabHulk
	//if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster"),
	//	CMonster::Create(m_pDevice, m_pContext, ID_MONSTER))))
	//	return E_FAIL;





	
	
	//Effect
	/* For.Prototype_GameObject_Effect_Point_Instancing */
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Point_Instancing"),
		CEffect_Point_Instancing::Create(m_pDevice, m_pContext, ID_EFFECT))))
		return E_FAIL;


	//Dust
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_PlayerPoint_Instancing"),
		CEffect_Point_PlayerDust::Create(m_pDevice, m_pContext, ID_EFFECT))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_TestPoint_Instancing"),
		CEffect_PlayerDust::Create(m_pDevice, m_pContext, ID_EFFECT))))
		return E_FAIL;


	//if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_WindowTest_Instancing"),
	//	CEffect_Point_PlayerWindow::Create(m_pDevice, m_pContext, ID_EFFECT))))
	//	return E_FAIL;







	//Goblin Spot
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin2"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin3"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin4"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;


	//GB F2

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin5"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin6"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin7"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin8"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin9"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin10"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Goblin11"),
		CGoblin::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;



	//SubMo Spot


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SubMo"),
		CSubMo::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SubMo2"),
		CSubMo::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SubMo3"),
		CSubMo::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SubMo4"),
		CSubMo::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SubMo5"),
		CSubMo::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SubMo6"),
		CSubMo::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SubMo7"),
		CSubMo::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SubMo8"),
		CSubMo::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;
	//SteamRollder Spot


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Steamroller"),
		CSteamroller::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Steamroller2"),
		CSteamroller::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Steamroller3"),
		CSteamroller::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;




	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Steamroller4"),
		CSteamroller::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Steamroller5"),
		CSteamroller::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Steamroller6"),
		CSteamroller::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	//HellHound Spot

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HellHound"),
		CHellHound::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HellHound2"),
		CHellHound::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HellHound3"),
		CHellHound::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HellHound4"),
		CHellHound::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HellHound5"),
		CHellHound::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;


	//Grinner Spot
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Grinner"),
		CGrinner::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;




	//Moloch Spot
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Moloch"),
		CMoloch::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;









	// For.Prototpye_GameObject_Weapon(Sword)
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon"),
		CWeapon::Create(m_pDevice, m_pContext, ID_WEAPON))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_WarSkill"),
		CWarSkill::Create(m_pDevice, m_pContext, ID_WEAPON))))
		return E_FAIL;


	//MolochStone
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_StoneMo"),
		CMo_Stone::Create(m_pDevice, m_pContext, ID_WEAPON))))
		return E_FAIL;

	//Goblin Sword
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_GoblinSword"),
		CGoblin_Sword::Create(m_pDevice, m_pContext, ID_WEAPON))))
		return E_FAIL;
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MolochSword"),
		CMoloch_Sword::Create(m_pDevice, m_pContext, ID_WEAPON))))
		return E_FAIL;

	//OBJECT

	// DesertTerrainA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DesertTerrainA"),
		CDesertTerrain::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	//DesertRockA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DesertRockA"),
		CDeserRockA::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	//DesertRockB
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DesertRockB"),
		CDesertRockB::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	//DesertCliff
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CliffRock"),
		CCliffRock::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	//LandingCrater
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Landing"),
		CLandingCrater::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;
	//DesertRoadA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DesertRoadA"),
		CDesertRoadA::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;
	//DesertRoadB
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DesertRoadB"),
		CDesertRoadB::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;


	//DesertPillar
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_RockPillar"),
		CDesertRockPillar::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_RockTall"),
		CDesertRockTall::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TallCollision"),
		CDesertTallCollision::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TallLODA"),
		CDesertTallLODA::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TALLLODB"),
		CDesertTallLODB::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;
	
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TerrainRubble"),
		CDesertTerrainRubble::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stair"),
		CDStair::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_StairTwo"),
		CDStairTwo::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	//DStairCase
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DStairCase"),
		CStairCase::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_RoadCliffRock"),
		CRoadCliffRock::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_RoadCliffRockA"),
		CRoadCliffRockA::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_RoadCliffRockB"),
		CRoadCliffRockB::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_RoadCliffRockC"),
		CRoadCliffRockC::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	//Desert EnviroMents
	//Tree
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DTree"),
		CDTree::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	//DBirchA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DBirchA"),
		CDBirchA::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	//DBirchB
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DBirchB"),
		CDBirchB::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;
	
	//DBirchC
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DBirchC"),
		CDBirchC::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	//ForliageA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ForliageA"),
		CForliageA::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;


	//Barrel
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DBarrel"),
		CDBarrel::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;
	//BarrelA
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DBarrelA"),
		CDBarrelA::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;


	//Barricade
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DBarricade"),
		CDBarricade::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;


	//Fortress
	// Stone
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stone"),
		CStone::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;
	//SideWall
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FSideWall"),
		CFSideWall::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;
	//FortressGate
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FortressGate"),
		CFortressGate::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;


	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FSpikeA"),
		CFSideCliff::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FSpikeAHalf"),
		CFSpikeA::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FSpikeB"),
		CFSpikeAHlaf::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FWallA"),
		CFSpikeB::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FWallB"),
		CFWallA::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;



	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FStairUp"),
		CStairUp::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FRoadMain"),
		CFRoadMain::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	//isEndingRoad
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_DesertRoad"),
		CFWallB::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;





	//UI
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_UI_Player"),
		CUI_Player::Create(m_pDevice, m_pContext, ID_UI, UI_IMAGE))))
		return E_FAIL;




	//UI_BAR
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_UI_PlayerHPBAR"),
		CUI_HPBAR::Create(m_pDevice, m_pContext, ID_UI, UI_IMAGE))))
		return E_FAIL;

	//UI
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_UI_PlayerHP"),
		CUI_HP::Create(m_pDevice, m_pContext, ID_UI, UI_IMAGE))))
		return E_FAIL;
	
	//SKill Tool 
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_UI_Player_SkillTool"),
		CUI_Player_SkillTool::Create(m_pDevice, m_pContext, ID_UI, UI_IMAGE))))
		return E_FAIL;

	//UI_Skill1
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_UI_Player_Skill1"),
		CUI_Skill::Create(m_pDevice, m_pContext, ID_UI, UI_IMAGE))))
		return E_FAIL;

	//UI_Skill2
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_UI_Player_Skill2"),
		CUI_Skill2::Create(m_pDevice, m_pContext, ID_UI, UI_IMAGE))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_UI_MonsterHP"),
		CUI_MonsterHP::Create(m_pDevice, m_pContext, ID_UI, UI_IMAGE))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("로딩끝. "));

	m_isFinished = true;



	ENGINE_CLOSE;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteObject(m_hThread);
	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
