#pragma once
#include "rxskin.h"
#include "rxcontrol.h"
#include <vfw.h>
class RXSKIN_API CRXSkinHotkeyCtrl : public CRXControl
{
public:
	CRXSkinHotkeyCtrl(void);
	~CRXSkinHotkeyCtrl(void);
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
	HWND    GetEditWnd();
	CRXSkinString GetWindowText();
	virtual VOID  SetText(const TCHAR* strTxt);
	virtual VOID Enable(const BOOL bEnabled);
	BOOL    Create();
	VOID    SetBorder(const BOOL bBorder);
	BOOL    IsBorder();

	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void    OnFinalMessage(HWND hWnd);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(WPARAM /*wParam*/, LPARAM /*lParam*/);
	virtual LRESULT OnKeyUp(WPARAM /*wParam*/, LPARAM /*lParam*/);
	virtual LRESULT OnChar(WPARAM /*wParam*/, LPARAM /*lParam*/);
private:
	HWND    m_hEditWnd;
	BOOL    m_bBoder;
	WNDPROC m_OldWndProc;
	HBRUSH  m_hBkBrush;
	BOOL    m_bFinished;
	UINT32  m_dwVirtualKeyCode;
	UINT32  m_dwModifiers;
private:
	static LRESULT CALLBACK _WndHotkeyProc(HWND hWnd,
		UINT message,
		WPARAM        wParam,
		LPARAM        lParam);

	CRXSkinString     _GetKeyText();
};
