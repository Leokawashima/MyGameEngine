#include "WinMain.h"

#pragma region WinMain

//-------------------------------------------------------------------------
//　＜リンクは 「Ctrl + 左クリック」 で標準の検索エンジンで開かれるよ！＞
//-------------------------------------------------------------------------
// https://www.keicode.com/winprimer/wp07.php#:~:text=__stdcall%20%E3%81%AF%20Windows%20API,__stdcall%20%E3%81%AB%E3%81%97%E3%81%A6%E3%81%84%E3%81%BE%E3%81%99%E3%80%82
// _stdcallってなに...？を教えてくれるURL↑
// https://code-bug.net/entry/2019/06/11/080327
// http://wisdom.sakura.ne.jp/system/winapi/win32/win3.html
// https://www.ritsumei.ac.jp/~mmr14135/johoWeb/cmnds.html#:~:text=%E3%82%B3%E3%83%9E%E3%83%B3%E3%83%89%E3%83%A9%E3%82%A4%E3%83%B3%E5%BC%95%E6%95%B0%E3%81%A8%E3%81%AF,%E3%81%8C%E4%BB%8A%E6%97%A5%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A7%E3%81%99%E3%80%82
// WinMain関数の引数の意味を解説してるURL↑
// ～第一引数はインスタンスなのは若干分かるがそれ以降の引数なんやねん～な人はチラ見するとおもろい。
// https://replication.hatenablog.com/entry/2019/05/05/225954
// https://scorpion140309.blog.fc2.com/blog-entry-130.html
// https://learn.microsoft.com/ja-jp/previous-versions/visualstudio/visual-studio-2015/code-quality/annotating-function-parameters-and-return-values?view=vs-2015&redirectedfrom=MSDN
// 引数になんかついてる...何...？が解決するURL↑
// 三番目のリンクはMicrosoftのドキュメントで英語だが、
// 自動翻訳で元から日本語で書かれていたのかと思うほど分かりやすい説明で書かれている
int __stdcall WinMain(
	_In_ HINSTANCE instance_,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int show_cmd_)
{
	//宣言だけで中身がないのでnewでインスタンス生成(Unityと同じ)
	//javaやC#のようなガベージコレクション(自動メモリ制御)がないが疑似機能としてスマートポインタがあるのでそれでもいい
	//今回はWinMainの最初と最後だけ生成されると決まっているので余分なオーバーヘッドをかさませないために標準ポインタ
	GE = new MyPG::MyGameEngine(instance_);

	//ウィンドウ生成の矩形設定　MyGameEngine.cppのコンストラクタで設定することを推奨
	RECT ws = {
		GE->screenState.StartPosX,
		GE->screenState.StartPosY,
		(int)(GE->screenState.WidthDef * GE->screenState.ViewScaleX),
		(int)(GE->screenState.HeightDef * GE->screenState.ViewScaleY)
	};
	//ウィンドウハンドルを生成　(ほぼ)ウィンドウのインスタンスと思っていいはず
	HWND wnd = MyProgram_CreateWindow(
		instance_,						//windowsに登録されたインスタンス番号(囚人番号みたいなもん)
		&GE->windowState,				//ウィンドウ情報の構造体
		&GE->screenState,				//ウィンドウ描画の構造体
		&ws,							//ウィンドウを生成する矩形　すぐ上で宣言済
		show_cmd_);						//どのようにウィンドウを表示するか…らしい　Microsoft公式によると通常最小化最大化のフラグが入っている？っぽい
	
	if (wnd == nullptr) { return 0; }

	//メッセージループ
	//基本的に実行中はWhile内に留まってウィンドウとタスクに対して行った行動に応じて処理を行う
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (GetActiveWindow() == wnd)
		{
			GE->Step(wnd);
		}
	}
	//インスタンスを破棄　これをしないとメモリにデータが残り続けて誰も消せない触れない地獄の領域が生まれる
	//...はずなんだけど　_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//を使ってメモリリークするか試してもデバッグのログには出てこない
	//VisualStudioでコンパイルする時に自動的にリークしないように防いでる？
	//Debugモードだとダメ？Resourceモードだとログ見れないけど...
	//そんなこんなでよくわからん　何が正解なんや
	delete GE;

	return 0;
}

#pragma endregion WinMain

#pragma region CreateWindow

// CreateWindow関数で引数の変数を変更することはまずないが読み取り専用である事を明示している
HWND MyProgram_CreateWindow(
	const HINSTANCE inst_,
	const MyPG::MyGameEngine::WindowState* winState_,
	const MyPG::MyGameEngine::ScreenState* screenState_,
	const LPRECT rect_,
	const int showCmd_)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = winState_->Style;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = winState_->CbClsExtra;
	wcex.cbWndExtra = winState_->CbWndExtra;
	wcex.hInstance = inst_;
	wcex.hIcon = winState_->HIcon;
	wcex.hCursor = winState_->HCursor;
	wcex.hbrBackground = screenState_->BackGroundColor;
	wcex.lpszMenuName = winState_->MenuName;
	wcex.lpszClassName = winState_->TaskName;
	wcex.hIconSm = winState_->HIconSm;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(nullptr, "ウインドウ登録に失敗", nullptr, MB_OK);
		return nullptr;
	}

	DWORD dws = GE->screenState.WindowStyle;

	AdjustWindowRectEx(rect_, dws, false, WS_EX_APPWINDOW);
	HWND wnd = CreateWindowEx(WS_EX_APPWINDOW,
		winState_->TaskName,
		winState_->WindowName,
		dws,
		0, 0,
		(rect_->right - rect_->left),
		(rect_->bottom - rect_->top),
		nullptr, nullptr, inst_, nullptr);

	if (!wnd)
	{
		MessageBox(nullptr, "ウインドウ生成に失敗", nullptr, MB_OK);
		return nullptr;
	}

	ShowWindow(wnd, showCmd_);
	UpdateWindow(wnd);
	return wnd;
}

#pragma endregion CreateWindow

#pragma region WindowProcedure

LRESULT CALLBACK WndProc(
	const HWND wnd_,
	const UINT msg_,
	const WPARAM wParam_,
	const LPARAM lParam_)
{
	static HPEN hPen[2];
	LRESULT ret = (LRESULT)0;
	static int width, height;

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
	case WM_PAINT://ウィンドウを再表示するとき(最小化最大化サイズ変更すべて)
	{
		PAINTSTRUCT ps;
		BeginPaint(wnd_, &ps);
		HDC hdc = ps.hdc;
		SelectObject(hdc, hPen[1]);
		Rectangle(hdc, 0, 0, GE->screenState.WidthDef, GE->screenState.HeightDef);
		EndPaint(wnd_, &ps);
	}
	break;
	case WM_MOUSEMOVE://ウィンドウ上でマウス移動したとき
	{
		int mPosX = LOWORD(lParam_);
		int mPosY = HIWORD(lParam_);
		HDC hdc = GetDC(wnd_);
		SelectObject(hdc, hPen[0]);
		Rectangle(hdc, mPosX, mPosY, mPosX + 30, mPosY + 30);//四角を書く
		//SetPixel(hdc, x + 15, y + 15, 0x000000FF);//赤い点を書く
		//SetPixel(hdc, x + 16, y + 16, 0x000000FF);//赤い点を書く
		//SetPixel(hdc, x + 17, y + 17, 0x000000FF);//赤い点を書く
		ReleaseDC(wnd_, hdc);
	}
	break;
	case WM_KEYDOWN://ウィンドウがアクティブでキー入力を受け付けつけたとき
		if (wParam_ == VK_ESCAPE)
		{
			DestroyWindow(wnd_);
		}
		if (wParam_ == 0x57)
		{

		}
	case WM_SIZE:
		width =  max(min(LOWORD(lParam_), GE->screenState.WidthMax), GE->screenState.WidthMin);
		height = HIWORD(lParam_);
		break;
	default://それ以外はWindows君に投げつける
		ret = DefWindowProc(wnd_, msg_, wParam_, lParam_);
		break;
	}
	return ret;
}

#pragma endregion WindowProcedure

// http://kaitei.net/winapi/window-procedures/
// WinMainのわかりやすぅ～いサイト↑