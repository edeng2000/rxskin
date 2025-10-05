#include "pch.h"
#include "windows.h"
#include "commdlg.h"
#include "common/rximage.h"
#include "controls\rxskinbrowser.h"
#include "common/graphics.h"
#include <olectl.h>
#include "windowsx.h"
#include "commctrl.h"
#include <ShlObj_core.h>
#include "WinUser.h"
#include "common/RXSkinService.h"
#include "common/rxskindefines.h"
#include "main/rxskinfileselect.h"
#include "main/rxskinfolderselect.h"
#include "fundation/rxutil.h"

#define GWL_USERDATA        (-21)

#define OFFSET_X            5

CRXSkinBrowser::CRXSkinBrowser(void)
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
	m_enType = BROWSER_SELECT_FILE;
	m_wndEdit.SetOwner(this);
	memset(m_szFileExt, 0, sizeof(TCHAR) * MAX_PATH);
	m_dwTextStyle = DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_hBmpEditBk = NULL;
	m_bFocus = false;
}

CRXSkinBrowser::~CRXSkinBrowser(void)
{
	_DeleteEdit();

}

void CRXSkinBrowser::SetFileExt(const TCHAR* pszExt)
{
	memset(m_szFileExt, 0, sizeof(TCHAR) * MAX_PATH);
	_RXTStrcpy(m_szFileExt, pszExt);
}

HWND CRXSkinBrowser::GetEditWnd()
{
	return m_wndEdit.GetHWnd();
}

VOID CRXSkinBrowser::SetRect(const CRXSkinRect rcItem)
{
	CRXControl::SetRect(rcItem);
}

VOID CRXSkinBrowser::OnEditSetFocus(HWND hWnd)
{
	m_bFocus = true;
	Invalidate();
}
VOID CRXSkinBrowser::OnEditKillFocus(HWND hWnd)
{
	m_bFocus = false;
	Invalidate();
}

VOID CRXSkinBrowser::SetText(const TCHAR* strTxt)
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

void CRXSkinBrowser::SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_wndEdit.SendMessage(uMsg, wParam, lParam);
}

CRXSkinString CRXSkinBrowser::GetWindowText()
{
	return m_wndEdit.GetTextValue().c_str();
}

void CRXSkinBrowser::Destroy()
{
	m_wndEdit.Destroy();
}

void CRXSkinBrowser::ModifyRect()
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

void  CRXSkinBrowser::SetIsReadyOnly()
{
	m_wndEdit.SetIsReadOnly(true);
}

VOID CRXSkinBrowser::_ShowEdit()
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
	rc.top += 4;
	rc.bottom -= 2;
	rc.right -= (2 * OFFSET_X+poImage->GetWidth());
	m_wndEdit.Create(WC_EDIT, _T(""), WS_CHILD | WS_VISIBLE, GetParentWnd(), NULL);
	rc.top += 4;
	rc.left += 4;
	m_wndEdit.MoveWindowEx(rc);
}

BOOL CRXSkinBrowser::Create(const UINT32 dwID, CRXSkinRect rcClient, const BOOL bIsPassword)
{
	m_bIsPassword = bIsPassword;
	SetRect(rcClient);
	if (m_strTipValue.GetLength() <= 0)
	{
		_ShowEdit();
	}
	return TRUE;
}

VOID CRXSkinBrowser::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
	if (bIsVisible)
	{
		m_wndEdit.Show();
	}
	else
		m_wndEdit.Hide();
}

VOID CRXSkinBrowser::Enable(const BOOL bEnabled)
{
	CRXControl::Enable(bEnabled);
	m_wndEdit.EnableWindow(bEnabled);
}


VOID CRXSkinBrowser::_DeleteEdit()
{
	m_wndEdit.Destroy();
}

BOOL CRXSkinBrowser::OnLButtonUp(POINT pt)
{
	if (FALSE == IsEnabled()) return FALSE;
	if (m_rcItem.PtInRect(pt))
	{
		_DeleteEdit();
		_ShowEdit();
		m_wndEdit.SetText(GetText());
		return TRUE;
	}
	return FALSE;
}

VOID CRXSkinBrowser::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible() == FALSE)
	{
		return;
	}
	SetRect(rcItem);
	DrawRectEdge(hDC, rcItem, 2, m_clrBorder);
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
			HFONT hOldFont = (HFONT)::SelectObject(hDC, CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
			tstring strTxt = m_strTipValue.GetData();
			INT32 nModel = SetBkMode(hDC, TRANSPARENT);
			COLORREF clrOld = ::SetTextColor(hDC, m_clrTipText);
			rcText.left += 2;
			SkCanvas* poCanvas = poSurface->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcText,
					m_clrTipText,
					ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			}
			::SelectObject(hDC, hOldFont);
			::SetBkMode(hDC, nModel);
			::SetTextColor(hDC, clrOld);
		}

	}
	IRXImage* poImage = NULL;
	if (GetState() == CONTROL_STATE_NORMAL)
	{
		poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BROWSER_BTN_NORMAL);
	}
	else
	{
		poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BROWSER_BTN_HOT);
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
	CRXSkinRect rcBK = rcItem;
	rcBK.left += 6;
	rcBK.top += 6;
	rcBK.right -= 2;
	rcBK.bottom -= 2;
	m_hBmpEditBk = CDrawMgr::CopyDCToBitmap(hDC, &rcBK);
}

VOID CRXSkinBrowser::_OnClickSelect()
{
	if (m_enType == BROWSER_SELECT_FILE)
	{
		tstring strTxt = GetText();
		tstring strResult = RXSkinFileSelect(GetParentWnd(), _T("选择文件"), _T(""), strTxt.c_str(), RXSKIN_FILE_SELECT_FILE);
		if (strResult.size()>0)
		{
			SetText(strResult.c_str());
		}
	}
	else
	{
		tstring strTxt = GetText();
		tstring strResult = RXSkinFolderSelect(GetParentWnd(), _T("选择文件夹"), _T(""), strTxt.c_str(), RXSKIN_FILE_SELECT_DIRECTORY);
		if (strResult.size() > 0)
		{
			SetText(strResult.c_str());
		}
	}
}

BOOL CRXSkinBrowser::OnClick(POINT pt)
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

BOOL CRXSkinBrowser::OnLButtonDown(POINT pt)
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

BOOL CRXSkinBrowser::PtInRect(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CRXSkinBrowser::OnMouseMove(POINT pt)
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

BOOL CRXSkinBrowser::OnSize(const UINT32 dwX, const UINT32 dwY)
{
	CRXControl::OnSize(dwX, dwY);
	return TRUE;
}

VOID CRXSkinBrowser::ReleaseControl()
{
	delete this;
}

void  CRXSkinBrowser::MoveWindow(const CRXSkinRect& rcItem)
{
	m_wndEdit.MoveWindowEx(rcItem);
}


HBRUSH CRXSkinBrowser::OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC)
{
	if (hWndChild == m_wndEdit.GetHWnd())
	{
		HBRUSH hBrush = m_wndEdit.OnCtlColorControl(dwMsgID, hWndChild, hDC);

		return hBrush;
	}
	return CRXControl::OnCtlColorControl(dwMsgID,hWndChild,hDC);
}

void CRXSkinBrowser::Paint(HDC hDC, RECT rc)
{
	DrawRect(hDC, m_clrEditBKColor, rc.left, rc.right, rc.right, rc.bottom);
}

