#pragma once
#include "rxskin.h"
#include "rxskinitem.h"
#include "rxcontrol.h"
#include "fundation/rxstring.h"
#include "main/rxskinwnd.h"
#include <map>
using namespace std;
class CRXSkinLocationPicker;

class RXSKIN_API CRXSkinLocationPickerPopup : public CRXSkinWnd
{
public:
	CRXSkinLocationPickerPopup();
	~CRXSkinLocationPickerPopup();

	virtual BOOL Create(const TCHAR* pszWindowClassName,
		const TCHAR* pszWindowName,
		const UINT32 dwStyle,
		const HWND hParentWnd,
		HICON hIcon = NULL);

	VOID      Show(const BOOL bShowed,POINT pt);
	CRXSkinString GetText();
	VOID      SetDataPicker(CRXSkinLocationPicker* poPicker);

	VOID      SetCountry(const TCHAR* pszCountry);
	VOID      SetProvince(const TCHAR* pszProvince);
	VOID      SetCity(const TCHAR* pszCity);
private:
	CRXSkinLocationPicker* m_poPicker;
	CRXControlMgr m_oControlMgr;
	CRXControl*     m_poMouseOn;
	CRXControl*     m_poPressed;
	CRXSkinString   m_strCountry;
	CRXSkinString   m_strProvince;
	CRXSkinString   m_strCity;
protected:
	virtual VOID  OnPaint();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL  OnMouseMove(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL  OnMessage(UINT32 dwMessageID,WPARAM wParam, LPARAM lParam);
	VOID      _OnCountryChanged();
	VOID      _OnProvinceChanged();
};


class RXSKIN_API CRXSkinLocationPicker: public CRXControl
{
public:
	CRXSkinLocationPicker(void);
	~CRXSkinLocationPicker(void);
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
	virtual VOID SetText(const TCHAR* strTxt);
	VOID    SetCountry(const TCHAR* pszCountry);
	TCHAR*  GetCountry();

	VOID    SetProvince(const TCHAR* pszProvince);
	TCHAR*  GetProvince();

	VOID    SetCity(const TCHAR* pszCity);
	TCHAR*  GetCity();

	VOID    SetTipValue(const TCHAR* pszTipValue) { m_strTipValue = pszTipValue; };
	VOID    SetTipTextColor(const COLORREF& clr) { m_clrTipText = clr; }
	VOID    SetTipTextBG(const COLORREF& clr) { m_clrTipBG = clr; }
	bool    IsEmpty();
private:
	CRXSkinLocationPickerPopup*  m_poLocationPickerPopup;

	tstring m_strCountry;
	tstring m_strProvince;
	tstring m_strCity;

	CRXSkinString m_strTipValue;
	COLORREF  m_clrTipText;
	COLORREF  m_clrTipBG;
};
