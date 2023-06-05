#include "..\Public\ImGui_Manager.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "ImGuizmo.h"
#include "ImSequencer.h"

#include "ImGui_Console.h"

#include "GameInstance.h"


IMPLEMENT_SINGLETON(CImGui_Manager)

_bool	CImGui_Manager::Show_IO_Test = false;
_bool	CImGui_Manager::Show_Slider_Test = false;
_bool	CImGui_Manager::Show_TreeNode_Test = false;
_bool	CImGui_Manager::Show_ListBox_Test = false;
_bool	CImGui_Manager::Show_Level_Status = false;

CImGui_Manager::CImGui_Manager()
{
}

HRESULT CImGui_Manager::Ready_Imgui(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	return S_OK;
}

void CImGui_Manager::Tick(_double TimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CImGui_Manager::Late_Tick(_double TimeDelta)
{
	for (auto& iter : m_mapConsole)
	{
		iter.second->Show_Window();
	}
}

HRESULT CImGui_Manager::Render(void)
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

void CImGui_Manager::CreateMainWindow(void)
{
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::SetWindowPos(ImVec2(200, 200));

	//void BeginFrame();

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	if (ImGui::Button("Clear"))
	{
		MSG_BOX("잘된다ㅋㅋ");
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void CImGui_Manager::IO_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	ImGui::Begin("IO_TEST", &Show_IO_Test);

	//void BeginFrame();

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

void CImGui_Manager::Slider_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	ImGui::Begin("Slider_Test", &Show_Slider_Test);
	//void BeginFrame();

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

void CImGui_Manager::TreeNode_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	ImGui::Begin("TreeNode_Test", &Show_TreeNode_Test);
	//void BeginFrame();

	if (ImGui::TreeNode("Trees"))
	{
		if (ImGui::TreeNode("Basic trees"))
		{
			if (ImGui::TreeNode("Child"))
			{
				if (ImGui::TreeNode("Child2"))
				{
					if (ImGui::Button("Hi"))
					{
						// 해당 경로 파일 오픈
						_tchar szCurrentDir[MAX_PATH] = TEXT("");
						GetCurrentDirectory(MAX_PATH, szCurrentDir);
						lstrcpy(szCurrentDir, L"..\\..\\Data\\");
						ShellExecute(NULL, L"open", L"explorer.exe", szCurrentDir, NULL, SW_SHOW);
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void CImGui_Manager::Listbox_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	ImGui::Begin("Listbox_Test", &Show_ListBox_Test);
	//void BeginFrame();

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

	//	탐색
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

void CImGui_Manager::Level_Status(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	ImGui::Begin("Level_Status", &Show_Level_Status);
	void BeginFrame();

	switch (CGameInstance::GetInstance()->Render_Level())
	{
	case LEVEL_LOGO:
		ImGui::Text("Level : LOGO");
		break;
	}

	if (ImGui::Button("LEVEL::InputBtn"))
	{
		//CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOADING, eID));
	}

	ImGui::End();
}

HRESULT CImGui_Manager::Add_Console(const _tchar * pName, CImGui_Console * pConsole)
{
	if (nullptr == pConsole)
		return E_FAIL;

	m_mapConsole.emplace(pName, pConsole);

	return S_OK;
}

HRESULT CImGui_Manager::Delete_Console(const _tchar * pName)
{
	CImGui_Console*	pConsole = Find_Console(pName);

	if (nullptr == pConsole)
		return E_FAIL;

	Safe_Release(pConsole);

	return S_OK;
}

CImGui_Console * CImGui_Manager::Find_Console(const _tchar * pName)
{
	auto iter = find_if(m_mapConsole.begin(), m_mapConsole.end(), CTag_Finder(pName));

	if (iter == m_mapConsole.end())
		return nullptr;

	return iter->second;
}

void CImGui_Manager::Free(void)
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	for_each(m_mapConsole.begin(), m_mapConsole.end(), CDeleteMap());
	m_mapConsole.clear();

	for_each(CharList.begin(), CharList.end(), Safe_Delete_Array<char*>);
	CharList.clear();

	for_each(m_ListBox.begin(), m_ListBox.end(), Safe_Delete_Array<char*>);
	m_ListBox.clear();

	for_each(TcharList.begin(), TcharList.end(), Safe_Delete_Array<wchar_t*>);
	TcharList.clear();
}
