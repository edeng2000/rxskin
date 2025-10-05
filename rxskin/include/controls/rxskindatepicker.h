#pragma once
#include "rxskin.h"
#include "rxskinitem.h"
#include "rxcontrol.h"
#include "main/rxskinwnd.h"
#include "fundation/rxstring.h"
#include <map>
using namespace std;
class CRXSkinDatePicker;
class RXSKIN_API CRXSkinDatePickerPopup : CRXSkinWnd
{
public:
	CRXSkinDatePickerPopup();
	~CRXSkinDatePickerPopup();

	virtual BOOL Create(const TCHAR* pszWindowClassName,
						const TCHAR* pszWindowName,
						const UINT32 dwStyle,
						const HWND hParentWnd,
						HICON hIcon = NULL);
	VOID      Show(const BOOL bShowed,POINT pt);
	CRXSkinString GetText();
	VOID      SetYear(const TCHAR* pszYear);
	VOID      SetMonth(const TCHAR* pszMonth);
	VOID      SetDay(const TCHAR* pszDay);
	VOID      SetDataPicker(CRXSkinDatePicker* poPicker);
private:
	CRXSkinDatePicker* m_poPicker;
	CRXControl*     m_poMouseOn;
	CRXControl*     m_poPressed;

	CRXSkinString   m_strYear;
	CRXSkinString   m_strMonth;
	CRXSkinString   m_strDay;
protected:
	virtual VOID  OnPaint();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL  OnMouseMove(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL  OnMessage(UINT32 dwMessageID,WPARAM wParam, LPARAM lParam);
};


class RXSKIN_API CRXSkinDatePicker: public CRXControl
{
public:
	CRXSkinDatePicker(void);
	~CRXSkinDatePicker(void);
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
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
private:
	CRXSkinDatePickerPopup*  m_poDatePickerPopup;

};
