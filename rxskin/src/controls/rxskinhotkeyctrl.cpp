#include "pch.h"
#include "controls\rxskinhotkeyctrl.h"
#include "common\RXSkinService.h"
#include "common\graphics.h"
#include <olectl.h>
#include "windowsx.h"
#include "commctrl.h"

CRXSkinHotkeyCtrl::CRXSkinHotkeyCtrl(void)
{
	m_hEditWnd = NULL;
	m_bBoder = TRUE;
	m_OldWndProc = ::DefWindowProc;
	m_hBkBrush = NULL;
	m_bFinished = FALSE;
	m_dwVirtualKeyCode = 0;
	m_dwModifiers = 0;
}

CRXSkinHotkeyCtrl::~CRXSkinHotkeyCtrl(void)
{
	if (m_hEditWnd)
	{
		::DestroyWindow(m_hEditWnd);
		m_hEditWnd = NULL;
	}
}


HWND CRXSkinHotkeyCtrl::GetEditWnd()
{
	return m_hEditWnd;
}

VOID CRXSkinHotkeyCtrl::SetText(const TCHAR* strTxt)
{
	CRXControl::SetText(strTxt);
	if (m_hEditWnd)
	{
		::SetWindowText(m_hEditWnd,strTxt);
	}
}

VOID CRXSkinHotkeyCtrl::Enable(const BOOL bEnabled)
{
	CRXControl::Enable(bEnabled);
	if (m_hEditWnd)
	{
		::EnableWindow(m_hEditWnd,bEnabled);
	}
}

CRXSkinString CRXSkinHotkeyCtrl::GetWindowText()
{
	CRXSkinString strContent;
	if (m_hEditWnd)
	{
		static TCHAR szContent[256]={0};
		memset(szContent,0,sizeof(TCHAR)*256);
		::GetWindowText(m_hEditWnd,szContent,256);
		strContent = szContent;
	}
	return strContent;
}

VOID CRXSkinHotkeyCtrl::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible() ==FALSE)
	{
		return;
	}
	SetRect(rcItem);
	if (IsBorder())
	{
		CRXSkinService::Instance()->GetDrawMgr().DrawControlBorder(hDC,GetState(),GetRect(),IsEnabled());
	}
}

VOID CRXSkinHotkeyCtrl::ReleaseControl()
{
	delete this;
}

BOOL CRXSkinHotkeyCtrl::PtInRect(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}
BOOL CRXSkinHotkeyCtrl::OnClick(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinHotkeyCtrl::OnMouseMove(POINT pt)
{
	return PtInRect(pt);
}
BOOL CRXSkinHotkeyCtrl::OnLButtonDown(POINT pt)
{
	return FALSE;
}

VOID CRXSkinHotkeyCtrl::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
	if (m_hEditWnd)
	{
		if (IsVisible())
		{
			ShowWindow(m_hEditWnd,SW_SHOW);
		}
		else
			ShowWindow(m_hEditWnd,SW_HIDE);
	}
}

BOOL CRXSkinHotkeyCtrl::OnTimer(WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXControl::OnTimer(wParam,lParam);
	return bResult;
}

BOOL CRXSkinHotkeyCtrl::Create()
{
	INT32 nOffSet = CRXSkinService::Instance()->GetDrawMgr().GetControlBorderSize();

	m_hEditWnd = CreateWindow(_T("Edit"),
		_T(""),
		WS_CHILD|WS_VISIBLE|ES_LEFT,
		GetRect().left+nOffSet,
		GetRect().top+nOffSet,
		GetRect().Width() - 2 * nOffSet ,
		GetRect().Height() - nOffSet,
		GetParentWnd(),
		NULL,
		NULL,
		NULL);

	if (m_hEditWnd)
	{
		m_OldWndProc = SubclassWindow(m_hEditWnd,CRXSkinHotkeyCtrl::_WndHotkeyProc);
		::SetWindowLongPtr(m_hEditWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(this));
		::SendMessage(m_hEditWnd,WM_SETFONT,(WPARAM)CRXSkinService::Instance()->GetDrawMgr().GetNormalFont(),MAKELONG(0,0));
		Edit_SetReadOnly(m_hEditWnd,true);
	}

	return TRUE;
}

BOOL CRXSkinHotkeyCtrl::IsBorder()
{
	return m_bBoder;
}

VOID CRXSkinHotkeyCtrl::SetBorder(const BOOL bBorder)
{
	m_bBoder = bBorder;
}

LRESULT CALLBACK CRXSkinHotkeyCtrl::_WndHotkeyProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CRXSkinHotkeyCtrl* pThis = NULL;
	if( message == WM_NCCREATE ) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<CRXSkinHotkeyCtrl*>(lpcs->lpCreateParams);
		pThis->m_hEditWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
	} 
	else {
		pThis = reinterpret_cast<CRXSkinHotkeyCtrl*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if( message == WM_NCDESTROY && pThis != NULL ) {
			LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, message, wParam, lParam);
			::SetWindowLongPtr(pThis->m_hEditWnd, GWLP_USERDATA, 0L);
			pThis->m_hEditWnd = NULL;
			pThis->OnFinalMessage(hWnd);
			return lRes;
		}
	}
	if( pThis != NULL ) {
		return pThis->OnHandleMessage(message, wParam, lParam);
	}

	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void CRXSkinHotkeyCtrl::OnFinalMessage(HWND /*hWnd*/)
{
	Invalidate();
	if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
}

LRESULT CRXSkinHotkeyCtrl::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = ::DefWindowProc(m_hEditWnd, uMsg, wParam, lParam);
/*	::PostMessage(m_hEditWnd, WM_CLOSE, wParam, lParam);*/
	return lRes;
}

LRESULT CRXSkinHotkeyCtrl::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Copy text back
	int cchLen = ::GetWindowTextLength(m_hEditWnd) + 1;
	LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
	ASSERT(pstr);
	if( pstr == NULL ) return 0;
	::GetWindowText(m_hEditWnd, pstr, cchLen);
// 	m_pOwner->m_sText = pstr;
// 	m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
	return 0;
}


LRESULT CRXSkinHotkeyCtrl::OnHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	if( uMsg == WM_KILLFOCUS ) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
	else if( uMsg == OCM_COMMAND ) {
		if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
		else if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE ) {
			RECT rcClient;
			::GetClientRect(m_hEditWnd, &rcClient);
			::InvalidateRect(m_hEditWnd, &rcClient, FALSE);
		}
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		::SetFocus(m_hEditWnd);
	}
	else if( uMsg == WM_KEYDOWN) 
	{
		OnKeyDown(wParam,lParam);
	}
	else if (uMsg == WM_KEYUP)
	{
		OnKeyUp(wParam,lParam);
	}
	else if (uMsg == WM_CHAR)
	{
		OnChar(wParam,lParam);
	}
	else if( uMsg == OCM__BASE + WM_CTLCOLOREDIT  || uMsg == OCM__BASE + WM_CTLCOLORSTATIC ) {
		::SetBkMode((HDC)wParam, TRANSPARENT);
		DWORD dwTextColor = CRXSkinService::Instance()->GetDrawMgr().GetNormalText();
		::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor)));
		if( m_hBkBrush == NULL ) {
			DWORD clrColor = CRXSkinService::Instance()->GetDrawMgr().GetCommonBk();
			m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
		}
		return (LRESULT)m_hBkBrush;
	}
	else bHandled = FALSE;
	if( !bHandled ) return ::CallWindowProc(m_OldWndProc, m_hEditWnd, uMsg, wParam, lParam);
	return lRes;
}

LRESULT CRXSkinHotkeyCtrl::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	if (m_bFinished)
	{
		m_bFinished = FALSE;
		m_dwVirtualKeyCode  = 0;
		m_dwModifiers = 0;
		if (::GetKeyState (VK_SHIFT) & 0x80 )
		{
			m_dwVirtualKeyCode |= HOTKEYF_SHIFT;
		}
		else if (::GetKeyState (VK_CONTROL) & 0x80)
		{
			m_dwVirtualKeyCode |= HOTKEYF_CONTROL;
		}
	}
	else 
	{
		if (::GetKeyState (VK_SHIFT) & 0x80 )
		{
			m_dwVirtualKeyCode |= HOTKEYF_SHIFT;
		}
		else if (::GetKeyState (VK_CONTROL) & 0x80)
		{
			m_dwVirtualKeyCode |= HOTKEYF_CONTROL;
		}
	}
	Edit_SetText(m_hEditWnd,_GetKeyText().GetData());
	return 0;
}

LRESULT CRXSkinHotkeyCtrl::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	m_bFinished = TRUE;
	return 0;
}

LRESULT CRXSkinHotkeyCtrl::OnChar(WPARAM wParam, LPARAM lParam)
{
	m_dwModifiers = (UINT32)(wParam);
	Edit_SetText(m_hEditWnd,_GetKeyText());
	return 0;
}

CRXSkinString CRXSkinHotkeyCtrl::_GetKeyText()
{
	CRXSkinString strKeys = _T("");
	if (m_dwVirtualKeyCode & HOTKEYF_SHIFT)
	{
		strKeys += _T("SHIFT + ");
	}
	if (m_dwVirtualKeyCode & HOTKEYF_CONTROL)
	{
		strKeys += _T("CTRL + ");
	}
	TCHAR szChar = m_dwModifiers;
	strKeys += szChar;
	return strKeys;
}