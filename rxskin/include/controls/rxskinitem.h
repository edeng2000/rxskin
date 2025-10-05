#pragma once
#include <vector>
#include <map>
#include "RXSkin.h"
#include "common/imagemgr.h"
#include "common/rxskinutil.h"

using namespace std;
enum EItemState
{
	ITEM_NORMAL = 0,
	ITEM_MOUSEON,
	ITEM_PRESSED,
	ITEM_SELECTED,
};
class RXSKIN_API CRXSkinItem
{
public:

	CRXSkinItem();
	~CRXSkinItem();
	
	virtual VOID  SetState(EItemState enState);
	EItemState    GetState();

	CRXSkinRect   GetRect();
	virtual VOID  SetRect(CRXSkinRect rcItem);

	VOID          SetID(const uint64_t dwID);
	uint64_t      GetID();

	VOID          SetParentWnd(HWND hParentWnd);
	HWND          GetParentWnd();

	inline TCHAR* GetText() { return m_szText; }
	virtual void   SetText(const TCHAR* pszTxt)
	{
		memset(m_szText, 0, sizeof(TCHAR)*256);
		_RXTStrcpy(m_szText,pszTxt);
	}

	inline TCHAR* GetTooltip() { return m_szTootip; }
	inline void   SetTooltip(const TCHAR* pszTooltip)
	{
		memset(m_szTootip, 0, sizeof(TCHAR) * 512);
		_RXTStrcpy(m_szTootip, pszTooltip);
	}

	BOOL          IsSelected() { return m_enState == ITEM_SELECTED; }

	virtual VOID  Enable(const BOOL bEnabled);
	virtual BOOL  IsEnabled();
	virtual CRXSkinString GetClassType();
	virtual void  Invalidate();
	BOOL             IsVisible();
	virtual VOID     SetVisible(const BOOL bIsVisible);
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)=0;
	virtual CRXSkinItem* HitTest(POINT pt)=0;
private:
	uint64_t        m_dwID;
	CRXSkinRect     m_rcItem;
	EItemState		m_enState;
	BOOL            m_bEnabled;
	HWND            m_hParentWnd;
	BOOL            m_bVisible;
	TCHAR           m_szText[256];
	TCHAR           m_szTootip[512];
};
