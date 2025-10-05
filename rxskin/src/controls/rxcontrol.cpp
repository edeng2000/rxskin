#include "pch.h"
#include "controls/rxcontrol.h"
#include "controls/rxskinmenu.h"
#include "controls/rxskinscrollbar.h"

#include "common/graphics.h"
#include "common/rxskindefines.h"
#include "common/imagemgr.h"
#include "common/rxskinservice.h"
#include "common/rxskinfont.h"

RXSKIN_API EControlType RXAPI RXSkinGetControlType(const CHAR* pszControlType)
{
	EControlType enType = CONTROL_NONE;
	if (strcmp(pszControlType,"RXSkinText") == 0)
	{
		enType = CONTROL_TEXT;
	}
	else if (strcmp(pszControlType,"RXSkinButton") == 0)
	{
		enType = CONTROL_BUTTON;
	}
	else if (strcmp(pszControlType,"RXSkinRadioList") == 0)
	{
		enType = CONTROL_RADIOLIST;
	}
	else if (strcmp(pszControlType,"RXSkinStatusButton") == 0)
	{
		enType = CONTROL_STATUS_BUTTON;
	}
	else if (strcmp(pszControlType,"RXSkinHyperLink") == 0)
	{
		enType = CONTROL_HYPERLINK;
	}
	else if (strcmp(pszControlType,"RXSkinCheckbox") == 0)
	{
		enType = CONTROL_CHECKBOX_BUTTON;
	}
	else if (strcmp(pszControlType,"RXSkinMenuButton") == 0)
	{
		enType = CONTROL_MENU_BUTTON;
	}
	else if (strcmp(pszControlType,"RXSkinStatic") == 0)
	{
		enType = CONTROL_STATIC;
	}
	else if (strcmp(pszControlType,"RXSkinImageButton") == 0)
	{
		enType = CONTROL_IMAGE_BUTTON;
	}
	else if (strcmp(pszControlType,"RXSkinCheckboxImageButton") == 0)
	{
		enType = CONTROL_CHECKBOX_IMAGE_BUTTON;
	}
	else if (strcmp(pszControlType,"RXSkinTxtButton") == 0)
	{
		enType = CONTROL_TXT_BUTTON;
	}
	else if (strcmp(pszControlType,"RXSkinSperator") == 0)
	{
		enType = CONTROL_SPERATOR;
	}
	else if (strcmp(pszControlType,"RXSkinButtonList") == 0)
	{
		enType = CONTROL_BUTTON_LIST;
	}
	else if (strcmp(pszControlType,"RXSkinGroupLine") == 0)
	{
		enType = CONTROL_GROUP_LINE;
	}
	else if (strcmp(pszControlType,"RXSkinComboBox") == 0)
	{
		enType = CONTROL_COMBOBOX;
	}
	else if (strcmp(pszControlType,"RXSkinEdit") == 0)
	{
		enType = CONTROL_EDIT;
	}
	else if (strcmp(pszControlType,"RXSkinStaticImage") == 0)
	{
		enType = CONTROL_STATIC_IMAGE;
	}
	else if (strcmp(pszControlType,"RXSkinTabCtrl") == 0)
	{
		enType = CONTROL_TAB_CTRL;
	}
	else if (strcmp(pszControlType,"RXSkinDatePicker") == 0)
	{
		enType = CONTROL_DATE_PICKER;
	}
	else if (strcmp(pszControlType,"RXSkinLocationPicker") == 0)
	{
		enType = CONTROL_LOCATION_PICKER;
	}
	else if (strcmp(pszControlType,"RXSkinPicture") == 0)
	{
		enType = CONTROL_PICTURE;
	}
	else if (strcmp(pszControlType,"RXSkinGif") == 0)
	{
		enType = CONTROL_GIF;
	}
	else if (strcmp(pszControlType,"RXSkinStatusButton") == 0)
	{
		enType = CONTROL_STATUS_BUTTON;
	}
	else if (strcmp(pszControlType,"RXSkinOutbar") == 0)
	{
		enType = CONTROL_OUTBAR;
	}
	else if (strcmp(pszControlType,"RXSkinButtonList") == 0)
	{
		enType = CONTROL_BUTTON_LIST;
	}
	else if (strcmp(pszControlType, "RXSkinProgress") == 0)
	{
		enType = CONTROL_PROGRESS;
	}
	else if (strcmp(pszControlType,"RXSkinBrownser") == 0)
	{
		enType = CONTROL_BROWNSER;
	}
	else if (strcmp(pszControlType,"RXSkinListCtrl") == 0)
	{
		enType = CONTROL_LISTCTRL;
	}
	else if (strcmp(pszControlType,"RXSkinSpinEdit") == 0)
	{
		enType = CONTROL_SPIN_EDIT;
	}
	else if (strcmp(pszControlType,"RXSkinBG") == 0)
	{
		enType = CONTROL_BG;
	}
	else if (strcmp(pszControlType, "RXSkinWebBrowser") == 0)
	{
		enType = CONTROL_WEB_BROWSER;
	}
	else if (strcmp(pszControlType, "RXSkinMenubar") == 0)
	{
		enType = CONTROL_MENUBAR;
	}
	else if (strcmp(pszControlType, "RXSkinSlider") == 0)
	{
		enType = CONTROL_SLIDER;
	}
	else if (strcmp(pszControlType, "RXSkinTree") == 0)
	{
		enType = CONTROL_TREE;
	}
	else if (strcmp(pszControlType, "RXSkinRichEdit") == 0)
	{
		enType = CONTROL_RICHEDIT;
	}
	else if (strcmp(pszControlType, "RXSkinSearchEdit") == 0)
	{
		enType = CONTROL_SEARCH_EDIT;
	}
	return enType;
}


RXSKIN_API CRXSkinResizePoint RXAPI RXSkinGetAlignType(const CHAR* pszAlignType)
{
	CRXSkinResizePoint enType;
	if (strcmp(pszAlignType,"BOTTOM_CENTER") == 0)
	{
		enType = RXSKIN_ANCHOR_BOTTOMCENTER;
	}
	else if (strcmp(pszAlignType,"BOTTOM_LEFT") == 0)
	{
		enType = RXSKIN_ANCHOR_BOTTOMLEFT;
	}
	else if (strcmp(pszAlignType,"BOTTOM_RIGHT") == 0)
	{
		enType = RXSKIN_ANCHOR_BOTTOMRIGHT;
	}
	else if (strcmp(pszAlignType,"MIDDLE_CENTER") == 0)
	{
		enType = RXSKIN_ANCHOR_MIDDLECENTER;
	}
	else if (strcmp(pszAlignType,"MIDDLE_LEFT") == 0)
	{
		enType = RXSKIN_ANCHOR_MIDDLELEFT;
	}
	else if (strcmp(pszAlignType,"MIDDLE_RIGHT") == 0)
	{
		enType = RXSKIN_ANCHOR_MIDDLERIGHT;
	}
	else if (strcmp(pszAlignType,"TOP_CENTER") == 0)
	{
		enType = RXSKIN_ANCHOR_TOPCENTER;
	}
	else if (strcmp(pszAlignType,"TOP_LEFT") == 0)
	{
		enType = RXSKIN_ANCHOR_TOPLEFT;
	}
	else if (strcmp(pszAlignType,"TOP_RIGHT") == 0)
	{
		enType = RXSKIN_ANCHOR_TOPRIGHT;
	}
	return enType;
}

CRXControl::CRXControl(void) :m_bMouseEnabled(true)
{
	memset(m_szToolTip,0,sizeof(TCHAR)*256);
	memset(m_szText,0,sizeof(TCHAR)*256);
	m_rcItem.SetRectEmpty();
	m_dwID = 0;
	m_enState = CONTROL_STATE_NORMAL;
	m_hParentWnd = NULL;
	m_rrcSizing.bottom = m_rrcSizing.left = m_rrcSizing.right = m_rrcSizing.top = 0;
	m_bEnabled = TRUE;
	m_bVisible = TRUE;
	m_bBorder = false;
}

CRXControl::~CRXControl(void)
{
}

BOOL CRXControl::IsVisible()
{
	return m_bVisible;
}

VOID CRXControl::SetVisible(const BOOL bIsVisible)
{
	m_bVisible = bIsVisible;
}

TCHAR* CRXControl::GetText()
{
	return m_szText;
}

CRXControl* CRXControl::HitTest(POINT pt)
{
	if (m_rcItem.PtInRect(pt))
	{
		return this;
	}
	return NULL;
}

VOID CRXControl::Invalidate()
{
	if (GetParentWnd())
	{
		RECT rc;
		rc.bottom = m_rcItem.bottom;
		rc.top = m_rcItem.top;
		rc.left = m_rcItem.left;
		rc.right = m_rcItem.right;
		::InvalidateRect(m_hParentWnd,&rc,FALSE);
	}
}

VOID CRXControl::SetText(const TCHAR* strTxt)
{
	memset(m_szText,0,sizeof(TCHAR)*256);
	_tcsncpy(m_szText,strTxt,256);

}

VOID CRXControl::SetTooltip(const TCHAR* strTxt)
{
	memset(m_szToolTip,0,sizeof(TCHAR)*256);
	_tcsncpy(m_szToolTip,strTxt,256);
}

TCHAR* CRXControl::GetTooltip()
{
	return m_szToolTip;
}

CRXSkinRect CRXControl::GetRect()
{
	return m_rcItem;
}

CRXSkinRect CRXControl::GetInvalidateRect()
{
	return m_rcItem;
}

CRXControl* CRXControl::GetInvalidateControl()
{
	return this;
}

VOID CRXControl::SetRect(const CRXSkinRect rcItem)
{
	m_rcItem = rcItem;
}

POINT CRXControl::GetWindowPos()
{
	RECT rcWin = GetRect();
	POINT pt;
	pt.x = rcWin.left;
	pt.y = rcWin.top;
	ClientToScreen(m_hParentWnd, &pt);
	
	return pt;
}

UINT32 CRXControl::GetID()
{
	return m_dwID;
}

void CRXControl::ModifyRect()
{

}

void CRXControl::OffsetRect(int32_t nX, int32_t nY)
{

}

HDC CRXControl::GetPaintDC()
{
	CRXSkinWnd* poWnd = CRXSkinService::Instance()->FindWnd(m_hParentWnd);
	if (poWnd)
	{
		return poWnd->GetPaintDC();
	}
	return NULL;
}
bool CRXControl::IsDCReady()
{
	CRXSkinWnd* poWnd = CRXSkinService::Instance()->FindWnd(m_hParentWnd);
	if (poWnd)
	{
		return poWnd->IsDCReady();
	}
	return false;
}
VOID CRXControl::SetID(const UINT32 dwID)
{
	m_dwID = dwID;
}

EControlState CRXControl::GetState()
{
	return m_enState;
}

LRESULT CRXControl::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CRXControl::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


VOID CRXControl::SetState(EControlState enState)
{
	m_enState = enState;
}

LRESULT CRXControl::SendMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	ASSERT(::IsWindow(m_hParentWnd));
	return ::SendMessage(m_hParentWnd, uMsg, wParam, lParam);
}

LRESULT CRXControl::PostMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	ASSERT(::IsWindow(m_hParentWnd));
	return ::PostMessage(m_hParentWnd, uMsg, wParam, lParam);
}

CRXSkinWnd* CRXControl::GetPaintWnd()
{
	CRXSkinWnd* poWnd = CRXSkinService::Instance()->FindWnd(m_hParentWnd);
	return poWnd;
}


VOID CRXControl::SetParentWnd(const HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
	RECT rc;
	::GetClientRect(m_hParentWnd,&rc);
	m_rcLast = rc;
}

HWND CRXControl::GetParentWnd()
{
	return m_hParentWnd;
}

CRXSkinRect& CRXControl::GetLast()
{
	return m_rcLast;
}

VOID CRXControl::SetLastRect(CRXSkinRect rcLast)
{
	m_rcLast = rcLast;
}

VOID CRXControl::OnMouseLeave()
{

}

VOID CRXControl::SetResize(const RXSKIN_RESIZEPOINT& rpTopLeft, const RXSKIN_RESIZEPOINT& rpBottomRight)
{
	m_rrcSizing = CRXSkinResizeRect(rpTopLeft.x, rpTopLeft.y, rpBottomRight.x, rpBottomRight.y);
	RECT rc;
	::GetClientRect(m_hParentWnd,&rc);
	CRXSkinRect rcClient = rc;
	m_oMarginTopLeft.cx = m_rcItem.left - (INT32)(rcClient.Width() * m_rrcSizing.left);
	m_oMarginTopLeft.cy = m_rcItem.top - (INT32)(rcClient.Height() * m_rrcSizing.top);

	// calculate margin for the bottom-right corner

	m_oMarginBottomRight.cx = m_rcItem.right - (INT32)(rcClient.Width() * m_rrcSizing.right);
	m_oMarginBottomRight.cy = m_rcItem.bottom - (INT32)(rcClient.Height() * m_rrcSizing.bottom);

}

CRXSkinResizeRect& CRXControl::GetResize()
{
	return m_rrcSizing;
}

BOOL CRXControl::OnMessage(UINT32 dwMessageID,WPARAM wParam,LPARAM pParam)
{
	return FALSE;
}

BOOL CRXControl::OnCommand(WPARAM wParam,LPARAM pParam)
{
	return FALSE;
}

HBRUSH CRXControl::OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC)
{
	return NULL;
}

BOOL CRXControl::OnMouseWheel(UINT nFlags, short zDelta,POINT pt)
{
	return FALSE;
}

VOID CRXControl::OnScrollBarChanged(EScrollType enType,INT32 nPercent)
{
}

BOOL CRXControl::OnTimer(WPARAM wParam,LPARAM pParam)
{
	return FALSE;
}

LRESULT CRXControl::OnEditChanged(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

BOOL CRXControl::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	CRXSkinResizeRect& rcSizing = GetResize();
	if (rcSizing.left == 0 &&
		rcSizing.right == 0 &&
		rcSizing.top == 0 &&
		rcSizing.bottom == 0)
	{
		return FALSE;
	}
	if (!rcSizing.IsNormalized())
	{
		return FALSE;
	}

 	RECT rc;
	::GetClientRect(GetParentWnd(),&rc);
	CRXSkinRect rcClient = rc;
	CRXSkinRect rectNew;
	rectNew.left = m_oMarginTopLeft.cx + (INT32)(rcClient.Width() * rcSizing.left);
	rectNew.top = m_oMarginTopLeft.cy + (INT32)(rcClient.Height() * rcSizing.top);
	rectNew.right = m_oMarginBottomRight.cx + (INT32)(rcClient.Width() * rcSizing.right);
	rectNew.bottom = m_oMarginBottomRight.cy + (INT32)(rcClient.Height() * rcSizing.bottom);
	SetRect(rectNew);
	return TRUE;
}

BOOL CRXControl::IsEnabled()
{
	return m_bEnabled;
}

VOID CRXControl::Enable(const BOOL bEnabled)
{
	m_bEnabled = bEnabled;
}

////////////  控件管理器  /////////////////
CRXControlMgr::CRXControlMgr()
{

}

CRXControlMgr::~CRXControlMgr()
{
	_DeleteAll();
}

BOOL CRXControlMgr::AddControl(CRXControl* poControl)
{
	CRXControl* poFind = FindControl(poControl->GetID());
	if (NULL == poFind)
	{
		m_oControlMap.insert(make_pair(poControl->GetID(),poControl));
		return TRUE;
	}
	return FALSE;
}

CRXControl* CRXControlMgr::FindControl(const UINT32 dwID)
{
	CRXControlMapItr itr = m_oControlMap.find(dwID);
	if (itr!=m_oControlMap.end())
	{
		return itr->second;
	}
	return NULL;
}

CRXControl* CRXControlMgr::RemoveControl(const UINT32 dwID)
{
	CRXControlMapItr itr = m_oControlMap.find(dwID);
	if (itr!=m_oControlMap.end())
	{
		CRXControl* poControl = itr->second;
		m_oControlMap.erase(itr);
		return poControl;
	}
	return NULL;
}

CRXControl* CRXControlMgr::HitTest(POINT pt)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl&& poControl->IsVisible() )
		{
			poControl = poControl->HitTest(pt);
			if (poControl)
			{
				return poControl;
			}	
		}
	}
	return NULL;
}

VOID CRXControlMgr::_DeleteAll()
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl)
		{
			poControl->ReleaseControl();
		}
	}
	m_oControlMap.clear();
}

BOOL CRXControlMgr::OnClick(POINT pt)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl && poControl->OnClick(pt))
		{
 			return TRUE;
		}
	}
	return FALSE;
}

VOID CRXControlMgr::SetVisible(const BOOL bIsVisible)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl)
		{
			poControl->SetVisible(bIsVisible);
		}
	}
}

BOOL CRXControlMgr::OnMouseWheel(UINT nFlags, short zDelta,POINT pt)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl && poControl->OnMouseWheel(nFlags,zDelta,pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

VOID CRXControlMgr::OnScrollBarChanged(EScrollType enType,INT32 nPercent)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl)
		{
			poControl->OnScrollBarChanged(enType,nPercent);
		}
	}
}

CRXControl* CRXControlMgr::OnLButtonDown(POINT pt)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl && poControl->OnLButtonDown(pt))
		{
			return poControl;
		}
	}
	return NULL;
}

CRXControl* CRXControlMgr::OnLButtonUp(POINT pt)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (; itr != m_oControlMap.end(); itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl && poControl->OnLButtonUp(pt))
		{
			return poControl;
		}
	}
	return NULL;
}
CRXControl* CRXControlMgr::OnRButtonUp(POINT pt)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (; itr != m_oControlMap.end(); itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl && poControl->OnRButtonUp(pt))
		{
			return poControl;
		}
	}
	return NULL;
}
CRXControl* CRXControlMgr::OnLButtonDblClk(POINT pt)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (; itr != m_oControlMap.end(); itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl && poControl->OnLButtonDblClk(pt))
		{
			return poControl;
		}
	}
	return NULL;
}


LRESULT CRXControlMgr::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (; itr != m_oControlMap.end(); itr++)
	{
		CRXControl* poControl = itr->second;
		poControl->OnKillFocus(wParam, lParam);
	}
	return 0;
}
LRESULT CRXControlMgr::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (; itr != m_oControlMap.end(); itr++)
	{
		CRXControl* poControl = itr->second;
		poControl->OnSetFocus(wParam, lParam);
	}
	return 0;
}

CRXControl* CRXControlMgr::OnMouseMove(POINT pt)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl && poControl->OnMouseMove(pt))
		{
			return poControl;
		}
	}
	return NULL;
}

VOID CRXControlMgr::OnMouseLeave()
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl)
		{
			poControl->OnMouseLeave();
		}
	}
}

bool CRXControl::IsFocused() const
{
	return m_bFocused;
}

void CRXControl::SetFocus()
{
	m_bFocused = true;
	
}

bool CRXControl::IsMouseEnabled() const
{
	return m_bMouseEnabled;
}


BOOL CRXControlMgr::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	BOOL bSized = FALSE;
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl && poControl->OnSize(dwX,dwY))
		{
			bSized = TRUE;
		}
	}
	return FALSE;
}

VOID CRXControlMgr::OnDraw(SkSurface* poSurface, HDC hDC)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		poControl->OnDraw(poSurface,hDC,poControl->GetRect());
	}
}

BOOL CRXControlMgr::OnTimer(WPARAM wParam, LPARAM lParam)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl && poControl->OnTimer(wParam,lParam))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXControlMgr::OnMessage(UINT32 dwMessageID,WPARAM wParam,LPARAM pParam)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl && poControl->OnMessage(dwMessageID,wParam,pParam))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXControlMgr::OnCommand(WPARAM wParam,LPARAM pParam)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl!= nullptr && poControl->IsEnabled() && poControl->IsVisible())
		{
			if(poControl->OnCommand(wParam,pParam))
				return TRUE;
		}
	}
	return FALSE;
}

LRESULT CRXControlMgr::OnEditChanged(WPARAM wParam, LPARAM lParam)
{
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl)
		{	
			LRESULT _b = poControl->OnEditChanged(wParam, lParam);
			if (_b != 0)
			{
				return _b;
			}
		}
	}
	return 0;
}

HBRUSH CRXControlMgr::OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC)
{
	HBRUSH _brush = NULL;
	CRXControlMapItr itr = m_oControlMap.begin();
	for (;itr!=m_oControlMap.end();itr++)
	{
		CRXControl* poControl = itr->second;
		if (poControl)
		{	
			HBRUSH _b = poControl->OnCtlColorControl(dwMsgID,hWndChild,hDC);
			if (_b != NULL)
			{
				_brush = _b;
			}
		}
	}
	return _brush;
}
////////////  控件管理器  /////////////////


////////////  Radio控件   /////////////////
#define RADIO_OFFSET   7
CRXRadioButton::CRXRadioButton()
{
	m_bChecked = FALSE;
}

CRXRadioButton::~CRXRadioButton()
{
}

VOID CRXRadioButton::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	INT32 nTextLeft = 0;
	if (FALSE == IsEnabled())
	{
		if (GetChecked())
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_RADIO_CHECKED);
			if (poImage)
			{
				INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
				INT32 nLeft = rcItem.left + RADIO_OFFSET;
				nTextLeft += RADIO_OFFSET*2;
				nTextLeft += poImage->GetWidth(); 
				CRXSkinRect rcImage;
				rcImage.left = nLeft;
				rcImage.top = nTop;
				rcImage.right = rcImage.left + poImage->GetWidth();
				rcImage.bottom = rcImage.top + poImage->GetHeight();
				CRXSkinService::Instance()->GetDrawMgr().DrawImageGray(poImage,rcImage,hDC);
			}
		}
		else
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_RADIO_NORMAL);
			if (poImage)
			{
				INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
				INT32 nLeft = rcItem.left + RADIO_OFFSET;
				nTextLeft += RADIO_OFFSET*2;
				nTextLeft += poImage->GetWidth(); 
				CRXSkinRect rcImage;
				rcImage.left = nLeft;
				rcImage.top = nTop;
				rcImage.right = rcImage.left + poImage->GetWidth();
				rcImage.bottom = rcImage.top + poImage->GetHeight();
				CRXSkinService::Instance()->GetDrawMgr().DrawImageGray(poImage,rcImage,hDC);
			}
		}
	}
	else
	{
		if (GetChecked())
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_RADIO_CHECKED);
			if (poImage)
			{
				INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
				INT32 nLeft = rcItem.left + RADIO_OFFSET;
				nTextLeft += RADIO_OFFSET*2;
				nTextLeft += poImage->GetWidth(); 
				poImage->Stretch(hDC,nLeft,nTop,poImage->GetWidth(),poImage->GetHeight());
			}
		}
		else
		{
			if (GetState() == CONTROL_STATE_MOUSEON)
			{
				IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_RADIO_HOT);
				if (poImage)
				{
					INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
					INT32 nLeft = rcItem.left + RADIO_OFFSET;
					nTextLeft += RADIO_OFFSET*2;
					nTextLeft += poImage->GetWidth(); 
					poImage->Stretch(hDC,nLeft,nTop,poImage->GetWidth(),poImage->GetHeight());
				}
			}
			else
			{
				IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_RADIO_NORMAL);
				if (poImage)
				{
					INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
					INT32 nLeft = rcItem.left + RADIO_OFFSET;
					nTextLeft += RADIO_OFFSET*2;
					nTextLeft += poImage->GetWidth(); 
					poImage->Stretch(hDC,nLeft,nTop,poImage->GetWidth(),poImage->GetHeight());
				}
			}
		}
	}

	INT32 nModel = ::SetBkMode(hDC,TRANSPARENT);
	HFONT hOld = (HFONT)SelectObject(hDC,CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
	COLORREF clrOld;
	if (IsEnabled())
	{
		clrOld =CRXSkinService::Instance()->GetDrawMgr().GetButtonListNormalText();
	}
	else
	{
		clrOld = CRXSkinService::Instance()->GetDrawMgr().GetButtonListNormalText();
	}
	 
	CRXSkinRect rcTxt = rcItem;
	rcTxt.left += nTextLeft;
	tstring strTxt = GetText();
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
			clrOld,
			ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	}
	::SelectObject(hDC,hOld);
	::SetBkMode(hDC,nModel);
	::SetTextColor(hDC,clrOld);
}

BOOL CRXRadioButton::PtInRect(POINT pt)
{
	return GetRect().PtInRect(pt);
}

VOID CRXRadioButton::ReleaseControl()
{
	delete this;
}

void CRXRadioButton::ModifyRect()
{

}

VOID CRXRadioButton::Enable(const BOOL bEnabled)
{
	CRXControl::Enable(bEnabled);
	Invalidate();
}

BOOL CRXRadioButton::IsEnabled()
{
	return CRXControl::IsEnabled();
}

BOOL CRXRadioButton::GetChecked()
{
	return m_bChecked;
}

VOID CRXRadioButton::SetChecked(const BOOL bChecked)
{
	m_bChecked = bChecked;
}

BOOL CRXRadioButton::OnClick(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (PtInRect(pt))
		{
			SetChecked(TRUE);
			if (GetParentWnd())
			{
				::SendMessage(GetParentWnd(),WM_COMMAND,GetID(),0);
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXRadioButton::OnLButtonDown(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (PtInRect(pt))
		{
			if (GetState()!=CONTROL_STATE_PRESSED)
			{
				SetState(CONTROL_STATE_PRESSED);
				Invalidate();
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXRadioButton::OnMouseMove(POINT pt)
{
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_MOUSEON)
		{
			SetState(CONTROL_STATE_MOUSEON);
			Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CRXRadioButton::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	CRXSkinResizeRect& rcSizing = GetResize();
	if (rcSizing.left == 0 &&
		rcSizing.right == 0 &&
		rcSizing.top == 0 &&
		rcSizing.bottom == 0)
	{
		return FALSE;
	}
	if (!rcSizing.IsNormalized())
	{
		return FALSE;
	}
	CRXSkinRect& rcLast =GetLast();

	INT32 nWidth = dwX - rcLast.Width();
	CRXSkinRect rcItem = GetRect();
	rcItem.left = rcItem.left + (INT32)((float)nWidth * rcSizing.left);
	rcItem.right = rcItem.right + (INT32)((float)nWidth * rcSizing.right);
	INT32 nHeight = dwY - rcLast.Height();
	rcItem.top = rcItem.top + (INT32)((float)nHeight * rcSizing.top);
	rcItem.bottom = rcItem.bottom + (INT32)((float)nHeight * rcSizing.bottom);

	return TRUE;
}

////////////  Radio控件   /////////////////////

////////////  Radio控件列表   /////////////////
CRXRadioButtonList::CRXRadioButtonList()
{
	m_poSelected = NULL;
	m_poPressed = NULL;
}

CRXRadioButtonList::~CRXRadioButtonList()
{
	_DeleteAll();
}

BOOL CRXRadioButtonList::AddRadioButton(CRXRadioButton* poControl)
{
	CRXRadioButton* poFind = FindRadioButton(poControl->GetID());
	if (NULL == poFind)
	{
		m_lstRadioButton.push_back(poControl);
		return TRUE;
	}
	return FALSE;
}

CRXRadioButton* CRXRadioButtonList::FindRadioButton(const UINT32 dwID)
{
	CRXRadioButtonLstItr itr = m_lstRadioButton.begin();
	for (;itr!=m_lstRadioButton.end();itr++)
	{
		CRXRadioButton* poRadio = (*itr);
		if (poRadio && poRadio->GetID() == dwID)
		{
			return poRadio;
		}
	}
	return NULL;
}

CRXRadioButton* CRXRadioButtonList::RemoveRadioButton(const UINT32 dwID)
{
	CRXRadioButtonLstItr itr = m_lstRadioButton.begin();
	for (;itr!=m_lstRadioButton.end();itr++)
	{
		CRXRadioButton* poRadio = (*itr);
		if (poRadio && poRadio->GetID() == dwID)
		{
			m_lstRadioButton.erase(itr);
			return poRadio;
		}
	}
	return NULL;
}

UINT32 CRXRadioButtonList::GetSelect()
{
	if (m_poSelected)
	{
		return m_poSelected->GetID();
	}
	return 0;
}

VOID CRXRadioButtonList::Select(const UINT32 dwID)
{
	CRXRadioButton* poBtn = FindRadioButton(dwID);
	if (poBtn)
	{
		if (m_poSelected && m_poSelected!=poBtn)
		{
			m_poSelected->SetChecked(FALSE);
			m_poSelected->Invalidate();
			m_poSelected = poBtn;
			m_poSelected->SetChecked(TRUE);
			m_poSelected->Invalidate();
			
		}
		else
		{
			m_poSelected = poBtn;
			m_poSelected->SetChecked(TRUE);
			m_poSelected->Invalidate();
		}
	}
}

VOID CRXRadioButtonList::_DeleteAll()
{
	CRXRadioButtonLstItr itr = m_lstRadioButton.begin();
	for (;itr!=m_lstRadioButton.end();itr++)
	{
		CRXControl* poControl = *itr;
		if (poControl)
		{
			poControl->ReleaseControl();
		}
	}
	m_lstRadioButton.clear();
}

VOID CRXRadioButtonList::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	CRXRadioButtonLstItr itr = m_lstRadioButton.begin();
	for (;itr!=m_lstRadioButton.end();itr++)
	{
		CRXControl* poControl = *itr;
		if (poControl)
		{
			poControl->OnDraw(poSurface,hDC,poControl->GetRect());
		}
	}
}

BOOL CRXRadioButtonList::PtInRect(POINT pt)
{
	CRXRadioButtonLstItr itr = m_lstRadioButton.begin();
	for (;itr!=m_lstRadioButton.end();itr++)
	{
		CRXControl* poControl = *itr;
		if (poControl && poControl->PtInRect(pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXRadioButtonList::OnClick(POINT pt)
{
	BOOL bClick = FALSE;
	CRXRadioButton* poClick = FindRadioByPosition(pt);
	if (NULL == poClick)
	{
		return FALSE;
	}
	if (NULL == m_poSelected)
	{
		m_poSelected = poClick;
		if (m_poSelected->IsEnabled())
		{
			m_poSelected->SetChecked(TRUE);
			m_poSelected->Invalidate();
			::SendMessage(GetParentWnd(),WM_COMMAND,m_poSelected->GetID(),0);
		}
	}
	else
	{
		if (m_poSelected!=poClick)
		{
			if (m_poSelected->IsEnabled())
			{
				m_poSelected->SetChecked(FALSE);
				m_poSelected->Invalidate();				
			}
			

			m_poSelected = poClick;
			if (m_poSelected->IsEnabled())
			{
				m_poSelected->SetChecked(TRUE);
				m_poSelected->Invalidate();
				::SendMessage(GetParentWnd(),WM_COMMAND,m_poSelected->GetID(),0);
			}
		}
	}

	return FALSE;
}


BOOL CRXRadioButtonList::OnLButtonDown(POINT pt)
{
	BOOL bClick = FALSE;
	CRXRadioButton* poClick = FindRadioByPosition(pt);
	if (NULL == poClick)
	{
		return FALSE;
	}
	if (NULL == m_poPressed)
	{
		m_poPressed = poClick;
		if (m_poPressed->IsEnabled() && m_poPressed->IsVisible())
		{
			m_poPressed->SetState(CONTROL_STATE_PRESSED);
			m_poPressed->Invalidate();
		}
	}
	else
	{
		if (m_poPressed!=poClick)
		{
			if (m_poPressed->IsEnabled())
			{
				m_poPressed->SetState(CONTROL_STATE_NORMAL);
				m_poPressed->Invalidate();
			}


			m_poPressed = poClick;
			if (m_poPressed->IsEnabled())
			{
				m_poPressed->SetState(CONTROL_STATE_PRESSED);
				m_poPressed->Invalidate();
			}
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CRXRadioButtonList::OnMouseMove(POINT pt)
{
	CRXRadioButton* poClick = FindRadioByPosition(pt);
	if (NULL == poClick)
	{
		return FALSE;
	}
	CRXRadioButtonLstItr itr = m_lstRadioButton.begin();
	for (;itr!=m_lstRadioButton.end();itr++)
	{
		CRXRadioButton* poControl = *itr;
		if (poControl)
		{
			if (poControl == poClick)
			{
				poClick->OnMouseMove(pt);
			}
			else
			{
				if (poControl->GetState() == CONTROL_STATE_MOUSEON)
				{
					poControl->SetState(CONTROL_STATE_NORMAL);
					poControl->Invalidate();
					
				}
			}
		}
	}
	return TRUE;
}

CRXRadioButton* CRXRadioButtonList::FindRadioByPosition(POINT pt)
{
	CRXRadioButton* poFind = NULL;
	CRXRadioButtonLstItr itr = m_lstRadioButton.begin();
	for (;itr!=m_lstRadioButton.end();itr++)
	{
		CRXRadioButton* poControl = *itr;
		if (poControl && poControl->PtInRect(pt))
		{
			poFind = poControl;
			break;
		}
	}
	return poFind;
}

VOID CRXRadioButtonList::Enable(const BOOL bEnabled)
{
	CRXRadioButton* poFind = NULL;
	CRXRadioButtonLstItr itr = m_lstRadioButton.begin();
	for (;itr!=m_lstRadioButton.end();itr++)
	{
		CRXRadioButton* poControl = *itr;
		poControl->Enable(bEnabled);
	}
}


VOID CRXRadioButtonList::ReleaseControl()
{
	delete this;
}

void CRXRadioButtonList::ModifyRect()
{

}

void CRXRadioButtonList::OffsetRect(int32_t nX, int32_t nY)
{
	CRXRadioButtonLstItr itr = m_lstRadioButton.begin();
	for (; itr != m_lstRadioButton.end(); itr++)
	{
		CRXRadioButton* poControl = *itr;
		CRXSkinRect rc = poControl->GetRect();
		rc.left += nX;
		rc.right += nX;
		rc.top += nY;
		rc.bottom += nY;
		poControl->SetRect(rc);
	}
}

BOOL CRXRadioButtonList::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	BOOL bSized = FALSE;
	CRXRadioButtonLstItr itr = m_lstRadioButton.begin();
	for (;itr!=m_lstRadioButton.end();itr++)
	{
		CRXRadioButton* poControl = *itr;
		if (poControl && poControl->OnSize(dwX,dwY))
		{
			bSized = TRUE;
		}
	}
	return bSized;
}

////////////  Radio控件列表   /////////////////


////////////  Radio控件   /////////////////
CRXListButton::CRXListButton()
{
	m_bSelected = FALSE;
	m_poImage = NULL;
}

CRXListButton::~CRXListButton()
{
	if (m_poImage)
	{
		m_poImage->Release();
	}
	m_poImage = NULL;
}

VOID CRXListButton::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	INT32 nTextLeft = 0;
	COLORREF clrOld = CRXSkinService::Instance()->GetDrawMgr().GetButtonListNormalText();
	if (GetSelected())
	{
		switch(GetState())
		{
		case CONTROL_STATE_NORMAL:
			{
				clrOld = CRXSkinService::Instance()->GetDrawMgr().GetButtonListSelectedNormalText();
				HRGN hRgn = CreateRoundRectRgn(rcItem.left,
					rcItem.top,
					rcItem.right,
					rcItem.bottom,
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListRound().cx,
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListRound().cy);
				DrawGradientRgn(hDC,
					TRUE,
					hRgn,
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListSelectedNormalFrom(),
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListSelectedNormalTo());
				DeleteObject(hRgn);
			}
			break;
		case CONTROL_STATE_MOUSEON:
			{
				clrOld = CRXSkinService::Instance()->GetDrawMgr().GetButtonListSelectedMouseOnText();
				HRGN hRgn = CreateRoundRectRgn(rcItem.left,
					rcItem.top,
					rcItem.right,
					rcItem.bottom,
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListRound().cx,
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListRound().cy);
				DrawGradientRgn(hDC,
					TRUE,
					hRgn,
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListSelectedMouseOnFrom(),
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListSelectedMouseOnTo());
				DeleteObject(hRgn);
			}
			break;
		case CONTROL_STATE_PRESSED:
			{
				clrOld = ::SetTextColor(hDC,CRXSkinService::Instance()->GetDrawMgr().GetButtonListSelectedPressedText());
				HRGN hRgn = CreateRoundRectRgn(rcItem.left,
					rcItem.top,
					rcItem.right,
					rcItem.bottom,
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListRound().cx,
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListRound().cy);
				DrawGradientRgn(hDC,
					TRUE,
					hRgn,
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListSelectedPressedFrom(),
					CRXSkinService::Instance()->GetDrawMgr().GetButtonListSelectedPressedTo());
				DeleteObject(hRgn);
			}
			break;
		default:
			break;
		}
	}
	else
	{
		switch(GetState())
		{
		case CONTROL_STATE_NORMAL:
			{
				clrOld = CRXSkinService::Instance()->GetDrawMgr().GetButtonListNormalText();
			}
			break;
		case CONTROL_STATE_MOUSEON:
			{
				clrOld = CRXSkinService::Instance()->GetDrawMgr().GetButtonListMouseOnText();
			}
			break;
		case CONTROL_STATE_PRESSED:
			{
				clrOld = CRXSkinService::Instance()->GetDrawMgr().GetButtonListPressedText();
			}
			break;
		default:
			break;
		}
	}
	if (m_poImage == NULL)
	{
		m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenDirectImage(m_strImageFile.c_str());
	}
	if (m_poImage)
	{
		INT32 nTop = rcItem.top + (rcItem.Height() - m_poImage->GetHeight())/2;
		INT32 nLeft = rcItem.left + CRXSkinService::Instance()->GetDrawMgr().GetButtonListOffSetX();
		nTextLeft += CRXSkinService::Instance()->GetDrawMgr().GetButtonListOffSetX()*2;
		nTextLeft += m_poImage->GetWidth(); 
		CRXSkinRect rcImage;
		rcImage.left = nLeft;
		rcImage.top = nTop;
		rcImage.right = rcImage.left + m_poImage->GetWidth();
		rcImage.bottom = rcImage.top + m_poImage->GetHeight();
		CRXSkinService::Instance()->GetDrawMgr().DrawImage(m_poImage,rcImage,hDC);
	}

	INT32 nModel = SetBkMode(hDC,TRANSPARENT);
	HFONT hOld = (HFONT)SelectObject(hDC,CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
	CRXSkinRect rcTxt = rcItem;
	rcTxt.left += nTextLeft;
	tstring strTxt = GetText();
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
			clrOld,
			ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	}
	SelectObject(hDC,hOld);
	SetBkMode(hDC,nModel);
	::SetTextColor(hDC,clrOld);
}

BOOL CRXListButton::PtInRect(POINT pt)
{
	return GetRect().PtInRect(pt);
}

VOID CRXListButton::ReleaseControl()
{
	delete this;
}

VOID CRXListButton::Enable(const BOOL bEnabled)
{
	CRXControl::Enable(bEnabled);
	Invalidate();
}

BOOL CRXListButton::IsEnabled()
{
	return CRXControl::IsEnabled();
}

BOOL CRXListButton::GetSelected()
{
	return m_bSelected;
}

VOID CRXListButton::SetSelected(const BOOL bSelected)
{
	m_bSelected = bSelected;
}

VOID CRXListButton::SetImage(const TCHAR* pszImageFile)
{
	m_strImageFile = pszImageFile;
	
}

BOOL CRXListButton::OnClick(POINT pt)
{
	if (IsEnabled()&& IsVisible())
	{
		if (PtInRect(pt))
		{
			SetSelected(TRUE);
			if (GetParentWnd())
			{
				::SendMessage(GetParentWnd(),WM_COMMAND,GetID(),0);
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXListButton::OnLButtonDown(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (PtInRect(pt))
		{
			if (GetState()!=CONTROL_STATE_PRESSED)
			{
				SetState(CONTROL_STATE_PRESSED);
				Invalidate();
				
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXListButton::OnMouseMove(POINT pt)
{
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_MOUSEON)
		{
			SetState(CONTROL_STATE_MOUSEON);
			Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CRXListButton::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	CRXSkinResizeRect& rcSizing = GetResize();
	if (rcSizing.left == 0 &&
		rcSizing.right == 0 &&
		rcSizing.top == 0 &&
		rcSizing.bottom == 0)
	{
		return FALSE;
	}
	if (!rcSizing.IsNormalized())
	{
		return FALSE;
	}
	CRXSkinRect& rcLast =GetLast();

	INT32 nWidth = dwX - rcLast.Width();
	CRXSkinRect rcItem = GetRect();
	rcItem.left = rcItem.left + (INT32)((float)nWidth * rcSizing.left);
	rcItem.right = rcItem.right + (INT32)((float)nWidth * rcSizing.right);
	INT32 nHeight = dwY - rcLast.Height();
	rcItem.top = rcItem.top + (INT32)((float)nHeight * rcSizing.top);
	rcItem.bottom = rcItem.bottom + (INT32)((float)nHeight * rcSizing.bottom);

	return TRUE;
}

////////////  ListButton控件   /////////////////////


////////////  ListButton控件列表   /////////////////
CRXListButtonList::CRXListButtonList()
{
	m_poSelected = NULL;
	m_poMouseOn = NULL;
	m_poPressed = NULL;
}

CRXListButtonList::~CRXListButtonList()
{
	_DeleteAll();
}

BOOL CRXListButtonList::AddListButton(CRXListButton* poControl)
{
	CRXListButton* poFind = FindListButton(poControl->GetID());
	if (NULL == poFind)
	{
		m_lstListButton.push_back(poControl);
		return TRUE;
	}
	return FALSE;
}

CRXListButton* CRXListButtonList::FindListButton(const UINT32 dwID)
{
	CRXListButtonLstItr itr = m_lstListButton.begin();
	for (;itr!=m_lstListButton.end();itr++)
	{
		CRXListButton* poRadio = (*itr);
		if (poRadio && poRadio->GetID() == dwID)
		{
			return poRadio;
		}
	}
	return NULL;
}

CRXListButton* CRXListButtonList::RemoveListButton(const UINT32 dwID)
{
	CRXListButtonLstItr itr = m_lstListButton.begin();
	for (;itr!=m_lstListButton.end();itr++)
	{
		CRXListButton* poRadio = (*itr);
		if (poRadio && poRadio->GetID() == dwID)
		{
			m_lstListButton.erase(itr);
			return poRadio;
		}
	}
	return NULL;
}

VOID CRXListButtonList::Select(const UINT32 dwID)
{
	CRXListButton* poBtn = FindListButton(dwID);
	if (poBtn)
	{
		if (NULL == m_poSelected)
		{
			m_poSelected = poBtn;
			m_poSelected->SetSelected(TRUE);
			m_poSelected->Invalidate();
		}
		else if (m_poSelected!=poBtn)
		{
			m_poSelected->SetSelected(FALSE);
			m_poSelected->Invalidate();

			m_poSelected = poBtn;
			m_poSelected->SetSelected(TRUE);
			m_poSelected->Invalidate();
		}
		::SendMessage(GetParentWnd(),WM_COMMAND,m_poSelected->GetID(),0);
	}
}

VOID CRXListButtonList::_DeleteAll()
{
	CRXListButtonLstItr itr = m_lstListButton.begin();
	for (;itr!=m_lstListButton.end();itr++)
	{
		CRXControl* poControl = *itr;
		if (poControl)
		{
			poControl->ReleaseControl();
		}
	}
	m_lstListButton.clear();
}

VOID CRXListButtonList::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	CRXListButtonLstItr itr = m_lstListButton.begin();
	for (;itr!=m_lstListButton.end();itr++)
	{
		CRXControl* poControl = *itr;
		if (poControl)
		{
			poControl->OnDraw(poSurface,hDC,poControl->GetRect());
		}
	}
}

BOOL CRXListButtonList::PtInRect(POINT pt)
{
	CRXListButtonLstItr itr = m_lstListButton.begin();
	for (;itr!=m_lstListButton.end();itr++)
	{
		CRXControl* poControl = *itr;
		if (poControl && poControl->PtInRect(pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXListButtonList::OnClick(POINT pt)
{
	BOOL bClick = FALSE;
	CRXListButton* poClick = FindListByPosition(pt);
	if (NULL == poClick)
	{
		return FALSE;
	}
	if (NULL == m_poSelected)
	{
		m_poSelected = poClick;
		if (m_poSelected->IsEnabled())
		{
			m_poSelected->SetSelected(TRUE);
			m_poSelected->Invalidate();
			::SendMessage(GetParentWnd(),WM_COMMAND,m_poSelected->GetID(),0);
		}
	}
	else
	{
		if (m_poSelected!=poClick)
		{
			if (m_poSelected->IsEnabled())
			{
				m_poSelected->SetSelected(FALSE);
				m_poSelected->Invalidate();
			}


			m_poSelected = poClick;
			if (m_poSelected->IsEnabled())
			{
				m_poSelected->SetSelected(TRUE);
				m_poSelected->Invalidate();
				::SendMessage(GetParentWnd(),WM_COMMAND,m_poSelected->GetID(),0);
			}
		}
	}

	return FALSE;
}

BOOL CRXListButtonList::OnLButtonDown(POINT pt)
{
	BOOL bClick = FALSE;
	CRXListButton* poClick = FindListByPosition(pt);
	if (NULL == poClick)
	{
		if (m_poPressed)
		{
			m_poPressed->SetState(CONTROL_STATE_NORMAL);
			m_poPressed->Invalidate();
			m_poPressed = NULL;
		}
		return FALSE;
	}
	if (NULL == m_poPressed)
	{
		m_poPressed = poClick;
		if (m_poPressed->IsEnabled() && m_poPressed->IsVisible())
		{
			m_poPressed->SetState(CONTROL_STATE_PRESSED);
			m_poPressed->Invalidate();
		}
	}
	else
	{
		if (m_poPressed!=poClick)
		{
			if (m_poPressed->IsEnabled() && m_poPressed->IsVisible())
			{
				m_poPressed->SetState(CONTROL_STATE_NORMAL);
				m_poPressed->Invalidate();
			}

			m_poPressed = poClick;
			if (m_poPressed->IsEnabled() && m_poPressed->IsVisible())
			{
				m_poPressed->SetState(CONTROL_STATE_PRESSED);
				m_poPressed->Invalidate();
			}
		}
	}

	return FALSE;
}


BOOL CRXListButtonList::OnMouseMove(POINT pt)
{
	CRXListButton* poClick = FindListByPosition(pt);
	if (NULL == poClick)
	{
		if (m_poMouseOn)
		{
			m_poMouseOn->SetState(CONTROL_STATE_NORMAL);
			m_poMouseOn->Invalidate();
		}
		m_poMouseOn = NULL;
		return FALSE;
	}
	if (NULL ==m_poMouseOn)
	{
		m_poMouseOn = poClick;
		m_poMouseOn->SetState(CONTROL_STATE_MOUSEON);
		m_poMouseOn->Invalidate();
	}
	else if (poClick!=m_poMouseOn)
	{
		m_poMouseOn->SetState(CONTROL_STATE_NORMAL);
		m_poMouseOn->Invalidate();
		m_poMouseOn = poClick;
		m_poMouseOn->SetState(CONTROL_STATE_MOUSEON);
		m_poMouseOn->Invalidate();
	}
	return TRUE;
}

CRXListButton* CRXListButtonList::FindListByPosition(POINT pt)
{
	CRXListButton* poFind = NULL;
	CRXListButtonLstItr itr = m_lstListButton.begin();
	for (;itr!=m_lstListButton.end();itr++)
	{
		CRXListButton* poControl = *itr;
		if (poControl && poControl->PtInRect(pt))
		{
			poFind = poControl;
			break;
		}
	}
	return poFind;
}

VOID CRXListButtonList::Enable(const BOOL bEnabled)
{
	CRXListButton* poFind = NULL;
	CRXListButtonLstItr itr = m_lstListButton.begin();
	for (;itr!=m_lstListButton.end();itr++)
	{
		CRXListButton* poControl = *itr;
		poControl->Enable(bEnabled);
	}
}


VOID CRXListButtonList::ReleaseControl()
{
	delete this;
}

BOOL CRXListButtonList::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	BOOL bSized = FALSE;
	CRXListButtonLstItr itr = m_lstListButton.begin();
	for (;itr!=m_lstListButton.end();itr++)
	{
		CRXListButton* poControl = *itr;
		if (poControl && poControl->OnSize(dwX,dwY))
		{
			bSized = TRUE;
		}
	}
	return bSized;
}

////////////  ListButton控件列表   /////////////////


////////////  用户状态按钮控件   /////////////////

#define MOUSEON_FROM_COLOR      RGB(177,211,237)
#define MOUSEON_TO_COLOR        RGB(135,189,225)
#define BORDER_COLOR            RGB(91,157,205)
#define BK_COLOR                RGB(197,223,242)

#define ROUND_HEIGHT            3
#define ROUND_WIDTH             3

#define STATUS_OFFSET_X         1
#define STATUS_OFFSET_Y         1
#define IMAGE_OFFSET_X          4
#define ARROW_OFFSET_X          5

CRXStatusButton::CRXStatusButton()
{
	m_poImage = NULL;
	SetStatus(ID_STATUS_ONLINE,FALSE);
	m_poMenu = NULL;
}

CRXStatusButton::~CRXStatusButton()
{
	if (m_poMenu)
	{
		CRXSkinMenu* poMenu = (CRXSkinMenu*)m_poMenu;
		delete poMenu;
		m_poMenu = NULL;
	}
	CMenuItemListItr itr = m_lstMenus.begin();
	for(;itr!=m_lstMenus.end();itr++)
	{
		delete (*itr);
	}
	m_lstMenus.clear();
}

VOID CRXStatusButton::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (GetState() != CONTROL_STATE_NORMAL && GetState()!= CONTROL_STATE_DISABLE)
	{
		DrawRoundRect(hDC,BORDER_COLOR,BK_COLOR,rcItem,ROUND_WIDTH, ROUND_HEIGHT);
		CRXSkinRect rcRound  = rcItem;
		rcRound.left += STATUS_OFFSET_X+1;
		rcRound.right -= STATUS_OFFSET_X;
		rcRound.top += STATUS_OFFSET_Y+1;
		rcRound.bottom -= STATUS_OFFSET_Y;
		HRGN hRgn = ::CreateRoundRectRgn(rcRound.left,rcRound.top,rcRound.right,rcRound.bottom,ROUND_WIDTH,ROUND_HEIGHT);
		DrawGradientRgn(hDC,TRUE,hRgn,MOUSEON_FROM_COLOR,MOUSEON_TO_COLOR);
		::DeleteObject(hRgn);
	}
	if (m_poImage)
	{
		CRXSkinRect rcImage = rcItem;
		rcImage.left += IMAGE_OFFSET_X;
		rcImage.right  = rcImage.left + m_poImage->GetWidth();
		rcImage.top = rcImage.top + (rcImage.Height() - m_poImage->GetHeight())/2;
		rcImage.bottom = rcImage.top + m_poImage->GetHeight();
		m_poImage->Draw(hDC,rcImage);
	}
	COLORREF clrArrow = CRXSkinService::Instance()->GetDrawMgr().GetArrowNormal();
	INT32 nTop = rcItem.top + (rcItem.Height())/2-2;
	switch(GetState())
	{
	case CONTROL_STATE_NORMAL:
		{
			clrArrow = CRXSkinService::Instance()->GetDrawMgr().GetArrowNormal();
		}
		break;
	case CONTROL_STATE_MOUSEON:
		{
			clrArrow = CRXSkinService::Instance()->GetDrawMgr().GetArrowMouseOn();
		}
		break;
	case CONTROL_STATE_PRESSED:
		{
			nTop += 1;
			clrArrow = CRXSkinService::Instance()->GetDrawMgr().GetArrowPressed();
		}
		break;
	default:
		break;
	}

	DrawArrow(hDC,
		clrArrow,
		rcItem.right-CRXSkinService::Instance()->GetDrawMgr().GetArrowHeight() - ARROW_OFFSET_X,
		nTop,
		CRXSkinService::Instance()->GetDrawMgr().GetArrowHeight(),
		DIR_BOTTOM);

}

BOOL CRXStatusButton::PtInRect(POINT pt)
{
	return GetRect().PtInRect(pt);
}

VOID CRXStatusButton::ReleaseControl()
{
	delete this;
}

BOOL CRXStatusButton::OnClick(POINT pt)
{
	if (FALSE == IsVisible() || FALSE == IsEnabled())
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_PRESSED)
		{
			SetState(CONTROL_STATE_PRESSED);
			Invalidate();
		}
		if (NULL == m_poMenu)
		{
			m_poMenu = new CRXSkinMenu;
			((CRXSkinMenu*)m_poMenu)->Create(WND_RXSKIN_MENU_CLASS, _T(""), WS_POPUP, GetParentWnd(), NULL);
		}
		((CRXSkinMenu*)m_poMenu)->AppendMenuEx(MENU_STRING,ID_STATUS_ONLINE,_T("在线"),ID_STATUS_ONLINE);
		((CRXSkinMenu*)m_poMenu)->AppendMenuEx(MENU_STRING,ID_STATUS_OFFLINE,_T("离线"),ID_STATUS_OFFLINE);
		((CRXSkinMenu*)m_poMenu)->AppendMenuEx(MENU_STRING,ID_STATUS_AWAY,_T("离开"),ID_STATUS_AWAY);
		((CRXSkinMenu*)m_poMenu)->AppendMenuEx(MENU_STRING,ID_STATUS_BUSY,_T("忙碌"),ID_STATUS_BUSY);
		((CRXSkinMenu*)m_poMenu)->AppendMenuEx(MENU_STRING,ID_STATUS_CHAT,_T("自由聊天"),ID_STATUS_CHAT);
		((CRXSkinMenu*)m_poMenu)->AppendMenuEx(MENU_STRING,ID_STATUS_HIDE,_T("隐身"),ID_STATUS_HIDE);
		((CRXSkinMenu*)m_poMenu)->AppendMenuEx(MENU_STRING,ID_STATUS_DISTURB,_T("请勿打扰"),ID_STATUS_DISTURB);
		CMenuItemListItr itr = m_lstMenus.begin();
		for (;itr!=m_lstMenus.end();itr++)
		{
			((CRXSkinMenu*)m_poMenu)->AppendMenu((*itr)->m_enType,(*itr)->m_dwID,(*itr)->m_szName,(*itr)->m_szImagePath);
		}
		POINT ptMenu;
		ptMenu.x = GetRect().left;
		ptMenu.y = GetRect().bottom;
		ClientToScreen(GetParentWnd(),&ptMenu);
		((CRXSkinMenu*)m_poMenu)->ShowMenu(ptMenu);
		return TRUE;
	}
	return FALSE;
}

BOOL CRXStatusButton::OnLButtonDown(POINT pt)
{
	if (FALSE == IsVisible() || FALSE == IsEnabled())
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_PRESSED)
		{
			SetState(CONTROL_STATE_PRESSED);
			Invalidate();
		}	
		return TRUE;
	}
	return FALSE;
}


BOOL CRXStatusButton::OnMouseMove(POINT pt)
{
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_MOUSEON)
		{
			SetState(CONTROL_STATE_MOUSEON);
			Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}

VOID CRXStatusButton::AddMenuItem(EMenuItemType enType,const UINT32 dwID,const TCHAR* pszName,const TCHAR* pszImageFile/* =_T */)
{
	SMenuItem* poItem = new SMenuItem;
	poItem->m_enType = enType;
	poItem->m_dwID = dwID;
	_RXTStrcpy(poItem->m_szImagePath,pszImageFile);
	_RXTStrcpy(poItem->m_szName,pszName);
	m_lstMenus.push_back(poItem);
}

VOID CRXStatusButton::SetStatus(const UINT32 dwStatus,const BOOL bDraw)
{
	m_poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(dwStatus);
	
	switch(dwStatus)
	{
	case ID_STATUS_OFFLINE:
		m_enStatus = USER_STATE_OFFLINE;
		break;
	case ID_STATUS_ONLINE:
		m_enStatus = USER_STATE_ONLINE;
		break;
	case ID_STATUS_AWAY:
		m_enStatus = USER_STATE_AWAY;
		break;
	case ID_STATUS_BUSY:
		m_enStatus = USER_STATE_BUSY;
		break;
	case ID_STATUS_CHAT:
		m_enStatus = USER_STATE_CHAT;
		break;
	case ID_STATUS_HIDE:
		m_enStatus = USER_STATE_HIDE;
		break;
	case ID_STATUS_DISTURB:
		m_enStatus = USER_STATE_DISTURB;
		break;
	default: 
		break;
	}
	if (bDraw)
	{
		Invalidate();
	}
}

EUserState CRXStatusButton::GetStatus()
{
	return m_enStatus;
}


////////////  用户状态按钮控件   /////////////////////



////////////  Checkbox控件   /////////////////
#define CHECK_OFFSET   7
CRXCheckButton::CRXCheckButton()
{
	m_bChecked = FALSE;
}

CRXCheckButton::~CRXCheckButton()
{
}

VOID CRXCheckButton::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (FALSE == IsVisible())
	{
		return;
	}
	SetRect(rcItem);
	INT32 nTextLeft = 0;
	if (m_bChecked)
	{
		IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_CHECKBOX_CHECKED);
		if (poImage)
		{
			INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
			INT32 nLeft = rcItem.left + CHECK_OFFSET;
			nTextLeft += CHECK_OFFSET*2;
			nTextLeft += poImage->GetWidth(); 
			poImage->Stretch(hDC,nLeft,nTop,poImage->GetWidth(),poImage->GetHeight());
		}
	}
	else
	{
		if (GetState() == CONTROL_STATE_NORMAL)
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_CHECKBOX_NORMAL);
			if (poImage)
			{
				INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
				INT32 nLeft = rcItem.left + CHECK_OFFSET;
				nTextLeft += CHECK_OFFSET*2;
				nTextLeft += poImage->GetWidth(); 
				poImage->Stretch(hDC,nLeft,nTop,poImage->GetWidth(),poImage->GetHeight());
			}
		}
		else
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_BUTTON_CHECKBOX_HOT);
			if (poImage)
			{
				INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
				INT32 nLeft = rcItem.left + CHECK_OFFSET;
				nTextLeft += CHECK_OFFSET*2;
				nTextLeft += poImage->GetWidth(); 
				poImage->Stretch(hDC,nLeft,nTop,poImage->GetWidth(),poImage->GetHeight());
			}
		}
	}
	INT32 nModel = ::SetBkMode(hDC,TRANSPARENT);
	HFONT hOld = (HFONT)::SelectObject(hDC,CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
	COLORREF oldTxt = ::SetTextColor(hDC, CRXSkinService::Instance()->GetDrawMgr().GetButtonListNormalText());
	CRXSkinRect rcTxt = rcItem;
	rcTxt.left += nTextLeft;
	tstring strTxt = GetText();
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonListNormalText(),
			ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	}
	::SelectObject(hDC,hOld);
	SetBkMode(hDC,nModel);
	::SetTextColor(hDC, oldTxt);
}

BOOL CRXCheckButton::PtInRect(POINT pt)
{
	return GetRect().PtInRect(pt);
}

VOID CRXCheckButton::ReleaseControl()
{
	delete this;
}

BOOL CRXCheckButton::GetChecked()
{
	return m_bChecked;
		
}

VOID CRXCheckButton::SetChecked(const BOOL bChecked)
{
	m_bChecked = bChecked;
	Invalidate();
}

BOOL CRXCheckButton::OnClick(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		if (PtInRect(pt))
		{
			SetChecked(!GetChecked());
			Invalidate();
			if (GetParentWnd())
			{
				::SendMessage(GetParentWnd(),WM_COMMAND,GetID(),0);
			}
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CRXCheckButton::OnLButtonDown(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (PtInRect(pt))
		{
			SetState(CONTROL_STATE_PRESSED);
			Invalidate();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXCheckButton::OnMouseMove(POINT pt)
{
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_MOUSEON)
		{
			SetState(CONTROL_STATE_MOUSEON);
			Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}



////////////  Checkbox控件   /////////////////////


////////////  HyperLink控件   /////////////////
#define HYPERLINK_OFFSET   3
CRXHyperLink::CRXHyperLink()
{
	m_poImage = NULL;
	memset(m_szImageFile,0,sizeof(TCHAR)*256);
}

CRXHyperLink::~CRXHyperLink()
{
	if (m_poImage)
	{
		m_poImage->Release();
	}
	m_poImage = NULL;
}

VOID CRXHyperLink::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible()==FALSE)
	{
		return;
	}
	SetRect(rcItem);
	INT32 nTextLeft = 0;
	if (m_poImage == NULL)
	{
		m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenDirectImage(m_szImageFile);
	}
	if (m_poImage)
	{
		INT32 nTop = rcItem.top + (rcItem.Height() - m_poImage->GetHeight())/2;
		INT32 nLeft = rcItem.left + HYPERLINK_OFFSET;
		nTextLeft += HYPERLINK_OFFSET*2;
		nTextLeft += m_poImage->GetWidth(); 
		m_poImage->Stretch(hDC,nLeft,nTop,m_poImage->GetWidth(),m_poImage->GetHeight());
	}

	INT32 nModel = SetBkMode(hDC,TRANSPARENT);
	CRXSkinRect rcTxt = rcItem;
	rcTxt.left += nTextLeft;
	COLORREF clrTxt;
	LOGFONT oFont;
	memset(&oFont, 0, sizeof(LOGFONT));
	_RXTStrncpy(oFont.lfFaceName, _T("宋体"), 32);
	switch(GetState())
	{
	case CONTROL_STATE_PRESSED:
	case CONTROL_STATE_MOUSEON:
		{
			oFont.lfHeight = 12;	
			oFont.lfItalic = 1;
			clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetHyperLinkHotText();
		}
		break;
	default:
		{
			oFont.lfHeight = 12;
			clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetHyperLinkText();
		}
		break;
	}
	tstring strTxt = GetText();
	CRXSkinFont* poFont = CRXSkinFontMgr::Instance()->FindFontEx(oFont);
	if (poFont==NULL)
	{
		poFont = new CRXSkinFont;
		if (false == poFont->InitFont(oFont))
		{
			delete poFont;
			poFont = NULL;
		}
		else
		{
			CRXSkinFontMgr::Instance()->AddFontEx(oFont, poFont);
		}
	}
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas&&poFont)
	{
		CRXSkinFontMgr::Instance()->DrawTextByFont(poCanvas,
			strTxt, rcTxt, clrTxt, poFont, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	}
	::SetBkMode(hDC,nModel);

}

BOOL CRXHyperLink::PtInRect(POINT pt)
{
	return GetRect().PtInRect(pt);
}

VOID CRXHyperLink::ReleaseControl()
{
	delete this;
}

BOOL CRXHyperLink::OnClick(POINT pt)
{
	if (FALSE == IsEnabled() || IsVisible() == FALSE)
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		if (GetParentWnd())
		{
			::SendMessage(GetParentWnd(),WM_COMMAND,GetID(),0);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CRXHyperLink::OnLButtonDown(POINT pt)
{
	if (PtInRect(pt))
	{
		SetState(CONTROL_STATE_PRESSED);
		Invalidate();
		return TRUE;
	}
	return FALSE;
}


BOOL CRXHyperLink::OnMouseMove(POINT pt)
{
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_MOUSEON)
		{
			SetState(CONTROL_STATE_MOUSEON);
			Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}

VOID CRXHyperLink::SetImageFile(const TCHAR* pszImageFile)
{
	if (m_poImage)
	{
		memset(m_szImageFile,0,sizeof(TCHAR)*256);
		_tcsncpy(m_szImageFile,pszImageFile,256);
	}
}
////////////  HyperLink控件   /////////////////////

////////////  Button控件   /////////////////
#define BUTTON_OFFSET   3
#define BUTTON_MOUSEON_FROM_COLOR  RGB(170,190,250)
#define BUTTON_MOUSEON_TO_COLOR    RGB(230,220,255)

CRXButton::CRXButton()
{
	m_poImage = NULL;
	memset(m_szImage,0,sizeof(TCHAR)*256);
	m_clrBorder = CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor();

	m_clrMouseOnFrom = CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonMouseOnFromColor();
	m_clrMouseOnTo = CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonMouseOnToColor();
	m_clrMouseOnMiddle = CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonMouseOnMiddleColor();

	m_clrPressedFrom = CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonPressedFromColor();
	m_clrPressedTo = CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonPressedToColor();
	m_clrPressedMiddle = CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonPressedMiddleColor();

	m_bCheckBoxBtn = false;
	m_bSelected = false;
}

CRXButton::~CRXButton()
{
	if (m_poImage)
	{
		m_poImage->Release();
	}
	m_poImage = NULL;
}

void  CRXButton::SetSelect(const bool& bSelected) 
{
	m_bSelected = bSelected; 
	Invalidate();
}

VOID CRXButton::SetImage(const TCHAR* pszImageFile)
{
	if (m_poImage)
	{
		memset(m_szImage,0,sizeof(TCHAR)*256);
		_tcsncpy(m_szImage,pszImageFile,256);
	}
}

TCHAR* CRXButton::GetImageFile()
{
	return m_szImage;
}

void  CRXButton::_DrawCheckBoxButton(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (m_bSelected)
	{
		INT32 nOffSetY = 0;
		if (IsEnabled())
		{
			nOffSetY = 1;
			DrawRoundRect(hDC,
				m_clrBorder,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonNormalFromColor(),
				GetRect(),
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy);
			HRGN hRgn = ::CreateRoundRectRgn(m_rcItem.left + 1,
				m_rcItem.top + 1,
				m_rcItem.right - 1,
				m_rcItem.top + m_rcItem.Height() / 2,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy);
			CRXSkinRect rcTop = m_rcItem;
			rcTop.left += 2;
			rcTop.right -= 2;
			rcTop.top += 2;
			rcTop.bottom = rcTop.top + m_rcItem.Height() / 2;
			CreateRoundRectRgn(hRgn,
				rcTop,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy,
				TRUE, FALSE, TRUE, FALSE);
			::DrawGradientRgn(hDC, TRUE, hRgn, m_clrPressedFrom,
				m_clrPressedMiddle);
			CRXSkinRect rcBottom = m_rcItem;
			rcBottom.left += 2;
			rcBottom.right -= 2;
			rcBottom.top = m_rcItem.top + m_rcItem.Height() / 2;
			rcBottom.bottom = m_rcItem.bottom - 2;
			CreateRoundRectRgn(hRgn,
				rcBottom,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy,
				FALSE, TRUE, FALSE, TRUE);
			::DrawGradientRgn(hDC, TRUE, hRgn, m_clrPressedMiddle,
				m_clrPressedTo);
			DeleteObject(hRgn);

			CRXSkinRect rcTxt = m_rcItem;
			SIZE szTxt;
			tstring strTxt = GetText();
			CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, strTxt, szTxt);

			INT32 nTotalWidth = szTxt.cx;
			if (nTotalWidth > 0)
			{
				nTotalWidth += BUTTON_OFFSET;
			}
			INT32 nTextLeft = 0;
			if (m_poImage)
			{
				nTotalWidth += m_poImage->GetWidth();
				INT32 nTop = m_rcItem.top + (m_rcItem.Height() - m_poImage->GetHeight()) / 2;
				INT32 nLeft = m_rcItem.left + (m_rcItem.Width() - nTotalWidth) / 2;
				nTextLeft = nLeft + m_poImage->GetWidth() + BUTTON_OFFSET;
				m_poImage->Draw(hDC, nLeft, nTop + nOffSetY, m_poImage->GetWidth(), m_poImage->GetHeight());
			}
			else
			{
				nTextLeft = m_rcItem.left + (m_rcItem.Width() - szTxt.cx) / 2;
			}
			rcTxt.left = nTextLeft;
			SkCanvas* poCanvas = poSurface->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
					CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalTxt(),
					ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			}
		}
		else
		{
			CRXSkinRect rcTxt = m_rcItem;
			SIZE szTxt;
			tstring strTxt = GetText();
			CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, strTxt, szTxt);
			INT32 nTotalWidth = szTxt.cx;
			if (nTotalWidth > 0)
			{
				nTotalWidth += BUTTON_OFFSET;
			}
			INT32 nTextLeft = 0;
			if (m_poImage)
			{
				nTotalWidth += m_poImage->GetWidth();
				INT32 nTop = m_rcItem.top + (m_rcItem.Height() - m_poImage->GetHeight()) / 2;
				INT32 nLeft = m_rcItem.left + (m_rcItem.Width() - nTotalWidth) / 2;
				nTextLeft = nLeft + m_poImage->GetWidth() + BUTTON_OFFSET;
				CRXSkinRect rcImage;
				rcImage.left = nLeft;
				rcImage.top = nTop + nOffSetY;
				rcImage.right = rcImage.left + m_poImage->GetWidth();
				rcImage.bottom = rcImage.top + m_poImage->GetHeight();
				CRXSkinService::Instance()->GetDrawMgr().DrawImageGray(m_poImage, rcImage, hDC);
			}
			else
			{
				nTextLeft = m_rcItem.left + (m_rcItem.Width() - szTxt.cx) / 2;
			}
			rcTxt.left = nTextLeft;
			SkCanvas* poCanvas = poSurface->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
					GetSysColor(COLOR_GRAYTEXT),
					ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			}
		}
	}
	else
	{
		_DrawNormalButton(poSurface,hDC, m_rcItem);
	}
}

void CRXButton::_DrawNormalButton(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	INT32 nOffSetY = 0;
	if (IsEnabled())
	{
		COLORREF clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalTxt();
		switch (GetState())
		{
		case CONTROL_STATE_MOUSEON:
		{
			clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnTxt();
			DrawRoundRect(hDC,
				m_clrBorder,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonNormalFromColor(),
				GetRect(),
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy);
			HRGN hRgn = ::CreateRoundRectRgn(rcItem.left + 1,
				rcItem.top + 1,
				rcItem.right - 1,
				rcItem.top + rcItem.Height() / 2,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy);
			CRXSkinRect rcTop = rcItem;
			rcTop.left += 2;
			rcTop.right -= 2;
			rcTop.top += 2;
			rcTop.bottom = rcTop.top + rcItem.Height() / 2;
			CreateRoundRectRgn(hRgn,
				rcTop,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy,
				TRUE, FALSE, TRUE, FALSE);
			::DrawGradientRgn(hDC,
				TRUE,
				hRgn,
				m_clrMouseOnFrom,
				m_clrMouseOnMiddle);
			CRXSkinRect rcBottom = rcItem;
			rcBottom.left += 2;
			rcBottom.right -= 2;
			rcBottom.top = rcItem.top + rcItem.Height() / 2;
			rcBottom.bottom = rcItem.bottom - 2;
			CreateRoundRectRgn(hRgn,
				rcBottom,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy,
				FALSE, TRUE, FALSE, TRUE);
			::DrawGradientRgn(hDC,
				TRUE,
				hRgn,
				m_clrMouseOnMiddle,
				m_clrMouseOnTo);
			DeleteObject(hRgn);
		}
		break;
		case CONTROL_STATE_PRESSED:
		{
			nOffSetY = 1;
			clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedTxt();
			DrawRoundRect(hDC,
				m_clrBorder,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonNormalFromColor(),
				GetRect(),
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy);
			HRGN hRgn = ::CreateRoundRectRgn(rcItem.left + 1,
				rcItem.top + 1,
				rcItem.right - 1,
				rcItem.top + rcItem.Height() / 2,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy);
			CRXSkinRect rcTop = rcItem;
			rcTop.left += 2;
			rcTop.right -= 2;
			rcTop.top += 2;
			rcTop.bottom = rcTop.top + rcItem.Height() / 2;
			CreateRoundRectRgn(hRgn,
				rcTop,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy,
				TRUE, FALSE, TRUE, FALSE);
			::DrawGradientRgn(hDC, TRUE, hRgn, m_clrPressedFrom,
				m_clrPressedMiddle);
			CRXSkinRect rcBottom = rcItem;
			rcBottom.left += 2;
			rcBottom.right -= 2;
			rcBottom.top = rcItem.top + rcItem.Height() / 2;
			rcBottom.bottom = rcItem.bottom - 2;
			CreateRoundRectRgn(hRgn,
				rcBottom,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonRoundSize().cy,
				FALSE, TRUE, FALSE, TRUE);
			::DrawGradientRgn(hDC, TRUE, hRgn, m_clrPressedMiddle,
				m_clrPressedTo);
			DeleteObject(hRgn);
		}
		break;
		default:
			break;
		}
		CRXSkinRect rcTxt = rcItem;
		SIZE szTxt;
		tstring strTxt = GetText();
		CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, strTxt, szTxt);

		INT32 nTotalWidth = szTxt.cx;
		if (nTotalWidth > 0)
		{
			nTotalWidth += BUTTON_OFFSET;
		}
		INT32 nTextLeft = 0;
		if (m_poImage == NULL)
		{
			m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenDirectImage(m_szImage);
		}
		if (m_poImage)
		{
			nTotalWidth += m_poImage->GetWidth();
			INT32 nTop = rcItem.top + (rcItem.Height() - m_poImage->GetHeight()) / 2;
			INT32 nLeft = rcItem.left + (rcItem.Width() - nTotalWidth) / 2;
			nTextLeft = nLeft + m_poImage->GetWidth() + BUTTON_OFFSET;
			m_poImage->Draw(hDC, nLeft, nTop + nOffSetY, m_poImage->GetWidth(), m_poImage->GetHeight());
		}
		else
		{
			nTextLeft = rcItem.left + (rcItem.Width() - szTxt.cx) / 2;
		}
		rcTxt.left = nTextLeft;
		SkCanvas * poCanvas = poSurface->getCanvas();
		if (poCanvas)
		{
			CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
				clrTxt,
				ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
		}
	}
	else
	{
		CRXSkinRect rcTxt = rcItem;
		SIZE szTxt;
		tstring strTxt = GetText();
		CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, strTxt, szTxt);

		INT32 nTotalWidth = szTxt.cx;
		if (nTotalWidth > 0)
		{
			nTotalWidth += BUTTON_OFFSET;
		}
		INT32 nTextLeft = 0;
		if (m_poImage)
		{
			nTotalWidth += m_poImage->GetWidth();
			INT32 nTop = rcItem.top + (rcItem.Height() - m_poImage->GetHeight()) / 2;
			INT32 nLeft = rcItem.left + (rcItem.Width() - nTotalWidth) / 2;
			nTextLeft = nLeft + m_poImage->GetWidth() + BUTTON_OFFSET;
			CRXSkinRect rcImage;
			rcImage.left = nLeft;
			rcImage.top = nTop + nOffSetY;
			rcImage.right = rcImage.left + m_poImage->GetWidth();
			rcImage.bottom = rcImage.top + m_poImage->GetHeight();
			CRXSkinService::Instance()->GetDrawMgr().DrawImageGray(m_poImage, rcImage, hDC);
		}
		else
		{
			nTextLeft = rcItem.left + (rcItem.Width() - szTxt.cx) / 2;
		}
		rcTxt.left = nTextLeft;
		SkCanvas* poCanvas = poSurface->getCanvas();
		if (poCanvas)
		{
			CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
				GetSysColor(COLOR_GRAYTEXT),
				ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
		}
	
	}
}

VOID CRXButton::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (IsVisible())
	{
		if (m_bCheckBoxBtn)
		{
			_DrawCheckBoxButton(poSurface,hDC,rcItem);
			return;
		}
		_DrawNormalButton(poSurface, hDC, rcItem);
	}

}

BOOL CRXButton::PtInRect(POINT pt)
{
	if (FALSE == IsEnabled())
	{
		return FALSE;
	}
	return GetRect().PtInRect(pt);
}

VOID CRXButton::ReleaseControl()
{
	delete this;
}

BOOL CRXButton::OnClick(POINT pt)
{
	if (FALSE == IsEnabled() || IsVisible() == FALSE)
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		if (GetParentWnd())
		{
			::SendMessage(GetParentWnd(),WM_COMMAND,GetID(),0);
		}
		if (m_bCheckBoxBtn)
		{
			m_bSelected = !m_bSelected;
			Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CRXButton::OnLButtonDown(POINT pt)
{
	if (FALSE == IsEnabled() || IsVisible() == FALSE)
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		SetState(CONTROL_STATE_PRESSED);
		Invalidate();
		return TRUE;
	}
	return FALSE;
}

BOOL CRXButton::OnMouseMove(POINT pt)
{
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_MOUSEON)
		{
			SetState(CONTROL_STATE_MOUSEON);
			Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}

COLORREF CRXButton::GetBorderColor()
{
	return m_clrBorder;
}

VOID CRXButton::SetBorderColor(COLORREF clrBorderColor)
{
	m_clrBorder = clrBorderColor;
}

COLORREF CRXButton::GetMouseOnFromColor()
{
	return m_clrMouseOnFrom;
}

VOID CRXButton::SetMouseOnFromColor(COLORREF clrMouseOnFromColor)
{
	m_clrMouseOnFrom = clrMouseOnFromColor;
}

COLORREF CRXButton::GetMouseOnMiddleColor()
{
	return m_clrMouseOnMiddle;
}

VOID CRXButton::SetMouseOnMiddleColor(COLORREF clrMouseOnMiddleColor)
{
	m_clrMouseOnMiddle = clrMouseOnMiddleColor;
}

COLORREF CRXButton::GetMouseOnToColor()
{
	return m_clrMouseOnTo;
}

VOID CRXButton::SetMouseOnToColor(COLORREF clrNormalToColor)
{
	m_clrMouseOnTo = clrNormalToColor;
}

COLORREF CRXButton::GetPressedFromColor()
{
	return m_clrPressedFrom;
}

VOID CRXButton::SetPressedFromColor(COLORREF clrPressedFromColor)
{
	m_clrPressedFrom = clrPressedFromColor;
}

COLORREF CRXButton::GetPressedMiddleColor()
{
	return m_clrPressedMiddle;
}

VOID CRXButton::SetPressedMiddleColor(COLORREF clrPressedMiddleColor)
{
	m_clrPressedMiddle = clrPressedMiddleColor;
}

COLORREF CRXButton::GetPressedToColor()
{
	return m_clrPressedTo;
}

VOID CRXButton::SetPressedToColor(COLORREF clrPressedToColor)
{
	m_clrPressedTo = clrPressedToColor;
}

////////////  HyperLink控件   /////////////////////



////////////  MenuButton控件   /////////////////
#define IMAGE_BUTTON_OFFSET   3

CRXImageButton::CRXImageButton()
{
	m_poImageNormal = NULL;
	m_poImageHot = NULL;
	m_poImageDown = NULL;
	m_clrNormalBk = CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnFromColor();
	m_hFont = NULL;
	m_clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalTxt();
	m_bAutoRecycle = true;
}

CRXImageButton::~CRXImageButton()
{
	if (m_bAutoRecycle)
	{
		if (m_poImageDown)
		{
			m_poImageDown->Release();
		}
		m_poImageDown = NULL;
		if (m_poImageNormal)
		{
			m_poImageNormal->Release();
		}
		m_poImageNormal = NULL;

		if (m_poImageHot)
		{
			m_poImageHot->Release();
		}
		m_poImageHot = NULL;
	}

}

VOID CRXImageButton::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (IsVisible() == FALSE)
	{
		return;
	}
	if (m_poImageDown==NULL)
	{
		m_poImageDown = CRXSkinService::Instance()->GetImageMgr().OpenDirectImage(m_strDownImage.c_str());
	}
	if (m_poImageHot == NULL)
	{
		m_poImageHot = CRXSkinService::Instance()->GetImageMgr().OpenDirectImage(m_strHotImage.c_str());
	}
	if (m_poImageNormal == NULL)
	{
		m_poImageNormal = CRXSkinService::Instance()->GetImageMgr().OpenDirectImage(m_strNormalImage.c_str());
	}
	tstring strTxt = GetText();
	if (strTxt.size()>0)
	{
		IRXImage* poImage = NULL;
		switch (GetState())
		{
		case CONTROL_STATE_NORMAL:
		{
			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonBorderColor(),
				m_clrNormalBk,
				GetRect(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy);
			poImage = m_poImageNormal;
		}
		break;
		case CONTROL_STATE_MOUSEON:
		{
			poImage = m_poImageHot;
			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonBorderColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnFromColor(),
				GetRect(),
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
			::DrawGradientRgn(hDC, TRUE, hRgn, CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnFromColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnMiddleColor());
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
			::DrawGradientRgn(hDC, TRUE, hRgn, CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnMiddleColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnToColor());
			DeleteObject(hRgn);
		}
		break;
		case CONTROL_STATE_PRESSED:
		{
			poImage = m_poImageDown;

			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonBorderColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedFromColor(),
				GetRect(),
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
				CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedMiddleColor());
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
				CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedToColor());
			DeleteObject(hRgn);
		}
		break;
		default:
		{
			poImage = m_poImageNormal;
		}
		break;
		}
		if (poImage)
		{
			INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight()) / 2;
			INT32 nLeft = rcItem.left + 5;
			poImage->Draw(hDC, nLeft, nTop, poImage->GetWidth(), poImage->GetHeight());

			CRXSkinRect rcTxt = rcItem;
			rcTxt.left = nLeft + poImage->GetWidth() + 5;
			SIZE szTxt;
			tstring strTxt = GetText();
			CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON, strTxt, szTxt);

			INT32 nTotalWidth = szTxt.cx;
			if (nTotalWidth > 0)
			{
				nTotalWidth += BUTTON_OFFSET;
			}
			INT32 nTextLeft = 0;
			SkCanvas* poCanvas = poSurface->getCanvas();
			if (poCanvas)
			{
				CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
					m_clrTxt,
					ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			}
		}
	}
	else
	{
		IRXImage* poImage = NULL;
		switch (GetState())
		{
		case CONTROL_STATE_MOUSEON:
		{
			poImage = m_poImageHot;
			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonBorderColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnFromColor(),
				GetRect(),
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
			::DrawGradientRgn(hDC, TRUE, hRgn, CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnFromColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnMiddleColor());
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
			::DrawGradientRgn(hDC, TRUE, hRgn, CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnMiddleColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnToColor());
			DeleteObject(hRgn);
		}
		break;
		case CONTROL_STATE_PRESSED:
		{
			poImage = m_poImageDown;

			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonBorderColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedFromColor(),
				GetRect(),
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
				CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedMiddleColor());
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
				CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedToColor());
			DeleteObject(hRgn);
		}
		break;
		default:
		{
			poImage = m_poImageNormal;
		}
		break;
		}
		if (poImage)
		{
			INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight()) / 2;
			INT32 nLeft = rcItem.left + (rcItem.Width() - poImage->GetWidth()) / 2;
			poImage->Draw(hDC, nLeft, nTop, poImage->GetWidth(), poImage->GetHeight());
		}
	}

}

BOOL CRXImageButton::PtInRect(POINT pt)
{
	if (!IsVisible()||!IsEnabled())
	{
		return FALSE;
	}
	return GetRect().PtInRect(pt);
}

VOID CRXImageButton::ReleaseControl()
{
	delete this;
}

BOOL CRXImageButton::OnClick(POINT pt)
{
	if (!IsVisible()||!IsEnabled())
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		if (GetParentWnd())
		{
			::SendMessage(GetParentWnd(),WM_COMMAND,GetID(),0);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CRXImageButton::OnLButtonDown(POINT pt)
{
	if (!IsVisible()||!IsEnabled())
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		SetState(CONTROL_STATE_PRESSED);
		Invalidate();
		return TRUE;
	}
	return FALSE;
}

BOOL CRXImageButton::OnMouseMove(POINT pt)
{
	if (!IsVisible()||!IsEnabled())
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_MOUSEON)
		{
			SetState(CONTROL_STATE_MOUSEON);
			Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}

VOID CRXImageButton::SetDownImage(const TCHAR* pszImageFile)
{
	m_strDownImage = pszImageFile;
}

VOID CRXImageButton::ResetDownImage(const TCHAR* pszImageFile)
{
	m_strDownImage = pszImageFile;
	if (m_poImageDown)
	{
		m_poImageDown->Release();
		m_poImageDown = NULL;
	}
}

void CRXImageButton::SetNormalBK(COLORREF clr)
{
	m_clrNormalBk = clr;
}

COLORREF CRXImageButton::GetNormalBK()
{
	return m_clrNormalBk;
}

VOID CRXImageButton::SetHotImage(const TCHAR* pszImageFile)
{
	m_strHotImage = pszImageFile;
}

VOID CRXImageButton::ResetHotImage(const TCHAR* pszImageFile)
{
	m_strHotImage = pszImageFile;
	if (m_poImageHot)
	{
		m_poImageHot->Release();
		m_poImageHot = NULL;
	}
}


VOID CRXImageButton::SetNormalImage(const TCHAR* pszImageFile)
{	
	m_strNormalImage = pszImageFile;
}

VOID CRXImageButton::ResetNormalImage(const TCHAR* pszImageFile)
{
	m_strNormalImage = pszImageFile;
	if (m_poImageNormal)
	{
		m_poImageNormal->Release();
		m_poImageNormal = NULL;
	}

}
////////////  MenuButton控件   /////////////////////

////////////  Static控件   /////////////////
CRXSkinStatic::CRXSkinStatic()
{
	m_clrText = RGB(0,0,0);
	m_bCenter = false;
	m_bIsSingleLine = false;
	m_poFont =  CRXSkinFontMgr::Instance()->FindFont(ID_FONT_COMMON);
}

CRXSkinStatic::~CRXSkinStatic()
{
}

void CRXSkinStatic::SetFontID(const uint32_t& dwFontID)
{
	m_poFont = CRXSkinFontMgr::Instance()->FindFont(dwFontID);

}

void CRXSkinStatic::SetFont(const LOGFONT& oFont)
{
	m_poFont = CRXSkinFontMgr::Instance()->FindFontEx(oFont);
	if (NULL==m_poFont)
	{
		m_poFont = new CRXSkinFont;
		if (false == m_poFont->InitFont(oFont))
		{
			delete m_poFont;
		}
		else
		{
			CRXSkinFontMgr::Instance()->AddFontEx(oFont, m_poFont);
		}
	}
}

VOID CRXSkinStatic::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (!IsVisible())
	{
		return;
	}
	CRXSkinRect rcTxt = rcItem;
	COLORREF clrOld = m_clrText;
	if (!IsEnabled())
	{
		m_clrText = CRXSkinService::Instance()->GetDrawMgr().GetDisableText();
		clrOld = ::SetTextColor(hDC,CRXSkinService::Instance()->GetDrawMgr().GetDisableText());
	}
	else
	{
		clrOld = ::SetTextColor(hDC, m_clrText);
	}
	UINT32 dwStyle = DT_EDITCONTROL | DT_WORDBREAK | DT_END_ELLIPSIS;
	if (m_bCenter)
	{
		dwStyle |= DT_CENTER;
	}
	else
	{
		dwStyle |= DT_LEFT;
	}
	if (m_bIsSingleLine)
	{
		dwStyle |= DT_SINGLELINE;
	}
	tstring strTxt = GetText();
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawTextByFont(poCanvas, strTxt.c_str(), rcTxt,
			m_clrText,m_poFont, dwStyle);
	}
}


BOOL CRXSkinStatic::PtInRect(POINT pt)
{
	return FALSE;
}

VOID CRXSkinStatic::ReleaseControl()
{
	delete this;
}

BOOL CRXSkinStatic::OnClick(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinStatic::OnLButtonDown(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinStatic::OnMouseMove(POINT pt)
{
	return FALSE;
}


////////////  标题栏控件   /////////////////
CRXTitle::CRXTitle()
{
	m_poImage = NULL;
}

CRXTitle::~CRXTitle()
{
	if (m_poImage)
		m_poImage->Release();
	m_poImage = NULL;
}
#define TITLE_OFFSET   3
VOID CRXTitle::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	CRXSkinRect rcTxt = rcItem;
	if (NULL == m_poImage)
	{
		m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenDirectImage(m_strImage.c_str());
	}
	if (m_poImage)
	{
		INT32 nTop = rcItem.top + (rcItem.Height() - m_poImage->GetHeight())/2;
		INT32 nLeft = rcItem.left;
		m_poImage->Stretch(hDC,nLeft,nTop,m_poImage->GetWidth(),m_poImage->GetHeight());
		rcTxt.left += m_poImage->GetWidth();
		rcTxt.left += TITLE_OFFSET;
	}
 	SkCanvas* poCanvas = poSurface->getCanvas();
 	if (poCanvas)
 	{
 
 		CRXSkinFontMgr::Instance()->DrawText(poCanvas, 			
 			GetText(),
			rcTxt,
			CRXSkinService::Instance()->GetDrawMgr().GetTitleColor(),
 			ID_FONT_TITLE,
 			DT_VCENTER | DT_SINGLELINE | DT_LEFT | DT_WORDBREAK | DT_END_ELLIPSIS);
 	}
}

BOOL CRXTitle::PtInRect(POINT pt)
{
	return FALSE;
}

VOID CRXTitle::ReleaseControl()
{
	delete this;
}

BOOL CRXTitle::OnClick(POINT pt)
{
	return FALSE;
}


BOOL CRXTitle::OnLButtonDown(POINT pt)
{
	return FALSE;
}

BOOL CRXTitle::OnMouseMove(POINT pt)
{
	return FALSE;
}

VOID CRXTitle::SetImage(const TCHAR* pszImageFile)
{
	m_strImage = pszImageFile;
}

////////////  MenuButton控件   /////////////////////



////////////  Button控件   /////////////////
#define MENU_BUTTON_OFFSET   3
#define MENU_BUTTON_MOUSEON_FROM_COLOR  RGB(170,190,250)
#define MENU_BUTTON_MOUSEON_TO_COLOR    RGB(230,220,255)

CRXMenuButton::CRXMenuButton()
{
}

CRXMenuButton::~CRXMenuButton()
{
}

VOID CRXMenuButton::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	switch(GetState())
	{
	case CONTROL_STATE_MOUSEON:
		{
			HRGN hRgn = ::CreateRoundRectRgn(rcItem.left,rcItem.top,rcItem.right,rcItem.bottom,ROUND_WIDTH,ROUND_HEIGHT);
			::DrawGradientRgn(hDC,
				TRUE,
				hRgn,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonMouseOnFromColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonMouseOnToColor());
			DeleteObject(hRgn);
		}
		break;
	case CONTROL_STATE_PRESSED:
		{
			HRGN hRgn = ::CreateRoundRectRgn(rcItem.left,rcItem.top,rcItem.right,rcItem.bottom,ROUND_WIDTH,ROUND_HEIGHT);
			::DrawGradientRgn(hDC,
				TRUE,
				hRgn,
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonPressedFromColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonPressedToColor());
			DeleteObject(hRgn);
		}
		break;
	default:
		break;
	}
	INT32 nTextLeft = 0;
	UINT32 dwID = GetID();
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(dwID);
	if (poImage)
	{
		INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
		INT32 nLeft = rcItem.left + BUTTON_OFFSET;
		nTextLeft += BUTTON_OFFSET*2;
		nTextLeft += poImage->GetWidth(); 
		poImage->Stretch(hDC,nLeft,nTop,poImage->GetWidth(),poImage->GetHeight());
	}
	INT32 nModel = SetBkMode(hDC,TRANSPARENT);
	CRXSkinRect rcTxt = rcItem;
	rcTxt.left += nTextLeft;
	HFONT hOldFont = (HFONT)::SelectObject(hDC,CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
	tstring strTxt = GetText();
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonListNormalText(),
			ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	}
	SelectObject(hDC,hOldFont);
	SetBkMode(hDC,nModel);

}

BOOL CRXMenuButton::PtInRect(POINT pt)
{
	return GetRect().PtInRect(pt);
}

VOID CRXMenuButton::ReleaseControl()
{
	delete this;
}

BOOL CRXMenuButton::OnClick(POINT pt)
{
	if (FALSE == IsVisible() || IsEnabled() == FALSE)
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		if (GetParentWnd())
		{
			::SendMessage(GetParentWnd(),WM_COMMAND,GetID(),0);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CRXMenuButton::OnLButtonDown(POINT pt)
{
	if (FALSE == IsVisible() || IsEnabled() == FALSE)
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		SetState(CONTROL_STATE_PRESSED);
		Invalidate();
		
		return TRUE;
	}
	return FALSE;
}

BOOL CRXMenuButton::OnMouseMove(POINT pt)
{
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_MOUSEON)
		{
			SetState(CONTROL_STATE_MOUSEON);
			Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}

////////////  MenuButton控件   /////////////////////



////////////  Checkbox Image控件   /////////////////
#define CHECK_IMAGE_OFFSET   3
CRXCheckImageButton::CRXCheckImageButton()
{
	m_bChecked = FALSE;
}

CRXCheckImageButton::~CRXCheckImageButton()
{
}

VOID CRXCheckImageButton::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	INT32 nTextLeft = 0;
	if (m_bChecked)
	{
		COLORREF clrFrom = GetSysColor(CTLCOLOR_DLG);
		INT32 nRed = GetRValue(clrFrom)+50;
		INT32 nGreen = GetGValue(clrFrom)+50;
		INT32 nBlue = GetBValue(clrFrom)+50;
		if (nRed>255)
		{
			nRed = 255;
		}
		if (nGreen>255)
		{
			nGreen = 255;
		}
		if (nBlue>255)
		{
			nBlue = 255;
		}
		clrFrom = RGB(nRed,nGreen,nBlue);
		::DrawRoundRect(hDC,
			GetSysColor(CTLCOLOR_DLG),
			clrFrom,
			rcItem,
			ROUND_WIDTH,
			ROUND_HEIGHT);
		IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(GetID());
		if (poImage)
		{
			INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
			INT32 nLeft = rcItem.left + CHECK_IMAGE_OFFSET;
			nTextLeft += CHECK_IMAGE_OFFSET*2;
			nTextLeft += poImage->GetWidth(); 
			poImage->Stretch(hDC,nLeft,nTop,poImage->GetWidth(),poImage->GetHeight());
		}
	}
	else
	{
		if (GetState() == CONTROL_STATE_NORMAL)
		{
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(GetID());
			if (poImage)
			{
				INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
				INT32 nLeft = rcItem.left + CHECK_IMAGE_OFFSET;
				nTextLeft += CHECK_IMAGE_OFFSET*2;
				nTextLeft += poImage->GetWidth(); 
				poImage->Stretch(hDC,nLeft,nTop,poImage->GetWidth(),poImage->GetHeight());
			}
		}
		else
		{
			HRGN hRgn = ::CreateRoundRectRgn(rcItem.left,rcItem.top,rcItem.right,rcItem.bottom,ROUND_WIDTH,ROUND_HEIGHT);
			::DrawGradientRgn(hDC,
							  TRUE,
							  hRgn,
							  CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonNormalFromColor(),
							  CRXSkinService::Instance()->GetDrawMgr().GetCommonButtonNormalToColor());
			DeleteObject(hRgn);
			IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(GetID());
			if (poImage)
			{
				INT32 nTop = rcItem.top + (rcItem.Height() - poImage->GetHeight())/2;
				INT32 nLeft = rcItem.left + CHECK_OFFSET;
				nTextLeft += CHECK_OFFSET*2;
				nTextLeft += poImage->GetWidth(); 
				poImage->Stretch(hDC,nLeft,nTop,poImage->GetWidth(),poImage->GetHeight());
			}
		}
	}
	INT32 nModel = SetBkMode(hDC,TRANSPARENT);
	HFONT hOld = (HFONT)SelectObject(hDC,CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());

	CRXSkinRect rcTxt = rcItem;
	rcTxt.left += nTextLeft;
	tstring strTxt = GetText();
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
			CRXSkinService::Instance()->GetDrawMgr().GetButtonListNormalText(),
			ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	}
	SelectObject(hDC,hOld);
	SetBkMode(hDC,nModel);
}

BOOL CRXCheckImageButton::PtInRect(POINT pt)
{
	return GetRect().PtInRect(pt);
}

VOID CRXCheckImageButton::ReleaseControl()
{
	delete this;
}

BOOL CRXCheckImageButton::GetChecked()
{
	return m_bChecked;
}

VOID CRXCheckImageButton::SetChecked(const BOOL bChecked)
{
	m_bChecked = bChecked;
}

BOOL CRXCheckImageButton::OnClick(POINT pt)
{
	if (FALSE == IsEnabled() || IsVisible() == FALSE)
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		SetChecked(!GetChecked());
		Invalidate();
		if (GetParentWnd())
		{
			::SendMessage(GetParentWnd(),WM_COMMAND,GetID(),0);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CRXCheckImageButton::OnLButtonDown(POINT pt)
{
	if (FALSE == IsEnabled() || IsVisible() == FALSE)
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		SetState(CONTROL_STATE_PRESSED);
		Invalidate();
		return TRUE;
	}
	return FALSE;
}


BOOL CRXCheckImageButton::OnMouseMove(POINT pt)
{
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_MOUSEON)
		{
			SetState(CONTROL_STATE_MOUSEON);
			Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}



////////////  Checkbox Image控件   /////////////////////


////////////  TxtButton控件   /////////////////
#define TXT_BUTTON_OFFSET   3

CRXTxtButton::CRXTxtButton()
{
}

CRXTxtButton::~CRXTxtButton()
{
}

VOID CRXTxtButton::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (FALSE == IsVisible() || FALSE == IsEnabled())
	{
		return;
	}
	SetRect(rcItem);

	COLORREF clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalTxt();
	switch(GetState())
	{
	case CONTROL_STATE_MOUSEON:
		{
			clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnTxt();
			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonBorderColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnFromColor(),
				GetRect(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy);
			HRGN hRgn = ::CreateRoundRectRgn(rcItem.left+1,
				rcItem.top+1,
				rcItem.right-1,
				rcItem.top+rcItem.Height()/2,
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
				TRUE,FALSE,TRUE,FALSE);
			::DrawGradientRgn(hDC,TRUE,hRgn,CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnFromColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnMiddleColor());
			CRXSkinRect rcBottom = rcItem;
			rcBottom.left += 1;
			rcBottom.right -= 1;
			rcBottom.top = rcItem.top + rcItem.Height() / 2;
			rcBottom.bottom = rcItem.bottom - 1;
			CreateRoundRectRgn(hRgn,
				rcBottom,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy,
				FALSE,TRUE,FALSE,TRUE);
			::DrawGradientRgn(hDC,TRUE,hRgn,CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnMiddleColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnToColor());
			DeleteObject(hRgn);
		}
		break;
	case CONTROL_STATE_PRESSED:
		{
			clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedTxt();
			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonBorderColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedFromColor(),
				GetRect(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy);
			HRGN hRgn = ::CreateRoundRectRgn(rcItem.left+1,
				rcItem.top+1,
				rcItem.right-1,
				rcItem.top+rcItem.Height()/2,
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
				TRUE,FALSE,TRUE,FALSE);
			::DrawGradientRgn(hDC,TRUE,hRgn,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedFromColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedMiddleColor());
			CRXSkinRect rcBottom = rcItem;
			rcBottom.left += 1;
			rcBottom.right -= 1;
			rcBottom.top = rcItem.top + rcItem.Height() / 2;
			rcBottom.bottom = rcItem.bottom - 1;
			CreateRoundRectRgn(hRgn,
				rcBottom,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy,
				FALSE,TRUE,FALSE,TRUE);
			::DrawGradientRgn(hDC,TRUE,
				hRgn,CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedMiddleColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonPressedToColor());
			DeleteObject(hRgn);
		}
		break;
	default:
		{
			DrawRoundRect(hDC,
				          CRXSkinService::Instance()->GetDrawMgr().GetButtonBorderColor(),
						  CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalFromColor(),
						  GetRect(),
						  CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
						  CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy);
			HRGN hRgn = ::CreateRoundRectRgn(rcItem.left+1,
				rcItem.top+1,
				rcItem.right-1,
				rcItem.top+rcItem.Height()/2,
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
				TRUE,FALSE,TRUE,FALSE);
			::DrawGradientRgn(hDC,TRUE,hRgn,CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalFromColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalMiddleColor());
			CRXSkinRect rcBottom = rcItem;
			rcBottom.left += 1;
			rcBottom.right -= 1;
			rcBottom.top = rcItem.top + rcItem.Height() / 2;
			rcBottom.bottom = rcItem.bottom - 1;
			CreateRoundRectRgn(hRgn,
				rcBottom,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetButtonRoundSize().cy,
				FALSE,TRUE,FALSE,TRUE);
			::DrawGradientRgn(hDC,TRUE,hRgn,CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalMiddleColor(),
				CRXSkinService::Instance()->GetDrawMgr().GetButtonNormalToColor());
			DeleteObject(hRgn);
		}
		break;
	}
	INT32 nTextLeft = 0;
	INT32 nModel = SetBkMode(hDC,TRANSPARENT);
	CRXSkinRect rcTxt = rcItem;
	rcTxt.left += nTextLeft;
	HFONT hOldFont = (HFONT)::SelectObject(hDC,CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
	tstring strTxt = GetText();
	COLORREF oldClr = ::SetTextColor(hDC, clrTxt);
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
			clrTxt,
			ID_FONT_COMMON, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	}
	SelectObject(hDC,hOldFont);
	SetBkMode(hDC,nModel);
	::SetTextColor(hDC, oldClr);
}

BOOL CRXTxtButton::PtInRect(POINT pt)
{
	if (IsVisible() == FALSE || IsEnabled() == FALSE)
	{
		return FALSE;
	}
	return GetRect().PtInRect(pt);
}

VOID CRXTxtButton::ReleaseControl()
{
	delete this;
}

BOOL CRXTxtButton::OnClick(POINT pt)
{
	if (IsVisible() == FALSE || IsEnabled() == FALSE)
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		if (GetParentWnd())
		{
			::SendMessage(GetParentWnd(),WM_COMMAND,GetID(),0);
		}
		return TRUE;
	}
	return FALSE;
}


BOOL CRXTxtButton::OnLButtonDown(POINT pt)
{
	if (IsVisible() == FALSE || IsEnabled() == FALSE)
	{
		return FALSE;
	}
	if (PtInRect(pt))
	{
		SetState(CONTROL_STATE_PRESSED);
		Invalidate();
		
		return TRUE;
	}
	return FALSE;
}



BOOL CRXTxtButton::OnMouseMove(POINT pt)
{
	if (PtInRect(pt))
	{
		if (GetState()!=CONTROL_STATE_MOUSEON)
		{
			SetState(CONTROL_STATE_MOUSEON);
			Invalidate();
			
		}
		return TRUE;
	}
	return FALSE;
}

////////////  TxtButton控件   /////////////////////



////////////  TxtButton控件   /////////////////
#define TXT_BUTTON_OFFSET   3

CRXSperator::CRXSperator()
{
}

CRXSperator::~CRXSperator()
{
}

VOID CRXSperator::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
/*	CRect rcDraw = rcItem;
	rcDraw.bottom = rcDraw.top + rcDraw.Height() / 2;
	rcDraw.left = rcDraw.left + rcDraw.Width() / 2;
	rcDraw.right = rcDraw.left + 1;
	DrawGradientRect(pDC->m_hDC,
					 TRUE,
					 rcDraw,
					 CRXSkinDrawHelpers::LightenColor(40,GetSysColor(CTLCOLOR_DLG)),
					 CRXSkinDrawHelpers::DarkenColor(40,GetSysColor(CTLCOLOR_DLG)));
	rcDraw.top = rcDraw.bottom;
	rcDraw.bottom = rcItem.bottom;
	DrawGradientRect(pDC->m_hDC,
					 TRUE,
					 rcDraw,
					 CRXSkinDrawHelpers::DarkenColor(40,GetSysColor(CTLCOLOR_DLG)),
					 CRXSkinDrawHelpers::LightenColor(40,GetSysColor(CTLCOLOR_DLG)));
					 */
}

BOOL CRXSperator::PtInRect(POINT pt)
{
	return FALSE;
}

VOID CRXSperator::ReleaseControl()
{
	delete this;
}

BOOL CRXSperator::OnClick(POINT pt)
{
	return FALSE;
}

BOOL CRXSperator::OnLButtonDown(POINT pt)
{
	return FALSE;
}
BOOL CRXSperator::OnMouseMove(POINT pt)
{
	return FALSE;
}

////////////  Sperator控件   /////////////////////



////////////  GroupLine控件   /////////////////
#define GROUP_LINE_BUTTON_OFFSET   3

CRXGroupLine::CRXGroupLine()
{
}

CRXGroupLine::~CRXGroupLine()
{
}

VOID CRXGroupLine::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	CRXSkinRect rcTxt = GetRect();
	rcTxt.left += GROUP_LINE_BUTTON_OFFSET;
	COLORREF clrTxt = GetSysColor(COLOR_BTNTEXT);
	SIZE szTxt;
	tstring strTxt = GetText();
	CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON_BOLD, strTxt, szTxt);
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt,
			clrTxt,
			ID_FONT_COMMON_BOLD, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	}

	rcTxt.left += szTxt.cx;
	rcTxt.left += GROUP_LINE_BUTTON_OFFSET;
	HPEN hTopPen = CreatePen(PS_SOLID,1,::GetSysColor(COLOR_3DHIGHLIGHT));
	HPEN pOldPen = (HPEN)SelectObject(hDC,hTopPen);
	::MoveToEx(hDC,rcTxt.left , rcTxt.top + rcTxt.Height()/2,NULL);
	LineTo(hDC,rcTxt.right, rcTxt.top + rcTxt.Height()/2);
	::DeleteObject(hTopPen);
	HPEN hBottomPen = CreatePen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));

	pOldPen = (HPEN)SelectObject(hDC,hBottomPen);
	MoveToEx(hDC,rcTxt.left , rcTxt.top + rcTxt.Height()/2 - 1,NULL);
	LineTo(hDC,rcTxt.right, rcTxt.top + rcTxt.Height()/2 - 1);
	::DeleteObject(hBottomPen);
	if(NULL!=pOldPen){
		SelectObject(hDC,pOldPen);
	}
}

BOOL CRXGroupLine::PtInRect(POINT pt)
{
	return FALSE;
}

VOID CRXGroupLine::ReleaseControl()
{
	delete this;
}

BOOL CRXGroupLine::OnClick(POINT pt)
{
	return FALSE;
}
BOOL CRXGroupLine::OnLButtonDown(POINT pt)
{
	return FALSE;
}
BOOL CRXGroupLine::OnMouseMove(POINT pt)
{
	return FALSE;
}

////////////  GroupLine控件   /////////////////////


////////////  GroupLine控件   /////////////////
CRXStaticImage::CRXStaticImage()
{
	m_poImage = NULL;
	memset(m_szImage,0,sizeof(TCHAR)*256);
}

CRXStaticImage::~CRXStaticImage()
{
	if (m_poImage)
	{
		m_poImage->Release();
	}
	m_poImage = NULL;
}

VOID CRXStaticImage::SetImage(const TCHAR* pszImageFile)
{
	memset(m_szImage,0,sizeof(TCHAR)*256);
	_tcsncpy(m_szImage,pszImageFile,256);
}

VOID CRXStaticImage::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	CRXSkinService::Instance()->GetDrawMgr().DrawControlBorder(hDC,GetState(),GetRect());
	if (NULL == m_poImage)
		m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenDirectImage(m_szImage);
	if (m_poImage)
	{
		CRXSkinRect rcImage = GetRect();
		UINT32 dwOffSet = CRXSkinService::Instance()->GetDrawMgr().GetControlBorderSize();
		rcImage.left += dwOffSet;
		rcImage.right -= dwOffSet;
		rcImage.top += dwOffSet;
		rcImage.bottom -= dwOffSet;
		CRXSkinService::Instance()->GetDrawMgr().DrawImage(m_poImage,rcImage,hDC);
	}
}

BOOL CRXStaticImage::PtInRect(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (GetRect().PtInRect(pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

VOID CRXStaticImage::ReleaseControl()
{
	delete this;
}

BOOL CRXStaticImage::OnClick(POINT pt)
{
	return PtInRect(pt);
}

BOOL CRXStaticImage::OnLButtonDown(POINT pt)
{
	return PtInRect(pt);
}
BOOL CRXStaticImage::OnMouseMove(POINT pt)
{
	return PtInRect(pt);
}

////////////  GroupLine控件   /////////////////////



////////////  CRXSkinPicture控件   /////////////////
CRXSkinPicture::CRXSkinPicture()
{
	m_poImage = NULL;
	memset(m_szImage,0,sizeof(TCHAR)*256);
}

CRXSkinPicture::~CRXSkinPicture()
{
}

VOID CRXSkinPicture::SetImage(const TCHAR* pszImageFile)
{
	memset(m_szImage, 0, sizeof(TCHAR) * 256);
	_tcsncpy(m_szImage, pszImageFile, 256);

}

VOID CRXSkinPicture::SetImage(IRXImage* poImage)
{
	m_poImage = poImage;
}

VOID CRXSkinPicture::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (NULL == m_poImage)
	{
		m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenImage(m_szImage);
		if (m_poImage)
		{
			float fXRate = (float)m_rcItem.Width() / (float)m_poImage->GetWidth();
			float fYRate = (float)m_rcItem.Height() / (float)m_poImage->GetHeight();
			float fRate = fXRate;
			if (fYRate < fRate)
			{
				fRate = fYRate;
			}
			m_poImage->Scale(fRate);
		}
	}
	if (m_poImage)
	{
		CRXSkinRect rcImage = GetRect();
		m_poImage->Stretch(hDC,rcImage.left,rcImage.top,rcImage.Width(),rcImage.Height());
	}
}

BOOL CRXSkinPicture::PtInRect(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (GetRect().PtInRect(pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

VOID CRXSkinPicture::ReleaseControl()
{
	delete this;
}

BOOL CRXSkinPicture::OnClick(POINT pt)
{
	return PtInRect(pt);
}

BOOL CRXSkinPicture::OnLButtonDown(POINT pt)
{
	return PtInRect(pt);
}
BOOL CRXSkinPicture::OnMouseMove(POINT pt)
{
	return PtInRect(pt);
}

////////////  CRXSkinPicture控件   /////////////////////

CRXSkinBG::CRXSkinBG()
{
	m_clrTo = m_clrFrom = RGB(255,255,255);
}

CRXSkinBG::~CRXSkinBG()
{
}

VOID CRXSkinBG::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	DrawGradientRect(hDC, TRUE, rcItem, m_clrFrom, m_clrTo);
}

BOOL CRXSkinBG::PtInRect(POINT pt)
{
	return FALSE;
}

VOID CRXSkinBG::ReleaseControl()
{
	delete this;
}

BOOL CRXSkinBG::OnClick(POINT pt)
{
	return FALSE;
}

VOID CBorder::_DrawLeft(HDC hDC, CRXSkinRect rcClient, INT32 nTopOffSet, INT32 nBottomOffSet)
{
	if (m_poImage)
	{
		RECT rcDesc;
		rcDesc.left = rcClient.left;
		rcDesc.top = nTopOffSet;
		rcDesc.right = m_poImage->GetWidth();
		rcDesc.bottom = rcDesc.top + m_nOffSetFirst;
		RGBQUAD rcTrans;
		rcTrans.rgbRed = GetRValue(m_clrTrans);
		rcTrans.rgbGreen = GetGValue(m_clrTrans);
		rcTrans.rgbBlue = GetBValue(m_clrTrans);
		rcTrans.rgbReserved = 0;
		m_poImage->DrawTrans(hDC, rcTrans, rcDesc, 0, 0);

		INT32 nHeight = m_poImage->GetHeight() - m_nOffSetSecond;
		rcDesc.top = rcClient.bottom - nBottomOffSet - nHeight;
		rcDesc.bottom = rcDesc.top + nHeight;
		m_poImage->DrawTrans(hDC,
			rcTrans,
			rcDesc,
			0,
			m_nOffSetSecond);
		INT32 nMiddleHeight = m_nOffSetSecond - m_nOffSetFirst;
		INT32 nLessHeight = rcClient.Height() - m_nOffSetFirst - nHeight - nTopOffSet - nBottomOffSet;
		INT32 nCount = nLessHeight / nMiddleHeight;
		INT32 nTop = m_nOffSetFirst + nTopOffSet;
		for (INT32 n = 0; n < nCount; n++)
		{
			rcDesc.top = nTop;
			rcDesc.bottom = rcDesc.top + nMiddleHeight;
			m_poImage->DrawTrans(hDC,
				rcTrans,
				rcDesc,
				0,
				m_nOffSetFirst);
			nTop += nMiddleHeight;
		}
		if (nLessHeight % nMiddleHeight)
		{
			rcDesc.bottom = rcClient.bottom - (m_poImage->GetHeight() - m_nOffSetSecond) - nBottomOffSet;
			rcDesc.top = rcDesc.bottom - nMiddleHeight;
			m_poImage->DrawTrans(hDC,
				rcTrans,
				rcDesc,
				0,
				m_nOffSetFirst);

		}
	}
}

VOID CBorder::_DrawRight(HDC hDC, CRXSkinRect rcClient, INT32 nTopOffSet, INT32 nBottomOffSet)
{
	if (m_poImage)
	{
		RECT rcDesc;
		rcDesc.left = rcClient.right - m_poImage->GetWidth();
		rcDesc.top = nTopOffSet;
		rcDesc.right = rcClient.right;
		rcDesc.bottom = rcDesc.top + m_nOffSetFirst;
		RGBQUAD rcTrans;
		rcTrans.rgbRed = GetRValue(m_clrTrans);
		rcTrans.rgbGreen = GetGValue(m_clrTrans);
		rcTrans.rgbBlue = GetBValue(m_clrTrans);
		rcTrans.rgbReserved = 0;
		m_poImage->DrawTrans(hDC, rcTrans, rcDesc, 0, 0);

		INT32 nHeight = m_poImage->GetHeight() - m_nOffSetSecond;
		rcDesc.top = rcClient.bottom - nBottomOffSet - nHeight;
		rcDesc.bottom = rcDesc.top + nHeight;
		m_poImage->DrawTrans(hDC,
			rcTrans,
			rcDesc,
			0,
			m_nOffSetSecond);
		INT32 nMiddleHeight = m_nOffSetSecond - m_nOffSetFirst;
		INT32 nLessHeight = rcClient.Height() - m_nOffSetFirst - nHeight - nTopOffSet - nBottomOffSet;
		INT32 nCount = nLessHeight / nMiddleHeight;
		INT32 nTop = m_nOffSetFirst + nTopOffSet;
		for (INT32 n = 0; n < nCount; n++)
		{
			rcDesc.top = nTop;
			rcDesc.bottom = rcDesc.top + nMiddleHeight;
			m_poImage->DrawTrans(hDC,
				rcTrans,
				rcDesc,
				0,
				m_nOffSetFirst);
			nTop += nMiddleHeight;
		}
		if (nLessHeight % nMiddleHeight)
		{
			rcDesc.bottom = rcClient.bottom - (m_poImage->GetHeight() - m_nOffSetSecond) - nBottomOffSet;
			rcDesc.top = rcDesc.bottom - nMiddleHeight;
			m_poImage->DrawTrans(hDC,
				rcTrans,
				rcDesc,
				0,
				m_nOffSetFirst);

		}
	}
}


VOID CBorder::_DrawTop(HDC hDC, CRXSkinRect rcClient)
{
	if (m_poImage)
	{
		RECT rcDesc;
		rcDesc.left = rcClient.left;
		rcDesc.top = rcClient.top;
		rcDesc.right = m_nOffSetFirst + rcDesc.left;
		rcDesc.bottom = rcDesc.top + m_poImage->GetHeight();
		RGBQUAD rcTrans;
		rcTrans.rgbRed = GetRValue(m_clrTrans);
		rcTrans.rgbGreen = GetGValue(m_clrTrans);
		rcTrans.rgbBlue = GetBValue(m_clrTrans);
		rcTrans.rgbReserved = 0;
		m_poImage->DrawTrans(hDC, rcTrans, rcDesc, 0, 0);

		INT32 nWidth = m_poImage->GetWidth() - m_nOffSetSecond;
		rcDesc.left = rcClient.right - nWidth;
		rcDesc.top = rcClient.top;
		rcDesc.right = rcDesc.left + m_poImage->GetWidth();
		rcDesc.bottom = rcDesc.top + m_poImage->GetHeight();
		RECT rcCopy;
		rcCopy.left = m_poImage->GetWidth() - nWidth;
		rcCopy.right = m_poImage->GetWidth();
		rcCopy.top = 0;
		rcCopy.bottom = m_poImage->GetHeight();
		m_poImage->DrawTrans(hDC,
			rcTrans,
			rcDesc,
			m_nOffSetSecond,
			0);
		INT32 nMiddleWidth = m_nOffSetSecond - m_nOffSetFirst;
		INT32 nLessWidth = rcClient.Width() - m_nOffSetFirst - nWidth;
		INT32 nCount = nLessWidth / nMiddleWidth;
		INT32 nLeft = m_nOffSetFirst;
		for (INT32 n = 0; n < nCount; n++)
		{
			rcDesc.left = nLeft;
			rcDesc.top = rcClient.top;
			rcDesc.right = rcDesc.left + nMiddleWidth;
			rcDesc.bottom = rcDesc.top + m_poImage->GetHeight();
			m_poImage->DrawTrans(hDC,
				rcTrans,
				rcDesc,
				m_nOffSetFirst,
				0);
			nLeft += nMiddleWidth;
		}
		if (nLessWidth % nMiddleWidth)
		{
			rcDesc.top = rcClient.top;
			rcDesc.left = rcClient.right - nMiddleWidth - nWidth;
			rcDesc.right = rcDesc.left + nMiddleWidth;
			rcDesc.bottom = rcDesc.top + m_poImage->GetHeight();
			m_poImage->DrawTrans(hDC,
				rcTrans,
				rcDesc,
				m_nOffSetFirst,
				0);

		}
	}
}

VOID CBorder::_DrawBottom(HDC hDC, CRXSkinRect rcClient)
{
	if (m_poImage)
	{
		RECT rcDesc;
		rcDesc.left = rcClient.left;
		rcDesc.top = rcClient.bottom - m_poImage->GetHeight();
		rcDesc.right = m_nOffSetFirst + rcDesc.left;
		rcDesc.bottom = rcDesc.top + m_poImage->GetHeight();
		RGBQUAD rcTrans;
		rcTrans.rgbRed = GetRValue(m_clrTrans);
		rcTrans.rgbGreen = GetGValue(m_clrTrans);
		rcTrans.rgbBlue = GetBValue(m_clrTrans);
		rcTrans.rgbReserved = 0;
		m_poImage->DrawTrans(hDC, rcTrans, rcDesc, 0, 0);

		INT32 nWidth = m_poImage->GetWidth() - m_nOffSetSecond;
		rcDesc.left = rcClient.right - nWidth;
		rcDesc.top = rcClient.bottom - m_poImage->GetHeight();
		rcDesc.right = rcDesc.left + m_poImage->GetWidth();
		rcDesc.bottom = rcDesc.top + m_poImage->GetHeight();
		RECT rcCopy;
		rcCopy.left = m_poImage->GetWidth() - nWidth;
		rcCopy.right = m_poImage->GetWidth();
		rcCopy.top = 0;
		rcCopy.bottom = m_poImage->GetHeight();
		m_poImage->DrawTrans(hDC,
			rcTrans,
			rcDesc,
			m_nOffSetSecond,
			0);
		INT32 nMiddleWidth = m_nOffSetSecond - m_nOffSetFirst;
		INT32 nLessWidth = rcClient.Width() - m_nOffSetFirst - nWidth;
		INT32 nCount = nLessWidth / nMiddleWidth;
		INT32 nLeft = m_nOffSetFirst;
		for (INT32 n = 0; n < nCount; n++)
		{
			rcDesc.left = nLeft;
			rcDesc.top = rcClient.bottom - m_poImage->GetHeight();
			rcDesc.right = rcDesc.left + nMiddleWidth;
			rcDesc.bottom = rcDesc.top + m_poImage->GetHeight();
			m_poImage->DrawTrans(hDC,
				rcTrans,
				rcDesc,
				m_nOffSetFirst,
				0);
			nLeft += nMiddleWidth;
		}
		if (nLessWidth % nMiddleWidth)
		{
			rcDesc.top = rcClient.bottom - m_poImage->GetHeight();
			rcDesc.left = rcClient.right - nMiddleWidth - nWidth;
			rcDesc.right = rcDesc.left + nMiddleWidth;
			rcDesc.bottom = rcDesc.top + m_poImage->GetHeight();
			m_poImage->DrawTrans(hDC,
				rcTrans,
				rcDesc,
				m_nOffSetFirst,
				0);
		}
	}
}

VOID CBorder::OnDraw(HDC hDC, CRXSkinRect rcClient, INT32 nTopOffSet, INT32 nBottomOffSet)
{
	if (NULL == m_poImage)
	{
		m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenImage(m_szFilename);		
	}
	switch (m_enType)
	{
	case BORDER_TOP:
	{
		_DrawTop(hDC, rcClient);
	}
	break;
	case BORDER_BOTTOM:
	{
		_DrawBottom(hDC, rcClient);
	}
	break;
	case BORDER_LEFT:
	{
		_DrawLeft(hDC, rcClient, nTopOffSet, nBottomOffSet);
	}
	break;
	case BORDER_RIGHT:
	{
		_DrawRight(hDC, rcClient, nTopOffSet, nBottomOffSet);
	}
	break;
	default:
		break;
	}
}

VOID CBorder::SetImageFile(const TCHAR* pszImageFile)
{
	memset(m_szFilename, 0, 256);
	_tcsncpy(m_szFilename, pszImageFile, 256);

}

VOID CBorder::SetType(CBorder::Type enType)
{
	m_enType = enType;
}

VOID CBorder::SetTrans(COLORREF clrTrans)
{
	m_clrTrans = clrTrans;
}

INT32 CBorder::GetHeight()
{
	if (m_poImage)
	{
		return m_poImage->GetHeight();
	}
	return 0;
}

INT32 CBorder::GetWidth()
{
	if (m_poImage)
	{
		return m_poImage->GetWidth();
	}
	return 0;
}

CBorder::CBorder()
{
	m_poImage = NULL;
	memset(m_szFilename, 0, 256);
	m_nOffSetFirst = m_nOffSetSecond = 0;
	m_enType = BORDER_LEFT;
	m_clrTrans = RGB(0, 0, 0);
}

CBorder::~CBorder()
{

}

VOID CBorder::SetFirstOffSet(const UINT32 dwFirstOffSet)
{
	m_nOffSetFirst = dwFirstOffSet;
}

VOID CBorder::SetSecondOffSet(const UINT32 dwSecondOffSet)
{
	m_nOffSetSecond = dwSecondOffSet;
}
