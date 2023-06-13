#pragma once
#include <windows.h>
#include <memory>

namespace MyGE
{
	class MyGameEngine
	{
	public:
		struct WindowState
		{
			const TCHAR* TaskName;
			const TCHAR* WindowName;
			const TCHAR* MenuName;
			UINT Style;
			int CbClsExtra,
				CbWndExtra;
			HICON HIcon;
			HICON HIconSm;
			HCURSOR HCursor;
		};
		WindowState windowState;

		struct ScreenState
		{
			int StartPosX,
				StartPosY,
				WidthDef,
				WidthMin,
				WidthMax,
				HeightDef,
				HeightMin,
				HeightMax,
				ViewScaleX,
				ViewScaleY,
				FullScaleX,
				FullScaleY;
			DWORD MultiSample;
			BOOL FullScreenMode;
			HBRUSH BackGroundColor;
			DWORD WindowStyle;
		};
		ScreenState screenState;

		~MyGameEngine();
		void Step(HWND wnd_);
		bool QuitOrder;
		static MyGameEngine* DefaultInitialize(HINSTANCE instance_);
		RECT DefaultWindowRect();
	};
}

extern MyGE::MyGameEngine* GE;