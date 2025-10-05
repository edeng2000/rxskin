#pragma once
#include "rxskin.h"
#include "rxcontrol.h"
#include "controls/rxskinitem.h"
#include "common/rxskindefines.h"
#include "controls/rxskinhotkeyctrl.h"
#include "controls/rxskinedit.h"
#include <map>

using namespace std;
class IRXSkinListCtrlListener
{
public:
	virtual VOID   OnListCtrlLBDoubleClicked(const UINT32 dwID,const INT32 nRowID,const POINT& pt) = 0;
	virtual VOID   OnListCtrlRBClicked(const UINT32 dwID, const INT32 nRowID, const POINT& pt) = 0;
	virtual VOID   OnListCtrlCommandClicked(const UINT32 dwID, const INT32 nRowID, const INT32& dwItemID) = 0;
	virtual VOID   OnListCtrlEndDragged(const UINT32 dwFromID, const UINT32 dwToID) = 0;
	virtual VOID   OnListCtrlBeginDragged(const UINT32 dwFromID) = 0;
	virtual VOID   OnListCtrlSelectRow(UINT32 dwID,const  UINT32 dwRowID) = 0;
	virtual VOID   OnListCtrlCancelRow(UINT32 dwID) = 0;
};

enum ERXSkinListItemSortType
{
	LIST_CTRL_SORT_NONE = 0,
	LIST_CTRL_SORT_UP,
	LIST_CTRL_SORT_DOWN,
};

enum ERXSkinListItemValueType
{
	LIST_CTRL_VALUE_TYPE_INT = 0,
	LIST_CTRL_VALUE_TYPE_STRING,
};


class CRXSkinListCtrl;
class RXSKIN_API CRXSkinListCtrlItem : public CRXSkinItem, public IRXSkinEditListener
{
public:
	CRXSkinListCtrlItem();
	~CRXSkinListCtrlItem();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);
	virtual VOID   OnEditReturnOK(const UINT32 dwID);
	virtual VOID   OnEditKillFocus(const UINT32 dwID);
	virtual VOID   OnEditChanged(const UINT32 dwID);
	VOID    SetType(EListItemType enType);
	EListItemType GetType();
	VOID    SetPercent(const UINT32 dwPercent);
	UINT32  GetPercent();

	VOID    SetProgress(const UINT32 dwPercent);
	UINT32  GetProgress();

	virtual VOID  SetRect(CRXSkinRect rcItem);
	VOID    SetText(const TCHAR* pszText);
	CRXSkinString GetText();
	
	VOID    SetLasted(const BOOL bLasted);
	BOOL    IsLasted();

	VOID    CreateHotkey();

	VOID    SetVisibled(const BOOL bVisibled);
	BOOL    IsVisibled();

	VOID    AddDrawStyle(const UINT32 dwAdd);

	void    SetDrawStyle(const UINT32 dwStyle);

	VOID    SetRowSelected(const BOOL bRowSelected);
	BOOL    IsRowSelected();

	HWND    GetHotkeyWnd();
	void    SetChecked(const BOOL& bChecked);
	BOOL    IsChecked() { return m_bChecked; }

	void    SetImageID(const INT32 nImageID) { m_iImageID = nImageID; }
	INT32   GetImageID() { return m_iImageID; }
	void    SetOwner(CRXSkinListCtrl* poOwner) { m_poOwner = poOwner; }
	void    CancelRename();
	void    Rename();
	void    SetRowID(const uint32_t& dwRowID) { m_dwRowID = dwRowID; }
	uint32_t GetRowID() { return m_dwRowID; }
	tstring GetRenameValue();
	void    SetImage(IRXImage* poImage) { m_poImage = poImage; }
	void    SetImageList(CRXSkinImageList* poList);
	CRXSkinImageList* GetImageList() { return m_poImageList; }
	void    SetSortType(const ERXSkinListItemSortType& enType) { m_enSortType = enType; }
	ERXSkinListItemSortType GetSortType() { return m_enSortType; }
	void    SetCanSort(const bool& bCanSort) { m_bCanSort = bCanSort; }
	bool    IsCanSort() { return m_bCanSort; }
	void    SetIntValue(const int64_t& llValue) { m_llValue = llValue; }
	int64_t GetIntValue() { return m_llValue; }

	void    SetValueType(const ERXSkinListItemValueType& enType) { m_enValueType = enType; }
	ERXSkinListItemValueType GetValueType() { return m_enValueType; }
	void    ChangeSortModel();

	bool    CanResize() { return m_bCanResize; }
	void    SetCanResize(const bool& bCanResize) { m_bCanResize = bCanResize; }

	bool    IsResing() { return m_bResing; }
	void    SetResing(const bool& bResing) { m_bResing = bResing; }

	CRXSkinRect GetResingRect() { return m_rcResing; }
	void    SetResingRect(CRXSkinRect rc) { m_rcResing = rc; }
	uint32_t GetMinWidth() { return m_dwMinWidth; }
private:
	EListItemType m_enType;
	UINT32   m_dwPercent;
	CRXSkinString m_strText;
	BOOL     m_bLasted;
	BOOL     m_bVisibled;
	UINT32   m_dwDrawStyle;
	BOOL     m_bRowSelected;
	BOOL     m_bChecked;
	CRXSkinHotkeyCtrl* m_poHotkeyCtrl;
	INT32    m_iImageID;
	uint32_t m_dwRowID;
	CRXSkinListCtrl* m_poOwner;
	CRXSkinEdit* m_edtRename;
	ERXSkinListItemSortType m_enSortType;
	uint32_t m_dwProgress;
	IRXImage* m_poImage;
	CRXSkinImageList* m_poImageList;
	bool     m_bCanSort;
	ERXSkinListItemValueType m_enValueType;
	int64_t  m_llValue;

	//  ¸Ä±ä¿í¶È
	bool     m_bCanResize;
	uint32_t m_dwMinWidth;
	bool     m_bResing;
	CRXSkinRect m_rcResing;
private:
	VOID     _DrawColumn(SkSurface* poSurface, HDC hDC);
	VOID     _DrawText(SkSurface* poSurface, HDC hDC);
	VOID     _DrawHotkey(SkSurface* poSurface, HDC hDC);
	void     _DrawCheckBox(SkSurface* poSurface, HDC hDC);
	void     _DrawProgress(SkSurface* poSurface, HDC hDC);
	void     _DrawImage(SkSurface* poSurface, HDC hDC);
	VOID     _DrawImageButton(SkSurface* poSurface, HDC hDC);
};

class RXSKIN_API CRXSkinListCtrlRow :public CRXSkinItem
{
public:
	typedef map<UINT32,CRXSkinListCtrlItem*>         CRXSkinListCtrlRowItemMap;
	typedef CRXSkinListCtrlRowItemMap::iterator      CRXSkinListCtrlRowItemMapItr;
	CRXSkinListCtrlRow();
	~CRXSkinListCtrlRow();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);
	POINT GetWindowTopLeftPos();

	CRXSkinListCtrlItem* HitTestEx(POINT pt);
	CRXSkinListCtrlRowItemMap& GetItems();
	UINT32  GetHeight();

	BOOL    AddItem(CRXSkinListCtrlItem* poItem,const UINT32 dwPercent);
	CRXSkinListCtrlItem* FindItem(const UINT32 dwID);
	CRXSkinListCtrlItem* RemoveItem(const UINT32 dwID);
	VOID    SetVisibled(const BOOL bVisibled);
	BOOL    IsVisibled();

	VOID    SetRowSelected(const BOOL bRowSelected);
	LPARAM  GetData() { return m_lParam; }
	void    SetData(LPARAM lParam) { m_lParam = lParam; }
	BOOL    IsRowSelected();

	void    RenameItem(const UINT32& dwID);
	void    CancelRename();
	void    SetOwner(CRXSkinListCtrl* poList);
	CRXSkinListCtrl* GetOwner() { return m_poOwner; }
private:
	CRXSkinListCtrlRowItemMap  m_mapItems;
	BOOL     m_bVisibled;
	bool     m_bIsSelected;
	LPARAM   m_lParam;
	CRXSkinListCtrl* m_poOwner;
private:
	VOID    _DeleteAll();
};

class RXSKIN_API CRXSkinListCtrlDrag : public CRXSkinWnd
{
public:
	CRXSkinListCtrlDrag();
	~CRXSkinListCtrlDrag();
	BOOL Create(const TCHAR* pszWindowClassName,
		const TCHAR* pszWindowName,
		const UINT32 dwStyle,
		const HWND hParentWnd,
		HICON hIcon = NULL);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	void ShowDrag(BOOL bShow);
	void SetDragPosition(POINT pt);
	void Clear();
	uint32_t GetMaxWidth();
	uint32_t GetHeight();
	void AddRow(CRXSkinListCtrlRow* poRow);
	virtual void  OnFinalMessage(HWND hWnd);
private:
	HWND m_hParent;
	vector<CRXSkinListCtrlRow*> m_vecRow;
	
private:
	virtual VOID    OnPaint();
	virtual BOOL    OnCreate(LPCREATESTRUCT lpCreate);
};


class RXSKIN_API CRXSkinListCtrlBody:public CRXSkinChildWnd
{
public:
	enum ETimer
	{
		BEGIN_DRAGGING_TIMER_ID = 1,
	};

	CRXSkinListCtrlBody();
	~CRXSkinListCtrlBody();
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
	virtual BOOL  OnLButtonDblClk(UINT nFlags, POINT point);
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL  OnTimer(WPARAM wParam, LPARAM lParam);
	virtual VOID OnScrollBarChanged(EScrollType enType, INT32 nPercent);
	virtual BOOL  OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual VOID OnClickedClose();
	virtual BOOL  OnSize(WPARAM wParam, LPARAM lParam);
	virtual CRXControl* OnCreateControl(tinyxml2::XMLElement* poElement);
	virtual BOOL  OnMessage(UINT32 dwMessageID, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void   SetOwner(CRXSkinListCtrl* poOwner) { m_poOwner = poOwner; }
	void    UpdateScrollBar();
	VOID    ResetContent();
	LPARAM  GetRowData(const UINT32 dwRowID);
	LPARAM  GetCurFirstRowData();
	CRXSkinListCtrlRow* GetCurFirstRow();
	vector<CRXSkinListCtrlRow*>& GetAllSelectedRow() { return m_vecSelected; }
	void    RemoveRow(CRXSkinListCtrlRow* poRow);
	bool    IsSelectedEmpty() { return m_vecSelected.size() == 0; }
private:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
private:
	CRXSkinListCtrl* m_poOwner;
	CRXSkinScrollbar		  m_oScrollbar;
	INT32					  m_nTotalHeight;
	INT32					  m_nTopOffSet;
	vector<CRXSkinListCtrlRow*> m_vecSelected;
	CRXSkinListCtrlRow* m_poMouseOn;	
	CRXSkinListCtrlItem* m_poMouseOnItem;
	bool  m_bDragging;
	CRXSkinListCtrlDrag  m_wndDrag;
private:
	CRXSkinRect _GetItemRect(const UINT32 dwRowID, const UINT32 dwItemID);
	CRXSkinListCtrlRow* _HitTestRow(POINT pt);
	void					 _ComputeTotalHeight();
	virtual void  UpdateToolTip(CRXControl* poControl);
	void  UpdateToolTipEx(CRXSkinListCtrlItem* poItem);
	bool  IsSelected(const uint32_t& dwID);
	void  RemoveSelected(const uint32_t& dwID);
	void  ClearSelected();
};


class RXSKIN_API CRXSkinListCtrlHeader :public CRXSkinChildWnd
{
public:
	typedef map<UINT32, CRXSkinListCtrlItem*>         CRXSkinListCtrlHeaderItemMap;
	typedef CRXSkinListCtrlHeaderItemMap::iterator   CRXSkinListCtrlHeaderItemMapItr;
	enum ETimer
	{
		BEGIN_DRAGGING_TIMER_ID = 1,
	};

	CRXSkinListCtrlHeader();
	~CRXSkinListCtrlHeader();
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
	virtual BOOL  OnLButtonDblClk(UINT nFlags, POINT point);
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL  OnTimer(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual VOID OnClickedClose();
	virtual BOOL  OnSize(WPARAM wParam, LPARAM lParam);
	virtual CRXControl* OnCreateControl(tinyxml2::XMLElement* poElement);
	virtual BOOL  OnMessage(UINT32 dwMessageID, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void   SetOwner(CRXSkinListCtrl* poOwner) { m_poOwner = poOwner; }
	virtual CRXSkinItem* HitTest(POINT pt);
	CRXSkinListCtrlHeaderItemMap& GetColumns();

	BOOL    AddColumn(CRXSkinListCtrlItem* poItem, const UINT32 dwPercent);
	CRXSkinListCtrlItem* FindColumn(const UINT32 dwID);
	CRXSkinListCtrlItem* RemoveColumn(const UINT32 dwID);
	UINT32  GetHeight();
private:
	CRXSkinListCtrlHeaderItemMap  m_mapColumns;
private:
	VOID    _DeleteAll();
private:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
private:
	CRXSkinListCtrl* m_poOwner;
	CRXSkinListCtrlItem* m_poMouseOnItem;
	CRXSkinListCtrlItem* m_poSortedHeaderItem;
	CRXSkinListCtrlItem* m_poResingHeaderItem;
	bool  m_bDragging;
private:

};

class RXSKIN_API CRXSkinListCtrl :public CRXControl
{
public:
	typedef map<UINT32,CRXSkinListCtrlRow*>          CRXSkinListCtrlRowMap;
	typedef CRXSkinListCtrlRowMap::iterator          CRXSkinListCtrlRowMapItr;

	typedef vector<CRXSkinListCtrlRow*>				 CRXSkinListCtrlRowVec;
	typedef CRXSkinListCtrlRowVec::iterator          CRXSkinListCtrlRowVecItr;

	typedef vector<CRXSkinListCtrlRowVec>			 CRXSkinListCtrlRowGroupVec;
	typedef CRXSkinListCtrlRowGroupVec::iterator     CRXSkinListCtrlRowGroupVecItr;

	CRXSkinListCtrl(void);
	~CRXSkinListCtrl(void);
	void    CreateBody();
	void    CreateHeader();
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt);
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt);
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
	virtual VOID SetVisible(const BOOL bIsVisible);
	virtual BOOL OnSetCursor() { return FALSE; }
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	virtual BOOL OnLButtonDblClk(POINT pt);
	UINT32  AddRow(uint32_t dwGroupID);
	CRXSkinListCtrlRow*  FindRow(const UINT32 dwRowID);
	virtual VOID     SetRect(const CRXSkinRect rcItem);
	CRXSkinListCtrlItem* FindItem(const UINT32 dwRowID,const UINT32 dwItemID);
	VOID    RemoveRow(const UINT32 dwRowID);
	VOID    SetItemText(const UINT32 dwRowID,const UINT32 dwItemID,const TCHAR* pszText,EListItemType enType);
	VOID    SetItemProgress(const UINT32 dwRowID, const UINT32 dwItemID, const uint32_t& dwProgress);
	VOID    SetItemTextStyle(const UINT32 dwRowID, const UINT32 dwItemID, const UINT32& dwStyle);
	VOID    SetItemImage(const UINT32 dwRowID, const UINT32 dwItemID, const INT32& nImageID);
	VOID    SetItemImageEx(const UINT32 dwRowID,
		const UINT32 dwItemID,
		IRXImage* poImage,
		EListItemType enType = LIST_ITEM_IMAGE,
		const TCHAR* pszText=_T(""));
	INT32   GetItemImageID(const UINT32 dwRowID, const UINT32 dwItemID);
	VOID    SetItemImageList(const UINT32 dwRowID, const UINT32 dwItemID, CRXSkinImageList* poList);
	void    SetRowData(const UINT32 dwRowID, LPARAM lParam);
	LPARAM  GetRowData(const UINT32 dwRowID);
	LPARAM  GetCurFirstRowData();
	CRXSkinString GetItemText(const UINT32 dwRowID,const UINT32 dwItemID);
	UINT32  AddColumn(const TCHAR* pszText,const UINT32 dwPercent); 
	VOID    RemoveColumn(const UINT32 dwColumnID);
	CRXSkinListCtrlItem* FindColumn(const uint32_t& dwID);
	VOID    ResetContent();
	VOID    SetType(EListCtrlType enType);
	void    UpdateScrollBar();
	EListCtrlType GetType();
	void    Finished();
	VOID    SetImageList(CRXSkinImageList* poImageList);
	CRXSkinImageList* GetImageList() { return m_poImageList; }
	void    SetListener(IRXSkinListCtrlListener* poListener) { m_poListener = poListener; }
	IRXSkinListCtrlListener* GetListener() { return m_poListener; }
	uint32_t GetRowCount();
	CRXSkinListCtrlRow* GetCurFirstRow();
	CRXSkinListCtrlRowMap& GetAllRow() { return m_mapRows; }
	CRXSkinListCtrlRowGroupVec& GetAllRowVec() { return m_vecRows; }
	vector<CRXSkinListCtrlRow*>& GetAllSelectedRow() { return m_wndBody.GetAllSelectedRow(); }
	void    RenameRow(const uint32_t& dwRowID,const uint32_t& dwItemID);
	void    SetRowVisible(const uint32_t& dwRowID, const bool& bIsVisible);
	void    CancelRename();
	bool    IsSelectedEmpty() { return m_wndBody.IsSelectedEmpty(); }
	tstring GetItemRenameValue(const uint32_t& dwRowID, const uint32_t& dwItemID);
	void    SetCanDragged(const bool& bCanDragged);
	bool    CanDragged();
	void    SetRowSelectedBKFrom(COLORREF clr) { m_clrSelectedRowFrom = clr; }
	COLORREF GetRowSelectedBKFrom() { return m_clrSelectedRowFrom; }

	void    SetRowSelectedBKTo(COLORREF clr) { m_clrSelectedRowTo = clr; }
	COLORREF GetRowSelectedBKTo() { return m_clrSelectedRowTo; }

	void    SetRowSelectedText(COLORREF clr) { m_clrSelectedRowText = clr; }
	COLORREF GetRowSelectedText() { return m_clrSelectedRowText; }

	void    SetRowMouseOnBKFrom(COLORREF clr) { m_clrMouseOnRowFrom = clr; }
	COLORREF GetRowMouseOnBKFrom() { return m_clrMouseOnRowFrom; }

	void    SetRowMouseOnBKTo(COLORREF clr) { m_clrMouseOnRowTo = clr; }
	COLORREF GetRowMouseOnBKTo() { return m_clrMouseOnRowTo; }

	void    SetRowMouseOnText(COLORREF clr) { m_clrMouseOnRowText = clr; }
	COLORREF GetRowMouseOnText() { return m_clrMouseOnRowText; }

	void    SetButtonMouseOnBKFrom(COLORREF clr) { m_clrButtonMouseOnFrom = clr; }
	COLORREF GetButtonMouseOnBKFrom() { return m_clrButtonMouseOnFrom; }

	void    SetButtonMouseOnBKTo(COLORREF clr) { m_clrButtonMouseOnTo = clr; }
	COLORREF GetButtonMouseOnBKTo() { return m_clrButtonMouseOnTo; }

	void    SetButtonMouseOnBKMiddle(COLORREF clr) { m_clrButtonMouseOnMiddle = clr; }
	COLORREF GetButtonMouseOnBKMiddle() { return m_clrButtonMouseOnMiddle; }

	void    SetButtonPressedBKFrom(COLORREF clr) { m_clrButtonPressedFrom = clr; }
	COLORREF GetButtonPressedBKFrom() { return m_clrButtonPressedFrom; }

	void    SetButtonPressedBKTo(COLORREF clr) { m_clrButtonMouseOnTo = clr; }
	COLORREF GetButtonPressedBKTo() { return m_clrButtonMouseOnTo; }

	void    SetButtonPressedBKMiddle(COLORREF clr) { m_clrButtonPressedMiddle = clr; }
	COLORREF GetButtonPressedBKMiddle() { return m_clrButtonPressedMiddle; }

	void    SetCanSort(const bool& bCanSort) { m_bCanSort = bCanSort; }
	bool    IsCanSort() { return m_bCanSort; }

	void    SetGroupNum(const uint32_t& dwNum);
	uint32_t GetGroupNum() { return (uint32_t)m_vecRows.size(); }

	void    SetOrderGroup(const bool& bOrder) { m_bOrderGroup = bOrder; }
	bool    IsOrderGroup() { return m_bOrderGroup; }

	void    SetColumnSorted(const uint32_t& dwColumnID, const bool& bSorted);
	void    SetColumnValueType(const uint32_t& dwColumnID,
		const ERXSkinListItemValueType& enType);
	void    SetItemIntValue(const uint32_t& dwRowID, const uint32_t& dwItemID, const int64_t& llValue);
	void    SetItemValueType(const uint32_t& dwRowID,
		const uint32_t& dwItemID,
		const ERXSkinListItemValueType& enType);

	void    CancelMouseOnHeader();
	void    Sort(const uint32_t& dwItemID,
		const ERXSkinListItemSortType& enSortType);

	uint32_t GetCurSortColuminID() { return m_dwCurSortColumn; }
	ERXSkinListItemSortType GetCurSortType() { return m_enCurSortType; }

	void    InvalideBody() { m_wndBody.Invalidate(); }
	CRXSkinListCtrlBody& GetBody() { return m_wndBody; }
	void    SetColumnCanResize(const uint32_t& dwColumnID, const bool& bCanResize);
private:
	CRXSkinListCtrlHeader    m_wndHeader;
	CRXSkinListCtrlRowMap    m_mapRows;
	CRXSkinListCtrlRowGroupVec    m_vecRows;
	CRXSkinListCtrlRow* m_poRenameRow;
	UINT32  m_dwRowID;
	UINT32  m_dwColumnID;
	EListCtrlType  m_enType;
	IRXSkinListCtrlListener  * m_poListener;
	CRXSkinImageList*    m_poImageList;
	CRXSkinListCtrlBody  m_wndBody;

	COLORREF m_clrSelectedRowFrom;
	COLORREF m_clrSelectedRowTo;
	COLORREF m_clrSelectedRowText;
	COLORREF m_clrMouseOnRowFrom;
	COLORREF m_clrMouseOnRowTo;
	COLORREF m_clrMouseOnRowText;

	COLORREF m_clrButtonMouseOnFrom;
	COLORREF m_clrButtonMouseOnMiddle;
	COLORREF m_clrButtonMouseOnTo;

	COLORREF m_clrButtonPressedFrom;
	COLORREF m_clrButtonPressedMiddle;
	COLORREF m_clrButtonPressedTo;
	bool     m_bCanDragged;
	bool     m_bCanSort;
	bool     m_bOrderGroup;

	uint32_t m_dwCurSortColumn;
	ERXSkinListItemSortType m_enCurSortType;

private:
	VOID    _DeleteAll();
	CRXSkinRect _GetItemRect(const UINT32 dwRowID,const UINT32 dwItemID);
};
