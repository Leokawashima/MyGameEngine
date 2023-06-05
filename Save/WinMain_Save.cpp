#include <windows.h>

LRESULT CALLBACK WndProc(HWND wnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
HWND MyProgram_CreateWindow(HINSTANCE inst_, const TCHAR cName_[], const TCHAR tName_[],
	RECT* wSize_, BOOL screenMode_, int showCmd_);


int __stdcall WinMain(
	HINSTANCE inst_,
	HINSTANCE,
	LPSTR,
	int showCmd)
{
	HWND wnd; // ウインドウハンドル
	RECT ws = { 0, 0, 480, 270 };
	wnd = MyProgram_CreateWindow(inst_, "WinPGTest", "Windows プログラムテスト",
		&ws, false, showCmd);
	if (wnd == nullptr) { return 0; }

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

HWND MyProgram_CreateWindow(HINSTANCE inst_, const TCHAR cName_[], const TCHAR
	tName_[],
	RECT* wSize_, BOOL screenMode_, int showCmd_)
{
	WNDCLASSEX wcex; // ウインドウ作成に使用
	HWND wnd; // ウインドウハンドル
	// データ初期化
	wcex.style = (CS_HREDRAW | CS_VREDRAW);
	wcex.hIcon = LoadIcon(inst_, IDI_APPLICATION);
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hIconSm = LoadIcon(inst_, IDI_APPLICATION);
	wcex.hInstance = inst_;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = cName_;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// Windows に登録
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(nullptr, "ウインドウ登録に失敗", nullptr, MB_OK);
		return nullptr;
	}
	// ウインドウ作成
	DWORD dws = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	AdjustWindowRectEx(wSize_, dws, false, WS_EX_APPWINDOW);
	wnd = CreateWindowEx(WS_EX_APPWINDOW,
		cName_,
		tName_,
		dws, 0, 0,
		(wSize_->right - wSize_->left),
		(wSize_->bottom - wSize_->top),
		nullptr, nullptr, inst_, nullptr);
	if (!wnd)
	{
		MessageBox(nullptr, "ウインドウ生成に失敗", nullptr, MB_OK);
		return nullptr;
	}
	ShowWindow(wnd, showCmd_);
	UpdateWindow(wnd);
	return wnd; //成功
}

LRESULT CALLBACK WndProc(HWND wnd_,	UINT msg_, WPARAM wParam_, LPARAM lParam_)
{
	static HPEN hPen[2];
	LRESULT ret = (LRESULT)0;

	switch (msg_) {//メッセージハンドラ
	case WM_CREATE://ウィンドウ生成されたとき(開始時のみ)
		hPen[0] = CreatePen(PS_SOLID, 10, 0x0000FF00);
		hPen[1] = CreatePen(PS_DASH, 1, 0x000000FF);
		break;
	case WM_CLOSE://ウィンドウの✖が押されたとき
		DestroyWindow(wnd_);
		break;
	case WM_DESTROY://ウィンドウ破棄されたとき(終了時のみ)
		DeleteObject(hPen[0]);
		DeleteObject(hPen[1]);
		PostQuitMessage(0);
		break;
	case WM_PAINT://ウィンドウを再表示するとき(最小化最大化)
	{
		static int xpos = 0;
		PAINTSTRUCT ps;
		BeginPaint(wnd_, &ps);
		HDC hdc = ps.hdc;
		SelectObject(hdc, hPen[1]);
		Rectangle(hdc, xpos, 40, xpos + 200, 40 + 100);
		EndPaint(wnd_, &ps);
		xpos += 100;
	}
	break;
	case WM_MOUSEMOVE://ウィンドウ上でマウス移動したとき
	{
		int x = LOWORD(lParam_);
		int y = HIWORD(lParam_);
		HDC hdc = GetDC(wnd_);
		SelectObject(hdc, hPen[0]);
		Rectangle(hdc, x, y, x + 30, y + 30);//四角を書く
		//SetPixel(hdc, x + 15, y + 15, 0x000000FF);//赤い点を書く
		//SetPixel(hdc, x + 16, y + 16, 0x000000FF);//赤い点を書く
		//SetPixel(hdc, x + 17, y + 17, 0x000000FF);//赤い点を書く
		ReleaseDC(wnd_, hdc);
	}
	break;
	default://それ以外はWindows君に投げつける
		ret = DefWindowProc(wnd_, msg_, wParam_, lParam_);
		break;
	}
	return ret;
}