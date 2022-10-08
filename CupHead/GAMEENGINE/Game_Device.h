#pragma once
#include <GAMEMATH.h>
#include <Game_Debug.h>

#include <Game_Ptr.h>
#include <Game_Mgr.h>
#include <GAMEWINDOW.h>

#pragma comment(lib, "GAMEBASE.lib")

// � �����쿡 3Dȭ���� ���ǵ� �װ� ���ؼ�
// �׷���ī�带 ���Ŵϱ� �� ������ ������ ��. 
// �׷��� ī�带 ���� �ִ� ���ٱ����� ������ �ֶ�� ����
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

	// �ɼ��� �� ������ڴ�. = true
	UINT m_MC;
	UINT m_MQ;
	Game_Vector m_Color;					// �ʱ�ȭ ����. 
	ID3D11Device*		  m_pDevice;	// �׷���ī���� �޸𸮸� ����Ѵ�. �׷��� ī���� �޸𸮸� ���� �� �Ҵ� �����ϴ�. �ε�
	ID3D11DeviceContext*  m_pContext;	// �׷���ī���� ������ ����Ѵ�. ������

	// ����ü�� ���̷�Ʈ�� ȭ���� ����ϴ� �ý����� ������
	// ��Ⱑ Ư���� ���� ������. 
	IDXGISwapChain*			m_pSwapChain;	// ȭ���� Ŭ����� ����� ����Ѵ�.(ex) api ���� ���۸�)

	// ȭ�鿡 ����ϱ� ���� ���ϳ��� ��θ� �츮�� �ذ��̴�. 
	// ����ϱ� ���� ȭ�� rgba
	ID3D11Texture2D*		m_BackBufferTex;

	// ���� ���� ��� �ִ�. z
	ID3D11Texture2D*		m_DepthStencilTex;

	// �׸� �̶�� ������ �ִٰ� ������.
	// �׸��� �޸��� �������� �̿��ϴ� �������̽��� ID3D11Texture2D*;
	// �׸��� �׸��ų� Ư���� ������� �̿��ϰ� �ϴ� �������̽��� ID3D11RenderTargetView*;

	// ���̷�Ʈ�� Ȧ���� ����
	// ����ҽ��� ������
	// �޸��� ����Ҹ� ��ǥ�ϴ� �������̽�(��ü Ȥ�� �Լ�)�� 
	ID3D11RenderTargetView* m_RTV;

	// ���̹���
	// Z���� ���� �����ɿ� ���� �������̽�
	ID3D11DepthStencilView* m_DSV;

	// -> ����Ʈ ���
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
