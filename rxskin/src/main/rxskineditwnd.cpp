#include "pch.h"
#include "main/rxskineditwnd.h"
#include "common/drawmgr.h"
#include "main/rxskinapp.h"
#include "common/rxskinservice.h"
#include <olectl.h>
#include "windowsx.h"
#include "commctrl.h"
#include "WinUser.h"
#include "windows.h"

CRXSkinEditWnd::CRXSkinEditWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false), m_bDrawCaret(false)
{
	m_hFont = NULL;
	m_bIsReadOnly = FALSE;
	m_clrBorder = RGB(201, 212, 215);
	SetAutoDelete(true);
}

CRXSkinEditWnd::~CRXSkinEditWnd()
{
}

BOOL CRXSkinEditWnd::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinWnd::OnCreate(lpCreate);
	if (bResult)
	{
 		SetText(m_strText.c_str());
		if (m_pOwner->IsPassword())
		{
	
			SendMessage(EM_SETPASSWORDCHAR, WPARAM(L'¡ñ'), LPARAM());
	//		SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | ES_PASSWORD);
		}
	}
	return bResult;
}

BOOL CRXSkinEditWnd::Create(const TCHAR* pszWindowClassName,
	const TCHAR* pszWindowName,
	const UINT32 dwStyle,
	const HWND hParentWnd,
	HICON hIcon)
{

	RegisterSuperclass(pszWindowClassName);
	 
	RECT rcPos = CalPos();
	UINT uStyle = 0;
	uStyle = WS_CHILD | ES_AUTOHSCROLL;
	UINT uTextStyle = m_pOwner->GetTextStyle();
	if (uTextStyle & DT_LEFT) uStyle |= ES_LEFT;
	else if (uTextStyle & DT_CENTER) uStyle |= ES_CENTER;
	else if (uTextStyle & DT_RIGHT) uStyle |= ES_RIGHT;
	if (m_pOwner->IsPassword()) uStyle |= ES_PASSWORD;
	if (m_bIsReadOnly)
	{
		uStyle |= ES_READONLY;
	}

	if (uTextStyle & DT_LEFT) uStyle |= ES_LEFT;
	else if (uTextStyle & DT_CENTER) uStyle |= ES_CENTER;
	else if (uTextStyle & DT_RIGHT) uStyle |= ES_RIGHT;
	BOOL bResult = CRXSkinWnd::Create(pszWindowClassName, pszWindowName, uStyle, hParentWnd, hIcon);
	if (bResult)
	{
		HFONT hFont = m_hFont;
		if (hFont == NULL)
		{
			hFont = CRXSkinService::Instance()->GetDrawMgr().GetEditFont();
		}

		SetWindowFont(m_hWnd, hFont, TRUE);
		Edit_LimitText(m_hWnd, 128);
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
		::SetFocus(m_hWnd);
		ShowWindow(m_hWnd, SW_SHOW);
		::SetWindowText(m_hWnd, m_strText.c_str());
	}
	return bResult;
}

RECT CRXSkinEditWnd::CalPos()
{
	CRXSkinRect rcPos = m_pOwner->GetRect();
	RECT rcInset;
	rcInset.left = 2;
	rcInset.top = 2;
	rcInset.right = 2;
	rcInset.bottom = 2;
	rcPos.left += rcInset.left;
	rcPos.top += rcInset.top;
	rcPos.right -= rcInset.right;
	rcPos.bottom -= rcInset.bottom;
	LONG lEditHeight = 20;
	if (lEditHeight < rcPos.Height()) {
		rcPos.top += (rcPos.Height() - lEditHeight) / 2;
		rcPos.bottom = rcPos.top + lEditHeight;
	}

	CRXControl* pParent = m_pOwner;
	RECT rcParent;
	if (pParent) {
		if (!pParent->IsVisible()) {
			rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;

		}
		rcParent = pParent->GetRect();
		if (!::IntersectRect(&rcPos, &rcPos, &rcParent)) {
			rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;

		}
	}

	return rcPos;
}

void CRXSkinEditWnd::OnFinalMessage(HWND hWnd)
{

}

void CRXSkinEditWnd::SetText(const TCHAR* pszText) 
{
	if (false == IsValid())
	{
		m_strText = pszText;
		return;
	}
	::SetWindowText(GetHWnd(), pszText);
	
	int nSize = GetWindowTextLength(GetHWnd());
	if (nSize == 0) nSize = 1;
	Edit_SetSel(GetHWnd(), nSize, nSize);
}


int CRXSkinEditWnd::GetIntValue()
{
	tstring strValue = GetTextValue();
	return _ttoi(strValue.c_str());
}

tstring CRXSkinEditWnd::GetTextValue()
{
	TCHAR szMsg[1024] = { 0 };
	if (m_hWnd)
	{
		::Edit_GetText(m_hWnd, szMsg, 1024);
	}
	return tstring(szMsg);
}

LRESULT CRXSkinEditWnd::OnHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE;
	if (uMsg == WM_CREATE) {
		bHandled = FALSE;
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		return OnCreate(lpcs);
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		m_pOwner->OnEditKillFocus(m_hWnd);
		bHandled = FALSE;
	}
	else if (uMsg == WM_SETFOCUS)
	{
		m_pOwner->OnEditSetFocus(m_hWnd);
		bHandled = FALSE;
	}
	else if (uMsg == OCM_COMMAND) {
		if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
		else if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE) {
			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			::InvalidateRect(m_hWnd, &rcClient, FALSE);
		}
		bHandled = TRUE;
	}
	else if (uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN) {
		bHandled = TRUE;
		//	m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);
	}
	else if (uMsg == WM_KEYDOWN ) {
		bHandled = TRUE;
		return OnKeyDown(wParam, lParam);
	}
	else if (uMsg == WM_KEYUP) {
		bHandled = TRUE;
		return OnKeyUp(wParam, lParam);
	}
	else if (uMsg == OCM__BASE + WM_CTLCOLOREDIT || uMsg == OCM__BASE + WM_CTLCOLORSTATIC) {
		::SetBkMode((HDC)wParam, TRANSPARENT);
		DWORD dwTextColor = m_pOwner->GetTextColor();
		::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));
		DWORD clrColor = m_pOwner->GetBkColor();
		if (clrColor < 0xFF000000) {
		}
		else {
			if (m_hBkBrush == NULL) {
				m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
			}
		}
		return (LRESULT)m_hBkBrush;
	}
	else if (uMsg == WM_PAINT) {
		bHandled = FALSE;
		OnPaint();
	}
	else if (uMsg == WM_PRINT) {
		bHandled = FALSE;
		
	}

	else if (uMsg == WM_DESTROY) {
		bHandled = FALSE;
	}
	else if (uMsg == WM_TIMER) {
		if (wParam == CARET_TIMERID) {
			m_bDrawCaret = !m_bDrawCaret;
			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			::InvalidateRect(m_hWnd, &rcClient, FALSE);
			return 0;
		}
		bHandled = FALSE;
	}
	else bHandled = FALSE;

	if (!bHandled) return ::CallWindowProc(m_OldWndProc, m_hWnd, uMsg, wParam, lParam);
	return lRes;
}

VOID CRXSkinEditWnd::Destroy()
{
	CRXSkinWnd::Destroy();
}

void CRXSkinEditWnd::EnableWindow(const BOOL& bEnable)
{
	if (m_hWnd)
	{
		::EnableWindow(m_hWnd, bEnable);
	}
}

void CRXSkinEditWnd::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	DrawRectEdge(hDC, rcItem, 2, m_clrBorder);
}

VOID CRXSkinEditWnd::OnPaint()
{
	CRXSkinWnd::OnPaint();
	
}

HBRUSH CRXSkinEditWnd::OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC)
{
	::SetBkMode((HDC)hDC, TRANSPARENT);
	DWORD dwTextColor = m_pOwner->GetTextColor();
	::SetTextColor((HDC)hDC, m_pOwner->GetTextColor());
	DWORD clrColor = m_pOwner->GetBkColor();
	if (clrColor < 0xFF000000) {
		if (m_pOwner->IsDCReady())
		{
			HBITMAP hBmpEditBk = m_pOwner->GetBKBitmap();
			if (hBmpEditBk)
			{
				m_hBkBrush = ::CreatePatternBrush(hBmpEditBk);
			}
		}
	}
	else {
		if (m_hBkBrush == NULL) {
			m_hBkBrush = ::CreateSolidBrush(m_pOwner->GetBkColor());
		}
	}
	return m_hBkBrush;
}


LRESULT CRXSkinEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	return lRes;
}

void CRXSkinEditWnd::SetIsReadOnly(const BOOL& bIsReadOnly)
{
	m_bIsReadOnly = bIsReadOnly;
	if (m_hWnd)
	{
		SendMessage(EM_SETREADONLY, m_bIsReadOnly, LPARAM());
	}
}

LRESULT CRXSkinEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!m_bInit) return 0; 
	if (m_pOwner == NULL) return 0;
	// Copy text back
	int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
	LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
	ASSERT(pstr);
	if (pstr == NULL) return 0;
	::GetWindowText(m_hWnd, pstr, cchLen);
	if (_RXTStrcmp(pstr, m_strText.c_str()) != 0)
	{
		m_pOwner->OnEditChanged(m_hWnd);
	}
	m_pOwner->SetText(pstr);
	// 	m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
	m_pOwner->Invalidate();
	return 0;
}


LRESULT CRXSkinEditWnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(m_OldWndProc, m_hWnd, WM_KEYDOWN, wParam, lParam);
}
LRESULT CRXSkinEditWnd::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	if (m_pOwner == NULL) return 0;
	// Copy text back
	int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
	LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
	ASSERT(pstr);
	if (pstr == NULL) return 0;
	::GetWindowText(m_hWnd, pstr, cchLen);
 	if (_RXTStrcmp(pstr, m_strText.c_str()) != 0)
	{
		m_pOwner->OnEditChanged(m_hWnd);
		m_strText = pstr;
	}
	return ::CallWindowProc(m_OldWndProc, m_hWnd, WM_KEYUP, wParam, lParam);

}
LRESULT CRXSkinEditWnd::OnSysChar(WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(m_OldWndProc, m_hWnd, WM_SYSCHAR, wParam, lParam);

}
LRESULT CRXSkinEditWnd::OnSysKeydown(WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(m_OldWndProc, m_hWnd, WM_SYSKEYDOWN, wParam, lParam);

}