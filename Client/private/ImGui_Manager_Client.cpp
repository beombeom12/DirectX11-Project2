#include "stdafx.h"
#include "..\public\ImGui_Manager_Client.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "ImGuizmo.h"
#include "ImSequencer.h"

#include "GameInstance.h"
#include "Level_Loading.h"	
#include "Level_GamePlay.h"
#include "Transform.h"


_bool	CImGui_Manager_Client::Show_IO_Test = false;
_bool	CImGui_Manager_Client::Show_Slider_Test = false;
_bool	CImGui_Manager_Client::Show_TreeNode_Test = false;
_bool	CImGui_Manager_Client::Show_ListBox_Test = false;
_bool	CImGui_Manager_Client::Show_Level_Status = true;
_bool	CImGui_Manager_Client::Show_Math_Test = false;
_bool	CImGui_Manager_Client::Show_Phsx_Text = false;
_bool	CImGui_Manager_Client::Show_UI_Setting = true;


IMPLEMENT_SINGLETON(CImGui_Manager_Client)


CImGui_Manager_Client::CImGui_Manager_Client()
{
}

HRESULT CImGui_Manager_Client::Ready_Imgui(HWND hWnd, ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	io.ConfigViewportsNoAutoMerge = true;
	io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CImGui_Manager_Client::Tick(_double TimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	CreateMainWindow();
	IO_Test(m_pDevice, m_pContext);
	Slider_Test(m_pDevice, m_pContext);
	//TreeNode_Test(m_pDevice, m_pContext);
	Listbox_Test(m_pDevice, m_pContext);
	Level_Status(m_pDevice, m_pContext);
}

void CImGui_Manager_Client::Late_Tick(_double TimeDelta)
{
}

HRESULT CImGui_Manager_Client::Render()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	return S_OK;
}

void CImGui_Manager_Client::CreateMainWindow()
{
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	if (ImGui::Button("Clear"))
	{
		MSG_BOX("¤©¤·¤»¤»");
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void CImGui_Manager_Client::IO_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	if (false == Show_IO_Test)
		return;

	ImGui::Begin("IO_TEST", &Show_IO_Test);

	void BeginFram();

	ImGuiStyle * style = &ImGui::GetStyle();

	if (ImGui::CollapsingHeader("IO_Test", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputText("IO::Input", InputTest, sizeof(InputTest));
		ImGui::InputText("IO::Output", OutputTest, sizeof(OutputTest));
	}

	if (ImGui::Button("IO::Import"))
	{
		strcpy_s(InputTest, sizeof(InputTest), OutputTest);
	}
	ImGui::SameLine();
	if (ImGui::Button("IO::Export"))
	{
		strcpy_s(OutputTest, sizeof(OutputTest), InputTest);
	}

	ImGui::End();
}

void CImGui_Manager_Client::Slider_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	if (false == Show_Slider_Test)
		return;

	ImGui::Begin("Slider_Test", &Show_Slider_Test);
	void BeginFrame();

	if (ImGui::CollapsingHeader("Slider_Test", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderFloat("SLIDER::Slider_01", &Slider_01, 0, 100);
		ImGui::SameLine();
		ImGui::InputFloat("SLIDER::Input_01", &Slider_01, 10.f);

		ImGui::SliderFloat("SLIDER::Slider_02", &Slider_02, 0, 100);
		ImGui::SameLine();
		ImGui::InputFloat("SLIDER::Input_02", &Slider_02);

		ImGui::SliderFloat("SLIDER::Slider_03", &Slider_03, 0, 100, "%.1f", ImGuiSliderFlags_Logarithmic);
		ImGui::SameLine();
		ImGui::InputFloat("SLIDER::Input_03", &Slider_03);
	}

	if (ImGui::CollapsingHeader("Drag_Test", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragInt("drag int 0..100", &DragInt, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);
		ImGui::DragFloat("drag small float", &DragFloat, 0.0001f, 0.0f, 0.0f, "%.06f ns");
	}

	ImGui::End();
}

//void CImGui_Manager_Client::TreeNode_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
//{
//	if (false == Show_TreeNode_Test)
//		return;
//
//	ImGui::Begin("TreeNode_Test", &Show_TreeNode_Test);
//	void BeginFrame();
//
//	if (ImGui::TreeNode("Trees"))
//	{
//		if (ImGui::TreeNode("Basic trees"))
//		{
//			if (ImGui::TreeNode("Child"))
//			{
//				if (ImGui::TreeNode("Child2"))
//				{
//					if (ImGui::Button("Hi"))
//					{
//						_tchar szCurrentDir[MAX_PATH] = TEXT("");
//						GetCurrentDirectory(MAX_PATH, szCurrentDir);
//						lstrcpy(szCurrentDir, L"..\\..\\Data\\");
//						ShellExecute(NULL, L"open", L"explorer.exe", szCurrentDir, NULL, SW_SHOW);
//					}
//					ImGui::TreePop();
//				}
//				ImGui::TreePop();
//			}
//			ImGui::TreePop();
//		}
//		ImGui::TreePop();
//	}
//	ImGui::End();
//}

void CImGui_Manager_Client::Listbox_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	if (false == Show_ListBox_Test)
		return;

	ImGui::Begin("Listbox_Test", &Show_ListBox_Test);
	void BeginFrame();

	//list<char*>	ListBox = m_ListBox;

	int iCount = 0;
	size_t iSize = m_ListBox.size();

	const char* items[100];

	for (int i = 0; i < 100; ++i)
	{
		items[i] = "-";
	}

	for (auto& iter : m_ListBox)
	{
		char* cName = iter;

		items[iCount] = cName;
		iCount++;
	}

	static int item_current = 0;
	ImGui::ListBox("ListBox", &item_current, items, IM_ARRAYSIZE(items), 4);
	ImGui::Text("ListSize : %d", iSize);

	//	Å½»ö
	ImGui::InputText("LIST::Input", InputListBox, sizeof(InputListBox));
	ImGui::InputText("LIST::Select", SelectedListBox, sizeof(SelectedListBox));
	ImGui::InputText("LIST::Search", SearchListBox, sizeof(SearchListBox));

	int iMapRocate = 0;
	for (auto& iter : m_ListBox)
	{
		if (item_current == iMapRocate)
		{
			strcpy_s(SelectedListBox, sizeof(SelectedListBox), iter);
			break;
		}
		else
			iMapRocate++;
	}

	if (ImGui::Button("LIST::InputBtn"))
	{
		char* Input = new char[MAX_PATH];
		CharList.push_back(Input);

		strcpy_s(Input, sizeof(Input), InputListBox);
		m_ListBox.push_back(Input);
	}
	ImGui::SameLine();
	if (ImGui::Button("LIST::SearchBtn"))
	{
		int iSearch = 0;

		for (auto& iter : m_ListBox)
		{
			if (0 == strcmp(iter, SearchListBox))
			{
				item_current = iSearch;
			}
			else
				iSearch++;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("LIST::DeleteBtn"))
	{
		int iSearch = 0;

		list<char*>::iterator iter;

		for (iter = m_ListBox.begin(); iter != m_ListBox.end();)
		{
			if (item_current == iSearch)
			{
				iter = m_ListBox.erase(iter);
				break;
			}
			else
			{
				iSearch++;
				iter++;
			}
		}
	}

	ImGui::End();
}

void CImGui_Manager_Client::Level_Status(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	if (false == Show_Level_Status)
		return;

	ImGui::Begin("Level_Status", &Show_Level_Status);
	void BeginFrame();

	switch (CGameInstance::GetInstance()->Get_NowLevel())
	{
	case LEVEL_LOGO:
		ImGui::Text("Level : LOGO");
		break;

	case LEVEL_GAMEPLAY:
		ImGui::Text("Level : GAME_PALY");
		break;
	}

	if (ImGui::Button("LEVEL::LOGO"))
	{
		CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(pDevice, pContext, LEVEL_LOADING, LEVEL_LOGO));
	}

	if (ImGui::Button("LEVEL::GAME_PLAY"))
	{
		CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(pDevice, pContext, LEVEL_LOADING, LEVEL_GAMEPLAY));
	}

	ImGui::End();
}

void CImGui_Manager_Client::Math_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	if (false == Show_Math_Test)
		return;

	ImGui::Begin("Math_Test", &Show_Math_Test, ImGuiWindowFlags_AlwaysAutoResize);
	void BeginFrame();

	ImGui::PushItemWidth(200);

	if (ImGui::CollapsingHeader("VECTOR", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat4("MATH::VEC1", &(XmFloat4_01.x), "%.1f");
		ImGui::InputFloat4("MATH::VEC2", &(XmFloat4_02.x), "%.1f");
		ImGui::InputFloat4("MATH::RESULT", &(XmFloat4_03.x), "%.1f");
		ImGui::Text("%s : ", XmWhatResult);
		ImGui::SameLine();
		ImGui::Text("%f", XmResult);

		/*if (ImGui::Button("MATH::InBound"))
		{
			strcpy_s(XmWhatResult, "InBound");
			XmResult = CMathUtils::InBoud
		}*/
	}
}

void CImGui_Manager_Client::Phsx_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	if (false == Show_Phsx_Text)
		return;

	ImGui::Begin("Phsx_Test", &Show_Phsx_Text, ImGuiWindowFlags_AlwaysAutoResize);
	void BeginFrame();

	ImGui::PushItemWidth(200);

	if (ImGui::CollapsingHeader("Phsx Setting", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat("PHSX::Gravity", &PhsxGravity);
		ImGui::SliderFloat("PHSX::WEIGHT", &PhsxWeight, 0.f, 1.f);
	}

	if (ImGui::CollapsingHeader("Position Setting", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat4("PHSX::POSITION", &(CubePosition.x), "%.1f");
		ImGui::Text("Speed : ");
		ImGui::SameLine();

		if (ImGui::Button("PHSX::SetPos"))
		{

		}
	}

	ImGui::End();
}

void CImGui_Manager_Client::UI_Setting(CTransform * pTransform)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Rocation", &Show_UI_Setting, ImGuiWindowFlags_AlwaysAutoResize);
	void BeginFrame();

	ImGui::PushItemWidth(200);

	if (ImGui::CollapsingHeader("Slider_Test", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderFloat("SLIDER::Silder_01", &Slider_01, 0, 100);
		ImGui::SameLine();
		ImGui::InputFloat("SLIDER::Input_01", &Slider_01, 10.f);
	}

	ImGui::End();
}

void CImGui_Manager_Client::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	for_each(CharList.begin(), CharList.end(), Safe_Delete_Array<char*>);
	CharList.clear();

	for_each(TcharList.begin(), TcharList.end(), Safe_Delete_Array<wchar_t*>);
	TcharList.clear();

	for_each(m_ListBox.begin(), m_ListBox.end(), Safe_Delete_Array<char*>);
	m_ListBox.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
