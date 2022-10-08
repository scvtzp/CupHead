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
	// 지우는색
	// 디바이스 INIT으로 들어오게 되고.
	// 여기서 디바이스가 만들어지게 되면

	if (nullptr == _Window)
	{
		AMSG(L"존재하지 않는 윈도우에 디바이스를 장착시키려고 만드려고 했습니다");
	}

	m_WINDOW = _Window;

	int iFlag = 0;

#ifdef _DEBUG
	// 디바이스 만드는 모드가 디버그 모드 
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 디바이스 레벨
	D3D_FEATURE_LEVEL eLV = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	if (S_OK != D3D11CreateDevice(
		nullptr, 
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		iFlag,
		nullptr, // 가능한 최대버전 알아내는 것
		0,
		D3D11_SDK_VERSION, // 현재 윈도우에 설치된 SDK 버전
		&m_pDevice,
		&eLV,
		&m_pContext))
	{
		AMSG(L"디바이스와 컨텍스트 생성에 실패했습니다.");
	}

	m_MC = 4; // MC
	// 
	if (S_OK != m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R32G32B32A32_UINT, m_MC, &m_MQ))
	{
		m_MC = 1; // MC
		if (S_OK != m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UINT, m_MC, &m_MQ))
		{
			AMSG(L"멀티샘플링 체크가 제대로 되지 않았습니다.");
		}
	}

	CreateSwapChain();
	CreateDepthStencil();
	CreateViewPort();

	// 하나의 디바이스도 만들어지지 않았다면
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
	// 윈도우 좌표계 기반이다.
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

	// 여러분들 모티터 기기 정보를 얻어와서
	SCDESC.BufferDesc.RefreshRate.Numerator = 60;
	SCDESC.BufferDesc.RefreshRate.Denominator = 1;

	SCDESC.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SCDESC.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SCDESC.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 이걸 만드는 용도가 뭐냐?
	SCDESC.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	SCDESC.SampleDesc.Quality = 0;
	SCDESC.SampleDesc.Count = 1;
	SCDESC.OutputWindow = m_WINDOW->WINHWND();

	SCDESC.BufferCount = 2;

	SCDESC.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	SCDESC.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// 전체화면
	// true면 안한다.
	SCDESC.Windowed = true;

	IDXGIDevice* pD = nullptr;
	IDXGIAdapter* pA = nullptr;
	IDXGIFactory* pF = nullptr;

	// 우리에게 제공하는 말단 API
	// 얻어오는 용도
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
		AMSG(L"스왑체인 생성에 실패했습니다.");
	}

	pF->Release();
	pA->Release();
	pD->Release();

}

void Game_Device::CreateDepthStencil() 
{
	if (S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_BackBufferTex))
	{
		AMSG(L"스왑체인에 텍스처가 존재하지 않습니다.");
	}

	// 이 텍스처를 기반으로
	if (S_OK != m_pDevice->CreateRenderTargetView(m_BackBufferTex, 0, &m_RTV))
	{
		AMSG(L"백버퍼 이용권한(RTV)을 얻어오지 못했습니다.");
	}

	// 깊이 
	// 내부의 맴버변수가 모두 public일때만 된다.
	// 그리고 아무런 생성자도 없어야 한다.
	D3D11_TEXTURE2D_DESC TD = {0,};

	TD.ArraySize = 1;

	TD.Width = m_WINDOW->Size().UIX();
	TD.Height = m_WINDOW->Size().UIY();

	TD.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TD.SampleDesc.Count = 1;
	TD.SampleDesc.Quality = 0;
	TD.MipLevels = 1;

	// 무조건 그래픽 카드에 
	TD.Usage = D3D11_USAGE_DEFAULT;

	TD.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_pDevice->CreateTexture2D(&TD, 0, &m_DepthStencilTex);
	if (nullptr == m_DepthStencilTex)
	{
		AMSG(L"깊이 버퍼 텍스처를 만들지 못했습니다.");
	}

	m_pDevice->CreateDepthStencilView(m_DepthStencilTex, 0, &m_DSV);
	if (nullptr == m_DSV)
	{
		AMSG(L"깊이 버퍼 사용권한(DSV)을 만들지 못했습니다.");
	}


}

void Game_Device::RenderStart() 
{
	//ID3D11RasterizerState
	//m_pContext->RSSetState()
	// 윈도우 크기만한 화면에 뭔가를 출력하겠다.
	m_pContext->RSSetViewports(1, &m_VIEWPORT);
	// 그리기전에 화면을 한번 파란색으로 지운다.
	m_pContext->ClearRenderTargetView(m_RTV, m_Color.Arr);
	// 깊이와 스텐실을 초기화한다.
	m_pContext->ClearDepthStencilView(m_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 거기 위에 그리겠다.
	// 지금 부터 그릴 도화지는 여기야.
	m_pContext->OMSetRenderTargets(1, &m_RTV, m_DSV);

	// VTXSHADER

	// 내가 이걸 해주는순간 돌아가나요?
	// DRAW할때의 준비다.
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