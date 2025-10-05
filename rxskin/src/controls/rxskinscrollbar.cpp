#include "pch.h"
#include "controls\rxskinscrollbar.h"
#include "common\RXSkinService.h"
#include "common\rxskindefines.h"

#define   ID_SCROLLBAR_HOR_LEFT      1
#define   ID_SCROLLBAR_HOR_TRACK     2
#define   ID_SCROLLBAR_HOR_RIGHT     3
#define   ID_SCROLLBAR_VER_TOP       4
#define   ID_SCROLLBAR_VER_TRACK     5
#define   ID_SCROLLBAR_VER_BOTTOM    6

#define   SCROLLBAR_STEP             3


CRXSkinScrollbarItem::CRXSkinScrollbarItem()
{
	m_enType = SCROLLBAR_ITEM_VER_TOP;
}

CRXSkinScrollbarItem::~CRXSkinScrollbarItem()
{

}

VOID CRXSkinScrollbarItem::SetType(EScrollbarItemType enType)
{
	m_enType = enType;
}

VOID CRXSkinScrollbarItem::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	switch(m_enType)
	{
	case SCROLLBAR_ITEM_VER_TOP:
		{
			_DrawVerTop(hDC);
		}
		break;
	case SCROLLBAR_ITEM_VER_TRACK:
		{
			_DrawVerTrack(hDC);
		}
		break;
	case SCROLLBAR_ITEM_VER_BOTTOM:
		{
			_DrawVerBottom(hDC);
		}
		break;

	case SCROLLBAR_ITEM_HOR_LEFT:
		{
			_DrawHorLeft(hDC);
		}
		break;
	case SCROLLBAR_ITEM_HOR_TRACK:
		{
			_DrawHorTrack(hDC);
		}
		break;
	case SCROLLBAR_ITEM_HOR_RIGHT:
		{
			_DrawHorRight(hDC);
		}
		break;
	default:
		break;
	}
}

CRXSkinItem* CRXSkinScrollbarItem::HitTest(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return this;
	}
	return NULL;
}



VOID CRXSkinScrollbarItem::_DrawHorRight(HDC hDC)
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR_HOR);
	if (NULL == poImage)
	{
		return;
	}
	INT32 nHeight = poImage->GetHeight() / 3;
	INT32 nWidth = poImage->GetWidth() / 4;
	switch (GetState())
	{
	case ITEM_NORMAL:
	{
		poImage->Draw(hDC, GetRect(), 3 * nWidth, 0);
	}
	break;
	case ITEM_MOUSEON:
	{
		poImage->Draw(hDC, GetRect(), 3 * nWidth, nHeight);
	}
	break;
	case ITEM_PRESSED:
	{
		poImage->Draw(hDC, GetRect(),3 * nWidth, 2 * nHeight);
	}
	break;
	default:
		break;
	}
}

VOID CRXSkinScrollbarItem::_DrawHorLeft(HDC hDC)
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR_HOR);
	if (NULL == poImage)
	{
		return;
	}
	INT32 nHeight = poImage->GetHeight() / 3;
	INT32 nWidth = poImage->GetWidth() / 4;
	switch (GetState())
	{
	case ITEM_NORMAL:
	{
		poImage->Draw(hDC, GetRect(), nWidth,0);
	}
	break;
	case ITEM_MOUSEON:
	{
		poImage->Draw(hDC, GetRect(), nWidth, nHeight);
	}
	break;
	case ITEM_PRESSED:
	{
		poImage->Draw(hDC, GetRect(), nWidth, 2 * nHeight);
	}
	break;
	default:
		break;
	}
}

VOID CRXSkinScrollbarItem::_DrawHorTrack(HDC hDC)
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR_HOR);
	if (NULL == poImage)
	{
		return;
	}
	INT32 nHeight = poImage->GetHeight() / 3;
	INT32 nWidth = poImage->GetWidth() / 4;
	switch (GetState())
	{
	case ITEM_NORMAL:
	{
		INT32 nTotalWidth = GetRect().Width();
		INT32 nPiceWidth = nWidth / 3;
		CRXSkinRect rcLeft = GetRect();
		rcLeft.right = rcLeft.left + nPiceWidth;
		poImage->Draw(hDC, rcLeft, 0, 0);

		INT32 nCount = (nTotalWidth - 2 * nPiceWidth) / nPiceWidth;
		for (INT32 n = 0; n < nCount; n++)
		{
			rcLeft.left = rcLeft.right;
			rcLeft.right = rcLeft.left + nPiceWidth;
			poImage->Draw(hDC, rcLeft, nPiceWidth,0);
		}
		if ((nTotalWidth - 2 * nPiceWidth) % nPiceWidth)
		{
			rcLeft.left = GetRect().right - 2 * nPiceWidth;
			rcLeft.right = rcLeft.left + nPiceWidth;
			poImage->Draw(hDC, rcLeft, 2 * nPiceWidth, 0);
		}

		CRXSkinRect rcRight = GetRect();
		rcRight.left = rcRight.right - nPiceWidth - 1;
		poImage->Draw(hDC, rcRight, 2 * nPiceWidth, 0);
	}
	break;
	case ITEM_MOUSEON:
	{
		INT32 nTotalWidth = GetRect().Width();
		INT32 nPiceWidth = nWidth / 3;
		CRXSkinRect rcLeft = GetRect();
		rcLeft.right = rcLeft.left + nPiceWidth;
		poImage->Draw(hDC, rcLeft, 0, nHeight);

		INT32 nCount = (nTotalWidth - 2 * nPiceWidth) / nPiceWidth;
		for (INT32 n = 0; n < nCount; n++)
		{
			rcLeft.left = rcLeft.right;
			rcLeft.right = rcLeft.left + nPiceWidth;
			poImage->Draw(hDC, rcLeft, nPiceWidth, nHeight);
		}
		if ((nTotalWidth - 2 * nPiceWidth) % nPiceWidth)
		{
			rcLeft.left = GetRect().right - 2 * nPiceWidth;
			rcLeft.right = rcLeft.left + nPiceWidth;
			poImage->Draw(hDC, rcLeft, 2 * nPiceWidth, nHeight);
		}

		CRXSkinRect rcRight = GetRect();
		rcRight.left = rcRight.right - nPiceWidth - 1;
		poImage->Draw(hDC, rcRight, 2 * nPiceWidth, nHeight);
	}
	break;
	case ITEM_PRESSED:
	{
		INT32 nTotalWidth = GetRect().Width();
		INT32 nPiceWidth = nWidth / 3;
		CRXSkinRect rcLeft = GetRect();
		rcLeft.right = rcLeft.left + nPiceWidth;
		poImage->Draw(hDC, rcLeft, 0, 2 * nHeight);

		INT32 nCount = (nTotalWidth - 2 * nPiceWidth) / nPiceWidth;
		for (INT32 n = 0; n < nCount; n++)
		{
			rcLeft.left = rcLeft.right;
			rcLeft.right = rcLeft.left + nPiceWidth;
			poImage->Draw(hDC, rcLeft, nPiceWidth, 2 * nHeight);
		}
		if ((nTotalWidth - 2 * nPiceWidth) % nPiceWidth)
		{
			rcLeft.left = GetRect().right - 2 * nPiceWidth;
			rcLeft.right = rcLeft.left + nPiceWidth;
			poImage->Draw(hDC, rcLeft, 2 * nPiceWidth, 2 * nHeight);
		}

		CRXSkinRect rcRight = GetRect();
		rcRight.left = rcRight.right - nPiceWidth - 1;
		poImage->Draw(hDC, rcRight, 2 * nPiceWidth, 2 * nHeight);
	}
	break;
	default:
		break;
	}
}

VOID CRXSkinScrollbarItem::_DrawVerBottom(HDC hDC)
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR);
	if (NULL == poImage)
	{
		return;
	}
	INT32 nHeight = poImage->GetHeight() / 4;
	INT32 nWidth = poImage->GetWidth() / 3;
	switch(GetState())
	{
	case ITEM_NORMAL:
		{
			poImage->Draw(hDC,GetRect(), 0,2 * nHeight);
		}
		break;
	case ITEM_MOUSEON:
		{
			poImage->Draw(hDC,GetRect(), nWidth, 2 * nHeight);
		}
		break;
	case ITEM_PRESSED:
		{
			poImage->Draw(hDC,GetRect(), 2*nWidth, 2*nHeight);
		}
		break;
	default:
		break;
	}
}

VOID CRXSkinScrollbarItem::_DrawVerTop(HDC hDC)
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR);
	if (NULL == poImage)
	{
		return;
	}
	INT32 nHeight = poImage->GetHeight() / 5;
	INT32 nWidth = poImage->GetWidth() / 9;
	switch(GetState())
	{
	case ITEM_NORMAL:
		{
			poImage->Draw(hDC,GetRect());
		}
		break;
	case ITEM_MOUSEON:
		{
			poImage->Draw(hDC,GetRect(),0, nHeight);
		}
		break;
	case ITEM_PRESSED:
		{
			poImage->Draw(hDC,GetRect(),0,2 * nHeight);
		}
		break;
	default:
		break;
	}
}

VOID CRXSkinScrollbarItem::_DrawVerTrack(HDC hDC)
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR);
	if (NULL == poImage)
	{
		return;
	}
	INT32 nHeight = poImage->GetHeight() / 4;
	INT32 nWidth = poImage->GetWidth() / 3;
	switch(GetState())
	{
	case ITEM_NORMAL:
	{
		INT32 nTotalHeight = GetRect().Height();
		INT32 nPiceHight = nHeight / 3;
		CRXSkinRect rcTop = GetRect();
		rcTop.bottom = rcTop.top + nPiceHight;
		poImage->Draw(hDC, rcTop, 0, 3 * nHeight);

		INT32 nCount = (nTotalHeight - 2 * nPiceHight) / nPiceHight;
		for (INT32 n = 0; n < nCount; n++)
		{
			rcTop.top = rcTop.bottom;
			rcTop.bottom = rcTop.top + nPiceHight;
			poImage->Draw(hDC, rcTop, 0, 3 * nHeight + nPiceHight);
		}
		if ((nTotalHeight - 2 * nPiceHight) % nPiceHight)
		{
			rcTop.top = GetRect().bottom - 2 * nPiceHight;
			rcTop.bottom = rcTop.top + nPiceHight;
			poImage->Draw(hDC, rcTop, 0, 3 * nHeight + nPiceHight);
		}

		CRXSkinRect rcBottom = GetRect();
		rcBottom = GetRect();
		rcBottom.top = rcBottom.bottom - nPiceHight-1;
		poImage->Draw(hDC, rcBottom, 0, 3 * nHeight + 2 * nPiceHight);
	}
	break;
	case ITEM_MOUSEON:
	{
		INT32 nTotalHeight = GetRect().Height();
		INT32 nPiceHight = nHeight / 3;
		INT32 nPiceWidth = poImage->GetWidth() / 3;
		CRXSkinRect rcTop = GetRect();
		rcTop.bottom = rcTop.top + nPiceHight;
		poImage->Draw(hDC, rcTop, nPiceWidth, 3 * nHeight);

		INT32 nCount = (nTotalHeight - 2 * nPiceHight) / nPiceHight;
		for (INT32 n = 0; n < nCount; n++)
		{
			rcTop.top = rcTop.bottom;
			rcTop.bottom = rcTop.top + nPiceHight;
			poImage->Draw(hDC, rcTop, nPiceWidth, 3 * nHeight + nPiceHight);
		}
		if ((nTotalHeight - 2 * nPiceHight) % nPiceHight)
		{
			rcTop.top = GetRect().bottom - 2 * nPiceHight;
			rcTop.bottom = rcTop.top + nPiceHight;
			poImage->Draw(hDC, rcTop, nPiceWidth, 3 * nHeight + nPiceHight);
		}

		CRXSkinRect rcBottom = GetRect();
		rcBottom = GetRect();
		rcBottom.top = rcBottom.bottom - nPiceHight - 1;
		poImage->Draw(hDC, rcBottom, nPiceWidth, 3 * nHeight + 2 * nPiceHight);
	}
	break;
	case ITEM_PRESSED:
	{
		INT32 nTotalHeight = GetRect().Height();
		INT32 nPiceHight = nHeight / 3;
		INT32 nPiceWidth = poImage->GetWidth() / 3;
		CRXSkinRect rcTop = GetRect();
		rcTop.bottom = rcTop.top + nPiceHight;
		poImage->Draw(hDC, rcTop, 2 * nPiceWidth, 3 * nHeight);

		INT32 nCount = (nTotalHeight - 2 * nPiceHight) / nPiceHight;
		for (INT32 n = 0; n < nCount; n++)
		{
			rcTop.top = rcTop.bottom;
			rcTop.bottom = rcTop.top + nPiceHight;
			poImage->Draw(hDC, rcTop, 2 * nPiceWidth, 3 * nHeight + nPiceHight);
		}
		if ((nTotalHeight - 2 * nPiceHight) % nPiceHight)
		{
			rcTop.top = GetRect().bottom - 2 * nPiceHight;
			rcTop.bottom = rcTop.top + nPiceHight;
			poImage->Draw(hDC, rcTop, 2 * nPiceWidth, 3 * nHeight + nPiceHight);
		}

		CRXSkinRect rcBottom = GetRect();
		rcBottom = GetRect();
		rcBottom.top = rcBottom.bottom - nPiceHight - 1;
		poImage->Draw(hDC, rcBottom, 2 * nPiceWidth, 3 * nHeight + 2 * nPiceHight);
	}
	break;
	default:
		break;
	}
}


CRXSkinScrollbar::CRXSkinScrollbar(void)
{
	m_enType = SCROLLBAR_VER;
	m_nScrollPos = 0;
	m_nScrollBarHeight = 0;
	m_dwTotalHeight = 0;
	m_dwTotalWidth = 0;
	m_dwLessHeight = 0;
	m_dwLessWidth = 0;
	m_nScrollBarWidth = 0;
	m_poMouseOn = NULL;
	m_poPressed = NULL;
	m_fPercent = 0.0;
	m_oHorTrack.SetID(ID_SCROLLBAR_HOR_TRACK);
	m_oHorTrack.SetType(SCROLLBAR_ITEM_HOR_TRACK);
	m_mapItems.insert(make_pair(ID_SCROLLBAR_HOR_TRACK,&m_oHorTrack));

	m_oHorLeft.SetType(SCROLLBAR_ITEM_HOR_LEFT);
	m_oHorLeft.SetID(ID_SCROLLBAR_HOR_LEFT);
	m_mapItems.insert(make_pair(ID_SCROLLBAR_HOR_LEFT,&m_oHorLeft));

	m_oHorRight.SetType(SCROLLBAR_ITEM_HOR_RIGHT);
	m_oHorRight.SetID(ID_SCROLLBAR_HOR_RIGHT);
	m_mapItems.insert(make_pair(ID_SCROLLBAR_HOR_RIGHT,&m_oHorRight));

	m_oVerTrack.SetType(SCROLLBAR_ITEM_VER_TRACK);
	m_oVerTrack.SetID(ID_SCROLLBAR_VER_TRACK);
	m_mapItems.insert(make_pair(ID_SCROLLBAR_VER_TRACK,&m_oVerTrack));

	m_oVerTop.SetType(SCROLLBAR_ITEM_VER_TOP);
	m_oVerTop.SetID(ID_SCROLLBAR_VER_TOP);
	m_mapItems.insert(make_pair(ID_SCROLLBAR_VER_TOP,&m_oVerTop));

	m_oVerBottom.SetType(SCROLLBAR_ITEM_VER_BOTTOM);
	m_oVerBottom.SetID(ID_SCROLLBAR_VER_BOTTOM);
	m_mapItems.insert(make_pair(ID_SCROLLBAR_VER_BOTTOM,&m_oVerBottom));

	m_nItemHeight = m_nItemWidth = 0;

	m_bPressed = FALSE;
	m_nStep = SCROLLBAR_STEP;
	m_nTrackOffSet = 0;

	m_poOwner = NULL;
}

CRXSkinScrollbar::~CRXSkinScrollbar(void)
{
}

VOID CRXSkinScrollbar::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible())
	{		
		if (SCROLLBAR_HOR == m_enType)
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR_HOR);
			if (NULL == poImage)
			{
				return;
			}
			rcItem.left += 1;
			rcItem.right -= 1;
			
			rcItem.bottom -= 1;
			rcItem.top = rcItem.bottom - poImage->GetHeight() / 3;
			SetRect(rcItem);
			_DrawHor(poSurface,hDC);
		}
		else
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR);
			if (NULL == poImage)
			{
				return;
			}
			rcItem.left = rcItem.right - 1 - poImage->GetWidth() / 3;
			rcItem.right -= 1;
			rcItem.top += 1;
			rcItem.bottom -= 1;
			SetRect(rcItem);
			_DrawVer(poSurface,hDC);
		}
	}	
}

VOID CRXSkinScrollbar::ReleaseControl()
{
	delete this;
}

VOID CRXSkinScrollbar::SetTotalHeight(const UINT32 dwTotalHeight,const UINT32 dwHeight)
{
	m_dwTotalHeight = dwTotalHeight;
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR);
	if (poImage)
	{
		m_dwLessHeight = m_dwTotalHeight - dwHeight;
		m_nItemHeight = poImage->GetHeight() / 4;
		m_nItemWidth = poImage->GetWidth() / 3;
		INT32 nTrueHeight = dwHeight - 3 * m_nItemHeight;
		if (m_dwLessHeight<(UINT32)nTrueHeight)
		{
			m_nScrollBarHeight = nTrueHeight - m_dwLessHeight;
		}
		else
		{
			m_nScrollBarHeight = m_nItemHeight;
		}
		m_nScrollPos = m_nItemHeight;
	}

	m_oVerBottom.SetParentWnd(GetParentWnd());
	m_oVerTop.SetParentWnd(GetParentWnd());
	m_oVerTrack.SetParentWnd(GetParentWnd());
	m_oHorRight.SetParentWnd(GetParentWnd());
	m_oHorLeft.SetParentWnd(GetParentWnd());
	m_oHorTrack.SetParentWnd(GetParentWnd());
}

VOID CRXSkinScrollbar::SetTotalWidth(const UINT32 dwTotalWidth, const UINT32 dwWidth)
{
	m_dwTotalWidth = dwTotalWidth;
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR_HOR);
	if (poImage)
	{
		m_dwLessWidth = m_dwTotalWidth - dwWidth;
		m_nItemHeight = poImage->GetHeight() / 3;
		m_nItemWidth = poImage->GetWidth() / 4;
		INT32 nTrueWidth = dwWidth - 3 * m_nItemWidth;
		if (m_dwLessWidth < (UINT32)nTrueWidth)
		{
			m_nScrollBarWidth = nTrueWidth - m_dwLessWidth;
		}
		else
		{
			m_nScrollBarWidth = m_nItemWidth;
		}
		m_nScrollPos = m_nItemWidth;
	}

	m_oVerBottom.SetParentWnd(GetParentWnd());
	m_oVerTop.SetParentWnd(GetParentWnd());
	m_oVerTrack.SetParentWnd(GetParentWnd());
	m_oHorRight.SetParentWnd(GetParentWnd());
	m_oHorLeft.SetParentWnd(GetParentWnd());
	m_oHorTrack.SetParentWnd(GetParentWnd());
}

BOOL CRXSkinScrollbar::PtInRect(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}

BOOL CRXSkinScrollbar::OnClick(POINT pt)
{
	if (IsVisible() == FALSE) return FALSE;
	if (m_bPressed)
	{
		m_bPressed = FALSE;
		return TRUE;
	}
	if (PtInRect(pt))
	{
		CRXSkinScrollbarItem* poItem = (CRXSkinScrollbarItem*)_HitTest(pt);
		if (poItem)
		{
			if (poItem == &m_oVerBottom)
			{
				if (m_nScrollPos + m_nScrollBarHeight + SCROLLBAR_STEP <= 
					GetRect().Height() - m_nItemHeight)
				{
					m_nScrollPos += SCROLLBAR_STEP;
					RECT rc = GetInvalidateRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
				else
				{
					m_nScrollPos = GetRect().Height() - m_nItemHeight - m_nScrollBarHeight;
					RECT rc = GetInvalidateRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
			}
			else if (poItem == &m_oVerTop)
			{
				if (m_nScrollPos - SCROLLBAR_STEP>=m_nItemHeight)
				{
					m_nScrollPos -= SCROLLBAR_STEP;
					RECT rc = GetInvalidateRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
				else
				{
					m_nScrollPos = m_nItemHeight;
					RECT rc = GetInvalidateRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
			}
			else if (poItem == &m_oHorRight)
			{
				if (m_nScrollPos + m_nScrollBarWidth + SCROLLBAR_STEP <=
					GetRect().Width() - m_nItemWidth)
				{
					m_nScrollPos += SCROLLBAR_STEP;
					RECT rc = GetInvalidateRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
				else
				{
					m_nScrollPos = GetRect().Width() - m_nItemWidth - m_nScrollBarWidth;
					RECT rc = GetInvalidateRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
			}
			else if (poItem == &m_oHorLeft)
			{
				if (m_nScrollPos - SCROLLBAR_STEP >= m_nItemWidth)
				{
					m_nScrollPos -= SCROLLBAR_STEP;
					RECT rc = GetRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
				else
				{
					m_nScrollPos = m_nItemWidth;
					RECT rc = GetRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
			}
		}
		else
		{
			if (SCROLLBAR_VER == m_enType)
			{
				if (pt.y+ m_nScrollBarHeight<=GetRect().bottom - m_nItemHeight)
				{
					m_nScrollPos = pt.y - GetRect().top;
					RECT rc = GetRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
				else
				{
					m_nScrollPos = GetRect().Height() - m_nItemHeight - m_nScrollBarHeight;
					RECT rc = GetRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
			}
			else
			{
				if (pt.x + m_nScrollBarWidth <= GetRect().right - m_nItemWidth)
				{
					m_nScrollPos = pt.x - GetRect().left;
					RECT rc = GetRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
				else
				{
					m_nScrollPos = GetRect().Width() - m_nItemWidth - m_nScrollBarWidth;
					RECT rc = GetRect();
					InvalidateRect(GetParentWnd(), &rc, FALSE);
					_NotifyScrollChanged();
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

VOID CRXSkinScrollbar::_NotifyScrollChanged()
{
	if (m_enType ==  SCROLLBAR_VER)
	{
		INT32 nTop = (INT32)(m_nScrollPos - m_nItemHeight);
		INT32 nTotalHeight = GetRect().Height() - 2 * m_nItemHeight - m_nScrollBarHeight;
		m_fPercent = (float)(((double)nTop / (double)nTotalHeight));
		INT32 nPercent = (INT32)(m_fPercent * 100.00);
		if (m_poOwner)
		{
			m_poOwner->OnScrollBarChanged(m_enType, nPercent);
		}
		else
			::SendMessage(GetParentWnd(), WM_RXSKIN_SCROLLBAR_POS, m_enType, nPercent);
	}
	else
	{
		INT32 nLeft = (INT32)(m_nScrollPos - m_nItemWidth);
		INT32 nTotalWidth = GetRect().Width() - 2 * m_nItemWidth - m_nScrollBarWidth;
		m_fPercent = (float)(((double)nLeft / (double)nTotalWidth));
		INT32 nPercent = (INT32)(m_fPercent * 100.00);
		if (m_poOwner)
		{
			m_poOwner->OnScrollBarChanged(m_enType, nPercent);
		}
		else
			::SendMessage(GetParentWnd(), WM_RXSKIN_SCROLLBAR_POS, m_enType, nPercent);
	}
}

BOOL CRXSkinScrollbar::OnMouseMove(POINT pt)
{
	if (IsVisible() == FALSE) return FALSE;
	if (PtInRect(pt))
	{
		if (m_enType == SCROLLBAR_VER)
		{
			if (m_bPressed)
			{
				INT32 nScrollbarPos = pt.y - m_nTrackOffSet;
				if (nScrollbarPos <= m_nItemHeight)
				{
					nScrollbarPos = m_nItemHeight;
				}
				else if (nScrollbarPos >= GetRect().Height() - m_nItemHeight - m_nScrollBarHeight)
				{
					nScrollbarPos = GetRect().Height() - m_nItemHeight - m_nScrollBarHeight;
				}
				m_nScrollPos = nScrollbarPos;
				RECT rc = GetRect();
				InvalidateRect(GetParentWnd(), &rc, FALSE);
				_NotifyScrollChanged();
				return TRUE;
			}
			CRXSkinItem* poItem = _HitTest(pt);
			if (poItem)
			{
				if (m_poMouseOn == NULL)
				{
					m_poMouseOn = poItem;
					m_poMouseOn->SetState(ITEM_MOUSEON);
					m_poMouseOn->Invalidate();
				}
				else if (m_poMouseOn != poItem)
				{
					m_poMouseOn->SetState(ITEM_NORMAL);
					m_poMouseOn->Invalidate();
					m_poMouseOn = poItem;
					m_poMouseOn->SetState(ITEM_MOUSEON);
					m_poMouseOn->Invalidate();
				}

				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_HAND);

			}
			else if (CONTROL_STATE_NORMAL == GetState())
			{
				if (m_poMouseOn)
				{
					m_poMouseOn->SetState(ITEM_NORMAL);
					m_poMouseOn->Invalidate();
					m_poMouseOn = poItem;
				}
				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);

				SetState(CONTROL_STATE_MOUSEON);
				RECT rc = GetRect();
				InvalidateRect(GetParentWnd(), &rc, FALSE);
			}
			return TRUE;
		}
		else
		{
			if (m_bPressed)
			{
				INT32 nScrollbarPos = pt.x - m_nTrackOffSet;
				if (nScrollbarPos <= m_nItemWidth)
				{
					nScrollbarPos = m_nItemWidth;
				}
				else if (nScrollbarPos >= GetRect().Width() - m_nItemWidth - m_nScrollBarWidth)
				{
					nScrollbarPos = GetRect().Width() - m_nItemWidth - m_nScrollBarWidth;
				}
				m_nScrollPos = nScrollbarPos;
				RECT rc = GetRect();
				InvalidateRect(GetParentWnd(), &rc, FALSE);
				_NotifyScrollChanged();
				return TRUE;
			}
			CRXSkinItem* poItem = _HitTest(pt);
			if (poItem)
			{
				if (m_poMouseOn == NULL)
				{
					m_poMouseOn = poItem;
					m_poMouseOn->SetState(ITEM_MOUSEON);
					m_poMouseOn->Invalidate();
				}
				else if (m_poMouseOn != poItem)
				{
					m_poMouseOn->SetState(ITEM_NORMAL);
					m_poMouseOn->Invalidate();
					m_poMouseOn = poItem;
					m_poMouseOn->SetState(ITEM_MOUSEON);
					m_poMouseOn->Invalidate();
				}

				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_HAND);

			}
			else if (CONTROL_STATE_NORMAL == GetState())
			{
				if (m_poMouseOn)
				{
					m_poMouseOn->SetState(ITEM_NORMAL);
					m_poMouseOn->Invalidate();
					m_poMouseOn = poItem;
				}
				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);

				SetState(CONTROL_STATE_MOUSEON);
				RECT rc = GetRect();
				InvalidateRect(GetParentWnd(), &rc, FALSE);
			}
			return TRUE;
		}
	}
	else
	{
		
		if (CONTROL_STATE_MOUSEON == GetState())
		{
			SetState(CONTROL_STATE_NORMAL);
			RECT rc = GetRect();
			InvalidateRect(GetParentWnd(), &rc, FALSE);
		}
	}
	return FALSE;
}
BOOL CRXSkinScrollbar::OnLButtonDown(POINT pt)
{
	if (IsVisible() == FALSE) return FALSE;
	if (PtInRect(pt))
	{
		if (m_enType == SCROLLBAR_VER)
		{
			CRXSkinScrollbarItem* poItem = (CRXSkinScrollbarItem*)_HitTest(pt);
			if (poItem)
			{
				if (poItem == &m_oVerTrack)
				{
					m_bPressed = TRUE;
					m_nTrackOffSet = (INT32)(pt.y - m_nScrollPos);
				}

				if (m_poPressed == NULL)
				{
					m_poPressed = poItem;
					m_poPressed->SetState(ITEM_PRESSED);
					m_poPressed->Invalidate();
				}
				else if (m_poPressed != poItem)
				{
					m_poPressed->SetState(ITEM_NORMAL);
					m_poPressed->Invalidate();
					m_poPressed = poItem;
					m_poPressed->SetState(ITEM_PRESSED);
					m_poPressed->Invalidate();
				}
			}
			else
			{
				if (m_poPressed)
				{
					m_poPressed->SetState(ITEM_NORMAL);
					m_poPressed->Invalidate();
					m_poPressed = poItem;
				}
			}
			return TRUE;
		}
		else
		{
			CRXSkinScrollbarItem* poItem = (CRXSkinScrollbarItem*)_HitTest(pt);
			if (poItem)
			{
				if (poItem == &m_oVerTrack)
				{
					m_bPressed = TRUE;
					m_nTrackOffSet = (INT32)(pt.x - m_nScrollPos);
				}

				if (m_poPressed == NULL)
				{
					m_poPressed = poItem;
					m_poPressed->SetState(ITEM_PRESSED);
					m_poPressed->Invalidate();
				}
				else if (m_poPressed != poItem)
				{
					m_poPressed->SetState(ITEM_NORMAL);
					m_poPressed->Invalidate();
					m_poPressed = poItem;
					m_poPressed->SetState(ITEM_PRESSED);
					m_poPressed->Invalidate();
				}
			}
			else
			{
				if (m_poPressed)
				{
					m_poPressed->SetState(ITEM_NORMAL);
					m_poPressed->Invalidate();
					m_poPressed = poItem;
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXSkinScrollbar::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	if (IsVisible() == FALSE)
	{
		return FALSE;
	}
	if (m_enType == SCROLLBAR_HOR)
	{
		INT32 nTotalWidth = GetRect().Width() - 2 * m_nItemWidth - m_nScrollBarWidth;
		INT32 nLeft = (INT32)(m_fPercent * nTotalWidth);
		m_nScrollPos =(INT64)( nLeft + m_nItemWidth);
		_NotifyScrollChanged();
	}
	else
	{
		INT32 nTotalHeight = GetRect().Height() - 2 * m_nItemHeight - m_nScrollBarHeight;
		INT32 nTop = (INT32)(m_fPercent * nTotalHeight);
		m_nScrollPos = (INT64)(nTop + m_nItemHeight);
		_NotifyScrollChanged();
	}
	return TRUE;
}

CRXSkinItem* CRXSkinScrollbar::_HitTest(POINT pt)
{
	CRXSkinItem* poItem = NULL;
	CRXSkinScrollbarItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		poItem = itr->second->HitTest(pt);
		if (poItem)
		{
			return poItem;
		}
	}
	return poItem;
}


VOID CRXSkinScrollbar::_DrawHor(SkSurface* poSurface, HDC hDC)
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR_HOR);
	if (NULL == poImage)
	{
		return;
	}
	CRXSkinRect rcItem = GetRect();

	INT32 nHeight = poImage->GetHeight() / 3;
	INT32 nWidth = poImage->GetWidth() / 4;
	CRXSkinRect rcLeft = rcItem;
	rcLeft.right = rcLeft.left + nWidth;
	m_oHorLeft.OnDraw(poSurface,hDC, rcLeft);

	switch (GetState())
	{
	case CONTROL_STATE_NORMAL:
	{
		CRXSkinRect rcMiddle = rcItem;
		// 绘制头部轨道
		UINT32 nMiddleWidth = nWidth / 3;
		rcMiddle.left = rcItem.left + nWidth;
		rcMiddle.right = rcMiddle.left + nMiddleWidth;
		poImage->Draw(hDC, rcMiddle, 2*nWidth, 0);

		//  轨道中部
		rcMiddle.left = rcMiddle.right;
		INT32 nCount = (rcItem.Width() - 2 * nMiddleWidth) / nMiddleWidth;
		for (INT32 n = 0; n < nCount; n++)
		{
			poImage->Draw(hDC, rcMiddle, 2 * nWidth + nMiddleWidth, 0);
			rcMiddle.left = rcMiddle.right;
			rcMiddle.right = rcMiddle.left + nMiddleWidth;
		}
		if ((rcItem.Width() - 2 * nMiddleWidth) % nMiddleWidth)
		{
			rcMiddle.left = rcItem.right - 2 * nMiddleWidth;
			rcMiddle.right = rcMiddle.left + nMiddleWidth;
			poImage->Draw(hDC, rcMiddle, 2 * nWidth+2*nMiddleWidth,0);
		}

		//  轨道底部
		rcMiddle = rcItem;
		rcMiddle.left = rcMiddle.right - nMiddleWidth;
		poImage->Draw(hDC, rcMiddle, 2 * nWidth + 2 * nMiddleWidth, 0);
	}
	break;
	case CONTROL_STATE_MOUSEON:
	{
		CRXSkinRect rcMiddle = rcItem;
		// 绘制头部轨道
		UINT32 nMiddleWidth = nWidth / 3;
		rcMiddle.left = rcItem.left + nWidth;
		rcMiddle.right = rcMiddle.left + nMiddleWidth;
		poImage->Draw(hDC, rcMiddle, 2 * nWidth, nHeight);

		//  轨道中部
		rcMiddle.left = rcMiddle.right;
		INT32 nCount = (rcItem.Width() - 2 * nMiddleWidth) / nMiddleWidth;
		for (INT32 n = 0; n < nCount; n++)
		{
			poImage->Draw(hDC, rcMiddle, 2 * nWidth + nMiddleWidth, nHeight);
			rcMiddle.left = rcMiddle.right;
			rcMiddle.right = rcMiddle.left + nMiddleWidth;
		}
		if ((rcItem.Width() - 2 * nMiddleWidth) % nMiddleWidth)
		{
			rcMiddle.left = rcItem.right - 2 * nMiddleWidth;
			rcMiddle.right = rcMiddle.left + nMiddleWidth;
			poImage->Draw(hDC, rcMiddle, 2 * nWidth + 2 * nMiddleWidth, nHeight);
		}

		//  轨道底部
		rcMiddle = rcItem;
		rcMiddle.left = rcMiddle.right - nMiddleWidth;
		poImage->Draw(hDC, rcMiddle, 2 * nWidth + 2 * nMiddleWidth, nHeight);
	}
	break;
	case CONTROL_STATE_PRESSED:
	{
		CRXSkinRect rcMiddle = rcItem;
		// 绘制头部轨道
		UINT32 nMiddleWidth = nWidth / 3;
		rcMiddle.left = rcItem.left + nWidth;
		rcMiddle.right = rcMiddle.left + nMiddleWidth;
		poImage->Draw(hDC, rcMiddle, 2 * nWidth,2 * nHeight);

		//  轨道中部
		rcMiddle.left = rcMiddle.right;
		INT32 nCount = (rcItem.Width() - 2 * nMiddleWidth) / nMiddleWidth;
		for (INT32 n = 0; n < nCount; n++)
		{
			poImage->Draw(hDC, rcMiddle, 2 * nWidth + nMiddleWidth, 2 * nHeight);
			rcMiddle.left = rcMiddle.right;
			rcMiddle.right = rcMiddle.left + nMiddleWidth;
		}
		if ((rcItem.Width() - 2 * nMiddleWidth) % nMiddleWidth)
		{
			rcMiddle.left = rcItem.right - 2 * nMiddleWidth;
			rcMiddle.right = rcMiddle.left + nMiddleWidth;
			poImage->Draw(hDC, rcMiddle, 2 * nWidth + 2 * nMiddleWidth, 2 * nHeight);
		}

		//  轨道底部
		rcMiddle = rcItem;
		rcMiddle.left = rcMiddle.right - nMiddleWidth;
		poImage->Draw(hDC, rcMiddle, 2 * nWidth + 2 * nMiddleWidth, 2 * nHeight);
	}
	break;
	default:
		break;
	}
	CRXSkinRect rcRight = rcItem;
	rcRight.left = rcRight.right - nHeight;
	m_oHorRight.OnDraw(poSurface,hDC, rcRight);
	CRXSkinRect rcTrack = rcItem;
	rcTrack.left += (long)m_nScrollPos;
	rcTrack.right = rcTrack.left + m_nScrollBarWidth;
	m_oHorTrack.OnDraw(poSurface,hDC, rcTrack);
}

bool CRXSkinScrollbar::GetShow()
{
	return m_bShow;
}

void CRXSkinScrollbar::SetShow(bool bShow)
{
	m_bShow = bShow;
	Invalidate();
}

int CRXSkinScrollbar::GetScrollRange() const
{
	return (int)m_nRange;
}

void CRXSkinScrollbar::SetScrollPos(int nPos)
{
	if (m_nScrollPos == nPos) return;

	m_nScrollPos = nPos;
	if (m_nScrollPos < 0) m_nScrollPos = 0;
	if (m_nScrollPos > m_nRange) m_nScrollPos = m_nRange;
//	SetPos(m_rcItem);
}


void CRXSkinScrollbar::SetScrollRange(int nRange)
{
	if (m_nRange == nRange) return;

	m_nRange = nRange;
	if (m_nRange < 0) m_nRange = 0;
	if (m_nScrollPos > m_nRange) m_nScrollPos = m_nRange;
	//SetPos(m_rcItem);
}

void CRXSkinScrollbar::PushEnd()
{
	if (m_enType == SCROLLBAR_VER)
	{		
		INT32 nTotalHeight = GetRect().Height() - 2 * m_nItemHeight - m_nScrollBarHeight;
		m_nScrollPos = m_nItemHeight + nTotalHeight;
	}
	else
	{
		INT32 nTotalWidth = GetRect().Width() - 2 * m_nItemWidth - m_nScrollBarWidth;
		m_nScrollPos = m_nItemWidth + nTotalWidth;
	}
	Invalidate();
	_NotifyScrollChanged();
}

void CRXSkinScrollbar::PushStart()
{
	if (m_enType == SCROLLBAR_VER)
	{
		m_nScrollPos = m_nItemHeight;
	}
	else
		m_nScrollPos = m_nItemWidth;
	Invalidate();
	_NotifyScrollChanged();
}

VOID CRXSkinScrollbar::_DrawVer(SkSurface* poSurface, HDC hDC)
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR); 
	if (NULL == poImage)
	{
		return;
	}
	CRXSkinRect rcItem = GetRect();

	INT32 nHeight = poImage->GetHeight() / 4;
	INT32 nWidth = poImage->GetWidth() / 3;
	CRXSkinRect rcTop = rcItem;
	rcTop.bottom = rcTop.top + nHeight;
	m_oVerTop.OnDraw(poSurface,hDC,rcTop);


	switch(GetState())
	{
	case CONTROL_STATE_NORMAL:
		{
			CRXSkinRect rcMiddle = rcItem;
			// 绘制头部轨道
			UINT32 nMiddleHeight = nHeight / 3;
			rcMiddle.top = rcItem.top + nHeight;
			rcMiddle.bottom = rcMiddle.top + nMiddleHeight;
			poImage->Draw(hDC, rcMiddle, 0, nHeight);

			//  轨道中部
			rcMiddle.top = rcMiddle.bottom;
			INT32 nCount = (rcItem.Height() - 2 * nMiddleHeight)/ nMiddleHeight;
			for (INT32 n = 0; n < nCount;n ++)
			{
				poImage->Draw(hDC,rcMiddle, 0, nMiddleHeight+nHeight);
				rcMiddle.top = rcMiddle.bottom;
				rcMiddle.bottom = rcMiddle.top + nMiddleHeight;
			}
			if ((rcItem.Height() - 2 * nMiddleHeight)% nMiddleHeight)
			{
				rcMiddle.top = rcItem.bottom - 2 * nMiddleHeight;
				rcMiddle.bottom = rcMiddle.top + nMiddleHeight;
				poImage->Draw(hDC,rcMiddle, 0, nMiddleHeight);
			}

			//  轨道底部
			rcMiddle = rcItem;
			rcMiddle.top = rcMiddle.bottom - nMiddleHeight;
			poImage->Draw(hDC, rcMiddle, 0, 2 * nMiddleHeight);
		}
		break;
	case CONTROL_STATE_MOUSEON:
		{
			CRXSkinRect rcMiddle = rcItem;
			// 绘制头部轨道
			UINT32 nMiddleHeight = nHeight / 3;
			rcMiddle.top = rcItem.top + nHeight;
			rcMiddle.bottom = rcMiddle.top + nMiddleHeight;
			poImage->Draw(hDC, rcMiddle, 3 * nWidth, nHeight);

			//  轨道中部
			rcMiddle.top = rcMiddle.bottom;
			INT32 nCount = (rcItem.Height() - 2 * nMiddleHeight) / nMiddleHeight;
			for (INT32 n = 0; n < nCount; n++)
			{
				poImage->Draw(hDC, rcMiddle, 3 * nWidth, nHeight + nMiddleHeight);
				rcMiddle.top = rcMiddle.bottom;
				rcMiddle.bottom = rcMiddle.top + nMiddleHeight;
			}
			if ((rcItem.Height() - 2 * nMiddleHeight) % nMiddleHeight)
			{
				rcMiddle.top = rcItem.bottom - 2 * nMiddleHeight;
				rcMiddle.bottom = rcMiddle.top + nMiddleHeight;
				poImage->Draw(hDC, rcMiddle, 3 * nWidth, nHeight + nMiddleHeight);
			}

			//  轨道底部
			rcMiddle = rcItem;
			rcMiddle.top = rcMiddle.bottom - nMiddleHeight;
			poImage->Draw(hDC, rcMiddle, 3 * nWidth, nHeight + 2 * nMiddleHeight);
		}
		break;
	case CONTROL_STATE_PRESSED:
		{
			CRXSkinRect rcMiddle = rcItem;
			// 绘制头部轨道
			UINT32 nMiddleHeight = nHeight / 3;
			rcMiddle.top = rcItem.top + nHeight;
			rcMiddle.bottom = rcMiddle.top + nMiddleHeight;
			poImage->Draw(hDC, rcMiddle, 3 * nWidth,2 * nHeight);

			//  轨道中部
			rcMiddle.top = rcMiddle.bottom;
			INT32 nCount = (rcItem.Height() - 2 * nMiddleHeight) / nMiddleHeight;
			for (INT32 n = 0; n < nCount; n++)
			{
				poImage->Draw(hDC, rcMiddle, 3 * nWidth, 2 * nHeight + nMiddleHeight);
				rcMiddle.top = rcMiddle.bottom;
				rcMiddle.bottom = rcMiddle.top + nMiddleHeight;
			}
			if ((rcItem.Height() - 2 * nMiddleHeight) % nMiddleHeight)
			{
				rcMiddle.top = rcItem.bottom - 2 * nMiddleHeight;
				rcMiddle.bottom = rcMiddle.top + nMiddleHeight;
				poImage->Draw(hDC, rcMiddle, 3 * nWidth, 2 * nHeight + nMiddleHeight);
			}

			//  轨道底部
			rcMiddle = rcItem;
			rcMiddle.top = rcMiddle.bottom - nMiddleHeight;
			poImage->Draw(hDC, rcMiddle, 3 * nWidth, 2 * nHeight + 2 * nMiddleHeight);
		}
		break;
	default:
		break;
	}
	CRXSkinRect rcBottom = rcItem;
	rcBottom.top = rcBottom.bottom - nHeight;
	m_oVerBottom.OnDraw(poSurface,hDC, rcBottom);
	CRXSkinRect rcTrack = rcItem;
	rcTrack.top += (long)m_nScrollPos;
	rcTrack.bottom = rcTrack.top + m_nScrollBarHeight;
	m_oVerTrack.OnDraw(poSurface, hDC,rcTrack);
}

UINT32 CRXSkinScrollbar::GetWidth()
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR);
	if (NULL == poImage)
	{
		return 0;
	}
	if (m_enType == SCROLLBAR_VER)
	{
		return poImage->GetWidth() / 3;
	}
	return 0;
}

UINT32 CRXSkinScrollbar::GetHeight()
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SCROLLBAR_HOR);
	if (NULL == poImage)
	{
		return 0;
	}
	if (m_enType == SCROLLBAR_HOR)
	{
		return poImage->GetHeight() / 3;
	}
	return 0;
}

VOID CRXSkinScrollbar::SetType(EScrollType enType)
{
	m_enType = enType;
}

VOID CRXSkinScrollbar::Reset()
{
	m_bPressed = FALSE;
	m_poPressed = m_poMouseOn = NULL;
}

VOID CRXSkinScrollbar::Step(INT32 nStep)
{
	if (m_enType == SCROLLBAR_VER)
	{
		INT32 nScrollbarPos = (INT32)(m_nScrollPos + nStep);
		if (m_nItemHeight >= nScrollbarPos)
		{
			nScrollbarPos = m_nItemHeight;
		}
		else if (GetRect().Height() - m_nItemHeight - m_nScrollBarHeight <= nScrollbarPos)
		{
			nScrollbarPos = GetRect().Height() - m_nItemHeight - m_nScrollBarHeight;
		}
		m_nScrollPos = nScrollbarPos;
		_NotifyScrollChanged();
	}
	else
	{
		INT32 nScrollbarPos = (INT32)(m_nScrollPos + nStep);
		if (m_nItemWidth >= nScrollbarPos)
		{
			nScrollbarPos = m_nItemWidth;
		}
		else if (GetRect().Width() - m_nItemWidth - m_nScrollBarWidth <= nScrollbarPos)
		{
			nScrollbarPos = GetRect().Width() - m_nItemWidth - m_nScrollBarWidth;
		}
		m_nScrollPos = nScrollbarPos;
		_NotifyScrollChanged();
	}
}

BOOL CRXSkinScrollbar::OnMouseWheel(UINT nFlags, short zDelta,POINT pt)
{
	if (IsVisible() == FALSE) return FALSE;
	if (zDelta < 0)
	{
		Step(m_nStep);
	}
	else
	{
		Step(-m_nStep);
	}
	return TRUE;
}