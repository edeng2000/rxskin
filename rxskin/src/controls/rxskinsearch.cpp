#include "pch.h"
#include "windows.h"
#include "commdlg.h"
#include "common/rximage.h"
#include "controls/rxskinsearch.h"
#include "common/graphics.h"
#include <olectl.h>
#include "windowsx.h"
#include "commctrl.h"
#include <ShlObj_core.h>
#include "WinUser.h"
#include "common/RXSkinService.h"
#include "common/rxskindefines.h"
#include "main/rxskinfileselect.h"
#include "fundation/rxutil.h"

#define GWL_USERDATA        (-21)

#define OFFSET_X            5

CRXSkinSearch::CRXSkinSearch(void)
{
	m_strTipValue = _T("");
	m_clrTipText = RGB(0, 0, 0);
	m_clrTipBG = RGB(255, 255, 255);
	m_bIsPassword = false;
	m_bIsReadOnly = false;
	m_bIsMultLine = false;
	m_hBkBrush = NULL;
	m_clrEditTextColor = RGB(50, 250, 50);
	m_clrEditBKColor = RGB(201, 212, 215);
	m_clrBorder = RGB(201, 212, 215);
	m_bDrawCaret = false;
	m_wndEdit.SetOwner(this);
	m_dwTextStyle = DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_hBmpEditBk = NULL;
	m_bFocus = false;
	m_poListener = NULL;
}

CRXSkinSearch::~CRXSkinSearch(void)
{
	_DeleteEdit();

}

HWND CRXSkinSearch::GetEditWnd()
{
	return m_wndEdit.GetHWnd();
}

VOID CRXSkinSearch::SetRect(const CRXSkinRect rcItem)
{
	CRXControl::SetRect(rcItem);
}

VOID CRXSkinSearch::OnEditSetFocus(HWND hWnd)
{
	m_bFocus = true;
	Invalidate();
}
VOID CRXSkinSearch::OnEditKillFocus(HWND hWnd)
{
	m_bFocus = false;
	if (m_poListener)
	{
		m_poListener->OnEditKillFocus(GetID());
	}
	Invalidate();
}

VOID CRXSkinSearch::SetText(const TCHAR* strTxt)
{
	CRXControl::SetText(strTxt);
	if (_RXTStrlen(strTxt) > 0)
	{
		if (NULL == m_wndEdit.GetHWnd())
		{
			_ShowEdit();
		}
	}
	m_wndEdit.SetText(strTxt);
}

void CRXSkinSearch::SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_wndEdit.SendMessage(uMsg, wParam, lParam);
}

CRXSkinString CRXSkinSearch::GetWindowText()
{
	return m_wndEdit.GetTextValue().c_str();
}

void CRXSkinSearch::Destroy()
{
	m_wndEdit.Destroy();
}

void CRXSkinSearch::ModifyRect()
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BROWSER_BTN_NORMAL);
	if (poImage == NULL)
	{
		return;
	}
	CRXSkinRect rc = m_rcItem;
	rc.left += 2;
	rc.top += 4;
	rc.bottom -= 2;
	rc.right -= (2 * OFFSET_X + poImage->GetWidth());
	m_wndEdit.MoveWindowEx(rc);

}

VOID CRXSkinSearch::_ShowEdit()
{
	if (IsVisible() == FALSE || IsEnabled() == FALSE)
	{
		return;
	}
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BROWSER_BTN_NORMAL);
	if (poImage==NULL)
	{
		return;
	}
	CRXSkinRect rc = m_rcItem;
	rc.left += 2;
	rc.top += 2;
	rc.bottom -= 2;
	rc.right -= (2 * OFFSET_X+poImage->GetWidth());
	m_wndEdit.Create(WC_EDIT, _T(""), WS_CHILD | WS_VISIBLE, GetParentWnd(), NULL);
	rc.top += 4;
	rc.left += 4;
	m_wndEdit.MoveWindowEx(rc);
}

BOOL CRXSkinSearch::Create(const UINT32 dwID, CRXSkinRect rcClient, const BOOL bIsPassword)
{
	m_bIsPassword = bIsPassword;
	SetRect(rcClient);
	if (m_strTipValue.GetLength() <= 0)
	{
		_ShowEdit();
	}
	return TRUE;
}

VOID CRXSkinSearch::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
	if (bIsVisible)
	{
		m_wndEdit.Show();
	}
	else
		m_wndEdit.Hide();
}

VOID CRXSkinSearch::Enable(const BOOL bEnabled)
{
	CRXControl::Enable(bEnabled);
	m_wndEdit.EnableWindow(bEnabled);
}


VOID CRXSkinSearch::_DeleteEdit()
{
	m_wndEdit.Destroy();
}

BOOL CRXSkinSearch::OnLButtonUp(POINT pt)
{
	if (m_rcItem.PtInRect(pt))
	{
		_DeleteEdit();
		_ShowEdit();
		m_wndEdit.SetText(GetText());
		return TRUE;
	}
	return FALSE;
}

void CRXSkinSearch::SetTipTextColor(const COLORREF& clr)
{
	m_clrTipText = clr;
}

VOID CRXSkinSearch::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible() == FALSE)
	{
		return;
	}
	SetRect(rcItem);
	DrawRectEdge(hDC, rcItem, 1, m_clrBorder);

	CRXSkinRect rcBK = rcItem;
	rcBK.left += 6;
	rcBK.top += 2;
	rcBK.right -= 2;
	rcBK.bottom -= 2;
	m_hBmpEditBk = CDrawMgr::CopyDCToBitmap(hDC, &rcBK);

	CRXSkinRect rcText = m_rcItem;
	{
		rcText.left += 4;
		rcText.right -= 2;
		rcText.top += 2;
		rcText.bottom -= 2;
	}
	if (m_strTipValue.GetLength() > 0  && !m_bFocus)
	{
		tstring strValue = m_wndEdit.GetTextValue();
		if (strValue.size()<=0)
		{
			tstring strTxt = m_strTipValue.GetData();
			rcText.left += 2;
			SkCanvas* poCanvas = poSurface->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas,
					strTxt, rcText, m_clrTipText, ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			}
		}

	}
	IRXImage* poImage = NULL;
	if (GetState() == CONTROL_STATE_NORMAL)
	{
		poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_IMAGE_CLEAR_SEARCH);
	}
	else
	{
		poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_IMAGE_CLEAR_SEARCH);
	}
	if (poImage)
	{
		m_rcSelect = rcItem;
		m_rcSelect.left = m_rcSelect.right - OFFSET_X - poImage->GetWidth();
		m_rcSelect.right = m_rcSelect.left + poImage->GetWidth();
		m_rcSelect.top = m_rcSelect.top + (m_rcSelect.Height() - poImage->GetHeight()) / 2;
		m_rcSelect.bottom = m_rcSelect.top + poImage->GetHeight();
		poImage->Draw(hDC, m_rcSelect);
	}

}

VOID CRXSkinSearch::_OnClickSelect()
{
	m_wndEdit.SetText(_T(""));
}

BOOL CRXSkinSearch::OnClick(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (m_rcSelect.PtInRect(pt))
		{
			_OnClickSelect();
			return TRUE;
		}

		if (PtInRect(pt))
		{
			if (m_strTipValue.GetLength() > 0)
			{
				if (m_wndEdit.GetHWnd() == NULL)
				{
					_ShowEdit();
					m_wndEdit.SetText(GetText());
				}
			}
			return TRUE;
		}
		else
		{
			CRXSkinString strContent = GetWindowText();
			if (strContent.GetLength() <= 0 && m_strTipValue.GetLength() > 0)
			{
				_DeleteEdit();
			}
		}
	}
	return FALSE;
}

LRESULT CRXSkinSearch::OnEditChanged(WPARAM wParam, LPARAM lParam)
{
	if (lParam == (WPARAM)m_wndEdit.GetHWnd())
	{
		if (m_poListener)
		{
			m_poListener->OnEditChanged(GetID());
		}
		return 1;
	}
	return 0;
}

BOOL CRXSkinSearch::OnLButtonDown(POINT pt)
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

BOOL CRXSkinSearch::PtInRect(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CRXSkinSearch::OnMouseMove(POINT pt)
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

BOOL CRXSkinSearch::OnSize(const UINT32 dwX, const UINT32 dwY)
{
	CRXControl::OnSize(dwX, dwY);
	return TRUE;
}

VOID CRXSkinSearch::ReleaseControl()
{
	delete this;
}

void  CRXSkinSearch::MoveWindow(const CRXSkinRect& rcItem)
{
	m_wndEdit.MoveWindowEx(rcItem);
}


HBRUSH CRXSkinSearch::OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC)
{
	if (hWndChild == m_wndEdit.GetHWnd())
	{
		HBRUSH hBrush = m_wndEdit.OnCtlColorControl(dwMsgID, hWndChild, hDC);

		return hBrush;
	}
	return CRXControl::OnCtlColorControl(dwMsgID,hWndChild,hDC);
}

void CRXSkinSearch::Paint(HDC hDC, RECT rc)
{
	DrawRect(hDC, m_clrEditBKColor, rc.left, rc.right, rc.right, rc.bottom);
}

