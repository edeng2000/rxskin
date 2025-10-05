#pragma once
#include "rxskin.h"
#include "rxcontrol.h"
class RXSKIN_API CRXSkinProgress : public CRXControl
{
public:
	CRXSkinProgress(void);
	~CRXSkinProgress(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual VOID SetVisible(const BOOL bIsVisible);
	virtual BOOL OnTimer(WPARAM wParam, LPARAM lParam);
	virtual void SetRect(const CRXSkinRect rcItem);
	void  SetRound(const SIZE& sz) { m_szRound = sz; }
	void  SetBorderColor(COLORREF clr) { m_clrBorder = clr; }
	void  SetHotColor(COLORREF clr) { m_clrHot = clr; }
	void  SetBKColor(COLORREF clr) { m_clrBK = clr; }
	void  SetPercent(const uint32_t& dwPercent);
private:

	SIZE  m_szRound;
	COLORREF m_clrBorder;
	COLORREF m_clrHot;
	COLORREF m_clrBK;
	uint32_t m_dwPercent;
};
