#include "pch.h"
#include "main\rxskinmessagebox.h"
#include "common\rxskinservice.h"
#include "common\rxskinmemdc.h"
#include "fundation\rxstring.h"
#include "common\rxskindefines.h"
#include "fundation\rxstring.h"
#include "rxskinres.h"

CRXSkinMessageBox::CRXSkinMessageBox(void)
{
	m_enTipType = RXSKIN_MB_HELP;
	m_enType = RXSKIN_MB_OK;
	m_nOffsetTop = m_nOffsetBottom = m_nOffsetX = 0;
	SetAutoDelete(true);
	m_dwContentWidth = 120;
}

CRXSkinMessageBox::~CRXSkinMessageBox(void)
{
}

VOID CRXSkinMessageBox::SetTipType(EMessageBoxTipType enTipType)
{
	m_enTipType = enTipType;
}

VOID CRXSkinMessageBox::SetType(EMessageBoxType enType)
{
	m_enType = enType;
}

VOID CRXSkinMessageBox::SetContent(const TCHAR* pszContent)
{
	m_strContent = pszContent;
}

VOID CRXSkinMessageBox::Destroy()
{
	CRXSkinDialog::Destroy();
	CRXSkinService::Instance()->GetPreMessages().Empty();
}

BOOL CRXSkinMessageBox::OnCreate(LPCREATESTRUCT lpCreate)
{
	CRXSkinDialog::OnCreate(lpCreate);
	::SetWindowLong(GetHWnd(),GWL_EXSTYLE,GetWindowLong(GetHWnd(),GWL_EXSTYLE) 
		&~WS_EX_APPWINDOW|WS_EX_TOOLWINDOW ); 
	::SetWindowPos(GetHWnd(),NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE); 
	TCHAR* pszResource = MAKEINTRESOURCE(IDR_XML_MESSAGEBOX);
	BOOL bResult =	ReadConfigFromResource(pszResource);
	if (bResult==FALSE)
	{
		ReadConfig(_T("configs/messagebox.xml"));
	}
	
	AddMsgFilter(this);
	CRXSkinService::Instance()->GetPreMessages().Add(this);
	CRXSkinService::Instance()->GetPreMessages().Add(&m_oToolTip);
	INT32 nLeft = 0;
	INT32 nTop = 0;
	CRXSkinPicture* poPicture = (CRXSkinPicture*)GetControlMgr().FindControl(ID_PICTURE_TIP);
	if (poPicture)
	{
		nLeft = poPicture->GetRect().right;
		nTop = poPicture->GetRect().top;
		switch(m_enTipType)
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
		case RXSKIN_MB_WARN:
			{
				IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_MESSAGEBOX_WARN);
				poPicture->SetImage(poImage);
			}
			break;
		default:
			break;
		}
	}
	tinyxml2::XMLDocument* poDoc = CRXSkinResourceMgr::Instance()->GetXml(pszResource);
	if (NULL == poDoc)
	{
		poDoc = CRXSkinResourceMgr::Instance()->LoadXmlFromResource(_T("configs/messagebox.xml"));
	}
	if (poDoc)
	{
		tinyxml2::XMLElement* poRoot = poDoc->RootElement();
		if (poRoot !=NULL)
		{
			tinyxml2::XMLElement* poContent = poRoot->FirstChildElement("content");
			if (poContent !=NULL)
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

	nLeft += m_nOffsetX;
	//  计算宽度
	INT32 nWidth = m_szMin.cx;
	INT32 nHeight = m_szMin.cy;

	INT32 nContentWidth = nWidth - nLeft - m_nOffsetX;

	SIZE szTxt;
	CRXSkinFontMgr::Instance()->GetTextSize(ID_FONT_COMMON,
		m_strContent.GetData(), szTxt);

	int nLine = szTxt.cx / nContentWidth;
	if (szTxt.cx % nContentWidth)
	{
		nLine += 1;
	}
	nHeight = (szTxt.cy+ 5)*nLine;
	if (nHeight< poPicture->GetRect().Height())
	{
		nHeight = poPicture->GetRect().Height();
	}
	nHeight = poPicture->GetRect().top + nHeight;

	nHeight += (3 * m_nOffsetBottom + DEFAULT_BUTTON_HEIGHT);
	::MoveWindow(m_hWnd, 0, 0, nWidth, nHeight, FALSE);
	CenterWindow();
	RECT rcWin;
	GetClientRect(m_hWnd,&rcWin);
	switch(m_enType)
	{
	case RXSKIN_MB_OK:
		{
			CRXTxtButton* poBtn = new CRXTxtButton;
			poBtn->SetID(IDOK);
			RECT rcBtn;
			rcBtn.left = rcWin.right - DEFAULT_BUTTON_WIDTH - DEFAULT_OFFSET_X;
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3*DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
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
			rcBtn.left = rcWin.right - 2*(DEFAULT_BUTTON_WIDTH+DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
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
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
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
			rcBtn.left = rcWin.right - 2*(DEFAULT_BUTTON_WIDTH+DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
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
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
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
			rcBtn.left = rcWin.right - 2*(DEFAULT_BUTTON_WIDTH+DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
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
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
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
			rcBtn.left = rcWin.right - 3*(DEFAULT_BUTTON_WIDTH+DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("放弃"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDRETRY);
			rcBtn.left = rcWin.right - 2*(DEFAULT_BUTTON_WIDTH+DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("重试"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDIGNORE);
			rcBtn.left = rcWin.right - (DEFAULT_BUTTON_WIDTH+DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
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
			rcBtn.left = rcWin.right - 3*(DEFAULT_BUTTON_WIDTH+DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("是"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDNO);
			rcBtn.left = rcWin.right - 2*(DEFAULT_BUTTON_WIDTH+DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("否"));

			poBtn = new CRXTxtButton;
			poBtn->SetID(IDCANCEL);
			rcBtn.left = rcWin.right - (DEFAULT_BUTTON_WIDTH+DEFAULT_OFFSET_X);
			rcBtn.right = rcBtn.left + DEFAULT_BUTTON_WIDTH;
			rcBtn.top = rcWin.bottom - 3 * DEFAULT_OFFSET_Y - DEFAULT_BUTTON_HEIGHT;
			rcBtn.bottom = rcBtn.top + DEFAULT_BUTTON_HEIGHT;
			poBtn->SetRect(rcBtn);
			poBtn->SetResize(RXSKIN_ANCHOR_TOPLEFT,RXSKIN_ANCHOR_TOPLEFT);
			poBtn->SetParentWnd(m_hWnd);
			GetControlMgr().AddControl(poBtn);
			poBtn->SetText(_T("取消"));
		}
		break;
	default:
		break;
	}
	return TRUE;
}

VOID CRXSkinMessageBox::OnClickedClose()
{
	Destroy();
	CRXSkinDialog::OnClickedClose();
}

VOID CRXSkinMessageBox::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
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
			CRXSkinFontMgr::Instance()->DrawText(poCanvas, m_strContent.GetData(),
				rcTxt, CRXSkinService::Instance()->GetDrawMgr().GetButtonMouseOnTxt(),
				ID_FONT_COMMON, DT_EDITCONTROL | DT_LEFT | DT_WORDBREAK);
		}

	}
}

BOOL CRXSkinMessageBox::OnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinDialog::OnCommand(wParam,lParam);
	if (FALSE == bResult)
	{
		switch(wParam)
		{
		case IDOK:
			{
				m_nModelRet = IDOK;
				Destroy();
				return TRUE;
			}
			break;
		case IDCANCEL:
			{
				m_nModelRet = IDCANCEL;
				Destroy();
				return TRUE;
			}
			break;
		case IDABORT:
			{
				m_nModelRet = IDABORT;
				Destroy();
				return TRUE;
			}
			break;
		case IDRETRY:
			{
				m_nModelRet = IDRETRY;
				Destroy();
				return TRUE;
			}
			break;
		case IDIGNORE:
			{
				m_nModelRet = IDIGNORE;
				Destroy();
				return TRUE;
			}
			break;
		case IDNO:
			{
				m_nModelRet = IDNO;
				Destroy();
				return TRUE;
			}
			break;
		case IDYES:
			{
				m_nModelRet = IDYES;
				Destroy();
				return TRUE;
			}
			break;
		default:
			break;
		}
	}
	return bResult;
}