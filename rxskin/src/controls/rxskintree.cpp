#include "pch.h"
#include "controls\rxskintree.h"
#include "common\RXSkinService.h"
#include "common\rxskindefines.h"

#define TREE_ITEM_IMAGE_HEIGHT    16
#define TREE_ITEM_IMAGE_WIDTH     16
#define TREE_ITEM_OFFSET_X		  5
#define TREE_ITEM_OFFSET_Y		  3

CRXSkinTreeItem::CRXSkinTreeItem()
{
	m_poOwner = NULL;
	m_poNextItem = NULL;
	m_poParentItem = NULL;
	m_poPreItem = NULL;
	m_vecChildItem.clear();
	m_bIsOpened = false;
	m_dwImageID = 0;
	m_bIsRoot = false;
}

CRXSkinTreeItem::~CRXSkinTreeItem()
{
}

int32_t CRXSkinTreeItem::GetBottom()
{
	if (m_bIsOpened==false)
	{
		return GetRect().bottom;
	}
	if (m_vecChildItem.size()<=0)
	{
		return GetRect().bottom;
	}
	int32 nBottom = -999999;
	for (uint32_t t=0;t<(uint32_t)m_vecChildItem.size();t++)
	{
		if (m_vecChildItem[t]->GetRect().bottom>nBottom)
		{
			nBottom  = m_vecChildItem[t]->GetRect().bottom;
		}
	}
	return nBottom;
}

int32_t CRXSkinTreeItem::OnDrawEx(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	OnDraw(poSurface,hDC, rcItem);
	return rcItem.bottom;
}

void CRXSkinTreeItem::Invalidate()
{
	CRXSkinRect rc = GetRect();
	rc.left += m_poOwner->GetRect().left;
	rc.right += m_poOwner->GetRect().left;
	rc.top += m_poOwner->GetRect().top;
	rc.bottom += m_poOwner->GetRect().top;
	::InvalidateRect(GetParentWnd(),& rc, TRUE);
}

void CRXSkinTreeItem::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (m_bIsRoot)
	{
		CRXSkinRect rcChildItem = rcItem;
		rcChildItem.bottom = rcChildItem.top + CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemHeight();
		for (uint32_t t=0;t<(uint32_t)m_vecChildItem.size();t++)
		{
			int32_t nBottom = m_vecChildItem[t]->OnDrawEx(poSurface,hDC, rcChildItem);			
			rcChildItem.top += m_vecChildItem[t]->GetItemHeight();
			rcChildItem.bottom = rcChildItem.top + CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemHeight();
		}
		return;
	}
	IRXImage* poStatusImage = NULL;
	COLORREF clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemNormalText();
	switch (GetState())
	{
	case ITEM_NORMAL:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemNormalText();
		if (IsOpened())
		{
			poStatusImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_ARROW_COLLASPSE_EXPAND_NORMAL);
		}
		else
			poStatusImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_ARROW_COLLASPSE_NORMAL);
	}
	break;
	case ITEM_MOUSEON:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemMouseOnText();
		DrawGradientRect(hDC, TRUE, rcItem, CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemMouseOnFrom(),
			CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemMouseOnTo());
		if (IsOpened())
		{
			poStatusImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_ARROW_COLLASPSE_EXPAND_HOT);
		}
		else
			poStatusImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_ARROW_COLLASPSE_HOT);
	}
	break;
	case ITEM_PRESSED:
	case ITEM_SELECTED:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemPressedText();
		DrawGradientRect(hDC, TRUE, rcItem, CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemPressedFrom(),
			CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemPressedTo());
		if (IsOpened())
		{
			poStatusImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_ARROW_COLLASPSE_EXPAND_HOT);
		}
		else
			poStatusImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_ARROW_COLLASPSE_HOT);
	}
	break;
	default:
		break;
	}
	CRXSkinRect rcTxt = rcItem;
	if (poStatusImage)
	{
		uint32_t dwTop = rcTxt.top + (rcTxt.Height() - poStatusImage->GetHeight())/2;
		uint32_t dwLeft = rcTxt.left + TREE_ITEM_OFFSET_X;
		poStatusImage->Draw(hDC, dwLeft, dwTop);
		rcTxt.left = dwLeft + TREE_ITEM_OFFSET_X + TREE_ITEM_IMAGE_WIDTH;
	}

	CRXSkinImageList* poImageList = m_poOwner->GetImageList();
	if (poImageList)
	{
		IRXImage* poImage = poImageList->GetImage();
		if (poImage)
		{
			int cx = poImageList->GetX();
			int cy = poImageList->GetY();
			RGBQUAD tran;
			tran.rgbBlue = 0;
			tran.rgbGreen = 0;
			tran.rgbRed = 0;
			tran.rgbReserved = 0;
			if (poImageList->CheckImageIndex(m_dwImageID))
			{
				uint32_t dwTop = rcTxt.top + (rcTxt.Height() - TREE_ITEM_IMAGE_HEIGHT) / 2;
				uint32_t dwLeft = rcTxt.left;
				poImageList->GetImage()->DrawTrans(hDC,
					tran,
					dwLeft,
					dwTop,
					cx,
					cy,
					0,
					m_dwImageID * cy
				);	
				rcTxt.left = dwLeft + cx + TREE_ITEM_OFFSET_X;
			}
		}

	}


	rcTxt.left += CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlOffSetX();
	rcTxt.right -= CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlOffSetX();
	tstring strTxt = m_strText;
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt, 
			clrTxt, ID_FONT_COMMON,
			DT_VCENTER | DT_SINGLELINE | DT_LEFT | DT_WORDBREAK | DT_END_ELLIPSIS);
	}
	
	SIZE szTxt;
	CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, strTxt, szTxt);

	rcTxt.right = rcTxt.left + szTxt.cx;
	
	m_poOwner->ModifyTotalHeight(rcItem.top, rcItem.bottom);
	m_poOwner->ModifyTotalWidth(rcItem.top, rcItem.right);
	CRXSkinRect rcChild = rcItem;
	rcItem.right = rcTxt.right+3;
	SetRect(rcItem);
	if (m_bIsOpened)
	{
		rcChild.left += (TREE_ITEM_OFFSET_X + TREE_ITEM_IMAGE_WIDTH);
		rcChild.top = rcChild.bottom;
		rcChild.bottom = rcChild.top + CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemHeight();
		for (uint32_t t = 0; t < (uint32_t)m_vecChildItem.size(); t++)
		{
		    int32_t nBottom	= m_vecChildItem[t]->OnDrawEx(poSurface,hDC, rcChild);
			rcChild.top += m_vecChildItem[t]->GetItemHeight();
			rcChild.bottom = rcChild.top + CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemHeight();
		}
	}
}

uint32_t CRXSkinTreeItem::GetItemHeight()
{
	if (m_bIsOpened)
	{
		uint32_t dwHeight = CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemHeight();
		for (uint32_t t=0;t<(uint32_t)m_vecChildItem.size();t++)
		{
			dwHeight += m_vecChildItem[t]->GetItemHeight();
		}
		return dwHeight;
	}
	return CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemHeight();
}

void CRXSkinTreeItem::AddChildItem(CRXSkinTreeItem* poItem)
{
	poItem->SetParentItem(this);
	if (m_vecChildItem.size()>0)
	{
		poItem->SetPreItem(m_vecChildItem[m_vecChildItem.size() - 1]);
		poItem->SetNextItem(NULL);
		m_vecChildItem[m_vecChildItem.size() - 1]->SetNextItem(poItem);
	}
	else
	{
		poItem->SetPreItem(NULL);
		poItem->SetNextItem(NULL);
	}
	m_vecChildItem.push_back(poItem);
}

CRXSkinTreeItem* CRXSkinTreeItem::FindChildItem(const uint32_t& dwID)
{
	for (uint32_t t=0;t<(uint32_t)m_vecChildItem.size();t++)
	{
		if (dwID == m_vecChildItem[t]->GetID())
		{
			return m_vecChildItem[t];
		}
	}
	return NULL;
}

CRXSkinTreeItem* CRXSkinTreeItem::RemoveChildItem(const uint32_t& dwID)
{
	vector<CRXSkinTreeItem*>::iterator itr = m_vecChildItem.begin();
	for (;itr!=m_vecChildItem.end();itr++)
	{
		if (dwID == (*itr)->GetID())
		{
			CRXSkinTreeItem* poItem = *itr;
			m_vecChildItem.erase(itr);
			return poItem;
		}
	}
	return NULL;
}

void CRXSkinTreeItem::SetIsOpened(const bool& bIsOpened)
{
	m_bIsOpened = bIsOpened;
}

CRXSkinItem* CRXSkinTreeItem::HitTest(POINT pt)
{
	if (m_bIsRoot)
	{
		return NULL;
	}
	CRXSkinRect rc = GetRect();
	rc.left += m_poOwner->GetRect().left;
	rc.right += m_poOwner->GetRect().left;
	rc.top += m_poOwner->GetRect().top;
	rc.bottom += m_poOwner->GetRect().top;
	if (rc.PtInRect(pt))
	{
		return this;
	}
	return NULL;
}


CRXSkinTree::CRXSkinTree(void)
{
	m_clrText = RGB(50, 250, 50);
	m_clrBK = RGB(201, 212, 215);
	m_clrBorder = RGB(201, 212, 215);
	m_poMouseOn = NULL;
	m_poPressed = NULL; 
	m_poSelected = NULL;
	m_poListener = NULL;
	m_poImageList = NULL;
	m_dwCurItemID = 1;
	m_nMinY = 999999;
	m_nMaxY = -999999;
	m_nTopOffSet = 0;
	m_nMinX = 999999;
	m_nMaxX = -999999;
	m_nLeftOffSet = 0;
	m_oScrollbarVer.SetType(SCROLLBAR_VER);
	m_oScrollbarVer.SetVisible(FALSE);
	m_oScrollbarVer.SetOwner(this);

	m_oScrollbarHov.SetType(SCROLLBAR_HOR);
	m_oScrollbarHov.SetVisible(FALSE);
	m_oScrollbarHov.SetOwner(this);
}

void CRXSkinTree::InitRootItem()
{
	m_poRootItem = new CRXSkinTreeItem;
	m_poRootItem->SetID(ID_TREE_ROOT);
	m_poRootItem->SetIsRoot(true);
	m_poRootItem->SetOwner(this);
	m_poRootItem->SetParentWnd(GetParentWnd());
	m_mapItem[ID_TREE_ROOT] = m_poRootItem;
}

CRXSkinTree::~CRXSkinTree(void)
{
	_DeleteAll();
}

VOID CRXSkinTree::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (IsVisible())
	{
		CRXSkinMemDC memDC(hDC, rcItem);
		POINT pt = rcItem.TopLeft();
		rcItem.OffsetRect(pt);
		DrawRect(memDC.GetHDC(),
			CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBorder(),
			CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBK(),
			rcItem.left,rcItem.top,rcItem.right,rcItem.bottom);

		CRXSkinTreeItemMapItr itr = m_mapItem.begin();
		for (;itr!=m_mapItem.end();itr++)
		{
			itr->second->GetRect().SetRectEmpty();
		}
		if (m_poRootItem)
		{
			m_nMaxY = -99999;
			m_nMinY = 99999;

			m_nMaxX = -99999;
			m_nMinX = 99999;

			CRXSkinRect rcChild = rcItem;
			rcChild.left += TREE_ITEM_OFFSET_X;
			rcChild.top += TREE_ITEM_OFFSET_Y;
			rcChild.top -= m_nTopOffSet;
			rcChild.left -= m_nLeftOffSet;
			if (m_oScrollbarVer.IsVisible())
			{
				rcChild.right -= m_oScrollbarVer.GetWidth();
			}
			
			m_poRootItem->OnDraw(memDC.GetSurface(), memDC.GetHDC(), rcChild);
			int32_t nTotalHeight = GetTotalHeight();
			if (nTotalHeight<0)
			{
				if (m_oScrollbarVer.IsVisible())
				{
					m_oScrollbarVer.SetVisible(FALSE);
					m_nTopOffSet = 0;
					Invalidate();
				}
			}
			else
			{
				if (nTotalHeight > rcItem.Height())
				{
					if (m_oScrollbarVer.IsVisible() == FALSE)
					{
						m_oScrollbarVer.SetVisible(TRUE);
						m_oScrollbarVer.SetStep(CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemHeight());
						m_oScrollbarVer.SetTotalHeight(rcItem.Height(), (INT32)nTotalHeight);
						Invalidate();
					}
				}
				else
				{
					if (m_oScrollbarVer.IsVisible())
					{
						m_oScrollbarVer.SetVisible(FALSE);
						m_nTopOffSet = 0;
						Invalidate();
					}
				}
			}
		
		}
		m_oScrollbarVer.OnDraw(memDC.GetSurface(), memDC.GetHDC(), rcItem);

		int32_t nTotalWidth = GetTotalWidth();
		if (nTotalWidth <= 0)
		{
			if (m_oScrollbarHov.IsVisible())
			{
				m_oScrollbarHov.SetVisible(FALSE);
				m_nTopOffSet = 0;
				Invalidate();
			}
		}
		else
		{
			if (nTotalWidth > rcItem.Width())
			{
				if (m_oScrollbarHov.IsVisible() == FALSE)
				{
					m_oScrollbarHov.SetVisible(TRUE);
					m_oScrollbarHov.SetStep(CRXSkinService::Instance()->GetDrawMgr().GetTreeCtrlItemHeight());
					m_oScrollbarHov.SetTotalHeight(rcItem.Width(), (INT32)nTotalWidth);
					Invalidate();
				}
			}
			else
			{
				if (m_oScrollbarHov.IsVisible())
				{
					m_oScrollbarHov.SetVisible(FALSE);
					m_nLeftOffSet = 0;
					Invalidate();
				}
			}
		}
		memDC.TakeSnapshot();
	}
}

VOID CRXSkinTree::ReleaseControl()
{
	delete this;
}


BOOL CRXSkinTree::PtInRect(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}

BOOL CRXSkinTree::OnClick(POINT pt)
{
	OnLButtonUp(pt);
	if (m_poListener)
	{
		CRXSkinTreeItem* poItem = _HitTest(pt);
		if (poItem)
		{
			m_poListener->OnTreeItemSelected(this, poItem);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXSkinTree::OnMouseMove(POINT pt)
{
	if (IsVisible() == FALSE) return FALSE;
	if (PtInRect(pt))
	{
		CRXSkinTreeItem* poItem = _HitTest(pt);
		if (poItem)
		{
			if (m_poMouseOn == NULL)
			{
				if (poItem->IsSelected() == FALSE)
				{
					m_poMouseOn = poItem;
					m_poMouseOn->SetState(ITEM_MOUSEON);
					m_poMouseOn->Invalidate();
				}
			}
			else {
				if (m_poMouseOn != poItem)
				{
					if (m_poMouseOn->IsSelected() == FALSE)
					{
						m_poMouseOn->SetState(ITEM_NORMAL);
						m_poMouseOn->Invalidate();
					}
					if (poItem->IsSelected() == FALSE)
					{
						m_poMouseOn = poItem;
						m_poMouseOn->SetState(ITEM_MOUSEON);
						m_poMouseOn->Invalidate();
					}
				}
			}
		}
		return TRUE;
	}
	else
	{
		if (m_poMouseOn)
		{
			if (m_poMouseOn->IsSelected() == FALSE)
			{
				m_poMouseOn->SetState(ITEM_NORMAL);
				m_poMouseOn->Invalidate();				
			}
			m_poMouseOn = NULL;
		}
	}
	return FALSE;
}
BOOL CRXSkinTree::OnLButtonDown(POINT pt)
{
	if (IsVisible() == FALSE) return FALSE;
	if (PtInRect(pt))
	{
		CRXSkinTreeItem* poItem = _HitTest(pt);
		if (poItem)
		{
			if (m_poPressed == NULL)
			{
				if (!poItem->IsSelected())
				{
					m_poPressed = poItem;
					m_poPressed->SetState(ITEM_PRESSED);
					m_poPressed->Invalidate();
				}
			}
			else {
				if (m_poPressed != poItem)
				{
					if (!m_poPressed->IsSelected())
					{
						m_poPressed->SetState(ITEM_NORMAL);
						m_poPressed->Invalidate();
					}

					if (!poItem->IsSelected())
					{
						m_poPressed = poItem;
						m_poPressed->SetState(ITEM_PRESSED);
						m_poPressed->Invalidate();
					}
				}
			}
		}
		return TRUE;
	}
	else
	{
		if (m_poPressed && !m_poPressed->IsSelected())
		{
			m_poPressed->SetState(ITEM_NORMAL);
			m_poPressed->Invalidate();
			m_poPressed = NULL;
		}
	}
	return FALSE;
}

BOOL CRXSkinTree::OnLButtonUp(POINT pt)
{
	if (IsVisible() == FALSE) return FALSE;
	if (m_oScrollbarVer.IsVisible())
	{
		m_oScrollbarVer.OnClick(pt);
	}
	if (PtInRect(pt))
	{
		CRXSkinTreeItem* poItem = _HitTest(pt);
		if (poItem)
		{
			if (m_poSelected == NULL)
			{
				m_poSelected = poItem;
				m_poSelected->SetState(ITEM_SELECTED);
				m_poSelected->Invalidate();
			}
			else {
				if (m_poSelected != poItem)
				{
					m_poSelected->SetState(ITEM_NORMAL);
					m_poSelected->Invalidate();
					m_poSelected = poItem;
					m_poSelected->SetState(ITEM_SELECTED);
					m_poSelected->Invalidate();
				}
			}
		}
		return TRUE;
	}
	else
	{
		if (m_poSelected)
		{
			m_poSelected->SetState(ITEM_NORMAL);
			m_poSelected->Invalidate();
			m_poSelected = NULL;
		}
	}
	return TRUE;

}

BOOL CRXSkinTree::OnSize(const UINT32 dwX, const UINT32 dwY)
{
	CRXControl::OnSize(dwX, dwY);
	return FALSE;
}

void CRXSkinTree::SetRect(const CRXSkinRect rcItem)
{
	CRXControl::SetRect(rcItem);
}

CRXSkinTreeItem* CRXSkinTree::FindItem(const uint32_t& dwItemID)
{
	CRXSkinTreeItemMapItr itr = m_mapItem.find(dwItemID);
	if (itr!=m_mapItem.end())
	{
		return itr->second;
	}
	return NULL;
}

uint32_t CRXSkinTree::AddItem(const uint32_t& dwParentID,
	const TCHAR* pszTxt,
	const TCHAR* pszToolTip ,
	const TCHAR* pszExtern,
	const uint32_t& dwImageID)
{
	CRXSkinTreeItem* poParent = FindItem(dwParentID);
	if (NULL == poParent)
	{
		return INVALID_32BIT_ID;
	}
	uint32_t dwItemID = m_dwCurItemID++;
	CRXSkinTreeItem* poItem = new CRXSkinTreeItem;
	poItem->SetOwner(this);
	poItem->SetID(dwItemID);
	poItem->SetText(pszTxt);
	poItem->SetTooltip(pszToolTip);
	poItem->SetImageID(dwImageID);
	poItem->SetExtern(pszExtern);
	poParent->AddChildItem(poItem);
	poItem->SetParentWnd(GetParentWnd());
	m_mapItem.insert(make_pair(dwItemID, poItem));

	Invalidate();
	return dwItemID;
}

CRXSkinTreeItem* CRXSkinTree::_HitTest(POINT pt)
{
	CRXSkinTreeItemMapItr itr = m_mapItem.begin();
	for (;itr!=m_mapItem.end();itr++)
	{
		if (itr->second->HitTest(pt))
		{
			return itr->second;
		}
	}
	return NULL;
}

void CRXSkinTree::RemoveItem(const UINT32& dwID)
{
	CRXSkinTreeItemMapItr itr = m_mapItem.find(dwID);
	if (itr!=m_mapItem.end())
	{
		CRXSkinTreeItem* poItem = itr->second;
		m_mapItem.erase(itr);
		delete poItem;
		Invalidate();
	}
}

void CRXSkinTree::_DeleteAll()
{
	for (CRXSkinTreeItemMapItr itr = m_mapItem.begin();itr!=m_mapItem.end();itr++)
	{
		delete itr->second;
	}
	m_mapItem.clear();
}


BOOL CRXSkinTree::OnRButtonUp(POINT pt) 
{
	if (m_poListener)
	{
		if (PtInRect(pt))
		{
			m_poListener->OnTreeItemRClick(this, m_poPressed);
			return TRUE;
		}
	}
	return FALSE; 
}

void  CRXSkinTree::ExpandItem(const uint32_t& dwItemID)
{
	CRXSkinTreeItem* poItem = FindItem(dwItemID);
	if (poItem)
	{
		poItem->SetIsOpened(true);
		Invalidate();
	}
}

void  CRXSkinTree::CollaspseItem(const uint32_t& dwItemID)
{
	CRXSkinTreeItem* poItem = FindItem(dwItemID);
	if (poItem)
	{
		poItem->SetIsOpened(false);
		Invalidate();
	}
}

void CRXSkinTree::ModifyTotalHeight(const int32_t& nTop, const int32_t& nBottom)
{
	if (nTop<m_nMinY)
	{
		m_nMinY = nTop;
	}
	if (nBottom>m_nMaxY)
	{
		m_nMaxY = nBottom;
	}
		
}

void CRXSkinTree::ModifyTotalWidth(const int32_t& nLeft, const int32_t& nRight)
{
	if (nLeft < m_nMinX)
	{
		m_nMinX = nLeft;
	}
	if (nRight > m_nMaxX)
	{
		m_nMaxX = nRight;
	}
}

VOID CRXSkinTree::OnScrollBarChanged(EScrollType enType, INT32 nPercent)
{
	if (enType == SCROLLBAR_VER)
	{	
		int32_t nTotalHeight = GetTotalHeight();
		if (nTotalHeight>0 && nTotalHeight>GetRect().Height())
		{
			CRXSkinRect rcClient = GetRect();
			m_nTopOffSet = ((nTotalHeight - rcClient.Height()) * nPercent) / 100;
			Invalidate();
		}

	}
}

BOOL CRXSkinTree::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	BOOL bResult = CRXControl::OnMouseWheel(nFlags, zDelta, pt);
	if (m_oScrollbarVer.OnMouseWheel(nFlags, zDelta, pt))
	{
		return TRUE;
	}
	return bResult;
}