#pragma once
#include "rxskin.h"
#include "rxcontrol.h"

class CRXSkinDialog;
class RXSKIN_API CRXSkinChild : public CRXControl
{
public:
	CRXSkinChild(void);
	~CRXSkinChild(void);
	virtual CRXControl* HitTest(POINT pt);
	virtual VOID     SetVisible(const BOOL bIsVisible);
	virtual CRXSkinRect    GetInvalidateRect();
	virtual VOID OnMouseLeave();
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt);
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt);
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnLButtonDblClk(POINT pt);
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
	virtual BOOL OnTimer(WPARAM wParam, LPARAM lParam);
	virtual CRXControl*      GetInvalidateControl();
	virtual BOOL Create(CRXSkinDialog* poParent,const TCHAR* pszConfigFile);
	CRXControlMgr& GetControlMgr();
	virtual HBRUSH OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC);
	virtual BOOL  OnMouseWheel(UINT nFlags, short zDelta,POINT pt);
	virtual VOID  OnScrollBarChanged(EScrollType enType,INT32 nPercent);
	virtual void  OffsetRect(int32_t nX, int32_t nY);
private:
	CRXSkinDialog*		m_poParent;
	CRXControlMgr   m_oControlMgr;
	CRXSkinRect     m_rcInvalidate;
	CRXControl*     m_poInvalidateControl;
private:
	BOOL    _ReadConfig(const TCHAR* pszConfig);
};
