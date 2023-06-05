#include "stdafx.h"
#include "..\public\Navigation_Control.h"
#include "Terrain.h"
#include "Navigation.h"
#include "Cell.h"
#include "Navigation_Cube.h"
CNavigation_Control::CNavigation_Control(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CImGui_Console(pDevice, pContext)
{
}

HRESULT CNavigation_Control::Initialize(void)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_bCreate = false;
	m_bCellCreate = false;
	m_bDelete = false;
	return S_OK;
}

void CNavigation_Control::Show_Window(void)
{
	ImGui::Begin("Navigation Control");
	void BeginFrame();

	ImGui::PushItemWidth(300);

	ENGINE_OPEN;

	if (ENGINE->Get_NowLevel() == LEVEL_GAMEPLAY && m_NaviCom == nullptr)
	{
		m_NaviCom = static_cast<CTerrain*>(ENGINE->Get_GameObject(LEVEL_GAMEPLAY,
			TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Terrain")))
			->Get_NaviCom();

		ImGui::InputFloat4("Pos.x", &m_fCellPoint.x);
	}

	if (ENGINE->Get_NowLevel() == LEVEL_GAMEPLAY)
	{
		m_fCellPoint = static_cast<CTerrain*>(ENGINE->Get_GameObject(LEVEL_GAMEPLAY,
			TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Terrain")))
			->Get_Pick();

		ImGui::InputFloat4("Pos.x", &m_fCellPoint.x);
	}
	ImGui::Checkbox("Create", &m_bCreate);
	ImGui::SameLine();
	ImGui::Checkbox("CellCreate", &m_bCellCreate);

	ImGui::InputText("Cell", m_szCellName, MAX_PATH);


	if (ImGui::Button("SaveCell"))
	{
		_tchar szName[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szCellName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));

		_tchar szPath[MAX_PATH] = TEXT("../Bin/Data/");

		lstrcat(szPath, szName);
		lstrcpy(szName, szPath);
		lstrcat(szName, TEXT(".dat"));

		HANDLE		hFile = CreateFile(szName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		DWORD	dwByte = 0;

		for (auto& iter : m_vPoint)
			m_PointList.push_back(iter);

		for (auto& iter : m_PointList)
		{
			WriteFile(hFile, &iter, sizeof(_float3), &dwByte, nullptr);
		}

		CloseHandle(hFile);
		MSG_BOX("SaveCell_Complete");
	}

	ImGui::SameLine();

	if (ImGui::Button("LoadCell"))
	{
		_tchar* szName = new _tchar[MAX_PATH];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szCellName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));
		TcharList.push_back(szName);

		_tchar szPath[MAX_PATH] = TEXT("../Bin/Data/");

		lstrcat(szPath, szName);
		lstrcpy(szName, szPath);
		lstrcat(szName, TEXT(".dat"));

		_ulong		dwByte = 0;
		HANDLE		hFile = CreateFileW(szName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;

		//_float3		vPoints[CCell::POINT_END];
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}


		for (auto& iter : m_PointList)
			m_PointList.clear();





		_float3		vPoints;

		while (true)
		{
			ReadFile(hFile, &vPoints, sizeof(_float3), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			m_PointList.push_back(vPoints);

		}

		CloseHandle(hFile);
		MSG_BOX("LoadCell_Complete");
	}


	ImGui::InputText("Cube : ", m_szCubeName, MAX_PATH);

	if (ImGui::Button("SaveCube"))
	{
		_tchar szName[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szCubeName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));

		_tchar szPath[MAX_PATH] = TEXT("../Bin/Data/");

		lstrcat(szPath, szName);
		lstrcpy(szName, szPath);
		lstrcat(szName, TEXT(".dat"));

		DWORD		dwByte = 0;
		HANDLE		hFile = CreateFileW(szName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		for (auto& iter : m_SquareCube)
		{
			_vector vAsve = static_cast<CTransform*>(iter->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_TRANSLATION);
			WriteFile(hFile, &vAsve, sizeof(_vector), &dwByte, nullptr);
		}

		CloseHandle(hFile);
		MSG_BOX("SaveCube_Complete");
	}


	ImGui::SameLine();

	if (ImGui::Button("LoadCube"))
	{
		_tchar* szName = new _tchar[MAX_PATH];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szCubeName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));
		TcharList.push_back(szName);

		_tchar szPath[MAX_PATH] = TEXT("../Bin/Data/");

		lstrcat(szPath, szName);
		lstrcpy(szName, szPath);
		lstrcat(szName, TEXT(".dat"));

		HANDLE		hFile = CreateFile(szName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		for (auto& iter : m_SquareCube)
			Safe_Release(iter);
		m_SquareCube.clear();

		DWORD	dwByte = 0;

		while (true)
		{
			_vector vPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
			ReadFile(hFile, &vPosition, sizeof(_vector), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameObject*		pCellCube = nullptr;

			pCellCube = ENGINE->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Square"));
			if (nullptr == pCellCube)
				return;

			static_cast<CTransform*>(pCellCube->Get_Component(TEXT("Com_Transform")))->
				Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPosition), XMVectorGetZ(vPosition), 1.f));

			m_SquareCube.push_back(pCellCube);
		}

		CloseHandle(hFile);
		MSG_BOX("LoadCube_Complete");
	}


	ImGui::Checkbox("DeleteCell", &m_bDelete);

	_int iSelect = -1;
	if (ENGINE->Get_NowLevel() == LEVEL_GAMEPLAY && m_NaviCom != nullptr)
	{
		_uint iNum = 1;
		D3D11_VIEWPORT ViewPort;
		m_pContext->RSGetViewports(&iNum, &ViewPort);

//		iSelect = m_NaviCom->isPeeking_NaviCell(ENGINE->GetHWND(), ViewPort.Width, ViewPort.Height);
		ImGui::Text("iSelect : %d", iSelect);

		if (ENGINE->Key_Down(DIK_B))
		{
			m_iDeleteCellIndex = iSelect;
		}
		//m_NaviCom->SetPickingIndex(iSelect);
	}
	ImGui::Text("DeleteCellIndex : %d", m_iDeleteCellIndex);
	if (m_bDelete && ENGINE->Mouse_Down(DIM_LB))
	{
		if (-1 != m_iDeleteCellIndex)
		{
			m_NaviCom->DeleteCell(m_iDeleteCellIndex);
		}
	}






	for (auto& iter : m_SquareCube)
	{
		iter->Tick(0.f);
		iter->Late_Tick(0.f);
		iter->Render();
	}




	Picking_Cube();
	Init();
	CreateImgui();
	if (m_bCreate || m_bCellCreate)
		m_iCurrentIndex = -1;

	if (-1 != m_iCurrentIndex)
		ImGui_Setting();


	ENGINE_CLOSE;

	ImGui::End();

}

void CNavigation_Control::ImGui_Setting(void)
{
	ENGINE_OPEN;

	_float4x4	WorldMatrix = static_cast<CTransform*>(m_SquareCube[m_iCurrentIndex]->Get_Component(TEXT("Com_Transform")))->Get_WorldMatrix();

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

	static_cast<CTransform*>(m_SquareCube[m_iCurrentIndex]->Get_Component(TEXT("Com_Transform")))->Set_WorldMatrix(WorldMatrix);

	ENGINE_CLOSE;

}

void CNavigation_Control::CreateImgui()
{

	ENGINE_OPEN;

	if (!m_bCreate && !m_bCellCreate && ENGINE->Mouse_Down(DIM_LB))
	{
		_uint iCount = 0;

		for (auto& iter : m_SquareCube)
		{
			if (static_cast<CNavigation_Cube*>(iter)->Peeking())
			{
				m_iCurrentIndex = iCount;
				break;
			}
			else
				iCount++;
		}
	}

	ENGINE_CLOSE;

}

void CNavigation_Control::Picking_Cube()
{

	ENGINE_OPEN;

	if (m_bCreate  && ENGINE->Mouse_Down(DIM_LB) && ENGINE->Key_Pressing(DIK_V))
	{
		Ready_Layer_Cube(TEXT("Navi"));
	}


	ENGINE_CLOSE;
}
void CNavigation_Control::Ready_Layer_Cube(const _tchar * pLayerTag)
{


	ENGINE_OPEN;

	CGameObject*		pCellCube = nullptr;

	pCellCube = ENGINE->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Square"));

	if (nullptr == pCellCube)
		return;

	static_cast<CTransform*>(pCellCube->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_TRANSLATION,
		XMVectorSet(m_fCellPoint.x, m_fCellPoint.y, m_fCellPoint.z, 1.f));

	m_SquareCube.push_back(pCellCube);


	ENGINE_CLOSE;
}

void CNavigation_Control::Init()
{

	ENGINE_OPEN;

	if (m_bCellCreate && ENGINE->Mouse_Down(DIM_LB))
	{
		_uint iCount = 0;

		for (auto& iter : m_SquareCube)
		{
			if (static_cast<CNavigation_Cube*>(iter)->Peeking())
			{
				_vector vPos =
					static_cast<CTransform*>
					(iter->Get_Component(TEXT("Com_Transform")))
					->Get_State(CTransform::STATE_TRANSLATION);

				m_vPoint[m_iCurIndex++] =
					_float3(XMVectorGetX(vPos),
						XMVectorGetY(vPos),
						XMVectorGetZ(vPos));
			}
		}

		if (m_iCurIndex >= 3)
		{
			_ulong		dwByte = 0;
			HANDLE		hFile = CreateFile(TEXT("../Bin/Data/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (0 == hFile)
				return;

			//Sorting
			const _vector vPointA = XMLoadFloat3(&m_vPoint[0]);
			const _vector vPointB = XMLoadFloat3(&m_vPoint[1]);
			const _vector vPointC = XMLoadFloat3(&m_vPoint[2]);

			const _vector vAB = vPointB - vPointA;
			const _vector vBC = vPointC - vPointB;

			_float3 vCross;
			XMStoreFloat3(&vCross, XMVector3Cross(vAB, vBC));


			if (vCross.y < 0)
			{
				_float3 tmp = m_vPoint[1];
				m_vPoint[1] = m_vPoint[2];
				m_vPoint[2] = tmp;
			}

			for (auto& iter : m_vPoint)
				m_PointList.push_back(iter);

			for (auto& iter : m_PointList)
				WriteFile(hFile, &iter, sizeof(_float3), &dwByte, nullptr);


			CloseHandle(hFile);

			m_NaviCom->iGo(TEXT("../Bin/Data/Navigation.dat"));

			m_iCurIndex = 0;

		}


	}

	ENGINE_CLOSE;
}

CNavigation_Control * CNavigation_Control::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CNavigation_Control* pInstance = new CNavigation_Control(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create : Navigation_Control");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavigation_Control::Free()
{
	__super::Free();

	for (auto& rSquare : m_SquareCube)
		Safe_Release(rSquare);

	m_SquareCube.clear();
}
