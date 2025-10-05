#ifndef _RXSKIN_TOOLTIP_H_
#define _RXSKIN_TOOLTIP_H_
#include "RXSkin.h"
#ifndef TTS_NOANIMATE
	#define TTS_NOANIMATE           0x10
	#define TTS_NOFADE              0x20
	#define TTS_BALLOON             0x40
	#define TTS_CLOSE               0x80

	#define TTM_SETTITLEA           (WM_USER + 32)  // wParam = TTI_*, lParam = char* szTitle
	#define TTM_SETTITLEW           (WM_USER + 33)  // wParam = TTI_*, lParam = wchar* szTitle

	#ifdef UNICODE
	#define TTM_SETTITLE            TTM_SETTITLEW
	#else
	#define TTM_SETTITLE            TTM_SETTITLEA
	#endif
#endif

#include "main/rxskinwnd.h"
#define SHOW_TOOLTIP_INTERVAL_TIME     10000
#define HIDE_TOOLTIP_TIMER_ID          100

class RXSKIN_API CRXSkinToolTip : public CRXSkinWnd
{
  public:
	CRXSkinToolTip();
	~CRXSkinToolTip();
	BOOL Create(const TCHAR* pszWindowClassName,
		const TCHAR* pszWindowName,
		const UINT32 dwStyle,
		const HWND hParentWnd,
		HICON hIcon = NULL);
	void ShowToolTip(BOOL bShow);
	void SetToolTipPosition(POINT pt);
	void UpdateToolTipText(LPCTSTR szBuff, HINSTANCE hInstance = 0);
	virtual void  OnFinalMessage(HWND hWnd);
	virtual BOOL  OnTimer(WPARAM wParam, LPARAM lParam);
private:
		HWND m_hParent;
		TCHAR m_szToolTip[256];
private:
	virtual VOID    OnPaint();
	virtual BOOL    OnCreate(LPCREATESTRUCT lpCreate);
};

#endif _RXSKIN_TOOLTIP_H_