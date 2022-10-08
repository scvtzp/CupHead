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
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		// 현재 실행되는 메세지함수가 0을 리턴하게 하는 함수다.
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
	// 만들어지는 부분에서 m_HWND이 있다.
	// 윈도우 핸들이 있어야.
	// HDC를 얻어올수 있다.

	// 어떤 스타일로 만들어졌는지 WS_OVERLAPPEDWINDOW

	Style = WS_OVERLAPPEDWINDOW;
	// 이러한 한번만 된다.
	m_HWND = CreateWindowW(L"DEF", _Name, Style,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, nullptr, nullptr);

	if (!m_HWND)
	{
		assert(false);
	}

	// 윈도우에 무언가를 그릴수 있는 권한이므로
	// 윈도우를 통해서 얻어올수 있다.
	m_HDC = GetDC(m_HWND);

	ShowWindow(m_HWND, SW_SHOW);
	UpdateWindow(m_HWND);
}

GAMEWINDOW::~GAMEWINDOW()
{
}


void GAMEWINDOW::Size(const Game_Vector& _Size) 
{
	// 윈도우 바와 좌우에도 바가 있는데 그 사이즈가 무시된 것이다.
	// 그걸 고려하도록 짜야 한다.

	// 현재 윈도우의 크기를 얻어온다.
	// GetClientRect(m_HWND, )

	// 800 600
	// 804 620 => 요거 바가 합쳐진 사이즈를 나에게 주니까
	// 기존 윈도우의 크기에서 바를 포함시킨 사이즈로 알아온다.

	m_Size = _Size;

	RECT Rect = {0, 0, _Size.IX(), _Size.IY() };

	// Rect들어갔다 나오면 수정될수 있는것.
	AdjustWindowRect(&Rect, Style, FALSE);

	SetWindowPos(m_HWND, nullptr, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOMOVE | SWP_NOZORDER);
}

// 윈도우 마다 다르다. 그러므로 윈도우에 구현해야 한다.
Game_Vector GAMEWINDOW::MousePos()
{
	// 커서의 위치
	// 상식적으로 이름이 다 되어있다.
	// LP가 붙어있으면 LP이름
	POINT P;
	// 모니터에서의 마우스 위치
	GetCursorPos(&P);
	// m_HWND 윈도우 기준으로 
	ScreenToClient(m_HWND, &P);

	return { (float)P.x, (float)P.y };
}

// SOLID 객체지향의 다섯가지 권장 설계중
// 인터페이스 분리원칙
// 자잘한함수를 만들고 그것들을 조합해서 새로운 기능을 만들어라.

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