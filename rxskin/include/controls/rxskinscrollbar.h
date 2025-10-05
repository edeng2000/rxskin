#pragma once
#include "rxskin.h"
#include "rxcontrol.h"
#include "rxskinitem.h"
#include "common/rxskindefines.h"
#include <map>
using namespace std;

enum EScrollbarItemType
{
	SCROLLBAR_ITEM_VER_TOP = 0,
	SCROLLBAR_ITEM_VER_BOTTOM,
	SCROLLBAR_ITEM_VER_TRACK,
	SCROLLBAR_ITEM_HOR_LEFT,
	SCROLLBAR_ITEM_HOR_RIGHT,
	SCROLLBAR_ITEM_HOR_TRACK,
};
class CRXSkinScrollbar;
class RXSKIN_API CRXSkinScrollbarItem : public CRXSkinItem
{
public:
	CRXSkinScrollbarItem();
	~CRXSkinScrollbarItem();
	VOID     SetType(EScrollbarItemType enType);
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);
private:
	EScrollbarItemType m_enType;
private:
	VOID     _DrawVerTop(HDC hDC);
	VOID     _DrawVerBottom(HDC hDC);
	VOID     _DrawVerTrack(HDC hDC);
	VOID     _DrawHorLeft(HDC hDC);
	VOID     _DrawHorRight(HDC hDC);
	VOID     _DrawHorTrack(HDC hDC);
};

class RXSKIN_API CRXSkinScrollbar: public CRXControl
{
public:

	typedef map<UINT32,CRXSkinItem*>            CRXSkinScrollbarItemMap;
	typedef CRXSkinScrollbarItemMap::iterator   CRXSkinScrollbarItemMapItr;


	CRXSkinScrollbar(void);
	~CRXSkinScrollbar(void);
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
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta,POINT pt);
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
	VOID    Step(INT32 nStep);
	VOID    Reset();
	VOID    SetType(EScrollType enType);
	VOID    SetTotalHeight(const UINT32 dwTotalHeight,const UINT32 dwHeight);
	VOID    SetTotalWidth(const UINT32 dwTotalWidth, const UINT32 dwWidth);
	bool    GetShow();
	void    SetShow(bool bShow);

	UINT32  GetWidth();
	UINT32  GetHeight();
	void    SetOwner(CRXControl* poOwner) { m_poOwner = poOwner; }
	int     GetScrollRange() const;
	void    SetScrollRange(int nRange);
	void	SetScrollPos(int nPos);
	void    SetStep(const INT32& nStep) { m_nStep = nStep; }
	void    PushStart();
	void    PushEnd();
private:
	VOID    _DrawVer(SkSurface* poSurface, HDC hDC);
	VOID    _DrawHor(SkSurface* poSurface, HDC hDC);
	CRXSkinItem* _HitTest(POINT pt);
	VOID    _NotifyScrollChanged();
private:
	EScrollType  m_enType;
	UINT32		 m_dwTotalHeight;
	UINT32		 m_dwTotalWidth;
	INT32        m_nScrollBarHeight;
	INT32        m_nScrollBarWidth;
	UINT32       m_dwLessHeight;
	UINT32       m_dwLessWidth;
	INT32        m_nItemWidth;
	INT32        m_nItemHeight;
	INT32        m_nStep;
	CRXSkinScrollbarItem m_oVerTop;
	CRXSkinScrollbarItem m_oVerTrack;
	CRXSkinScrollbarItem m_oVerBottom;
	CRXSkinScrollbarItem m_oHorLeft;
	CRXSkinScrollbarItem m_oHorRight;
	CRXSkinScrollbarItem m_oHorTrack;

	CRXSkinItem*  m_poMouseOn;
	CRXSkinItem*  m_poPressed;
	CRXSkinScrollbarItemMap  m_mapItems;
	BOOL          m_bPressed;
	INT32         m_nTrackOffSet;
	BOOL          m_bHorizontal;
	CRXControl*   m_poOwner;
	bool		  m_bShow;
	__int64		  m_nRange;
	__int64		  m_nScrollPos;
	float         m_fPercent;
private:
	enum
	{
		DEFAULT_SCROLLBAR_SIZE = 16,
		DEFAULT_TIMERID = 10,
	};
};
