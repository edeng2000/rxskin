#include "pch.h"
#include "controls/rxskincefbrowser.h"
#include "common/RXSkinService.h"

CRXSkinCefBrowser::CRXSkinCefBrowser(void)
{
	m_poBrowser = NULL;
}

CRXSkinCefBrowser::~CRXSkinCefBrowser()
{
	if (m_poBrowser)
	{
	//	m_poBrow//ser->ReleaseBrowser();
	}
}

void CRXSkinCefBrowser::SetHandler(ICefCallbackHandler* poHandler)
{
	//m_poHandler = poHandler;
}

bool CRXSkinCefBrowser::CreateBrowser(const HWND& hParentWnd,
	const TCHAR* pszUrl,
	const CRXSkinRect& rcItem)
{
	m_poBrowser = m_poApp->GetCefModel()->CreateBrowser(hParentWnd, pszUrl, rcItem);
	return true;
}

void  CRXSkinCefBrowser::WriteText(const TCHAR* pszText)
{
	if (m_poBrowser)
	{
	//	m_poBrowser->WriteText(pszText);
	}
}

void CRXSkinCefBrowser::AppendText(const TCHAR* pszText)
{
	if (m_poBrowser)
	{
		m_poBrowser->AppendText(pszText);
	}
}

void CRXSkinCefBrowser::Empty()
{
	if (m_poBrowser)
	{
		m_poBrowser->Empty();
	}
}

VOID CRXSkinCefBrowser::ReleaseControl()
{
	if (m_poBrowser)
	{
		m_poApp->GetCefModel()->ReleaseBrowser(m_poBrowser);
	}
	delete this;
}
BOOL CRXSkinCefBrowser::PtInRect(POINT pt)
{
	return FALSE;
}
BOOL CRXSkinCefBrowser::OnClick(POINT pt)
{
	return FALSE;
}
BOOL CRXSkinCefBrowser::OnMouseMove(POINT pt)
{
	return FALSE;
}
BOOL CRXSkinCefBrowser::OnLButtonDown(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinCefBrowser::OnSize(const UINT32 dwX, const UINT32 dwY)
{
	BOOL bReusult = CRXControl::OnSize(dwX,dwY);
	if (m_poBrowser)
	{
		m_poBrowser->MoveWindow(m_rcItem);
	}	
	return bReusult;
}
VOID CRXSkinCefBrowser::SetVisible(const BOOL bIsVisible)
{
	if (m_poBrowser)
	{
		m_poBrowser->SetVisible(bIsVisible);
	}
}
HBRUSH CRXSkinCefBrowser::OnCtlColorControl(HWND hWndChild, HDC hDC)
{
	return NULL;
}

VOID CRXSkinCefBrowser::OnDraw(HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible() == FALSE)
	{
		return;
	}
	SetRect(rcItem);
	HPEN	hpen;
	HGDIOBJ hpenOld;

	hpen = ::CreatePen(PS_SOLID, 1, CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor());
	hpenOld = ::SelectObject(hDC, hpen);
	::MoveToEx(hDC, rcItem.left - 2, rcItem.top, NULL);
	::LineTo(hDC, rcItem.left - 2, rcItem.bottom);
	::LineTo(hDC, rcItem.right, rcItem.bottom);
	::LineTo(hDC, rcItem.right, rcItem.top);
	::SelectObject(hDC, hpenOld);
	::DeleteObject(hpen);
}

