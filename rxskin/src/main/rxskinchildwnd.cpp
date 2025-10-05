#include "pch.h"
#include "main\rxskinchildwnd.h"
#include "common\rxskinutil.h"
#include "common\rxskinservice.h"
#include "common\graphics.h"
#include "common\rxskinmemdc.h"
#include "common\rxskindefines.h"
#include "fundation\tinyxml2.h"
#include <olectl.h>
#include <WindowsX.h>
#include "commctrl.h"
#include <tchar.h>
CRXSkinChildWnd::CRXSkinChildWnd(void)
{
	m_hWnd = NULL;
	m_OldWndProc = ::DefWindowProc;
	m_bBordered = FALSE;
	m_borderBottom.SetType(CBorder::BORDER_BOTTOM);
	m_borderLeft.SetType(CBorder::BORDER_LEFT);
	m_borderTop.SetType(CBorder::BORDER_TOP);
	m_borderRight.SetType(CBorder::BORDER_RIGHT);
	m_bTracking = FALSE;
	m_poMouseOn = NULL;
	m_nPosition = POSITION_CLIENT;
	m_bMoving = FALSE;
	m_poPressed = NULL;
	m_bMax = FALSE;
	m_bResized = FALSE;
	m_bDrawControls = TRUE;
}

CRXSkinChildWnd::~CRXSkinChildWnd(void)
{
	Destroy();
}

VOID CRXSkinChildWnd::Destroy()
{
	CRXSkinWnd::Destroy();
}

BOOL CRXSkinChildWnd::Create(const TCHAR* pszWindowClassName,
	const TCHAR* pszWindowName,
	const UINT32 dwStyle,
	const HWND hParentWnd,
	HICON hIcon)
{
	return CRXSkinWnd::Create(pszWindowClassName, pszWindowName, dwStyle, hParentWnd, hIcon);
}

UINT32 CRXSkinChildWnd::_HitTest(POINT point)
{
	EPosition enPosition = POSITION_NONE;
	RECT rc;
	CRXSkinRect rcClient;
	GetClientRect(m_hWnd, &rc);
	rcClient = rc;
	if (rcClient.left <= point.x &&
		rcClient.left + CORNER_WIDTH >= point.x &&
		rcClient.top <= point.y &&
		rcClient.top + CORNER_HEIGHT >= point.y)
	{
		enPosition = POSITION_TOPLEFT;
	}
	else if (rcClient.right >= point.x &&
		rcClient.right - CORNER_WIDTH <= point.x &&
		rcClient.top <= point.y &&
		rcClient.top + CORNER_HEIGHT >= point.y)
	{
		enPosition = POSITION_TOPRIGHT;
	}
	else if (rcClient.right >= point.x &&
		rcClient.right - CORNER_WIDTH <= point.x &&
		rcClient.bottom >= point.y &&
		rcClient.bottom - CORNER_HEIGHT <= point.y)
	{
		enPosition = POSITION_BOTTOMRIGHT;
	}
	else if (rcClient.left <= point.x &&
		rcClient.left + CORNER_WIDTH >= point.x &&
		rcClient.bottom >= point.y &&
		rcClient.bottom - CORNER_HEIGHT <= point.y)
	{
		enPosition = POSITION_BOTTOMLEFT;
	}
	else if (rcClient.left <= point.x &&
		rcClient.left + EDGE_WIDTH >= point.x)
	{
		enPosition = POSITION_LEFT_EDGE;
	}
	else if (rcClient.bottom >= point.y &&
		rcClient.bottom - EDGE_HEIGHT <= point.y)
	{
		enPosition = POSITION_BOTTOM_EDGE;
	}
	else if (rcClient.right >= point.x &&
		rcClient.right - EDGE_WIDTH <= point.x)
	{
		enPosition = POSITION_RIGHT_EDGE;
	}
	else if (rcClient.top <= point.y &&
		rcClient.top + EDGE_HEIGHT >= point.y)
	{
		enPosition = POSITION_TOP_EDGE;
	}
	else
	{
		enPosition = POSITION_CLIENT;
	}
	return enPosition;
}

BOOL CRXSkinChildWnd::OnCreate(LPCREATESTRUCT lpCreate)
{
	if (FALSE == CRXSkinWnd::OnCreate(lpCreate))
	{
		return FALSE;
	}
	m_oToolTip.Create(WND_RXSKIN_TOOLTIP_NAME, _T(""), WS_POPUP, m_hWnd, NULL);
	m_oMenu.Create(WND_RXSKIN_MENU_CLASS, _T(""), WS_POPUP, m_hWnd, NULL);
	return TRUE;
}

VOID CRXSkinChildWnd::OnPaintBk(HDC hDC)
{
	CRXSkinRect rcClient;
	GetClientRectEx(rcClient);
	DrawRoundRect(hDC, CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor(),
		CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(), rcClient,
		CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx,
		CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cy);
	INT32 nTopHeight = m_rcBkTop.Height();
	m_rcBkTop.left = rcClient.left = CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
	m_rcBkTop.top = rcClient.top + CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cy;
	m_rcBkTop.right = rcClient.right - CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
	m_rcBkTop.bottom = m_rcBkTop.top + nTopHeight;
	DrawGradientRect(hDC, TRUE, m_rcBkTop, m_clrBkTopFrom, m_clrBkTopTo);
	m_rcBkMiddle = rcClient;
	m_rcBkMiddle.top = m_rcBkTop.bottom;
	m_rcBkMiddle.left = m_rcBkTop.left;
	m_rcBkMiddle.right = m_rcBkTop.right;
	m_rcBkMiddle.bottom = rcClient.bottom - m_rcBkBottom.Height();
	DrawGradientRect(hDC, TRUE, m_rcBkMiddle, m_clrBkMiddleFrom, m_clrBkMiddleTo);

	INT32 nHeight = m_rcBkBottom.Height();
	CRXSkinRect rcBottom = rcClient;
	rcBottom.top = rcBottom.bottom - nHeight;
	rcBottom.left = m_rcBkTop.left;
	rcBottom.right = m_rcBkTop.right;
	rcBottom.bottom -= CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cy;
	DrawGradientRect(hDC, TRUE, rcBottom, m_clrBkBottomFrom, m_clrBkBottomTo);
}

VOID CRXSkinChildWnd::OnPaint()
{
	CRXSkinWnd::OnPaint();
	PAINTSTRUCT ps = { 0 };
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	CRXSkinMemDC memDC(hDC, rcClient);
	OnDraw(memDC.GetSurface(), memDC.GetHDC(), rcClient);
	if (m_bDrawControls)
		m_oControlMgr.OnDraw(memDC.GetSurface(),memDC.GetHDC());
	memDC.TakeSnapshot();
	::EndPaint(m_hWnd, &ps);
}

BOOL CRXSkinChildWnd::OnTimer(WPARAM wParam, LPARAM lParam)
{
	return GetControlMgr().OnTimer(wParam, lParam);
}

void CRXSkinChildWnd::SetTimer(const DWORD& dwEventID, const UINT32 dwInterval, LPARAM lpParam)
{
	::SetTimer(m_hWnd, dwEventID, dwInterval, (TIMERPROC)lpParam);
}

void CRXSkinChildWnd::KillTimer(const DWORD& dwEventID)
{
	::KillTimer(m_hWnd, dwEventID);
}

BOOL CRXSkinChildWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	CRXSkinRect rcClient = rc;
	INT32 cx = rcClient.Width();
	INT32 cy = rcClient.Height();
	m_oControlMgr.OnSize(cx, cy);

	//	if (m_bBordered == FALSE)
	{
		if (m_nRoundHeight > 0 && m_nRoundWidth > 0)
		{
			HRGN hRgn = ::CreateRoundRectRgn(0, 0, cx + 1, cy + 1, m_nRoundWidth, m_nRoundHeight);
			if (hRgn)
			{
				::SetWindowRgn(m_hWnd, hRgn, FALSE);
				::DeleteObject(hRgn);
				InvalidateRect(rcClient, FALSE);
			}
		}
	}
	HDWP hdwp = ::BeginDeferWindowPos((INT32)m_mapLayout.size());
	CLayoutMapItr itr = m_mapLayout.begin();
	for (; itr != m_mapLayout.end(); itr++)
	{
		itr->second->OnSize(cx, cy, hdwp);
	}
	::EndDeferWindowPos(hdwp);
	return TRUE;
}

void CRXSkinChildWnd::OnFinalMessage(HWND hWnd)
{
	m_oToolTip.Destroy();
	m_oMenu.Destroy();
	CRXSkinWnd::OnFinalMessage(hWnd);
}

BOOL CRXSkinChildWnd::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bClicked = FALSE;
	m_nPosition = _HitTest(point);
	if (m_bResized && POSITION_CLIENT != m_nPosition &&
		POSITION_NONE != m_nPosition)
	{
		if (m_bBordered)
		{
			::SetFocus(m_hWnd);
			m_bMoving = TRUE;
			::SetCapture(m_hWnd);
			bClicked = TRUE;
		}
	}
	else
	{
		CRXControl* poControl = m_oControlMgr.OnLButtonDown(point);
		if (poControl == NULL)
		{
			if (point.y <= m_borderTop.GetHeight())
			{
				::PostMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
			}
			if (m_poPressed)
			{
				m_poPressed->SetState(CONTROL_STATE_NORMAL);
				CRXSkinRect rc = m_poPressed->GetInvalidateRect();
				InvalidateRect(rc, FALSE);
				m_poPressed = poControl;
			}
		}
		else if ((poControl = poControl->GetInvalidateControl()))
		{
			bClicked = TRUE;
			if (NULL == m_poPressed)
			{
				m_poPressed = poControl;
				m_poPressed->SetState(CONTROL_STATE_PRESSED);
				CRXSkinRect rc = m_poPressed->GetInvalidateRect();
				InvalidateRect(rc, FALSE);
			}
			else if (m_poPressed != poControl)
			{
				m_poPressed->SetState(CONTROL_STATE_NORMAL);
				CRXSkinRect rc = m_poPressed->GetInvalidateRect();
				InvalidateRect(rc, FALSE);
				m_poPressed = poControl;
				m_poPressed->SetState(CONTROL_STATE_PRESSED);
				rc = m_poPressed->GetInvalidateRect();
				InvalidateRect(rc, FALSE);
			}
		}
	}
	return bClicked;
}

BOOL CRXSkinChildWnd::OnLButtonUp(UINT nFlags, POINT point)
{
	BOOL bResult = FALSE;
	if (m_bMoving)
	{
		m_bMoving = FALSE;
		ReleaseCapture();
	}
	else
	{
		bResult = m_oControlMgr.OnClick(point);
	}
	return bResult;
}

BOOL CRXSkinChildWnd::OnRButtonUp(UINT nFlags, POINT point)
{
	CRXControl* poControl = m_oControlMgr.HitTest(point);
	if (poControl && poControl->OnRButtonUp(point))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CRXSkinChildWnd::OnRButtonDown(UINT nFlags, POINT point)
{
	CRXControl* poControl = m_oControlMgr.HitTest(point);
	if (poControl && poControl->OnRButtonDown(point))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CRXSkinChildWnd::OnRButtonDblClk(UINT nFlags, POINT point)
{
	CRXControl* poControl = m_oControlMgr.HitTest(point);
	if (poControl && poControl->OnRButtonDblClk(point))
	{
		return TRUE;
	}
	return FALSE;
}

VOID CRXSkinChildWnd::InvalidateRect(CRXSkinRect& rcClient, BOOL bRePaint /* = FALSE */)
{
	::InvalidateRect(m_hWnd, &rcClient, FALSE);
}

BOOL CRXSkinChildWnd::OnMove(int x, int y)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	CRXSkinRect rcClient = rc;
	InvalidateRect(rcClient, TRUE);
	return TRUE;
}

LRESULT CRXSkinChildWnd::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	if (m_poMouseOn)
	{
		m_poMouseOn->SetState(CONTROL_STATE_NORMAL);
		m_poMouseOn->OnMouseLeave();
		CRXSkinRect rc = m_poMouseOn->GetInvalidateRect();
		InvalidateRect(rc, FALSE);
		m_poMouseOn = NULL;
	}
	CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);

	m_oToolTip.ShowToolTip(FALSE);
	return 0;
}

VOID CRXSkinChildWnd::OnRButtonSystemToolbar()
{
	POINT pt;
	GetCursorPos(&pt);
	if (m_bResized)
	{
		m_oMenu.AppendMenu(MENU_STRING, ID_CLOSE, _T("关闭"));
		m_oMenu.AppendMenu(MENU_STRING, ID_MIN, _T("最小化"));
		m_oMenu.AppendMenu(MENU_STRING, ID_MAX, _T("最大化"));
	}
	else
	{
		m_oMenu.AppendMenu(MENU_STRING, ID_CLOSE, _T("关闭"));
		m_oMenu.AppendMenu(MENU_STRING, ID_MIN, _T("最小化"));
	}
	m_oMenu.ShowMenu(pt);
}

BOOL CRXSkinChildWnd::OnMouseMove(UINT nFlags, POINT point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	INT32 nPosition = _HitTest(point);
	if (m_bResized && POSITION_CLIENT != nPosition &&
		POSITION_NONE != nPosition)
	{
		if (m_bBordered)
		{
			switch (nPosition)
			{
			case POSITION_BOTTOMRIGHT:
			case POSITION_TOPLEFT:
			{
				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_NWSE);
			}
			break;
			case POSITION_TOPRIGHT:
			case POSITION_BOTTOMLEFT:
			{
				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_NESW);
			}
			break;
			case POSITION_TOP_EDGE:
			case POSITION_BOTTOM_EDGE:
			{
				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_NS);
			}
			break;
			case POSITION_LEFT_EDGE:
			case POSITION_RIGHT_EDGE:
			{
				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_WE);
			}
			break;
			default:
				break;
			}
		}

	}
	else
	{
		CRXControl* poControl = m_oControlMgr.OnMouseMove(point);
		if (poControl && (poControl = poControl->GetInvalidateControl()))
		{
			if (NULL == m_poMouseOn)
			{
				m_poMouseOn = poControl;
				m_poMouseOn->SetState(CONTROL_STATE_MOUSEON);
				CRXSkinRect rc = m_poMouseOn->GetInvalidateRect();
				InvalidateRect(rc, FALSE);
			}
			else if (poControl != m_poMouseOn)
			{
				m_poMouseOn->SetState(CONTROL_STATE_NORMAL);
				CRXSkinRect rc = m_poMouseOn->GetInvalidateRect();
				InvalidateRect(rc, FALSE);
				m_poMouseOn = poControl;
				m_poMouseOn->SetState(CONTROL_STATE_MOUSEON);
				rc = m_poMouseOn->GetInvalidateRect();
				InvalidateRect(rc, FALSE);
			}
			if (poControl->OnSetCursor())
			{
				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_HAND);
			}

			UpdateToolTip(poControl);

		}
		else
		{
			UpdateToolTip(NULL);
			//	m_oToolTip.UpdateTipText(_T(""),this);
			if (m_poMouseOn)
			{
				m_poMouseOn->SetState(CONTROL_STATE_NORMAL);
				CRXSkinRect rc = m_poMouseOn->GetInvalidateRect();
				InvalidateRect(rc, FALSE);
				m_poMouseOn = poControl;
			}
			if (poControl)
			{
				if (poControl->OnSetCursor())
				{
					CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);
				}
			}
			else
				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);

		}
	}
	if (m_bMoving)
	{
		CRXSkinRect rcWin;
		RECT rc;
		GetWindowRect(m_hWnd, &rc);
		rcWin = rc;
		POINT pt;
		GetCursorPos(&pt);

		switch (m_nPosition)
		{
		case POSITION_TOP_EDGE:
		{
			rcWin.top = pt.y;
			if (rcWin.Width() > m_szMin.cx &&
				rcWin.Height() > m_szMin.cy)
			{
				MoveWindow(rcWin);
			}
		}
		break;
		case POSITION_BOTTOM_EDGE:
		{
			rcWin.bottom = pt.y;
			if (rcWin.Width() > m_szMin.cx &&
				rcWin.Height() > m_szMin.cy)
			{
				MoveWindow(rcWin);
			}
		}
		break;
		case POSITION_LEFT_EDGE:
		{
			rcWin.left = pt.x;
			if (rcWin.Width() > m_szMin.cx &&
				rcWin.Height() > m_szMin.cy)
			{
				MoveWindow(rcWin);
			}
		}
		break;
		case POSITION_RIGHT_EDGE:
		{
			rcWin.right = pt.x;
			if (rcWin.Width() > m_szMin.cx &&
				rcWin.Height() > m_szMin.cy)
			{
				MoveWindow(rcWin);
			}
		}
		break;
		case POSITION_TOPLEFT:
		{
			rcWin.left = pt.x;
			rcWin.top = pt.y;
			if (rcWin.Width() > m_szMin.cx &&
				rcWin.Height() > m_szMin.cy)
			{
				MoveWindow(rcWin);
			}
		}
		break;
		case POSITION_BOTTOMRIGHT:
		{
			rcWin.right = pt.x;
			rcWin.bottom = pt.y;
			if (rcWin.Width() > m_szMin.cx &&
				rcWin.Height() > m_szMin.cy)
			{
				MoveWindow(rcWin);
			}
		}
		break;
		case POSITION_BOTTOMLEFT:
		{
			rcWin.left = pt.x;
			rcWin.bottom = pt.y;
			if (rcWin.Width() > m_szMin.cx &&
				rcWin.Height() > m_szMin.cy)
			{
				MoveWindow(rcWin, FALSE);
			}
		}
		break;
		case POSITION_TOPRIGHT:
		{
			rcWin.right = pt.x;
			rcWin.top = pt.y;
			if (rcWin.Width() > m_szMin.cx &&
				rcWin.Height() > m_szMin.cy)
			{
				MoveWindow(rcWin);
			}
		}
		break;
		default:
			break;
		}
	}
	return TRUE;
}


BOOL CRXSkinChildWnd::OnLButtonDblClk(UINT nFlags, POINT point)
{
	if (m_bResized)
	{
		if (m_oControlMgr.HitTest(point) == NULL)
		{
			
		}
		else
		{
			m_oControlMgr.OnLButtonDblClk(point);
		}
	}

	return TRUE;
}

BOOL CRXSkinChildWnd::OnMouseHover(UINT nFlags, POINT point)
{
	CRXControl* poControl = m_oControlMgr.HitTest(point);
	if (poControl)
	{

	}
	return TRUE;
}

VOID CRXSkinChildWnd::MoveWindow(CRXSkinRect rcWin, BOOL bRepaint)
{
	::MoveWindow(m_hWnd, rcWin.left, rcWin.top, rcWin.Width(), rcWin.Height(), FALSE);
}

VOID CRXSkinChildWnd::UpdateToolTip(CRXControl* poControl)
{
	if (poControl && poControl->IsEnabled() && poControl->IsVisible())
	{
		tstring strToolTip = poControl->GetTooltip();
		if (strToolTip.size() > 0)
		{
			POINT pt;
			pt.x = poControl->GetInvalidateRect().left;
			pt.y = poControl->GetInvalidateRect().bottom;
			/*ClientToScreen(m_hWnd,&pt);*/
			m_oToolTip.SetToolTipPosition(pt);
			m_oToolTip.UpdateToolTipText(strToolTip.c_str(), CRXSkinService::Instance()->GetInstance());
			m_oToolTip.ShowToolTip(TRUE);
			//	::SetFocus(m_hWnd);
		}
	}
	else
	{
		m_oToolTip.ShowToolTip(FALSE);
	}
}

BOOL CRXSkinChildWnd::OnMessage(UINT32 dwMessageID, WPARAM wParam, LPARAM lParam)
{
	return m_oControlMgr.OnMessage(dwMessageID, wParam, lParam);
}

BOOL CRXSkinChildWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CRXSkinWnd::OnCommand(wParam, lParam);
	return m_oControlMgr.OnCommand(wParam, lParam);;
}

LRESULT CRXSkinChildWnd::OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = CRXSkinWnd::OnHandleMessage(message, wParam, lParam);
	if (nResult == 0)
	{
		switch (message)
		{
		case 0x0313: //任务栏应用程序图标上点击右键
		{
			OnRButtonSystemToolbar();
			return TRUE;
		}
		break;
		case WM_CTLCOLOREDIT:
		{
			if (lParam == 0) break;
			/*			HWND hWndChild = (HWND)lParam;
						int lRes = ::SendMessage(hWndChild, OCM__BASE + message, wParam, lParam);
						if (lRes != 0) return true;*/
		}
		break;
		default:
			break;
		}
		if (message > WM_USER)
		{
			return OnMessage(message, wParam, lParam);
		}
		return ::DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return nResult;
}

BOOL CRXSkinChildWnd::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	return m_oControlMgr.OnMouseWheel(nFlags, zDelta, pt);
}

VOID CRXSkinChildWnd::OnScrollBarChanged(EScrollType enType, INT32 nPercent)
{
	m_oControlMgr.OnScrollBarChanged(enType, nPercent);
}

CRXSkinChildWnd::CLayout::CLayout()
{
	m_ptTopLeft = RXSKIN_ANCHOR_TOPLEFT;
	m_ptBottomRight = RXSKIN_ANCHOR_TOPLEFT;
	m_hWnd = NULL;
}

CRXSkinChildWnd::CLayout::~CLayout()
{

}

CRXSkinResizePoint CRXSkinChildWnd::CLayout::GetBottomRight()
{
	return m_ptBottomRight;
}

VOID CRXSkinChildWnd::CLayout::SetBottomRight(CRXSkinResizePoint pt)
{
	m_ptBottomRight = pt;
}

CRXSkinResizePoint CRXSkinChildWnd::CLayout::GetTopLeft()
{
	return m_ptTopLeft;
}

VOID CRXSkinChildWnd::CLayout::SetTopLeft(CRXSkinResizePoint pt)
{
	m_ptTopLeft = pt;
}

VOID CRXSkinChildWnd::CLayout::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

HWND CRXSkinChildWnd::CLayout::GetWnd()
{
	return m_hWnd;
}

VOID CRXSkinChildWnd::CLayout::SetParentWnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

VOID CRXSkinChildWnd::CLayout::SetResize(CRXSkinResizePoint ptTopLeft, CRXSkinResizePoint ptBottomRight)
{
	m_rrcSizing = CRXSkinResizeRect(ptTopLeft.x, ptTopLeft.y, ptBottomRight.x, ptBottomRight.y);
	RECT rc;
	::GetClientRect(m_hParentWnd, &rc);
	RECT rcControl;
	::GetWindowRect(m_hWnd, &rcControl);
	POINT ptControlTopLeft;
	ptControlTopLeft.x = rcControl.left;
	ptControlTopLeft.y = rcControl.top;
	::ScreenToClient(m_hParentWnd, &ptControlTopLeft);
	POINT ptControlBottomRight;
	ptControlBottomRight.x = rcControl.right;
	ptControlBottomRight.y = rcControl.bottom;
	::ScreenToClient(m_hParentWnd, &ptControlBottomRight);
	CRXSkinRect rcClient = rc;
	m_oMarginTopLeft.cx = ptControlTopLeft.x - (INT32)(rcClient.Width() * m_rrcSizing.left);
	m_oMarginTopLeft.cy = ptControlTopLeft.y - (INT32)(rcClient.Height() * m_rrcSizing.top);

	// calculate margin for the bottom-right corner

	m_oMarginBottomRight.cx = ptControlBottomRight.x - (INT32)(rcClient.Width() * m_rrcSizing.right);
	m_oMarginBottomRight.cy = ptControlBottomRight.y - (INT32)(rcClient.Height() * m_rrcSizing.bottom);
}

CRXSkinResizeRect CRXSkinChildWnd::CLayout::GetResize()
{
	return m_rrcSizing;
}

HDWP CRXSkinChildWnd::CLayout::OnSize(const UINT32 cx, const UINT32 cy, HDWP hdwp)
{
	CRXSkinResizeRect rcSizing = GetResize();
	if (rcSizing.left == 0 &&
		rcSizing.right == 0 &&
		rcSizing.top == 0 &&
		rcSizing.bottom == 0)
	{
		return hdwp;
	}
	if (!rcSizing.IsNormalized())
	{
		return hdwp;
	}
	RECT rectChild;
	::GetWindowRect(m_hWnd, &rectChild);
	::MapWindowPoints(NULL, m_hParentWnd, (LPPOINT)&rectChild, 2);
	CRXSkinRect rChild = rectChild;
	RECT rc;
	::GetClientRect(m_hParentWnd, &rc);
	CRXSkinRect rcClient = rc;
	CRXSkinRect rectNew;
	rectNew.left = m_oMarginTopLeft.cx + (INT32)(rcClient.Width() * rcSizing.left);
	rectNew.top = m_oMarginTopLeft.cy + (INT32)(rcClient.Height() * rcSizing.top);
	rectNew.right = m_oMarginBottomRight.cx + (INT32)(rcClient.Width() * rcSizing.right);
	rectNew.bottom = m_oMarginBottomRight.cy + (INT32)(rcClient.Height() * rcSizing.bottom);
	rectNew.OffsetRect(rcClient.TopLeft());

	UINT uFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION;
	uFlags |= SWP_NOCOPYBITS;
	if (rectNew.TopLeft().x == rChild.TopLeft().x && rectNew.TopLeft().y == rChild.TopLeft().y)
		uFlags |= SWP_NOMOVE;
	if (rectNew.Width() == rChild.Width() && rectNew.Height() == rChild.Height())
		uFlags |= SWP_NOSIZE;
	// only if size or position changed
	if ((uFlags & (SWP_NOMOVE | SWP_NOSIZE)) != (SWP_NOMOVE | SWP_NOSIZE))
	{
		hdwp = ::DeferWindowPos(hdwp, m_hWnd, NULL, rectNew.left,
			rectNew.top, rectNew.Width(), rectNew.Height(), uFlags);
	}
	return hdwp;
}


VOID CRXSkinChildWnd::GetClientRectEx(CRXSkinRect& oRect)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	oRect = rc;
	oRect.left = oRect.left + m_borderLeft.GetWidth();
	oRect.right = oRect.right - m_borderRight.GetWidth();
	oRect.top = oRect.top + m_borderTop.GetHeight();
	oRect.bottom = oRect.bottom - m_borderBottom.GetHeight();
}

BOOL CRXSkinChildWnd::TranslateMessage(const LPMSG pMsg)
{
	HWND hwndParent = ::GetParent(pMsg->hwnd);
	UINT uStyle = GetWindowStyle(pMsg->hwnd);
	LRESULT lRes = 0;
	if (CRXSkinService::Instance()->GetPreMessages().IsEmpty())
	{
		return FALSE;
	}
	for (int i = 0; i < CRXSkinService::Instance()->GetPreMessages().GetSize(); i++) {
		CRXSkinWnd* pT = static_cast<CRXSkinWnd*>(CRXSkinService::Instance()->GetPreMessages()[i]);
		if (pT)
		{
			if (pMsg->hwnd == pT->GetHWnd()
				|| (hwndParent == pT->GetHWnd() && ((uStyle & WS_CHILD) != 0)))
			{
				if (pT->OnPreMessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, lRes)) return TRUE;
			}
		}

	}
	return TRUE;
}

LRESULT  CRXSkinChildWnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT  CRXSkinChildWnd::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT  CRXSkinChildWnd::OnSysChar(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT  CRXSkinChildWnd::OnSysKeydown(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
BOOL CRXSkinChildWnd::OnPreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& /*lRes*/)
{
	switch (uMsg) {
	case WM_KEYDOWN:
	{
		OnKeyDown(wParam, lParam);
		if (wParam == VK_TAB)
		{
			// 				if( m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && _tcsstr(m_pFocus->GetClass(), _T("RichEditUI")) != NULL ) {
			// 					if( static_cast<CRichEditUI*>(m_pFocus)->IsWantTab() ) return false;
			// 				}
			// 				SetNextTabControl(::GetKeyState(VK_SHIFT) >= 0);
			return true;
		}
	}
	break;
	case WM_SYSCHAR:
	{
		OnSysChar(wParam, lParam);
		// Handle ALT-shortcut key-combinations
// 			FINDSHORTCUT fs = { 0 };
// 			fs.ch = toupper((int)wParam);
// 			CControlUI* pControl = m_pRoot->FindControl(__FindControlFromShortcut, &fs, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
// 			if( pControl != NULL ) {
// 				pControl->SetFocus();
// 				pControl->Activate();
// 				return true;
// 			}
	}
	break;
	case WM_SYSKEYDOWN:
	{
		OnSysKeydown(wParam, lParam);
	}
	break;
	}
	return FALSE;
}



BOOL CRXSkinChildWnd::ReadConfig(const TCHAR* pszConfig)
{
	tinyxml2::XMLDocument*poDoc = CRXSkinResourceMgr::Instance()->LoadXmlFromResource(pszConfig);

	if (poDoc)
	{
		tinyxml2::XMLElement* poRoot = poDoc->RootElement();
		if (poRoot)
		{
			CHAR* pszMiniInfo = (CHAR*)poRoot->Attribute("mininfo");
			if (pszMiniInfo)
			{
				string strValue = pszMiniInfo;
				INT32 nX = -1;
				INT32 nY = -1;
				INT32 nPos = (INT32)strValue.find(",");
				if (nPos >= 0)
				{
					string strX = strValue.substr(0, nPos);
					nX = atoi(strX.c_str());
					string strY = strValue.substr((size_t)nPos + 1, strValue.size() - nPos - 1);
					nY = atoi(strY.c_str());
				}
				if (nX > 0 && nY > 0)
				{
					m_szMin.cx = nX;
					m_szMin.cy = nY;

				}
			}
			CHAR* pszResized = (CHAR*)poRoot->Attribute("resize");
			if (pszResized && strcmp(pszResized, "true") == 0)
			{
				m_bResized = TRUE;
			}
			CHAR* pszArea = (CHAR*)poRoot->Attribute("area");
			if (pszArea)
			{
				m_rcInit = RXSkinGetRect(pszArea);
				::MoveWindow(m_hWnd, m_rcInit.left, m_rcInit.top, m_rcInit.Width(), m_rcInit.Height(), TRUE);
			}
			tinyxml2::XMLElement* poBackground = poRoot->FirstChildElement("Background");
			if (poBackground)
			{
				tinyxml2::XMLElement* poTop = poBackground->FirstChildElement("Top");
				if (poTop)
				{
					CHAR* pszValue = (CHAR*)poTop->Attribute("area");
					if (pszValue)
					{
						m_rcBkTop = RXSkinGetRect(pszValue);
					}
					pszValue = (CHAR*)poTop->Attribute("from");
					if (pszValue)
					{
						m_clrBkTopFrom = GetColor(pszValue);
					}
					pszValue = (CHAR*)poTop->Attribute("to");
					if (pszValue)
					{
						m_clrBkTopTo = GetColor(pszValue);
					}
				}

				tinyxml2::XMLElement* poBottom = poBackground->FirstChildElement("Bottom");
				if (poBottom)
				{
					CHAR* pszValue = (CHAR*)poBottom->Attribute("area");
					if (pszValue)
					{
						m_rcBkBottom = RXSkinGetRect(pszValue);
					}
					pszValue = (CHAR*)poBottom->Attribute("from");
					if (pszValue)
					{
						m_clrBkBottomFrom = GetColor(pszValue);
					}
					pszValue = (CHAR*)poBottom->Attribute("to");
					if (pszValue)
					{
						m_clrBkBottomTo = GetColor(pszValue);
					}
				}

				tinyxml2::XMLElement* poMiddle = poBackground->FirstChildElement("Middle");
				if (poMiddle)
				{
					CHAR* pszValue = (CHAR*)poMiddle->Attribute("area");
					if (pszValue)
					{
						m_rcBkMiddle = RXSkinGetRect(pszValue);
					}
					pszValue = (CHAR*)poMiddle->Attribute("from");
					if (pszValue)
					{
						m_clrBkMiddleFrom = GetColor(pszValue);
					}
					pszValue = (CHAR*)poMiddle->Attribute("to");
					if (pszValue)
					{
						m_clrBkMiddleTo = GetColor(pszValue);
					}
				}
			}

			tinyxml2::XMLElement* poElementBorders = poRoot->FirstChildElement("Borders");
			if (poElementBorders)
			{
				CHAR* pszValue = (CHAR*)poElementBorders->Attribute("need");
				if (pszValue)
				{
					if (strcmp(pszValue, "true") == 0)
						m_bBordered = TRUE;
					CHAR* cpRound = (CHAR*)poElementBorders->Attribute("round");
					if (cpRound)
					{
						string strValue = cpRound;
						INT32 nX = -1;
						INT32 nY = -1;
						INT32 nPos = (INT32)strValue.find(",");
						if (nPos >= 0)
						{
							string strX = strValue.substr(0, nPos);
							nX = atoi(strX.c_str());
							string strY = strValue.substr((size_t)nPos + 1, strValue.size() - nPos - 1);
							nY = atoi(strY.c_str());
						}
						if (nX > 0 && nY > 0)
						{
							m_nRoundWidth = nX;
							m_nRoundHeight = nY;
						}
					}
					tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
					tinyxml2::XMLElement* poElementTop = poElementBorders->FirstChildElement("Top");
					if (poElementTop)
					{
						CHAR* pszSrc = (CHAR*)poElementTop->Attribute("src");
						if (pszSrc)
						{
							strPath += _RXTUTF82T(pszSrc);
							m_borderTop.SetImageFile(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementTop->Attribute("OffSetLeft");
						if (pszSrc)
						{
							m_borderTop.SetFirstOffSet(atoi(pszSrc));
						}
						pszSrc = (CHAR*)poElementTop->Attribute("OffSetRight");
						if (pszSrc)
						{
							m_borderTop.SetSecondOffSet(atoi(pszSrc));
						}
						pszSrc = (CHAR*)poElementTop->Attribute("Trans");
						if (pszSrc)
						{
							INT32 nR = -1, nG = -1, nB = -1;
							string strValue = pszSrc;
							INT32 nPos = (INT32)strValue.find("(");
							if (nPos >= 0)
							{
								strValue = strValue.substr((size_t)nPos + 1, strValue.size() - 1 - nPos);
								nPos = (INT32)strValue.find(",");
								string strR = strValue.substr(0, nPos);
								nR = atoi(strR.c_str());
								strValue = strValue.substr((size_t)nPos + 1, strValue.size() - 1 - nPos);
								string strG = strValue.substr(0, nPos);
								nG = atoi(strG.c_str());
								strValue = strValue.substr((size_t)nPos + 1, strValue.size() - 2 - nPos);
								nB = atoi(strValue.c_str());
							}
							if (nG >= 0 && nR >= 0 && nB >= 0)
							{
								m_borderTop.SetTrans(RGB(nR, nG, nB));
							}
						}
					}

					tinyxml2::XMLElement* poElementBottom = poElementBorders->FirstChildElement("Bottom");
					if (poElementBottom)
					{
						strPath = CRXSkinService::Instance()->GetResourceRootPath();
						CHAR* pszSrc = (CHAR*)poElementBottom->Attribute("src");
						if (pszSrc)
						{
							strPath += _RXTUTF82T(pszSrc);
							m_borderBottom.SetImageFile(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementBottom->Attribute("OffSetLeft");
						if (pszSrc)
						{
							m_borderBottom.SetFirstOffSet(atoi(pszSrc));
						}
						pszSrc = (CHAR*)poElementBottom->Attribute("OffSetRight");
						if (pszSrc)
						{
							m_borderBottom.SetSecondOffSet(atoi(pszSrc));
						}
						pszSrc = (CHAR*)poElementBottom->Attribute("Trans");
						if (pszSrc)
						{
							INT32 nR = -1, nG = -1, nB = -1;
							string strValue = pszSrc;
							INT32 nPos = (INT32)strValue.find("(");
							if (nPos >= 0)
							{
								strValue = strValue.substr((size_t)nPos + 1, strValue.size() - 1 - nPos);
								nPos = (INT32)strValue.find(",");
								string strR = strValue.substr(0, nPos);
								nR = atoi(strR.c_str());
								strValue = strValue.substr((size_t)nPos + 1, strValue.size() - 1 - nPos);
								string strG = strValue.substr(0, nPos);
								nG = atoi(strG.c_str());
								strValue = strValue.substr((size_t)nPos + 1, strValue.size() - 2 - nPos);
								nB = atoi(strValue.c_str());
							}
							if (nG >= 0 && nR >= 0 && nB >= 0)
							{
								m_borderBottom.SetTrans(RGB(nR, nG, nB));
							}
						}
					}

					tinyxml2::XMLElement* poElementLeft = poElementBorders->FirstChildElement("Left");
					if (poElementLeft)
					{
						strPath = CRXSkinService::Instance()->GetResourceRootPath();
						CHAR* pszSrc = (CHAR*)poElementLeft->Attribute("src");
						if (pszSrc)
						{
							strPath += _RXTUTF82T(pszSrc);
							m_borderLeft.SetImageFile(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementLeft->Attribute("OffSetTop");
						if (pszSrc)
						{
							m_borderLeft.SetFirstOffSet(atoi(pszSrc));
						}
						pszSrc = (CHAR*)poElementLeft->Attribute("OffSetBottom");
						if (pszSrc)
						{
							m_borderLeft.SetSecondOffSet(atoi(pszSrc));
						}
						pszSrc = (CHAR*)poElementLeft->Attribute("Trans");
						if (pszSrc)
						{
							INT32 nR = -1, nG = -1, nB = -1;
							string strValue = pszSrc;
							INT32 nPos = (INT32)strValue.find("(");
							if (nPos >= 0)
							{
								strValue = strValue.substr((size_t)nPos + 1, strValue.size() - 1 - nPos);
								nPos = (INT32)strValue.find(",");
								string strR = strValue.substr(0, nPos);
								nR = atoi(strR.c_str());
								strValue = strValue.substr((size_t)nPos + 1, strValue.size() - 1 - nPos);
								string strG = strValue.substr(0, nPos);
								nG = atoi(strG.c_str());
								strValue = strValue.substr((size_t)nPos + 1, strValue.size() - 2 - nPos);
								nB = atoi(strValue.c_str());
							}
							if (nG >= 0 && nR >= 0 && nB >= 0)
							{
								m_borderLeft.SetTrans(RGB(nR, nG, nB));
							}
						}
					}

					tinyxml2::XMLElement* poElementRight = poElementBorders->FirstChildElement("Right");
					if (poElementRight)
					{
						strPath = CRXSkinService::Instance()->GetResourceRootPath();
						CHAR* pszSrc = (CHAR*)poElementRight->Attribute("src");
						if (pszSrc)
						{
							strPath += _RXTUTF82T(pszSrc);
							m_borderRight.SetImageFile(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementRight->Attribute("OffSetTop");
						if (pszSrc)
						{
							m_borderRight.SetFirstOffSet(atoi(pszSrc));
						}
						pszSrc = (CHAR*)poElementRight->Attribute("OffSetBottom");
						if (pszSrc)
						{
							m_borderRight.SetSecondOffSet(atoi(pszSrc));
						}
						pszSrc = (CHAR*)poElementRight->Attribute("Trans");
						if (pszSrc)
						{
							m_borderRight.SetTrans(GetColor(pszSrc));
						}
					}
					if (m_nRoundHeight > 0 && m_nRoundWidth > 0)
					{
						CRXSkinRect rcClient;
						RECT rc;
						GetClientRect(m_hWnd, &rc);
						rcClient = rc;
						HRGN hRgn = ::CreateRoundRectRgn(0, 0, rcClient.right + 1, rcClient.bottom + 1, m_nRoundWidth, m_nRoundHeight);
						if (hRgn)
						{
							::SetWindowRgn(m_hWnd, hRgn, TRUE);
							::DeleteObject(hRgn);
						}
					}
				}
			}
			tinyxml2::XMLElement* poElementCaptions = poRoot->FirstChildElement("Captions");
			if (poElementCaptions)
			{
				CHAR* cpNeed = (CHAR*)poElementCaptions->Attribute("need");
				if (cpNeed && strcmp(cpNeed, "true") == 0)
				{
					tinyxml2::XMLElement* poElementClose = poElementCaptions->FirstChildElement("Close");
					if (poElementClose)
					{
						CRXImageButton* poBtn = new CRXImageButton;
						poBtn->SetID(ID_CLOSE);
						CRXSkinRect rcBtn;
						CRXSkinRect rcClient;
						RECT rc;
						GetClientRect(m_hWnd, &rc);
						rcClient = rc;
						rcBtn = rcClient;
						CHAR* pszSrc = (CHAR*)poElementClose->Attribute("normalsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetNormalImage(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementClose->Attribute("hotsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetHotImage(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementClose->Attribute("downsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetDownImage(strPath.c_str());
						}

						CHAR* cpValue = (CHAR*)poElementClose->Attribute("OffSetX");
						if (cpValue)
						{
							rcBtn.right = rcClient.right - atoi(cpValue);
						}
						cpValue = (CHAR*)poElementClose->Attribute("OffSetY");
						if (cpValue)
						{
							rcBtn.top = rcClient.top + atoi(cpValue);
						}
						cpValue = (CHAR*)poElementClose->Attribute("width");
						if (cpValue)
						{
							rcBtn.left = rcBtn.right - atoi(cpValue);
						}
						cpValue = (CHAR*)poElementClose->Attribute("height");
						if (cpValue)
						{
							rcBtn.bottom = rcBtn.top + atoi(cpValue);
						}
						poBtn->SetRect(rcBtn);
						poBtn->SetParentWnd(this->m_hWnd);
						cpValue = (CHAR*)poElementClose->Attribute("tooltip");
						if (cpValue)
						{
							poBtn->SetTooltip(_RXTUTF82T(cpValue).c_str());
						}
						cpValue = (CHAR*)poElementClose->Attribute("text");
						if (cpValue)
						{
							poBtn->SetText(_RXTUTF82T(cpValue).c_str());
						}
						poBtn->SetResize(RXSKIN_ANCHOR_TOPRIGHT, RXSKIN_ANCHOR_TOPRIGHT);
						m_oControlMgr.AddControl(poBtn);
					}
					tinyxml2::XMLElement* poElementMax = poElementCaptions->FirstChildElement("Max");
					if (poElementMax)
					{
						CRXImageButton* poBtn = new CRXImageButton;
						poBtn->SetID(ID_MAX);
						CRXSkinRect rcBtn;
						CRXSkinRect rcClient;
						RECT rc;
						GetClientRect(m_hWnd, &rc);
						rcClient = rc;
						rcBtn = rcClient;
						CHAR* pszSrc = (CHAR*)poElementMax->Attribute("normalsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetNormalImage(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementMax->Attribute("hotsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetHotImage(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementMax->Attribute("downsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetDownImage(strPath.c_str());
						}

						CHAR* cpValue = (CHAR*)poElementMax->Attribute("OffSetX");
						if (cpValue)
						{
							rcBtn.right = rcClient.right - atoi(cpValue);
						}
						cpValue = (CHAR*)poElementMax->Attribute("OffSetY");
						if (cpValue)
						{
							rcBtn.top = rcClient.top + atoi(cpValue);
						}
						cpValue = (CHAR*)poElementMax->Attribute("width");
						if (cpValue)
						{
							rcBtn.left = rcBtn.right - atoi(cpValue);
						}
						cpValue = (CHAR*)poElementMax->Attribute("height");
						if (cpValue)
						{
							rcBtn.bottom = rcBtn.top + atoi(cpValue);
						}
						poBtn->SetRect(rcBtn);
						poBtn->SetParentWnd(this->m_hWnd);
						cpValue = (CHAR*)poElementMax->Attribute("tooltip");
						if (cpValue)
						{
							poBtn->SetTooltip(_RXTUTF82T(cpValue).c_str());
						}
						cpValue = (CHAR*)poElementMax->Attribute("text");
						if (cpValue)
						{
							poBtn->SetText(_RXTUTF82T(cpValue).c_str());
						}
						poBtn->SetResize(RXSKIN_ANCHOR_TOPRIGHT, RXSKIN_ANCHOR_TOPRIGHT);
						m_oControlMgr.AddControl(poBtn);
					}
					tinyxml2::XMLElement* poElementMin = poElementCaptions->FirstChildElement("Min");
					if (poElementMin)
					{
						CRXImageButton* poBtn = new CRXImageButton;
						poBtn->SetParentWnd(this->m_hWnd);
						poBtn->SetID(ID_MIN);
						CRXSkinRect rcBtn;
						CRXSkinRect rcClient;
						RECT rc;
						GetClientRect(m_hWnd, &rc);
						rcClient = rc;
						rcBtn = rcClient;
						CHAR* pszSrc = (CHAR*)poElementMin->Attribute("normalsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetNormalImage(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementMin->Attribute("hotsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetHotImage(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementMin->Attribute("downsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetDownImage(strPath.c_str());
						}

						CHAR* cpValue = (CHAR*)poElementMin->Attribute("OffSetX");
						if (cpValue)
						{
							rcBtn.right = rcClient.right - atoi(cpValue);
						}
						cpValue = (CHAR*)poElementMin->Attribute("OffSetY");
						if (cpValue)
						{
							rcBtn.top = rcClient.top + atoi(cpValue);
						}
						cpValue = (CHAR*)poElementMin->Attribute("width");
						if (cpValue)
						{
							rcBtn.left = rcBtn.right - atoi(cpValue);
						}
						cpValue = (CHAR*)poElementMin->Attribute("height");
						if (cpValue)
						{
							rcBtn.bottom = rcBtn.top + atoi(cpValue);
						}
						poBtn->SetRect(rcBtn);
						poBtn->SetParentWnd(this->m_hWnd);
						cpValue = (CHAR*)poElementMin->Attribute("tooltip");
						if (cpValue)
						{
							poBtn->SetTooltip(_RXTUTF82T(cpValue).c_str());
						}
						cpValue = (CHAR*)poElementMin->Attribute("text");
						if (cpValue)
						{
							poBtn->SetText(_RXTUTF82T(cpValue).c_str());
						}
						poBtn->SetResize(RXSKIN_ANCHOR_TOPRIGHT, RXSKIN_ANCHOR_TOPRIGHT);
						m_oControlMgr.AddControl(poBtn);
					}

					tinyxml2::XMLElement* poElementRestore = poElementCaptions->FirstChildElement("Restore");
					if (poElementMin)
					{
						CRXImageButton* poBtn = new CRXImageButton;
						poBtn->SetParentWnd(this->m_hWnd);
						poBtn->SetID(ID_RESTORE);
						poBtn->SetVisible(FALSE);
						CRXSkinRect rcBtn;
						CRXSkinRect rcClient;
						RECT rc;
						GetClientRect(m_hWnd, &rc);
						rcClient = rc;
						rcBtn = rcClient;
						CHAR* pszSrc = (CHAR*)poElementRestore->Attribute("normalsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetNormalImage(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementRestore->Attribute("hotsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetHotImage(strPath.c_str());
						}
						pszSrc = (CHAR*)poElementRestore->Attribute("downsrc");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetDownImage(strPath.c_str());
						}

						CHAR* cpValue = (CHAR*)poElementRestore->Attribute("OffSetX");
						if (cpValue)
						{
							rcBtn.right = rcClient.right - atoi(cpValue);
						}
						cpValue = (CHAR*)poElementRestore->Attribute("OffSetY");
						if (cpValue)
						{
							rcBtn.top = rcClient.top + atoi(cpValue);
						}
						cpValue = (CHAR*)poElementRestore->Attribute("width");
						if (cpValue)
						{
							rcBtn.left = rcBtn.right - atoi(cpValue);
						}
						cpValue = (CHAR*)poElementRestore->Attribute("height");
						if (cpValue)
						{
							rcBtn.bottom = rcBtn.top + atoi(cpValue);
						}
						poBtn->SetRect(rcBtn);
						poBtn->SetParentWnd(this->m_hWnd);
						cpValue = (CHAR*)poElementRestore->Attribute("tooltip");
						if (cpValue)
						{
							poBtn->SetTooltip(_RXTUTF82T(cpValue).c_str());
						}
						cpValue = (CHAR*)poElementRestore->Attribute("text");
						if (cpValue)
						{
							poBtn->SetText(_RXTUTF82T(cpValue).c_str());
						}
						poBtn->SetResize(RXSKIN_ANCHOR_TOPRIGHT, RXSKIN_ANCHOR_TOPRIGHT);
						m_oControlMgr.AddControl(poBtn);
					}

					tinyxml2::XMLElement* poElementTitle = poElementCaptions->FirstChildElement("Title");
					if (poElementTitle)
					{
						CRXTitle* poBtn = new CRXTitle;
						poBtn->SetParentWnd(this->m_hWnd);
						poBtn->SetID(ID_TITLE);
						CRXSkinRect rcBtn;
						CRXSkinRect rcClient;
						RECT rc;
						GetClientRect(m_hWnd, &rc);
						rcClient = rc;
						rcBtn = rcClient;
						CHAR* pszSrc = (CHAR*)poElementTitle->Attribute("src");
						if (pszSrc)
						{
							tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
							strPath += _RXTUTF82T(pszSrc);
							poBtn->SetImage(strPath.c_str());
						}

						CHAR* cpValue = (CHAR*)poElementTitle->Attribute("OffSetX");
						if (cpValue)
						{
							rcBtn.left = rcClient.left + atoi(cpValue);
						}
						cpValue = (CHAR*)poElementTitle->Attribute("OffSetY");
						if (cpValue)
						{
							rcBtn.top = rcClient.top + atoi(cpValue);
						}
						cpValue = (CHAR*)poElementTitle->Attribute("width");
						if (cpValue)
						{
							rcBtn.right = rcBtn.left + atoi(cpValue);
						}
						cpValue = (CHAR*)poElementTitle->Attribute("height");
						if (cpValue)
						{
							rcBtn.bottom = rcBtn.top + atoi(cpValue);
						}
						CRXSkinResizePoint ptTopLeft, ptBottomRight;
						CHAR* pszAlignTopLeft = (CHAR*)poElementTitle->Attribute("aligntopleft");
						if (pszAlignTopLeft)
						{
							ptTopLeft = RXSkinGetAlignType(pszAlignTopLeft);
						}
						CHAR* pszAlignBottomRight = (CHAR*)poElementTitle->Attribute("alignbottomright");
						if (pszAlignBottomRight)
						{
							ptBottomRight = RXSkinGetAlignType(pszAlignBottomRight);
						}

						poBtn->SetRect(rcBtn);
						poBtn->SetParentWnd(this->m_hWnd);
						cpValue = (CHAR*)poElementTitle->Attribute("tooltip");
						if (cpValue)
						{
							poBtn->SetTooltip(_RXTUTF82T(cpValue).c_str());
						}
						cpValue = (CHAR*)poElementTitle->Attribute("text");
						if (cpValue)
						{
							poBtn->SetText(_RXTUTF82T(cpValue).c_str());
							SetTitle(_RXTUTF82T(cpValue).c_str());
						}
						poBtn->SetResize(ptTopLeft, ptBottomRight);
						m_oControlMgr.AddControl(poBtn);
					}
				}
			}
			tinyxml2::XMLElement* poElementControls = poRoot->FirstChildElement("Controls");
			if (poElementControls)
			{
				tinyxml2::XMLElement* poElementControl = poElementControls->FirstChildElement("Control");
				while (poElementControl)
				{
					CRXSkinService::Instance()->GetDrawMgr().CreateControl(poElementControl, &m_oControlMgr, this);
					poElementControl = poElementControl->NextSiblingElement("Control");
				}
			}
		}
		delete poDoc;
	}

	return TRUE;
}

CRXControl* CRXSkinChildWnd::OnCreateControl(tinyxml2::XMLElement* poElement)
{
	return NULL;
}

TCHAR* CRXSkinChildWnd::GetTitle()
{
	return m_szTitle;
}

VOID CRXSkinChildWnd::SetTitle(const TCHAR* pszTitle)
{
	memset(m_szTitle, 0, sizeof(TCHAR) * 256);
	_tcsncpy(m_szTitle, pszTitle, 256);
	CRXTitle* poTitle = (CRXTitle*)m_oControlMgr.FindControl(ID_TITLE);
	if (poTitle)
	{
		poTitle->SetText(m_szTitle);
		poTitle->Invalidate();
	}
}

VOID CRXSkinChildWnd::AddAnchor(HWND hWnd, CRXSkinResizePoint ptTopLeft, CRXSkinResizePoint ptBottomRight)
{
	CLayoutMapItr itr = m_mapLayout.find(hWnd);
	if (itr == m_mapLayout.end())
	{
		CLayout* poLayout = new CLayout;
		poLayout->SetWnd(hWnd);
		poLayout->SetParentWnd(m_hWnd);
		poLayout->SetTopLeft(ptTopLeft);
		poLayout->SetBottomRight(ptBottomRight);
		poLayout->SetResize(ptTopLeft, ptBottomRight);
		m_mapLayout.insert(make_pair(poLayout->GetWnd(), poLayout));
	}
}

HBRUSH CRXSkinChildWnd::OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC)
{
	return m_oControlMgr.OnCtlColorControl(dwMsgID,hWndChild, hDC);
}


LRESULT CRXSkinChildWnd::OnSystemCommand(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case SC_MINIMIZE:
	{
		RECT rcWin;
		::GetWindowRect(m_hWnd, &rcWin);
		m_rcRestore = rcWin;
		return 0;
	}
	default:
		break;
	}
	return 0;
}

