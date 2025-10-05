#include "pch.h"
#include "common/rximage.h"
#include "controls\rxskintaskbarnotifier.h"
#include "common/graphics.h"
#include "common/RXSkinService.h"
#include "common/rxskindefines.h"

#define IDT_HIDDEN		0
#define IDT_APPEARING		1
#define IDT_WAITING		2
#define IDT_DISAPPEARING	3

#define TASKBAR_ON_TOP		1
#define TASKBAR_ON_LEFT		2
#define TASKBAR_ON_RIGHT	3
#define TASKBAR_ON_BOTTOM	4

CRXSkinTaskbarNotifier::CRXSkinTaskbarNotifier()
{
	m_dwTimeToShow = 500;
	m_dwTimeToLive = 3000;
	m_dwTimeToHide = 500;
	m_dwDelayBetweenShowEvents = 0;
	m_dwDelayBetweenHideEvents = 0;
	m_nIncrement = 1;
	m_nAnimStatus = IDT_HIDDEN;
	m_nStartPosX = 0;
	m_nStartPosY = 0;
	m_nCurrentPosX = 0;
	m_nCurrentPosY = 0;
	m_nTaskbarPlacement = 0;
	m_bPaused = FALSE;
	m_nOffsetTop = m_nOffsetBottom = m_nOffsetX = 0;
	m_dwContentWidth = 120;
	m_enTipType = RXSKIN_MB_OK;
	m_bAutoClose = false;
	m_hSendWnd = NULL;
	m_dwID = 0;
}

CRXSkinTaskbarNotifier::~CRXSkinTaskbarNotifier()
{
}

VOID CRXSkinTaskbarNotifier::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	CRXSkinDialog::OnPaintBk(hDC);
	RECT rcTxt;
	rcTxt.left = 0;
	CRXSkinPicture* poPicture = (CRXSkinPicture*)GetControlMgr().FindControl(ID_PICTURE_TIP);
	if (poPicture)
	{
		rcTxt.left = poPicture->GetRect().right + m_nOffsetX;
		CRXSkinRect rcClient;
		GetClientRectEx(rcClient);
		rcTxt.left = poPicture->GetRect().right + m_nOffsetX;
		rcTxt.right = rcClient.right - m_nOffsetX;
		rcTxt.top = poPicture->GetRect().top;
		rcTxt.bottom = rcClient.bottom - m_nOffsetBottom;
		rcTxt.right = rcClient.right - m_nOffsetX;
		SkCanvas* poCanvas = poSurface->getCanvas();
		if (poCanvas)
		{
			CRXSkinFontMgr::Instance()->DrawText(poCanvas, m_strContent.c_str(),
				rcTxt, CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnTxt(),
				ID_FONT_COMMON, DT_EDITCONTROL | DT_LEFT | DT_WORDBREAK);
		}
	}
}
BOOL CRXSkinTaskbarNotifier::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinDialog::OnCreate(lpCreate);
	if (bResult)
	{
		::SetWindowLong(GetHWnd(), GWL_EXSTYLE, GetWindowLong(GetHWnd(), GWL_EXSTYLE)
			& ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
		::SetWindowPos(GetHWnd(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		ReadConfig(m_strConfigFile.c_str());

		tinyxml2::XMLDocument* poDoc = CRXSkinResourceMgr::Instance()->LoadXmlFromResource(m_strConfigFile.c_str());
		if (poDoc)
		{
			tinyxml2::XMLElement* poRoot = poDoc->RootElement();
			if (poRoot != NULL)
			{
				tinyxml2::XMLElement* poContent = poRoot->FirstChildElement("content");
				if (poContent != NULL)
				{
					char* pszValue = (char*)poContent->Attribute("offsetx");
					if (pszValue)
					{
						m_nOffsetX = atoi(pszValue);
					}
					pszValue = (char*)poContent->Attribute("offsettop");
					if (pszValue)
					{
						m_nOffsetTop = atoi(pszValue);
					}
					pszValue = (char*)poContent->Attribute("offsetbottom");
					if (pszValue)
					{
						m_nOffsetBottom = atoi(pszValue);
					}
				}
			}
			delete poDoc;
		}
		INT32 nLeft = 0;
		INT32 nTop = 0;
		CRXSkinPicture* poPicture = (CRXSkinPicture*)GetControlMgr().FindControl(ID_PICTURE_TIP);
		if (poPicture)
		{
			nLeft = poPicture->GetRect().right;
			nTop = poPicture->GetRect().top;
			switch (m_enTipType)
			{
			case RXSKIN_MB_ERROR:
			{
				IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_MESSAGEBOX_ERROR);
				poPicture->SetImage(poImage);
			}
			break;
			case RXSKIN_MB_HELP:
			{
				IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_MESSAGEBOX_HELP);
				poPicture->SetImage(poImage);
			}
			break;
			case RXSKIN_MB_INFO:
			{
				IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_MESSAGEBOX_TIP);
				poPicture->SetImage(poImage);
			}
			break;
			case RXSKIN_MB_TIP:
			{
				IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_MESSAGEBOX_TIP);
				poPicture->SetImage(poImage);
			}
			break;
			case RXSKIN_MB_WARN:
			{
				IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_MESSAGEBOX_WARN);
				poPicture->SetImage(poImage);
			}
			break;
			case RXSKIN_MB_YESNO:
			{
				IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_MESSAGEBOX_TIP);
				poPicture->SetImage(poImage);
			}
			break;
			default:
				break;
			}
		}

		nLeft += m_nOffsetX;
		//  计算宽度
		INT32 nWidth = m_szMin.cx;
		INT32 nHeight = m_szMin.cy;

		INT32 nContentWidth = nWidth - nLeft - m_nOffsetX;

		SIZE szTxt;
		CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON,
			m_strContent.c_str(), szTxt);

		int nLine = szTxt.cx / nContentWidth;
		if (szTxt.cx % nContentWidth)
		{
			nLine += 1;
		}
		nHeight = (szTxt.cy + 5) * nLine;
		if (nHeight < poPicture->GetRect().Height())
		{
			nHeight = poPicture->GetRect().Height();
		}
		nHeight = poPicture->GetRect().top + nHeight;

		nHeight += (3 * m_nOffsetBottom + DEFAULT_BUTTON_HEIGHT);
		::MoveWindow(m_hWnd, 0, 0, nWidth, nHeight, FALSE);
	
		RECT rcWin;
		GetClientRect(m_hWnd, &rcWin);
		switch (m_enTipType)
		{
		case RXSKIN_MB_OK:
		{
			CRXTxtButton* poBtn = new CRXTxtButton;
			poBtn->SetID(IDOK);
			RECT rcBtn;
			rcBtn.left = rcWin.right - DEFAULT_BUTTON_WIDTH - DEFAULT_OFFSET_X;
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("确定"));
		}
		break;
		case RXSKIN_MB_OKCANCEL:
		{
			CRXTxtButton* poBtn = new CRXTxtButton;
			poBtn->SetID(IDOK);
			RECT rcBtn;
			rcBtn.left = rcWin.right - 2 * (DEFAULT_BUTTON_WIDTH + DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("确定"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDCANCEL);
			rcBtn.left = rcBtn.right + DEFAULT_OFFSET_X;
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("取消"));
		}
		break;
		case RXSKIN_MB_RETRYCANCEL:
		{
			CRXTxtButton* poBtn = new CRXTxtButton;
			poBtn->SetID(IDRETRY);
			RECT rcBtn;
			rcBtn.left = rcWin.right - 2 * (DEFAULT_BUTTON_WIDTH + DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("重试"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDCANCEL);
			rcBtn.left = rcBtn.right + DEFAULT_OFFSET_X;
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("取消"));
		}
		break;
		case RXSKIN_MB_YESNO:
		{
			CRXTxtButton* poBtn = new CRXTxtButton;
			poBtn->SetID(IDYES);
			RECT rcBtn;
			rcBtn.left = rcWin.right - 2 * (DEFAULT_BUTTON_WIDTH + DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("是"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDNO);
			rcBtn.left = rcBtn.right + DEFAULT_OFFSET_X;
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("否"));
		}
		break;
		case RXSKIN_MB_ABORTRETRYIGNORE:
		{
			CRXTxtButton* poBtn = new CRXTxtButton;
			poBtn->SetID(IDABORT);
			RECT rcBtn;
			rcBtn.left = rcWin.right - 3 * (DEFAULT_BUTTON_WIDTH + DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("放弃"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDRETRY);
			rcBtn.left = rcWin.right - 2 * (DEFAULT_BUTTON_WIDTH + DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("重试"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDIGNORE);
			rcBtn.left = rcWin.right - (DEFAULT_BUTTON_WIDTH + DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("忽略"));
		}
		break;
		case RXSKIN_MB_YESNOCANCEL:
		{
			CRXTxtButton* poBtn = new CRXTxtButton;
			poBtn->SetID(IDYES);
			RECT rcBtn;
			rcBtn.left = rcWin.right - 3 * (DEFAULT_BUTTON_WIDTH + DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("是"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDNO);
			rcBtn.left = rcWin.right - 2 * (DEFAULT_BUTTON_WIDTH + DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("否"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDCANCEL);
			rcBtn.left = rcWin.right - (DEFAULT_BUTTON_WIDTH + DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT, RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("取消"));
		}
		break;
		default:
			break;
		}

		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		nWidth = rcClient.right - rcClient.left;
		nHeight = rcClient.bottom - rcClient.top;
		SetTitle(m_strTitle.c_str());

		UINT32 dwDesktopHeight;
		UINT32 dwDesktopWidth;
		UINT32 dwScreenWidth;
		UINT32 dwScreenHeight;
		CRXSkinRect rcDesktop;


		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);
		dwDesktopWidth = rcDesktop.right - rcDesktop.left;
		dwDesktopHeight = rcDesktop.bottom - rcDesktop.top;
		dwScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
		dwScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

		BOOL bTaskbarOnRight = dwDesktopWidth < dwScreenWidth && rcDesktop.left == 0;
		BOOL bTaskbarOnLeft = dwDesktopWidth < dwScreenWidth && rcDesktop.left != 0;
		BOOL bTaskBarOnTop = dwDesktopHeight < dwScreenHeight && rcDesktop.top != 0;
		BOOL bTaskbarOnBottom = dwDesktopHeight < dwScreenHeight && rcDesktop.top == 0;

		switch (m_nAnimStatus)
		{
		case IDT_HIDDEN:
			::ShowWindow(m_hWnd, SW_SHOW);
			if (bTaskbarOnRight)
			{
				m_dwDelayBetweenShowEvents = m_dwTimeToShow / (nWidth / m_nIncrement);
				m_dwDelayBetweenHideEvents = m_dwTimeToHide / (nWidth / m_nIncrement);
				m_nStartPosX = rcDesktop.right;
				m_nStartPosY = rcDesktop.bottom - nHeight;
				m_nTaskbarPlacement = TASKBAR_ON_RIGHT;
			}
			else if (bTaskbarOnLeft)
			{
				m_dwDelayBetweenShowEvents = m_dwTimeToShow / (nWidth / m_nIncrement);
				m_dwDelayBetweenHideEvents = m_dwTimeToHide / (nWidth / m_nIncrement);
				m_nStartPosX = rcDesktop.left - nWidth;
				m_nStartPosY = rcDesktop.bottom - nHeight;
				m_nTaskbarPlacement = TASKBAR_ON_LEFT;
			}
			else if (bTaskBarOnTop)
			{
				m_dwDelayBetweenShowEvents = m_dwTimeToShow / (nHeight / m_nIncrement);
				m_dwDelayBetweenHideEvents = m_dwTimeToHide / (nHeight / m_nIncrement);
				m_nStartPosX = rcDesktop.right - nWidth;
				m_nStartPosY = rcDesktop.top - nHeight;
				m_nTaskbarPlacement = TASKBAR_ON_TOP;
			}
			else //if (bTaskbarOnBottom)
			{
				// Taskbar is on the bottom or Invisible
				m_dwDelayBetweenShowEvents = m_dwTimeToShow / (nHeight / m_nIncrement);
				m_dwDelayBetweenHideEvents = m_dwTimeToHide / (nHeight / m_nIncrement);
				m_nStartPosX = rcDesktop.right - nWidth;
				m_nStartPosY = rcDesktop.bottom;
				m_nTaskbarPlacement = TASKBAR_ON_BOTTOM;
			}

			m_nCurrentPosX = m_nStartPosX;
			m_nCurrentPosY = m_nStartPosY;

			::SetTimer(m_hWnd, IDT_APPEARING, m_dwDelayBetweenShowEvents, NULL);
			break;

		case IDT_WAITING:
		{
			GetClientRect(m_hWnd, &rcClient);
			CRXSkinRect rc = rcClient;
			InvalidateRect(rc);
			::KillTimer(m_hWnd, IDT_WAITING);
			::SetTimer(m_hWnd, IDT_WAITING, m_dwTimeToLive, NULL);
		}

		break;

		case IDT_APPEARING:
		{
			GetClientRect(m_hWnd, &rcClient);
			CRXSkinRect rc = rcClient;
			InvalidateRect(rc);
			break;
		}
		case IDT_DISAPPEARING:
		{
			::KillTimer(m_hWnd, IDT_DISAPPEARING);
			::SetTimer(m_hWnd, IDT_WAITING, m_dwTimeToLive, NULL);
			if (bTaskbarOnRight)
				m_nCurrentPosX = rcDesktop.right - nWidth;
			else if (bTaskbarOnLeft)
				m_nCurrentPosX = rcDesktop.left;
			else if (bTaskBarOnTop)
				m_nCurrentPosY = rcDesktop.top;
			else //if (bTaskbarOnBottom)
				m_nCurrentPosY = rcDesktop.bottom - nHeight;

			::SetWindowPos(m_hWnd, NULL,
				m_nCurrentPosX, m_nCurrentPosY,
				nWidth, nHeight,
				SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
			GetClientRect(m_hWnd, &rcClient);
			CRXSkinRect rc = rcClient;
			InvalidateRect(rc);
		}
		break;
		}
	}
	return bResult;
}
BOOL CRXSkinTaskbarNotifier::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bClicked = CRXSkinDialog::OnLButtonDown(nFlags, point);
	if (FALSE == bClicked)
	{
		CRXSkinRect rcCaption = GetCaptionRect();
		if (rcCaption.PtInRect(point))
		{
			::PostMessage(GetHWnd(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
			return TRUE;
		}
	}
	return bClicked;
}
VOID CRXSkinTaskbarNotifier::OnClickedClose()
{
	Destroy();
}
BOOL CRXSkinTaskbarNotifier::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDYES:
	{
		if (m_hSendWnd)
		{
			::PostMessage(m_hSendWnd, WM_COMMAND, ID_NOTIFY_COMMAND, m_dwID +wParam);
		}
		OnClickedClose();
	}
	break;
	case IDNO:
	{
		if (m_hSendWnd)
		{
			::PostMessage(m_hSendWnd, WM_COMMAND, ID_NOTIFY_COMMAND, m_dwID + wParam);
		}
		OnClickedClose();
	}
	break;
	default:
		break;
	}
	return CRXSkinDialog::OnCommand(wParam,lParam);
}
void CRXSkinTaskbarNotifier::OnFinalMessage(HWND hWnd)
{
	CRXSkinDialog::OnFinalMessage(hWnd);
	delete this;
}

VOID CRXSkinTaskbarNotifier::_OnClickedOK()
{

}
VOID CRXSkinTaskbarNotifier::_OnClickedCancel()
{

}

BOOL CRXSkinTaskbarNotifier::OnMouseMove(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinDialog::OnMouseMove(nFlags,point);
	m_bPaused = TRUE;
	return bResult;
}

BOOL CRXSkinTaskbarNotifier::OnMouseHover(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinDialog::OnMouseHover(nFlags, point);
	m_bPaused = TRUE;
	return bResult;
}

LRESULT CRXSkinTaskbarNotifier::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = CRXSkinDialog::OnMouseLeave(wParam, lParam);
	m_bPaused = FALSE;
	return nResult;
}

BOOL CRXSkinTaskbarNotifier::OnTimer(WPARAM wParam, LPARAM lParam)
{
	if (m_bPaused) return TRUE;
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	INT32 nWidth = rcClient.right - rcClient.left;
	INT32 nHeight = rcClient.bottom - rcClient.top;
	INT32 nIDEvent = (INT32)wParam;
	switch (nIDEvent)
	{
	case IDT_APPEARING:
		m_nAnimStatus = IDT_APPEARING;
		::ShowWindow(GetHWnd(), SW_SHOW);
		switch (m_nTaskbarPlacement)
		{
		case TASKBAR_ON_BOTTOM:
			if (m_nCurrentPosY > (m_nStartPosY - nHeight))
				m_nCurrentPosY -= m_nIncrement;
			else
			{
				::KillTimer(m_hWnd, IDT_APPEARING);
				::SetTimer(m_hWnd, IDT_WAITING, m_dwTimeToLive, NULL);
				m_nAnimStatus = IDT_WAITING;
			}
			break;
		case TASKBAR_ON_TOP:
			if ((m_nCurrentPosY - m_nStartPosY) < nHeight)
				m_nCurrentPosY += m_nIncrement;
			else
			{
				::KillTimer(m_hWnd, IDT_APPEARING);
				::SetTimer(m_hWnd, IDT_WAITING, m_dwTimeToLive, NULL);
				m_nAnimStatus = IDT_WAITING;
			}
			break;
		case TASKBAR_ON_LEFT:
			if ((m_nCurrentPosX - m_nStartPosX) < nHeight)
				m_nCurrentPosX += m_nIncrement;
			else
			{
				::KillTimer(m_hWnd, IDT_APPEARING);
				::SetTimer(m_hWnd, IDT_WAITING, m_dwTimeToLive, NULL);
				m_nAnimStatus = IDT_WAITING;
			}
			break;
		case TASKBAR_ON_RIGHT:
			if (m_nCurrentPosX > (m_nStartPosX - nWidth))
				m_nCurrentPosX -= m_nIncrement;
			else
			{
				::KillTimer(m_hWnd, IDT_APPEARING);
				::SetTimer(m_hWnd, IDT_WAITING, m_dwTimeToLive, NULL);
				m_nAnimStatus = IDT_WAITING;
			}
			break;
		}
		::SetWindowPos(m_hWnd, NULL,
			m_nCurrentPosX, m_nCurrentPosY,
			nWidth, nHeight,
			SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		//RedrawWindow();
		break;

	case IDT_WAITING:
		::KillTimer(m_hWnd, IDT_WAITING);
		::SetTimer(m_hWnd, IDT_DISAPPEARING, m_dwDelayBetweenHideEvents, NULL);
		break;

	case IDT_DISAPPEARING:
	{
		if (m_bAutoClose==false)
		{
			return TRUE;
		}
		m_nAnimStatus = IDT_DISAPPEARING;
		switch (m_nTaskbarPlacement)
		{
		case TASKBAR_ON_BOTTOM:
			if (m_nCurrentPosY < m_nStartPosY)
				m_nCurrentPosY += m_nIncrement;
			else
			{
				::KillTimer(m_hWnd, IDT_DISAPPEARING);
				Hide();
			}
			break;
		case TASKBAR_ON_TOP:
			if (m_nCurrentPosY > m_nStartPosY)
				m_nCurrentPosY -= m_nIncrement;
			else
			{
				::KillTimer(m_hWnd, IDT_DISAPPEARING);
				Hide();
			}
			break;
		case TASKBAR_ON_LEFT:
			if (m_nCurrentPosX > m_nStartPosX)
				m_nCurrentPosX -= m_nIncrement;
			else
			{
				::KillTimer(m_hWnd, IDT_DISAPPEARING);
				Hide();
			}
			break;
		case TASKBAR_ON_RIGHT:
			if (m_nCurrentPosX < m_nStartPosX)
				m_nCurrentPosX += m_nIncrement;
			else
			{
				::KillTimer(m_hWnd, IDT_DISAPPEARING);
				Hide();
			}
			break;
		}
		::SetWindowPos(m_hWnd, NULL,
			m_nCurrentPosX, m_nCurrentPosY,
			nWidth, nHeight,
			SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		//RedrawWindow();
	}
	break;
	}
	return FALSE;
}

bool CRXSkinTaskbarNotifier::Show(const TCHAR* pszWindowName,
	const TCHAR* pszContent,
	const EMessageBoxType& enType,
	UINT32 dwTimeToShow,
	UINT32 dwTimeToStay,
	UINT32 dwTimeToHide,
	INT32 nIncrement )
{
	m_enTipType = enType;
	m_strContent = pszContent;
	m_dwTimeToShow = dwTimeToShow;
	m_dwTimeToLive = dwTimeToStay;
	m_dwTimeToHide = dwTimeToHide;
	m_nIncrement = nIncrement;
	m_strTitle = pszWindowName;
	if (!CRXSkinDialog::Create(WND_RXSKIN_TASKBAR_NOTIFIER_NAME,
		pszWindowName,
		WS_POPUP,
		GetDesktopWindow(),
		NULL))
	{
		return false;
	}
	return true;
}


void CRXSkinTaskbarNotifier::Hide()
{
	switch (m_nAnimStatus)
	{
	case IDT_APPEARING:
		::KillTimer(m_hWnd, IDT_APPEARING);
		break;
	case IDT_WAITING:
		::KillTimer(m_hWnd, IDT_WAITING);
		break;
	case IDT_DISAPPEARING:
		::KillTimer(m_hWnd, IDT_DISAPPEARING);
		break;
	}
	::MoveWindow(m_hWnd, 0, 0, 0, 0, TRUE);
	::ShowWindow(m_hWnd, SW_HIDE);
	m_nAnimStatus = IDT_HIDDEN;
	Destroy();
}
