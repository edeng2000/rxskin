#include "protocol_wnd.h"
#include "../main.h"
#include "resource.h"
#include "rxutil.h"
#include "common/graphics.h"
#include <olectl.h>
#include <WindowsX.h>
#include "commctrl.h"
#include "common/rxskinresourcemgr.h"
#include "common/rxskinservice.h"

#define DEFALT_LINE_OFFSET_X       10
#define DEFALT_LINE_OFFSET_Y       10

#define ID_BROWSER_PROTOCOL        33001

CProtocolWnd::CProtocolWnd()
{
	SetAutoDelete(true);
}

CProtocolWnd ::~CProtocolWnd()
{
}

BOOL CProtocolWnd::OnLButtonUp(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinDialog::OnLButtonUp(nFlags, point);
	return bResult;
}

BOOL CProtocolWnd::OnLButtonDown(UINT nFlags, POINT point)
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

BOOL CProtocolWnd::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinDialog::OnCreate(lpCreate);
	if (bResult)
	{
		::SetWindowLong(GetHWnd(), GWL_EXSTYLE, GetWindowLong(GetHWnd(), GWL_EXSTYLE)
			& ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
		tstring strConfig = _T("configs/protocol.xml");
		ReadConfig(strConfig.c_str());
		CenterWindow();	
	}
	m_poWebBrowser = (CRXSkinWebBrowser*)GetControlMgr().FindControl(ID_BROWSER_PROTOCOL);
	m_poWebBrowser->SetWebBrowserEventHandler(this);
	SetForgeWindow();
	m_poWebBrowser->Navigate(_T("http://14.103.85.174:8800/rxremote/proto.html"));;
	return bResult;
}

BOOL CProtocolWnd::Create(const TCHAR* pszWindowClassName,
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

void CProtocolWnd::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	CRXSkinDialog::OnPaintBk(hDC);
}


LRESULT CProtocolWnd::OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = CRXSkinDialog::OnHandleMessage(message, wParam, lParam);
	return nResult;
}

VOID CProtocolWnd::OnClickedClose()
{
	HWND hWndParent = ::GetWindowOwner(m_hWnd);
	if (hWndParent)
	{
		::EnableWindow(hWndParent, TRUE);
	}
	::ShowWindow(GetHWnd(), SW_HIDE);
}

BOOL CProtocolWnd::OnRButtonUp(UINT nFlags, POINT point)
{
	return GetControlMgr().OnRButtonUp(point) == NULL ? FALSE : TRUE;
}

BOOL CProtocolWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	return CRXSkinDialog::OnSize(wParam, lParam);
}

void CProtocolWnd::OnFinalMessage(HWND hWnd)
{
	CRXSkinDialog::OnFinalMessage(hWnd);
	delete this;
}

BOOL CProtocolWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{	
	return CRXSkinDialog::OnCommand(wParam, lParam);
}

BOOL CProtocolWnd::OnTimer(WPARAM wParam, LPARAM lParam)
{
	return CRXSkinDialog::OnTimer(wParam, lParam);
}

void CProtocolWnd::DocumentComplete(CRXSkinWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url)
{

}