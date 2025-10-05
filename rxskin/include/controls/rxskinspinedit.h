#pragma once
#include "rxskin.h"
#include "rxskinitem.h"
#include "rxcontrol.h"
#include <map>
#include "tchar.h"
#include "common/rxskindefines.h"
using namespace std;


class RXSKIN_API CRXSkinSpinButton : public CRXSkinItem
{
public:
	CRXSkinSpinButton();
	~CRXSkinSpinButton();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);
	VOID    SetType(ESpinType enType);
	ESpinType GetType();
private:
	ESpinType     m_enType;
private:
	VOID    _DrawTop(HDC hDC);
	VOID    _DrawBottom(HDC hDC);
};


class RXSKIN_API CRXSkinSpinEdit: public CRXControl
{
public:
	CRXSkinSpinEdit(void);
	~CRXSkinSpinEdit(void);
	BOOL    Create();
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
	virtual VOID SetVisible(const BOOL bIsVisible);
	virtual VOID Enable(const BOOL bEnabled);
	HWND    GetEditWnd();
	CRXSkinString GetWindowText();
	virtual VOID  SetText(const TCHAR* strTxt);
private:
	HWND    m_hEditWnd;
	CRXSkinSpinButton  m_btnBottom;
	CRXSkinSpinButton  m_btnTop;
};
