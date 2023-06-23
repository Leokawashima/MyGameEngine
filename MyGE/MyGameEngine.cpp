#include "MyWindowPalette.h"
using namespace MyGE;
using namespace MyWP;

// ゲームエンジンのポインタ(インスタンスを受け渡すとデカいので)
// スマートポインタでも良かったが正直WinMain内でnewして終了時にdeleteするだけのポインタで
// 結構な回数アクセスされるのにわざわざオーバーヘッドを増やす必要はない
MyGameEngine* GE;

// エンジンのデコンストラクタ(廃棄時処理)
MyGameEngine::~MyGameEngine()
{
	//どうやらDeleteObjectしなくてもメモリリークはしないらしい
	//DeleteObject(this->screenState.BackGroundColor);
}

void MyGameEngine::Step(HWND wnd_)
{
	static int cnt = 0;
	HDC hdc = GetDC(wnd_);
	int x = cnt % 800;
	int y = cnt / 800;
	SetPixel(hdc, x, y, 0x000000FF);
	ReleaseDC(wnd_, hdc);
	cnt++;
	Sleep(16);
	if (QuitOrder)
	{
		DestroyWindow(wnd_);
	}
}

MyGameEngine* MyGameEngine::DefaultInitialize(const HINSTANCE instance_)
{
	auto ge = new MyGameEngine();

	//ウィンドウの設定情報
	ge->windowState.TaskName = "ASOBI";
	ge->windowState.WindowName = "ASOBI";
	ge->windowState.MenuName = nullptr;
	ge->windowState.Style = (CS_HREDRAW | CS_VREDRAW);
	ge->windowState.CbClsExtra = 0;
	ge->windowState.CbWndExtra = 0;
	ge->windowState.HIcon = LoadIcon(instance_, MyWI::WindowIconTable[MyWI::Application]);
	ge->windowState.HIconSm = LoadIcon(instance_, MyWI::WindowIconTable[MyWI::Application]);
	ge->windowState.HCursor = LoadCursor(nullptr, MyWC::WindowCursorTable[MyWC::Arrow]);

	//ウィンドウの描画情報
	ge->screenState.StartPosX = 0;
	ge->screenState.StartPosY = 0;
	ge->screenState.WidthDef = 240;//960
	ge->screenState.WidthMin = 480;
	ge->screenState.WidthMax = 1920;
	ge->screenState.HeightDef = 240;//540
	ge->screenState.HeightMin = 270;
	ge->screenState.HeightMax = 1080;
	ge->screenState.ViewScaleX = 1;
	ge->screenState.ViewScaleY = 1;
	ge->screenState.MultiSample = 1;
	ge->screenState.FullScreenMode = false;
	ge->screenState.BackGroundColor = CreateSolidBrush(RGB(0xFF, 0xFF, 0x00));
	ge->screenState.WindowStyle = MyWS::WindowStyleTable[MyWS::Window];

	RECT dtr;
	if (GetWindowRect(GetDesktopWindow(), &dtr)) {
		ge->screenState.FullScaleX = int((float)dtr.right / (float)ge->screenState.WidthDef);
		ge->screenState.FullScaleY = int((float)dtr.bottom / (float)ge->screenState.HeightDef);
	}

	ge->QuitOrder = false;

	return ge;
}

RECT MyGameEngine::DefaultWindowRect()
{
	return RECT{
		GE->screenState.StartPosX,
		GE->screenState.StartPosY,
		(int)(GE->screenState.WidthDef * GE->screenState.ViewScaleX),
		(int)(GE->screenState.HeightDef * GE->screenState.ViewScaleY)
	};
}

#pragma region Docment
// https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-wndclassexw
// http://kaitei.net/winapi/creating-windows/
// WNDCLASSEXW構造体(ウィンドウ生成登録)のドキュメント

// https://learn.microsoft.com/ja-jp/windows/win32/menurc/using-icons
// http://wisdom.sakura.ne.jp/system/winapi/win32/win73.html
// アイコンとカーソルについて詳しく書いてあるドキュメント

// https://nmrnbpw.blog.fc2.com/blog-entry-312.html
// hIconSmについてのURL　ここまでいじれるようになったら一人前そう

// http://marupeke296.com/WINT_GetWindowHandle.html
// cbClsExtra 及び cbWndExtra の説明が他のページより多いWNDCLASSEXW構造体の解説URL
// 特定のアプリ何かに対してアプローチしたい時とかにそのアプリのハンドルをポインタで保持したりとかに使えるとかそんな感じ？
// 英語のQ&Aのサイトもあったけど正直よくわからない

// http://www.c-lang.org/constant.html
// 末尾修飾子についてのURL↑

// https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q1312241603
// LPSTRのNEARやFARってなんぞ

// http://wisdom.sakura.ne.jp/system/winapi/win32/win27.html
// https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/nf-wingdi-createsolidbrush
// ブラシ類についてのURL↑
// 学校のエンジンは
// (HBRUSH)GetStockObject(BLACK_BRUSH);
// で背景ブラシのバリエーションがGRAY_BRUSH等の白黒に近い色数色しかない方法で背景を描画しているが、
// CreateSolidBrush()で色を指定する事で好きな色に変更できる
// HBURSH型のオブジェクトは必要なくなったらDeleteObject()で破棄しなければいけない...らしい
// メモリリークが発生する可能性があるのかもしれないが、実際はどうなっているかわからない
// 学校のエンジン(GP1)とかはそういう処理が書かれていないが、メモリリークを起こしたことが結構あったので
// もしかしたら裏でちょっとずつメモリリークを起こしているのかもしれない
// なのでデストラクタに破棄処理を一応書いている

// http://wisdom.sakura.ne.jp/system/winapi/win32/win76.html
// メニュー

// http://wisdom.sakura.ne.jp/system/winapi/win32/win58.html
// ウィンドウタイトルを変更する

// https://xelalko.blogspot.com/2013/07/tchar.html
// TCHAR LPSTRってなんぞ

// https://daeudaeu.com/extern/
// externについてのURL↑
// includeについての理解があればなぜヘッダーファイルで変数を宣言しないでCppに宣言した変数をexternで外部参照できるようにしているかがわかる
// 要は別のCppにある変数は外部のCpp内でexternキーワードで宣言すれば外部から扱えるよってこと

// ウィンドウスタイルを列挙型と配列を用いて分かりやすく宣言したかったのでチャットGPTにいい方法ない？
// って聞いたら要素の最後にCountを追加することで要素数の大きさが得られるって帰ってきたから
// 「その手があったか！！」となった。
// 要素数を得るためにCountは最後に宣言すること。

// https://dxlib.xsrv.jp/cgi/patiobbs/patio.cgi?
// DrectXのライブラリ置き場らしい
// ディスプレイを判別したりなんだりもあっておもろい
#pragma endregion Docment