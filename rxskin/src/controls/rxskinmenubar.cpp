#include "pch.h"
#include "controls\rxskinmenubar.h"

#include "common\RXSkinService.h"
#include "common\graphics.h"
#include "common\RXSkinMemDC.h"
#include "common\rxskinutil.h"


CRXSkinMenubarItem::CRXSkinMenubarItem()
{

}

CRXSkinMenubarItem::~CRXSkinMenubarItem()
{

}

CRXSkinItem* CRXSkinMenubarItem::HitTest(POINT pt)
{
	if (IsEnabled())
	{
		if (GetRect().PtInRect(pt))
		{
			return this;
		}		
	}
	return NULL;
}

void CRXSkinMenubarItem::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	COLORREF clrTxt = m_poMenubar->GetItemNormalTextColor();
	//  Drawback
	if (GetState() != ITEM_NORMAL)
	{
		COLORREF clrBk = RGB(255, 255, 255);
		if (GetState() == ITEM_MOUSEON)
		{
			clrBk = m_poMenubar->GetItemMouseOnBkColor();
			clrTxt = m_poMenubar->GetItemMouseOnTextColor();
		}
		else if (GetState() == ITEM_PRESSED)
		{
			clrBk = m_poMenubar->GetItemPressedBkColor();
			clrTxt = m_poMenubar->GetItemPressedTextColor();
		}

		DrawRoundRect(hDC,
			CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonBorderColor(),
			clrBk, 
			rcItem,
			CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cy);
	}	
	INT32 nModel = SetBkMode(hDC, TRANSPARENT);
	HFONT hOldFont = (HFONT)::SelectObject(hDC,
		CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
	COLORREF oldClr = ::SetTextColor(hDC, clrTxt);
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas,
			m_strName.GetData(), rcItem,
			CRXSkinService::Instance()->GetDrawMgr().GetMenuItemPressedText(),
			ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	}
	::SelectObject(hDC, hOldFont);
	::SetBkMode(hDC, nModel);
	::SetTextColor(hDC, oldClr);
}

CRXSkinMenuItem*  CRXSkinMenubarItem::AppendMenuEx(EMenuItemType enType,
	const UINT32 dwID,
	const TCHAR* pszName /* = _T("") */, 
	const UINT32 dwImageID /* = 0 */)
{
	return m_oMenu.AppendMenuEx(enType, dwID, pszName, dwImageID);
}

CRXSkinMenuItem* CRXSkinMenubarItem::AppendMenu(EMenuItemType enType,
	const UINT32 dwID,
	const TCHAR* pszName /* = _T("") */,
	const TCHAR* pszImageFile /* = _T("") */)
{
	return m_oMenu.AppendMenu(enType, dwID, pszName, pszImageFile);
}

void CRXSkinMenubarItem::PopupMenu()
{
	if (0==m_oMenu.GetItems().size())
	{
		return;
	}
	POINT pt;
	pt.x = GetRect().left;
	pt.y = GetRect().bottom;
	::ClientToScreen(GetParentWnd(), &pt);
	m_oMenu.ShowMenu(pt, GetParentWnd());
}

void CRXSkinMenubarItem::SetChecked(const UINT32& dwID, const bool& bChecked)
{
	m_oMenu.SetItemChecked(dwID, bChecked);
}

CRXSkinMenubar::CRXSkinMenubar()
{
	m_poPressed = NULL;
	m_poMouseOn = NULL;
	m_clrFrom = RGB(255,255,255);
	m_clrMiddle = RGB(255, 255, 255);
	m_clrTo = RGB(255, 255, 255);
	m_clrItemPressedBk = RGB(255, 255, 255);
	m_clrItemMouseOnBk = RGB(255, 255, 255);
	m_clrItemPressedTxt = RGB(255, 255, 255);
	m_clrItemMouseOnTxt = RGB(255, 255, 255);
	m_clrItemNormalTxt = RGB(255, 255, 255);
}

CRXSkinMenubar::~CRXSkinMenubar()
{
	_DeleteAll();
}

BOOL CRXSkinMenubar::Create(const UINT32 dwID, CRXSkinRect rcClient)
{
	SetID(dwID);
	SetRect(rcClient);
	return TRUE;
}


VOID CRXSkinMenubar::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsEnabled()!=TRUE)
	{
		return;
	}
	if (false == IsVisible())
	{
		return;
	}
	SetRect(rcItem);
	CRXSkinRect rcTop = rcItem;
	rcTop.bottom = rcTop.top + rcItem.Height() / 2;
	DrawGradientRect(hDC, TRUE, rcTop, m_clrFrom, m_clrMiddle);
	CRXSkinRect rcBottom = rcTop;
	rcBottom.top = rcTop.bottom;
	rcBottom.bottom = rcItem.bottom;
	DrawGradientRect(hDC, TRUE, rcBottom, m_clrMiddle, m_clrTo);

	for (UINT32 m = 0; m < (UINT32)m_vecItem.size(); m++)
	{
		m_vecItem[m]->OnDraw(poSurface,hDC,m_vecItem[m]->GetRect());
	}
}
VOID CRXSkinMenubar::ReleaseControl()
{
	delete this;
}
BOOL CRXSkinMenubar::PtInRect(POINT pt)
{
	return _HitTest(pt)?true:false;
}

BOOL CRXSkinMenubar::OnClick(POINT pt)
{
	CRXSkinMenubarItem* poItem = _HitTest(pt);
	if (poItem)
	{
		poItem->PopupMenu();
		return TRUE;
	}
	return FALSE;
}

BOOL CRXSkinMenubar::OnMouseMove(POINT pt)
{
	CRXSkinMenubarItem* poItem = _HitTest(pt);
	if (poItem)
	{
		if (NULL == m_poMouseOn)
		{
			m_poMouseOn = poItem;
			m_poMouseOn->SetState(ITEM_MOUSEON);
			m_poMouseOn->Invalidate();
		}
		else {
			if (m_poMouseOn!=poItem)
			{
				m_poMouseOn->SetState(ITEM_NORMAL);
				m_poMouseOn->Invalidate();
				m_poMouseOn = poItem;
				m_poMouseOn->SetState(ITEM_MOUSEON);
				m_poMouseOn->Invalidate();
			}
		}
		return TRUE;
	}
	else
	{
		if (m_poMouseOn)
		{
			m_poMouseOn->SetState(ITEM_NORMAL);
			m_poMouseOn->Invalidate();
			m_poMouseOn = NULL;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CRXSkinMenubar::OnLButtonDown(POINT pt)
{
	CRXSkinMenubarItem* poItem = _HitTest(pt);
	if (poItem)
	{
		if (NULL == m_poPressed)
		{
			m_poPressed = poItem;
			m_poPressed->SetState(ITEM_PRESSED);
			m_poPressed->Invalidate();
		}
		else {
			if (m_poPressed != poItem)
			{
				m_poPressed->SetState(ITEM_NORMAL);
				m_poPressed->Invalidate();
				m_poPressed = poItem;
				m_poPressed->SetState(ITEM_PRESSED);
				m_poPressed->Invalidate();
			}
		}
		return TRUE;
	}
	else
	{
		if (m_poPressed)
		{
			m_poPressed->SetState(ITEM_NORMAL);
			m_poPressed->Invalidate();
			m_poPressed = NULL;
		}
		return FALSE;
	}
	return FALSE;	
}

BOOL CRXSkinMenubar::OnLButtonUp(POINT pt) 
{
	CRXSkinMenubarItem* poItem = _HitTest(pt);
	if (poItem)
	{
		poItem->PopupMenu();
		return TRUE;
	}
	return FALSE; 
}

BOOL CRXSkinMenubar::OnRButtonDown(POINT pt) 
{
	return FALSE; 
}

BOOL CRXSkinMenubar::OnRButtonUp(POINT pt)
{
	return FALSE; 
}

BOOL CRXSkinMenubar::OnRButtonDblClk(POINT pt) 
{
	return FALSE; 
}

BOOL CRXSkinMenubar::OnSize(const UINT32 dwX, const UINT32 dwY)
{
	CRXSkinRect rc = GetRect();
	rc.left = CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
	rc.right = dwX - CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
	SetRect(rc);
	Invalidate();
	return CRXControl::OnSize(dwX, dwY);
}
VOID CRXSkinMenubar::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
}

VOID CRXSkinMenubar::Enable(const BOOL bEnabled)
{
	CRXControl::Enable(bEnabled);
}

UINT32  CRXSkinMenubar::GetCount()
{
	return (UINT32)m_vecItem.size();
}

void CRXSkinMenubar::_DeleteAll()
{
	for (UINT32 m = 0;m <(UINT32)m_vecItem.size();m ++)
	{
		delete m_vecItem[m];
	}
	m_vecItem.clear();
}

CRXSkinMenubarItem* CRXSkinMenubar::_HitTest(POINT pt)
{
	for (UINT32 m = 0; m < (UINT32)m_vecItem.size(); m++)
	{
		if (m_vecItem[m]->HitTest(pt))
			return m_vecItem[m];
	}
	return NULL;
}

CRXSkinMenubarItem* CRXSkinMenubar::AddSubMenu(const UINT32& dwID, const  TCHAR* pszName, CRXSkinRect rcItem)
{
	CRXSkinMenubarItem* poItem = new CRXSkinMenubarItem;
	poItem->SetMenubar(this);
	poItem->SetID(dwID);
	poItem->SetName(pszName);
	poItem->SetParentWnd(GetParentWnd());
	poItem->SetRect(rcItem);
	poItem->GetMenu().Create(WND_RXSKIN_MENU_CLASS, _T(""), WS_POPUP, GetParentWnd(), NULL);
	m_vecItem.push_back(poItem);
	return poItem;
}

void CRXSkinMenubar::SetChecked(const UINT32& dwID, const bool& bChecked)
{
	for (UINT32 m = 0; m < (UINT32)m_vecItem.size(); m++)
	{
		m_vecItem[m]->SetChecked(dwID, bChecked);			
	}
}