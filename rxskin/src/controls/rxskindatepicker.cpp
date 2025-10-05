#include "pch.h"

#include <time.h>

#include "controls\rxskindatepicker.h"
#include "controls\RXSkinComboBox.h"

#include "common\RXSkinService.h"
#include "common\graphics.h"
#include "common\RXSkinMemDC.h"
#include "common\rxskinutil.h"

#define WND_DATE_PICKER_POPUP      _T("RXSkinDatePickerPopup")

#define ID_DATEPICKER_COMBOBOX_YEAR   10000
#define ID_DATEPICKER_COMBOBOX_MONTH  10001
#define ID_DATEPICKER_COMBOBOX_DAY    10002

CRXSkinDatePickerPopup::CRXSkinDatePickerPopup()
{
	m_poPicker = NULL;
	m_poMouseOn = m_poPressed = NULL;
}

CRXSkinDatePickerPopup::~CRXSkinDatePickerPopup()
{
}

VOID CRXSkinDatePickerPopup::SetDataPicker(CRXSkinDatePicker* poPicker)
{
	m_poPicker = poPicker;
}

BOOL CRXSkinDatePickerPopup::Create(const TCHAR* pszWindowClassName,
									const TCHAR* pszWindowName,
									const UINT32 dwStyle,
									const HWND hParentWnd,
									HICON hIcon)
{
	BOOL bResult = CRXSkinWnd::Create(pszWindowClassName,pszWindowName,dwStyle,hParentWnd,hIcon);
	return bResult;
}

VOID CRXSkinDatePickerPopup::Show(const BOOL bShowed,POINT pt)
{
	if (FALSE == bShowed)
	{
		ShowWindow(m_hWnd,SW_HIDE);
		ReleaseCapture();
		if (m_poPicker)
		{
			m_poPicker->SetText(GetText().GetData());
			m_poPicker->Invalidate();
		}
	}
	else
	{
		RECT rcWin;
		GetWindowRect(m_hWnd,&rcWin);
		rcWin.top = pt.y;
		rcWin.left = pt.x;
		rcWin.bottom = rcWin.top + CRXSkinService::Instance()->GetDrawMgr().GetDatePickerSize().cy;
		rcWin.right = rcWin.left + CRXSkinService::Instance()->GetDrawMgr().GetDatePickerSize().cx;
		MoveWindow(m_hWnd,rcWin.left,
			rcWin.top,
			CRXSkinService::Instance()->GetDrawMgr().GetDatePickerSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetDatePickerSize().cy,
			FALSE);
		ShowWindow(m_hWnd,SW_SHOW);
		::SetCapture(m_hWnd);
	}
}

CRXSkinString CRXSkinDatePickerPopup::GetText()
{
	CRXSkinString strValue = _T("");
	CRXSkinComboBox* poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_DATEPICKER_COMBOBOX_YEAR);
	if (poComboBox)
	{
		strValue+=poComboBox->GetText();
		strValue += _T("-");
	}
	poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_DATEPICKER_COMBOBOX_MONTH);
	if (poComboBox)
	{
		strValue+=poComboBox->GetText();
		strValue += _T("-");
	}
	poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_DATEPICKER_COMBOBOX_DAY);
	if (poComboBox)
	{
		strValue+=poComboBox->GetText();
	}
	return strValue;
}

VOID CRXSkinDatePickerPopup::SetDay(const TCHAR* pszDay)
{
	m_strDay = pszDay;
	CRXSkinComboBox* poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_DATEPICKER_COMBOBOX_DAY);
	if (poComboBox)
	{
		INT32 nCount = poComboBox->GetCount();
		for (INT32 n = 0; n < nCount;n++)
		{
			if (_RXTStrcmp(m_strDay.GetData(),poComboBox->GetLBText(n)) == 0)
			{
				poComboBox->SetCurSel(n);
				break;
			}
		}
	}
}

VOID CRXSkinDatePickerPopup::SetMonth(const TCHAR* pszMonth)
{
	m_strMonth = pszMonth;
	CRXSkinComboBox* poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_DATEPICKER_COMBOBOX_MONTH);
	if (poComboBox)
	{
		INT32 nCount = poComboBox->GetCount();
		for (INT32 n = 0; n < nCount;n++)
		{
			if (_RXTStrcmp(m_strMonth.GetData(),poComboBox->GetLBText(n)) == 0)
			{
				poComboBox->SetCurSel(n);
				break;
			}
		}
	}
}

VOID CRXSkinDatePickerPopup::SetYear(const TCHAR* pszYear)
{
	m_strYear = pszYear;
	CRXSkinComboBox* poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_DATEPICKER_COMBOBOX_YEAR);
	if (poComboBox)
	{
		INT32 nCount = poComboBox->GetCount();
		for (INT32 n = 0; n < nCount;n++)
		{
			if (_RXTStrcmp(m_strYear.GetData(),poComboBox->GetLBText(n)) == 0)
			{
				poComboBox->SetCurSel(n);
				break;
			}
		}
	}
}

VOID CRXSkinDatePickerPopup::OnPaint()
{
	CRXSkinWnd::OnPaint();
	PAINTSTRUCT ps = { 0 };
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	CRXSkinMemDC memDC(hDC,rcClient);
	OnDraw(memDC.GetSurface(), memDC.GetHDC(),rcClient);
	m_oControlMgr.OnDraw(memDC.GetSurface(),memDC.GetHDC());
	memDC.TakeSnapshot();
	::EndPaint(m_hWnd, &ps);
}

BOOL CRXSkinDatePickerPopup::OnMessage(UINT32 dwMessageID,WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinWnd::OnMessage(dwMessageID,wParam,lParam);
	if (bResult == FALSE)
	{
		switch(dwMessageID)
		{
		case WM_CAPTURECHANGED:
			{
				BOOL bShowed = FALSE;
				CRXSkinComboBox* poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_DATEPICKER_COMBOBOX_YEAR);
				if (poComboBox && poComboBox->GetHWnd() == GetCapture())
				{
					bShowed = TRUE;
				}
				if (FALSE == bShowed)
				{
					poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_DATEPICKER_COMBOBOX_MONTH);
					if (poComboBox && poComboBox->GetHWnd() == GetCapture())
					{
						bShowed = TRUE;
					}
				}
				if (FALSE == bShowed)
				{
					poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_DATEPICKER_COMBOBOX_DAY);
					if (poComboBox && poComboBox->GetHWnd() == GetCapture())
					{
						bShowed = TRUE;
					}		
				}
				if (FALSE == bShowed)
				{
					POINT pt;
					pt.x = pt.y = 0;
					Show(FALSE,pt);
				}
			}
			break;
		default:
			break;
		}
	}
	return bResult;
}


BOOL CRXSkinDatePickerPopup::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinWnd::OnCreate(lpCreate);
	if (bResult)
	{
		CRXSkinComboBox* poControl = new CRXSkinComboBox;

		RECT rcControl;
		rcControl.left = 10;
		rcControl.top = 5;
		rcControl.right = 90;
		rcControl.bottom = rcControl.top + 22;
		poControl->SetRect(rcControl);
		poControl->SetID(ID_DATEPICKER_COMBOBOX_YEAR);
		poControl->SetParentWnd(m_hWnd);
		poControl->SetCaptureWnd(m_hWnd);
		poControl->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
		poControl->Create(ID_DATEPICKER_COMBOBOX_YEAR,rcControl,COMBOBOX_DROPLIST);
		time_t t = time(NULL);
		tm localTime  = *localtime(&t);
		for (INT32 n = 1950; n < localTime.tm_year+1900;n++)
		{
			CRXSkinString strData;
			strData.Format(_T("%d"),n);
			INT32 nCur = poControl->AddString(strData.GetData());
			if (m_strYear == strData)
			{
				poControl->SetCurSel(nCur);
			}
		}
		m_oControlMgr.AddControl(poControl);

		poControl = new CRXSkinComboBox;
		rcControl.left = 100;
		rcControl.top = 5;
		rcControl.right = 170;
		rcControl.bottom = rcControl.top + 22;
		poControl->SetRect(rcControl);
		poControl->SetID(ID_DATEPICKER_COMBOBOX_MONTH);
		poControl->SetParentWnd(m_hWnd);
		poControl->SetCaptureWnd(m_hWnd);
		poControl->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
		poControl->Create(ID_DATEPICKER_COMBOBOX_MONTH,rcControl,COMBOBOX_DROPLIST);
		for (INT32 n = 1; n < 13;n++)
		{
			CRXSkinString strData;
			strData.Format(_T("%d"),n);
			INT32 nCur = poControl->AddString(strData.GetData());
			if (m_strMonth == strData)
			{
				poControl->SetCurSel(nCur);
			}
		}
		m_oControlMgr.AddControl(poControl);

		poControl = new CRXSkinComboBox;
		rcControl.left = 180;
		rcControl.top = 5;
		rcControl.right = 240;
		rcControl.bottom = rcControl.top + 22;
		poControl->SetRect(rcControl);
		poControl->SetID(ID_DATEPICKER_COMBOBOX_DAY);
		poControl->SetParentWnd(m_hWnd);
		poControl->SetCaptureWnd(m_hWnd);
		poControl->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
		poControl->Create(ID_DATEPICKER_COMBOBOX_DAY,rcControl,COMBOBOX_DROPLIST);
		for (INT32 n = 1; n < 32;n++)
		{
			CRXSkinString strData;
			strData.Format(_T("%d"),n);
			INT32 nCur = poControl->AddString(strData.GetData());
			if (m_strDay == strData)
			{
				poControl->SetCurSel(nCur);
			}
		}
		m_oControlMgr.AddControl(poControl);
	}
	return TRUE;
}

BOOL CRXSkinDatePickerPopup::OnMouseMove(UINT nFlags, POINT point)
{
	CRXControl* poItem = m_oControlMgr.HitTest(point);
	if (poItem)
	{
		if (m_poMouseOn == NULL)
		{
			m_poMouseOn = poItem;
			m_poMouseOn->SetState(CONTROL_STATE_MOUSEON);
			m_poMouseOn->Invalidate();
		}
		else if (m_poMouseOn!=poItem)
		{
			m_poMouseOn->SetState(CONTROL_STATE_NORMAL);
			m_poMouseOn->Invalidate();
			m_poMouseOn = poItem;
			m_poMouseOn->SetState(CONTROL_STATE_MOUSEON);
			m_poMouseOn->Invalidate();
		}

		CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_HAND);
		return TRUE;
	}
	else
	{
		if (m_poMouseOn)
		{
			m_poMouseOn->SetState(CONTROL_STATE_NORMAL);
			m_poMouseOn->Invalidate();
			m_poMouseOn = poItem;
		}
		CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);
	}
	return FALSE;
}

BOOL CRXSkinDatePickerPopup::OnLButtonDown(UINT nFlags, POINT point)
{
	return m_oControlMgr.OnLButtonDown(point) == NULL ? FALSE:TRUE;
}

BOOL CRXSkinDatePickerPopup::OnLButtonUp(UINT nFlags, POINT point)
{
	RECT rc;
	GetClientRect(m_hWnd,&rc);
	CRXSkinRect rcClient = rc;
	if (rcClient.PtInRect(point) == FALSE)
	{
		CRXSkinString strYear;
		CRXSkinComboBox* poYearComboBox = (CRXSkinComboBox*)GetControlMgr().FindControl(ID_DATEPICKER_COMBOBOX_YEAR);
		if (poYearComboBox)
		{
			strYear = poYearComboBox->GetText();
		}
		CRXSkinString strMonth;
		CRXSkinComboBox* poMonthComboBox = (CRXSkinComboBox*)GetControlMgr().FindControl(ID_DATEPICKER_COMBOBOX_MONTH);
		if (poMonthComboBox)
		{
			strMonth = poMonthComboBox->GetText();
		}
		CRXSkinString strDay;
		CRXSkinComboBox* poDayComboBox = (CRXSkinComboBox*)GetControlMgr().FindControl(ID_DATEPICKER_COMBOBOX_DAY);
		if (poDayComboBox)
		{
			strDay = poDayComboBox->GetText();
		}
		if (m_poPicker)
		{
			
		}
		Show(FALSE,point);
		return TRUE;
	}
	else
	{
		return m_oControlMgr.OnClick(point);
	}
	return FALSE;
}

VOID CRXSkinDatePickerPopup::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	DrawRect(hDC,
		CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor(),
		CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(),
		rcItem.left, rcItem.top, rcItem.right, rcItem.bottom
		);
	m_oControlMgr.OnDraw(poSurface,hDC);
}


CRXSkinDatePicker::CRXSkinDatePicker(void)
{
	m_poDatePickerPopup = NULL;

}

CRXSkinDatePicker::~CRXSkinDatePicker(void)
{
	if (m_poDatePickerPopup)
	{
		delete m_poDatePickerPopup;
		m_poDatePickerPopup = NULL;
	}
}

VOID CRXSkinDatePicker::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible())
	{
		SetRect(rcItem);
 		CRXSkinService::Instance()->GetDrawMgr().DrawControlBorder(hDC,GetState(),rcItem);
		rcItem.left += CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
		rcItem.right -= CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
		rcItem.top += CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cy;
		rcItem.bottom -= CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cy;
		INT32 nModel = SetBkMode(hDC,TRANSPARENT);
		HFONT hOld = (HFONT)SelectObject(hDC,CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
		tstring strTxt = GetText();
		SkCanvas* poCanvas = poSurface->getCanvas();
		if (poCanvas)
		{
			CRXSkinFontMgr::Instance()->DrawText(poCanvas,
				strTxt, rcItem,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalTxt(),
				ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
		}
		SelectObject(hDC,hOld);
		SetBkMode(hDC,nModel);
	}	
}

VOID CRXSkinDatePicker::ReleaseControl()
{
	delete this;
}

BOOL CRXSkinDatePicker::PtInRect(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}

BOOL CRXSkinDatePicker::OnClick(POINT pt)
{
	if (PtInRect(pt))
	{
		POINT pt;
		pt.x = GetRect().left;
		pt.y = GetRect().bottom;
		ClientToScreen(GetParentWnd(),&pt);
		if (NULL == m_poDatePickerPopup)
		{
			m_poDatePickerPopup = new CRXSkinDatePickerPopup;
			m_poDatePickerPopup->SetDataPicker(this);
			m_poDatePickerPopup->Create(WND_DATE_PICKER_POPUP,
				_T(""),WS_POPUP,GetParentWnd(),NULL);
		}
		CRXSkinString strTxt = GetText();
		INT32 nPos = strTxt.Find(_T("-"));
		if (nPos>=0)
		{
			CRXSkinString strYear = strTxt.Left(nPos);
			m_poDatePickerPopup->SetYear(strYear.GetData());
			strTxt = strTxt.Right(strTxt.GetLength() - 1 - nPos);
		}
		nPos = strTxt.Find(_T("-"));
		if (nPos>=0)
		{
			CRXSkinString strMonth = strTxt.Left(nPos);
			m_poDatePickerPopup->SetMonth(strMonth.GetData());
			strTxt = strTxt.Right(strTxt.GetLength() - 1 - nPos);
			m_poDatePickerPopup->SetDay(strTxt);
		}

		m_poDatePickerPopup->Show(TRUE,pt);
		return TRUE;
	}
	return FALSE;
}
BOOL CRXSkinDatePicker::OnMouseMove(POINT pt)
{
	return PtInRect(pt);
}
BOOL CRXSkinDatePicker::OnLButtonDown(POINT pt)
{
	return PtInRect(pt);
}

BOOL CRXSkinDatePicker::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	return TRUE;
}