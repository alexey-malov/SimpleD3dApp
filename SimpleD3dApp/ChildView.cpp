
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "SimpleD3dApp.h"
#include "ChildView.h"

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

void CChildView::InitD3D()
{
	ATLENSURE_SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)));
	// предыдущая строчка раскрывается в аналоги нижеследующей
	//ATLENSURE_SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory1), (void**)&m_dxgiFactory));
	
	m_device = CreateDevice();
	ATLENSURE(m_device != nullptr);
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = m_hWnd;
	desc.SampleDesc.Count = 1; // no multisampling
	desc.SampleDesc.Quality = 0; // default quality
	desc.Windowed = TRUE;
	ATLENSURE_SUCCEEDED(m_dxgiFactory->CreateSwapChain(m_device, &desc, &m_swapChain));
	
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
