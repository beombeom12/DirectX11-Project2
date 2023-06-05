#include "stdafx.h"
#include "..\public\UI_Player_Button.h"



CUI_Player_Button::CUI_Player_Button(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CUI(pDevice, pContext)
{
}

CUI_Player_Button::CUI_Player_Button(const CUI_Player_Button & rhs)
	:CUI(rhs)
{
}
