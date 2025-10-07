#pragma once
#include "rxskinincludes.h"
#include "wnds/main_wnd.h"
class CMainApp :public CRXSkinApp
{
public:
	CMainApp();
	~CMainApp();
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	CMainWnd& GetMain() { return m_wndMain; }
private:
	CMainWnd  m_wndMain;
	ULONG_PTR m_gdiplusToken;
	Gdiplus::GdiplusStartupInput* m_pGdiplusStartupInput;
};

extern CMainApp theApp;

