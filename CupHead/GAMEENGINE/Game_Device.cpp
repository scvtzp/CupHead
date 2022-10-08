#include "Game_Device.h"

bool Game_Device::bDefResInit = false;

Game_Device::Game_Device() 
	: m_MC(0)
	, m_MQ(0)
	, m_Color(Game_Vector::WHITE)
	, m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_BackBufferTex(nullptr)
	, m_DepthStencilTex(nullptr)
	, m_RTV(nullptr)
	, m_DSV(nullptr)
{

}

Game_Device::~Game_Device() 
{
	if (nullptr != m_DSV)	{		m_DSV->Release();			   }
	if (nullptr != m_RTV)	{		m_RTV->Release();			   }
	if (nullptr != m_DepthStencilTex)	{		m_DepthStencilTex->Release();  }
	if (nullptr != m_BackBufferTex)	{		m_BackBufferTex->Release();	   }
	if (nullptr != m_pSwapChain)	{		m_pSwapChain->Release();	   }
	if (nullptr != m_pContext)	{		m_pContext->Release();		   }
	if (nullptr != m_pDevice)	{		m_pDevice->Release();		   }

}

void Game_Device::Init(const Game_Ptr<GAMEWINDOW>& _Window) 
{
	// ����»�
	// ����̽� INIT���� ������ �ǰ�.
	// ���⼭ ����̽��� ��������� �Ǹ�

	if (nullptr == _Window)
	{
		AMSG(L"�������� �ʴ� �����쿡 ����̽��� ������Ű���� ������� �߽��ϴ�");
	}

	m_WINDOW = _Window;

	int iFlag = 0;

#ifdef _DEBUG
	// ����̽� ����� ��尡 ����� ��� 
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// ����̽� ����
	D3D_FEATURE_LEVEL eLV = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	if (S_OK != D3D11CreateDevice(
		nullptr, 
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		iFlag,
		nullptr, // ������ �ִ���� �˾Ƴ��� ��
		0,
		D3D11_SDK_VERSION, // ���� �����쿡 ��ġ�� SDK ����
		&m_pDevice,
		&eLV,
		&m_pContext))
	{
		AMSG(L"����̽��� ���ؽ�Ʈ ������ �����߽��ϴ�.");
	}

	m_MC = 4; // MC
	// 
	if (S_OK != m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R32G32B32A32_UINT, m_MC, &m_MQ))
	{
		m_MC = 1; // MC
		if (S_OK != m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UINT, m_MC, &m_MQ))
		{
			AMSG(L"��Ƽ���ø� üũ�� ����� ���� �ʾҽ��ϴ�.");
		}
	}

	CreateSwapChain();
	CreateDepthStencil();
	CreateViewPort();

	// �ϳ��� ����̽��� ��������� �ʾҴٸ�
	if (false == bDefResInit)
	{
		MESHINIT();
		SHADERINIT();
		RASTERIZERINIT();
		DEPTHINIT();
		BLENDINIT();
		SAMPLERINIT();
		bDefResInit = true;
	}
}

void Game_Device::CreateViewPort() 
{
	m_VIEWPORT = {0,};
	// ������ ��ǥ�� ����̴�.
	m_VIEWPORT.TopLeftX = 0.0f;
	m_VIEWPORT.TopLeftY = 0.0f;
	m_VIEWPORT.Width = m_WINDOW->Size().X;
	m_VIEWPORT.Height = m_WINDOW->Size().Y;
	m_VIEWPORT.MinDepth = 0.0f;
	m_VIEWPORT.MaxDepth = 1.0f;
}

void Game_Device::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC SCDESC = {0,};

	SCDESC.BufferDesc.Width = m_WINDOW->Size().UIX();
	SCDESC.BufferDesc.Height = m_WINDOW->Size().UIY();

	// �����е� ��Ƽ�� ��� ������ ���ͼ�
	SCDESC.BufferDesc.RefreshRate.Numerator = 60;
	SCDESC.BufferDesc.RefreshRate.Denominator = 1;

	SCDESC.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SCDESC.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SCDESC.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// �̰� ����� �뵵�� ����?
	SCDESC.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	SCDESC.SampleDesc.Quality = 0;
	SCDESC.SampleDesc.Count = 1;
	SCDESC.OutputWindow = m_WINDOW->WINHWND();

	SCDESC.BufferCount = 2;

	SCDESC.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	SCDESC.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// ��üȭ��
	// true�� ���Ѵ�.
	SCDESC.Windowed = true;

	IDXGIDevice* pD = nullptr;
	IDXGIAdapter* pA = nullptr;
	IDXGIFactory* pF = nullptr;

	// �츮���� �����ϴ� ���� API
	// ������ �뵵
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pD);
	if (nullptr == pD)
	{
		AMSG(L"if (nullptr == pD)");
	}

	pD->GetParent(__uuidof(IDXGIAdapter), (void**)&pA);
	if (nullptr == pA)
	{
		AMSG(L"if (nullptr == pA)");
	}

	pA->GetParent(__uuidof(IDXGIFactory), (void**)&pF);
	if (nullptr == pF)
	{
		AMSG(L"if (nullptr == pF)");
	}

	if (S_OK != pF->CreateSwapChain(m_pDevice, &SCDESC, &m_pSwapChain))
	{
		AMSG(L"����ü�� ������ �����߽��ϴ�.");
	}

	pF->Release();
	pA->Release();
	pD->Release();

}

void Game_Device::CreateDepthStencil() 
{
	if (S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_BackBufferTex))
	{
		AMSG(L"����ü�ο� �ؽ�ó�� �������� �ʽ��ϴ�.");
	}

	// �� �ؽ�ó�� �������
	if (S_OK != m_pDevice->CreateRenderTargetView(m_BackBufferTex, 0, &m_RTV))
	{
		AMSG(L"����� �̿����(RTV)�� ������ ���߽��ϴ�.");
	}

	// ���� 
	// ������ �ɹ������� ��� public�϶��� �ȴ�.
	// �׸��� �ƹ��� �����ڵ� ����� �Ѵ�.
	D3D11_TEXTURE2D_DESC TD = {0,};

	TD.ArraySize = 1;

	TD.Width = m_WINDOW->Size().UIX();
	TD.Height = m_WINDOW->Size().UIY();

	TD.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TD.SampleDesc.Count = 1;
	TD.SampleDesc.Quality = 0;
	TD.MipLevels = 1;

	// ������ �׷��� ī�忡 
	TD.Usage = D3D11_USAGE_DEFAULT;

	TD.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_pDevice->CreateTexture2D(&TD, 0, &m_DepthStencilTex);
	if (nullptr == m_DepthStencilTex)
	{
		AMSG(L"���� ���� �ؽ�ó�� ������ ���߽��ϴ�.");
	}

	m_pDevice->CreateDepthStencilView(m_DepthStencilTex, 0, &m_DSV);
	if (nullptr == m_DSV)
	{
		AMSG(L"���� ���� ������(DSV)�� ������ ���߽��ϴ�.");
	}


}

void Game_Device::RenderStart() 
{
	//ID3D11RasterizerState
	//m_pContext->RSSetState()
	// ������ ũ�⸸�� ȭ�鿡 ������ ����ϰڴ�.
	m_pContext->RSSetViewports(1, &m_VIEWPORT);
	// �׸������� ȭ���� �ѹ� �Ķ������� �����.
	m_pContext->ClearRenderTargetView(m_RTV, m_Color.Arr);
	// ���̿� ���ٽ��� �ʱ�ȭ�Ѵ�.
	m_pContext->ClearDepthStencilView(m_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// �ű� ���� �׸��ڴ�.
	// ���� ���� �׸� ��ȭ���� �����.
	m_pContext->OMSetRenderTargets(1, &m_RTV, m_DSV);

	// VTXSHADER

	// ���� �̰� ���ִ¼��� ���ư�����?
	// DRAW�Ҷ��� �غ��.
	// m_pContext->IASetVertexBuffers
	// m_pContext->PSSetShader()
	// m_pContext->VSSetShader()

	// m_pContext->OMSetRenderTargets(1, &m_RTV, m_DSV);


	// m_pContext->Draw()
}

void Game_Device::RenderEnd() 
{
	m_pSwapChain->Present(0, 0);

}

void Game_Device::Reset()
{
	Game_Device::MAINOBJ()->Context()->PSSetShader(nullptr, 0, 0);
	Game_Device::MAINOBJ()->Context()->GSSetShader(nullptr, 0, 0);
	Game_Device::MAINOBJ()->Context()->HSSetShader(nullptr, 0, 0);
	Game_Device::MAINOBJ()->Context()->DSSetShader(nullptr, 0, 0);
	Game_Device::MAINOBJ()->Context()->VSSetShader(nullptr, 0, 0);
}