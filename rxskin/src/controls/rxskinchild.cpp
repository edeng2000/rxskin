#include "pch.h"
#include "controls\RXSkinChild.h"
#include "controls/RXSkinComboBox.h"
#include "controls/RXSkinEdit.h"
#include "controls/RXSkinTabCtrl.h"
#include "controls/rxskindatepicker.h"
#include "controls/rxskinlocationpicker.h"
#include "controls/rxskingif.h"
#include "controls\rxskinoutbar.h"
#include "controls\rxskinbrowser.h"
#include "controls\rxskinlistctrl.h"
#include "controls/rxskinspinedit.h"

#include "main\rxskindialog.h"
#include "fundation/rxstring.h"
#include "common\rxskinutil.h"
#include "common\RXSkinService.h"
#include "fundation/tinyxml2.h"
CRXSkinChild::CRXSkinChild(void)
{
	m_poParent = NULL;
	m_poInvalidateControl = NULL;
}

CRXSkinChild::~CRXSkinChild(void)
{
}

VOID CRXSkinChild::OnMouseLeave()
{
	m_oControlMgr.OnMouseLeave();
}

CRXControl* CRXSkinChild::HitTest(POINT pt)
{
	return m_oControlMgr.HitTest(pt);
}

VOID CRXSkinChild::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible() && IsEnabled())
	{
		m_oControlMgr.OnDraw(poSurface,hDC);
	}	
}

VOID CRXSkinChild::ReleaseControl()
{
	delete this;
}

BOOL CRXSkinChild::PtInRect(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}

BOOL CRXSkinChild::OnClick(POINT pt) 
{
	if (IsEnabled() && IsVisible())
	{
		return m_oControlMgr.OnClick(pt);
	}
	return FALSE;
}

CRXSkinRect CRXSkinChild::GetInvalidateRect()
{
	return m_rcInvalidate;
}

CRXControl* CRXSkinChild::GetInvalidateControl()
{
	return m_poInvalidateControl;
}

VOID CRXSkinChild::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
	m_oControlMgr.SetVisible(bIsVisible);
}

BOOL CRXSkinChild::OnMouseMove(POINT pt)
{
	CRXControl* poControl = m_oControlMgr.OnMouseMove(pt);
	if (poControl)
	{
		m_poInvalidateControl = poControl;
	}
	return poControl == NULL ? FALSE : TRUE;
}


BOOL CRXSkinChild::OnMouseWheel(UINT nFlags, short zDelta,POINT pt)
{
	return m_oControlMgr.OnMouseWheel(nFlags,zDelta,pt);
}

VOID CRXSkinChild::OnScrollBarChanged(EScrollType enType,INT32 nPercent)
{
	m_oControlMgr.OnScrollBarChanged(enType,nPercent);
}


BOOL CRXSkinChild::OnLButtonDown(POINT pt)
{
	CRXControl* poControl = m_oControlMgr.OnLButtonDown(pt);
	if (poControl)
	{
		m_poInvalidateControl = poControl;
	}
	return poControl == NULL ? FALSE : TRUE;
}
BOOL CRXSkinChild::OnLButtonUp(POINT pt)
{
	CRXControl* poControl = m_oControlMgr.OnLButtonUp(pt);
	if (poControl)
	{
	}
	return poControl == NULL ? FALSE : TRUE;
}
BOOL CRXSkinChild::OnLButtonDblClk(POINT pt)
{
	CRXControl* poControl = m_oControlMgr.OnLButtonDblClk(pt);
	if (poControl)
	{
	}
	return poControl == NULL ? FALSE : TRUE;
}
BOOL CRXSkinChild::OnRButtonUp(POINT pt)
{
	CRXControl* poControl = m_oControlMgr.OnRButtonUp(pt);
	if (poControl)
	{
	}
	return poControl == NULL ? FALSE : TRUE;
}
BOOL CRXSkinChild::OnTimer(WPARAM wParam, LPARAM lParam)
{
	if (CRXControl::OnTimer(wParam,lParam))
	{
		return TRUE;
	}
	return GetControlMgr().OnTimer(wParam,lParam);
}

BOOL CRXSkinChild::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	return m_oControlMgr.OnSize(dwX,dwY);
}

BOOL CRXSkinChild::Create(CRXSkinDialog* poParent,const TCHAR* pszConfigFile)
{
	m_poParent = poParent;
	return _ReadConfig(pszConfigFile);
}

BOOL CRXSkinChild::_ReadConfig(const TCHAR* pszConfig)
{
	tinyxml2::XMLDocument* poDoc = CRXSkinResourceMgr::Instance()->LoadXmlFromResource(pszConfig);
	if (poDoc)
	{
		tinyxml2::XMLElement* poRoot = poDoc->RootElement();
		if (poRoot)
		{
			tinyxml2::XMLElement* poElementControls = poRoot->FirstChildElement("Controls");
			if (poElementControls)
			{
				tinyxml2::XMLElement* poElementControl = poElementControls->FirstChildElement("Control");
				while (poElementControl)
				{
				 	CRXControl* poControl = CRXSkinService::Instance()->GetDrawMgr().CreateControl(poElementControl, &m_oControlMgr, m_poParent);
					if (poControl)
					{
						CRXSkinRect rcItem = poControl->GetRect();
						CRXSkinRect rcCur = GetRect();
						int nWidth = rcItem.Width();
						int nHeight = rcItem.Height();
						rcItem.left += rcCur.left;
						rcItem.top += rcCur.top;;
						rcItem.right = rcItem.left + nWidth;
						rcItem.bottom = rcItem.top + nHeight;
						poControl->SetRect(rcItem);
						poControl->OffsetRect(rcCur.left, rcCur.top);
						poControl->ModifyRect();
					}
					poElementControl = poElementControl->NextSiblingElement("Control");
				}
			}
		}
		delete poDoc;
	}
	return TRUE;

}

void CRXSkinChild::OffsetRect(int32_t nX, int32_t nY)
{

}

CRXControlMgr& CRXSkinChild::GetControlMgr()
{
	return m_oControlMgr;
}

HBRUSH CRXSkinChild::OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC)
{
	return m_oControlMgr.OnCtlColorControl(dwMsgID,hWndChild,hDC);
}