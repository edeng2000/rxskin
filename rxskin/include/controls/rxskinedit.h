#pragma once
#include "rxskin.h"
#include "rxcontrol.h"
#include "main/rxskinwnd.h"
#include "main/rxskineditwnd.h"


class CRXSkinEdit;


class RXSKIN_API CRXSkinEdit: public CRXControl
{
public:
	CRXSkinEdit(void);
	~CRXSkinEdit(void);
	BOOL    Create(const UINT32 dwID,CRXSkinRect rcClient,const BOOL bIsPassword);
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
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
	virtual HBRUSH OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC);
	virtual VOID SetVisible(const BOOL bIsVisible);
	virtual VOID Enable(const BOOL bEnabled);
	virtual void SetFocus();
	virtual void ModifyRect();
	HWND    GetEditWnd();
	CRXSkinString GetWindowText();
	virtual VOID  SetText(const TCHAR* strTxt);
	virtual TCHAR* GetText();
	virtual VOID OnEditSetFocus(HWND hWnd);
	virtual VOID OnEditKillFocus(HWND hWnd);
	virtual VOID OnEditChanged(HWND hWnd);
	virtual LRESULT OnEditChanged(WPARAM wParam, LPARAM lParam);
	int32_t GetTextValueInt();
	uint32_t GetTextStyle() { return m_dwTextStyle; };
	void    SetTextStyle(const uint32_t& dwStyle) { m_dwTextStyle = dwStyle; }
	void    SetIntValue(const int32_t& nValue);
	void    Destroy();
	void    OffsetPos(int32_t nX, int32_t nY);
	void    MoveWindow(const CRXSkinRect& rcItem);
	VOID    SetTipValue(const TCHAR* pszTipValue) { m_strTipValue = pszTipValue; };
	VOID    SetTipTextColor(const COLORREF& clr) { m_clrTipText = clr; }
	VOID    SetTipTextBG(const COLORREF& clr) { m_clrTipBG = clr; }
	VOID    SetReadOnly(const BOOL& bIsOnly) { m_bIsReadOnly = bIsOnly; }
	VOID    SetIsMultLine(const bool& bIsMultLine) { m_bIsMultLine = bIsMultLine; }
	void    SetBkColor(const COLORREF& clr) { m_clrEditBKColor = clr; }
	virtual void    SetTextColor(const COLORREF& clr) { m_clrEditTextColor = clr; }
	virtual COLORREF GetTextColor() { return m_clrEditTextColor; }
	virtual COLORREF GetBkColor() { return m_clrEditBKColor; }
	void    SetBorderColor(const COLORREF& clr) {
		m_clrBorder = clr; 
		m_wndEdit.SetBorderColor(m_clrBorder);
	}
	COLORREF GetBorderColor() { return m_clrBorder; }
	void     OnReturn();
	void    SetListener(IRXSkinEditListener* poListener) { m_poListener = poListener; }
	virtual bool    IsPassword() { return m_bIsPassword; }
	virtual HBITMAP GetBKBitmap() { return m_hBmpEditBk; }
	virtual void    ReleaseBKBitmap() {
		::DeleteObject(m_hBmpEditBk);
		m_hBmpEditBk = NULL;
	}

	void   SetIsReadyOnly();

	void   SetFontName(const TCHAR* pszFontName) { m_strFontName = pszFontName; }
	void   SetFontSize(const uint32_t& dwSize) { m_dwFontSize = dwSize; }
	void   SetIsBold(const BOOL& bIsBold) { m_bIsBold = bIsBold; }
	void   SetRename() { m_bRename = true; }
protected:
	CRXSkinEditWnd  m_wndEdit;
	CRXSkinString m_strTipValue;
	COLORREF  m_clrTipText;
	COLORREF  m_clrTipBG;
	bool    m_bIsPassword;
	bool    m_bIsReadOnly;
	bool    m_bIsMultLine;
	bool    m_bRename;
	CRXSkinString m_strContent;
	bool m_bDrawCaret;
	COLORREF  m_clrEditBKColor;
	COLORREF  m_clrEditTextColor;
	COLORREF  m_clrBorder;
	IRXSkinEditListener* m_poListener;
	uint32_t  m_dwTextStyle;
	HBITMAP	  m_hBmpEditBk;
	bool      m_bFocus;
	tstring   m_strFontName;
	BOOL      m_bIsBold;
	uint32_t  m_dwFontSize;
	HFONT     m_hFont;
	tstring   m_strText;
protected:
	VOID    _ShowEdit();
	VOID    _DeleteEdit();
};
