#pragma once
#include "rxskin.h"
#include "controls/rxcontrol.h"
#include "main/rxskinwnd.h"


class RXSKIN_API CRXSkinEditWnd :public CRXSkinWnd
{
public:
	CRXSkinEditWnd();
	~CRXSkinEditWnd();
	virtual BOOL Create(const TCHAR* pszWindowClassName,
		const TCHAR* pszWindowName,
		const UINT32 dwStyle,
		const HWND hParentWnd,
		HICON hIcon = NULL);
	virtual VOID Destroy();
	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysChar(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysKeydown(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnCreate(LPCREATESTRUCT lpCreate);
	void SetOwner(CRXControl* pOwner) { m_pOwner = pOwner; };
	RECT CalPos();
	void EnableWindow(const BOOL& bEnable);
	void OnFinalMessage(HWND hWnd);
	virtual HBRUSH  OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC);
	LRESULT OnHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void    SetText(const TCHAR* pszText);
	int     GetIntValue();
	tstring GetTextValue();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID  OnPaint();
	void    SetBorderColor(const COLORREF& clr) { m_clrBorder = clr; }
	COLORREF GetBorderColor() { return m_clrBorder; }
	void    SetFont(HFONT hFont) { m_hFont = hFont; }
	void    SetIsReadOnly(const BOOL& bIsReadOnly);
protected:
	enum {
		DEFAULT_TIMERID = 20,
	};

	CRXControl* m_pOwner;
	HBRUSH m_hBkBrush;
	bool m_bInit;
	tstring m_strText;
	bool m_bDrawCaret;
	COLORREF  m_clrBorder;
	HFONT  m_hFont;

	BOOL   m_bIsReadOnly;
};