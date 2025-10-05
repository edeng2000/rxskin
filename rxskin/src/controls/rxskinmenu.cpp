#include "pch.h"
#include "controls\RXSkinMenu.h"
#include "common\graphics.h"
#include "common\rxskinutil.h"
#include "common\RXSkinMemDC.h"
#include "common\RXSkinService.h"
#include "fundation/rxstring.h"

#include "commctrl.h"

#define ARROW_OFFSET_X          5


CRXSkinMenuItem::CRXSkinMenuItem()
{
	m_enType = MENU_STRING;
	memset(m_szName,0,sizeof(TCHAR)*256);
	m_poImage = NULL;
	m_bChecked = FALSE;
	m_poChildMenu = NULL;
}

CRXSkinMenuItem::~CRXSkinMenuItem()
{
}

VOID CRXSkinMenuItem::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	switch(m_enType)
	{
	case MENU_STRING:
		{
			_DrawString(poSurface, hDC, rcItem);
		}
		break;
	case MENU_SEPERTOR:
		{
			_DrawSperator(poSurface, hDC, rcItem);
			break;
		}
	case MENU_CHILD:
	{
		_DrawSubMenu(poSurface, hDC, rcItem);
	}
	break;
	case MENU_CHECKBOX:
	{
		_DrawCheckedMenu(poSurface,hDC,rcItem);
	}
	break;
	default:
		break;
	}
}

TCHAR* CRXSkinMenuItem::GetName()
{
	return m_szName;
}

VOID CRXSkinMenuItem::SetName(const TCHAR* pszName)
{
	memset(m_szName,0,sizeof(TCHAR)*256);
	_tcsncpy(m_szName,pszName,256);
}

VOID CRXSkinMenuItem::SetImage(const TCHAR* pszImageFile)
{
	m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenImage(pszImageFile);
}
VOID CRXSkinMenuItem::SetImage(const UINT32 dwImageID)
{
	m_poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(dwImageID);
}
UINT32 CRXSkinMenuItem::GetWidth()
{
	UINT32 dwWidth = CRXSkinService::Instance()->GetDrawMgr().GetMenuSidebarWidth();
	dwWidth += CRXSkinService::Instance()->GetDrawMgr().GetMenuItemOffSetX();
	if (MENU_SEPERTOR != m_enType)
	{
		tstring strTxt = m_szName;
		dwWidth += CRXSkinFontMgr::Instance()->GetTextWidth(ID_FONT_COMMON, strTxt);
		if (MENU_CHILD == m_enType)
		{
			dwWidth += CRXSkinService::Instance()->GetDrawMgr().GetMenuItemArrowWidth();
		}
	}
	return dwWidth;
}

UINT32 CRXSkinMenuItem::GetHeight()
{
	UINT32 dwHeight = CRXSkinService::Instance()->GetDrawMgr().GetMenuSeperatorHeight();
	if (m_enType!= MENU_SEPERTOR)
	{
		dwHeight = CRXSkinService::Instance()->GetDrawMgr().GetMenuItemHeight();
	}
	else
	{
		dwHeight = CRXSkinService::Instance()->GetDrawMgr().GetMenuSeperatorHeight();
	}
	return dwHeight;
}

CRXSkinItem* CRXSkinMenuItem::HitTest(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return this;
	}
	return NULL;
}

EMenuItemType CRXSkinMenuItem::GetType()
{
	return m_enType;
}

VOID CRXSkinMenuItem::SetType(EMenuItemType enType)
{
	m_enType = enType;
}

void CRXSkinMenuItem::_DrawCheckedMenu(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	_DrawString(poSurface,hDC,rcItem);
	if (m_bChecked)
	{
		CRXSkinRect rcItem = GetRect();
		IRXImage* poImage = NULL;
		poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_MENU_CHECKED_SELECTED);
		if (poImage)
		{
			rcItem.top = GetRect().top + (rcItem.Height() - poImage->GetHeight()) / 2;
			rcItem.left = rcItem.left + (CRXSkinService::Instance()->GetDrawMgr().GetMenuSidebarWidth() - poImage->GetWidth()) / 2;
			rcItem.right = rcItem.left + poImage->GetWidth();
			rcItem.bottom = rcItem.top + poImage->GetHeight();
			poImage->Draw(hDC, rcItem);
		}
	}
}

void CRXSkinMenuItem::_DrawSubMenu(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	_DrawString(poSurface,hDC,rcItem);
	COLORREF clrArrow = CRXSkinService::Instance()->GetDrawMgr().GetArrowNormal();
	INT32 nTop = rcItem.top + (rcItem.Height()) / 2 - 2;
	
	IRXImage* poImage = NULL;
	poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_ARROW_RIGHT_NORMAL);
	if (poImage)
	{
		rcItem.top = GetRect().top + (rcItem.Height() - poImage->GetHeight()) / 2;
		rcItem.left = rcItem.right - ARROW_OFFSET_X - poImage->GetWidth();
		rcItem.right = rcItem.left + poImage->GetWidth();
		rcItem.bottom = rcItem.top + poImage->GetHeight();
		poImage->Draw(hDC, rcItem);
	}
}

VOID CRXSkinMenuItem::_DrawString(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	switch(GetState())
	{
	case ITEM_NORMAL:
		{
			CRXSkinRect rcClient = GetRect();
			rcClient.left += CRXSkinService::Instance()->GetDrawMgr().GetMenuSidebarWidth();
			rcClient.left += CRXSkinService::Instance()->GetDrawMgr().GetMenuItemOffSetX();
			tstring strTxt = m_szName;
			SkCanvas* poCanvas = poSurface->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas,
					m_szName, rcClient,
					CRXSkinService::Instance()->GetDrawMgr().GetMenuItemNormalText(),
					ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			}			
		}
		break;
	case ITEM_MOUSEON:
		{
			CRXSkinRect rcClient = GetRect();
			RECT rcBK;
			rcBK.left = rcClient.left + 2;
			rcBK.right = rcClient.right - 2;
			rcBK.top = rcClient.top;
			rcBK.bottom = rcClient.bottom;
			DrawGradientRect(hDC,TRUE,rcBK,CRXSkinService::Instance()->GetDrawMgr().GetMenuItemMouseOnFromBk(),
				CRXSkinService::Instance()->GetDrawMgr().GetMenuItemMouseOnToBk());
			rcClient.left += CRXSkinService::Instance()->GetDrawMgr().GetMenuSidebarWidth();
			rcClient.left += CRXSkinService::Instance()->GetDrawMgr().GetMenuItemOffSetX();
			tstring strTxt = m_szName;
			SkCanvas* poCanvas = poSurface->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas,
					m_szName, rcClient,
					CRXSkinService::Instance()->GetDrawMgr().GetMenuItemMouseOnText(),
					ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			}					
		}
		break;
	case ITEM_PRESSED:
		{
			CRXSkinRect rcClient = GetRect();
			RECT rcBK;
			rcBK.left = rcClient.left + 2;
			rcBK.right = rcClient.right - 2;
			rcBK.top = rcClient.top;
			rcBK.bottom = rcClient.bottom;
			DrawGradientRect(hDC,TRUE,rcBK,CRXSkinService::Instance()->GetDrawMgr().GetMenuItemPressedFromBk(),
				CRXSkinService::Instance()->GetDrawMgr().GetMenuItemPressedToBk());
			rcClient.left += CRXSkinService::Instance()->GetDrawMgr().GetMenuSidebarWidth();
			rcClient.left += CRXSkinService::Instance()->GetDrawMgr().GetMenuItemOffSetX();
			tstring strTxt = m_szName;
			SkCanvas* poCanvas = poSurface->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas,
					m_szName, rcClient,
					CRXSkinService::Instance()->GetDrawMgr().GetMenuItemPressedText(),
					ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			}

		}
		break;
	default:
		break;
	}
	if (m_poImage)
	{
		CRXSkinRect rcImage = GetRect();
		rcImage.right = rcImage.left + CRXSkinService::Instance()->GetDrawMgr().GetMenuSidebarWidth();
		rcImage.left = (rcImage.Width() - m_poImage->GetWidth())/2 + rcImage.left;
		rcImage.top = rcImage.top + (rcImage.Height() - m_poImage->GetHeight())/2;
		rcImage.right = rcImage.left + m_poImage->GetWidth();
		rcImage.bottom = rcImage.top + m_poImage->GetHeight();
		m_poImage->Draw(hDC,rcImage);
	}
}

VOID CRXSkinMenuItem::_DrawSperator(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	POINT ptStart;
	ptStart.x = rcItem.left + CRXSkinService::Instance()->GetDrawMgr().GetMenuSidebarWidth()+CRXSkinService::Instance()->GetDrawMgr().GetMenuItemOffSetX();
	ptStart.y = rcItem.top + rcItem.Height() / 2;
	POINT ptEnd;
	ptEnd.x = rcItem.right - CRXSkinService::Instance()->GetDrawMgr().GetMenuItemOffSetX();
	ptEnd.y = rcItem.top + rcItem.Height() / 2;
	HPEN	hpen;
	HGDIOBJ hpenOld;

	hpen = ::CreatePen( PS_SOLID, 1, CRXSkinService::Instance()->GetDrawMgr().GetMenuSeperatorBk() );
	hpenOld = ::SelectObject( hDC, hpen );
	::MoveToEx( hDC, ptStart.x, ptStart.y, NULL );
	::LineTo( hDC, ptEnd.x, ptEnd.y );
	::SelectObject( hDC, hpenOld );
	::DeleteObject( hpen );
}



CRXSkinMenu::CRXSkinMenu(void)
{
	m_bTracking = FALSE;
	m_poMouseOn = NULL;
	m_poPressed = NULL;
	m_poParentMenu = NULL;
}

CRXSkinMenu::~CRXSkinMenu(void)
{
	_DeleteAll(); 
}

VOID CRXSkinMenu::Destroy()
{
	CRXSkinWnd::Destroy();
}


BOOL CRXSkinMenu::Create(const TCHAR* pszWindowClassName,
	const TCHAR* pszWindowName,
	const UINT32 dwStyle,
	const HWND hParentWnd,
	HICON hIcon)
{
	return CRXSkinWnd::Create(pszWindowClassName, pszWindowName, dwStyle, hParentWnd, hIcon);
}

CRXSkinMenuItem* CRXSkinMenu::AppendMenu(EMenuItemType enType, 
										 const UINT32 dwID, 
										 const TCHAR* pszName,
										 const TCHAR* pszImageFile)
{
	CRXSkinMenuItem* poItem = FindMenuItem(dwID);
	if (NULL == poItem)
	{
		poItem = new CRXSkinMenuItem;
		poItem->SetID(dwID);
		poItem->SetImage(pszImageFile);
		poItem->SetName(pszName);
		poItem->SetType(enType);
		m_mapItems.insert(make_pair(dwID,poItem));
		m_vecItems.push_back(poItem);
	}
	return poItem;
}

CRXSkinMenuItem* CRXSkinMenu::AppendMenuEx(EMenuItemType enType, 
										 const UINT32 dwID, 
										 const TCHAR* pszName,
										 const UINT32 dwImageID,
	CRXSkinMenu* poChild)
{
	CRXSkinMenuItem* poItem = FindMenuItem(dwID);
	if (NULL == poItem)
	{
		poItem = new CRXSkinMenuItem;
		poItem->SetID(dwID);
		poItem->SetImage(dwImageID);
		poItem->SetName(pszName);
		poItem->SetType(enType);
		poItem->SetChildMenu(poChild);
		if (poChild)
		{
			poChild->SetParentMenu(this);
		}
		
		m_mapItems.insert(make_pair(dwID,poItem));
		m_vecItems.push_back(poItem);
	}
	return poItem;
}

CRXSkinMenuItem* CRXSkinMenu::FindMenuItem(const UINT32 dwID)
{
	CRXSkinMenuItemMapItr itr = m_mapItems.find(dwID);
	if (itr!=m_mapItems.end())
	{
		return itr->second;
	}
	return NULL;
}

CRXSkinMenuItem* CRXSkinMenu::RemoveMenuItem(const UINT32 dwID)
{
	CRXSkinMenuItemMapItr itr = m_mapItems.find(dwID);
	if (itr!=m_mapItems.end())
	{
		CRXSkinMenuItem* poItem = itr->second;
		m_mapItems.erase(itr);
		CRXSkinMenuItemVectItr itrVect = m_vecItems.begin();
		for (;itrVect!=m_vecItems.end();itrVect++)
		{
			if ((*itrVect) == poItem)
			{
				m_vecItems.erase(itrVect);
				break;
			}
		}
		return poItem;
	}
	return NULL;
}

VOID CRXSkinMenu::_DeleteAll()
{
	CRXSkinMenuItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		delete itr->second;
	}
	m_mapItems.clear();
	m_vecItems.clear();
	m_poMouseOn = NULL;
	m_poPressed = NULL;
}


VOID CRXSkinMenu::OnFinalMessage(HWND hWnd)
{
	printf("");
}
BOOL CRXSkinMenu::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinWnd::OnCreate(lpCreate);
	if (bResult)
	{
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE)
			& ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	}
	return bResult;
}


VOID CRXSkinMenu::OnPaint()
{
	PAINTSTRUCT ps = { 0 };
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	CRXSkinMemDC memDC(hDC,rcClient);
	SIZE szRound = CRXSkinService::Instance()->GetDrawMgr().GetMenuRound();
	HRGN hRgn = ::CreateRoundRectRgn(rcClient.left,rcClient.top,
		rcClient.right,rcClient.bottom-1,szRound.cx,szRound.cy);

	DrawRgn(memDC.GetHDC(),
		CRXSkinService::Instance()->GetDrawMgr().GetMenuBorder(),
		CRXSkinService::Instance()->GetDrawMgr().GetMenuBk(),
		hRgn);
	RECT rcSidebar = rcClient;
	rcSidebar.right = rcSidebar.left + CRXSkinService::Instance()->GetDrawMgr().GetMenuSidebarWidth();
	rcSidebar.left += 2;
	rcSidebar.top += 2;
	rcSidebar.bottom -= 4;	
	CreateRoundRectRgn(hRgn,rcSidebar,szRound.cx,szRound.cy,TRUE,TRUE,FALSE,FALSE);
	DrawRgn(memDC.GetHDC(),
		CRXSkinService::Instance()->GetDrawMgr().GetMenuSidebarBk(),
		hRgn);
	DeleteObject(hRgn);
	CRXSkinRect rcItem = rcClient;
	rcItem.top += CRXSkinService::Instance()->GetDrawMgr().GetMenuItemOffSetY();
	CRXSkinMenuItemVectItr itr = m_vecItems.begin();
	for (;itr!=m_vecItems.end();itr++)
	{
		rcItem.bottom = rcItem.top + (*itr)->GetHeight();
		(*itr)->OnDraw(memDC.GetSurface(),memDC.GetHDC(),rcItem);
		rcItem.top = rcItem.bottom;
	}
	memDC.TakeSnapshot();
	::EndPaint(m_hWnd, &ps);
}

VOID  CRXSkinMenu::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{

}

BOOL CRXSkinMenu::OnSize(WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinWnd::OnSize(wParam, lParam);
	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	SIZE szRound = CRXSkinService::Instance()->GetDrawMgr().GetMenuRound();
	if (szRound.cx>0&&szRound.cy>0)
	{
		HRGN hRgn = ::CreateRoundRectRgn(rcClient.left,rcClient.top,
			rcClient.right,rcClient.bottom,szRound.cx,szRound.cy);
		::SetWindowRgn(m_hWnd,hRgn,TRUE);
		DeleteObject(hRgn);
	}
	return bResult;
}

LRESULT CRXSkinMenu::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	if (m_poMouseOn)
	{
		m_poMouseOn->SetState(ITEM_NORMAL);
		RECT rc = m_poMouseOn->GetRect();
		InvalidateRect(m_hWnd,&(rc),FALSE);
		m_poMouseOn = NULL;
	}
	CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);

	return CRXSkinWnd::OnMouseLeave(wParam,lParam);
}

BOOL CRXSkinMenu::OnLButtonUp(UINT nFlags, POINT point)
{
	HideMenu();
	ReleaseCapture();
	if (m_poParentMenu)
	{
		m_poParentMenu->SetCapture();
	}
	CRXSkinMenuItem* poControl = _HitTest(point);
	if (poControl)
	{
		::SendMessage(GetParentWnd(),WM_COMMAND,poControl->GetID(),NULL);
		if (m_poParentMenu)
		{
			m_poParentMenu->ReleaseCapture();
			m_poParentMenu->HideMenu();
		}
	}

	return CRXSkinWnd::OnLButtonUp(nFlags,point);
}

LRESULT CRXSkinMenu::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = CRXSkinWnd::OnKillFocus(wParam, lParam);
	//HideMenu();
	return lResult;
}

BOOL CRXSkinMenu::OnLButtonDown(UINT nFlags, POINT point)
{
	CRXSkinMenuItem* poControl = _HitTest(point);
	if (poControl == NULL)
	{
		if (m_poPressed)
		{
			m_poPressed->SetState(ITEM_NORMAL);
			RECT rc = m_poPressed->GetRect();
			InvalidateRect(m_hWnd, &(rc), FALSE);
			m_poPressed = poControl;
		}
	}
	else 
	{
		if (NULL == m_poPressed)
		{
			m_poPressed = poControl;
			m_poPressed->SetState(ITEM_PRESSED);
			RECT rc = m_poPressed->GetRect();
			InvalidateRect(m_hWnd, &(rc), FALSE);
		}
		else if (m_poPressed!=poControl)
		{
			m_poPressed->SetState(ITEM_NORMAL);
			RECT rc = m_poPressed->GetRect();
			InvalidateRect(m_hWnd, &(rc), FALSE);
			m_poPressed = poControl;
			m_poPressed->SetState(ITEM_PRESSED);
			rc = m_poPressed->GetRect();
			InvalidateRect(m_hWnd, &(rc), FALSE);
		}
	}
	return CRXSkinWnd::OnLButtonDown(nFlags,point);
}


BOOL CRXSkinMenu::OnMouseMove(UINT nFlags, POINT point)
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
	CRXSkinMenuItem* poControl = _HitTest(point);
	if (poControl)
	{
		if (NULL == m_poMouseOn)
		{
			m_poMouseOn = poControl;
			m_poMouseOn->SetState(ITEM_MOUSEON);
			RECT rc = m_poMouseOn->GetRect();
			InvalidateRect(m_hWnd, &(rc), FALSE);
	
		}
		else if (poControl != m_poMouseOn)
		{
			m_poMouseOn->SetState(ITEM_NORMAL);
			RECT rc = m_poMouseOn->GetRect();
			InvalidateRect(m_hWnd, &(rc), FALSE);
			if (m_poMouseOn->GetChildMenu())
			{
				m_poMouseOn->GetChildMenu()->HideMenu();
			}
			m_poMouseOn = poControl;
			m_poMouseOn->SetState(ITEM_MOUSEON);
			rc = m_poMouseOn->GetRect();
			InvalidateRect(m_hWnd, &(rc), FALSE);
		}
		if (m_poMouseOn)
		{
			if (m_poMouseOn->GetChildMenu())
			{
				RECT rc = m_poMouseOn->GetRect();
				POINT pt;
				pt.x = rc.right;
				pt.y = rc.top;
				ClientToScreen(GetHWnd(), &pt);
				m_poMouseOn->GetChildMenu()->ShowMenu(pt, GetParentWnd());
			}
		}
		CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_HAND);

	}
	else
	{
		if (m_poMouseOn)
		{
			m_poMouseOn->SetState(ITEM_NORMAL);
			RECT rc = m_poMouseOn->GetRect();
			InvalidateRect(m_hWnd, &(rc), FALSE);
			m_poMouseOn = poControl;
		}
		CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);

	}
	return TRUE;
}

CRXSkinMenu::CRXSkinMenuItemMap& CRXSkinMenu::GetItems()
{
	return m_mapItems;
}

UINT32 CRXSkinMenu::_GetWidth()
{
	UINT32 dwWidth = 0;
	CRXSkinMenuItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		UINT32 dwItemWidth = itr->second->GetWidth();
		if (dwItemWidth>dwWidth)
		{
			dwWidth = dwItemWidth;
		}
	}
	return dwWidth;
}

UINT32 CRXSkinMenu::_GetHeight()
{
	UINT32 dwHeight = 2*CRXSkinService::Instance()->GetDrawMgr().GetMenuItemOffSetY();
	CRXSkinMenuItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		dwHeight += itr->second->GetHeight();
	}
	return dwHeight;
}

BOOL CRXSkinMenu::ShowMenu(POINT pt, const HWND& hFocusWnd)
{
	if (m_hWnd == NULL)
	{
		Create(WND_RXSKIN_MENU_CLASS, _T(""), WS_POPUP, hFocusWnd, NULL);
	}
	UINT32 dwHeight = _GetHeight();
	UINT32 dwWidth = _GetWidth();
	UINT32 dwScreenX = GetSystemMetrics(SM_CXSCREEN);
	UINT32 dwScreenY = GetSystemMetrics(SM_CYSCREEN);
	UINT32 dwLeft = pt.x;
	INT32 dwTop  = pt.y;
	if (dwLeft+dwWidth>dwScreenX)
	{
		dwLeft = dwScreenX - dwWidth;
	}
	if (dwTop+dwHeight>dwScreenY)
	{
		dwTop = (dwScreenY - dwHeight)/2;
	}
	
	::MoveWindow(m_hWnd,dwLeft,dwTop,dwWidth,dwHeight,FALSE);
	ShowWindow(m_hWnd,SW_SHOW);
	m_hParentWnd = hFocusWnd;
	SetWindowPos(m_hWnd, HWND_TOPMOST,dwLeft,dwTop,dwWidth,dwHeight, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	//
	SetCapture();
	if (hFocusWnd)
		SetFocus(hFocusWnd);
 	return TRUE;
}

VOID CRXSkinMenu::HideMenu()
{
	ShowWindow(m_hWnd,SW_HIDE);
}

CRXSkinMenuItem* CRXSkinMenu::_HitTest(POINT pt)
{
	CRXSkinMenuItem* poItem = NULL;
	CRXSkinMenuItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		if (itr->second->GetRect().PtInRect(pt))
		{
			poItem = itr->second;
			break;
		}
	}
	return poItem;
}

void CRXSkinMenu::SetItemChecked(const UINT32& dwID, const bool& bChecked)
{
	CRXSkinMenuItem* poItem = FindMenuItem(dwID);
	if (poItem)
	{
		poItem->SetChecked(bChecked);
		poItem->Invalidate();
	}
}