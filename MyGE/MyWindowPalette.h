#pragma once
#include "MyGameEngine.h"

namespace MyWP
{
	namespace MyWS
	{
		enum MyWindowStyle
		{
			FullScreen,
			Window,
			Window_BorderLess,
			Count//要素数カウンタ　この後ろには追加しないこと
		};
		DWORD WindowStyleTable[MyWindowStyle::Count]{
			WS_POPUP,
			(WS_CAPTION | WS_SYSMENU | WS_SIZEBOX),
			WS_POPUP,
		};
	}

	namespace MyWI
	{
		enum MyWindowIcon
		{
			Application,
			Hand,
			Question,
			Exclamation,
			Asterisk,
			WinLogo,
			Shield,
			Warning,
			Error,
			Information,
			Count//要素数カウンタ　この後ろには追加しないこと
		};
		LPSTR WindowIconTable[MyWindowIcon::Count]{
			IDI_APPLICATION,
			IDI_HAND,
			IDI_QUESTION,
			IDI_EXCLAMATION,
			IDI_ASTERISK,
			IDI_WINLOGO,
			IDI_SHIELD,
			IDI_WARNING,
			IDI_ERROR,
			IDI_INFORMATION
		};
	}

	namespace MyWC
	{
		enum MyWindowCursor
		{
			Arrow,
			Ibeam,
			Wait,
			Cross,
			UpArrow,
			Size,
			Icon,
			SizeNWSE,
			SizeNESW,
			SizeWE,
			SizeNS,
			SizeAll,
			No,
			Hand,
			AppStarting,
			Help,
			Pin,
			Person,
			Count//要素数カウンタ　この後ろには追加しないこと
		};
		LPSTR WindowCursorTable[MyWindowCursor::Count]{
			IDC_ARROW,
			IDC_IBEAM,
			IDC_WAIT,
			IDC_CROSS,
			IDC_UPARROW,
			IDC_SIZE,
			IDC_ICON,
			IDC_SIZENWSE,
			IDC_SIZENESW,
			IDC_SIZEWE,
			IDC_SIZENS,
			IDC_SIZEALL,
			IDC_NO,
			IDC_HAND,
			IDC_APPSTARTING,
			IDC_HELP,
			IDC_PIN,
			IDC_PERSON
		};
	}
}