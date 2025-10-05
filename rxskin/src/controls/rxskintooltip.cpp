#include "pch.h"
#include "common/rxskinutil.h"
#include "common/RXSkinService.h"
#include "common/RXSkinMemDC.h"
#include "common/graphics.h"
#include "controls/RXSkinToolTip.h"
#include "commctrl.h"
#include "tchar.h"



CRXSkinToolTip::CRXSkinToolTip()
:m_hParent(NULL)
{
	memset(m_szToolTip,0,sizeof(TCHAR)*256);
}

CRXSkinToolTip::~CRXSkinToolTip()
{
}

BOOL CRXSkinToolTip::Create(const TCHAR* pszWindowClassName,
							const TCHAR* pszWindowName,
							const UINT32 dwStyle,
							const HWND hParentWnd,
							HICON hIcon)
{
	if (FALSE == CRXSkinWnd::Create(pszWindowClassName,pszWindowName,dwStyle,hParentWnd,hIcon))
	{
		return FALSE;
	}
	m_hParent = hParentWnd;
	return TRUE;
}

BOOL CRXSkinToolTip::OnCreate(LPCREATESTRUCT lpCreate)
{
	if (FALSE == CRXSkinWnd::OnCreate(lpCreate))
	{
		return FALSE;
	}
	::SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE) 
		&~WS_EX_APPWINDOW|WS_EX_TOOLWINDOW ); 
	::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE); 
	return TRUE;
}

void CRXSkinToolTip::ShowToolTip(BOOL bShow)
{
	if (bShow)
	{
		ShowWindow(m_hWnd,SW_SHOW);
		InvalidateRect(m_hWnd,NULL,FALSE);
		::SetTimer(m_hWnd, HIDE_TOOLTIP_TIMER_ID, SHOW_TOOLTIP_INTERVAL_TIME, NULL);
	}
	else
	{
		ShowWindow(m_hWnd,SW_HIDE);
	}
}

void CRXSkinToolTip::SetToolTipPosition(POINT pt)
{
	SIZE szTxt;
	tstring strTxt = m_szToolTip;
	CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, m_szToolTip, szTxt);
	INT32 nWidth = CRXSkinService::Instance()->GetDrawMgr().GetToolTipOffSetX()*2+szTxt.cx;
	INT32 nHeight = CRXSkinService::Instance()->GetDrawMgr().GetToolTipOffSetY()*2+szTxt.cy;
	GetCursorPos(&pt);
	INT32 nCusorY = GetSystemMetrics(SM_CXCURSOR)/2;
	::MoveWindow(m_hWnd,pt.x,pt.y+nCusorY,nWidth,nHeight,TRUE);
	SetWindowPos(m_hWnd,HWND_TOPMOST,pt.x,pt.y,nWidth,nHeight,SWP_NOMOVE);
	::SetTimer(m_hWnd, HIDE_TOOLTIP_TIMER_ID, SHOW_TOOLTIP_INTERVAL_TIME,NULL);
}

BOOL  CRXSkinToolTip::OnTimer(WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinWnd::OnTimer(wParam, lParam);
	if (HIDE_TOOLTIP_TIMER_ID == wParam)
	{
		::KillTimer(m_hParent, wParam);
		ShowToolTip(FALSE);
	}
	return bResult;
}

void CRXSkinToolTip::UpdateToolTipText(LPCTSTR szBuff, HINSTANCE hInstance)
{
	tstring strToolTip = szBuff;
	if (strToolTip.size()<=0)
	{
		ShowToolTip(FALSE);
		return;
	}
	memset(m_szToolTip,0,sizeof(TCHAR)*256);
	_tcsncpy(m_szToolTip,szBuff,256);
}

void CRXSkinToolTip::OnFinalMessage(HWND hWnd)
{
	CRXSkinWnd::OnFinalMessage(hWnd);
}

VOID CRXSkinToolTip::OnPaint()
{
	PAINTSTRUCT ps = { 0 };
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	CRXSkinMemDC memDC(hDC,rcClient);
	DrawRect(memDC.GetHDC(),
		CRXSkinService::Instance()->GetDrawMgr().GetToolTipBorder(),
		CRXSkinService::Instance()->GetDrawMgr().GetToolTipBk(),
		rcClient.left,
		rcClient.top,
		rcClient.right,
		rcClient.bottom);

	tstring strTxt = m_szToolTip;
	SkCanvas* poCanvas = memDC.GetSurface()->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, m_szToolTip, rcClient, CRXSkinService::Instance()->GetDrawMgr().GetButtonListNormalText(), ID_FONT_COMMON,
			DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	}
	memDC.TakeSnapshot();
	::EndPaint(m_hWnd, &ps);
}