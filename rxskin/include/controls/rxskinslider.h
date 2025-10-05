#pragma once
#include "rxskin.h"
#include "rxcontrol.h"
#include "rxskinitem.h"
#include "common/rxskindefines.h"
#include <map>
using namespace std;


class RXSKIN_API CRXSkinSlider : public CRXControl
{
public:

	CRXSkinSlider(void);
	~CRXSkinSlider(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt);
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	virtual BOOL OnSize(const UINT32 dwX, const UINT32 dwY);
	virtual VOID SetRect(const CRXSkinRect rcItem);
	VOID    Step(INT32 nStep);
	VOID    Reset();
	VOID    SetType(ESliderType enType);
	void    SetPercent(float fPercent);
private:
	VOID    _DrawVer(HDC hDC);
	VOID    _DrawHor(HDC hDC);
	VOID    _NotifySliderChanged();
	void    _SetPos(POINT pt);
private:
	ESliderType  m_enType;
	double 		 m_fSliderlPercent;
	BOOL          m_bPressed;
};
