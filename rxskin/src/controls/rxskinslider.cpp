#include "pch.h"
#include "controls\rxskinslider.h"
#include "common\RXSkinService.h"
#include "common\rxskindefines.h"

#define   SLIDER_STEP             3

CRXSkinSlider::CRXSkinSlider(void)
{
	m_enType = SLIDER_HOR;
	m_fSliderlPercent = 0.0;
	m_bPressed = FALSE;
}

CRXSkinSlider::~CRXSkinSlider(void)
{
}

VOID CRXSkinSlider::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (IsVisible())
	{
		if (SLIDER_HOR == m_enType)
		{
			_DrawHor(hDC);
		}
		else
		{
			_DrawVer(hDC);
		}
	}
}

VOID CRXSkinSlider::ReleaseControl()
{
	delete this;
}


BOOL CRXSkinSlider::PtInRect(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}

BOOL CRXSkinSlider::OnClick(POINT pt)
{
	if (m_bPressed)
	{
		m_bPressed = FALSE;
		_NotifySliderChanged();
	}
	return FALSE;
}

VOID CRXSkinSlider::_NotifySliderChanged()
{
	if (m_enType == SLIDER_HOR)
	{		
		INT32 nPercent = (INT32)(m_fSliderlPercent*100.00);
		if (nPercent<0)
		{
			nPercent = 0;
			m_fSliderlPercent = 0.0;
		}
		if (nPercent > 100) {
			nPercent = 100; 
			m_fSliderlPercent = 1.0f;
		}
		::SendMessage(GetParentWnd(), WM_RXSKIN_SLIDER_POS, GetID(), nPercent);
	}
	else
	{
		INT32 nPercent = (INT32)(m_fSliderlPercent * 100.00);
		if (nPercent < 0)
		{
			nPercent = 0;
			m_fSliderlPercent = 0.0;
		}
		if (nPercent > 100) {
			nPercent = 100;
			m_fSliderlPercent = 1.0f;
		}
		::SendMessage(GetParentWnd(), WM_RXSKIN_SLIDER_POS, GetID(), nPercent);
	}
}

void CRXSkinSlider::SetPercent(float fPercent)
{
	m_fSliderlPercent = fPercent;
	Invalidate();
}

void CRXSkinSlider::_SetPos(POINT pt)
{
	if (m_enType == SLIDER_VER)
	{
		INT32 nSliderPos = pt.y;
		if (nSliderPos <= GetRect().top)
		{
			nSliderPos = GetRect().top;
		}
		else if (nSliderPos >= GetRect().bottom)
		{
			nSliderPos = GetRect().bottom;
		}
		m_fSliderlPercent = (double)(nSliderPos - GetRect().top) / (double)GetRect().Height();
		CRXSkinRect rcItem = GetRect();
		IRXImage* poSliderbar = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SLIDER_BAR);
		if (poSliderbar)
		{
			rcItem.top -= poSliderbar->GetHeight() / 2;
		}
		InvalidateRect(GetParentWnd(), &rcItem, FALSE);
		_NotifySliderChanged();
	}
	else
	{
		INT32 nSliderPos = pt.x;
		if (nSliderPos <= GetRect().left)
		{
			nSliderPos = GetRect().left;
		}
		else if (nSliderPos >= GetRect().right)
		{
			nSliderPos = GetRect().right;
		}
		m_fSliderlPercent = (double)(nSliderPos - GetRect().left) / (double)GetRect().Width();
		CRXSkinRect rcItem = GetRect();
		IRXImage* poSliderbar = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SLIDER_BAR);
		if (poSliderbar)
		{
			rcItem.left -= (poSliderbar->GetWidth() - 2) / 6;
			rcItem.right += (poSliderbar->GetWidth() - 2) / 6;
		}
		InvalidateRect(GetParentWnd(), &rcItem, FALSE);
		_NotifySliderChanged();
	}
}

BOOL CRXSkinSlider::OnMouseMove(POINT pt)
{
	if (IsVisible() == FALSE) return FALSE;
	if (PtInRect(pt))
	{
		if (m_bPressed)
		{
			_SetPos(pt);
			return TRUE;
		}
	
		if (PtInRect(pt))
		{
			CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_HAND);

		}
		else if (CONTROL_STATE_NORMAL == GetState())
		{
			CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);

			SetState(CONTROL_STATE_MOUSEON);
			RECT rc = GetRect();
			InvalidateRect(GetParentWnd(), &rc, FALSE);
		}
		return TRUE;
	}
	else
	{

		if (CONTROL_STATE_MOUSEON == GetState())
		{
			SetState(CONTROL_STATE_NORMAL);
			RECT rc = GetRect();
			InvalidateRect(GetParentWnd(), &rc, FALSE);
		}
	}
	return FALSE;
}
BOOL CRXSkinSlider::OnLButtonDown(POINT pt)
{
	if (IsVisible() == FALSE) return FALSE;
	if (PtInRect(pt))
	{
		m_bPressed = TRUE;
		_NotifySliderChanged();
	}
	return FALSE;
}

BOOL CRXSkinSlider::OnLButtonUp(POINT pt)
{
	if (IsVisible() == FALSE) return FALSE;
	if (m_bPressed)
	{
		m_bPressed = FALSE;
		_SetPos(pt);
		_NotifySliderChanged();
	}

	return TRUE;

}

BOOL CRXSkinSlider::OnSize(const UINT32 dwX, const UINT32 dwY)
{
	CRXControl::OnSize(dwX, dwY);
	return FALSE;
}

void CRXSkinSlider::SetRect(const CRXSkinRect rcItem)
{
	CRXControl::SetRect(rcItem);
}

VOID CRXSkinSlider::_DrawHor(HDC hDC)
{
	IRXImage* poSlider = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SLIDER_NORMAL_BK);
	if (NULL == poSlider)
	{
		return;
	}
	IRXImage* poSliderbar = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SLIDER_BAR);
	if (NULL == poSliderbar)
	{
		return;
	}
	IRXImage* poSliderSelected = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SLIDER_SELECTED_BK);
	if (NULL == poSliderSelected)
	{
		return;
	}
	UINT32 dwBarWidth = (poSliderbar->GetWidth() - 2) / 3;

	CRXSkinRect rcTrack = GetRect();
	rcTrack.left += dwBarWidth / 2;
	rcTrack.right -= dwBarWidth / 2;
	//  绘制顶部
	poSlider->DrawHor(hDC, CRXSkinService::Instance()->GetDrawMgr().GetSliderTrans(), rcTrack,
		CRXSkinService::Instance()->GetDrawMgr().GetSliderHorLeftOffset(),
		CRXSkinService::Instance()->GetDrawMgr().GetSliderHorRightOffset());

	UINT32 dwOffSetX = (UINT32)(m_fSliderlPercent*GetRect().Width());
	RECT rcSelected = rcTrack;
	rcSelected.right = rcSelected.left + dwOffSetX;
	poSliderSelected->DrawHor(hDC, CRXSkinService::Instance()->GetDrawMgr().GetSliderTrans(), rcSelected,
		CRXSkinService::Instance()->GetDrawMgr().GetSliderHorSelectedLeftOffset(),
		CRXSkinService::Instance()->GetDrawMgr().GetSliderHorSelectedRightOffset());


	RECT rcDesc = GetRect();
	rcDesc.top = GetRect().top + (GetRect().Height() - poSliderbar->GetHeight()) / 2;
	rcDesc.bottom = rcDesc.top + poSliderbar->GetHeight();
	rcDesc.left = rcDesc.left + dwOffSetX;
	rcDesc.right = rcDesc.left + dwBarWidth;
	RGBQUAD rcTrans;
	rcTrans.rgbRed = GetRValue(CRXSkinService::Instance()->GetDrawMgr().GetSliderTrans());
	rcTrans.rgbGreen = GetGValue(CRXSkinService::Instance()->GetDrawMgr().GetSliderTrans());
	rcTrans.rgbBlue = GetBValue(CRXSkinService::Instance()->GetDrawMgr().GetSliderTrans());
	rcTrans.rgbReserved = 0;
	switch (GetState())
	{
	case CONTROL_STATE_NORMAL:
	{
		poSliderbar->DrawTrans(hDC, rcTrans, rcDesc, 0, 0);
	}
	break;
	case  CONTROL_STATE_MOUSEON:
	{

		poSliderbar->DrawTrans(hDC, rcTrans, rcDesc, dwBarWidth + 1, 0);
	}
	break;
	case  CONTROL_STATE_PRESSED:
	{

		poSliderbar->DrawTrans(hDC, rcTrans, rcDesc, 2 * dwBarWidth + 2, 0);
	}
	break;
	default:
		poSliderbar->DrawTrans(hDC, rcTrans, rcDesc, 0, 0);
		break;
	}

}

VOID CRXSkinSlider::_DrawVer(HDC hDC)
{
	IRXImage* poSlider = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SLIDER_NORMAL_BK);
	if (NULL == poSlider)
	{
		return;
	}

	//  绘制顶部
	poSlider->DrawHor(hDC, CRXSkinService::Instance()->GetDrawMgr().GetSliderTrans(), GetRect(),
		CRXSkinService::Instance()->GetDrawMgr().GetSliderHorLeftOffset(),
		CRXSkinService::Instance()->GetDrawMgr().GetSliderHorRightOffset());
	IRXImage* poSliderSelected = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SLIDER_SELECTED_BK);
	if (NULL == poSliderSelected)
	{
		return;
	}
	UINT32 dwOffSetY = (UINT32)(m_fSliderlPercent*GetRect().Height());
	RECT rcSelected = GetRect();
	rcSelected.right = rcSelected.left + (GetRect().Width() - poSliderSelected->GetWidth()) / 2;
	rcSelected.bottom = rcSelected.top + dwOffSetY;
	poSliderSelected->DrawHor(hDC, CRXSkinService::Instance()->GetDrawMgr().GetSliderTrans(), rcSelected,
		CRXSkinService::Instance()->GetDrawMgr().GetSliderHorSelectedLeftOffset(),
		CRXSkinService::Instance()->GetDrawMgr().GetSliderHorSelectedRightOffset());
	IRXImage* poSliderbar = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_SLIDER_BAR);
	if (NULL == poSliderbar)
	{
		return;
	}
	RECT rcDesc = GetRect();
	rcDesc.top = GetRect().top + dwOffSetY- poSliderbar->GetHeight() / 2;
	rcDesc.bottom = rcDesc.top + poSliderbar->GetHeight();
	rcDesc.left = rcDesc.left +( GetRect().Width() - poSliderbar->GetWidth() / 3)/2;
	rcDesc.right = rcDesc.left + poSliderbar->GetWidth() / 3;
	RGBQUAD rcTrans;
	rcTrans.rgbRed = GetRValue(CRXSkinService::Instance()->GetDrawMgr().GetSliderTrans());
	rcTrans.rgbGreen = GetGValue(CRXSkinService::Instance()->GetDrawMgr().GetSliderTrans());
	rcTrans.rgbBlue = GetBValue(CRXSkinService::Instance()->GetDrawMgr().GetSliderTrans());
	rcTrans.rgbReserved = 0;
	switch (GetState())
	{
	case CONTROL_STATE_NORMAL:
	{
		poSliderbar->DrawTrans(hDC, rcTrans, rcDesc, 0, 0);
	}
	break;
	case  CONTROL_STATE_MOUSEON:
	{

		poSliderbar->DrawTrans(hDC, rcTrans, rcDesc, poSliderbar->GetWidth() / 3, 0);
	}
	break;
	case  CONTROL_STATE_PRESSED:
	{

		poSliderbar->DrawTrans(hDC, rcTrans, rcDesc, (2 * poSliderbar->GetWidth()) / 3, 0);
	}
	break;
	default:
		poSliderbar->DrawTrans(hDC, rcTrans, rcDesc, 0, 0);
		break;
	}

}


VOID CRXSkinSlider::SetType(ESliderType enType)
{
	m_enType = enType;	
}

VOID CRXSkinSlider::Reset()
{
	m_bPressed = FALSE;
}

VOID CRXSkinSlider::Step(INT32 nStep)
{
	if (m_enType == SLIDER_VER)
	{
		UINT32 dwOffSetY = (UINT32)(GetRect().Height() * m_fSliderlPercent);
		INT32 nSliderPos = dwOffSetY + nStep + GetRect().top;
		if (GetRect().top  >= nSliderPos)
		{
			nSliderPos = GetRect().top;
		}
		else if (GetRect().bottom  <= nSliderPos)
		{
			nSliderPos = GetRect().bottom;
		}
		m_fSliderlPercent = (double)(nSliderPos - GetRect().top) / (double)GetRect().Height();
		_NotifySliderChanged();
	}
	else
	{
		UINT32 dwOffSetY = (UINT32)(GetRect().Width() * m_fSliderlPercent);
		INT32 nSliderPos = dwOffSetY + nStep + GetRect().left;
		if (GetRect().left >= nSliderPos)
		{
			nSliderPos = GetRect().left;
		}
		else if (GetRect().right <= nSliderPos)
		{
			nSliderPos = GetRect().right;
		}
		m_fSliderlPercent = (double)(nSliderPos - GetRect().left) / (double)GetRect().Width();
		_NotifySliderChanged();
	}

	Invalidate();
}

BOOL CRXSkinSlider::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	// 	LOG_INFO(_T("OnMouseWheel:nFlags:%d；zDelta：%d；x:%d；y:%d"),
	// 		nFlags,zDelta,pt.x,pt.y);
	if (zDelta < 0)
	{
		Step(SLIDER_STEP);
	}
	else
	{
		Step(-SLIDER_STEP);
	}
	return TRUE;
}