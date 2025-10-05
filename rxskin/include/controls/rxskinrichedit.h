#pragma once
#include "rxskin.h"
#include "rxcontrol.h"
#include "rxskinricholeobject.h"
#include "rxskinrichtexthost.h"

class CRXSkinRichEdit;

class RXSKIN_API CRXSkinRichEditBody :public CRXSkinChildWnd
{
public:
	struct MemBlock
	{
		LPBYTE pBuf;
		uint32_t dwLen;
		uint32_t dwCurPos;
		MemBlock()
		{
			dwCurPos = 0;
			dwLen = 0;
			pBuf = NULL;
		}
		MemBlock(const MemBlock& oMem)
		{
			if (pBuf)
			{
				free(pBuf);
				pBuf = NULL;
			}
			pBuf = (LPBYTE)malloc(oMem.dwLen);
			memcpy(pBuf, oMem.pBuf, oMem.dwLen);
			dwLen = oMem.dwLen;
			dwCurPos = oMem.dwCurPos;
		}
		~MemBlock()
		{
			if (pBuf)
			{
				free(pBuf);
				pBuf = NULL;
			}
		}
	};
	CRXSkinRichEditBody();
	~CRXSkinRichEditBody();
	virtual BOOL Create(const TCHAR* pszWindowClassName,
		const TCHAR* pszWindowName,
		const UINT32 dwStyle,
		const HWND hParentWnd,
		HICON hIcon = NULL);
	virtual BOOL  OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnMouseMove(UINT nFlags, POINT point);
	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysChar(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysKeydown(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnLButtonDblClk(UINT nFlags, POINT point);
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL  OnTimer(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
	virtual VOID OnScrollBarChanged(EScrollType enType, INT32 nPercent);
	virtual BOOL  OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual VOID OnClickedClose();
	virtual BOOL  OnSize(WPARAM wParam, LPARAM lParam);
	virtual CRXControl* OnCreateControl(tinyxml2::XMLElement* poElement);
	virtual BOOL  OnMessage(UINT32 dwMessageID, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void   SetOwner(CRXSkinRichEdit* poOwner) { m_poOwner = poOwner; }
	void   SetText(const BYTE* pszText,const uint32_t&dwLen);
	MemBlock& GetMemory() {
		return m_oMem;
	}
	BOOL   EnableScrollbar(INT fuSBFlags);
	RECT   GetEditRect();
private:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CComPtr<MyOLEObject> GetOLEObjectAtMouseCursor();
private:
	CRXSkinRichEdit* m_poOwner;
	CRXSkinTextHost* m_poTextHost;
	MemBlock m_oMem;
	INT m_nfuSBFlags;
	CRXSkinScrollbar  m_oScrollbarVer;
	CRXSkinScrollbar  m_oScrollbarHor;
private:
};

class RXSKIN_API CRXSkinRichEdit : public CRXControl
{
public:
	CRXSkinRichEdit(void);
	~CRXSkinRichEdit(void);
	BOOL    Create(const UINT32 dwID, CRXSkinRect rcClient, const BOOL bIsPassword);
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
	virtual BOOL OnSize(const UINT32 dwX, const UINT32 dwY);
	virtual HBRUSH OnCtlColorControl(HWND hWndChild, HDC hDC);
	virtual LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
	virtual VOID SetVisible(const BOOL bIsVisible);
	virtual VOID Enable(const BOOL bEnabled);
	void    ClearAll();
	HWND    GetEditWnd();
	CRXSkinString GetWindowText();
	virtual VOID  SetText(const TCHAR* strTxt);

	void    AppendText(const TCHAR* pszText);
	void    SetData(const BYTE* pszData ,const uint32_t& dwLen);
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

	void     SetBorderColor(const COLORREF& clr) { m_clrBorder = clr; }
	COLORREF GetBorderColor() { return m_clrBorder; }
	void     SetDefaultColorFont(int nSize, const TCHAR* face);
protected:
	
	HBRUSH  m_hBkBrush;
	CRXSkinString m_strTipValue;
	COLORREF  m_clrTipText;
	COLORREF  m_clrTipBG;
	bool    m_bIsPassword;
	bool    m_bIsReadOnly;
	bool    m_bIsMultLine;
	CRXSkinString m_strContent;
	COLORREF  m_clrEditBKColor;
	COLORREF  m_clrEditTextColor;
	COLORREF  m_clrBorder;
	CRXSkinRichEditBody m_wndBody;

protected:
	VOID    _ShowEdit();
	VOID    _DeleteEdit();
};
