#pragma once
#include "rxskin.h"
#include "rxcontrol.h"
#include "main/rxskineditwnd.h"
enum EBrowserType
{
	BROWSER_SELECT_FILE = 0,
	BROWSER_SELECT_DIRECTORY,
};


class RXSKIN_API CRXSkinBrowser : public CRXControl
{
public:
	CRXSkinBrowser(void);
	~CRXSkinBrowser(void);
	BOOL    Create(const UINT32 dwID, CRXSkinRect rcClient, const BOOL bIsPassword);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID OnEditSetFocus(HWND hWnd);
	virtual VOID OnEditKillFocus(HWND hWnd);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt);
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnSize(const UINT32 dwX, const UINT32 dwY);
	virtual HBRUSH OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC);
	virtual VOID SetVisible(const BOOL bIsVisible);
	virtual VOID Enable(const BOOL bEnabled);
	virtual VOID SetRect(const CRXSkinRect rcItem);
	virtual void  ModifyRect();
	HWND    GetEditWnd();
	CRXSkinString GetWindowText();
	virtual VOID  SetText(const TCHAR* strTxt);
	void    Destroy();
	void    MoveWindow(const CRXSkinRect& rcItem);
	VOID    SetTipValue(const TCHAR* pszTipValue) { m_strTipValue = pszTipValue; };
	VOID    SetTipTextColor(const COLORREF& clr) { m_clrTipText = clr; }
	VOID    SetTipTextBG(const COLORREF& clr) { m_clrTipBG = clr; }
	VOID    SetReadOnly(const BOOL& bIsOnly) { m_bIsReadOnly = bIsOnly; }
	VOID    SetIsMultLine(const bool& bIsMultLine) { m_bIsMultLine = bIsMultLine; }
	void    SetBkColor(const COLORREF& clr) { m_clrEditBKColor = clr; }
	void    SetTextColor(const COLORREF& clr) { m_clrEditTextColor = clr; }
	COLORREF GetTextColor() { return m_clrEditTextColor; }
	COLORREF GetBkColor() { return m_clrEditBKColor; }
	void    SendMessage(UINT uMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0L */);
	void    SetBorderColor(const COLORREF& clr) { m_clrBorder = clr; }
	COLORREF GetBorderColor() { return m_clrBorder; }
	virtual void  Paint(HDC hDC, RECT rc);

	void    SetFileExt(const TCHAR* pszExt);
	virtual HBITMAP GetBKBitmap() { return m_hBmpEditBk; }
	virtual void    ReleaseBKBitmap() {
		::DeleteObject(m_hBmpEditBk);
		m_hBmpEditBk = NULL;
	}
	void    SetBrowserType(const EBrowserType& enType) { m_enType = enType; }
	void    SetIsReadyOnly();
protected:
	CRXSkinEditWnd    m_wndEdit;
	HBRUSH  m_hBkBrush;
	CRXSkinString m_strTipValue;
	COLORREF  m_clrTipText;
	COLORREF  m_clrTipBG;
	bool    m_bIsPassword;
	bool    m_bIsReadOnly;
	bool    m_bIsMultLine;
	CRXSkinString m_strContent;
	bool m_bDrawCaret;
	COLORREF  m_clrEditBKColor;
	COLORREF  m_clrEditTextColor;
	COLORREF  m_clrBorder;
	CRXSkinRect m_rcSelect;
	uint32_t  m_dwTextStyle;
	HBITMAP	  m_hBmpEditBk;
	TCHAR     m_szFileExt[MAX_PATH];
	EBrowserType  m_enType;
	bool      m_bFocus;
protected:
	VOID    _ShowEdit();
	VOID    _OnClickSelect();
	VOID    _DeleteEdit();
};
