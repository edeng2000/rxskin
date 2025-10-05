#include "pch.h"
#include "controls\rxskinlistctrl.h"
#include "common\RXSkinService.h"
#include "common\graphics.h"
#include "main\rxskinapp.h"
#define DEFAULT_HOTKEY_WIDTH    85

#define CHECK_OFFSET   7

CRXSkinListCtrlItem::CRXSkinListCtrlItem()
{
	m_enType = LIST_ITEM_TEXT;
	m_dwPercent = 0;
	m_bLasted = FALSE;
	m_bVisibled = FALSE;
	m_dwDrawStyle = DT_VCENTER|DT_SINGLELINE| DT_END_ELLIPSIS| DT_LEFT;
	m_bRowSelected = FALSE;
	m_poHotkeyCtrl = NULL;
	m_bChecked = FALSE;
	m_iImageID = -1;
	m_poOwner = NULL;
	m_edtRename = NULL;
	m_dwRowID = 0;
	m_dwProgress = 0;
	m_poImage = NULL;
	m_poImageList = NULL;
	m_enSortType = LIST_CTRL_SORT_NONE;
	m_bCanSort = false;
	m_enValueType = LIST_CTRL_VALUE_TYPE_STRING;
	m_bCanResize = false;
	m_dwMinWidth = 100;
	m_bResing = false;
}

CRXSkinListCtrlItem::~CRXSkinListCtrlItem()
{
	if (m_edtRename)
	{
		m_edtRename->ReleaseControl();
		m_edtRename = NULL;
	}
}

CRXSkinString CRXSkinListCtrlItem::GetText()
{
	return m_strText;
}

VOID CRXSkinListCtrlItem::SetText(const TCHAR* pszText)
{
	m_strText = pszText;
}

void CRXSkinListCtrlItem::CancelRename()
{
	if (m_edtRename)
	{
		m_edtRename->Destroy();
		m_edtRename = NULL;
	}
}

void CRXSkinListCtrlItem::OnEditKillFocus(const UINT32 dwID)
{

}

void CRXSkinListCtrlItem::OnEditChanged(const UINT32 dwID)
{

}

void CRXSkinListCtrlItem::OnEditReturnOK(const UINT32 dwID)
{
	::SendMessage(m_poOwner->GetParentWnd(), WM_RXSKIN_LISTCTRL_RENAME, m_poOwner->GetID(), m_dwRowID);
}

tstring CRXSkinListCtrlItem::GetRenameValue()
{
	if (m_edtRename)
	{
		return m_edtRename->GetText();
	}
	return _T("");
}

void CRXSkinListCtrlItem::Rename()
{
	CRXSkinRect rcChild = GetRect();
	INT32 nHeight = 18;
	if (m_iImageID >= 0 && m_poOwner && m_poOwner->GetImageList())
	{
		rcChild.left += m_poOwner->GetImageList()->GetX();
		rcChild.left += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
	}
	rcChild.top = (rcChild.Height() - nHeight) / 2 + rcChild.top;
	rcChild.bottom = rcChild.top + nHeight;
	if (NULL == m_edtRename)
	{
		m_edtRename = new CRXSkinEdit;
		m_edtRename->SetRename();
		m_edtRename->SetParentWnd(GetParentWnd());		
		m_edtRename->Create((uint32_t)GetID(), rcChild, FALSE);
	}
	m_edtRename->SetListener(this);
	m_edtRename->SetRect(rcChild);
	m_edtRename->ModifyRect();
	m_edtRename->SetText(GetText());
	m_edtRename->SetVisible(TRUE);
}

VOID CRXSkinListCtrlItem::SetRowSelected(const BOOL bRowSelected)
{
	m_bRowSelected = bRowSelected;
	if (m_poHotkeyCtrl)
	{
		m_poHotkeyCtrl->SetVisible(bRowSelected);
	}
}

BOOL CRXSkinListCtrlItem::IsRowSelected()
{
	return m_bRowSelected;
}

HWND CRXSkinListCtrlItem::GetHotkeyWnd()
{
	if (m_poHotkeyCtrl)
	{
		return m_poHotkeyCtrl->GetEditWnd();
	}
	return NULL;
}

VOID CRXSkinListCtrlItem::_DrawCheckBox(SkSurface* poSurface, HDC hDC)
{
	CRXSkinRect rcItem = GetRect();
	INT32 nTextLeft = 0;
	if (m_bChecked)
	{
		IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_CHECKBOX_CHECKED);
		if (poImage)
		{
			INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight()) / 2;
			INT32 nLeft = rcItem.left + CHECK_OFFSET;
			nTextLeft += CHECK_OFFSET * 2;
			nTextLeft += poImage->GetWidth();
			poImage->Stretch(hDC, nLeft, nTop, poImage->GetWidth(), poImage->GetHeight());
		}
	}
	else
	{
		if (GetState() == CONTROL_STATE_NORMAL)
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_CHECKBOX_NORMAL);
			if (poImage)
			{
				INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight()) / 2;
				INT32 nLeft = rcItem.left + CHECK_OFFSET;
				nTextLeft += CHECK_OFFSET * 2;
				nTextLeft += poImage->GetWidth();
				poImage->Stretch(hDC, nLeft, nTop, poImage->GetWidth(), poImage->GetHeight());
			}
		}
		else
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_CHECKBOX_HOT);
			if (poImage)
			{
				INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight()) / 2;
				INT32 nLeft = rcItem.left + CHECK_OFFSET;
				nTextLeft += CHECK_OFFSET * 2;
				nTextLeft += poImage->GetWidth();
				poImage->Stretch(hDC, nLeft, nTop, poImage->GetWidth(), poImage->GetHeight());
			}
		}
	}
	CRXSkinRect rcTxt = rcItem;
	rcTxt.left += nTextLeft;
	tstring strTxt = GetText().GetData();
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas,
			strTxt, rcItem,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonListNormalText(),
			ID_FONT_COMMON, m_dwDrawStyle);
	}
}

VOID  CRXSkinListCtrlItem::_DrawImageButton(SkSurface* poSurface, HDC hDC)
{
	CRXSkinRect rcItem = GetRect();
	rcItem.left += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
	rcItem.right -= CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
	rcItem.top += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetY();
	rcItem.bottom -= CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetY();
	switch (GetState())
	{
	case  ITEM_MOUSEON:
	{
		DrawRoundRect(hDC,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonBorderColor(),
			m_poOwner->GetButtonMouseOnBKFrom(),
			rcItem,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy);
		HRGN hRgn = ::CreateRoundRectRgn(rcItem.left + 1,
			rcItem.top + 1,
			rcItem.right - 1,
			rcItem.top + rcItem.Height() / 2,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy);
		CRXSkinRect rcTop = rcItem;
		rcTop.left += 1;
		rcTop.right -= 1;
		rcTop.top += 1;
		rcTop.bottom = rcTop.top + rcItem.Height() / 2;
		CreateRoundRectRgn(hRgn,
			rcTop,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy,
			TRUE, FALSE, TRUE, FALSE);
		::DrawGradientRgn(hDC, TRUE, hRgn, m_poOwner->GetButtonMouseOnBKFrom(),
			m_poOwner->GetButtonMouseOnBKMiddle());
		CRXSkinRect rcBottom = rcItem;
		rcBottom.left += 1;
		rcBottom.right -= 1;
		rcBottom.top = rcItem.top + rcItem.Height() / 2;
		rcBottom.bottom = rcItem.bottom - 1;
		CreateRoundRectRgn(hRgn,
			rcBottom,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy,
			FALSE, TRUE, FALSE, TRUE);
		::DrawGradientRgn(hDC, TRUE, hRgn, m_poOwner->GetButtonMouseOnBKMiddle(),
			m_poOwner->GetButtonMouseOnBKTo());
		DeleteObject(hRgn);
	}
	break;
	case ITEM_PRESSED:
	case ITEM_SELECTED:
	{
		DrawRoundRect(hDC,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonBorderColor(),
			m_poOwner->GetButtonPressedBKFrom(),
			rcItem,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy);
		HRGN hRgn = ::CreateRoundRectRgn(rcItem.left + 1,
			rcItem.top + 1,
			rcItem.right - 1,
			rcItem.top + rcItem.Height() / 2,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy);
		CRXSkinRect rcTop = rcItem;
		rcTop.left += 1;
		rcTop.right -= 1;
		rcTop.top += 1;
		rcTop.bottom = rcTop.top + rcItem.Height() / 2;
		CreateRoundRectRgn(hRgn,
			rcTop,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy,
			TRUE, FALSE, TRUE, FALSE);
		::DrawGradientRgn(hDC, TRUE, hRgn,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedFromColor(),
			m_poOwner->GetButtonPressedBKMiddle());
		CRXSkinRect rcBottom = rcItem;
		rcBottom.left += 1;
		rcBottom.right -= 1;
		rcBottom.top = rcItem.top + rcItem.Height() / 2;
		rcBottom.bottom = rcItem.bottom - 1;
		CreateRoundRectRgn(hRgn,
			rcBottom,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy,
			FALSE, TRUE, FALSE, TRUE);
		::DrawGradientRgn(hDC, TRUE,
			hRgn, CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedMiddleColor(),
			m_poOwner->GetButtonPressedBKTo());
		DeleteObject(hRgn);
	}
	break;
	default:
		break;
	}
	if (m_poImage)
	{
		INT32 nTop = rcItem.top + (rcItem.Height() - m_poImage->GetHeight()) / 2;
		INT32 nLeft = rcItem.left + (rcItem.Width() - m_poImage->GetWidth())/2;
		m_poImage->Stretch(hDC, nLeft, nTop, m_poImage->GetWidth(), m_poImage->GetHeight());
	}
}

void  CRXSkinListCtrlItem::_DrawImage(SkSurface* poSurface, HDC hDC)
{
	if (m_poImage)
	{
		CRXSkinRect rcItem = GetRect();
		rcItem.left = rcItem.left + (rcItem.Width() - m_poImage->GetWidth()) / 2;
		rcItem.right = rcItem.left + m_poImage->GetWidth();
		rcItem.top = rcItem.top + (rcItem.Height() - m_poImage->GetHeight()) / 2;
		rcItem.bottom = rcItem.top + m_poImage->GetHeight();
		m_poImage->Stretch(hDC, rcItem.left, rcItem.top, m_poImage->GetWidth(), m_poImage->GetHeight());
	}
}

VOID CRXSkinListCtrlItem::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	switch(m_enType)
	{
	case LIST_ITEM_HEADER:
		{
			_DrawColumn(poSurface,hDC);
		}
		break;
	case LIST_ITEM_TEXT:
		{
			_DrawText(poSurface,hDC);
		}
		break;
	case LIST_ITEM_HOTKEY:
		{
			_DrawHotkey(poSurface,hDC);
		}
		break;
	case LIST_ITEM_CHECKBOX:
		{
			_DrawCheckBox(poSurface,hDC);
		}
		break;
	case LIST_ITEM_PROGRESS:
		{
			_DrawProgress(poSurface,hDC);
		}
		break;
	case LIST_ITEM_IMAGE:
		{
			_DrawImage(poSurface,hDC);
		}
		break;
	case LIST_ITEM_IMAGE_BUTTON:
	{
		_DrawImageButton(poSurface,hDC);
	}
	break;
	default:
		break;
	}

}

VOID CRXSkinListCtrlItem::AddDrawStyle(const UINT32 dwAdd)
{
	m_dwDrawStyle |= dwAdd;
}

void CRXSkinListCtrlItem::SetDrawStyle(const UINT32 dwStyle)
{
	m_dwDrawStyle = dwStyle;
}

void CRXSkinListCtrlItem::SetRect(CRXSkinRect rcItem)
{
	CRXSkinItem::SetRect(rcItem);
	if (m_edtRename)
	{
		CRXSkinRect rxTxt = rcItem;
		if (m_iImageID>=0 && m_poOwner->GetImageList())
		{
			int cx = m_poOwner->GetImageList()->GetX();
			int cy = m_poOwner->GetImageList()->GetY();
			rxTxt.left += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX() * 2;
			rxTxt.left += cx;
			INT32 nHeight = 18;
			rxTxt.top = rxTxt.top + (rcItem.Height() - nHeight) / 2;
			rxTxt.bottom = rxTxt.top + nHeight;
		}
		m_edtRename->SetRect(rxTxt);
		m_edtRename->ModifyRect();
	}
}

void CRXSkinListCtrlItem::_DrawProgress(SkSurface* poSurface, HDC hDC)
{
	CRXSkinRect rcTxt = GetRect();
	rcTxt.top += 2;
	rcTxt.bottom -= 2;
	rcTxt.left += 2;
	rcTxt.right -= 2;
	DrawRectEdge(hDC, rcTxt, 1, CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBorder());
	float fPercent = (float)m_dwProgress / 100.0f;
	uint32_t dwPercentWidth = (uint32_t)(rcTxt.Width() * fPercent);
	CRXSkinRect rcProgress = rcTxt;
	rcProgress.left += 1;
	rcProgress.top += 1;
	rcProgress.right = rcProgress.left + dwPercentWidth;
	rcProgress.bottom -= 1;
	DrawRect(hDC, RGB(155,100,120),rcProgress.left,
		rcProgress.top,rcProgress.right,rcProgress.bottom);
	TCHAR szTxt[256] = {0};
	_RXTsprintf(szTxt, _T("%d%%"), m_dwProgress);
	tstring strTxt = szTxt;
	COLORREF  clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemNormalTxtColor();
	switch (GetState())
	{
	case ITEM_NORMAL:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemNormalTxtColor();
	}
	break;
	case  ITEM_MOUSEON:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemMouseOnTxtColor();
	}
	break;
	case ITEM_PRESSED:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemPressedTxtColor();
	}
	break;
	default:
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemNormalTxtColor();
		break;
	}
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas,
			strTxt, rcTxt,
			clrTxt,
			ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER);
	}
}

void CRXSkinListCtrlItem::SetImageList(CRXSkinImageList* poList)
{
	m_poImageList = poList;
}

void CRXSkinListCtrlItem::ChangeSortModel()
{
	if (m_bCanSort)
	{
		if (m_enSortType == LIST_CTRL_SORT_NONE)
		{
			m_enSortType = LIST_CTRL_SORT_UP;
			m_poOwner->Sort((uint32_t)GetID(), ERXSkinListItemSortType::LIST_CTRL_SORT_UP);
			m_poOwner->InvalideBody();
		}
		else if (m_enSortType == LIST_CTRL_SORT_DOWN)
		{
			m_enSortType = LIST_CTRL_SORT_UP;
			m_poOwner->Sort((uint32_t)GetID(), ERXSkinListItemSortType::LIST_CTRL_SORT_UP);
			m_poOwner->InvalideBody();
		}
		else if (m_enSortType == LIST_CTRL_SORT_UP)
		{
			m_enSortType = LIST_CTRL_SORT_DOWN;
			m_poOwner->Sort((uint32_t)GetID(), ERXSkinListItemSortType::LIST_CTRL_SORT_DOWN);
			m_poOwner->InvalideBody();
		}
		Invalidate();
	}
}


VOID CRXSkinListCtrlItem::_DrawText(SkSurface* poSurface, HDC hDC)
{
	CRXSkinRect rcTxt = GetRect();
	rcTxt.left += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
	INT32 nModel = SetBkMode(hDC,TRANSPARENT);
	HFONT hOld = (HFONT)SelectObject(hDC,CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
	tstring strTxt = GetText().GetData();
	COLORREF  clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemNormalTxtColor();
	switch (GetState())
	{
	case ITEM_NORMAL:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemNormalTxtColor();
	}
	break;
	case  ITEM_MOUSEON:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemMouseOnTxtColor();
	}
	break;
	case ITEM_PRESSED:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemPressedTxtColor();
	}
	break;
	default:
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemNormalTxtColor();
		break;
	}
	CRXSkinImageList* poImageList = m_poImageList;
	if (poImageList == NULL)
	{
		if (m_poOwner && m_poOwner->GetImageList())
		{
			poImageList = m_poOwner->GetImageList();
		}
	}
	if (m_iImageID>=0 && poImageList)
	{

		int cx = poImageList->GetX();
		int cy = poImageList->GetY();
		RGBQUAD tran;
		tran.rgbBlue = 0;
		tran.rgbGreen = 0;
		tran.rgbRed = 0;
		tran.rgbReserved = 0;
		if (poImageList->CheckImageIndex(m_iImageID))
		{
			poImageList->GetImage()->DrawTrans(hDC,
				tran,
				rcTxt.left,
				rcTxt.top + (rcTxt.Height() - cy) / 2,
				cx,
				cy,
				0,
				m_iImageID * cy
			);
		}

		rcTxt.left += cx;
		rcTxt.left+= CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
	}

	COLORREF oldTxt = SetTextColor(hDC, clrTxt);
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas,
			strTxt, rcTxt,
			clrTxt,
			ID_FONT_COMMON, m_dwDrawStyle);
	}
	SelectObject(hDC,hOld);
	SetBkMode(hDC,nModel);
	SetTextColor(hDC, oldTxt);
	if (m_edtRename)
	{
		m_edtRename->OnDraw(poSurface, hDC, rcTxt);
	}
	
}

VOID CRXSkinListCtrlItem::_DrawHotkey(SkSurface* poSurface, HDC hDC)
{
	if (m_bRowSelected && m_poHotkeyCtrl)
	{
		CRXSkinRect rcHot = m_poHotkeyCtrl->GetRect();
		CRXSkinRect rcTxt = GetRect();
		rcTxt.left = rcHot.right;
		rcTxt.left += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
		INT32 nModel = SetBkMode(hDC,TRANSPARENT);
		HFONT hOld = (HFONT)SelectObject(hDC,CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
		tstring strTxt = _T("直接按键盘进行设置");
		SkCanvas* poCanvas = poSurface->getCanvas();
		if (poCanvas)
		{
			CRXSkinFontMgr::Instance()->DrawText(poCanvas,
				strTxt, rcTxt,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalTxt(),
				ID_FONT_COMMON, m_dwDrawStyle);
		}
		SelectObject(hDC,hOld);
		SetBkMode(hDC,nModel);
	}
}

VOID CRXSkinListCtrlItem::_DrawColumn(SkSurface* poSurface, HDC hDC)
{
	CRXSkinRect rcItem = GetRect();
	COLORREF clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListCtrlHeaderNormal();
	switch (GetState())
	{
	case CONTROL_STATE_NORMAL:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemNormalTxtColor();
	}
	break;
	case CONTROL_STATE_MOUSEON:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemMouseOnTxtColor();
		DrawGradientRect(hDC, TRUE, rcItem,
			CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemMouseOnFrom(),
			CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemMouseOnTo());
	}
	break;
	case CONTROL_STATE_PRESSED:
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetListItemPressedTxtColor();
		DrawGradientRect(hDC, TRUE, rcItem,
			CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemSelecedFrom(),
			CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemSelecedTo());

	}
	break;
	default:
		break;
	}
	if (m_bCanSort)
	{
		switch (m_enSortType)
		{
		case LIST_CTRL_SORT_NONE:
		break;
		case LIST_CTRL_SORT_UP:
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_IMAGE_UP_ARROW);
			if (poImage)
			{
				CRXSkinRect rcImage = rcItem;
				rcImage.left += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
				rcImage.top = rcImage.top + (rcItem.Height() - poImage->GetHeight()) / 2;
				rcImage.right = rcImage.left + poImage->GetWidth();
				rcImage.bottom = rcImage.top + poImage->GetHeight();
				poImage->Stretch(hDC,rcImage);
			}
		}
		break;
		case LIST_CTRL_SORT_DOWN:
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_IMAGE_DOWN_ARROW);
			if (poImage)
			{
				CRXSkinRect rcImage = rcItem;
				rcImage.left += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
				rcImage.top = rcImage.top + (rcItem.Height() - poImage->GetHeight()) / 2;
				rcImage.right = rcImage.left + poImage->GetWidth();
				rcImage.bottom = rcImage.top + poImage->GetHeight();
				poImage->Stretch(hDC, rcImage);
			}
		}
		break;
		default:
			break;
		}
	}

	CRXSkinRect rcTxt = rcItem;
	tstring strTxt = GetText().GetData();
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas,
			strTxt, rcTxt,
			clrTxt,
			ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	}

	if (FALSE == m_bLasted)
	{
		POINT ptStart;
		POINT ptEnd;
		ptStart.x = rcItem.right - 1;
		ptStart.y = rcItem.top + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetY();
		ptEnd.x = rcItem.right - 1;
		ptEnd.y = rcItem.bottom - CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetY();

		HPEN	hpen;
		HGDIOBJ hpenOld;

		hpen = ::CreatePen( PS_SOLID, 1, CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBorder() );
		hpenOld = ::SelectObject( hDC, hpen );
		::MoveToEx( hDC, ptStart.x, ptStart.y, NULL );
		::LineTo( hDC, ptEnd.x, ptEnd.y );
		::SelectObject( hDC, hpenOld );
		::DeleteObject( hpen );
	}
}

CRXSkinItem* CRXSkinListCtrlItem::HitTest(POINT pt)
{
	return NULL;
}

EListItemType CRXSkinListCtrlItem::GetType()
{
	return m_enType;
}

VOID CRXSkinListCtrlItem::SetType(EListItemType enType)
{
	m_enType = enType;
}

UINT32 CRXSkinListCtrlItem::GetPercent()
{
	return m_dwPercent;
}

VOID CRXSkinListCtrlItem::SetPercent(const UINT32 dwPercent)
{
	m_dwPercent = dwPercent;
}
UINT32 CRXSkinListCtrlItem::GetProgress()
{
	return m_dwProgress;
}

VOID CRXSkinListCtrlItem::SetProgress(const UINT32 dwPercent)
{
	m_dwProgress = dwPercent;
}
VOID CRXSkinListCtrlItem::SetLasted(const BOOL bLasted)
{
	m_bLasted = bLasted;
}

BOOL CRXSkinListCtrlItem::IsLasted()
{
	return m_bLasted;
}

VOID CRXSkinListCtrlItem::CreateHotkey()
{
	if (NULL == m_poHotkeyCtrl)
	{
		m_poHotkeyCtrl = new CRXSkinHotkeyCtrl;
		CRXSkinRect rcHotkey = GetRect();
		rcHotkey.left = GetRect().left + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
		rcHotkey.top = GetRect().top + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetY();
		rcHotkey.right = rcHotkey.left + DEFAULT_HOTKEY_WIDTH;
		rcHotkey.bottom = rcHotkey.top + GetRect().Height() - CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetY()*2 ;
		m_poHotkeyCtrl->SetBorder(FALSE);
		m_poHotkeyCtrl->SetRect(rcHotkey);
		m_poHotkeyCtrl->SetParentWnd(GetParentWnd());
		m_poHotkeyCtrl->Create();
		m_poHotkeyCtrl->SetVisible(FALSE);
	}
}

BOOL CRXSkinListCtrlItem::IsVisibled()
{
	return m_bVisibled;
}

void CRXSkinListCtrlItem::SetChecked(const BOOL& bChecked)
{
	if (LIST_ITEM_CHECKBOX == m_enType)
	{
		m_bChecked = bChecked;
		Invalidate();
	}

}

VOID CRXSkinListCtrlItem::SetVisibled(const BOOL bVisibled)
{
	m_bVisibled = bVisibled;
}



CRXSkinItem* CRXSkinListCtrlHeader::HitTest(POINT pt)
{
	CRXSkinListCtrlHeaderItemMapItr itr = m_mapColumns.begin();
	for (;itr!=m_mapColumns.end();itr++)
	{
		if (itr->second->GetRect().PtInRect(pt))
		{
			return itr->second;
		}
	}
	return NULL;
}

VOID CRXSkinListCtrlHeader::_DeleteAll()
{
	CRXSkinListCtrlHeaderItemMapItr itr = m_mapColumns.begin();
	for (;itr!=m_mapColumns.end();itr++)
	{
		delete itr->second;
	}
	m_mapColumns.clear();
}


CRXSkinListCtrlHeader::CRXSkinListCtrlHeaderItemMap& CRXSkinListCtrlHeader::GetColumns()
{
	return m_mapColumns;
}

BOOL CRXSkinListCtrlHeader::AddColumn(CRXSkinListCtrlItem* poItem,const UINT32 dwPercent)
{
	CRXSkinListCtrlItem* poFindItem = FindColumn((uint32_t)poItem->GetID());
	if (poFindItem)
	{
		return FALSE;
	}
	poItem->SetPercent(dwPercent);
	poItem->SetType(LIST_ITEM_HEADER);
	m_mapColumns.insert(make_pair((uint32_t)poItem->GetID(),poItem));
	return TRUE;
}

CRXSkinListCtrlItem* CRXSkinListCtrlHeader::FindColumn(const UINT32 dwID)
{
	CRXSkinListCtrlHeaderItemMapItr itr = m_mapColumns.find(dwID);
	if (itr!=m_mapColumns.end())
	{
		return itr->second;
	}
	return NULL;
}

UINT32 CRXSkinListCtrlHeader::GetHeight()
{
	return CRXSkinService::Instance()->GetDrawMgr().GetListCtrlHeaderHeight();
}

CRXSkinListCtrlItem* CRXSkinListCtrlHeader::RemoveColumn(const UINT32 dwID)
{
	CRXSkinListCtrlHeaderItemMapItr itr = m_mapColumns.find(dwID);
	if (itr!=m_mapColumns.end())
	{
		CRXSkinListCtrlItem* poItem = itr->second;
		m_mapColumns.erase(itr);
		return poItem;
	}
	return NULL;
}

//  行

CRXSkinListCtrlRow::CRXSkinListCtrlRow()
{
	m_bIsSelected = false;
	m_lParam = NULL;
}

CRXSkinListCtrlRow::~CRXSkinListCtrlRow()
{
	_DeleteAll();
	if (m_lParam)
	{
		free((void*)m_lParam);
	}
}

UINT32 CRXSkinListCtrlRow::GetHeight()
{
	return CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemHeight();
}

VOID CRXSkinListCtrlRow::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	COLORREF oldClr = RGB(0,0,0);
	if (m_bIsSelected)
	{
		CRXSkinRect rcBK = GetRect();
		rcBK.left += 1;
		rcBK.right -= 1;
		DrawGradientRect(hDC, TRUE, rcBK, m_poOwner->GetRowSelectedBKFrom(),
			m_poOwner->GetRowSelectedBKTo());
		oldClr = SetTextColor(hDC, m_poOwner->GetRowSelectedText());
	}
	else
	{
		switch (GetState())
		{
		case ITEM_SELECTED:
		{
			CRXSkinRect rcBK = GetRect();
			rcBK.left += 1;
			rcBK.right -= 1;
			DrawGradientRect(hDC, TRUE, rcBK, m_poOwner->GetRowSelectedBKFrom(),
				m_poOwner->GetRowSelectedBKTo());
			oldClr = SetTextColor(hDC, m_poOwner->GetRowSelectedText());
		}
		break;
		case ITEM_MOUSEON:
		{
			CRXSkinRect rcBK = GetRect();
			rcBK.left += 1;
			rcBK.right -= 1;
			DrawGradientRect(hDC, TRUE, rcBK, m_poOwner->GetRowMouseOnBKFrom(),
				m_poOwner->GetRowMouseOnBKTo());
			oldClr = SetTextColor(hDC, m_poOwner->GetRowMouseOnText());
		}
		break;
		default:
			break;
		}
	}

	CRXSkinRect rcHeader = rcItem;
	CRXSkinListCtrlRowItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		rcHeader.right = rcHeader.left + (INT32)((double)rcItem.Width() * ((double)itr->second->GetPercent() / 100.00));
		CRXSkinListCtrlItem* poColumItem = m_poOwner->FindColumn(itr->first);
		if (poColumItem)
		{
			rcHeader.left = poColumItem->GetRect().left;
			rcHeader.right = poColumItem->GetRect().right;
			itr->second->OnDraw(poSurface,hDC, rcHeader);
		}

		rcHeader.left = rcHeader.right;
	}
	if (ITEM_SELECTED == GetState())
	{
		SetTextColor(hDC,oldClr);
	}
}

CRXSkinItem* CRXSkinListCtrlRow::HitTest(POINT pt)
{
	if (IsVisibled())
	{
		if (GetRect().PtInRect(pt))
		{
			return this;
		}
	}
	return NULL;
}

POINT CRXSkinListCtrlRow::GetWindowTopLeftPos()
{
	CRXSkinRect rc = GetRect();
	POINT pt;
	pt.x = rc.left;
	pt.y = rc.top;
	::ClientToScreen(m_poOwner->GetBody().GetHWnd(), &pt);
	return pt;
}


CRXSkinListCtrlItem* CRXSkinListCtrlRow::HitTestEx(POINT pt)
{
	CRXSkinListCtrlItem* poItem = NULL;
	CRXSkinListCtrlRowItemMapItr itr = m_mapItems.begin();
	for (;itr != m_mapItems.end();itr ++)
	{
		if (itr->second->GetRect().PtInRect(pt))
		{
			return itr->second;
		}
	}
	return poItem;
}

CRXSkinListCtrlRow::CRXSkinListCtrlRowItemMap& CRXSkinListCtrlRow::GetItems()
{
	return m_mapItems;
}

BOOL CRXSkinListCtrlRow::AddItem(CRXSkinListCtrlItem* poItem,const UINT32 dwPercent)
{
	CRXSkinListCtrlItem* poFindItem = FindItem((uint32_t)poItem->GetID());
	if (poFindItem)
	{
		return FALSE;
	}
	poItem->SetPercent(dwPercent);
	m_mapItems.insert(make_pair((uint32_t)poItem->GetID(),poItem));
	return TRUE;
}

CRXSkinListCtrlItem* CRXSkinListCtrlRow::FindItem(const UINT32 dwID)
{
	CRXSkinListCtrlRowItemMapItr itr = m_mapItems.find(dwID);
	if (itr!=m_mapItems.end())
	{
		return itr->second;
	}
	return NULL;
}

void CRXSkinListCtrlRow::RenameItem(const UINT32& dwID)
{
	CRXSkinListCtrlItem* poItem = FindItem(dwID);
	if (poItem)
	{
		poItem->Rename();
	}
}

void CRXSkinListCtrlRow::CancelRename()
{
	CRXSkinListCtrlRowItemMapItr itr = m_mapItems.begin();
	for (;itr != m_mapItems.end();itr++)
	{
		itr->second->CancelRename();
	}
}

CRXSkinListCtrlItem* CRXSkinListCtrlRow::RemoveItem(const UINT32 dwID)
{
	CRXSkinListCtrlRowItemMapItr itr = m_mapItems.find(dwID);
	if (itr!=m_mapItems.end())
	{
		CRXSkinListCtrlItem* poItem = itr->second;
		m_mapItems.erase(itr);
		return poItem;
	}
	return NULL;
}

VOID CRXSkinListCtrlRow::SetVisibled(const BOOL bVisibled)
{
	m_bVisibled = bVisibled;
	CRXSkinListCtrlRowItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		itr->second->SetVisibled(m_bVisibled);
	}
}

BOOL CRXSkinListCtrlRow::IsVisibled()
{
	return m_bVisibled;
}

BOOL CRXSkinListCtrlRow::IsRowSelected()
{
	if (GetState() == ITEM_SELECTED)
	{
		return TRUE;
	}
	return FALSE;
}

VOID CRXSkinListCtrlRow::SetRowSelected(const BOOL bRowSelected)
{
	m_bIsSelected = bRowSelected;
	CRXSkinListCtrlRowItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		itr->second->SetRowSelected(bRowSelected);
	}
}

VOID CRXSkinListCtrlRow::_DeleteAll()
{
	CRXSkinListCtrlRowItemMapItr itr = m_mapItems.begin();
	for (;itr!=m_mapItems.end();itr++)
	{
		delete itr->second;
	}
	m_mapItems.clear();
}

void CRXSkinListCtrlRow::SetOwner(CRXSkinListCtrl* poList)
{
	m_poOwner = poList;
}

//  行


CRXSkinListCtrl::CRXSkinListCtrl(void)
{
	m_dwRowID = 0;
	m_dwColumnID = 0;
	m_enType = LIST_CTRL_COMMON;
	m_poListener = NULL;
	m_poImageList = NULL;
	m_wndBody.SetOwner(this);
	m_poRenameRow = NULL;

	m_bCanDragged = false;
	m_bCanSort = false;

	m_clrSelectedRowFrom = CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemSelecedFrom();
	m_clrSelectedRowTo = CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemSelecedTo();
	m_clrSelectedRowText = CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemSelecedText();
	m_clrMouseOnRowFrom = CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemMouseOnFrom();
	m_clrMouseOnRowTo = CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemMouseOnTo();
	m_clrMouseOnRowText = CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemSelecedText();

	m_clrButtonMouseOnTo = CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnToColor();
	m_clrButtonMouseOnFrom = CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnFromColor();
	m_clrButtonMouseOnMiddle = CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnMiddleColor();


	m_clrButtonPressedTo = CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedToColor();
	m_clrButtonPressedFrom = CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedFromColor();
	m_clrButtonPressedMiddle = CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedMiddleColor();
}

CRXSkinListCtrl::~CRXSkinListCtrl(void)
{

}
void CRXSkinListCtrl::SetImageList(CRXSkinImageList* poImageList)
{
	m_poImageList = poImageList;
}

void CRXSkinListCtrl::CancelMouseOnHeader()
{

}

VOID CRXSkinListCtrl::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsEnabled() && IsVisible())
	{
		SetRect(rcItem);
		CRXSkinRect rcGroup = rcItem;
		DrawRect(hDC,
			CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBorder(),
			CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBK(),
			rcItem.left,
			rcItem.top,
			rcItem.right,
			rcItem.bottom);
		CRXSkinRect rcHeader = rcItem;
		if (m_enType == LIST_CTRL_COMMON)
		{
			rcHeader.bottom = rcHeader.top + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlHeaderHeight();			
		}
		else
		{
			rcHeader.bottom = rcHeader.top;
		}
	}
}

void CRXSkinListCtrl::CreateBody()
{
	m_wndBody.Create(_T("RXSkinListCtrlBody"), _T("RXSkinListCtrlBodyWnd"), WS_CHILD | WS_VISIBLE, GetParentWnd());
}
void CRXSkinListCtrl::CreateHeader()
{
	m_wndHeader.SetOwner(this);
	m_wndHeader.Create(_T("RXSkinListCtrlHeader"), _T("RXSkinListCtrlHeaderWnd"), WS_CHILD | WS_VISIBLE, GetParentWnd());
}
void CRXSkinListCtrl::SetColumnSorted(const uint32_t& dwColumnID, const bool& bSorted)
{
	CRXSkinListCtrlItem* poItem = m_wndHeader.FindColumn(dwColumnID);
	if (poItem)
	{
		poItem->SetCanSort(bSorted);
	}
}

bool mycomp_asc(CRXSkinListCtrlRow* poFrom, CRXSkinListCtrlRow* poTo) {
	CRXSkinListCtrlItem* poFromItem = poFrom->FindItem(poFrom->GetOwner()->GetCurSortColuminID());
	if (poFromItem == NULL) return false;
	CRXSkinListCtrlItem* poToItem = poTo->FindItem(poTo->GetOwner()->GetCurSortColuminID());
	if (poToItem == NULL) return false;
	if (poFromItem->GetValueType() == LIST_CTRL_VALUE_TYPE_STRING)
	{
		return _RXTStrcmp(poFromItem->GetText(), poToItem->GetText()) > 0;
	}
	else
		return poFromItem->GetIntValue() > poToItem->GetIntValue();

	return false;
}

bool mycomp_desc(CRXSkinListCtrlRow* poFrom, CRXSkinListCtrlRow* poTo) {
	CRXSkinListCtrlItem* poFromItem = poFrom->FindItem(poFrom->GetOwner()->GetCurSortColuminID());
	if (poFromItem == NULL) return false;
	CRXSkinListCtrlItem* poToItem = poTo->FindItem(poTo->GetOwner()->GetCurSortColuminID());
	if (poToItem == NULL) return false;
	if (poFromItem->GetValueType() == LIST_CTRL_VALUE_TYPE_STRING)
	{
		return _RXTStrcmp(poFromItem->GetText(), poToItem->GetText()) < 0;
	}
	else
		return poFromItem->GetIntValue() < poToItem->GetIntValue();

	return false;
}

void  CRXSkinListCtrl::Sort(const uint32_t& dwItemID,
	const ERXSkinListItemSortType& enSortType)
{
	m_enCurSortType = enSortType;
	m_dwCurSortColumn = dwItemID;
	if (enSortType == ERXSkinListItemSortType::LIST_CTRL_SORT_UP)
	{
		for (uint32_t t=0;t<(uint32_t)m_vecRows.size();t++)
		{
			std::sort(m_vecRows[t].begin(), m_vecRows[t].end(), mycomp_asc);
		}
	}
	else
	{
		for (uint32_t t = 0; t < (uint32_t)m_vecRows.size(); t++)
		{
			std::sort(m_vecRows[t].begin(), m_vecRows[t].end(), mycomp_desc);
		}
	}
}

void CRXSkinListCtrl::SetColumnValueType(const uint32_t& dwColumnID,
	const ERXSkinListItemValueType& enType)
{
	CRXSkinListCtrlItem* poItem = m_wndHeader.FindColumn(dwColumnID);
	if (poItem)
	{
		poItem->SetValueType(enType);
	}
}

void CRXSkinListCtrl::SetColumnCanResize(const uint32_t& dwColumnID, const bool& bCanResize)
{
	CRXSkinListCtrlItem* poItem = m_wndHeader.FindColumn(dwColumnID);
	if (poItem)
	{
		poItem->SetCanResize(bCanResize);
	}
}

void CRXSkinListCtrl::SetItemIntValue(const uint32_t& dwRowID, const uint32_t& dwItemID, const int64_t& llValue)
{
	CRXSkinListCtrlItem* poItem = FindItem(dwRowID, dwItemID);
	if (poItem)
	{
		poItem->SetIntValue(llValue);
	}
}
void CRXSkinListCtrl::SetItemValueType(const uint32_t& dwRowID,
	const uint32_t& dwItemID,
	const ERXSkinListItemValueType& enType)
{
	CRXSkinListCtrlItem* poItem = FindItem(dwRowID, dwItemID);
	if (poItem)
	{
		poItem->SetValueType(enType);
	}
}
VOID CRXSkinListCtrl::ReleaseControl()
{
	m_wndBody.Destroy();
	delete this;
}
BOOL CRXSkinListCtrl::PtInRect(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}
BOOL CRXSkinListCtrl::OnClick(POINT pt)
{
	return FALSE;
}
BOOL CRXSkinListCtrl::OnMouseMove(POINT pt)
{
	return FALSE;
}
BOOL CRXSkinListCtrl::OnLButtonDown(POINT pt)
{
	return PtInRect(pt);
}

BOOL CRXSkinListCtrl::OnLButtonUp(POINT pt)
{
	return FALSE;
}


BOOL CRXSkinListCtrl::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	BOOL bResult = CRXControl::OnSize(dwX, dwY);
	UpdateScrollBar();
	
	return TRUE;
}

VOID CRXSkinListCtrl::SetRect(const CRXSkinRect rcItem)
{
	CRXControl::SetRect(rcItem);
	CRXSkinRect rcBody = GetRect();
	rcBody.top += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlHeaderHeight();
	m_wndBody.MoveWindow(rcBody);
	CRXSkinRect rcHeader = GetRect();
	rcHeader.bottom = rcHeader.top + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlHeaderHeight();
	m_wndHeader.MoveWindow(rcHeader);
}

VOID CRXSkinListCtrl::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
	CRXSkinListCtrlRowMapItr itr = m_mapRows.begin();
	for (;itr!=m_mapRows.end();itr++)
	{
		itr->second->SetVisibled(bIsVisible);
	}
}

VOID CRXSkinListCtrl::_DeleteAll()
{
	CRXSkinListCtrlRowMapItr itr = m_mapRows.begin();
	for (;itr!=m_mapRows.end();itr++)
	{
		delete itr->second;
	}
	m_mapRows.clear();
	m_wndBody.ResetContent();
	for (uint32_t t=0;t<(uint32_t)m_vecRows.size();t++)
	{
		m_vecRows[t].clear();
	}	
}

UINT32 CRXSkinListCtrl::AddRow(uint32_t dwGroupID)
{
	if (dwGroupID>=m_vecRows.size())
	{
		return 9999999;
	}
	CRXSkinListCtrlRow* poRow = new CRXSkinListCtrlRow;
	poRow->SetOwner(this);
	poRow->SetID(m_dwRowID);
	poRow->SetParentWnd(m_wndBody.GetHWnd());
	m_mapRows.insert(make_pair(m_dwRowID,poRow));
	m_vecRows[dwGroupID].push_back(poRow);
	return m_dwRowID++;
}

void CRXSkinListCtrl::Finished()
{
}

CRXSkinListCtrlRow* CRXSkinListCtrl::FindRow(const UINT32 dwRowID)
{
	CRXSkinListCtrlRowMapItr itr = m_mapRows.find(dwRowID);
	if (itr!=m_mapRows.end())
	{
		return itr->second;
	}
	return NULL;
}

CRXSkinListCtrlItem* CRXSkinListCtrl::FindItem(const UINT32 dwRowID,const UINT32 dwItemID)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (poRow)
	{
		return poRow->FindItem(dwItemID);
	}
	return NULL;
}

VOID CRXSkinListCtrl::RemoveRow(const UINT32 dwRowID)
{
	CRXSkinListCtrlRowMapItr itr = m_mapRows.find(dwRowID);
	if (itr!=m_mapRows.end())
	{
		CRXSkinListCtrlRow* poRow = itr->second;
		if (m_poRenameRow == poRow)
			m_poRenameRow = NULL;
		m_wndBody.RemoveRow(poRow);
		for (uint32_t t=0;t<(uint32_t)m_vecRows.size();t++)
		{
			CRXSkinListCtrlRowVecItr itrVec = std::find(m_vecRows[t].begin(), m_vecRows[t].end(), poRow);
			if (itrVec != m_vecRows[t].end())
			{
				m_vecRows[t].erase(itrVec);
				break;
			}
		}

		delete poRow;
		m_mapRows.erase(itr);
	}
	Invalidate();
}

void CRXSkinListCtrl::SetItemTextStyle(const UINT32 dwRowID, const UINT32 dwItemID, const UINT32& dwStyle)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (poRow)
	{
		CRXSkinListCtrlItem* poItem = poRow->FindItem(dwItemID);
		if (poItem)
		{
			poItem->SetDrawStyle(dwStyle);
		}
	}
}

VOID CRXSkinListCtrl::SetItemText(const UINT32 dwRowID,const UINT32 dwItemID,const TCHAR* pszText,EListItemType enType)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (poRow)
	{
		CRXSkinListCtrlItem* poItem = poRow->FindItem(dwItemID);
		if (NULL == poItem)
		{
			poItem = new CRXSkinListCtrlItem;
			poItem->SetText(pszText);
			poItem->SetRowID(dwRowID);
			poItem->SetID(dwItemID);
			poItem->SetOwner(this);
			poItem->SetParentWnd(m_wndBody.GetHWnd());
			poItem->SetType(enType);
			poItem->SetRect(_GetItemRect(dwRowID,dwItemID));
			CRXSkinListCtrlItem* poColumn = m_wndHeader.FindColumn(dwItemID);
			if (poColumn)
			{
				if (FALSE == poRow->AddItem(poItem,poColumn->GetPercent()))
				{
					delete poItem;
				}
				else
				{
					if (enType == LIST_ITEM_HOTKEY)
					{
						poItem->CreateHotkey();
					}
				}
			}
			else
			{
				delete poItem;
			}
		}
		else
		{
			poItem->SetText(pszText);
			poItem->SetType(enType);
		}
	}
	Invalidate();
}

VOID CRXSkinListCtrl::SetItemProgress(const UINT32 dwRowID, const UINT32 dwItemID, const uint32_t& dwProgress)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (poRow)
	{
		CRXSkinListCtrlItem* poItem = poRow->FindItem(dwItemID);
		if (NULL == poItem)
		{
			poItem = new CRXSkinListCtrlItem;
			poItem->SetText(_T(""));
			poItem->SetRowID(dwRowID);
			poItem->SetID(dwItemID);
			poItem->SetOwner(this);
			poItem->SetParentWnd(m_wndBody.GetHWnd());
			poItem->SetType(EListItemType::LIST_ITEM_PROGRESS);
			poItem->SetProgress(dwProgress);
			poItem->SetRect(_GetItemRect(dwRowID, dwItemID));
			CRXSkinListCtrlItem* poColumn = m_wndHeader.FindColumn(dwItemID);
			if (poColumn)
			{
				if (FALSE == poRow->AddItem(poItem, poColumn->GetPercent()))
				{
					delete poItem;
				}
			}
			else
			{
				delete poItem;
			}
		}
		else
		{
			poItem->SetProgress(dwProgress);
			poItem->SetType(EListItemType::LIST_ITEM_PROGRESS);
		}
	}
	Invalidate();
}

VOID CRXSkinListCtrl::SetItemImage(const UINT32 dwRowID, const UINT32 dwItemID, const INT32& nImageID)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (poRow)
	{
		CRXSkinListCtrlItem* poItem = poRow->FindItem(dwItemID);
		if (poItem)
		{
			poItem->SetImageID(nImageID);
		}
	}
	Invalidate();
}

VOID CRXSkinListCtrl::SetItemImageList(const UINT32 dwRowID, const UINT32 dwItemID, CRXSkinImageList* poList)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (poRow)
	{
		CRXSkinListCtrlItem* poItem = poRow->FindItem(dwItemID);
		if (poItem)
		{
			poItem->SetImageList(poList);
		}
	}
}

VOID CRXSkinListCtrl::SetItemImageEx(const UINT32 dwRowID,
	const UINT32 dwItemID,
	IRXImage* poImage,
	EListItemType enType,
	const TCHAR* pszText)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (poRow)
	{
		CRXSkinListCtrlItem* poItem = poRow->FindItem(dwItemID);
		if (poItem)
		{
			poItem->SetImage(poImage);
			poItem->SetType(enType);
			poItem->SetText(pszText);
		}
		else
		{
			poItem = new CRXSkinListCtrlItem;
			poItem->SetText(pszText);
			poItem->SetRowID(dwRowID);
			poItem->SetID(dwItemID);
			poItem->SetOwner(this);
			poItem->SetParentWnd(m_wndBody.GetHWnd());
			poItem->SetType(enType);
			poItem->SetRect(_GetItemRect(dwRowID, dwItemID));
			poItem->SetImage(poImage);
			CRXSkinListCtrlItem* poColumn = m_wndHeader.FindColumn(dwItemID);
			if (poColumn)
			{
				if (FALSE == poRow->AddItem(poItem, poColumn->GetPercent()))
				{
					delete poItem;
				}
			}
			else
			{
				delete poItem;
			}
		}
	}
	Invalidate();
}
CRXSkinRect CRXSkinListCtrl::_GetItemRect(const UINT32 dwRowID,const UINT32 dwItemID)
{
	CRXSkinRect rcItem = GetRect();
	if (m_enType == LIST_CTRL_COMMON)
	{
		rcItem.top += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlHeaderHeight();
	}
	rcItem.top += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemHeight()*dwRowID;
	rcItem.bottom = rcItem.top + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemHeight();
	CRXSkinListCtrlHeader::CRXSkinListCtrlHeaderItemMap& oMap = m_wndHeader.GetColumns();
	CRXSkinListCtrlHeader::CRXSkinListCtrlHeaderItemMapItr itr = oMap.begin();
	for (;itr!=oMap.end();itr++)
	{
		if (itr->first < dwItemID)
		{
			rcItem.left += (INT32)(((double)itr->second->GetPercent() / 100.00)*GetRect().Width());
		}
		else if (itr->first == dwItemID)
		{
			rcItem.right = rcItem.left + (INT32)(((double)itr->second->GetPercent() / 100.00)*GetRect().Width());;
		}
	}
	return rcItem;
}

CRXSkinString CRXSkinListCtrl::GetItemText(const UINT32 dwRowID,const UINT32 dwItemID)
{
	CRXSkinString strTxt = _T("");
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (poRow)
	{
		CRXSkinListCtrlItem* poItem = poRow->FindItem(dwItemID);
		if (poItem)
		{
			strTxt = poItem->GetText();
		}
	}
	return strTxt;
}

UINT32 CRXSkinListCtrl::AddColumn(const TCHAR* pszText,const UINT32 dwPercent)
{
	CRXSkinListCtrlItem* poItem = new CRXSkinListCtrlItem;
	poItem->SetParentWnd(m_wndHeader.GetHWnd());
	poItem->SetID(m_dwColumnID);
	poItem->SetType(LIST_ITEM_HEADER);
	poItem->SetText(pszText);
	poItem->SetOwner(this);
	if (FALSE == m_wndHeader.AddColumn(poItem,dwPercent))
	{
		delete poItem;
		return -1;
	}
	Invalidate();
	return m_dwColumnID++;
}

VOID CRXSkinListCtrl::RemoveColumn(const UINT32 dwColumnID)
{
	CRXSkinListCtrlItem* poColumn = m_wndHeader.RemoveColumn(dwColumnID);
	if (poColumn)
	{
		delete poColumn;
	}
	CRXSkinListCtrlRowMapItr itr = m_mapRows.begin();
	for (;itr!=m_mapRows.end();itr++)
	{
		CRXSkinListCtrlItem* poItem = itr->second->RemoveItem(dwColumnID);
		if (poItem)
		{
			delete poItem;
		}
	}
	Invalidate();
}

CRXSkinListCtrlItem* CRXSkinListCtrl::FindColumn(const uint32_t& dwID)
{
	return m_wndHeader.FindColumn(dwID);
}

VOID CRXSkinListCtrl::ResetContent()
{
	_DeleteAll();
	m_poRenameRow = NULL;
	m_wndBody.ResetContent();
	Invalidate();
}

EListCtrlType CRXSkinListCtrl::GetType()
{
	return m_enType;
}

VOID CRXSkinListCtrl::SetType(EListCtrlType enType)
{
	m_enType = enType;
}

void CRXSkinListCtrl::UpdateScrollBar()
{
	m_wndBody.UpdateScrollBar();
	Invalidate();
}
void  CRXSkinListCtrl::RenameRow(const uint32_t& dwRowID, const uint32_t& dwItemID)
{
	if (m_poRenameRow)
	{
		m_poRenameRow->CancelRename();
		m_poRenameRow = NULL;
	}
	m_poRenameRow = FindRow(dwRowID);
	if (m_poRenameRow)
	{
		m_poRenameRow->RenameItem(dwItemID);
	}
}

void CRXSkinListCtrl::SetRowVisible(const uint32_t& dwRowID, const bool& bIsVisible)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (poRow)
	{
		poRow->SetVisibled(bIsVisible);
	}
}

void CRXSkinListCtrl::CancelRename()
{
	if (m_poRenameRow)
	{
		m_poRenameRow->CancelRename();
		m_poRenameRow->Invalidate();
		m_poRenameRow = NULL;

	}
}

BOOL CRXSkinListCtrl::OnLButtonDblClk(POINT pt)
{
	BOOL bResult = CRXControl::OnLButtonDblClk(pt);

	return bResult;
}

BOOL CRXSkinListCtrl::OnRButtonUp(POINT pt)
{
	return FALSE;
}

void CRXSkinListCtrl::SetRowData(const UINT32 dwRowID, LPARAM lParam)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (poRow)
	{
		poRow->SetData(lParam);
	}
}

LPARAM CRXSkinListCtrl::GetRowData(const UINT32 dwRowID)
{
	return m_wndBody.GetRowData(dwRowID);
}

INT32 CRXSkinListCtrl::GetItemImageID(const UINT32 dwRowID, const UINT32 dwItemID)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (NULL == poRow)
	{
		return -1;
	}
	CRXSkinListCtrlItem* poItem = poRow->FindItem(dwItemID);
	if (NULL == poItem)
	{
		return -1;
	}
	return poItem->GetImageID();
}

LPARAM CRXSkinListCtrl::GetCurFirstRowData()
{
	return m_wndBody.GetCurFirstRowData();
}

CRXSkinListCtrlRow* CRXSkinListCtrl::GetCurFirstRow()
{
	return m_wndBody.GetCurFirstRow();
}



BOOL CRXSkinListCtrl::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	if (IsVisible() == FALSE || IsEnabled() == FALSE)
	{
		return FALSE;
	}
	::ScreenToClient(GetParentWnd(), &pt);
	if (GetRect().PtInRect(pt)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

tstring CRXSkinListCtrl::GetItemRenameValue(const uint32_t& dwRowID, const uint32_t& dwItemID)
{
	CRXSkinListCtrlRow* poRow = FindRow(dwRowID);
	if (NULL == poRow)
	{
		return _T("");
	}
	CRXSkinListCtrlItem* poItem = poRow->FindItem(dwItemID);
	if (NULL ==poItem)
	{
		return _T("");
	}
	return poItem->GetRenameValue();
}

CRXSkinListCtrlBody::CRXSkinListCtrlBody()
{
	m_poOwner = NULL;
	m_oScrollbar.SetVisible(FALSE);
	m_oScrollbar.SetType(SCROLLBAR_VER);
	m_nTotalHeight = 0;
	m_nTopOffSet = 0;
	m_vecSelected.clear();
	m_poMouseOn = NULL;	
	m_poMouseOnItem = NULL;
	m_bDragging = false;
	//m_oScrollbar.SetOwner(this);
}

CRXSkinListCtrlBody::~CRXSkinListCtrlBody()
{
}



BOOL CRXSkinListCtrlBody::OnLButtonUp(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinChildWnd::OnLButtonUp(nFlags, point);
	if (m_oScrollbar.IsVisible())
	{
		m_oScrollbar.OnClick(point);
	}
	if (m_bDragging)
	{
		m_bDragging = false;
		CRXSkinWnd* poParentWnd = CRXSkinService::Instance()->FindWnd(m_poOwner->GetParentWnd());
		if (poParentWnd)
		{
			RECT rcWind;
			::GetWindowRect(poParentWnd->GetHWnd(), &rcWind);
			POINT pt = point;
			ClientToScreen(m_hWnd, &pt);
			pt.x = pt.x - rcWind.left;
			pt.y = pt.y - rcWind.top;
			CRXControl* poSelectControl = poParentWnd->HitTest(pt);
			if (poSelectControl)
			{
				if (m_poOwner->GetListener())
				{
					m_poOwner->GetListener()->OnListCtrlEndDragged(m_poOwner->GetID(), poSelectControl->GetID());

				}
			}
		}
		m_wndDrag.ShowDrag(FALSE);
		ReleaseCapture();
		::KillTimer(m_hWnd, BEGIN_DRAGGING_TIMER_ID);
		return TRUE;
	}

	m_poOwner->CancelRename();
	return bResult;
}

BOOL CRXSkinListCtrlBody::OnTimer(WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinChildWnd::OnTimer(wParam, lParam);
	if (BEGIN_DRAGGING_TIMER_ID == wParam)
	{
		::KillTimer(m_hWnd,BEGIN_DRAGGING_TIMER_ID);
		if (m_poOwner->CanDragged())
		{
			if (m_vecSelected.size()>0)
			{
			
				if (m_poOwner->GetListener())
				{
					m_bDragging = true;
					m_poOwner->GetListener()->OnListCtrlBeginDragged(m_poOwner->GetID());
					m_wndDrag.Clear();
					for (uint32_t t=0;t<(uint32_t)m_vecSelected.size();t++)
					{
						m_wndDrag.AddRow(m_vecSelected[t]);
					}
					POINT pt;
					GetCursorPos(&pt);
					ScreenToClient(m_hWnd, &pt);
					m_wndDrag.SetDragPosition(pt);
					m_wndDrag.ShowDrag(TRUE);
					::SetFocus(m_hWnd);
					SetCapture();
				}
				
			}
		
		}
		
	}
	return bResult;
}

BOOL CRXSkinListCtrlBody::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bClicked = CRXSkinChildWnd::OnLButtonDown(nFlags, point);
	if (FALSE == bClicked)
	{
		if (m_poOwner->CanDragged())
		{
			if (m_vecSelected.size()>0)
			{
				::SetTimer(m_hWnd, BEGIN_DRAGGING_TIMER_ID, 500, NULL);
			}			
		}


		if (::GetKeyState(VK_CONTROL) & 0x80)
		{

		}
		else
		{
			ClearSelected();
		}
		CRXSkinListCtrlRow* poRow = _HitTestRow(point);
		if (poRow)
		{
			if (false == IsSelected((uint32_t)poRow->GetID()))
			{
				poRow->SetState(ITEM_SELECTED);
				poRow->SetRowSelected(TRUE);
				poRow->Invalidate();
				m_vecSelected.push_back(poRow);
			}

			CRXSkinListCtrlItem* poItem = poRow->HitTestEx(point);
			if (poItem)
			{
				poItem->SetChecked(!poItem->IsChecked());
				if (poItem->GetType() == LIST_ITEM_IMAGE_BUTTON ||
					poItem->GetType() == LIST_ITEM_OPERATOR)
				{
					if (m_poOwner->GetListener())
					{
						m_poOwner->GetListener()->OnListCtrlCommandClicked((uint32_t)m_poOwner->GetID(), (uint32_t)poRow->GetID(), (uint32_t)poItem->GetID());
					}
					::PostMessage(GetParentWnd(), WM_RXSKIN_LISTCTRL_ITEM_CLICK, (uint32_t)poRow->GetID(), (uint32_t)poItem->GetID());
				}
			}
			m_poOwner->CancelRename();
			if (m_poOwner->GetListener())
			{
				m_poOwner->GetListener()->OnListCtrlSelectRow((uint32_t)m_poOwner->GetID(), (uint32_t)poRow->GetID());
			}
			return TRUE;
		}
		else
		{
			ClearSelected();
			if (m_poOwner->GetListener())
			{
				m_poOwner->GetListener()->OnListCtrlCancelRow(m_poOwner->GetID());
			}
		}
	}
	return bClicked;
}

CRXControl* CRXSkinListCtrlBody::OnCreateControl(tinyxml2::XMLElement* poElement)
{
	char* pszType = (char*)poElement->Attribute("type");

	return NULL;
}

BOOL CRXSkinListCtrlBody::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinChildWnd::OnCreate(lpCreate);
	if (bResult)
	{
		m_oScrollbar.SetParentWnd(GetHWnd());
		m_oScrollbar.SetStep(CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemHeight());
		m_wndDrag.Create(_T("RXSkinListCtrlDragClass"),
			_T("RXSkinListCtrlDragWnd"),
			WS_POPUP,
			m_hWnd,NULL);
	}
	return bResult;
}

void CRXSkinListCtrlBody::OnClickedClose()
{

}

BOOL CRXSkinListCtrlBody::Create(const TCHAR* pszWindowClassName,
	const TCHAR* pszWindowName,
	const UINT32 dwStyle,
	const HWND hParentWnd, HICON hIcon /* = NULL */)
{
	BOOL bResult = CRXSkinChildWnd::Create(pszWindowClassName, pszWindowName, dwStyle, hParentWnd, hIcon);
	if (bResult)
	{
	}
	return bResult;
}

void CRXSkinListCtrlBody::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	DrawRect(hDC,
		CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBorder(),
		CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBK(),
		rcItem.left,
		rcItem.top,
		rcItem.right,
		rcItem.bottom);
	CRXSkinRect rcRow = rcItem;
	rcRow.top -= m_nTopOffSet;
	rcRow.bottom = rcRow.top + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemHeight();
	CRXSkinRect rcDraw = rcItem;
	rcDraw.top = rcDraw.top;
	rcDraw.right -= m_oScrollbar.GetWidth();
	if (m_poOwner->IsOrderGroup())
	{
		CRXSkinListCtrl::CRXSkinListCtrlRowGroupVec& vec = m_poOwner->GetAllRowVec();
		for (uint32_t t=0;t<(uint32_t)vec.size();t++)
		{
			for (uint32_t m=0;m<(uint32_t)vec[t].size();m++)
			{
				CRXSkinListCtrlRow* poCurRow = vec[t][m];
				poCurRow->SetRect(rcRow);
				CRXSkinRect rcDrawItem;
				poCurRow->OnDraw(poSurface,hDC, rcRow);
				rcRow.top = rcRow.bottom;
				rcRow.bottom = rcRow.top + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemHeight();				
			}
		}
	}
	else
	{
		CRXSkinListCtrl::CRXSkinListCtrlRowGroupVec& vec = m_poOwner->GetAllRowVec();
		for (uint32_t t = 0; t < (uint32_t)vec.size(); t++)
		{
			for (int32_t m = (uint32_t)(vec[t].size()-1); m >= 0; m--)
			{
				CRXSkinListCtrlRow* poCurRow = vec[t][m];
				poCurRow->SetRect(rcRow);
				CRXSkinRect rcDrawItem;
				poCurRow->OnDraw(poSurface,hDC, rcRow);
				rcRow.top = rcRow.bottom;
				rcRow.bottom = rcRow.top + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlItemHeight();				
			}
		}
	}

	m_oScrollbar.OnDraw(poSurface,hDC, rcItem);
}


LRESULT CRXSkinListCtrlBody::OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CRXSkinChildWnd::OnHandleMessage(message, wParam, lParam);
}

BOOL CRXSkinListCtrlBody::OnMessage(UINT32 dwMessageID, WPARAM wParam, LPARAM lParam)
{
	return CRXSkinChildWnd::OnMessage(dwMessageID,
		wParam,
		lParam);
}

BOOL CRXSkinListCtrlBody::OnRButtonUp(UINT nFlags, POINT point)
{
	BOOL bResult = FALSE;
	if (GetRect().PtInRect(point))
	{
		CRXSkinListCtrlRow* poRow = _HitTestRow(point);
		if (poRow)
		{
			::SendMessage(m_poOwner->GetParentWnd(), WM_RXSKIN_LISTCTRL_RCLIEECKED, m_poOwner->GetID(), (LPARAM)poRow->GetID());
			return TRUE;
		}
		else
		{
			::SendMessage(m_poOwner->GetParentWnd(), WM_RXSKIN_LISTCTRL_RCLIEECKED, m_poOwner->GetID(), -1);
		}
	}
	m_poOwner->CancelRename();
	return m_oControlMgr.OnRButtonUp(point) == NULL ? FALSE : TRUE;
}

BOOL CRXSkinListCtrlBody::OnSize(WPARAM wParam, LPARAM lParam)
{
	CRXSkinRect rcClient;
	GetClientRectEx(rcClient);
	UpdateScrollBar();
	return CRXSkinChildWnd::OnSize(wParam, lParam);

}

BOOL CRXSkinListCtrlBody::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CRXSkinChildWnd::OnCommand(wParam, lParam);
	return TRUE;
}


CRXSkinListCtrlRow* CRXSkinListCtrlBody::_HitTestRow(POINT pt)
{
	CRXSkinListCtrlRow* poRow = NULL;
	CRXSkinListCtrl::CRXSkinListCtrlRowMapItr itr = m_poOwner->GetAllRow().begin();
	for (; itr != m_poOwner->GetAllRow().end(); itr++)
	{
		poRow = (CRXSkinListCtrlRow*)itr->second->HitTest(pt);
		if (poRow)
		{
			break;
		}
	}
	return poRow;
}

BOOL CRXSkinListCtrlBody::OnMouseMove(UINT nFlags, POINT point)
{
	m_poOwner->CancelMouseOnHeader();
	BOOL bResult = CRXSkinChildWnd::OnMouseMove(nFlags, point);
	if (m_oScrollbar.IsVisible())
	{
		if (m_oScrollbar.OnMouseMove(point))
			return TRUE;
	}
	if (FALSE == m_poApp->IsLeftMousePressed())
	{
		if (m_bDragging)
		{
			m_bDragging = false;
			POINT pt;
			GetCursorPos(&pt);
			CRXSkinWnd* poParentWnd = CRXSkinService::Instance()->FindWnd(m_poOwner->GetParentWnd());
			if (poParentWnd)
			{
				ScreenToClient(m_poOwner->GetParentWnd(), &pt);
				CRXControl* poSelectControl = poParentWnd->HitTest(pt);
				if (poSelectControl)
				{
					if (m_poOwner->GetListener())
					{
						m_poOwner->GetListener()->OnListCtrlEndDragged(m_poOwner->GetID(), poSelectControl->GetID());
					}
				}
			}
			m_wndDrag.ShowDrag(FALSE);
			ReleaseCapture();
			::KillTimer(m_hWnd, BEGIN_DRAGGING_TIMER_ID);
			return TRUE;
		}
	}
	if (m_bDragging)
	{
		m_wndDrag.SetDragPosition(point);
		m_wndDrag.ShowDrag(TRUE);
		return TRUE;
	}
	CRXSkinRect rcClient;
	GetClientRectEx(rcClient);
	if (rcClient.PtInRect(point) == FALSE)
	{
		if (m_poMouseOnItem)
		{
			m_poMouseOnItem->SetState(ITEM_NORMAL);
			m_poMouseOnItem->Invalidate();
			m_poMouseOnItem = NULL;
		}
	}
	else
	{
		CRXSkinListCtrlRow* poRow = _HitTestRow(point);
		if (poRow)
		{
			if (m_poMouseOn)
			{
				if (m_poMouseOn != poRow)
				{
					m_poMouseOn->SetState(ITEM_NORMAL);
					m_poMouseOn->Invalidate();
					m_poMouseOn = poRow;
					m_poMouseOn->SetState(ITEM_MOUSEON);
					m_poMouseOn->Invalidate();
				}
			}
			else
			{
				m_poMouseOn = poRow;
				m_poMouseOn->SetState(ITEM_MOUSEON);
				m_poMouseOn->Invalidate();
			}
			CRXSkinListCtrlItem* poItem = poRow->HitTestEx(point);
			if (poItem)
			{
				if (m_poMouseOnItem == NULL)
				{
					m_poMouseOnItem = poItem;
					m_poMouseOnItem->SetState(ITEM_MOUSEON);
					m_poMouseOnItem->Invalidate();
				}
				else
				{
					if (m_poMouseOnItem!=poItem)
					{
						m_poMouseOnItem->SetState(ITEM_NORMAL);
						m_poMouseOnItem->Invalidate();
						m_poMouseOnItem = poItem;
						m_poMouseOnItem->SetState(ITEM_MOUSEON);
						m_poMouseOnItem->Invalidate();
					}
				}
			}
			else
			{
				if (m_poMouseOnItem)
				{
					m_poMouseOnItem->SetState(ITEM_NORMAL);
					m_poMouseOnItem->Invalidate();
					m_poMouseOnItem = NULL;
				}
			}
		}
		else
		{
			if (m_poMouseOnItem)
			{
				m_poMouseOnItem->SetState(ITEM_NORMAL);
				m_poMouseOnItem->Invalidate();
				m_poMouseOnItem = NULL;
			}
		}
	}
	UpdateToolTipEx(m_poMouseOnItem);
	return bResult;
}

void CRXSkinListCtrlBody::RemoveSelected(const uint32_t& dwID)
{
	for (vector<CRXSkinListCtrlRow*>::iterator itr = m_vecSelected.begin();
		itr!=m_vecSelected.end();
		itr++)
	{
		if ((*itr)->GetID()==dwID)
		{
			m_vecSelected.erase(itr);
			break;
		}
	}
}

void CRXSkinListCtrlBody::RemoveRow(CRXSkinListCtrlRow* poRow)
{
	RemoveSelected((uint32_t)poRow->GetID());

	if (poRow==m_poMouseOn)
	{
		m_poMouseOn = NULL;
	}
	if (m_poMouseOnItem && (uint32_t)poRow->GetID() == (uint32_t)m_poMouseOnItem->GetRowID())
	{
		m_poMouseOnItem = NULL;
	}
}

void CRXSkinListCtrlBody::UpdateToolTip(CRXControl* poControl)
{

}

bool CRXSkinListCtrlBody::IsSelected(const uint32_t& dwID)
{
	for (uint32_t t=0;t<(uint32_t)m_vecSelected.size();t++)
	{
		if (dwID == m_vecSelected[t]->GetID())
		{
			return true;
		}
	}
	return false;
}

void CRXSkinListCtrlBody::ClearSelected()
{
	for (uint32_t t = 0; t < (uint32_t)m_vecSelected.size(); t++)
	{
		m_vecSelected[t]->SetState(ITEM_NORMAL);
		m_vecSelected[t]->SetRowSelected(FALSE);
		m_vecSelected[t]->Invalidate();
	}
	m_vecSelected.clear();
}

void CRXSkinListCtrlBody::UpdateToolTipEx(CRXSkinListCtrlItem* poItem)
{
	if (poItem && poItem->IsEnabled())
	{
		tstring strToolTip = poItem->GetText().GetData();
		if (strToolTip.size() > 0)
		{
			POINT pt;
			pt.x = poItem->GetRect().right;
			pt.y = poItem->GetRect().bottom;
			GetCursorPos(&pt);
			ScreenToClient(m_hWnd, &pt);
			/*ClientToScreen(m_hWnd,&pt);*/
			m_oToolTip.SetToolTipPosition(pt);
			m_oToolTip.UpdateToolTipText(strToolTip.c_str(), CRXSkinService::Instance()->GetInstance());
			m_oToolTip.ShowToolTip(TRUE);
			//	::SetFocus(m_hWnd);
		}
		else
		{
			m_oToolTip.ShowToolTip(FALSE);
		}
	}
	else
	{
		m_oToolTip.ShowToolTip(FALSE);
	}
}

LRESULT CRXSkinListCtrlBody::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	if (m_poMouseOn)
	{
		m_poMouseOn->SetState(ITEM_NORMAL);
		m_poMouseOn->Invalidate();
		m_poMouseOn = NULL;
	}
	m_oToolTip.Hide();
	m_wndDrag.ShowDrag(FALSE);

	CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);

	m_oToolTip.ShowToolTip(FALSE);
	return 0;
}

void CRXSkinListCtrlBody::_ComputeTotalHeight()
{
	m_nTotalHeight = 0;
	CRXSkinListCtrl::CRXSkinListCtrlRowMapItr itr = m_poOwner->GetAllRow().begin();
	for (; itr != m_poOwner->GetAllRow().end(); itr++)
	{
		CRXSkinListCtrlRow* poGroup = itr->second;
		m_nTotalHeight += poGroup->GetHeight();
	}
}

void CRXSkinListCtrlBody::UpdateScrollBar()
{
	_ComputeTotalHeight();
	if (m_nTotalHeight > (INT32)(GetRect().Height()))
	{
		m_oScrollbar.SetVisible(TRUE);
		m_oScrollbar.SetTotalHeight(m_nTotalHeight, (INT32)GetRect().Height());
	}
	else
	{
		m_oScrollbar.SetVisible(FALSE);
	}
	Invalidate();
}

void CRXSkinListCtrlBody::ResetContent()
{
	m_poMouseOn = NULL;
	m_vecSelected.clear();
	m_nTopOffSet = 0;
	m_nTotalHeight = 0;
	m_poMouseOnItem = NULL;
}

LPARAM CRXSkinListCtrlBody::GetRowData(const UINT32 dwRowID)
{
	CRXSkinListCtrlRow* poRow = m_poOwner->FindRow(dwRowID);
	if (poRow)
	{
		return	poRow->GetData();
	}
	return NULL;
}
LPARAM CRXSkinListCtrlBody::GetCurFirstRowData()
{
	if (m_vecSelected.size()>0)
	{
		return m_vecSelected[0]->GetData();
	}
	return NULL;
}

CRXSkinListCtrlRow* CRXSkinListCtrlBody::GetCurFirstRow()
{
	if (m_vecSelected.size() > 0)
	{
		return m_vecSelected[0];
	}
	return NULL;
}

VOID CRXSkinListCtrlBody::OnScrollBarChanged(EScrollType enType, INT32 nPercent)
{
	if (enType == SCROLLBAR_VER)
	{
		CRXSkinRect rcClient = GetRect();
		m_nTopOffSet = ((m_nTotalHeight - rcClient.Height()) * nPercent) / 100;
		rcClient.right -= m_oScrollbar.GetWidth();
		Invalidate();
	}
}

BOOL CRXSkinListCtrlBody::OnLButtonDblClk(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinChildWnd::OnLButtonDblClk(nFlags, point);
	if (GetRect().PtInRect(point))
	{
		CRXSkinListCtrlRow* poRow = _HitTestRow(point);
		if (poRow)
		{
			::SendMessage(m_poOwner->GetParentWnd(), WM_RXSKIN_LISTCTRL_DB_CLIEECKED, m_poOwner->GetID(), (LPARAM)poRow->GetID());
			return TRUE;
		}
		else
		{

			::SendMessage(m_poOwner->GetParentWnd(), WM_RXSKIN_LISTCTRL_DB_CLIEECKED, m_poOwner->GetID(), -1);
		}
	}
	return bResult;
}

BOOL CRXSkinListCtrlBody::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	BOOL bResult = CRXSkinChildWnd::OnMouseWheel(nFlags, zDelta, pt);
	if (m_oScrollbar.OnMouseWheel(nFlags,zDelta,pt))
	{
		return TRUE;
	}
	return bResult;
}

uint32_t CRXSkinListCtrl::GetRowCount()
{
	return (uint32_t)m_mapRows.size();
}

bool CRXSkinListCtrl::CanDragged()
{
	return m_bCanDragged;
}
void CRXSkinListCtrl::SetCanDragged(const bool& bCanDragged)
{
	m_bCanDragged = bCanDragged;
}

void CRXSkinListCtrl::SetGroupNum(const uint32_t& dwNum)
{
	m_vecRows.clear();
	m_vecRows.resize(dwNum);
}



CRXSkinListCtrlDrag::CRXSkinListCtrlDrag()
	:m_hParent(NULL)
{
	
}

CRXSkinListCtrlDrag::~CRXSkinListCtrlDrag()
{
}

BOOL CRXSkinListCtrlDrag::Create(const TCHAR* pszWindowClassName,
	const TCHAR* pszWindowName,
	const UINT32 dwStyle,
	const HWND hParentWnd,
	HICON hIcon)
{
	if (FALSE == CRXSkinWnd::Create(pszWindowClassName, pszWindowName, dwStyle, hParentWnd, hIcon))
	{
		return FALSE;
	}
	m_hParent = hParentWnd;
	return TRUE;
}

BOOL CRXSkinListCtrlDrag::OnCreate(LPCREATESTRUCT lpCreate)
{
	if (FALSE == CRXSkinWnd::OnCreate(lpCreate))
	{
		return FALSE;
	}
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE)
		& ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
	::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(m_hWnd, 0, 128, LWA_ALPHA); // 128 is the opacity (50% transparent)

	return TRUE;
}

void CRXSkinListCtrlDrag::ShowDrag(BOOL bShow)
{
	if (bShow)
	{
		ShowWindow(m_hWnd, SW_SHOW);
		InvalidateRect(m_hWnd, NULL, FALSE);
	}
	else
	{
		ShowWindow(m_hWnd, SW_HIDE);
	}
}

uint32_t CRXSkinListCtrlDrag::GetMaxWidth()
{
	UINT32 dwMaxWidth = 0;
	for (uint32_t t=0;t<(uint32_t)m_vecRow.size();t++)
	{
		uint32_t dwCurMaxWidth = 0;
		CRXSkinListCtrlItem* poItem = m_vecRow[t]->FindItem(0);
		if (poItem)
		{
			dwCurMaxWidth += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
			dwCurMaxWidth += 16;
			dwCurMaxWidth += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
			SIZE szTxt;
			tstring strTxt = poItem->GetText().GetData();
			CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, strTxt, szTxt);
			dwCurMaxWidth += szTxt.cx;
			dwCurMaxWidth += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();

			if (dwCurMaxWidth>dwMaxWidth)
			{
				dwMaxWidth = dwCurMaxWidth;
			}

		}
	}
	return dwMaxWidth;
}

uint32_t CRXSkinListCtrlDrag::GetHeight()
{
	uint32_t dwHeight = 0;
	for (uint32_t t=0;t<(uint32_t)m_vecRow.size();t++)
	{
		dwHeight += m_vecRow[t]->GetHeight();
	}
	return dwHeight + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetY() * 2;
}

void CRXSkinListCtrlDrag::SetDragPosition(POINT pt)
{
	uint32_t dwHeight = GetHeight();
	uint32_t dwWidth = GetMaxWidth();

	ClientToScreen(m_hParent, &pt);
	INT32 nCusorY = GetSystemMetrics(SM_CXCURSOR);
	::MoveWindow(m_hWnd, pt.x, pt.y + nCusorY, dwWidth, dwHeight, TRUE);
}

void CRXSkinListCtrlDrag::Clear()
{
	m_vecRow.clear();
}

void CRXSkinListCtrlDrag::AddRow(CRXSkinListCtrlRow* poRow)
{
	m_vecRow.push_back(poRow);
}

void CRXSkinListCtrlDrag::OnFinalMessage(HWND hWnd)
{
	CRXSkinWnd::OnFinalMessage(hWnd);
}

VOID CRXSkinListCtrlDrag::OnPaint()
{
	PAINTSTRUCT ps = { 0 };
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	CRXSkinMemDC memDC(hDC, rcClient);
	DrawRect(memDC.GetHDC(),		
		RGB(250, 250, 250),
		rcClient.left,
		rcClient.top,
		rcClient.right,
		rcClient.bottom);
	CRXSkinRect rcItem = rcClient;
	for (uint32_t t=0;t<(uint32_t)m_vecRow.size();t++)
	{
		rcItem.bottom = rcItem.top + m_vecRow[t]->GetHeight();
		CRXSkinListCtrlItem* poItem = m_vecRow[t]->FindItem(0);
		if (poItem)
		{
			CRXSkinRect rcTxt = rcItem;
			rcTxt.left += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
			CRXSkinImageList* poImageList = m_vecRow[t]->FindItem(0)->GetImageList();
			if (NULL == poImageList)
			{
				poImageList = m_vecRow[t]->GetOwner()->GetImageList();
			}
			int32_t nImageID = poItem->GetImageID();
			if (nImageID >= 0 && poImageList)
			{
				int cx = poImageList->GetX();
				int cy = poImageList->GetY();
				RGBQUAD tran;
				tran.rgbBlue = 0;
				tran.rgbGreen = 0;
				tran.rgbRed = 0;
				tran.rgbReserved = 0;
				if (poImageList->CheckImageIndex(nImageID))
				{
					poImageList->GetImage()->DrawTrans(memDC.GetHDC(),
						tran,
						rcTxt.left,
						rcTxt.top + (rcTxt.Height() - cy) / 2,
						cx,
						cy,
						0,
						nImageID * cy
					);
				}

				rcTxt.left += cx;
				rcTxt.left += CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX();
			}
			HFONT hOld = (HFONT)SelectObject(memDC.GetHDC(), CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
			COLORREF oldTxt = SetTextColor(memDC.GetHDC(), RGB(0,0,0));
			INT32 nModel = ::SetBkMode(memDC.GetHDC(), TRANSPARENT);
			SkCanvas* poCanvas = memDC.GetSurface()->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas,
					poItem->GetText().GetData(), rcTxt,
					RGB(0, 0, 0),
					ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_LEFT);
			}
			SelectObject(memDC.GetHDC(), hOld);
			SetBkMode(memDC.GetHDC(), nModel);
			SetTextColor(memDC.GetHDC(), oldTxt);
		}
		rcItem.top = rcItem.bottom;
	}
	
	memDC.TakeSnapshot();
	::EndPaint(m_hWnd, &ps);
}

BOOL  CRXSkinListCtrlDrag::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinWnd::OnLButtonDown(nFlags, point);
	ShowDrag(FALSE);
	return bResult;
}

BOOL  CRXSkinListCtrlDrag::OnLButtonUp(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinWnd::OnLButtonUp(nFlags, point);
	ShowDrag(FALSE);
	return bResult;
}




CRXSkinListCtrlHeader::CRXSkinListCtrlHeader()
{
	m_poOwner = NULL;
	m_poMouseOn = NULL;
	m_poMouseOnItem = NULL;
	m_bDragging = false;
	m_poSortedHeaderItem = NULL;
	m_poResingHeaderItem = NULL;
}

CRXSkinListCtrlHeader::~CRXSkinListCtrlHeader()
{
	_DeleteAll();
}



BOOL CRXSkinListCtrlHeader::OnLButtonUp(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinChildWnd::OnLButtonUp(nFlags, point);
	if (m_poResingHeaderItem)
	{
		m_poResingHeaderItem->SetResing(FALSE);
		ReleaseCapture();
		m_poResingHeaderItem = NULL;
		return TRUE;
	}
	CRXSkinListCtrlItem* poItem = (CRXSkinListCtrlItem*)HitTest(point);
	if (poItem)
	{
		if (false == poItem->IsCanSort())
		{
			poItem = NULL;
		}
	}
	if (poItem)
	{
		if (m_poSortedHeaderItem)
		{
			if (m_poSortedHeaderItem != poItem)
			{
				m_poSortedHeaderItem->SetSortType(ERXSkinListItemSortType::LIST_CTRL_SORT_NONE);
				m_poSortedHeaderItem->Invalidate();
			}
		}
		poItem->ChangeSortModel();
		m_poSortedHeaderItem = poItem;
	}
	return bResult;
}

BOOL CRXSkinListCtrlHeader::OnTimer(WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinChildWnd::OnTimer(wParam, lParam);
	if (BEGIN_DRAGGING_TIMER_ID == wParam)
	{
		::KillTimer(m_hWnd, BEGIN_DRAGGING_TIMER_ID);
	}
	return bResult;
}

BOOL CRXSkinListCtrlHeader::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bClicked = CRXSkinChildWnd::OnLButtonDown(nFlags, point);
	if (FALSE == bClicked)
	{
		CRXSkinListCtrlItem* poItem = (CRXSkinListCtrlItem*)HitTest(point);
		if (poItem && poItem->CanResize())
		{			
			CRXSkinRect rcResize = poItem->GetRect();
			rcResize.left = rcResize.right - CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX() * 2;
			rcResize.right = poItem->GetRect().right + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX() * 2;
			if (rcResize.PtInRect(point))
			{
				SetCapture();
				m_poResingHeaderItem = poItem;
				m_poResingHeaderItem->SetResingRect(m_poResingHeaderItem->GetRect());
				m_poResingHeaderItem->SetResing(true);
			}
		}
		else
		{
			ReleaseCapture();
			m_poResingHeaderItem = NULL;
		}
	}
	return bClicked;
}

CRXControl* CRXSkinListCtrlHeader::OnCreateControl(tinyxml2::XMLElement* poElement)
{
	char* pszType = (char*)poElement->Attribute("type");

	return NULL;
}

BOOL CRXSkinListCtrlHeader::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinChildWnd::OnCreate(lpCreate);
	if (bResult)
	{
	}
	return bResult;
}

void CRXSkinListCtrlHeader::OnClickedClose()
{

}

BOOL CRXSkinListCtrlHeader::Create(const TCHAR* pszWindowClassName,
	const TCHAR* pszWindowName,
	const UINT32 dwStyle,
	const HWND hParentWnd, HICON hIcon /* = NULL */)
{
	BOOL bResult = CRXSkinChildWnd::Create(pszWindowClassName, pszWindowName, dwStyle, hParentWnd, hIcon);
	if (bResult)
	{
	}
	return bResult;
}

void CRXSkinListCtrlHeader::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	CRXSkinRect rcHeader = rcItem;
	CRXSkinRect rcBK = rcHeader;
	DrawRect(hDC, CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBorder(),
		CRXSkinService::Instance()->GetDrawMgr().GetListCtrlHeaderFrom(), rcHeader.left,
		rcHeader.top,rcHeader.right,rcHeader.bottom);
	rcBK.top += 1;
	rcBK.bottom -= 1;
	rcBK.left += 1;
	rcBK.right -= 1;
	DrawGradientRect(hDC,
		TRUE,
		rcBK,
		CRXSkinService::Instance()->GetDrawMgr().GetListCtrlHeaderFrom(),
		CRXSkinService::Instance()->GetDrawMgr().GetListCtrlHeaderTo());
	INT32 nNumber = 0;
	CRXSkinListCtrlHeaderItemMapItr itr = m_mapColumns.begin();
	for (; itr != m_mapColumns.end(); itr++)
	{
		rcHeader.right = rcHeader.left + itr->second->GetRect().Width();
		if (nNumber == m_mapColumns.size() - 1)
		{
			itr->second->SetLasted(TRUE);
		}
		else
		{
			itr->second->SetLasted(FALSE);
		}
		itr->second->OnDraw(poSurface,hDC, rcHeader);
		rcHeader.left = rcHeader.right;
		nNumber++;
	}
}


LRESULT CRXSkinListCtrlHeader::OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CRXSkinChildWnd::OnHandleMessage(message, wParam, lParam);
}

BOOL CRXSkinListCtrlHeader::OnMessage(UINT32 dwMessageID, WPARAM wParam, LPARAM lParam)
{
	return CRXSkinChildWnd::OnMessage(dwMessageID,
		wParam,
		lParam);
}

BOOL CRXSkinListCtrlHeader::OnRButtonUp(UINT nFlags, POINT point)
{
	return m_oControlMgr.OnRButtonUp(point) == NULL ? FALSE : TRUE;
}

BOOL CRXSkinListCtrlHeader::OnSize(WPARAM wParam, LPARAM lParam)
{
	CRXSkinRect rcClient =  GetRect();
	CRXSkinRect rcHeader = rcClient;
	INT32 nNumber = 0;
	CRXSkinListCtrlHeaderItemMapItr itr = m_mapColumns.begin();
	for (; itr != m_mapColumns.end(); itr++)
	{
		rcHeader.right = rcHeader.left + (INT32)((double)rcClient.Width() * ((double)itr->second->GetPercent() / 100.00));
		itr->second->SetRect(rcHeader);
		rcHeader.left = rcHeader.right;
		nNumber++;
	}
	Invalidate();
	return CRXSkinChildWnd::OnSize(wParam, lParam);

}

BOOL CRXSkinListCtrlHeader::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CRXSkinChildWnd::OnCommand(wParam, lParam);
	return TRUE;
}


BOOL CRXSkinListCtrlHeader::OnMouseMove(UINT nFlags, POINT point)
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
	if (m_poResingHeaderItem)
	{
		CRXSkinRect rcResing = m_poResingHeaderItem->GetRect();
		int32_t nLeft = rcResing.left + m_poResingHeaderItem->GetMinWidth();
		if (nLeft<point.x&& point.x > rcResing.left)
		{
			rcResing.right = point.x;
			m_poResingHeaderItem->SetRect(rcResing);
			Invalidate();
			m_poOwner->InvalideBody();
		}
		return FALSE;
	}
	BOOL bResult = TRUE;
	CRXSkinListCtrlItem* poItem = (CRXSkinListCtrlItem*)HitTest(point);
	if (poItem)
	{
		if (false == poItem->IsCanSort())
		{
			poItem = NULL;
		}
	}
	if (poItem)
	{
		if (m_poMouseOnItem == NULL)
		{
			m_poMouseOnItem = poItem;
			m_poMouseOnItem->SetState(ITEM_MOUSEON);
			m_poMouseOnItem->Invalidate();
		}
		else if (m_poMouseOnItem != poItem)
		{
			m_poMouseOnItem->SetState(ITEM_NORMAL);
			m_poMouseOnItem->Invalidate();
			m_poMouseOnItem = poItem;
			m_poMouseOnItem->SetState(ITEM_MOUSEON);
			m_poMouseOnItem->Invalidate();
		}
	}
	else
	{
		if (m_poMouseOnItem)
		{
			m_poMouseOnItem->SetState(ITEM_NORMAL);
			m_poMouseOnItem->Invalidate();
			m_poMouseOnItem = poItem;
		}
	}

	if (m_poMouseOnItem)
	{
		if (m_poMouseOnItem->CanResize())
		{
			CRXSkinRect rcResize = m_poMouseOnItem->GetRect();
			rcResize.left = rcResize.right - CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX() * 2;
			rcResize.right = m_poMouseOnItem->GetRect().right + CRXSkinService::Instance()->GetDrawMgr().GetListCtrlOffSetX() * 2;
			if (rcResize.PtInRect(point))
			{
				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_WE);
			}
			else
			{
				HCURSOR hCursor = ::GetCursor();
				if (hCursor == CRXSkinService::Instance()->GetDrawMgr().GetWECursor())
				{
					CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);
				}
			}
		}
		else
		{
			HCURSOR hCursor = ::GetCursor();
			if (hCursor == CRXSkinService::Instance()->GetDrawMgr().GetWECursor())
			{
				CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);
			}
		}
	}
	else
	{
		HCURSOR hCursor = ::GetCursor();
		if (hCursor == CRXSkinService::Instance()->GetDrawMgr().GetWECursor())
		{
			CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);
		}
	}
	return m_poMouseOnItem == NULL ? FALSE : TRUE;
}


LRESULT CRXSkinListCtrlHeader::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	m_oToolTip.Hide();
	CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);
	if (m_poMouseOnItem)
	{
		m_poMouseOnItem->SetState(ITEM_NORMAL);
		m_poMouseOnItem->Invalidate();
		m_poMouseOnItem = NULL;
	}
	m_oToolTip.ShowToolTip(FALSE);
	return 0;
}

BOOL CRXSkinListCtrlHeader::OnLButtonDblClk(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinChildWnd::OnLButtonDblClk(nFlags, point);
	return bResult;
}

BOOL CRXSkinListCtrlHeader::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	BOOL bResult = CRXSkinChildWnd::OnMouseWheel(nFlags, zDelta, pt);
	return bResult;
}


