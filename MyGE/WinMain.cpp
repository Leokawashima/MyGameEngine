#include "WinMain.h"

#pragma region WinMain

// https://www.keicode.com/winprimer/wp07.php#:~:text=__stdcall%20%E3%81%AF%20Windows%20API,__stdcall%20%E3%81%AB%E3%81%97%E3%81%A6%E3%81%84%E3%81%BE%E3%81%99%E3%80%82
// _stdcallってなんぞ？を教えてくれるURL
// https://code-bug.net/entry/2019/06/11/080327
// http://wisdom.sakura.ne.jp/system/winapi/win32/win3.html
// https://www.ritsumei.ac.jp/~mmr14135/johoWeb/cmnds.html#:~:text=%E3%82%B3%E3%83%9E%E3%83%B3%E3%83%89%E3%83%A9%E3%82%A4%E3%83%B3%E5%BC%95%E6%95%B0%E3%81%A8%E3%81%AF,%E3%81%8C%E4%BB%8A%E6%97%A5%E3%81%AE%E7%9B%AE%E7%9A%84%E3%81%A7%E3%81%99%E3%80%82
// WinMain関数の引数の意味を解説してるURL
// ～第一引数はインスタンスなのは若干分かるがそれ以降の引数なんやねんになったら。
// https://replication.hatenablog.com/entry/2019/05/05/225954
// https://scorpion140309.blog.fc2.com/blog-entry-130.html
// https://learn.microsoft.com/ja-jp/previous-versions/visualstudio/visual-studio-2015/code-quality/annotating-function-parameters-and-return-values?view=vs-2015&redirectedfrom=MSDN
// 引数になんかついてる...何？が解決するURL
// 三番目のリンクはMicrosoftのドキュメントで英語だが、
// 自動翻訳すると元から日本語で書かれていたのかと思うほど分かりやすい説明で書かれている
int __stdcall WinMain(
	_In_ HINSTANCE instance_,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int show_cmd_)
{
	//宣言だけで中身がないのでnewでインスタンス生成(Unityと同じ)
	//javaやC#のようなガベージコレクション(自動メモリ制御)がないが疑似機能としてスマートポインタがあるのでそれでもいい
	//今回はWinMainの最初と最後だけ生成されると決まっているので余分なオーバーヘッドを重むのが嫌なので標準ポインタ
	GE = MyGE::MyGameEngine::DefaultInitialize(instance_);
	//ウィンドウ生成の矩形設定　MyWindowPalette空間で定義してある
	RECT ws = GE->DefaultWindowRect();
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
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);　もしくは
	//_CrtDumpMemoryLeaks();
	//前者はWinMain関数内の最初の行に書いてあげる
	//後者はプログラムが終了する時に書いてあげる
	//その状態でデバッガー有りで実行して終了までを行い、左下のの出力一覧に
	//{??}　なんたら～　みたいな行が二行以上出てメモリのアドレス値みたいなのがでたらリーク発生
	//あくまで確認するためにわざと起こしても良いがそのあとのPCのパフォーマンスが落ちるので推奨はしない
	delete GE;

	_CrtDumpMemoryLeaks();
	return 0;
}

#pragma endregion WinMain

#pragma region CreateWindow

// CreateWindow関数で引数の変数を変更することはまずないが読み取り専用である事を明示している
HWND MyProgram_CreateWindow(
	const HINSTANCE instance_,
	const MyGE::MyGameEngine::WindowState* windowState_,
	const MyGE::MyGameEngine::ScreenState* screenState_,
	const LPRECT rect_,
	const int showCmd_)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = windowState_->Style;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = windowState_->CbClsExtra;
	wcex.cbWndExtra = windowState_->CbWndExtra;
	wcex.hInstance = instance_;
	wcex.hIcon = windowState_->HIcon;
	wcex.hCursor = windowState_->HCursor;
	wcex.hbrBackground = screenState_->BackGroundColor;
	wcex.lpszMenuName = windowState_->MenuName;
	wcex.lpszClassName = windowState_->TaskName;
	wcex.hIconSm = windowState_->HIconSm;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(nullptr, "ウインドウ登録に失敗", nullptr, MB_OK);
		return nullptr;
	}

	DWORD dws = GE->screenState.WindowStyle;

	AdjustWindowRectEx(rect_, dws, false, WS_EX_APPWINDOW);
	HWND wnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		windowState_->TaskName,
		windowState_->WindowName,
		dws,
		0, 0,
		(rect_->right - rect_->left),
		(rect_->bottom - rect_->top),
		nullptr, nullptr, instance_, nullptr);

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
	LRESULT ret = (LRESULT)0;
	static int width, height;

	switch (msg_) {//メッセージハンドラ
	case WM_CREATE://ウィンドウ生成されたとき(開始時のみ)
		break;
	case WM_CLOSE://ウィンドウの✖が押されたとき
		GE->QuitOrder = true;
		break;
	case WM_DESTROY://ウィンドウ破棄されたとき(終了時のみ)
		PostQuitMessage(0);
		break;
	//case WM_PAINT://ウィンドウを再表示するとき(最小化最大化サイズ変更すべて)
		//break;
	case WM_MOUSEMOVE://ウィンドウ上でマウス移動したとき
		break;
	case WM_KEYDOWN://ウィンドウがアクティブでキー入力を受け付けつけたとき
		if (wParam_ == VK_ESCAPE)
		{
			GE->QuitOrder = true;
		}
		else if (wParam_ == VK_SPACE)
		{
			GE->screenState.FullScreenMode = ~GE->screenState.FullScreenMode;
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