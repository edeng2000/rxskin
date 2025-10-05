#include "pch.h"

#include "RXSkin.h"
#include "common/rximage.h"
#include "common/graphics.h"
#include "common/RXSkinService.h"
#include "common/rxskindefines.h"
#include "common/rxskinutil.h"
#include "common/RXSkinMemDC.h"
#include "controls/RXSkinComboBox.h"
#include <olectl.h>
#include "windowsx.h"
#include "commctrl.h"
#include "WinUser.h"
#define WND_COMBOBOX_POPUP      _T("RXSkinComboBoxPopup")

CRXSkinComboBoxPopupItem::CRXSkinComboBoxPopupItem()
{
	m_poComboBox = NULL;
	m_nIconNumber = -1;
}

CRXSkinComboBoxPopupItem::~CRXSkinComboBoxPopupItem()
{

}

VOID CRXSkinComboBoxPopupItem::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (m_poComboBox)
	{
		CRXSkinComboBox::SItemData* poItem = m_poComboBox->FindItem((uint32_t)GetID());
		if (poItem)
		{
			COLORREF clrOld = RGB(30,40,50);
			switch(GetState())
			{
			case ITEM_NORMAL:
				{
					clrOld = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupItemNormalTextColor();
				}
				break;
			case ITEM_MOUSEON:
				{
					clrOld = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupItemMouseOnTextColor();
					DrawRect(hDC,
						CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupItemMouseOnBkColor(),
						rcItem.left,
						rcItem.top,
						rcItem.right,
						rcItem.bottom);
				}
				break;
			case ITEM_PRESSED:
				{
					clrOld = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupItemPressedTextColor();
					DrawRect(hDC,
						CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupItemPressedBkColor(),
						rcItem.left,
						rcItem.top,
						rcItem.right,
						rcItem.bottom);
				}
				break;
			case ITEM_SELECTED:
				{
					clrOld = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupItemSelectedTextColor();
					DrawRect(hDC,
						CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupItemSelectedBkColor(),
						rcItem.left,
						rcItem.top,
						rcItem.right,
						rcItem.bottom);
				}
				break;
			default:
				break;
			}

			if (m_poComboBox->GetComboBoxType() == COMBOBOX_DROPLIST_FILE)
			{
				if (m_poComboBox->GetImageList())
				{
					int cx = m_poComboBox->GetImageList()->GetX();
					int cy = m_poComboBox->GetImageList()->GetY();
					CRXSkinRect rcTxt = rcItem;
					rcTxt.left = rcTxt.left + CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupOffSetX();
					RGBQUAD tran;
					tran.rgbBlue = 0;
					tran.rgbGreen = 0;
					tran.rgbRed = 0;
					tran.rgbReserved = 0;
					if (m_nIconNumber == -1)
					{
						m_nIconNumber = CRXSkinService::Instance()->GetDrawMgr().GetShellImageListIndex(poItem->m_szValue);
					}
					m_poComboBox->GetImageList()->GetImage()->DrawTrans(hDC,
						tran,
						rcTxt.left,
						rcTxt.top + (rcTxt.Height() - cy) / 2,
						cx,cy,
						0,
						m_nIconNumber * cy);

					SIZE szTxt;
					tstring strTxt = poItem->m_szValue;
					CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, poItem->m_szValue, szTxt);

					INT32 nLeft = rcItem.left + CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupOffSetX() * 2 + cx;
					INT32 nTop = rcItem.top + (rcItem.Height() - szTxt.cy) / 2;
					rcTxt = rcItem;
					rcTxt.left = nLeft;
					SkCanvas* poCanvas = poSurface->getCanvas();
					if (poCanvas)
					{
						CRXSkinFontMgr::Instance()->DrawText(poCanvas, 
							strTxt, rcTxt,
							clrOld,
							ID_FONT_COMMON,
							DT_END_ELLIPSIS| DT_VCENTER | DT_SINGLELINE | DT_LEFT);
					}			
				}
			}
			else
			{
				SIZE szTxt;
				tstring strTxt = poItem->m_szValue;
				if (m_poComboBox->GetComboBoxType() == COMBOBOX_DROPDOWN_EXT)
				{
					strTxt = poItem->m_szValueEx;
				}
				CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, strTxt, szTxt);

				INT32 nLeft = rcItem.left + CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupOffSetX();
				INT32 nTop = rcItem.top + (rcItem.Height() - szTxt.cy) / 2;
				CRXSkinRect rcTxt = rcItem;
				rcTxt.left = nLeft;
				SkCanvas* poCanvas = poSurface->getCanvas();
				if (poCanvas)
				{
					CRXSkinFontMgr::Instance()->DrawText(poCanvas,
						strTxt, rcTxt,
						clrOld,
						ID_FONT_COMMON, DT_END_ELLIPSIS|DT_VCENTER | DT_SINGLELINE | DT_LEFT);
				}				
			}
		}
	}
}

VOID CRXSkinComboBoxPopupItem::SetComboBox(CRXSkinComboBox* poComboBox)
{
	m_poComboBox = poComboBox;
}

CRXSkinItem* CRXSkinComboBoxPopupItem::HitTest(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return this;
	}
	return NULL;
}

CRXSkinComboBoxPopup::CRXSkinComboBoxPopup()
{
	m_hParentWnd = NULL;
	m_hCaptureWnd = NULL;
	m_poComboBox = NULL;
	m_poMouseOn = NULL;
	m_poPressed = NULL;
	m_OldWndProc = ::DefWindowProc;
	m_oScrollbar.SetVisible(FALSE);
	m_oScrollbar.SetType(SCROLLBAR_VER);
	m_nTotalHeight = 0;
	m_nTopOffSet = 0;
}

CRXSkinComboBoxPopup::~CRXSkinComboBoxPopup()
{
	_ResetContent();
}

VOID CRXSkinComboBoxPopup::SetCaptureWnd(HWND hCpatureWnd)
{
	m_hCaptureWnd = hCpatureWnd;
}

HWND CRXSkinComboBoxPopup::GetHWnd()
{
	return m_hWnd;
}

VOID CRXSkinComboBoxPopup::SetComboBox(CRXSkinComboBox* poComboBox)
{
	m_poComboBox = poComboBox;
}

VOID CRXSkinComboBoxPopup::ReleaseControl()
{
	delete this;
}

VOID CRXSkinComboBoxPopup::OnPaint()
{
	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	PAINTSTRUCT ps = { 0 };
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	CRXSkinMemDC memDC(hDC,rcClient);

	DrawRect(memDC.GetHDC(),
		CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupColorBorder(),
		CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupColorBK(),
		rcClient.left,rcClient.top,rcClient.right,rcClient.bottom
		);
	
	RECT rcItem = rcClient;
	rcItem.top -= m_nTopOffSet;
	rcItem.top += CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupOffSetY();
	rcItem.left += CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupOffSetX();
	rcItem.right -= m_oScrollbar.GetWidth();
	CRXSkinComboBoxPopupItemMapItr itr = m_oItemMap.begin();
	for (;itr!=m_oItemMap.end();itr++)
	{
		rcItem.bottom = rcItem.top + CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupItemHeight();
		rcItem.bottom += CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupOffSetY();
		itr->second->OnDraw(memDC.GetSurface(),memDC.GetHDC(),rcItem);
		rcItem.top = rcItem.bottom;
	}
	m_oScrollbar.OnDraw(memDC.GetSurface(), memDC.GetHDC(),rcClient);
	memDC.TakeSnapshot();
	EndPaint(m_hWnd,&ps);
}

BOOL CRXSkinComboBoxPopup::OnMessage(UINT32 dwMessageID, WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinWnd::OnMessage(dwMessageID,wParam,lParam);
	if (FALSE == bResult)
	{
	}
	return bResult;
}

BOOL CRXSkinComboBoxPopup::OnMouseWheel(UINT nFlags, short zDelta,POINT pt)
{
	return m_oScrollbar.OnMouseWheel(nFlags,zDelta,pt);
}

BOOL CRXSkinComboBoxPopup::OnMouseMove(UINT nFlags, POINT point)
{
	if (m_oScrollbar.OnMouseMove(point))
	{
		return TRUE;
	}
	CRXSkinComboBoxPopupItem* poItem = _HitTest(point);
	if (poItem)
	{
		if (m_poMouseOn == NULL)
		{
			m_poMouseOn = poItem;
			m_poMouseOn->SetState(ITEM_MOUSEON);
			m_poMouseOn->Invalidate();
		}
		else if (m_poMouseOn!=poItem)
		{
			m_poMouseOn->SetState(ITEM_NORMAL);
			m_poMouseOn->Invalidate();
			m_poMouseOn = poItem;
			m_poMouseOn->SetState(ITEM_MOUSEON);
			m_poMouseOn->Invalidate();
		}
		CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_HAND);
		return TRUE;
	}
	else
	{
		if (m_poMouseOn)
		{
			m_poMouseOn->SetState(ITEM_NORMAL);
			m_poMouseOn->Invalidate();
			m_poMouseOn = poItem;
		}
		CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);
	}
	return FALSE;
}

BOOL CRXSkinComboBoxPopup::OnLButtonDown(UINT nFlags, POINT point)
{
	if (m_oScrollbar.OnLButtonDown(point))
	{
		return TRUE;
	}
	CRXSkinComboBoxPopupItem* poItem = _HitTest(point);
	if (poItem)
	{
		if (m_poPressed == NULL)
		{
			m_poPressed = poItem;
			m_poPressed->SetState(ITEM_PRESSED);
			m_poPressed->Invalidate();
		}
		else if (m_poPressed!=poItem)
		{
			m_poPressed->SetState(ITEM_NORMAL);
			m_poPressed->Invalidate();
			m_poPressed = poItem;
			m_poPressed->SetState(ITEM_PRESSED);
			m_poPressed->Invalidate();
		}
		return TRUE;
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
	return FALSE;
}


BOOL CRXSkinComboBoxPopup::OnLButtonUp(UINT nFlags, POINT point)
{
	CRXSkinRect rcWin;
	RECT rc;
	GetClientRect(m_hWnd,&rc);
	rcWin = rc;
	if (!rcWin.PtInRect(point))
	{
		ReleaseCapture();
		ShowWindow(m_hWnd,SW_HIDE);
		if (m_hCaptureWnd)
		{
			::SetCapture(m_hCaptureWnd);
		}
	}
	else
	{
		CRXSkinComboBoxPopupItem* poItem = (CRXSkinComboBoxPopupItem*)_HitTest(point);
		if (poItem)
		{
			if (m_poComboBox)
			{
				m_poComboBox->SetCurSelEx((uint32_t)poItem->GetID());
			}
			ReleaseCapture();
			ShowWindow(m_hWnd,SW_HIDE);
			if (m_hCaptureWnd)
			{
				::SetCapture(m_hCaptureWnd);
			}
			return TRUE;
		}
		else
			return m_oScrollbar.OnClick(point);
	}
	return FALSE;
}

VOID CRXSkinComboBoxPopup::OnScrollBarChanged(EScrollType enType,INT32 nPercent)
{
	if (enType == SCROLLBAR_VER)
	{
		RECT rc;
		GetClientRect(m_hWnd,&rc);
		CRXSkinRect rcClient = rc;
		m_nTopOffSet = ((m_nTotalHeight - rcClient.Height())*nPercent)/100;
		rcClient.right -= m_oScrollbar.GetWidth();
		InvalidateRect(m_hWnd,&rcClient,FALSE);
	}
}

CRXSkinComboBoxPopupItem* CRXSkinComboBoxPopup::_HitTest(POINT pt)
{
	CRXSkinComboBoxPopupItem* poItem = NULL;
	CRXSkinComboBoxPopupItemMapItr itr = m_oItemMap.begin();
	for (;itr!=m_oItemMap.end();itr++)
	{
		poItem =(CRXSkinComboBoxPopupItem*) itr->second->HitTest(pt);
		if (poItem)
		{
			break;
		}
	}
	return poItem;
}

BOOL CRXSkinComboBoxPopup::Create(const TCHAR* pszWindowClassName,
								  const TCHAR* pszWindowName,
								  const UINT32 dwStyle,
								  const HWND hParentWnd,
								  HICON hIcon)
{
	BOOL bResult = CRXSkinWnd::Create(pszWindowClassName,
									  pszWindowName,
									  dwStyle,hParentWnd,hIcon);
	if (bResult)
	{
		m_oScrollbar.SetParentWnd(m_hWnd);
	}
	return TRUE;
}

VOID CRXSkinComboBoxPopup::_ResetContent()
{
	CRXSkinComboBoxPopupItemMapItr itr = m_oItemMap.begin();
	for (;itr!=m_oItemMap.end();itr++)
	{
		delete itr->second;
	}
	m_oItemMap.clear();
	m_poMouseOn = m_poPressed = NULL;
	m_oScrollbar.Reset();
}

VOID CRXSkinComboBoxPopup::Show(POINT pt)
{
	_ResetContent();
	if (m_poComboBox)
	{
		CRXSkinComboBox::CItemDataVec& oVec = m_poComboBox->GetValueVec();
		CRXSkinComboBox::CItemDataVecItr itr = oVec.begin();
		for (;itr!=oVec.end();itr++)
		{
			CRXSkinComboBoxPopupItem* poItem = new CRXSkinComboBoxPopupItem;
			poItem->SetID((*itr)->m_dwID);
			poItem->SetComboBox(m_poComboBox);
			poItem->SetParentWnd(m_hWnd);
			poItem->SetIconNumber((*itr)->m_nImageIndex);
			m_oItemMap.insert(make_pair(poItem->GetID(),poItem));
		}
		INT32 nOffSetX = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupOffSetX();
		INT32 nOffSetY = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupOffSetY();
		INT32 nItemHeight = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxPopupItemHeight();
		INT32 nHeight = (nOffSetY+ nItemHeight)* m_poComboBox->GetCount();
		nHeight += nOffSetY*2;
		CRXSkinRect rcControl = m_poComboBox->GetRect();
		CRXSkinRect rcWin;
		rcWin.left = pt.x;
		rcWin.top = pt.y;
		rcWin.bottom = rcWin.top + nHeight;
		rcWin.right = rcWin.left + rcControl.Width();
		m_nTotalHeight = nHeight;
		if (rcWin.Height()>(INT32)m_poComboBox->GetMaxHeight())
		{
			m_oScrollbar.SetVisible(TRUE);
			m_oScrollbar.SetTotalHeight(rcWin.Height(),(INT32)m_poComboBox->GetMaxHeight());
			rcWin.bottom = rcWin.top + m_poComboBox->GetMaxHeight();
		}
		MoveWindow(m_hWnd,rcWin.left,rcWin.top,rcWin.Width(),rcWin.Height(),TRUE);
		ShowWindow(m_hWnd,SW_SHOW);
		::SetCapture(m_hWnd);
	}
}

CRXSkinComboBox::CRXSkinComboBox()
{
	m_enType = COMBOBOX_DROPLIST;
	m_poPopup = NULL;
	m_poCurItem = NULL;
	m_dwGeneratorID = 0;
	m_dwMaxHeight =DEFAULT_MAX_COMBOBOX_HEIGHT;
	m_hCaptureWnd = NULL;
	m_poImageLeft = NULL;
	m_bFocus = false;
	m_strTipValue = _T("");
	m_clrTipText = RGB(0, 0, 0);
	m_clrTipBG = RGB(255, 255, 255);
	m_clrNormalBorder = RGB(212,200,230);
	m_clrHotBorder = RGB(212, 180, 210);
	m_poImageList = NULL;
	m_wndEdit.SetOwner(this);
	m_dwTextStyle = DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_hBmpEditBk = NULL;
	m_bIsBold = FALSE;
	m_dwFontSize = 0;
	m_hFont = NULL;
	m_poListener = NULL;
}

CRXSkinComboBox::~CRXSkinComboBox()
{
	m_wndEdit.Destroy();
	if (m_poPopup)
	{
		delete m_poPopup;
		m_poPopup = NULL;
	}
	ResetContent();
	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	if (m_poImageLeft)
	{
		m_poImageLeft->Release();
	}
	m_poImageLeft = NULL;
}

VOID CRXSkinComboBox::SetMaxHeight(const UINT32 dwMaxHeight)
{
	m_dwMaxHeight = dwMaxHeight;
}

void CRXSkinComboBox::ModifyRect()
{
	SetRect(GetRect());
	CRXSkinRect rcItem = GetRect();
	INT32 nOffSet = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowOffSet();
	CRXSkinRect rc = rcItem;
	if (m_poImageLeft)
	{
		rc.left += (2 * nOffSet + m_poImageLeft->GetWidth());
		rc.left += nOffSet;
		rc.top += nOffSet;
		rc.bottom = rc.bottom - nOffSet;
		rc.right = rc.right - nOffSet - CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowWidth();
	}
	else
	{
		rc.left += nOffSet;
		rc.top += nOffSet;
		rc.bottom = rc.bottom - nOffSet;
		rc.right = rc.left + rcItem.Width() - nOffSet - CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowWidth();
	}

	if (NULL != m_wndEdit.GetHWnd())
	{
		rc.top += 4;
		rc.left += 4;
		m_wndEdit.MoveWindowEx(rc);
	}
	Invalidate();
}

UINT32 CRXSkinComboBox::GetMaxHeight()
{
	return m_dwMaxHeight;
}

VOID CRXSkinComboBox::OnEditSetFocus(HWND hWnd)
{
	m_bFocus = true;
	Invalidate();
}
VOID CRXSkinComboBox::OnEditKillFocus(HWND hWnd)
{
	m_bFocus = false;
	Invalidate();
}
VOID CRXSkinComboBox::OnEditChanged(HWND hWnd)
{	
	Invalidate();
}
HWND CRXSkinComboBox::GetEditWnd()
{
	return m_wndEdit.GetHWnd();
}

void CRXSkinComboBox::SetImageList(CRXSkinImageList* poImageList)
{
	m_poImageList = poImageList;
}

BOOL CRXSkinComboBox::Create(const UINT32 dwID,CRXSkinRect rcClient,EComboBoxType enType)
{
	m_enType = enType;
	if (m_enType == COMBOBOX_DROPDOWN ||
		m_enType == COMBOBOX_DROPDOWN_EXT)
	{
		INT32 nOffSet = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowOffSet();
		CRXSkinRect rc = rcClient;
		if (m_poImageLeft)
		{
			rc.left += (2 * 2 + m_poImageLeft->GetWidth());
			rc.left += 2;
			rc.top += 8;
			rc.bottom = rc.bottom - 4;
			rc.right = rc.right - nOffSet - CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowWidth();
		}
		else
		{
			rc.left += nOffSet;
			rc.top += nOffSet;
			rc.bottom = rc.bottom - nOffSet;
			rc.right = rc.left + rcClient.Width() - nOffSet - CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowWidth();
		}

		if (NULL == m_wndEdit.GetHWnd())
		{			
			if (m_strFontName.size() > 0 && m_dwFontSize > 0)
			{
				if (m_hFont == NULL)
				{
					m_hFont = CreateFont(
						m_dwFontSize,
						0,
						0,
						0,
						m_bIsBold ? FW_BOLD : FW_NORMAL,
						FALSE,
						FALSE,
						0,
						ANSI_CHARSET,
						OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS,
						m_strFontName.c_str());
					m_wndEdit.SetFont(m_hFont);
				}
			}
			m_wndEdit.Create(WC_EDIT, _T(""), WS_CHILD | WS_VISIBLE, GetParentWnd(), NULL);
			rc.top += 4;
			rc.left += 4;
			m_wndEdit.MoveWindowEx(rc);
		}
	}	
	return TRUE;
}
HBRUSH CRXSkinComboBox::OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC)
{
	if (hWndChild == m_wndEdit.GetHWnd())
	{
		HBRUSH hBrush = m_wndEdit.OnCtlColorControl(dwMsgID, hWndChild, hDC);

		return hBrush;
	}

	return (HBRUSH)CRXControl::OnCtlColorControl(dwMsgID, hWndChild, hDC);
}
VOID CRXSkinComboBox::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible() ==FALSE)
	{
		return;
	}
	SetRect(rcItem);

	if (GetState() == CONTROL_STATE_NORMAL)
	{
		DrawRectEdge(hDC, rcItem, 1, m_clrNormalBorder);
	}
	else
	{
		DrawRectEdge(hDC, rcItem, 1, m_clrHotBorder);
	}

	INT32 nOffSet = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowOffSet();
	m_rcArrow = rcItem;
	m_rcArrow.right -= nOffSet;
	INT32 nHeight = CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowWidth();
	m_rcArrow.left = m_rcArrow.right - CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowWidth();
	m_rcArrow.top = rcItem.top + (rcItem.Height() - nHeight) / 2;
	m_rcArrow.bottom = m_rcArrow.top + nHeight;
	if (GetState() == CONTROL_STATE_NORMAL)
	{
		IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_COMBOBOX_NORMAL);
		if (poImage)
		{
			CRXSkinRect rcImage = m_rcArrow;
			rcImage.left = rcImage.left + (m_rcArrow.Width() - poImage->GetWidth()) / 2;
			rcImage.right = rcImage.left + poImage->GetWidth();
			rcImage.top = rcImage.top + (m_rcArrow.Height() - poImage->GetHeight()) / 2;
			rcImage.bottom = rcImage.top + poImage->GetHeight();
			if (IsEnabled())
				poImage->Draw(hDC, rcImage);
			else
				poImage->DrawGray(hDC, rcImage);
		}
	}
	else
	{
		IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_COMBOBOX_HOT);
		if (poImage)
		{
			CRXSkinRect rcImage = m_rcArrow;
			rcImage.left = rcImage.left + (m_rcArrow.Width() - poImage->GetWidth()) / 2;
			rcImage.right = rcImage.left + poImage->GetWidth();
			rcImage.top = rcImage.top + (m_rcArrow.Height() - poImage->GetHeight()) / 2;
			rcImage.bottom = rcImage.top + poImage->GetHeight();
			if (IsEnabled())
				poImage->Draw(hDC, rcImage);
			else
				poImage->DrawGray(hDC, rcImage);
		}
	}

	if (m_enType == COMBOBOX_DROPLIST || m_enType == COMBOBOX_DROPLIST_FILE)
	{
		if (m_enType == COMBOBOX_DROPLIST)
		{
			if (m_poCurItem)
			{
				CRXSkinRect rcTxt = rcItem;
				rcTxt.left = rcTxt.left + nOffSet;
				rcTxt.right -= CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowWidth() - 2 * nOffSet;
				tstring strTxt = m_poCurItem->m_szValue;
				COLORREF clrOld = RGB(0, 0, 0);
				if (!IsEnabled())
				{
					clrOld = CRXSkinService::Instance()->GetDrawMgr().GetDisableText();
				}
				SkCanvas* poCanvas = poSurface->getCanvas();
				if (poCanvas)
				{
					CRXSkinFontMgr::Instance()->DrawText(poCanvas,
						strTxt, rcTxt,
						clrOld,
						ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT| DT_WORDBREAK);
				}		
			}
		}
		if (m_enType == COMBOBOX_DROPLIST_FILE)
		{
			if (m_poCurItem)
			{
				if (m_poImageList)
				{
					if (m_poCurItem->m_nImageIndex == -1)
					{
						m_poCurItem->m_nImageIndex = CRXSkinService::Instance()->GetDrawMgr().GetShellImageListIndex(m_poCurItem->m_szValue);
					}
					CRXSkinRect rcTxt = rcItem;
					rcTxt.left = rcTxt.left + nOffSet;
					int cx = m_poImageList->GetX();
					int cy = m_poImageList->GetY();
					RGBQUAD tran;
					tran.rgbBlue = 0;
					tran.rgbGreen = 0;
					tran.rgbRed = 0;
					tran.rgbReserved = 255;
					if (m_poCurItem->m_nImageIndex>=0)
					{
						m_poImageList->GetImage()->DrawTrans(hDC,
							tran,
							rcTxt.left,
							rcTxt.top + (rcTxt.Height() - cy) / 2,
							cx, cy,
							0,
							m_poCurItem->m_nImageIndex * cy
						);
					}		

					rcTxt.left = rcTxt.left + cx + nOffSet;
					rcTxt.right -= CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowWidth() - 2 * nOffSet;
					tstring strTxt = m_poCurItem->m_szValue;
					COLORREF clrOld = RGB(0, 0, 0);					
					if (!IsEnabled())
					{
						clrOld = CRXSkinService::Instance()->GetDrawMgr().GetDisableText();
					}
					SkCanvas* poCanvas = poSurface->getCanvas();
					if (poCanvas)
					{
						CRXSkinFontMgr::Instance()->DrawText(poCanvas,
							strTxt, rcTxt,
							clrOld,
							ID_FONT_COMMON, DT_END_ELLIPSIS| DT_VCENTER | DT_SINGLELINE | DT_LEFT | DT_WORDBREAK);
					}	
				}
			}
			
		}
	}
	CRXSkinRect rcTip = rcItem;
	CRXSkinService::Instance()->GetDrawMgr().DrawControlBorder(hDC, GetState(), GetRect(), IsEnabled());
	if (NULL == m_poImageLeft)
	{
		m_poImageLeft = CRXSkinService::Instance()->GetImageMgr().OpenDirectImage(m_strLeftImage.c_str());
	}
	if (m_poImageLeft)
	{
		CRXSkinRect rc = rcItem;
		rc.left += 4;
		rc.top = rc.top + (rcItem.Height() - m_poImageLeft->GetHeight()) / 2;
		rc.bottom = rc.top + m_poImageLeft->GetHeight();
		rc.right = rc.left + m_poImageLeft->GetWidth();
		m_poImageLeft->Draw(hDC, rc);
		rcTip.left = rc.right + nOffSet;
	}
	else
	{
		rcTip.left += nOffSet;
	}
	rcTip.top += nOffSet;
	rcTip.bottom -= nOffSet;
	rcTip.right = rcItem.right - CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowWidth() - 2 * nOffSet;
	if (m_strTipValue.GetLength()>0 && m_poCurItem==NULL)
	{
		tstring strTxt = m_wndEdit.GetTextValue();
		if (strTxt.size()<=0)
		{
			tstring strTxt = m_strTipValue.GetData();
			rcTip.left += 2;
			SkCanvas* poCanvas = poSurface->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas,
					strTxt, rcTip,
					m_clrTipText,
					ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			}

		}

	}
	if (m_enType == COMBOBOX_DROPDOWN||
		m_enType == COMBOBOX_DROPDOWN_EXT)
	{
		CRXSkinRect rcBK = rcItem;
		rcBK.left += nOffSet;
		if (m_poImageLeft)
		{
			rcBK.left += m_poImageLeft->GetWidth();
			rcBK.left += nOffSet;
		}
		rcBK.top += 6;
		rcBK.right -= 2;
		rcBK.bottom -= 2;
		m_hBmpEditBk = CDrawMgr::CopyDCToBitmap(hDC, &rcBK);
	}
}

VOID CRXSkinComboBox::SetText(const TCHAR* strTxt)
{
	CRXControl::SetText(strTxt);
	if (m_enType == COMBOBOX_DROPDOWN||
		m_enType == COMBOBOX_DROPDOWN_EXT)
	{
		m_wndEdit.SetText(strTxt);		
	}
}

VOID CRXSkinComboBox::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
	if (bIsVisible)
	{
		m_wndEdit.Show();
	}
	else
		m_wndEdit.Hide();
}

VOID CRXSkinComboBox::Enable(const BOOL bEnabled)
{
	CRXControl::Enable(bEnabled);
	m_wndEdit.EnableWindow(bEnabled);
}

VOID CRXSkinComboBox::SetCaptureWnd(HWND hCpatureWnd)
{
	m_hCaptureWnd = hCpatureWnd;
}

HWND CRXSkinComboBox::GetHWnd()
{
	if (m_poPopup)
	{
		return m_poPopup->GetHWnd();
	}
	return RXSKIN_INVALIDATE_WND;
}

BOOL CRXSkinComboBox::OnClick(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		INT32 nOffSet = CRXSkinService::Instance()->GetDrawMgr().GetControlBorderSize();

		if (COMBOBOX_DROPLIST == m_enType || COMBOBOX_DROPLIST_FILE == m_enType )
		{
			if (PtInRect(pt))
			{
				if (NULL == m_poPopup)
				{
					m_poPopup = new CRXSkinComboBoxPopup;
					m_poPopup->SetAutoDelete(true);
					m_poPopup->SetCaptureWnd(m_hCaptureWnd);
					m_poPopup->SetComboBox(this);
					CRXSkinRect rcClient = GetRect();
					POINT pt;
					pt.x = rcClient.left;
					pt.y = rcClient.bottom;
					ClientToScreen(GetParentWnd(),&pt);
					m_poPopup->Create(WND_COMBOBOX_POPUP,
						_T(""),WS_POPUP,GetParentWnd(),NULL);
					m_poPopup->Show(pt);
				}
				else
				{
					CRXSkinRect rcClient = GetRect();
					POINT pt;
					pt.x = rcClient.left;
					pt.y = rcClient.bottom;
					ClientToScreen(GetParentWnd(),&pt);
					m_poPopup->Show(pt);
				}
				return TRUE;
			}
		}
		else
		{
			CRXSkinRect rc = GetRect();
			rc.left = rc.right - CRXSkinService::Instance()->GetDrawMgr().GetComboBoxArrowWidth() 
				- 2 * nOffSet;
			if (rc.PtInRect(pt))
			{
				if (NULL == m_poPopup)
				{
					m_poPopup = new CRXSkinComboBoxPopup;
					m_poPopup->SetAutoDelete(true);
					m_poPopup->SetCaptureWnd(m_hCaptureWnd);
					m_poPopup->SetComboBox(this);
					CRXSkinRect rcClient = GetRect();
					POINT pt;
					pt.x = rcClient.left;
					pt.y = rcClient.bottom;
					ClientToScreen(GetParentWnd(),&pt);
					m_poPopup->Create(WND_COMBOBOX_POPUP,
						_T(""),WS_POPUP,GetParentWnd(),NULL);

					m_poPopup->Show(pt);
				}
				else
				{
					CRXSkinRect rcClient = GetRect();
					POINT pt;
					pt.x = rcClient.left;
					pt.y = rcClient.bottom;
					ClientToScreen(GetParentWnd(),&pt);
					m_poPopup->Show(pt);
				}
				return TRUE;
			}
		}

	}
	return FALSE;
}

BOOL CRXSkinComboBox::OnLButtonDown(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (PtInRect(pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXSkinComboBox::PtInRect(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CRXSkinComboBox::OnMouseMove(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (PtInRect(pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXSkinComboBox::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	CRXControl::OnSize(dwX,dwY);
	return TRUE;
}

VOID CRXSkinComboBox::ReleaseControl()
{
	delete this;
}

CRXSkinComboBox::CItemDataVec& CRXSkinComboBox::GetValueVec()
{
	return m_vecValues;
}

UINT32 CRXSkinComboBox::GetCount()
{
	return (UINT32)m_vecValues.size();
}

VOID CRXSkinComboBox::ResetContent()
{
	CItemDataVecItr itr = m_vecValues.begin();
	for (;itr!= m_vecValues.end();itr++)
	{
		SItemData* poItem = *itr;
		if (poItem)
		{
			delete poItem;
		}
	}
	m_vecValues.clear();
	SetText(_T(""));
	m_poCurItem = NULL;
	if (m_poPopup)
	{
		delete m_poPopup;
		m_poPopup = NULL;
	}
	Invalidate();
}

TCHAR* CRXSkinComboBox::GetLBText(const UINT32 dwID)
{
	if (dwID>=m_vecValues.size())
	{
		return (TCHAR*)_T("");
	}
	return m_vecValues[dwID]->m_szValue;
}
TCHAR* CRXSkinComboBox::GetLBTextEx(const UINT32 dwID)
{
	for (int m=0;m <(int)m_vecValues.size();m ++)
	{
		if (dwID == m_vecValues[m]->m_dwID)
		{
			return m_vecValues[m]->m_szValue;
		}
	}
	return (TCHAR*)_T("");
}
VOID CRXSkinComboBox::SetCurSel(const UINT32 dwID,const BOOL& bNotify)
{
	if (dwID >= m_vecValues.size())
	{
		m_poCurItem = NULL;
	}
	else
		m_poCurItem = m_vecValues[dwID];

	if (m_poCurItem)
	{
	//	m_wndEdit.SetText(m_poCurItem->m_szValue);
 		SetText(m_poCurItem->m_szValue);
	}
	else
	{
		m_wndEdit.SetText(_T(""));
		SetText(_T(""));
	}

	RECT rc = GetInvalidateRect();
	InvalidateRect(GetParentWnd(),&rc,FALSE);

	if (bNotify)
	{
		::PostMessage(GetParentWnd(), WM_RXSKIN_COMBOBOX_CHANGED, GetID(), NULL);
		if (m_poListener)
		{
			m_poListener->OnComboBoxChanged(GetID());
		}
	}
}

VOID CRXSkinComboBox::SetCurSelEx(const UINT32 dwID)
{
	SItemData* poItemData = NULL;
	for (int m=0;m < (int)m_vecValues.size();m ++)
	{
		if (m_vecValues[m]->m_dwID == dwID)
		{
			poItemData = m_vecValues[m];
			break;
		}
	}

	if (poItemData)
	{
		if (poItemData != m_poCurItem)
		{
			m_poCurItem = poItemData;
			m_wndEdit.SetText(m_poCurItem->m_szValue);
			SetText(m_poCurItem->m_szValue);
			RECT rc = GetInvalidateRect();
			InvalidateRect(GetParentWnd(), &rc, FALSE);
			::SendMessage(GetParentWnd(), WM_RXSKIN_COMBOBOX_CHANGED, GetID(), NULL);
			if (m_poListener)
			{
				m_poListener->OnComboBoxChanged(GetID());
			}

		}
	}
	else
	{
		m_wndEdit.SetText(_T("'"));
		SetText(_T(""));
	}
}

void CRXSkinComboBox::SelectFirst()
{
	int ncur = -1;
	if (m_vecValues.size()>0)
	{
		SetCurSel(0, TRUE);
	}

}

void CRXSkinComboBox::SelectString(const TCHAR* cpName)
{
	int ncur = -1;
	for (int m =0;m <(int)m_vecValues.size();m ++)
	{
		SItemData* poData = m_vecValues[m];
		if (_tcscmp(poData->m_szValue,cpName) == 0)
		{
			SetCurSel(m,FALSE);
			break;
		}
	}
}
INT32 CRXSkinComboBox::GetCurSel()
{
	if (m_poCurItem)
	{
		return m_poCurItem->m_dwID;
	}
	return -1;
}

CRXSkinString CRXSkinComboBox::GetWindowText()
{
	if (m_enType == COMBOBOX_DROPDOWN||
		m_enType == COMBOBOX_DROPDOWN_EXT)
	{
		return (TCHAR*)m_wndEdit.GetTextValue().c_str();
	}
	else
	{
		if (m_poCurItem)
		{
			return m_poCurItem->m_szValue;
		}
	}
	return (TCHAR*)_T("");
}

void CRXSkinComboBox::SetFocus()
{
	if (m_enType == COMBOBOX_DROPDOWN||
		m_enType == COMBOBOX_DROPDOWN_EXT)
	{
		if (m_wndEdit.GetHWnd()!=NULL)
		{
			::SetFocus(m_wndEdit.GetHWnd());
		}
	}
}

TCHAR* CRXSkinComboBox::GetText()
{
	if (m_enType == COMBOBOX_DROPDOWN||
		m_enType == COMBOBOX_DROPDOWN_EXT)
	{
		m_strText = (TCHAR*)m_wndEdit.GetTextValue().c_str();
		return (TCHAR*)m_strText.GetData();
	}
	else 
	{
		if (m_poCurItem)
		{
			return m_poCurItem->m_szValue;
		}
	}
	return (TCHAR*)_T("");
}

LPARAM CRXSkinComboBox::GetCurSelData()
{
	if (m_poCurItem)
	{
		return m_poCurItem->m_wParam;
	}
	return NULL;
}

void CRXSkinComboBox::SetListener(IRXSkinComboBoxListener* poListener)
{
	m_poListener = poListener;
}

LPARAM CRXSkinComboBox::GetItemData(const UINT32 dwID)
{
	CItemDataVecItr itr = m_vecValues.begin();
	for (; itr != m_vecValues.end(); itr++)
	{
		if ((*itr)->m_dwID == dwID)
		{
			SItemData* poData = *itr;

			return poData->m_wParam;
		}
	}
	return NULL;
}

UINT32 CRXSkinComboBox::AddString(const TCHAR* pszData, INT32 nImageIndex,LPARAM lpPara/* =NULLm */)
{	
	SItemData* poItem = FindItemByText(pszData);
	if (poItem)
	{
		return poItem->m_dwID;
	}
	poItem = new SItemData;
	poItem->m_dwID = m_dwGeneratorID++;
	_tcsncpy(poItem->m_szValue,pszData,256);
	poItem->m_wParam = lpPara;
	poItem->m_nImageIndex = nImageIndex;
	m_vecValues.push_back(poItem);
	return poItem->m_dwID;
}

UINT32 CRXSkinComboBox::AddStringEx(const TCHAR* pszData,
	const TCHAR* pszValueEx,
	INT32 nImageIndex /* = -1 */,
	LPARAM lpParam /* = NULL */)
{
	SItemData* poItem = FindItemByText(pszData);
	if (poItem)
	{
		return poItem->m_dwID;
	}
	poItem = new SItemData;
	poItem->m_dwID = m_dwGeneratorID++;
	_tcsncpy(poItem->m_szValue, pszData, 256);
	_tcsncpy(poItem->m_szValueEx, pszValueEx, 256);
	poItem->m_wParam = lpParam;
	poItem->m_nImageIndex = nImageIndex;
	m_vecValues.push_back(poItem);
	return poItem->m_dwID;
}
VOID CRXSkinComboBox::DelString(const UINT32 dwID)
{
	CItemDataVecItr itr = m_vecValues.begin();
	for (;itr!= m_vecValues.end();itr++)
	{
		if ((*itr)->m_dwID == dwID)
		{
			SItemData* poData = *itr;
			m_vecValues.erase(itr);
			delete poData;
			break;
		}
	}
}

VOID CRXSkinComboBox::DelString(const TCHAR* pszData)
{
	CItemDataVecItr itr = m_vecValues.begin();
	for (; itr != m_vecValues.end(); )
	{
		SItemData* poItem = *itr;
		if (poItem && _tcsncmp(pszData,poItem->m_szValue,256) == 0)
		{
			itr = m_vecValues.erase(itr);
			if (m_poCurItem == poItem)
			{
				m_poCurItem = NULL;
			}
			delete poItem;
		}
		else
		{
			itr++;
		}
	}
}

void CRXSkinComboBox::SetLeftImage(const TCHAR* pszImageFile)
{
	m_strLeftImage = pszImageFile;
	if (m_poImageLeft)
	{
		m_poImageLeft->Release();
	}
	m_poImageLeft = NULL;
}

CRXSkinComboBox::SItemData* CRXSkinComboBox::FindItem(const UINT32 dwID)
{
	CItemDataVecItr itr = m_vecValues.begin();
	for (; itr != m_vecValues.end(); itr++)
	{
		if ((*itr)->m_dwID == dwID)
		{
			SItemData* poData = *itr;
			return poData;
		}
	}
	return NULL;
}

CRXSkinComboBox::SItemData* CRXSkinComboBox::FindItemByText(const TCHAR* pszTxt)
{
	CItemDataVecItr itr = m_vecValues.begin();
	for (; itr != m_vecValues.end(); itr++)
	{
		if (_RXTStrcmp((*itr)->m_szValue,pszTxt) == 0)
		{
			SItemData* poData = *itr;
			return poData;
		}
	}
	return NULL;
}

