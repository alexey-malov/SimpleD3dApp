
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "SimpleD3dApp.h"
#include "ChildView.h"
#ifdef NDEBUG
#include "shaders/Release/SimpleVertexShader.h"
#else
#include "shaders/Debug/SimpleVertexShader.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	FLOAT clearColor[4] = {0.5f, 0.5f, 0.1f, 1.0f};
	m_device->ClearRenderTargetView(m_currentRenderTargetView, clearColor);

	m_swapChain->Present(1, 0);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	try
	{
		InitD3D();
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

struct VertexInfo
{
	float x, y, z;
};

void CChildView::InitD3D()
{
	ATLENSURE_SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)));
	// предыдущая строчка раскрывается в аналоги нижеследующей
	//ATLENSURE_SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory1), (void**)&m_dxgiFactory));
	
	m_device = CreateDevice();
	ATLENSURE(m_device != nullptr);

	m_swapChain = CreateSwapChain();
	m_currentRenderTargetView = CreateRenderTargetView();

	CreateVertexBuffer();

}

CComPtr<ID3D10Buffer> CChildView::CreateVertexBuffer()
{
	static const VertexInfo vertices[] = {
		{ -0.5f, -0.5f, 0.0f },
		{ +0.5f, -0.5f, 0.0f },
		{ +0.0f, +0.5f, 0.0f },
	};
	D3D10_INPUT_ELEMENT_DESC vertexBufferDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D10_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(vertices);
	desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;

	CComPtr<ID3D10Buffer> buffer;
	ATLENSURE_SUCCEEDED(m_device->CreateBuffer(&desc, nullptr, &buffer));


	CComPtr<ID3D10InputLayout> inputLayout;
	m_device->CreateInputLayout(vertexBufferDesc, 1, g_main, sizeof(g_main), &inputLayout);

	return buffer;
}

CComPtr<ID3D10RenderTargetView> CChildView::CreateRenderTargetView()
{
	CComPtr<ID3D10Texture2D> backBuffer;
	ATLENSURE_SUCCEEDED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	CComPtr<ID3D10RenderTargetView> renderTargetView;
	ATLENSURE_SUCCEEDED(m_device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView));
	return renderTargetView;
}

CComPtr<IDXGISwapChain> CChildView::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = m_hWnd;
	desc.SampleDesc.Count = 1; // no multisampling
	desc.SampleDesc.Quality = 0; // default quality
	desc.Windowed = TRUE;

	CComPtr<IDXGISwapChain> swapChain;
	ATLENSURE_SUCCEEDED(m_dxgiFactory->CreateSwapChain(m_device, &desc, &swapChain));
	return swapChain;
}

// Создание первого устройства, которое бы поддерживало требуемый набор фич
CComPtr<ID3D10Device1> CChildView::CreateDevice()
{
	CComPtr<IDXGIAdapter> adapter;
	UINT adapterIndex = 0;
	while (m_dxgiFactory->EnumAdapters(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		// some hardware support 10, but don't support 9
		static const D3D10_FEATURE_LEVEL1 LEVELS[] =
		{
			D3D10_FEATURE_LEVEL_10_1,
			D3D10_FEATURE_LEVEL_10_0,
			D3D10_FEATURE_LEVEL_9_3,
			D3D10_FEATURE_LEVEL_9_2,
			D3D10_FEATURE_LEVEL_9_1,
		};

		for (auto featureLevel : LEVELS)
		{
			CComPtr<ID3D10Device1> device;
			if (SUCCEEDED(D3D10CreateDevice1(
				adapter,
				D3D10_DRIVER_TYPE_HARDWARE,
				nullptr,
				D3D10_CREATE_DEVICE_BGRA_SUPPORT | D3D10_CREATE_DEVICE_SINGLETHREADED,
				featureLevel,
				D3D10_1_SDK_VERSION,
				&device
			)))
			{
				return device;
			}
		}

		adapter.Release();
	}
	return nullptr;
}


BOOL CChildView::OnEraseBkgnd(CDC* /*pDC*/)
{
	// мы уже сами "очистили фон"
	return TRUE;
}
