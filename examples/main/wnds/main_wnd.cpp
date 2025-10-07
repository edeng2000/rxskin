#include "main_wnd.h"
#include "main.h"
#include "resource.h"
#include "rxutil.h"
#include "common/graphics.h"
#include <olectl.h>
#include <WindowsX.h>
#include "commctrl.h"
#include "common/rxskinresourcemgr.h"
#include "common/rxskinservice.h"

#include "protocol_wnd.h"
#include "wnds/help_wnd.h"

#define DEFALT_LINE_OFFSET_X       10
#define DEFALT_LINE_OFFSET_Y       10
#define ID_BUTTON_INSTALL			23002
#define ID_BROWSER_SETUP_PATH       23004

#define ID_BUTTON_VIEW_PROTOCOL     23006
#define ID_BUTTON_AGREE_PROTOCOL    23005
#define ID_STATIC_INSTALL_DETAIL    23007
#define ID_STATIC_PATH              23003
#define ID_BUTTON_HELP				23010
#define ID_PROGRESS_INSTALL         23009

CMainWnd::CMainWnd()
{
	SetAutoDelete(true);
	m_bFinished = false;
}

CMainWnd ::~CMainWnd()
{
}

BOOL CMainWnd::OnLButtonUp(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinDialog::OnLButtonUp(nFlags, point);
	return bResult;
}

BOOL CMainWnd::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bClicked = CRXSkinDialog::OnLButtonDown(nFlags, point);
	if (FALSE == bClicked)
	{
		CRXSkinRect rcCaption = GetCaptionRect();
		if (rcCaption.PtInRect(point))
		{
			::PostMessage(GetHWnd(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
			return TRUE;
		}
	}
	return bClicked;
}

BOOL CMainWnd::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinDialog::OnCreate(lpCreate);
	if (bResult)
	{
		::SetWindowLong(GetHWnd(), GWL_EXSTYLE, GetWindowLong(GetHWnd(), GWL_EXSTYLE)
			& ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
		tstring strConfig = _T("configs/main.xml");		
		ReadConfig(strConfig.c_str());
		CenterWindow();	
	}
	m_poBtnInstall = (CRXImageButton*)GetControlMgr().FindControl(ID_BUTTON_INSTALL);
	m_poBtnBrowser = (CRXSkinBrowser*)GetControlMgr().FindControl(ID_BROWSER_SETUP_PATH);
	m_poStaticDetail = (CRXSkinStatic*)GetControlMgr().FindControl(ID_STATIC_INSTALL_DETAIL);
	m_poStaticPath = (CRXSkinStatic*)GetControlMgr().FindControl(ID_STATIC_PATH);
	m_poProgress = (CRXSkinProgress*)GetControlMgr().FindControl(ID_PROGRESS_INSTALL);
	m_poBtnViewProtocol = (CRXHyperLink*)GetControlMgr().FindControl(ID_BUTTON_VIEW_PROTOCOL);
	m_poProgress->SetVisible(FALSE);
	m_poStaticDetail->SetVisible(FALSE);
	m_poBtnBrowser->SetBrowserType(EBrowserType::BROWSER_SELECT_DIRECTORY);
	LPITEMIDLIST pidl;
	if (FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAM_FILES, &pidl)))
	{
		int32_t error = GetLastError();
		LOG_INFO("SHGetSpecialFolderLocation:%d", error);
		return false;
	}
	TCHAR* szPath = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
	memset(szPath, 0, sizeof(TCHAR) * MAX_PATH);
	SHGetPathFromIDList(pidl, szPath);
	m_poBtnBrowser->SetText(szPath);
	m_poBtnBrowser->SetIsReadyOnly();
	free(szPath);
	m_poBtnAgreeProtocol = (CRXCheckButton*)GetControlMgr().FindControl(ID_BUTTON_AGREE_PROTOCOL);
	m_poBtnAgreeProtocol->SetChecked(TRUE);
	SetForgeWindow();
	
	return bResult;
}

BOOL CMainWnd::Create(const TCHAR* pszWindowClassName,
	const TCHAR* pszWindowName,
	const UINT32 dwStyle,
	const HWND hParentWnd, HICON hIcon /* = NULL */)
{
	BOOL bResult = CRXSkinDialog::Create(pszWindowClassName, pszWindowName, dwStyle, hParentWnd, hIcon);
	if (bResult)
	{
	}
	return bResult;
}

void CMainWnd::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	CRXSkinDialog::OnPaintBk(hDC);
}


LRESULT CMainWnd::OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = CRXSkinDialog::OnHandleMessage(message, wParam, lParam);
	switch (message)
	{
	default:
		break;
	}
	return nResult;
}

VOID CMainWnd::OnClickedClose()
{
	HWND hWndParent = ::GetWindowOwner(m_hWnd);
	if (hWndParent)
	{
		::EnableWindow(hWndParent, TRUE);
	}
	::ShowWindow(GetHWnd(), SW_HIDE);
	exit(0);
}

BOOL CMainWnd::OnRButtonUp(UINT nFlags, POINT point)
{
	return GetControlMgr().OnRButtonUp(point) == NULL ? FALSE : TRUE;
}

BOOL CMainWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	return CRXSkinDialog::OnSize(wParam, lParam);
}

void CMainWnd::OnFinalMessage(HWND hWnd)
{
	CRXSkinDialog::OnFinalMessage(hWnd);
	delete this;
}

BOOL CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{	
	switch (wParam)
	{
	case ID_BUTTON_VIEW_PROTOCOL:
	{
		CProtocolWnd oProtocol;
		if (!oProtocol.Create(_T("RXInstallProtocolWnd"),
			_T("RXInstallProtocol"),
			WS_POPUP ,
			GetDesktopWindow(),
			LoadIcon(theApp.GetInstance(), MAKEINTRESOURCE(IDI_MAIN))))
		{
			return FALSE;
		}
		oProtocol.ShowModal();
		SetTopmostWindow();
		SetForgeWindow();
	}
	break;
	case ID_BUTTON_AGREE_PROTOCOL:
	{
		bool bAgree = m_poBtnAgreeProtocol->GetChecked();
		m_poBtnBrowser->Enable(bAgree);
		m_poBtnInstall->Enable(bAgree);
	}
	break;
	case ID_BUTTON_HELP:
	{
		CHelpWnd oProtocol;
		if (!oProtocol.Create(_T("RXInstallHelpWnd"),
			_T("RXInstallHelp"),
			WS_POPUP,
			GetDesktopWindow(),
			LoadIcon(theApp.GetInstance(), MAKEINTRESOURCE(IDI_MAIN))))
		{
			return FALSE;
		}
		oProtocol.ShowModal();
		SetTopmostWindow();
		SetForgeWindow();
	}
	break;
	case ID_BUTTON_INSTALL:
	{
		if (m_bFinished)
		{

			OnClickedClose();
			return TRUE;
		}
		m_poStaticDetail->SetVisible(TRUE);
		m_poProgress->SetVisible(TRUE);
		m_poBtnBrowser->SetVisible(FALSE);
		m_poStaticPath->SetVisible(FALSE);
		m_poBtnInstall->Enable(FALSE);
		m_poBtnAgreeProtocol->SetVisible(FALSE);
		m_poBtnViewProtocol->SetVisible(FALSE);
		Invalidate();
	}
	break;
	default:
		break;
	}
	return CRXSkinDialog::OnCommand(wParam, lParam);
}

BOOL CMainWnd::OnTimer(WPARAM wParam, LPARAM lParam)
{
	return CRXSkinDialog::OnTimer(wParam, lParam);
}