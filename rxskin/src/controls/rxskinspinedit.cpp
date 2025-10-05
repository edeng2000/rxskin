#include "pch.h"
#include "controls\rxskinspinedit.h"
#include "common/graphics.h"

#include "common/RXSkinService.h"
#include "common/rxskindefines.h"

#define ARROW_OFFSET_X          5

CRXSkinSpinButton::CRXSkinSpinButton()
{
	m_enType = SPIN_TOP;
}

CRXSkinSpinButton::~CRXSkinSpinButton()
{

}

VOID CRXSkinSpinButton::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (m_enType == SPIN_BOTTOM)
	{
		_DrawBottom(hDC);
	}
	else
	{
		_DrawTop(hDC);
	}
}

ESpinType CRXSkinSpinButton::GetType()
{
	return m_enType;
}

VOID CRXSkinSpinButton::SetType(ESpinType enType)
{
	m_enType = enType;
}

CRXSkinItem* CRXSkinSpinButton::HitTest(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return this;
	}
	return NULL;
}

VOID CRXSkinSpinButton::_DrawBottom(HDC hDC)
{
	CRXSkinRect rcItem = GetRect();
	COLORREF clrArrow = CRXSkinService::Instance()->GetDrawMgr().GetArrowNormal();
	INT32 nTop = rcItem.top + (rcItem.Height() - CRXSkinService::Instance()->GetDrawMgr().GetSpinEditArrowHeight())/2;
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
		rcItem.left+(rcItem.Width() - CRXSkinService::Instance()->GetDrawMgr().GetSpinEditArrowHeight())/2,
		nTop,
		CRXSkinService::Instance()->GetDrawMgr().GetSpinEditArrowHeight(),
		DIR_BOTTOM);
}

VOID CRXSkinSpinButton::_DrawTop(HDC hDC)
{
	CRXSkinRect rcItem = GetRect();
	COLORREF clrArrow = CRXSkinService::Instance()->GetDrawMgr().GetArrowNormal();
	INT32 nTop = rcItem.top + (rcItem.Height() - CRXSkinService::Instance()->GetDrawMgr().GetSpinEditArrowHeight())/2+CRXSkinService::Instance()->GetDrawMgr().GetSpinEditArrowHeight();
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
		rcItem.left+(rcItem.Width() - CRXSkinService::Instance()->GetDrawMgr().GetSpinEditArrowHeight())/2,
		nTop,
		CRXSkinService::Instance()->GetDrawMgr().GetSpinEditArrowHeight(),
		DIR_TOP);
}




CRXSkinSpinEdit::CRXSkinSpinEdit(void)
{
	m_hEditWnd = NULL;
	m_btnBottom.SetType(SPIN_BOTTOM);
	m_btnTop.SetType(SPIN_TOP);
}

CRXSkinSpinEdit::~CRXSkinSpinEdit(void)
{
	if (m_hEditWnd)
	{
		::DestroyWindow(m_hEditWnd);
		m_hEditWnd = NULL;
	}
}

BOOL CRXSkinSpinEdit::Create()
{
	INT32 nOffSet = CRXSkinService::Instance()->GetDrawMgr().GetControlBorderSize();

	m_hEditWnd = CreateWindow(_T("Edit"),
		_T(""),
		WS_CHILD|WS_VISIBLE|ES_LEFT,
		GetRect().left+nOffSet,
		GetRect().top+nOffSet,
		GetRect().Width() - GetRect().Height() ,
		GetRect().Height() - 2 * nOffSet,
		GetParentWnd(),
		NULL,
		NULL,
		NULL);

	if (m_hEditWnd)
	{
		::SendMessage(m_hEditWnd,WM_SETFONT,(WPARAM)CRXSkinService::Instance()->GetDrawMgr().GetNormalFont(),MAKELONG(0,0));
	}
	return TRUE;
}

HWND CRXSkinSpinEdit::GetEditWnd()
{
	return m_hEditWnd;
}

VOID CRXSkinSpinEdit::SetText(const TCHAR* strTxt)
{
	CRXControl::SetText(strTxt);
	if (m_hEditWnd)
	{
		::SetWindowText(m_hEditWnd,strTxt);
	}
}

CRXSkinString CRXSkinSpinEdit::GetWindowText()
{
	CRXSkinString strContent;
	if (m_hEditWnd)
	{
		static TCHAR szContent[256]={0};
		memset(szContent,0,sizeof(TCHAR)*256);
		::GetWindowText(m_hEditWnd,szContent,256);
		strContent = szContent;
	}
	return strContent;
}



VOID CRXSkinSpinEdit::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
	if (m_hEditWnd)
	{
		if (bIsVisible)
		{
			::ShowWindow(m_hEditWnd,SW_SHOW);
		}
		else
		{
			::ShowWindow(m_hEditWnd,SW_HIDE);
		}
	}
}

VOID CRXSkinSpinEdit::Enable(const BOOL bEnabled)
{
	CRXControl::Enable(bEnabled);
	if (m_hEditWnd)
	{
		::EnableWindow(m_hEditWnd,bEnabled);
	}
}

VOID CRXSkinSpinEdit::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (FALSE == IsVisible())
	{
		return;
	}
	INT32 nOffSet = CRXSkinService::Instance()->GetDrawMgr().GetControlBorderSize();
	CRXSkinRect rcBK = rcItem;
	rcBK.left += nOffSet;
	rcBK.right -= nOffSet;
	rcBK.top += nOffSet;
	rcBK.bottom -= nOffSet;
	DrawRect(hDC,CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(),rcBK.left,
		rcBK.top,rcBK.right,rcBK.bottom);
	CRXSkinService::Instance()->GetDrawMgr().DrawControlBorder(hDC,GetState(),GetRect(),IsEnabled());
	CRXSkinRect rcTop = rcItem;
	rcTop.left = rcTop.right - rcTop.Height();
	rcTop.bottom = rcTop.top + rcTop.Height() / 2;
	m_btnTop.OnDraw(poSurface,hDC,rcTop);

	CRXSkinRect rcBottom = rcItem;
	rcBottom.left = rcBottom.right - rcBottom.Height();
	rcBottom.top = rcBottom.top + rcBottom.Height() / 2;
	m_btnBottom.OnDraw(poSurface,hDC,rcBottom);
}
VOID CRXSkinSpinEdit::ReleaseControl()
{
	delete this;
}
BOOL CRXSkinSpinEdit::PtInRect(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}
BOOL CRXSkinSpinEdit::OnClick(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinSpinEdit::OnMouseMove(POINT pt)
{
	return PtInRect(pt);
}

BOOL CRXSkinSpinEdit::OnLButtonDown(POINT pt)
{
	return PtInRect(pt);
}
BOOL CRXSkinSpinEdit::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	return CRXControl::OnSize(dwX,dwY);
}
