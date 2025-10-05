#ifndef _RXSKIN_BASE_WND_INCLUDE_
#define _RXSKIN_BASE_WND_INCLUDE_

#include "RXSkin.h"
#include "common/rxskindefines.h"
#include "controls/rxcontrol.h"
#include "common/rxskinutil.h"
#include "fundation/tinyxml2.h"
#include "core/SkSurface.h"


class RXSKIN_API CRXSkinWnd
{
public:

	struct STimerEvent
	{
		uint32_t  dwEventID;
		uint32_t  dwControlID;
		uint32_t  dwInterval;
		STimerEvent()
		{
			dwInterval = 0;
			dwEventID = 0;
			dwControlID = 0;
		}

		STimerEvent(const STimerEvent& oEvent)
		{
			dwControlID = oEvent.dwControlID;
			dwEventID = oEvent.dwEventID;
			dwInterval = oEvent.dwInterval;
		}
	};

	CRXSkinWnd();
	~CRXSkinWnd();
	virtual VOID Destroy();
	virtual BOOL Create(const TCHAR* pszWindowClassName,
		const TCHAR* pszWindowName,
		const UINT32 dwStyle,
		const HWND hParentWnd,
		HICON hIcon = NULL);
	virtual VOID  OnDraw(SkSurface* poSurface,HDC hDC, RECT rcClient);
	virtual BOOL  OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL  OnSize(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnSizing(WPARAM wParam, LPARAM lParam);
	virtual void  OnFinalMessage(HWND hWnd);
	virtual BOOL  OnMouseMove(UINT nFlags, POINT point);
	virtual BOOL  OnMouseHover(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonDblClk(UINT nFlags, POINT point);
	virtual BOOL  OnMButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnMButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnMButtonDblClk(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonDblClk(UINT nFlags, POINT point);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysChar(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysKeydown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam);
	virtual VOID  OnActive(WPARAM nState,HWND hWnd);
	virtual BOOL  OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void  OnCancelMode();
	virtual BOOL  OnMessage(UINT32 dwMessageID,WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnTimer(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnShowWindow(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnMove(int x, int y);
	virtual HBRUSH  OnCtlColorControl(uint32_t&dwMsgID,HWND hWndChild,HDC hDC);
	virtual LRESULT OnEditChanged(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL    OnPreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	virtual VOID  OnPaint();
	virtual BOOL  OnMouseWheel(UINT nFlags, short zDelta,POINT pt);
	virtual VOID  OnScrollBarChanged(EScrollType enType,INT32 nPercent);
	virtual LRESULT OnSystemCommand(WPARAM wParam, LPARAM lParam);
	virtual CRXControl* OnCreateControl(tinyxml2::XMLElement* poElement);
	virtual void    AddAnchor(HWND hWnd, CRXSkinResizePoint ptTopLeft, CRXSkinResizePoint ptBottomRight);
	bool    RegisterSuperclass(const TCHAR* pszClassName);
	HDC     GetPaintDC() { return m_hDcPaint; }
	bool    IsDCReady() {
		return m_bDCReady;
	}
	bool    IsValid();
	HWND    GetHWnd();
	BOOL    IsSizing() { return m_bSizing; }
	HWND    GetParentWnd();
	CRXSkinRect  GetRect();
	CRXSkinRect  GetClientRectEx();
	CRXSkinRect  GetWindowRectEx();
	void    Invalidate();
	void    MoveWindowEx(RECT rc);
	CRXControl* HitTest(POINT pt);
	LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

	void    SendMessage(const UINT32& dwMsgID, const WPARAM& wParam = NULL, const LPARAM& lParam = NULL);
	bool    IsLayered() { return m_bLayered; }
	void    SetCapture();
	void    ReleaseCapture();
	CRXControlMgr& GetControlMgr();
	bool    SetTimer(CRXControl* poControl, int nEventID, int nTimeOut);
	void    KillTimer(CRXControl* poControl, int nEventID);
	void    Show();
	void    Hide();
	bool    IsVisible();
	bool    IsAutoDelete();
	void    SetAutoDelete(const bool& bAutoDelete);
	vector<STimerEvent>* FindTimer(const uint32_t& dwEventID);
	void   RemoveTimer(const uint32_t& dwEventID,const uint32_t& dwControlID);
	void   SetForgeWindow();
	void   SetTopmostWindow();
private:
	static LRESULT CALLBACK _WndProc(HWND hWnd,
		UINT message,
		WPARAM        wParam,
		LPARAM          lParam);
protected:
	HWND    m_hWnd;
	HWND    m_hParentWnd;
	WNDPROC m_OldWndProc;
	HDC     m_hDcPaint;
	bool    m_bDCReady;
	bool	m_bLayered;
	BOOL    m_bSizing;
	bool    m_bAutoDelete;
	CRXControlMgr m_oControlMgr;
	map<uint32_t, vector<STimerEvent>>  m_mapTimer;

};

#endif