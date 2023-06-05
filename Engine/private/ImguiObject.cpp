#include "..\public\ImguiObject.h"
#include "GameInstance.h"
#include "Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include "Transform.h"
CImguiObject::CImguiObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CImGui_Console(pDevice, pContext)
{
}

HRESULT CImguiObject::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	
	return S_OK;
}

void CImguiObject::Show_Window()
{
	ImGui::Begin("ImguiObject");
	void BeginFrame();	

	ImGui::PushItemWidth(200);

	ENGINE_OPEN;
	MANAGER_OPEN(CObject_Manager);

	int iA = 0;

	switch (iA = ENGINE->Get_NowLevel())
	{
	case LEVEL_LOADING:
		m_mapLayer = MANAGER->Get_MapLayer(LEVEL_LOADING);
		break;

	case LEVEL_LOGO:
		m_mapLayer = MANAGER->Get_MapLayer(LEVEL_LOGO);
		break;

	case LEVEL_GAMEPLAY:
		m_mapLayer = MANAGER->Get_MapLayer(LEVEL_GAMEPLAY);
		break;
	}

	Peeking_Object();
	SaveLoad_Object();

	if (ImGui::CollapsingHeader("Layers", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int iCount = 0;
		size_t iSize = m_mapLayer->size();

		const char* Layers[150];

		for (int i = 0; i < 150; ++i)
		{
			Layers[i] = ".";
		}

		for (auto& iter : *m_mapLayer)
		{
			const _tchar* szName = iter.first;
			char* cName = new char[MAX_PATH];
			CharList.push_back(cName);

			WideCharToMultiByte(CP_ACP, 0, szName, sizeof(_tchar[MAX_PATH]), cName, sizeof(char[MAX_PATH]), NULL, NULL);
			Layers[iCount] = cName;
			iCount++;
		}

		static int Layer_current = 0;

		if (lstrcmp(TEXT(""), m_PeekedLayerName))
		{
			int iLayerCurrentFind = 0;
			for (auto& iter : *m_mapLayer)
			{
				if (!lstrcmp(iter.first, m_PeekedLayerName))
				{
					Layer_current = iLayerCurrentFind;
					m_PeekedLayerName = TEXT("");
					break;
				}
				++iLayerCurrentFind;
			}
		}

		ImGui::ListBox("Layers", &Layer_current, Layers, IM_ARRAYSIZE(Layers), 4);

		if ((Layer_current < iSize) && (nullptr != m_mapLayer))
		{
			_tchar szName[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Layers[Layer_current], sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));

			m_ObjectList = MANAGER->Get_ObjectList(ENGINE->Get_NowLevel(), szName);
		}
	}

	if (ImGui::CollapsingHeader("Objects", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int iCount = 0;
		
		const char* Objects[250];
		size_t iSize;

		for (int i = 0; i < 250; ++i)
		{
			Objects[i] = ".";
		}

		if (nullptr != m_ObjectList)
		{
			iSize = m_ObjectList->size();
			for (auto& iter : *m_ObjectList)
			{
				const _tchar* szName = iter->Get_MyName();
				char* cName = new char[MAX_PATH];
				CharList.push_back(cName);

				WideCharToMultiByte(CP_ACP, 0, szName, sizeof(_tchar[MAX_PATH]), cName, sizeof(char[MAX_PATH]), NULL, NULL);
				Objects[iCount] = cName;
				iCount++;
			}
		}
		
		static int Object_current = 0;

		if (lstrcmp(TEXT(""), m_PeekedObjectName))
		{
			int iObjectCurrentFind = 0;
			for (auto& iter : *m_ObjectList)
			{
				if (!lstrcmp(iter->Get_MyName(), m_PeekedObjectName))
				{
					Object_current = iObjectCurrentFind;
					m_PeekedObjectName = TEXT("");
					break;
				}
				++iObjectCurrentFind;
			}
		}
		ImGui::ListBox("Objects", &Object_current, Objects, IM_ARRAYSIZE(Objects), 4);

		if ((Object_current < iSize) && (m_ObjectList != nullptr))
		{
			_tchar szName[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Objects[Object_current], sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));

			m_CurrentObject = Find_Object(szName);
			Safe_AddRef(m_CurrentObject);
		}
	}

	if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int iCount = 0;
		size_t iSize;

		const char* Components[100];

		for (int i = 0; i < 100; ++i)
		{
			Components[i] = "-";
		}

		if (nullptr != m_mapComponent)
		{
			iSize = m_mapComponent->size();

			for (auto& iter : *m_mapComponent)
			{
				const _tchar* szName = iter.first;
				char* cName = new char[MAX_PATH];
				CharList.push_back(cName);

				WideCharToMultiByte(CP_ACP, 0, szName, sizeof(_tchar[MAX_PATH]), cName, sizeof(char[MAX_PATH]), NULL, NULL);
				Components[iCount] = cName;
				iCount++;
			}
		}

		static int Component_current = 0;
		
		if (lstrcmp(TEXT(""), m_PeekedComponent))
		{
			int iComponentCurrentFind = 0;
			for (auto& iter : *m_mapComponent)
			{
				if (!lstrcmp(iter.first, m_PeekedComponent))
				{
					Component_current = iComponentCurrentFind;
					m_PeekedComponent = TEXT("");
					break;
				}
				++iComponentCurrentFind;
			}
		}

		ImGui::ListBox("Components", &Component_current, Components, IM_ARRAYSIZE(Components), 5);

		Control_Component(Components[Component_current]);
	}

	MANAGER_CLOSE;
	ENGINE_CLOSE;

	if (ImGui::Button("Delete"))
	{
		for (auto& iter : *m_ObjectList)
		{
			if (!lstrcmp(iter->Get_MyName(), m_CurrentObject->Get_MyName()))
			{
				iter->Kill_Obj();
			}
		}
	}
	
	Release_Info();

	ImGui::End();

	__super::Show_Window();
}

void CImguiObject::Release_Info()
{
	Safe_Release(m_CurrentObject);
	Safe_Release(m_CurrentComponent);

	m_mapLayer = nullptr;
	m_ObjectList = nullptr;
	m_mapComponent = nullptr;
	m_CurrentObject = nullptr;
	m_CurrentComponent = nullptr;

	for_each(CharList.begin(), CharList.end(), Safe_Delete_Array<char*>);
	CharList.clear();
}

CGameObject * CImguiObject::Find_Object(const _tchar * pMyName)
{
	for (auto iter : *m_ObjectList)
	{
		if (!lstrcmp(iter->Get_MyName(), pMyName))
		{
			m_mapComponent = iter->Get_MapCompoent();
			return iter;
		}
	}

	return nullptr;
}

void CImguiObject::Control_Component(const char * pComponentName)
{
	if (!strcmp(pComponentName, "Com_Transform"))
	{
		if (ImGui::CollapsingHeader("Com_Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			m_CurrentComponent = m_CurrentObject->Get_Component(TEXT("Com_Transform"));
			Safe_AddRef(m_CurrentComponent);

			ImGui::InputFloat4("[0]", &(static_cast<CTransform*>(m_CurrentComponent)->Get_WorldMatrix().m[0][0]));
			ImGui::InputFloat4("[1]", &(static_cast<CTransform*>(m_CurrentComponent)->Get_WorldMatrix().m[1][0]));
			ImGui::InputFloat4("[2]", &(static_cast<CTransform*>(m_CurrentComponent)->Get_WorldMatrix().m[2][0]));
			ImGui::InputFloat4("[3]", &(static_cast<CTransform*>(m_CurrentComponent)->Get_WorldMatrix().m[3][0]));

			ImGuizmo_Setting();
		}
	}

	if (!strcmp(pComponentName, "Texture_Component"))
	{
		if (ImGui::CollapsingHeader("Texture_Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
		}
	}

	if (!strcmp(pComponentName, "Prototype_Component_Shader_VtxTex"))
	{
		if (ImGui::CollapsingHeader("Prototype_Component_Shader_VtxTex", ImGuiTreeNodeFlags_DefaultOpen))
		{
		}
	}

	if (!strcmp(pComponentName, "Buffer_Component"))
	{
		if (ImGui::CollapsingHeader("Buffer_Component", ImGuiTreeNodeFlags_DefaultOpen))
		{

		}
	}

	if (!strcmp(pComponentName, "Prototype_Component_Renderer"))
	{
		if (ImGui::CollapsingHeader("Prototype_Component_Renderer", ImGuiTreeNodeFlags_DefaultOpen))
		{

		}
	}
}

void CImguiObject::Peeking_Object()
{
	ENGINE_OPEN;

	if (ENGINE->Mouse_Down(DIM_LB))
	{
		for (auto Layer : *m_mapLayer)
		{
			for (auto Object : *Layer.second->Get_ObjectList())
			{
				if (ID_PEEKING != Object->Get_ID())
					continue;
			
				_bool isPeek = static_cast<CCalculator*>(Object->Get_Component(TEXT("Prototype_Component_Calculator")))
					->Ray_CubePeeking(
						static_cast<CVIBuffer_Square*>(Object->Get_Component(TEXT("Buffer_Component"))),
						static_cast<CTransform*>(Object->Get_Component(TEXT("Com_Transform"))));

				if (false == isPeek)
					continue;

				m_PeekedLayerName = Layer.first;
				m_PeekedObjectName = Object->Get_MyName();
				m_PeekedComponent = TEXT("Com_Transform");

				break;
			}
		}
	}

	ENGINE_CLOSE;
}

void CImguiObject::ImGuizmo_Setting(void)
{
	ENGINE_OPEN;

	_float4x4	WorldMatrix = static_cast<CTransform*>(m_CurrentComponent)->Get_WorldMatrix();

	ImGuizmo::BeginFrame();
	static float snap[3] = { 1.f, 1.f, 1.f };
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	if (ImGui::IsKeyPressed(90))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(69))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(82))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	ImGuizmo::DecomposeMatrixToComponents(reinterpret_cast<float*>(&WorldMatrix), matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Translate", matrixTranslation);
	ImGui::InputFloat3("Rotate", matrixRotation);
	ImGui::InputFloat3("Scale", matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, reinterpret_cast<float*>(&WorldMatrix));

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}

	static bool useSnap(false);
	if (ImGui::IsKeyPressed(83))
		useSnap = !useSnap;
	ImGui::Checkbox("##something", &useSnap);
	ImGui::SameLine();
	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}

	ImGuiIO& io = ImGui::GetIO();
	RECT rt;
	GetClientRect(ENGINE->GetHWND(), &rt);
	POINT lt{ rt.left, rt.top };
	ClientToScreen(ENGINE->GetHWND(), &lt);
	ImGuizmo::SetRect((_float)lt.x, (_float)lt.y, io.DisplaySize.x, io.DisplaySize.y);

	_float4x4 matView, matProj;
	XMStoreFloat4x4(&matView, ENGINE->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW));
	XMStoreFloat4x4(&matProj, ENGINE->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ));

	ImGuizmo::Manipulate(
		reinterpret_cast<float*>(&matView),
		reinterpret_cast<float*>(&matProj),
		mCurrentGizmoOperation,
		mCurrentGizmoMode,
		reinterpret_cast<float*>(&WorldMatrix),
		nullptr, useSnap ? &snap[0] : nullptr);

	static_cast<CTransform*>(m_CurrentComponent)->Set_WorldMatrix(WorldMatrix);

	ENGINE_CLOSE;
}

void CImguiObject::SaveLoad_Object()
{
	ENGINE_OPEN;

	ImGui::Begin("Save/Load");

	ImGui::PushItemWidth(100);

	ImGui::InputText("FileName", m_szSaveDateName, MAX_PATH);

	if (ImGui::Button("SaveMap"))
	{
		_tchar szName[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szSaveDateName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));

		_tchar szPath[MAX_PATH] = TEXT("../../Data/");
		
		lstrcat(szPath, szName);
		lstrcpy(szName, szPath);
		lstrcat(szName, TEXT(".dat"));

		HANDLE		hFile = CreateFileW(szName,
			GENERIC_WRITE,
			NULL,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		DWORD dwByte = 0;

		_uint iObjectCount = 0;
		for (auto Layer : *m_mapLayer)
		{
			for (auto Object : *Layer.second->Get_ObjectList())
			{
				if (ID_INSTALLATLON != Object->Get_ID())
					continue;

				iObjectCount++;
			}
		}
		WriteFile(hFile, &iObjectCount, sizeof(_uint), &dwByte, nullptr);

		for (auto Layer : *m_mapLayer)
		{
			for (auto Object : *Layer.second->Get_ObjectList())
			{
				if (ID_INSTALLATLON != Object->Get_ID())
					continue;

				_float4x4	WorldMatrix = static_cast<CTransform*>(Object->Get_Component(TEXT("Com_Transform")))->Get_WorldMatrix();
				const _tchar* LayerName = Layer.first;
				const _tchar* ProtoName = Object->Get_ProtoName();
				const _tchar* ObjectName = Object->Get_MyName();

				WriteFile(hFile, &WorldMatrix, sizeof(XMFLOAT4X4), &dwByte, nullptr);
				WriteFile(hFile, LayerName, sizeof(_tchar[MAX_PATH]), &dwByte, nullptr);
				WriteFile(hFile, ProtoName, sizeof(_tchar[MAX_PATH]), &dwByte, nullptr);
				WriteFile(hFile, ObjectName, sizeof(_tchar[MAX_PATH]), &dwByte, nullptr);
			}
		}

		CloseHandle(hFile);
		MSG_BOX("Complete");
	}
	ImGui::SameLine();
	if (ImGui::Button("LoadMap"))
	{
		_tchar* szName = new _tchar[MAX_PATH];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szSaveDateName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));
		TcharList.push_back(szName);

		_tchar szPath[MAX_PATH] = TEXT("../../Data/");

		lstrcat(szPath, szName);
		lstrcpy(szName, szPath);
		lstrcat(szName, TEXT(".dat"));

		HANDLE		hFile = CreateFileW(szName,
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		DWORD	dwByte = 0;

		_uint iObjectCount = 0;
		ReadFile(hFile, &iObjectCount, sizeof(_uint), &dwByte, nullptr);

		for (auto Layer : *m_mapLayer)
		{
			_bool bClear = false;
			for (auto Object : *Layer.second->Get_ObjectList())
			{
				if (ID_INSTALLATLON != Object->Get_ID())
					continue;

				Safe_Release(Object);
				bClear = true;
			}

			if (bClear)
				Layer.second->Get_ObjectList()->clear();
		}

		while (iObjectCount--)
		{
			_float4x4	WorldMatrix = _float4x4();
			_tchar*	LayerName = new _tchar[MAX_PATH];
			_tchar*	ProtoName = new _tchar[MAX_PATH];
			_tchar*	ObjectName = new _tchar[MAX_PATH];

			TcharList.push_back(LayerName);
			TcharList.push_back(ProtoName);
			TcharList.push_back(ObjectName);

			ReadFile(hFile, &WorldMatrix, sizeof(XMFLOAT4X4), &dwByte, nullptr);
			ReadFile(hFile, LayerName, sizeof(_tchar[MAX_PATH]), &dwByte, nullptr);
			ReadFile(hFile, ProtoName, sizeof(_tchar[MAX_PATH]), &dwByte, nullptr);
			ReadFile(hFile, ObjectName, sizeof(_tchar[MAX_PATH]), &dwByte, nullptr);

			if (FAILED(ENGINE->Clone_GameObject(ENGINE->Get_NowLevel(), LayerName, ProtoName, ObjectName)))
			{
				MSG_BOX("Fail");
				CloseHandle(hFile);
				ENGINE_CLOSE;
				ImGui::End();
			}

			static_cast<CTransform*>(
				ENGINE->Get_GameObject(ENGINE->Get_NowLevel(), LayerName, ObjectName)
				->Get_Component(TEXT("Com_Transform")))
				->Set_WorldMatrix(WorldMatrix);

			if (0 == dwByte)
				break;
		}

		CloseHandle(hFile);
		MSG_BOX("Complete");
	}

	ENGINE_CLOSE;

	ImGui::End();
}

CImguiObject * CImguiObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CImguiObject* pInstance = new CImguiObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImguiObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImguiObject::Free()
{
	__super::Free();
	
	Safe_Release(m_CurrentObject);
	Safe_Release(m_CurrentComponent);
	
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_mapLayer[i])
			Safe_Release(Pair.second);

		m_mapLayer[i].clear();
	}

	Safe_Delete_Array(m_mapLayer);

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_mapComponent[i])
			Safe_Release(Pair.second);

		m_mapComponent[i].clear();
	}

	Safe_Delete_Array(m_mapComponent);
}
