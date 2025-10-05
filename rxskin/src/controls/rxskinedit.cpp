#include "pch.h"
#include "windows.h"
#include "common/rximage.h"
#include "controls\RXSkinEdit.h"
#include "common/graphics.h"
#include <olectl.h>
#include "windowsx.h"
#include "commctrl.h"
#include "WinUser.h"
#include "common/RXSkinService.h"
#include "common/rxskindefines.h"
#include "main/rxskinapp.h"
#define GWL_USERDATA        (-21)

CRXSkinEdit::CRXSkinEdit(void)
{
	m_strTipValue = _T("");
	m_clrTipText = RGB(0, 0, 0);
	m_clrTipBG = RGB(255, 255, 255);
	m_bIsPassword = false;
	m_bIsReadOnly = false;
	m_bIsMultLine = false;
	m_clrEditTextColor = RGB(50,250,50);
	m_clrEditBKColor = RGB(201, 232, 235);
	m_clrBorder = RGB(201, 212, 215);
	m_bDrawCaret = false;
	m_poListener = NULL;
	m_wndEdit.SetOwner(this);
	m_dwTextStyle = DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_hBmpEditBk = NULL;
	m_bFocus = false;
	m_bIsBold = FALSE;
	m_dwFontSize = 0;
	m_hFont = NULL;
	m_bRename = false;
}

CRXSkinEdit::~CRXSkinEdit(void)
{
	_DeleteEdit();
	if (m_hBmpEditBk)
	{
		::DeleteObject(m_hBmpEditBk);
		m_hBmpEditBk = NULL;
	}
	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
}

HWND CRXSkinEdit::GetEditWnd()
{
	return m_wndEdit.GetHWnd();
}

void CRXSkinEdit::OffsetPos(int32_t nX, int32_t nY)
{
	POINT pt;
	pt.x = nX;
	pt.y = nY;
	m_rcItem.OffsetRect(pt);
	ModifyRect();
}

VOID CRXSkinEdit::SetText(const TCHAR* strTxt)
{
	CRXControl::SetText(strTxt);
	m_strText = strTxt;
	m_wndEdit.SetText(strTxt);
}

void CRXSkinEdit::OnEditChanged(HWND hWnd)
{
	Invalidate();
}

VOID CRXSkinEdit::SetIntValue(const int32_t& nValue)
{
	TCHAR szValue[64] = {0};
	_RXTsprintf(szValue, _T("%d"), nValue);
	CRXControl::SetText(szValue);
	SetText(szValue);	
}

int32_t CRXSkinEdit::GetTextValueInt()
{
	return m_wndEdit.GetIntValue();
}

TCHAR* CRXSkinEdit::GetText()
{
	m_strContent = m_wndEdit.GetTextValue().c_str();
	return (TCHAR*)m_strContent.GetData();
}

CRXSkinString CRXSkinEdit::GetWindowText()
{
	return m_wndEdit.GetTextValue().c_str();
}

void CRXSkinEdit::SetFocus()
{
	if (m_wndEdit.GetHWnd()!=NULL)
	{
		::SetFocus(m_wndEdit.GetHWnd());
	}
}

void CRXSkinEdit::Destroy()
{
	m_wndEdit.Destroy();
}

VOID CRXSkinEdit::_ShowEdit()
{
	if (IsVisible() == FALSE || IsEnabled() == FALSE)
	{
		return;
	}
	m_wndEdit.SetBorderColor(m_clrBorder);
	if (NULL == m_wndEdit.GetHWnd())
	{
		if (m_strFontName.size()>0 && m_dwFontSize>0 )
		{
			if (m_hFont==NULL)
			{
				m_hFont = CreateFont(
					m_dwFontSize,
					0,
					0,
					0,
					m_bIsBold?FW_BOLD:FW_NORMAL,
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
		m_wndEdit.SetText(GetText());
		m_wndEdit.SetIsReadOnly(m_bIsReadOnly);
		m_wndEdit.Create(WC_EDIT, _T(""), WS_CHILD | WS_VISIBLE, GetParentWnd(), NULL);
		m_poApp->AddSkinEdit(m_wndEdit.GetHWnd(), this);
		CRXSkinRect rcEdit = GetRect();
		rcEdit.left += 6;
		rcEdit.top += 6;
		rcEdit.right -= 2;
		rcEdit.bottom -= 2;
		m_wndEdit.MoveWindowEx(rcEdit);

	}

}

BOOL CRXSkinEdit::Create(const UINT32 dwID,CRXSkinRect rcClient,const BOOL bIsPassword)
{
	m_bIsPassword = bIsPassword;
	SetRect(rcClient);
	if (m_strTipValue.GetLength()<=0)
	{
		_ShowEdit();
	}
	return TRUE;
}

VOID CRXSkinEdit::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
	if (bIsVisible)
	{
		_ShowEdit();
 		m_wndEdit.Show();
		m_wndEdit.SetText(m_strText.c_str());
	}
	else
		m_wndEdit.Hide();
}

VOID CRXSkinEdit::Enable(const BOOL bEnabled)
{
	CRXControl::Enable(bEnabled);
	
}


VOID CRXSkinEdit::_DeleteEdit()
{
	m_poApp->RemoveSkinEdit(m_wndEdit.GetHWnd());
	m_strContent = m_wndEdit.GetTextValue().c_str();
	m_wndEdit.Destroy();
}

BOOL CRXSkinEdit::OnLButtonUp(POINT pt)
{
	if (FALSE == IsEnabled()) return FALSE;
	if (m_rcItem.PtInRect(pt))
	{		
		_ShowEdit();
		return TRUE;
	}
	return FALSE;
}

void CRXSkinEdit::SetIsReadyOnly()
{
	m_wndEdit.SetIsReadOnly(true);
}

VOID CRXSkinEdit::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible() ==FALSE)
	{
		return;
	}
	SetRect(rcItem);
	CRXSkinRect rcText = m_rcItem;
	{
		rcText.left += 4;
		rcText.right -= 2;
		rcText.top += 2;
		rcText.bottom -= 2;
	}
	if (m_strTipValue.GetLength()>0 )
	{
		tstring strTxt = m_wndEdit.GetTextValue();
		if (strTxt.size()<=0)
		{
			HFONT hOldFont = (HFONT)::SelectObject(hDC, CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
			tstring strTxt = m_strTipValue.GetData();
			INT32 nModel = SetBkMode(hDC, TRANSPARENT);
			COLORREF clrOld = ::SetTextColor(hDC, m_clrTipText);
			rcText.left += 2;
			SkCanvas* poCanvas = poSurface->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas,
					strTxt, rcText,
					m_clrTipText,
					ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			}
			::SelectObject(hDC, hOldFont);
			::SetBkMode(hDC, nModel);
			::SetTextColor(hDC, clrOld);
		}
	}

	ReleaseBKBitmap();
	CRXSkinRect rcBK = rcItem;
	rcBK.left += 6;
	rcBK.top += 6;
	rcBK.right -= 2;
	rcBK.bottom -= 2;
	DrawRectEdge(hDC, rcItem, 1, m_clrBorder);

	m_hBmpEditBk = CDrawMgr::CopyDCToBitmap(hDC, &rcBK);
}
VOID CRXSkinEdit::OnEditSetFocus(HWND hWnd)
{
	m_bFocus = true;
	Invalidate();
}


VOID CRXSkinEdit::OnEditKillFocus(HWND hWnd)
{
	m_bFocus = false;
	Invalidate();
}
BOOL CRXSkinEdit::OnClick(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (PtInRect(pt))
		{
			if (m_strTipValue.GetLength()>0)
			{
				if (m_wndEdit.GetHWnd()==NULL)
				{
					_ShowEdit();
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

BOOL CRXSkinEdit::OnLButtonDown(POINT pt)
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

BOOL CRXSkinEdit::PtInRect(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return TRUE;
	}
	return FALSE;
}


LRESULT CRXSkinEdit::OnEditChanged(WPARAM wParam, LPARAM lParam)
{
	if (lParam == (WPARAM)m_wndEdit.GetHWnd())
	{
		return 1;
	}
	return 0;
}

BOOL CRXSkinEdit::OnMouseMove(POINT pt)
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

void CRXSkinEdit::ModifyRect()
{
	MoveWindow(m_rcItem);
}

BOOL CRXSkinEdit::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	CRXControl::OnSize(dwX,dwY);
	ModifyRect();
	return TRUE;
}

VOID CRXSkinEdit::ReleaseControl()
{
	delete this;
}

void  CRXSkinEdit::MoveWindow(const CRXSkinRect& rcItem)
{
	if (false == m_bRename)
	{
		RECT rcEdit = rcItem;
		rcEdit.left += 6;
		rcEdit.top += 6;
		rcEdit.bottom -= 4;
		rcEdit.right -= 2;
		m_wndEdit.MoveWindowEx(rcEdit);
	}
	else
	{
		RECT rcEdit = rcItem;
		m_wndEdit.MoveWindowEx(rcEdit);
	}
}


HBRUSH CRXSkinEdit::OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC)
{
	if (hWndChild == m_wndEdit.GetHWnd() && m_bRename == false)
	{
		HBRUSH hBrush = m_wndEdit.OnCtlColorControl(dwMsgID, hWndChild, hDC);
		
		return hBrush;
	}

	return (HBRUSH)CRXControl::OnCtlColorControl(dwMsgID,hWndChild,hDC);
}


void CRXSkinEdit::OnReturn()
{
	if (m_poListener)
	{
		m_poListener->OnEditReturnOK(GetID());
	}
}

