#include "GAMEWINDOW.h"
#include <Windows.h>
#include <assert.h>


GAMEWINDOW::GAMEWINDOWSTART GAMEWINDOW::Starter = GAMEWINDOW::GAMEWINDOWSTART();

bool GAMEWINDOW::IsProcess = true;

LRESULT CALLBACK GAMEWINDOW::HWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		// ���� ����Ǵ� �޼����Լ��� 0�� �����ϰ� �ϴ� �Լ���.
		IsProcess = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void GAMEWINDOW::HRegDef() 
{
	Game_Debug::LeckCheck();

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &GAMEWINDOW::HWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = nullptr;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"DEF";
	wcex.hIconSm = nullptr;

	RegisterClassExW(&wcex);
}


void GAMEWINDOW::HWINMESSAGE(INITFUNC _Init, DEADTIME _CallBack)
{
	if (nullptr != _Init)
	{
		_Init();
	}

	MSG msg;
	while (true == IsProcess)
	{
		if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			if (nullptr != _CallBack)
			{
				_CallBack();
			}
		}
	}
}

void GAMEWINDOW::Create(const Game_String& _WindowName, const Game_String& _ClassName)
{
	Game_Ptr<GAMEWINDOW> NewWindow = CreateToInsert(_WindowName, _ClassName);
}

GAMEWINDOW::GAMEWINDOW(const Game_String& _Name)
{
	// ��������� �κп��� m_HWND�� �ִ�.
	// ������ �ڵ��� �־��.
	// HDC�� ���ü� �ִ�.

	// � ��Ÿ�Ϸ� ����������� WS_OVERLAPPEDWINDOW

	Style = WS_OVERLAPPEDWINDOW;
	// �̷��� �ѹ��� �ȴ�.
	m_HWND = CreateWindowW(L"DEF", _Name, Style,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, nullptr, nullptr);

	if (!m_HWND)
	{
		assert(false);
	}

	// �����쿡 ���𰡸� �׸��� �ִ� �����̹Ƿ�
	// �����츦 ���ؼ� ���ü� �ִ�.
	m_HDC = GetDC(m_HWND);

	ShowWindow(m_HWND, SW_SHOW);
	UpdateWindow(m_HWND);
}

GAMEWINDOW::~GAMEWINDOW()
{
}


void GAMEWINDOW::Size(const Game_Vector& _Size) 
{
	// ������ �ٿ� �¿쿡�� �ٰ� �ִµ� �� ����� ���õ� ���̴�.
	// �װ� ����ϵ��� ¥�� �Ѵ�.

	// ���� �������� ũ�⸦ ���´�.
	// GetClientRect(m_HWND, )

	// 800 600
	// 804 620 => ��� �ٰ� ������ ����� ������ �ִϱ�
	// ���� �������� ũ�⿡�� �ٸ� ���Խ�Ų ������� �˾ƿ´�.

	m_Size = _Size;

	RECT Rect = {0, 0, _Size.IX(), _Size.IY() };

	// Rect���� ������ �����ɼ� �ִ°�.
	AdjustWindowRect(&Rect, Style, FALSE);

	SetWindowPos(m_HWND, nullptr, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOMOVE | SWP_NOZORDER);
}

// ������ ���� �ٸ���. �׷��Ƿ� �����쿡 �����ؾ� �Ѵ�.
Game_Vector GAMEWINDOW::MousePos()
{
	// Ŀ���� ��ġ
	// ��������� �̸��� �� �Ǿ��ִ�.
	// LP�� �پ������� LP�̸�
	POINT P;
	// ����Ϳ����� ���콺 ��ġ
	GetCursorPos(&P);
	// m_HWND ������ �������� 
	ScreenToClient(m_HWND, &P);

	return { (float)P.x, (float)P.y };
}

// SOLID ��ü������ �ټ����� ���� ������
// �������̽� �и���Ģ
// �������Լ��� ����� �װ͵��� �����ؼ� ���ο� ����� ������.

Game_Vector GAMEWINDOW::MousePosTo3D() 
{
	Game_Vector Pos = MousePos();

	Pos.X -= m_Size.HX();
	Pos.Y -= m_Size.HY();
	Pos.Y *= -1.0f;

	return Pos;
}

Game_Vector GAMEWINDOW::WindowPosTo3DPos(Game_Vector _WindowPos)
{
	return Game_Vector::WindowPosTo3DPos(_WindowPos, m_Size);
}