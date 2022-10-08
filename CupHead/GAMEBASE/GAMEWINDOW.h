#pragma once
#include <Windows.h>
#include "Game_Debug.h"
#include <map>
#include <string>
#include "GAMEMATH.h"
#include "Game_Mgr.h"
#include "Game_Ptr.h"

// �Լ������͸� TYPEDEF
//typedef int(__stdcall* DEADTIME)(); 
//typedef int(__stdcall* INITFUNC)();
// �ΰ����� ������ �Ȱ�����
// typedef �̸��� DEADTIME INITFUNC

typedef int(__stdcall* DEADTIME)();
typedef DEADTIME INITFUNC; // 

class GAMEWINDOW : public HFOBJMGR<GAMEWINDOW>
{
	////////////////////////////////////////////////////// static
private:
	// ���α׷��ӵ��� Ʈ���̴�.
	// �޸𸮿����� Ư¡�� �̿��ؼ�
	// ���α׷��� �����ڸ��� Main���ٵ�
	// ���� ����Ǵ� �Լ��� ����� ������ ����Ҽ� �ִ� ����̴�.
	static void HRegDef();
	

	class GAMEWINDOWSTART 
	{
	public:
		GAMEWINDOWSTART() {
			HRegDef();
		}
	};
	// ����� ������ �����̴�.
	static GAMEWINDOWSTART Starter;

	// �̳༮�� �ƹ��� �ǵ�� �ȵȴ�.
	static bool IsProcess;
	static LRESULT CALLBACK HWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static void Create(const Game_String& _WindowName, const Game_String& _ClassName = L"DEF");

	// GAMEWINDOW*�� �̸��ϳ� ���δ�.
public:
	static void HWINMESSAGE(INITFUNC _Init, DEADTIME _CallBack);

	////////////////////////////////////////////////////// member

private:
	HWND m_HWND;
	HDC m_HDC;
	int Style;
	Game_Vector m_Size;

public:
	HWND WINHWND() {
		return m_HWND;
	}

	HDC WINHDC() {
		return m_HDC;
	}

	Game_Vector Size() {
		return m_Size;
	}

	Game_Vector HSize() {
		return m_Size.HalfVector();
	}

	Game_Vector MousePos();
	Game_Vector MousePosTo3D();

	void Size(const Game_Vector& _Size);
	Game_Vector WindowPosTo3DPos(Game_Vector _WindowPos);

	// ������� ����°��� ���� �������� �ʴ´�.
public:
	GAMEWINDOW(const Game_String& _Name);
	~GAMEWINDOW();
};
