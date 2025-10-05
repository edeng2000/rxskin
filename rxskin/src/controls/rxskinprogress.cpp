#include "pch.h"
#include "controls\rxskinprogress.h"
#include "common\RXSkinService.h"
#include "common\graphics.h"
#include "common\RXSkinMemDC.h"
#include "common\rxskinutil.h"
#include "rxlogger.h"
#include "rxtime.h"
#include <wingdi.h>

CRXSkinProgress::CRXSkinProgress(void)
{
	m_clrHot = CRXSkinService::Instance()->GetDrawMgr().GetProgressHot();
	m_szRound = CRXSkinService::Instance()->GetDrawMgr().GetProgressRound();
	m_clrBorder = CRXSkinService::Instance()->GetDrawMgr().GetProgressBorder();
	m_clrBK = CRXSkinService::Instance()->GetDrawMgr().GetProgressBK();
	m_dwPercent = 0;
}

CRXSkinProgress::~CRXSkinProgress(void)
{
}

VOID CRXSkinProgress::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (IsVisible() == FALSE)
	{
		return;
	}
	DrawRoundRect(hDC,m_clrBorder,m_clrBK,rcItem,m_szRound.cx,m_szRound.cy);
	float fRate = (float)m_dwPercent / 100.0f;
	uint32_t dwWidth = (uint32_t)((rcItem.Width()-2) * fRate);
	if (dwWidth > 0)
	{
		HRGN hRgn = CreateRoundRectRgn(rcItem.left + 1,
			rcItem.top + 1,
			rcItem.left + 1 + dwWidth,
			rcItem.bottom - 1,
			m_szRound.cx,
			m_szRound.cy);
		DrawGradientRgn(hDC, TRUE, hRgn, m_clrHot, m_clrHot);
		::DeleteObject(hRgn);
	}


	TCHAR szTxt[256] = {0};
	_RXTsprintf(szTxt, _T("%d%%"), m_dwPercent);
	CRXSkinRect rcTxt = rcItem;
	tstring strTxt = szTxt;
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, strTxt, rcTxt, GetTextColor(), ID_FONT_COMMON,
			DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	}
}

VOID CRXSkinProgress::ReleaseControl()
{
	delete this;
}
BOOL CRXSkinProgress::PtInRect(POINT pt)
{
	if (IsVisible() && IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}

BOOL CRXSkinProgress::OnClick(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinProgress::OnMouseMove(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinProgress::OnLButtonDown(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinProgress::OnTimer(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

void CRXSkinProgress::SetRect(const CRXSkinRect rcItem)
{
	CRXControl::SetRect(rcItem);
}


VOID CRXSkinProgress::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
}

void CRXSkinProgress::SetPercent(const uint32_t& dwPercent)
{
	m_dwPercent = dwPercent;
	Invalidate();
}