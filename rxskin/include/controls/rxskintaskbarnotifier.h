#pragma once

#include "rxskin.h"
#include "rxskinitem.h"
#include "rxcontrol.h"
#include "main/rxskindialog.h"
#include <map>
#include "tchar.h"
#include "common/rxskindefines.h"
using namespace std;

class RXSKIN_API CRXSkinTaskbarNotifier : public CRXSkinDialog
{
public:
	CRXSkinTaskbarNotifier(void);
	~CRXSkinTaskbarNotifier(void);
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL  OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual VOID  OnClickedClose();
	virtual BOOL  OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void  OnFinalMessage(HWND hWnd);
	virtual BOOL  OnTimer(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnMouseMove(UINT nFlags, POINT point);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnMouseHover(UINT nFlags, POINT point);
public:
	bool    Show(const TCHAR* pszWindowName,
		const TCHAR* pszContent,
		const EMessageBoxType& enType ,
		UINT32 dwTimeToShow = 500,
		UINT32 dwTimeToStay = 3000,
		UINT32 dwTimeToHide = 500, 
		INT32 nIncrement = 1);
	void    Hide();
	void    SetID(const UINT32& dwID) { m_dwID = dwID; }
	void    SetSendWnd(const HWND& hSendWnd) { m_hSendWnd = hSendWnd; }
	void    SetAutoClose(const bool& bAutoClose) { m_bAutoClose = bAutoClose; }
	void    SetConfigFile(const TCHAR* pszConfigFile) { m_strConfigFile = pszConfigFile; }
private:
	VOID    _OnClickedOK();
	VOID    _OnClickedCancel();
private:
	UINT32 m_dwTimeToShow;
	UINT32 m_dwTimeToLive;
	UINT32 m_dwTimeToHide;
	UINT32 m_dwDelayBetweenShowEvents;
	UINT32 m_dwDelayBetweenHideEvents;
	INT32  m_nIncrement;
	INT32  m_nAnimStatus;
	INT32  m_nStartPosX;
	INT32  m_nStartPosY;
	INT32  m_nCurrentPosX;
	INT32  m_nCurrentPosY;
	INT32  m_nTaskbarPlacement;
	BOOL   m_bPaused;
	tstring m_strTitle;
	tstring m_strConfigFile;
	tstring m_strContent;
	bool    m_bAutoClose;
	UINT32  m_dwID;
	INT32               m_nOffsetX;
	INT32               m_nOffsetTop;
	INT32               m_nOffsetBottom;
	UINT32              m_dwContentWidth;
	EMessageBoxType m_enTipType;
	HWND   m_hSendWnd;
};
