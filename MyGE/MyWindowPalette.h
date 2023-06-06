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
			Count//�v�f���J�E���^�@���̌��ɂ͒ǉ����Ȃ�����
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
			Count//�v�f���J�E���^�@���̌��ɂ͒ǉ����Ȃ�����
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
			Count//�v�f���J�E���^�@���̌��ɂ͒ǉ����Ȃ�����
		};
	}	
}