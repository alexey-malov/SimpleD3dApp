
// SimpleD3dApp.h : main header file for the SimpleD3dApp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSimpleD3dAppApp:
// See SimpleD3dApp.cpp for the implementation of this class
//

class CSimpleD3dAppApp : public CWinApp
{
public:
	CSimpleD3dAppApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSimpleD3dAppApp theApp;
