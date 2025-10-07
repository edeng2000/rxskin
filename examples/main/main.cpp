#include <windows.h>
#include <tchar.h>
#include <cmath>
#include "resource.h"
#include <io.h>
#include <fcntl.h>
#pragma fenv_access (on)
#include "main.h"
#include <iostream>
#include <cfenv>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <conio.h>
#include "rxfile.h"
#include "rxdir.h"
#include "rxskinres.h"
using namespace GNDP;

CMainApp::CMainApp()
{
	m_pGdiplusStartupInput = new Gdiplus::GdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, m_pGdiplusStartupInput, NULL); // 加载GDI接口
}

CMainApp::~CMainApp()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

BOOL CMainApp::InitInstance()
{
	OleInitialize(NULL);
	CRXLogger::CreateInstance();
	CRXLogger::Instance()->SetName("main");
	CRXLogger::Instance()->StartThread();
	tstring strResource = GetModulePath();
	strResource += _T("uires.zip");
	if (false == CRXSkinResourceMgr::Instance()->OpenZipResource(strResource.c_str(), "QAZwsx!@#456"))
	{
		return FALSE;
	}
	CRXSkinApp::InitInstance();

	CRXSkinService::Instance()->SetResourceModel(EResourceType::RESOURCE_FROM_ZIP);
	tstring strConfig = _T("configs/common.xml");
	CRXSkinService::Instance()->Init(strConfig.c_str());
	CRXSkinService::Instance()->GetImageMgr().Init(strConfig.c_str());
	CRXSkinService::Instance()->GetDrawMgr().LoadSelfBmp();
	if (!m_wndMain.Create(_T("RXInstallWaitingWnd"),
		_T("RXInstall"),
		WS_POPUP | WS_VISIBLE,
		GetDesktopWindow(),
		LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_MAIN))))
	{
		return FALSE;
	}
	CRXSkinApp::Run();

	return TRUE;
}

int CMainApp::ExitInstance()
{
	OleUninitialize();
	/*	CService::Instance()->Uninit();*/
	CRXLogger::Instance()->StopThread();
	CRXLogger::DestroyInstance();
	return 0;
}


CMainApp theApp;
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	theApp.SetInstance(hInstance, lpCmdLine, nCmdShow);

	if (FALSE == theApp.InitInstance())
	{
		return FALSE;
	}
	theApp.ExitInstance();


	return TRUE;
}