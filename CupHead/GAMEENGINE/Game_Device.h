#pragma once
#include <GAMEMATH.h>
#include <Game_Debug.h>

#include <Game_Ptr.h>
#include <Game_Mgr.h>
#include <GAMEWINDOW.h>

#pragma comment(lib, "GAMEBASE.lib")

// 어떤 윈도우에 3D화면을 띄울건데 그걸 위해서
// 그래픽카드를 쓸거니까 그 권한을 나한테 줘. 
// 그래픽 카드를 쓸수 있는 접근권한을 나한테 주라는 개념
class Game_Device : public HFOBJMGR<Game_Device>
{
public:
	static bool bDefResInit;

	static Game_Ptr<Game_Device> Create(const Game_String& _Window)
	{
		Game_Ptr<Game_Device> DEVICE = CreateToInsert(_Window);
		DEVICE->Init(GAMEWINDOW::Find(_Window));
		return DEVICE;
	}

private:
	Game_Ptr<GAMEWINDOW> m_WINDOW;

	// 옵션이 그 기술쓰겠다. = true
	UINT m_MC;
	UINT m_MQ;
	Game_Vector m_Color;					// 초기화 색깔. 
	ID3D11Device*		  m_pDevice;	// 그래픽카드의 메모리를 담당한다. 그래픽 카드의 메모리를 조작 및 할당 가능하다. 로딩
	ID3D11DeviceContext*  m_pContext;	// 그래픽카드의 연산을 담당한다. 랜더링

	// 스왑체인 다이렉트의 화면을 출력하는 시스템의 개념을
	// 기기가 특별해 지기 때문에. 
	IDXGISwapChain*			m_pSwapChain;	// 화면의 클리어와 출력을 담당한다.(ex) api 더블 버퍼링)

	// 화면에 출력하기 위한 단하나의 통로를 우리게 준것이다. 
	// 출력하기 위한 화면 rgba
	ID3D11Texture2D*		m_BackBufferTex;

	// 깊이 값을 들고 있다. z
	ID3D11Texture2D*		m_DepthStencilTex;

	// 그림 이라는 개념이 있다고 쳤을때.
	// 그림을 메모리적 관점에서 이용하는 인터페이스는 ID3D11Texture2D*;
	// 그림을 그리거나 특수한 기능으로 이용하게 하는 인터페이스는 ID3D11RenderTargetView*;

	// 다이렉트가 홀수가 명작
	// 어떤리소스가 있을때
	// 메모리의 저장소를 대표하는 인터페이스(객체 혹은 함수)가 
	ID3D11RenderTargetView* m_RTV;

	// 깊이버퍼
	// Z값에 대한 연산기능에 대한 인터페이스
	ID3D11DepthStencilView* m_DSV;

	// -> 뷰포트 행렬
	D3D11_VIEWPORT m_VIEWPORT;

public:
	ID3D11Device* Device() {
		return m_pDevice;	
	}
	ID3D11DeviceContext* Context() {
		return m_pContext;
	}

public:
	Game_Device();
	~Game_Device();

public:
	void CreateSwapChain();
	void CreateDepthStencil();
	void CreateViewPort();

public:
	void RenderStart();
	void RenderEnd();

	void MESHINIT();
	void SHADERINIT();
	void RASTERIZERINIT();
	void DEPTHINIT();
	void BLENDINIT();
	void SAMPLERINIT();

	static void Reset();

private:
	void Init(const Game_Ptr<GAMEWINDOW>& _Window);

};
