#include "MyWindowPalette.h"
using namespace MyWP;

// ゲームエンジンのポインタ(インスタンスを受け渡すとデカいので)
// スマートポインタでも良かったが正直WinMain内でnewして終了時にdeleteするだけのポインタで
// 結構な回数アクセスされるのにわざわざオーバーヘッドを増やす必要はない
MyGE::MyGameEngine* GE;

// エンジンのコンストラクタ(初期化処理)
MyGE::MyGameEngine::MyGameEngine(const HINSTANCE& inst_)
{
	//ウィンドウの設定情報
	this->windowState.TaskName = "MyGameEngine_ProtoType";
	this->windowState.WindowName = "MyGameEngine";
	this->windowState.MenuName = nullptr;
	this->windowState.Style = (CS_HREDRAW | CS_VREDRAW);
	this->windowState.CbClsExtra = 0;
	this->windowState.CbWndExtra = 0;
	this->windowState.HIcon = LoadIcon(inst_, MyWI::WindowIconTable[MyWI::Application]);
	this->windowState.HIconSm = LoadIcon(inst_, MyWI::WindowIconTable[MyWI::Application]);
	this->windowState.HCursor = LoadCursor(nullptr, MyWC::WindowCursorTable[MyWC::Arrow]);

	//ウィンドウの描画情報
	this->screenState.StartPosX = 0;
	this->screenState.StartPosY = 0;
	this->screenState.WidthDef = 960;
	this->screenState.WidthMin = 480;
	this->screenState.WidthMax = 1920;
	this->screenState.HeightDef = 540;
	this->screenState.HeightMin = 270;
	this->screenState.HeightMax = 1080;
	this->screenState.ViewScaleX = 1;
	this->screenState.ViewScaleY = 1;
	this->screenState.MultiSample = 1;
	this->screenState.FullScreenMode = false;
	this->screenState.BackGroundColor = CreateSolidBrush(RGB(0xFF, 0xFF, 0x00));
	this->screenState.WindowStyle = MyWS::WindowStyleTable[MyWS::Window];

	RECT dtr;
	if (GetWindowRect(GetDesktopWindow(), &dtr)) {
		this->screenState.FullScaleX = int((float)dtr.right / (float)this->screenState.WidthDef);
		this->screenState.FullScaleY = int((float)dtr.bottom / (float)this->screenState.HeightDef);
	}

	this->QuitRequire = false;
}

// エンジンのデコンストラクタ(廃棄時処理)
MyGE::MyGameEngine::~MyGameEngine()
{
	//DeleteObject(this->screenState.BackGroundColor);
}

void MyGE::MyGameEngine::Step(HWND wnd_)
{
	static int cnt = 0;
	HDC hdc = GetDC(wnd_);
	int x = cnt % 800;
	int y = cnt / 800;
	SetPixel(hdc, x, y, 0x000000FF);
	ReleaseDC(wnd_, hdc);
	cnt++;
	Sleep(16);
	if (this->QuitRequire)
	{
		DestroyWindow(wnd_);
	}
}

RECT MyGE::MyGameEngine::WindowDefRect()
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