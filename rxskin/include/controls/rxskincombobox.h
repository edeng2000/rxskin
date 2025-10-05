#pragma once
#include "rxskin.h"
#include "rxskinitem.h"
#include "rxcontrol.h"
#include "rxskinscrollbar.h"
#include "fundation/rxstring.h"
#include "main/rxskinwnd.h"
#include "main/rxskineditwnd.h"
#include <shlobj.h>
#include <map>
using namespace std;

enum EComboBoxType
{
	COMBOBOX_DROPDOWN = 0,
	COMBOBOX_DROPLIST,
	COMBOBOX_DROPLIST_FILE,
	COMBOBOX_DROPDOWN_EXT,
};

class IRXSkinComboBoxListener
{
public:
	virtual  void OnComboBoxChanged(const uint32_t& dwID) = 0;
};

class CRXSkinComboBox;
class RXSKIN_API CRXSkinComboBoxPopupItem : public CRXSkinItem
{
public:
	CRXSkinComboBoxPopupItem();
	~CRXSkinComboBoxPopupItem();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);
	VOID  SetComboBox(CRXSkinComboBox* poComboBox);
	void  SetIconNumber(const int32_t& nNumber) { m_nIconNumber = nNumber; }
private:
	CRXSkinComboBox* m_poComboBox;
	int32_t  m_nIconNumber;
};

class RXSKIN_API CRXSkinComboBoxPopup : public CRXSkinWnd
{
public:
	typedef map<UINT32,CRXSkinComboBoxPopupItem*>   CRXSkinComboBoxPopupItemMap;
	typedef CRXSkinComboBoxPopupItemMap::iterator   CRXSkinComboBoxPopupItemMapItr;

	CRXSkinComboBoxPopup();
	~CRXSkinComboBoxPopup();
	virtual VOID ReleaseControl();
	virtual BOOL Create(const TCHAR* pszWindowClassName,
						const TCHAR* pszWindowName,
						const UINT32 dwStyle,
						const HWND hParentWnd,
						HICON hIcon = NULL);
	VOID      Show(POINT pt);
	VOID      SetCaptureWnd(HWND hCpatureWnd);
	HWND      GetHWnd();
	VOID      SetComboBox(CRXSkinComboBox* poComboBox);
protected:
	virtual VOID  OnPaint();
	virtual BOOL  OnMouseMove(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnMessage(UINT32 dwMessageID,WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnMouseWheel(UINT nFlags, short zDelta,POINT pt);
	virtual VOID  OnScrollBarChanged(EScrollType enType,INT32 nPercent);
private:
	CRXSkinComboBox* m_poComboBox;
	CRXSkinComboBoxPopupItemMap   m_oItemMap;
	CRXSkinComboBoxPopupItem*     m_poMouseOn;
	CRXSkinComboBoxPopupItem*     m_poPressed;
	HWND      m_hCaptureWnd;
	CRXSkinScrollbar    m_oScrollbar;
	INT32     m_nTotalHeight;
	INT32     m_nTopOffSet;
protected:
	CRXSkinComboBoxPopupItem*     _HitTest(POINT pt);
	VOID                          _ResetContent();
};


class RXSKIN_API CRXSkinComboBox : public CRXControl
{
public:
	struct SItemData
	{
	public:
		TCHAR    m_szValue[256];
		TCHAR    m_szValueEx[256];
		UINT32   m_dwID;
		LPARAM   m_wParam;
		INT32    m_nImageIndex;
		SItemData()
		{
			memset(m_szValue,0,sizeof(TCHAR)*256);
			memset(m_szValueEx, 0, sizeof(TCHAR) * 256);
			m_dwID = 0;
			m_nImageIndex = -1;
			m_wParam = NULL;
		}
		~SItemData()
		{
			if (m_wParam)
			{
				free((void*)m_wParam);
				m_wParam = NULL;
			}
		}
	};
	typedef vector<SItemData*>			CItemDataVec;
	typedef CItemDataVec::iterator		CItemDataVecItr;
	CRXSkinComboBox();
	virtual ~CRXSkinComboBox();
	BOOL    Create(const UINT32 dwID,CRXSkinRect rcClient,EComboBoxType enType);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual HBRUSH OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual VOID OnEditSetFocus(HWND hWnd);
	virtual VOID OnEditKillFocus(HWND hWnd);
	virtual VOID OnEditChanged(HWND hWnd);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
	virtual VOID SetVisible(const BOOL bIsVisible);
	virtual VOID Enable(const BOOL bEnabled);
	virtual void SetFocus();
	virtual VOID SetText(const TCHAR* strTxt);
	virtual void ModifyRect();
	void    SetComboboxType(const EComboBoxType& enType) { m_enType = enType; }
	EComboBoxType GetComboBoxType() { return m_enType; }
	VOID    SetCaptureWnd(HWND hCpatureWnd);
	HWND    GetHWnd();

	VOID    SetMaxHeight(const UINT32 dwMaxHeight);
	UINT32  GetMaxHeight();
	HWND    GetEditWnd();
	CItemDataVec&  GetValueVec();
	void    SetListener(IRXSkinComboBoxListener* poListener);
	UINT32  GetCount();
	VOID    ResetContent();
	TCHAR*  GetLBText(const UINT32 dwID);
	TCHAR*  GetLBTextEx(const UINT32 dwID);
	VOID    SetCurSel(const UINT32 dwID, const BOOL& bNotify = TRUE);
	VOID    SetCurSelEx(const UINT32 dwID);
	void    SelectString(const TCHAR* cpName);
	void    SelectFirst();
	INT32   GetCurSel();
	LPARAM  GetCurSelData();
	LPARAM  GetItemData(const UINT32 dwID);

	UINT32  AddString(const TCHAR* pszData, INT32 nImageIndex = -1,LPARAM lpParam=NULL);
	UINT32  AddStringEx(const TCHAR* pszData, 
		const TCHAR* pszValueEx,
		INT32 nImageIndex = -1,
		LPARAM lpParam = NULL);
	VOID    DelString(const UINT32 dwID);
	VOID    DelString(const TCHAR* pszData);

	SItemData*    FindItem(const UINT32 dwID);
	SItemData* FindItemByText(const TCHAR* pszTxt);
	TCHAR*  GetText();
	CRXSkinString GetWindowText();
	VOID    SetLeftImage(const TCHAR* pszImageFile);
	VOID    SetLeftImageEx(IRXImage* poImage) { m_poImageLeft = poImage; };
	VOID    SetTipValue(const TCHAR* pszTipValue) { m_strTipValue = pszTipValue; };
	VOID    SetTipTextColor(const COLORREF& clr) { m_clrTipText = clr; }
	VOID    SetTipTextBG(const COLORREF& clr) { m_clrTipBG = clr; }

	VOID    SetNormalBorder(const COLORREF& clr) { m_clrNormalBorder = clr; }
	VOID    SetHotBorder(const COLORREF& clr) { m_clrHotBorder = clr; }
	VOID    SetImageList(CRXSkinImageList* poImageList);
	CRXSkinImageList* GetImageList() { return m_poImageList; }
	virtual bool    IsPassword() { return false; }
	virtual HBITMAP GetBKBitmap() { return m_hBmpEditBk; }
	virtual void    ReleaseBKBitmap() {
		::DeleteObject(m_hBmpEditBk);
		m_hBmpEditBk = NULL;
	}
	virtual uint32_t GetTextStyle() { return m_dwTextStyle; }
	void   SetFontName(const TCHAR* pszFontName) { m_strFontName = pszFontName; }
	void   SetFontSize(const uint32_t& dwSize) { m_dwFontSize = dwSize; }
	void   SetIsBold(const BOOL& bIsBold) { m_bIsBold = bIsBold; }
private:
	SItemData*    m_poCurItem;
	EComboBoxType m_enType;
	CRXSkinEditWnd     m_wndEdit;
	CRXSkinComboBoxPopup* m_poPopup;
	CItemDataVec  m_vecValues;
	UINT32        m_dwMaxHeight;
	HWND          m_hCaptureWnd;
	UINT32        m_dwGeneratorID;
	IRXSkinComboBoxListener* m_poListener;
	CRXSkinString m_strText;
	CRXSkinString m_strTipValue;
	COLORREF  m_clrTipText;
	COLORREF  m_clrTipBG;
	COLORREF  m_clrNormalBorder;
	COLORREF  m_clrHotBorder;
	CRXSkinImageList*    m_poImageList;
	CRXSkinRect   m_rcArrow;
	uint32_t  m_dwTextStyle;
	HBITMAP	  m_hBmpEditBk;
	bool      m_bFocus;

	tstring   m_strFontName;
	BOOL      m_bIsBold;
	uint32_t  m_dwFontSize;
	HFONT     m_hFont;
private:
	IRXImage* m_poImageLeft;
	tstring  m_strLeftImage;
};


