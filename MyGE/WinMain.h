#pragma once
#include "MyGameEngine.h"

LRESULT CALLBACK WndProc(
	const HWND wnd_,
	const UINT msg_,
	const WPARAM wParam_,
	const LPARAM lParam_
);

HWND MyProgram_CreateWindow(
	const HINSTANCE inst_,
	const MyGE::MyGameEngine::WindowState* winState_,
	const MyGE::MyGameEngine::ScreenState* screenState_,
	const LPRECT rect_,
	const int showCmd_
);