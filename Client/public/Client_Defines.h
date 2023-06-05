#pragma once
#pragma warning (disable : 4251)

#pragma warning (disable : 4717)
#include <Process.h>

#include "GameInstance.h"

namespace Client
{
	static const unsigned int g_iWinSizeX = 1250;
	static const unsigned int g_iWinSizeY = 800;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
extern bool g_bFullScreen;
extern bool g_bNeedResizeSwapChain;

using namespace Client;

static const bool g_bFalse = false;
static const bool g_bTrue = true;


