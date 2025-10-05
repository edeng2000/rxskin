#include "pch.h"
#include "controls\rxskinlocationpicker.h"

#include <time.h>

#include "controls\RXSkinComboBox.h"

#include "common\RXSkinService.h"
#include "common\graphics.h"
#include "common\RXSkinMemDC.h"
#include "common\rxskinutil.h"
#include "common\rxskindefines.h"

#define WND_LOCATION_PICKER_POPUP      _T("RXSkinLocationPickerPopup")
#define RECT_COUNTRY                   CRXSkinRect(10,5,90,27)
#define RECT_PROVINCE                  CRXSkinRect(100,5,170,27)
#define RECT_CITY                      CRXSkinRect(180,5,290,27)


#define ID_LOCATIONPICKER_COMBOBOX_COUNTRY   10003
#define ID_LOCATIONPICKER_COMBOBOX_PROVINCE  10004
#define ID_LOCATIONPICKER_COMBOBOX_CITY      10005

CRXSkinLocationPickerPopup::CRXSkinLocationPickerPopup()
{
	m_poPicker = NULL;
	m_poMouseOn = m_poPressed = NULL;
}

CRXSkinLocationPickerPopup::~CRXSkinLocationPickerPopup()
{
}

VOID CRXSkinLocationPickerPopup::SetDataPicker(CRXSkinLocationPicker* poPicker)
{
	m_poPicker = poPicker;
}

VOID CRXSkinLocationPickerPopup::SetCity(const TCHAR* pszCity)
{
	m_strCity = pszCity;
	CRXSkinComboBox* poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_CITY);
	if (poComboBox)
	{
		INT32 nCount = poComboBox->GetCount();
		for (INT32 n = 0; n < nCount;n++)
		{
			if (_RXTStrcmp(m_strCity.GetData(),poComboBox->GetLBText(n)) == 0)
			{
				poComboBox->SetCurSel(n);
				break;
			}
		}
	}
}

VOID CRXSkinLocationPickerPopup::SetCountry(const TCHAR* pszCountry)
{
	m_strCountry = pszCountry;
	CRXSkinComboBox* poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_COUNTRY);
	if (poComboBox)
	{
		INT32 nCount = poComboBox->GetCount();
		for (INT32 n = 0; n < nCount;n++)
		{
			if (_RXTStrcmp(m_strCountry.GetData(),poComboBox->GetLBText(n)) == 0)
			{
				poComboBox->SetCurSel(n);
				break;
			}
		}
	}
}

VOID CRXSkinLocationPickerPopup::SetProvince(const TCHAR* pszProvince)
{
	m_strProvince = pszProvince;
	CRXSkinComboBox* poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_PROVINCE);
	if (poComboBox)
	{
		INT32 nCount = poComboBox->GetCount();
		for (INT32 n = 0; n < nCount;n++)
		{
			if (_RXTStrcmp(m_strProvince.GetData(),poComboBox->GetLBText(n)) == 0)
			{
				poComboBox->SetCurSel(n);
				_OnProvinceChanged();
				break;
			}
		}
	}
}

BOOL CRXSkinLocationPickerPopup::Create(const TCHAR* pszWindowClassName,
									const TCHAR* pszWindowName,
									const UINT32 dwStyle,
									const HWND hParentWnd,
									HICON hIcon)
{
	BOOL bResult = CRXSkinWnd::Create(pszWindowClassName,pszWindowName,dwStyle,hParentWnd,hIcon);
	return bResult;
}



VOID CRXSkinLocationPickerPopup::Show(const BOOL bShowed,POINT pt)
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
		rcWin.bottom = rcWin.top + CRXSkinService::Instance()->GetDrawMgr().GetLocationPickerSize().cy;
		rcWin.right = rcWin.left + CRXSkinService::Instance()->GetDrawMgr().GetLocationPickerSize().cx;
		MoveWindow(m_hWnd,rcWin.left,
			rcWin.top,
			CRXSkinService::Instance()->GetDrawMgr().GetLocationPickerSize().cx,
			CRXSkinService::Instance()->GetDrawMgr().GetLocationPickerSize().cy,
			FALSE);
		ShowWindow(m_hWnd,SW_SHOW);
		::SetCapture(m_hWnd);
	}
}

CRXSkinString CRXSkinLocationPickerPopup::GetText()
{
	CRXSkinString strValue = _T("");
	CRXSkinComboBox* poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_COUNTRY);
	if (poComboBox)
	{
		if (m_poPicker)
		{
			m_poPicker->SetCountry(poComboBox->GetText());
		}
		strValue+=poComboBox->GetText();

		strValue += _T("-");
	}
	poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_PROVINCE);
	if (poComboBox)
	{
		if (m_poPicker)
		{
			m_poPicker->SetProvince(poComboBox->GetText());
		}
		strValue+=poComboBox->GetText();
		strValue += _T("-");
	}
	poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_CITY);
	if (poComboBox)
	{
		if (m_poPicker)
		{
			m_poPicker->SetCity(poComboBox->GetText());
		}
		strValue+=poComboBox->GetText();
	}
	return strValue;
}


BOOL CRXSkinLocationPickerPopup::OnMessage(UINT32 dwMessageID,WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinWnd::OnMessage(dwMessageID,wParam,lParam);
	if (FALSE == bResult)
	{
		switch(dwMessageID)
		{
		case WM_CAPTURECHANGED:
			{
				BOOL bShowed = FALSE;
				CRXSkinComboBox* poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_COUNTRY);
				if (poComboBox && poComboBox->GetHWnd() == GetCapture())
				{
					bShowed = TRUE;
				}
				if (FALSE == bShowed)
				{
					poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_PROVINCE);
					if (poComboBox && poComboBox->GetHWnd() == GetCapture())
					{
						bShowed = TRUE;
					}
				}
				if (FALSE == bShowed)
				{
					poComboBox = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_CITY);
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
				return TRUE;
			}
			break;
		case WM_RXSKIN_COMBOBOX_CHANGED:
			{
				if (wParam == ID_LOCATIONPICKER_COMBOBOX_COUNTRY)
				{
					_OnCountryChanged();
				}
				if (wParam == ID_LOCATIONPICKER_COMBOBOX_PROVINCE)
				{
					_OnProvinceChanged();
				}
				return TRUE;
			}
			break;
		default:
			break;
		}
	}

	return bResult;
}

VOID CRXSkinLocationPickerPopup::_OnCountryChanged()
{
	CRXSkinComboBox* poComboProvice = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_PROVINCE);
	CRXSkinComboBox* poComboCountry = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_COUNTRY);
	if (poComboCountry && poComboProvice)
	{
		poComboProvice->ResetContent();
		UINT32 dwCountryID =(UINT32) poComboCountry->GetItemData(poComboCountry->GetCurSel());
		
		
	}
}

VOID CRXSkinLocationPickerPopup::_OnProvinceChanged()
{
	CRXSkinComboBox* poComboProvice = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_PROVINCE);
	CRXSkinComboBox* poComboCountry = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_COUNTRY);
	CRXSkinComboBox* poComboCity = (CRXSkinComboBox*)m_oControlMgr.FindControl(ID_LOCATIONPICKER_COMBOBOX_CITY);
	if (poComboCountry && poComboProvice && poComboCity)
	{
		poComboCity->ResetContent();
		UINT32 dwCountryID = (UINT32)poComboCountry->GetItemData(poComboCountry->GetCurSel());
		
	}
}
BOOL CRXSkinLocationPickerPopup::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinWnd::OnCreate(lpCreate);
	if (bResult)
	{
		CRXSkinComboBox* poControlCountry = new CRXSkinComboBox;
		poControlCountry->SetRect(RECT_COUNTRY);
		poControlCountry->SetID(ID_LOCATIONPICKER_COMBOBOX_COUNTRY);
		poControlCountry->SetParentWnd(m_hWnd);
		poControlCountry->SetCaptureWnd(m_hWnd);
		poControlCountry->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
		poControlCountry->Create(ID_LOCATIONPICKER_COMBOBOX_COUNTRY,RECT_COUNTRY,COMBOBOX_DROPLIST);


		CRXSkinComboBox* poControlProvince  = new CRXSkinComboBox;
		poControlProvince->SetRect(RECT_PROVINCE);
		poControlProvince->SetID(ID_LOCATIONPICKER_COMBOBOX_PROVINCE);
		poControlProvince->SetParentWnd(m_hWnd);
		poControlProvince->SetCaptureWnd(m_hWnd);
		poControlProvince->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
		poControlProvince->Create(ID_LOCATIONPICKER_COMBOBOX_PROVINCE,RECT_PROVINCE,COMBOBOX_DROPLIST);
		m_oControlMgr.AddControl(poControlProvince);

		CRXSkinComboBox* poControlCity = new CRXSkinComboBox;
		poControlCity->SetRect(RECT_CITY);
		poControlCity->SetID(ID_LOCATIONPICKER_COMBOBOX_CITY);
		poControlCity->SetParentWnd(m_hWnd);
		poControlCity->SetCaptureWnd(m_hWnd);
		poControlCity->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
		poControlCity->Create(ID_LOCATIONPICKER_COMBOBOX_CITY,RECT_CITY,COMBOBOX_DROPLIST);
		m_oControlMgr.AddControl(poControlCity);
		INT32 nCountCountry = poControlCountry->GetCount();
		for(INT32 n = 0;n < nCountCountry; n++)
		{
			if (_RXTStrcmp(poControlCountry->GetLBText(n) , m_strCountry.GetData()) == 0)
			{
				poControlCountry->SetCurSel(n);
				break;
			}
		}
		INT32 nCountProvince = poControlProvince->GetCount();
		for(INT32 n = 0;n < nCountProvince; n++)
		{
			if (_RXTStrcmp(poControlProvince->GetLBText(n) , m_strProvince.GetData()) == 0)
			{
				poControlProvince->SetCurSel(n);
				break;
			}
		}
		INT32 nCountCity = poControlCity->GetCount();
		for(INT32 n = 0;n < nCountCity; n++)
		{
			if (_RXTStrcmp(poControlCity->GetLBText(n) , m_strCity.GetData()) == 0)
			{
				poControlCity->SetCurSel(n);
				break;
			}
		}
	}
	return TRUE;

}

BOOL CRXSkinLocationPickerPopup::OnMouseMove(UINT nFlags, POINT point)
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

BOOL CRXSkinLocationPickerPopup::OnLButtonDown(UINT nFlags, POINT point)
{
	return m_oControlMgr.OnLButtonDown(point)?TRUE:FALSE;
}

BOOL CRXSkinLocationPickerPopup::OnLButtonUp(UINT nFlags, POINT point)
{
	RECT rc;
	GetClientRect(m_hWnd,&rc);
	CRXSkinRect rcClient = rc;
	if (rcClient.PtInRect(point) == FALSE)
	{
		Show(FALSE,point);
		return TRUE;
	}
	else
	{
		return m_oControlMgr.OnClick(point);
	}
	return FALSE;
}

VOID CRXSkinLocationPickerPopup::OnPaint()
{
	RECT rcClient;
	GetClientRect(m_hWnd,&rcClient);
	PAINTSTRUCT ps = { 0 };
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	CRXSkinMemDC memDC(hDC,rcClient);

	DrawRect(memDC.GetHDC(),
		CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor(),
		CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(),
		rcClient.left,rcClient.top,rcClient.right,rcClient.bottom
		);
	m_oControlMgr.OnDraw(memDC.GetSurface(),memDC.GetHDC());
	memDC.TakeSnapshot();
	EndPaint(m_hWnd,&ps);
}

VOID CRXSkinLocationPickerPopup::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{

}

CRXSkinLocationPicker::CRXSkinLocationPicker(void)
{
	m_poLocationPickerPopup = NULL;
	m_strTipValue = _T("");
	m_clrTipText = RGB(0, 0, 0);
	m_clrTipBG = RGB(255, 255, 255);
}

CRXSkinLocationPicker::~CRXSkinLocationPicker(void)
{
	if (m_poLocationPickerPopup)
	{
		delete m_poLocationPickerPopup;
		m_poLocationPickerPopup = NULL;
	}
}

VOID CRXSkinLocationPicker::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible())
	{
		SetRect(rcItem);
		CRXSkinService::Instance()->GetDrawMgr().DrawControlBorder(hDC,GetState(),rcItem);
		rcItem.left += CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
		rcItem.right -= CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
		rcItem.top += CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cy;
		rcItem.bottom -= CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cy;
		rcItem.left += 2;
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

		CRXSkinRect rcText = m_rcItem;

		rcText.left += CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
		rcText.right -= CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
		rcText.top += CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cy;
		rcText.bottom -= CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cy;
		if (m_strTipValue.GetLength() > 0 && IsEmpty())
		{
			HFONT hOldFont = (HFONT)::SelectObject(hDC, CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
			tstring strTxt = m_strTipValue.GetData();
			DrawRect(hDC,
				m_clrTipBG,
				rcText.left,
				rcText.top,
				rcText.right,
				rcText.bottom);
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
}

VOID CRXSkinLocationPicker::ReleaseControl()
{
	delete this;
}

BOOL CRXSkinLocationPicker::PtInRect(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}

bool CRXSkinLocationPicker::IsEmpty()
{
	return m_strProvince.size() == 0 &&
		m_strCountry.size() == 0 &&
		m_strCity.size() == 0;
}

BOOL CRXSkinLocationPicker::OnClick(POINT pt)
{
	if (PtInRect(pt))
	{
		POINT pt;
		pt.x = GetRect().left;
		pt.y = GetRect().bottom;
		ClientToScreen(GetParentWnd(),&pt);

		if (NULL == m_poLocationPickerPopup)
		{
			m_poLocationPickerPopup = new CRXSkinLocationPickerPopup;
			m_poLocationPickerPopup->Create(WND_LOCATION_PICKER_POPUP,
				_T(""),WS_POPUP,GetParentWnd(),NULL);
			m_poLocationPickerPopup->SetDataPicker(this);
		}
				
	
		tstring strTxt = GetText();
		vector<tstring> vec = _RXTSplitStrings(strTxt,_T('-'));
		if (vec.size() == 3)
		{
			m_poLocationPickerPopup->SetCountry(vec[0].c_str());
			m_poLocationPickerPopup->SetProvince(vec[1].c_str());
			m_poLocationPickerPopup->SetCity(vec[2].c_str());
		}
	
		m_poLocationPickerPopup->Show(TRUE,pt);
		return TRUE;
	}
	return FALSE;
}
BOOL CRXSkinLocationPicker::OnMouseMove(POINT pt)
{
	return PtInRect(pt);
}
BOOL CRXSkinLocationPicker::OnLButtonDown(POINT pt)
{
	return PtInRect(pt);
}

BOOL CRXSkinLocationPicker::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	return FALSE;
}

VOID CRXSkinLocationPicker::SetCity(const TCHAR* pszCity)
{
	m_strCity = pszCity;
}

TCHAR* CRXSkinLocationPicker::GetCity()
{
	return (TCHAR*)m_strCity.c_str();
}

VOID CRXSkinLocationPicker::SetCountry(const TCHAR* pszCountry)
{
	m_strCountry = pszCountry;
}

TCHAR* CRXSkinLocationPicker::GetCountry()
{
	return (TCHAR*)m_strCountry.c_str();
}

VOID CRXSkinLocationPicker::SetProvince(const TCHAR* pszProvince)
{
	m_strProvince = pszProvince;
}

TCHAR* CRXSkinLocationPicker::GetProvince()
{
	return (TCHAR*)m_strProvince.c_str();
}

VOID CRXSkinLocationPicker::SetText(const TCHAR* strTxt)
{
	CRXControl::SetText(strTxt);
	CRXSkinString strLocation = strTxt;
	INT32 nPos = strLocation.Find(_T("-"));
	if (nPos>=0)
	{
		CRXSkinString strYear = strLocation.Left(nPos);
		SetCountry(strYear.GetData());
		strLocation = strLocation.Right(strLocation.GetLength() - 1 - nPos);
	}
	nPos = strLocation.Find(_T("-"));
	if (nPos>=0)
	{
		CRXSkinString strMonth = strLocation.Left(nPos);
		SetProvince(strMonth.GetData());
		strLocation = strLocation.Right(strLocation.GetLength() - 1 - nPos);
		SetCity(strLocation);
	}
}