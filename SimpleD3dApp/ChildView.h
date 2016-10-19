
// ChildView.h : interface of the CChildView class
//


#pragma once


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitD3D();

	CComPtr<ID3D10Buffer> CreateVertexBuffer();

	CComPtr<ID3D10RenderTargetView> CreateRenderTargetView();

	CComPtr<IDXGISwapChain> CreateSwapChain();

	CComPtr<ID3D10Device1>  CreateDevice();

	CComPtr<IDXGIFactory1> m_dxgiFactory;
	CComPtr<ID3D10Device1> m_device;
	CComPtr<IDXGISwapChain> m_swapChain;
	CComPtr<ID3D10RenderTargetView> m_currentRenderTargetView;
};

