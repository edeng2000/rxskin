#include "pch.h"
#include "common/rximage.h"
#include "controls\RXSkinTabCtrl.h"
#include "common/graphics.h"
#include "common/RXSkinService.h"
#include "common/rxskindefines.h"

#define CLOSE_IMAGE_WIDTH         16
#define CLOSE_IMAGE_HEIGHT        16

#define DEFAULT_CLOSE_ID          980001

CRXSkinTabItem::CRXSkinTabItem()
{
	m_poImage = NULL;
	m_poTabCtrl = NULL;
	m_poChild = NULL;
	memset(m_szToolTip,0,sizeof(TCHAR)*256);
	memset(m_szName,0,sizeof(TCHAR)*256);
	m_poChildWnd = NULL;
	m_bSelectClose = FALSE;
	m_poMouseOn = NULL;
	m_poPressed = NULL;
}

CRXSkinTabItem::~CRXSkinTabItem()
{
	
}

VOID CRXSkinTabItem::SetImage(const TCHAR* pszImage)
{
	m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenImage(pszImage);
}

VOID CRXSkinTabItem::SetTabCtrl(CRXSkinTabCtrl* poTabCtrl)
{
	m_poTabCtrl = poTabCtrl;
}

void CRXSkinTabItem::OnClicked(POINT pt)
{
	CRXControl* poCurControl = HitTestEx(pt);
	if (poCurControl)
	{
		if (m_poPressed && m_poPressed != poCurControl)
		{
			m_poPressed->SetState(CONTROL_STATE_NORMAL);
			m_poPressed->Invalidate();
		}
		m_poPressed = poCurControl;
		m_poPressed->SetState(CONTROL_STATE_PRESSED);
		m_poPressed->Invalidate();

		if (DEFAULT_CLOSE_ID == poCurControl->GetID())
		{
			if (m_poTabCtrl&& m_poTabCtrl->GetListener())
			{
				m_poTabCtrl->GetListener()->OnTabItemClosed(this);
			}
		}
		else
		{
			if (m_poTabCtrl && m_poTabCtrl->GetListener())
			{
				m_poTabCtrl->GetListener()->OnTabItemClicked(this,poCurControl->GetID());
			}
		}

	}
	else
	{
		if (m_poPressed)
		{
			m_poPressed->SetState(CONTROL_STATE_NORMAL);
			m_poPressed->Invalidate();
			m_poPressed = NULL;
		}
	}
}

void CRXSkinTabItem::OnMouseLeave()
{
	m_oControlMgr.OnMouseLeave();
}

CRXControl* CRXSkinTabItem::OnMouseOn(POINT pt)
{
	CRXControl* poCurControl = HitTestEx(pt);
	if (poCurControl)
	{
		if (m_poMouseOn && m_poMouseOn!=poCurControl)
		{
			m_poMouseOn->SetState(CONTROL_STATE_NORMAL);
			m_poMouseOn->Invalidate();
		}
		m_poMouseOn = poCurControl;
		m_poMouseOn->SetState(CONTROL_STATE_MOUSEON);
		m_poMouseOn->Invalidate();
	}
	else
	{
		if (m_poMouseOn)
		{
			m_poMouseOn->SetState(CONTROL_STATE_NORMAL);
			m_poMouseOn->Invalidate();
			m_poMouseOn = NULL;
		}
	}
	return poCurControl;
}


SIZE CRXSkinTabItem::GetItemSize()
{
	SIZE sz;
	sz.cx = sz.cy = 0;
	SIZE szLocal;
	tstring strTxt = m_szName;
	CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON_BOLD, strTxt, szLocal);
	INT32 nWidth = CRXSkinService::Instance()->GetDrawMgr().GetTabItemOffSetX();
	INT32 nHeight = CRXSkinService::Instance()->GetDrawMgr().GetTabItemOffSetX() * 2 + szLocal.cy;
	if (m_poImage)
	{
		nWidth += m_poImage->GetWidth();
	}
	nWidth += CRXSkinService::Instance()->GetDrawMgr().GetTabItemOffSetX();
	nWidth += szLocal.cx;
	nWidth += CRXSkinService::Instance()->GetDrawMgr().GetTabItemOffSetX();
	sz.cx = nWidth;
	sz.cy = nHeight;
	int nLeft = 0;
	CRXControlMgr::CRXControlMap& oMap = m_oControlMgr.GetAllControl();
	for (CRXControlMgr::CRXControlMapItr itr = oMap.begin();itr!=oMap.end();itr++)
	{
		nWidth += itr->second->GetRect().Width();
		nWidth += 2 * CRXSkinService::Instance()->GetDrawMgr().GetTabItemOffSetX();
	}

	sz.cx = nWidth;
	return sz;
}

VOID CRXSkinTabItem::SetTooltip(const TCHAR* pszTooltip)
{
	memset(m_szToolTip,0,sizeof(TCHAR)*256);
	_tcsncpy(m_szToolTip,pszTooltip,256);
}

CRXSkinItem* CRXSkinTabItem::HitTest(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return this;
	}
	return NULL;
}

TCHAR* CRXSkinTabItem::GetToolTip()
{
	return m_szToolTip;
}

TCHAR* CRXSkinTabItem::GetName()
{
	return m_szName;
}

VOID CRXSkinTabItem::SetName(const TCHAR* pszName)
{
	memset(m_szName,0,sizeof(TCHAR)*256);
	_tcsncpy(m_szName,pszName,256);
}

VOID CRXSkinTabItem::SetChild(CRXControl* poChild)
{
	m_poChild = poChild;
}

CRXControl* CRXSkinTabItem::GetChild()
{
	return m_poChild;
}
VOID CRXSkinTabItem::SetChildWnd(CRXSkinWnd* poChild)
{
	m_poChildWnd = poChild;
}

CRXSkinWnd* CRXSkinTabItem::GetChildWnd()
{
	return m_poChildWnd;
}

void CRXSkinTabItem::AddControl(CRXControl* poControl)
{
	CRXControl* poFind = FindControl((uint32_t)poControl->GetID());
	if (poFind)
	{
		return;
	}
	m_oControlMgr.AddControl(poControl);
}

CRXControl* CRXSkinTabItem::FindControl(const uint32_t& dwID)
{
	return m_oControlMgr.FindControl(dwID);
}

CRXControl* CRXSkinTabItem::HitTestEx(POINT pt)
{
	CRXControl* poControl = m_oControlMgr.HitTest(pt);
	return poControl;
}

VOID CRXSkinTabItem::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (NULL == m_poTabCtrl)
	{
		return;
	}
	SetRect(rcItem);
	if (ITEM_SELECTED == GetState())
	{
		HRGN hRect  = ::CreateRectRgn( rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height() );
		CreateRoundRectRgn(hRect,
			rcItem,
			m_poTabCtrl->GetTabCtrlRound().cx,
			m_poTabCtrl->GetTabCtrlRound().cy,
			TRUE,
			FALSE,
			TRUE,
			FALSE);
		DrawRgn(hDC,
			CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor(),
			CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(),
			hRect);
		rcItem.top += 2;
		rcItem.left += 2;
		rcItem.right -= 2;

		CreateRoundRectRgn(hRect,
			rcItem,
			m_poTabCtrl->GetTabCtrlRound().cx,
			m_poTabCtrl->GetTabCtrlRound().cy,
			TRUE,
			FALSE,
			TRUE,
			FALSE);
		DrawFrameRgn(hDC, hRect,
			m_poTabCtrl->GetTabItemBorder(),
			m_poTabCtrl->GetTabCtrlRound().cx,
			m_poTabCtrl->GetTabCtrlRound().cy);

		DeleteObject(hRect);
		rcItem.top += 2;
		rcItem.left += 2;
		rcItem.right -= 2;
		hRect = ::CreateRectRgn(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height());
		CreateRoundRectRgn(hRect,
			rcItem,
			m_poTabCtrl->GetTabCtrlRound().cx,
			m_poTabCtrl->GetTabCtrlRound().cy,
			TRUE,
			FALSE,
			TRUE,
			FALSE);

		DrawGradientRgn(hDC,
			TRUE,
			hRect,
			m_poTabCtrl->GetTabItemSelectedFromBk(),
			m_poTabCtrl->GetTabItemSelectedToBk());
		DeleteObject(hRect);
		SetPixel(hDC,GetRect().left+1,GetRect().bottom-1,CRXSkinService::Instance()->GetDrawMgr().GetCommonBk());
		SetPixel(hDC,GetRect().right-2,GetRect().bottom-1,CRXSkinService::Instance()->GetDrawMgr().GetCommonBk());
	}
	else
	{
		switch(GetState())
		{
		case ITEM_MOUSEON:
			{
			rcItem.top += 2;
			rcItem.left += 2;
			rcItem.right -= 2;
			HRGN hRect = ::CreateRectRgn(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height());
			CreateRoundRectRgn(hRect,
				rcItem,
				m_poTabCtrl->GetTabCtrlRound().cx,
				m_poTabCtrl->GetTabCtrlRound().cy,
				TRUE,
				FALSE,
				TRUE,
				FALSE);
			DrawFrameRgn(hDC, hRect,
				m_poTabCtrl->GetTabItemBorder(),
				m_poTabCtrl->GetTabCtrlRound().cx,
				m_poTabCtrl->GetTabCtrlRound().cy);

			DeleteObject(hRect);
			rcItem.top += 2;
			rcItem.left += 2;
			rcItem.right -= 2;
			hRect = ::CreateRectRgn(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height());
			CreateRoundRectRgn(hRect,
				rcItem,
				m_poTabCtrl->GetTabCtrlRound().cx,
				m_poTabCtrl->GetTabCtrlRound().cy,
				TRUE,
				FALSE,
				TRUE,
				FALSE);

			CreateRoundRectRgn(hRect,
				rcItem,
				m_poTabCtrl->GetTabCtrlRound().cx,
				m_poTabCtrl->GetTabCtrlRound().cy,
				TRUE,
				FALSE,
				TRUE,
				FALSE);
			DrawGradientRgn(hDC,
				TRUE,
				hRect,
				m_poTabCtrl->GetTabItemMouseOnFromBk(),
				m_poTabCtrl->GetTabItemMouseOnToBk());
			DeleteObject(hRect);
			}
			break;
		case  ITEM_NORMAL:
		{
			rcItem.top += 2;
			rcItem.left += 2;
			rcItem.right -= 2;
			HRGN hRect = ::CreateRectRgn(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height());
			CreateRoundRectRgn(hRect,
				rcItem,
				m_poTabCtrl->GetTabCtrlRound().cx,
				m_poTabCtrl->GetTabCtrlRound().cy,
				TRUE,
				FALSE,
				TRUE,
				FALSE);
			DrawFrameRgn(hDC, hRect,
				m_poTabCtrl->GetTabItemBorder(),
				m_poTabCtrl->GetTabCtrlRound().cx,
				m_poTabCtrl->GetTabCtrlRound().cy);
			DeleteObject(hRect);
			rcItem.top += 2;
			rcItem.left += 2;
			rcItem.right -= 2;
			hRect = ::CreateRectRgn(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height());
			CreateRoundRectRgn(hRect,
				rcItem,
				m_poTabCtrl->GetTabCtrlRound().cx,
				m_poTabCtrl->GetTabCtrlRound().cy,
				TRUE,
				FALSE,
				TRUE,
				FALSE);

			DrawGradientRgn(hDC,
				TRUE,
				hRect,
				m_poTabCtrl->GetTabItemNormalFromBk(),
				m_poTabCtrl->GetTabItemNormalToBk());

			DeleteObject(hRect);
		}
		break;
		default:
			break;
		}
	}

	SIZE szTxt;
	tstring strTxt = m_szName;
	CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, strTxt, szTxt);
	INT32 nLeft = GetRect().left;
	if (m_poImage)
	{
		INT32 nImageWidth = m_poImage->GetWidth();
		INT32 nImageHeight = m_poImage->GetHeight();
		nLeft += m_poTabCtrl->GetTabItemOffSetX();
	
 		INT32 nTop = ( GetRect().Height() - nImageHeight ) / 2 + GetRect().top;
		m_poImage->Draw(hDC,nLeft,nTop,nImageWidth,nImageHeight);
		nLeft += nImageWidth;
		nLeft += m_poTabCtrl->GetTabItemOffSetX();
	}
	else
	{
		nLeft = (rcItem.Width() - szTxt.cx)/2 + rcItem.left;
	}
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		if (ITEM_SELECTED == GetState())
		{
			CRXSkinFontMgr::Instance()->TextOut(poCanvas, nLeft,
				GetRect().top + (rcItem.Height() - szTxt.cy) / 2,
				strTxt, CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalTxt(),
				ID_FONT_COMMON_BOLD, DT_LEFT | DT_VCENTER);
		}
		else
		{
			CRXSkinFontMgr::Instance()->TextOut(poCanvas, nLeft,
				GetRect().top + (rcItem.Height() - szTxt.cy) / 2,
				strTxt, CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalTxt(),
				ID_FONT_COMMON, DT_LEFT | DT_VCENTER);
		}			
	}

	m_oControlMgr.OnDraw(poSurface, hDC);
}

VOID CRXSkinTabItem::SetRect(CRXSkinRect rcItem)
{
	CRXSkinItem::SetRect(rcItem);
	RECT rcButton;
	rcButton.left = rcItem.left;
	rcButton.left += m_poTabCtrl->GetTabItemOffSetX();
	if (m_poImage)
	{
		rcButton.left += m_poImage->GetWidth();
	}
	rcButton.left += m_poTabCtrl->GetTabItemOffSetX();
	SIZE sz;
	int32 nWidth = 0;
	CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON_BOLD,
		m_szName, sz);
	nWidth = sz.cx;
	rcButton.left += nWidth;
	rcButton.left += 2*m_poTabCtrl->GetTabItemOffSetX();
	CRXControlMgr::CRXControlMap& oMap = m_oControlMgr.GetAllControl();
	for (CRXControlMgr::CRXControlMapItr itr = oMap.begin(); itr != oMap.end(); itr++)
	{
		CRXSkinRect rc = GetRect();
		rc.left = rcButton.left;
		rc.right = rc.left + itr->second->GetRect().Width();
		rc.top = rc.top + (rc.Height() - itr->second->GetRect().Height()) / 2;
		rc.bottom = rc.top + itr->second->GetRect().Height();
		itr->second->SetRect(rc);
		rcButton.left = rc.right;
		rcButton.left += m_poTabCtrl->GetTabItemOffSetX();
	}
}

void  CRXSkinTabItem::AddTabButton(const uint32_t& dwCommandID,
	const TCHAR* pszToolTip, const TCHAR* pszNormal, const TCHAR* pszHot, const TCHAR* pszDown, const RECT rc)
{
	CRXImageButton* poBtn = new CRXImageButton;
	poBtn->SetID(dwCommandID);
	poBtn->SetAutoRecycle(false);
	poBtn->SetDownImage(pszDown);
	poBtn->SetHotImage(pszHot);
	poBtn->SetNormalImage(pszNormal);
	poBtn->SetNormalBK(RGB(0, 0, 0));
	poBtn->SetTooltip(pszToolTip);
	poBtn->SetParentWnd(GetParentWnd());
	poBtn->SetRect(rc);
	poBtn->SetLastRect(rc);
	AddControl(poBtn);
}



void CRXSkinTabItem::Init()
{
	CRXImageButton* poBtnClose = new CRXImageButton;
	poBtnClose->SetID(DEFAULT_CLOSE_ID);
	poBtnClose->SetAutoRecycle(false);
	poBtnClose->SetDownImage(CRXSkinService::Instance()->GetImageMgr().FindImage(ID_IMAGE_CLOSE_HOT));
	poBtnClose->SetHotImage(CRXSkinService::Instance()->GetImageMgr().FindImage(ID_IMAGE_CLOSE_HOT));
	poBtnClose->SetNormalImage(CRXSkinService::Instance()->GetImageMgr().FindImage(ID_IMAGE_CLOSE_NORMAL));
	poBtnClose->SetNormalBK(RGB(201, 212, 215));
	CRXSkinRect rc = GetRect();
	rc.left = 32;
	rc.right = rc.left + 20;
	rc.top = 0;
	rc.bottom = 20;
	poBtnClose->SetTooltip(_T("¹Ø±Õ"));
	poBtnClose->SetParentWnd(GetParentWnd());
	poBtnClose->SetRect(rc);
	poBtnClose->SetLastRect(rc);
	AddControl(poBtnClose);
}

void CRXSkinTabItem::SetSelectClose(const BOOL& bSelected)
{
	if (m_bSelectClose!=bSelected)
	{
		m_bSelectClose = bSelected;
		Invalidate();
	}
}

CRXSkinRect CRXSkinTabCtrl::GetInvalidateRect()
{
	return m_rcInvalidate;
}

void  CRXSkinTabCtrl::AddTabButton(const char* pszKey,  const uint32_t& dwCommandID,
	const TCHAR* pszToolTip, const TCHAR* pszNormal, const TCHAR* pszHot, const TCHAR* pszDown, const RECT rc)
{
	CRXSkinTabItem* poItem = FindTabByKey(pszKey);
	if (poItem)
	{
		poItem->AddTabButton(dwCommandID, pszToolTip, pszNormal, pszHot, pszDown,rc);
	}
}


CRXSkinTabCtrl::CRXSkinTabCtrl(void)
{
	m_clrTabItemNormalFromBk = CRXSkinService::Instance()->GetDrawMgr().GetTabItemNormalFromBk();
	m_clrTabItemNormalToBk = CRXSkinService::Instance()->GetDrawMgr().GetTabItemNormalToBk();
	m_clrTabItemNormalText = CRXSkinService::Instance()->GetDrawMgr().GetTabItemNormalText();

	m_clrTabItemMouseOnFromBk = CRXSkinService::Instance()->GetDrawMgr().GetTabItemMouseOnFromBk();
	m_clrTabItemMouseOnToBk = CRXSkinService::Instance()->GetDrawMgr().GetTabItemMouseOnToBk();
	m_clrTabItemMouseOnText = CRXSkinService::Instance()->GetDrawMgr().GetTabItemMouseOnText();

	m_clrTabItemPressedFromBk = CRXSkinService::Instance()->GetDrawMgr().GetTabItemPressedFromBk();
	m_clrTabItemPressedToBk = CRXSkinService::Instance()->GetDrawMgr().GetTabItemPressedToBk();
	m_clrTabItemPressedText = CRXSkinService::Instance()->GetDrawMgr().GetTabItemPressedText();

	m_clrTabItemSelectedFromBk = CRXSkinService::Instance()->GetDrawMgr().GetTabItemSelectedFromBk();
	m_clrTabItemSelectedToBk = CRXSkinService::Instance()->GetDrawMgr().GetTabItemSelectedToBk();
	m_clrTabItemSelectedText = CRXSkinService::Instance()->GetDrawMgr().GetTabItemSelectedText();
	m_clrTabItemBorder = CRXSkinService::Instance()->GetDrawMgr().GetTabItemBorder();

	m_dwTabItemHeight = CRXSkinService::Instance()->GetDrawMgr().GetTabCtrlItemHeight();
	m_szTabCtrlRound = CRXSkinService::Instance()->GetDrawMgr().GetTabCtrlRound();
	m_dwTabItemOffSetX = CRXSkinService::Instance()->GetDrawMgr().GetTabItemOffSetX();
	m_poSelected = NULL;
	m_poListener = NULL;
	m_poMouseOn = NULL;
	m_poPressed = NULL;
	m_dwCurID = 0;
}

CRXSkinTabCtrl::~CRXSkinTabCtrl(void)
{
	_RemoveAll();
}

VOID CRXSkinTabCtrl::OnMouseLeave()
{
	if (m_poMouseOn)
	{
		if (m_poMouseOn)
		{
			if (m_poMouseOn->GetState()!=ITEM_SELECTED)
			{
				m_poMouseOn->SetState(ITEM_NORMAL);
			}
			m_poMouseOn = NULL;
		}
	}
	if (m_poListener)
	{
		m_poListener->OnTabItemMouseLeave();
	}

	CCRXSkinTabItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		itr->second->OnMouseLeave();
	}
}


VOID CRXSkinTabCtrl::ReleaseControl()
{
	delete this;
}

VOID CRXSkinTabCtrl::SetListener(IWndTabCtrlListener* poListener)
{
	m_poListener = poListener;
}

VOID CRXSkinTabCtrl::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (m_mapItems.size()>0)
	{
		CRXSkinRect rcDraw = rcItem;
		rcDraw.bottom = rcDraw.top + m_dwTabItemHeight;
		CCRXSkinTabItemMapItr itr = m_mapItems.begin();
		for (;itr!=m_mapItems.end();itr++)
		{
			SIZE szItem = itr->second->GetItemSize();
			rcDraw.right = rcDraw.left + szItem.cx;
			itr->second->OnDraw(poSurface,hDC,rcDraw);
			rcDraw.left = rcDraw.right;
		}
	}

	if (IsBorder())
	{
		HPEN	hpen;
		HGDIOBJ hpenOld;
		CRXSkinRect rcBorder = rcItem;
		rcBorder.top = rcBorder.top + m_dwTabItemHeight;
		hpen = ::CreatePen(PS_SOLID, 1, CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor());
		hpenOld = ::SelectObject(hDC, hpen);
		::MoveToEx(hDC, rcBorder.left, rcBorder.top, NULL);
		::LineTo(hDC, rcBorder.left, rcBorder.bottom);
		::LineTo(hDC, rcBorder.right, rcBorder.bottom);
		::LineTo(hDC, rcBorder.right, rcBorder.top);
		::SelectObject(hDC, hpenOld);
		::DeleteObject(hpen);
	}
}

BOOL CRXSkinTabCtrl::OnClick(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		CRXSkinTabItem* poItem = _HitTest(pt);
		if (poItem)
		{			
			if (poItem->PtInClose(pt))
			{
				poItem = RemoveTab(poItem->GetID());			
				if (m_poListener)
				{
					m_poListener->OnTabItemClosed(poItem);
				}
				if (poItem)
				{
					if (m_poMouseOn == poItem)
					{
						m_poMouseOn = NULL;
					}
					if (m_poPressed == poItem)
					{
						m_poPressed = NULL;
					}
					if (m_poSelected == poItem)
					{
						m_poSelected = NULL;
					}
					delete poItem;
					Invalidate();
				}
			}
			else
				SelectTab(poItem->GetID());
		}
	}
	return FALSE;
}

BOOL CRXSkinTabCtrl::PtInRect(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}

BOOL CRXSkinTabCtrl::OnRButtonUp(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		CRXSkinItem* poControl = (CRXSkinItem*)_HitTest(pt);
		if (poControl)
		{
			return TRUE;
		}
		if (m_poSelected)
		{
			CRXControl* poControl = m_poSelected->GetChild();
			if (poControl)
			{
				poControl->OnRButtonUp(pt);
			}
		}
	}
	return FALSE;
}

BOOL CRXSkinTabCtrl::OnLButtonDown(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		CRXSkinItem* poControl = (CRXSkinItem*)_HitTest(pt);
		if (poControl)
		{
			if (NULL == m_poPressed)
			{
				m_poPressed = (CRXSkinTabItem*)poControl;
				if (m_poPressed->GetState()!=ITEM_SELECTED)
				{
					m_poPressed->SetState(ITEM_MOUSEON);
					RECT rc = m_poPressed->GetRect();
					InvalidateRect(GetParentWnd(),&rc,FALSE);
				}
			}
			else if (poControl != m_poPressed)
			{
				if (m_poPressed->GetState()!=ITEM_SELECTED)
				{
					m_poPressed->SetState(ITEM_NORMAL);
					RECT rc = m_poPressed->GetRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
				}
				m_poPressed = (CRXSkinTabItem*)poControl;
				if (m_poPressed->GetState()!=ITEM_SELECTED)
				{
					m_poPressed->SetState(ITEM_MOUSEON);
					RECT rc = m_poPressed->GetRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
				}
			}
			m_poPressed->OnClicked(pt);
		}
		else
		{
			if (m_poPressed)
			{
				if (m_poPressed->GetState()!=ITEM_SELECTED)
				{
					m_poPressed->SetState(ITEM_NORMAL);
					RECT rc = m_poPressed->GetRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
				}
				m_poPressed = (CRXSkinTabItem*)poControl;
			}
		}

		return poControl==NULL?FALSE:TRUE;
	}
	return FALSE;
}

CRXSkinTabItem* CRXSkinTabCtrl::_HitTest(POINT pt)
{
	CRXSkinTabItem* poItem = NULL;
	CCRXSkinTabItemMapItr itr = m_mapItems.begin();
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

BOOL CRXSkinTabCtrl::OnMouseMove(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		CRXSkinTabItem* poItem = _HitTest(pt);
		if (poItem)
		{
			if (NULL == m_poMouseOn)
			{
				m_poMouseOn = poItem;
				if (m_poMouseOn->GetState()!=ITEM_SELECTED)
				{
					m_poMouseOn->SetState(ITEM_MOUSEON);
					if (m_poMouseOn->PtInClose(pt))
					{
						m_poMouseOn->SetSelectClose(TRUE);
					}
					else
						m_poMouseOn->SetSelectClose(FALSE);

					m_poMouseOn->Invalidate();
				}
				if (m_poListener)
				{
					m_poListener->OnTabItemMouseOn(m_poMouseOn);
				}
			}
			else if (m_poMouseOn!=poItem)
			{
				m_poMouseOn->OnMouseLeave();
				if (m_poMouseOn->GetState()!=ITEM_SELECTED)
				{
					m_poMouseOn->SetState(ITEM_NORMAL);
					m_poMouseOn->SetSelectClose(FALSE);
					m_poMouseOn->Invalidate();
				}
				m_poMouseOn = poItem;
				if (m_poMouseOn->GetState()!=ITEM_SELECTED)
				{
					m_poMouseOn->SetState(ITEM_MOUSEON);
					if (m_poMouseOn->PtInClose(pt))
					{
						m_poMouseOn->SetSelectClose(TRUE);
					}
					else
						m_poMouseOn->SetSelectClose(FALSE);
					m_poMouseOn->Invalidate();
				}
				if (m_poListener)
				{
					m_poListener->OnTabItemMouseOn(m_poMouseOn);
				}
			}
			if (m_poMouseOn)
			{
				if (m_poMouseOn->PtInClose(pt))
				{
					m_poMouseOn->SetSelectClose(TRUE);
				}
				else
					m_poMouseOn->SetSelectClose(FALSE);
			}
			m_rcInvalidate = poItem->GetRect();
			SetTooltip(poItem->GetToolTip());
			CRXControl* poCurControl = m_poMouseOn->OnMouseOn(pt);
			if (poCurControl)
			{
				SetTooltip(poCurControl->GetTooltip());
			}
			return TRUE;
		}
		else
		{
			if (m_poMouseOn)
			{
				m_poMouseOn->OnMouseLeave();
				if (m_poMouseOn->GetState()!=ITEM_SELECTED)
				{
					m_poMouseOn->SetState(ITEM_NORMAL);
					m_poMouseOn->SetSelectClose(FALSE);
					m_poMouseOn->Invalidate();
				}
				m_poMouseOn = NULL;
				if (m_poListener)
				{
					m_poListener->OnTabItemMouseLeave();
				}

			}
		}

	}
	return FALSE;
}

BOOL CRXSkinTabCtrl::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	if (IsEnabled() && IsVisible())
	{
		CRXControl::OnSize(dwX,dwY);
	}
	return FALSE;
}

COLORREF CRXSkinTabCtrl::GetTabItemNormalFromBk()
{
	return m_clrTabItemNormalFromBk;
}

VOID CRXSkinTabCtrl::SetTabItemNormalFromBk(COLORREF clr)
{
	m_clrTabItemNormalFromBk = clr;
}

COLORREF CRXSkinTabCtrl::GetTabItemNormalToBk()
{
	return m_clrTabItemNormalToBk;
}

VOID CRXSkinTabCtrl::SetTabItemNormalToBk(COLORREF clr)
{
	m_clrTabItemNormalToBk = clr;
}


COLORREF CRXSkinTabCtrl::GetTabItemNormalText()
{
	return m_clrTabItemNormalText;
}

VOID CRXSkinTabCtrl::SetTabItemNormalText(COLORREF clr)
{
	m_clrTabItemNormalText = clr;
}


COLORREF CRXSkinTabCtrl::GetTabItemMouseOnFromBk()
{
	return m_clrTabItemMouseOnFromBk;
}

VOID CRXSkinTabCtrl::SetTabItemMouseOnFromBk(COLORREF clr)
{
	m_clrTabItemMouseOnFromBk = clr;
}


COLORREF CRXSkinTabCtrl::GetTabItemMouseOnToBk()
{
	return m_clrTabItemMouseOnToBk;
}

VOID CRXSkinTabCtrl::SetTabItemMouseOnToBk(COLORREF clr)
{
	m_clrTabItemMouseOnToBk = clr;
}


COLORREF CRXSkinTabCtrl::GetTabItemMouseOnText()
{
	return m_clrTabItemMouseOnText;
}

VOID CRXSkinTabCtrl::SetTabItemMouseOnText(COLORREF clr)
{
	m_clrTabItemMouseOnText = clr;
}


COLORREF CRXSkinTabCtrl::GetTabItemPressedFromBk()
{
	return m_clrTabItemPressedFromBk;
}

VOID CRXSkinTabCtrl::SetTabItemPressedFromBk(COLORREF clr)
{
	m_clrTabItemPressedFromBk = clr;
}


COLORREF CRXSkinTabCtrl::GetTabItemPressedToBk()
{
	return m_clrTabItemPressedToBk;
}

VOID CRXSkinTabCtrl::SetTabItemPressedToBk(COLORREF clr)
{
	m_clrTabItemPressedToBk = clr;
}

void  CRXSkinTabCtrl::SetTabTooltip(const uint64_t dwID, const TCHAR* pszText)
{
	CRXSkinTabItem* poTab = FindTab(dwID);
	if (poTab)
	{
		poTab->SetTooltip(pszText);
	}
}
void  CRXSkinTabCtrl::SetTabText(const uint64_t dwID, const TCHAR* pszText)
{
	CRXSkinTabItem* poTab = FindTab(dwID);
	if (poTab)
	{
		poTab->SetName(pszText);
	}
}
COLORREF CRXSkinTabCtrl::GetTabItemPressedText()
{
	return m_clrTabItemPressedText;
}

VOID CRXSkinTabCtrl::SetTabItemPressedText(COLORREF clr)
{
	m_clrTabItemPressedText = clr;
}


COLORREF CRXSkinTabCtrl::GetTabItemSelectedFromBk()
{
	return m_clrTabItemSelectedFromBk;
}

VOID CRXSkinTabCtrl::SetTabItemSelectedFromBk(COLORREF clr)
{
	m_clrTabItemSelectedFromBk = clr;
}


COLORREF CRXSkinTabCtrl::GetTabItemSelectedToBk()
{
	return m_clrTabItemSelectedToBk;
}

VOID CRXSkinTabCtrl::SetTabItemSelectedToBk(COLORREF clr)
{
	m_clrTabItemSelectedToBk = clr;
}


COLORREF CRXSkinTabCtrl::GetTabItemSelectedText()
{
	return m_clrTabItemSelectedText;
}

VOID CRXSkinTabCtrl::SetTabItemSelectedText(COLORREF clr)
{
	m_clrTabItemSelectedText = clr;
}

COLORREF CRXSkinTabCtrl::GetTabItemBorder()
{
	return m_clrTabItemBorder;
}

VOID CRXSkinTabCtrl::SetTabItemBorder(COLORREF clr)
{
	m_clrTabItemBorder = clr;
}


UINT32 CRXSkinTabCtrl::GetTabCtrlItemHeight()
{
	return m_dwTabItemHeight;
}

VOID CRXSkinTabCtrl::SetTabCtrlItemHeight(UINT32 dwHeight)
{
	m_dwTabItemHeight = dwHeight;
}

UINT32 CRXSkinTabCtrl::GetTabItemOffSetX()
{
	return m_dwTabItemOffSetX;
}

SIZE CRXSkinTabCtrl::GetTabCtrlRound()
{
	return m_szTabCtrlRound;
}

VOID CRXSkinTabCtrl::SetTabItemOffSetX(UINT32 dwOffSetX)
{
	m_dwTabItemOffSetX = dwOffSetX;
}

VOID CRXSkinTabCtrl::_RemoveAll()
{
	CCRXSkinTabItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		delete itr->second;
	}
	m_mapItems.clear();
}

CRXSkinTabItem* CRXSkinTabCtrl::AddTab(const TCHAR* pszName,
									   const char* pszKey,
									   const TCHAR* pszToolTip/* =_T */,
									   const TCHAR* pszImageFile,
									   CRXControl* poChild,
	CRXSkinWnd* poChildWnd)
{	
	CRXSkinTabItem* poItem = NULL;
	if (pszKey)
		poItem = FindTabByKey(pszKey);
	if (NULL == poItem)
	{
		poItem = new CRXSkinTabItem;
		poItem->SetID(m_dwCurID++);
		poItem->SetImage(pszImageFile);
		poItem->SetName(pszName);
		poItem->SetTooltip(pszToolTip);
		poItem->SetChild(poChild);
		poItem->SetChildWnd(poChildWnd);
		poItem->SetTabCtrl(this);
		poItem->SetParentWnd(GetParentWnd());
		poItem->SetKey(pszKey);
		poItem->Init();
		m_mapItems.insert(make_pair(poItem->GetID(),poItem));
	}
	return poItem;
}

CRXSkinTabItem* CRXSkinTabCtrl::FindTab(const uint64_t dwID)
{
	CCRXSkinTabItemMapItr itr = m_mapItems.find(dwID);
	if (itr!=m_mapItems.end())
	{
		return itr->second;
	}
	return NULL;
}

CRXSkinTabItem* CRXSkinTabCtrl::FindTabByKey(const char* pszKey)
{
	CCRXSkinTabItemMapItr itr = m_mapItems.begin();
	for (;itr != m_mapItems.end();itr++)
	{
		if (strcmp(pszKey,itr->second->GetKey().c_str())==0)
		{
			return itr->second;
		}
	}
	return NULL;
}


CRXSkinTabItem* CRXSkinTabCtrl::RemoveTabByKey(const char* pszKey)
{
	CCRXSkinTabItemMapItr itr = m_mapItems.begin();
	for (; itr != m_mapItems.end(); itr++)
	{
		if (strcmp(pszKey, itr->second->GetKey().c_str()) == 0)
		{
			CRXSkinTabItem* poItem = itr->second;
			m_mapItems.erase(itr);
			if (m_poSelected == poItem)
			{
				m_poSelected = NULL;
			}
			if (m_poMouseOn == poItem)
			{
				m_poMouseOn = NULL;
			}
			if (m_poPressed == poItem)
			{
				m_poPressed = NULL;
			}
			return poItem;
		}
	}
	return NULL;
}

CRXSkinTabItem* CRXSkinTabCtrl::RemoveTab(const uint64_t dwID)
{
	CCRXSkinTabItemMapItr itr = m_mapItems.find(dwID);
	if ( itr != m_mapItems.end())
	{
		CRXSkinTabItem* poItem = itr->second;
		m_mapItems.erase(itr);
		if (m_poSelected == poItem)
		{
			m_poSelected = NULL;
		}
		if (m_poMouseOn == poItem)
		{
			m_poMouseOn = NULL;
		}
		if (m_poPressed == poItem)
		{
			m_poPressed = NULL;
		}
		return poItem;
	}
	return NULL;
}


CRXSkinTabItem* CRXSkinTabCtrl::SelectTabByKey(const char* pszKey)
{
	CRXSkinTabItem* poItem = FindTabByKey(pszKey);
	if (poItem)
	{
		if (NULL == m_poSelected)
		{
			m_poSelected = poItem;
			CRXControl* poChild = poItem->GetChild();
			if (poChild)
			{
				poChild->SetVisible(TRUE);
			}
			CRXSkinWnd* poChildWnd = poItem->GetChildWnd();
			if (poChildWnd)
			{
				poChildWnd->Show();
			}
			m_poSelected->SetState(ITEM_SELECTED);
			m_poSelected->Invalidate();
		}
		else if (m_poSelected!=poItem)
		{
			CRXControl* poChild = m_poSelected->GetChild();
			if (poChild)
			{
				poChild->SetVisible(FALSE);
			}
			CRXSkinWnd* poChildWnd = m_poSelected->GetChildWnd();
			if (poChildWnd)
			{
				poChildWnd->Hide();
			}
			m_poSelected->SetState(ITEM_NORMAL);
			m_poSelected->Invalidate();
			m_poSelected = poItem;
			poChild = m_poSelected->GetChild();
			if (poChild)
			{
				poChild->SetVisible(TRUE);
			}
			poChildWnd = m_poSelected->GetChildWnd();
			if (poChildWnd)
			{
				poChildWnd->Show();
			}
			m_poSelected->SetState(ITEM_SELECTED);
			m_poSelected->Invalidate();
		}
		if (m_poListener)
		{
			m_poListener->OnTabItemChanged(poItem);
		}
		Invalidate();
	}
	return poItem;
}

CRXSkinTabItem* CRXSkinTabCtrl::SelectTab(const uint64_t dwID)
{
	CRXSkinTabItem* poItem = FindTab(dwID);
	if (poItem)
	{
		if (NULL == m_poSelected)
		{
			m_poSelected = poItem;
			CRXControl* poChild = poItem->GetChild();
			if (poChild)
			{
				poChild->SetVisible(TRUE);
			}
			CRXSkinWnd* poChildWnd = poItem->GetChildWnd();
			if (poChildWnd)
			{
				poChildWnd->Show();
			}
			m_poSelected->SetState(ITEM_SELECTED);
			m_poSelected->Invalidate();
		}
		else if (m_poSelected != poItem)
		{
			CRXControl* poChild = m_poSelected->GetChild();
			if (poChild)
			{
				poChild->SetVisible(FALSE);
			}
			CRXSkinWnd* poChildWnd = m_poSelected->GetChildWnd();
			if (poChildWnd)
			{
				poChildWnd->Hide();
			}
			m_poSelected->SetState(ITEM_NORMAL);
			m_poSelected->Invalidate();
			m_poSelected = poItem;
			poChild = m_poSelected->GetChild();
			if (poChild)
			{
				poChild->SetVisible(TRUE);
			}
			poChildWnd = m_poSelected->GetChildWnd();
			if (poChildWnd)
			{
				poChildWnd->Show();
			}
			m_poSelected->SetState(ITEM_SELECTED);
			m_poSelected->Invalidate();
		}
		if (m_poListener)
		{
			m_poListener->OnTabItemChanged(poItem);
		}
		Invalidate();
	}
	return poItem;
}