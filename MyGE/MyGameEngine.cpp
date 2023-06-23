#include "MyWindowPalette.h"
using namespace MyGE;
using namespace MyWP;

// �Q�[���G���W���̃|�C���^(�C���X�^���X���󂯓n���ƃf�J���̂�)
// �X�}�[�g�|�C���^�ł��ǂ�����������WinMain����new���ďI������delete���邾���̃|�C���^��
// ���\�ȉ񐔃A�N�Z�X�����̂ɂ킴�킴�I�[�o�[�w�b�h�𑝂₷�K�v�͂Ȃ�
MyGameEngine* GE;

// �G���W���̃f�R���X�g���N�^(�p��������)
MyGameEngine::~MyGameEngine()
{
	//�ǂ����DeleteObject���Ȃ��Ă����������[�N�͂��Ȃ��炵��
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

	//�E�B���h�E�̐ݒ���
	ge->windowState.TaskName = "ASOBI";
	ge->windowState.WindowName = "ASOBI";
	ge->windowState.MenuName = nullptr;
	ge->windowState.Style = (CS_HREDRAW | CS_VREDRAW);
	ge->windowState.CbClsExtra = 0;
	ge->windowState.CbWndExtra = 0;
	ge->windowState.HIcon = LoadIcon(instance_, MyWI::WindowIconTable[MyWI::Application]);
	ge->windowState.HIconSm = LoadIcon(instance_, MyWI::WindowIconTable[MyWI::Application]);
	ge->windowState.HCursor = LoadCursor(nullptr, MyWC::WindowCursorTable[MyWC::Arrow]);

	//�E�B���h�E�̕`����
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
// WNDCLASSEXW�\����(�E�B���h�E�����o�^)�̃h�L�������g

// https://learn.microsoft.com/ja-jp/windows/win32/menurc/using-icons
// http://wisdom.sakura.ne.jp/system/winapi/win32/win73.html
// �A�C�R���ƃJ�[�\���ɂ��ďڂ��������Ă���h�L�������g

// https://nmrnbpw.blog.fc2.com/blog-entry-312.html
// hIconSm�ɂ��Ă�URL�@�����܂ł������悤�ɂȂ������l�O����

// http://marupeke296.com/WINT_GetWindowHandle.html
// cbClsExtra �y�� cbWndExtra �̐��������̃y�[�W��葽��WNDCLASSEXW�\���̂̉��URL
// ����̃A�v�������ɑ΂��ăA�v���[�`���������Ƃ��ɂ��̃A�v���̃n���h�����|�C���^�ŕێ�������Ƃ��Ɏg����Ƃ�����Ȋ����H
// �p���Q&A�̃T�C�g�����������ǐ����悭�킩��Ȃ�

// http://www.c-lang.org/constant.html
// �����C���q�ɂ��Ă�URL��

// https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q1312241603
// LPSTR��NEAR��FAR���ĂȂ�

// http://wisdom.sakura.ne.jp/system/winapi/win32/win27.html
// https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/nf-wingdi-createsolidbrush
// �u���V�ނɂ��Ă�URL��
// �w�Z�̃G���W����
// (HBRUSH)GetStockObject(BLACK_BRUSH);
// �Ŕw�i�u���V�̃o���G�[�V������GRAY_BRUSH���̔����ɋ߂��F���F�����Ȃ����@�Ŕw�i��`�悵�Ă��邪�A
// CreateSolidBrush()�ŐF���w�肷�鎖�ōD���ȐF�ɕύX�ł���
// HBURSH�^�̃I�u�W�F�N�g�͕K�v�Ȃ��Ȃ�����DeleteObject()�Ŕj�����Ȃ���΂����Ȃ�...�炵��
// ���������[�N����������\��������̂�������Ȃ����A���ۂ͂ǂ��Ȃ��Ă��邩�킩��Ȃ�
// �w�Z�̃G���W��(GP1)�Ƃ��͂�������������������Ă��Ȃ����A���������[�N���N���������Ƃ����\�������̂�
// �����������痠�ł�����Ƃ����������[�N���N�����Ă���̂�������Ȃ�
// �Ȃ̂Ńf�X�g���N�^�ɔj���������ꉞ�����Ă���

// http://wisdom.sakura.ne.jp/system/winapi/win32/win76.html
// ���j���[

// http://wisdom.sakura.ne.jp/system/winapi/win32/win58.html
// �E�B���h�E�^�C�g����ύX����

// https://xelalko.blogspot.com/2013/07/tchar.html
// TCHAR LPSTR���ĂȂ�

// https://daeudaeu.com/extern/
// extern�ɂ��Ă�URL��
// include�ɂ��Ă̗���������΂Ȃ��w�b�_�[�t�@�C���ŕϐ���錾���Ȃ���Cpp�ɐ錾�����ϐ���extern�ŊO���Q�Ƃł���悤�ɂ��Ă��邩���킩��
// �v�͕ʂ�Cpp�ɂ���ϐ��͊O����Cpp����extern�L�[���[�h�Ő錾����ΊO�����爵�������Ă���

// �E�B���h�E�X�^�C����񋓌^�Ɣz���p���ĕ�����₷���錾�����������̂Ń`���b�gGPT�ɂ������@�Ȃ��H
// ���ĕ�������v�f�̍Ō��Count��ǉ����邱�Ƃŗv�f���̑傫������������ċA���Ă�������
// �u���̎肪���������I�I�v�ƂȂ����B
// �v�f���𓾂邽�߂�Count�͍Ō�ɐ錾���邱�ƁB

// https://dxlib.xsrv.jp/cgi/patiobbs/patio.cgi?
// DrectX�̃��C�u�����u����炵��
// �f�B�X�v���C�𔻕ʂ�����Ȃ񂾂�������Ă����낢
#pragma endregion Docment