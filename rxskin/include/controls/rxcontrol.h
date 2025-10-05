#pragma once
#include <list>
#include <map>
using namespace std;
#include "common/rximage.h"
#include "resize/rxskinresizepoint.h"
#include "resize/rxskinresizerect.h"
#include "common/rxskindefines.h"
#include "common/rxskinutil.h"
#include "core/SkSurface.h"
using namespace RX;
using namespace RX::Image;
#pragma warning(disable: 4251)
enum EControlState
{
	CONTROL_STATE_NORMAL = 0,
	CONTROL_STATE_MOUSEON,
	CONTROL_STATE_PRESSED,
	CONTROL_STATE_DISABLE,
};

enum EUserState
{
	USER_STATE_OFFLINE = 0,
	USER_STATE_ONLINE,
	USER_STATE_AWAY,
	USER_STATE_BUSY,
	USER_STATE_DISTURB,
	USER_STATE_CHAT,
	USER_STATE_HIDE,
};

enum EAlignType
{
	ANCHOR_BOTTOMCENTER = 0,
	ANCHOR_BOTTOMLEFT,
	ANCHOR_BOTTOMRIGHT,
	ANCHOR_MIDDLECENTER,
	ANCHOR_MIDDLELEFT,
	ANCHOR_MIDDLERIGHT,
	ANCHOR_TOPLEFT,
	ANCHOR_TOPRIGHT,
	ANCHOR_TOPCENTER
};

enum EControlType
{
	CONTROL_NONE = -1,
	CONTROL_TEXT = 0 ,
	CONTROL_BUTTON,
	CONTROL_RADIOLIST,
	CONTROL_STATUS_BUTTON,
	CONTROL_HYPERLINK,
	CONTROL_CHECKBOX_BUTTON,
	CONTROL_MENU_BUTTON,
	CONTROL_STATIC,
	CONTROL_IMAGE_BUTTON,
	CONTROL_CHECKBOX_IMAGE_BUTTON,
	CONTROL_TXT_BUTTON,
	CONTROL_SPERATOR,
	CONTROL_BUTTON_LIST,
	CONTROL_GROUP_LINE,
	CONTROL_COMBOBOX,
	CONTROL_EDIT,
	CONTROL_STATIC_IMAGE,
	CONTROL_TAB_CTRL,
	CONTROL_DATE_PICKER,
	CONTROL_LOCATION_PICKER,
	CONTROL_PICTURE,
	CONTROL_GIF,
	CONTROL_OUTBAR,
	CONTROL_BROWNSER,
	CONTROL_LISTCTRL,
	CONTROL_SPIN_EDIT,
	CONTROL_BG,
	CONTROL_WEB_BROWSER,
	CONTROL_MENUBAR,
	CONTROL_SLIDER,
	CONTROL_TREE,
	CONTROL_RICHEDIT,
	CONTROL_SEARCH_EDIT,
	CONTROL_PROGRESS,
};

class CRXSkinWnd;
class CRXSkinScrollbar;


RXSKIN_API EControlType RXAPI RXSkinGetControlType(const CHAR* pszControlType);
RXSKIN_API CRXSkinResizePoint RXAPI RXSkinGetAlignType(const CHAR* pszAlignType);

class IRXSkinEditListener
{
public:
	virtual VOID   OnEditReturnOK(const UINT32 dwID) = 0;
	virtual VOID   OnEditKillFocus(const UINT32 dwID) = 0;
	virtual VOID   OnEditChanged(const UINT32 dwID) = 0;
};

class RXSKIN_API CRXControl
{
public:
	CRXControl(void);
	~CRXControl(void);

	virtual TCHAR*	 GetText();
	virtual VOID     SetText(const TCHAR* strTxt);

	TCHAR* GetTooltip();
	VOID             SetTooltip(const TCHAR* strTooltip);

	UINT32           GetID();
	VOID             SetID(const UINT32 dwID);

	CRXSkinRect		 GetRect();
	virtual VOID     SetRect(const CRXSkinRect rcItem);

	POINT			 GetWindowPos();

	EControlState    GetState();
	VOID             SetState(EControlState enState);

	VOID             SetParentWnd(const HWND hParentWnd);
	HWND             GetParentWnd();

	VOID             SetResize(const RXSKIN_RESIZEPOINT& rpTopLeft, const RXSKIN_RESIZEPOINT& rpBottomRight);
	CRXSkinResizeRect& GetResize();
	CRXSkinRect& GetLast();
	VOID             SetLastRect(CRXSkinRect rcLast);

	VOID             Invalidate();

	BOOL             IsVisible();
	virtual VOID     SetVisible(const BOOL bIsVisible);

	virtual VOID	 Enable(const BOOL bEnabled);
	virtual BOOL	 IsEnabled();

	virtual CRXSkinRect    GetInvalidateRect();
	virtual VOID     OnMouseLeave();
	virtual CRXControl* GetInvalidateControl();
	virtual BOOL     OnSetCursor() { return TRUE; }
	virtual LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
	virtual VOID OnEditSetFocus(HWND hWnd) {};
	virtual VOID OnEditKillFocus(HWND hWnd) {};
	virtual VOID OnEditChanged(HWND hWnd) {};
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC,  CRXSkinRect rcItem) = 0;
	virtual VOID ReleaseControl() = 0;
	virtual BOOL PtInRect(POINT pt) = 0;
	virtual BOOL OnClick(POINT pt) = 0;
	virtual BOOL OnMouseMove(POINT pt) = 0;
	virtual BOOL OnLButtonDown(POINT pt) = 0;
	virtual BOOL OnLButtonUp(POINT pt) = 0;
	virtual BOOL OnLButtonDblClk(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) = 0;
	virtual BOOL OnRButtonUp(POINT pt) = 0;
	virtual BOOL OnRButtonDblClk(POINT pt) = 0;
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
	virtual BOOL OnTimer(WPARAM wParam,LPARAM pParam);
	virtual BOOL OnCommand(WPARAM wParam,LPARAM pParam);
	virtual BOOL OnMessage(UINT32 dwMessageID,WPARAM wParam,LPARAM pParam);
	virtual HBRUSH OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC);
	virtual LRESULT OnEditChanged(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnMouseWheel(UINT nFlags, short zDelta,POINT pt);
	virtual VOID  OnScrollBarChanged(EScrollType enType,INT32 nPercent);
	virtual bool  IsMouseEnabled() const;
	virtual bool  IsFocused() const;
	virtual void  SetFocus();
	virtual CRXControl*  HitTest(POINT pt);
	virtual void  ModifyRect();
	virtual void  OffsetRect(int32_t nX, int32_t nY);
	LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	HDC           GetPaintDC();
	bool          IsDCReady();

	void    EnableScrollBar(bool bEnableVertical, bool bEnableHorizontal);
	CRXSkinWnd* GetPaintWnd();
	void        SetBorder(const bool& bBorder) { m_bBorder = bBorder; }
	bool        IsBorder() { return m_bBorder; }


	virtual HBITMAP GetBKBitmap() { return NULL; }
	virtual	void    ReleaseBKBitmap() {
	}
	virtual uint32_t GetTextStyle() { return 0; }
	virtual bool    IsPassword() { return false; }
	virtual COLORREF GetTextColor() { return RGB(0,0,0); }
	virtual void    SetTextColor(COLORREF clr){}
	virtual COLORREF GetBkColor() { return RGB(0, 0, 0); }
private:
	TCHAR            m_szText[256];	
	UINT32           m_dwID;
	TCHAR            m_szToolTip[256];
	EControlState    m_enState;
	HWND             m_hParentWnd;
	CRXSkinRect      m_rcLast;
	CRXSkinResizeRect	 m_rrcSizing;
	BOOL             m_bEnabled;
	BOOL             m_bVisible;
	SIZE             m_oMarginTopLeft;
	SIZE             m_oMarginBottomRight;
	bool			 m_bMouseEnabled;
	bool			 m_bFocused;
	bool             m_bBorder;
protected:
	CRXSkinRect      m_rcItem;
};

class CRXSkinScrollbar;
class RXSKIN_API CRXControlMgr
{
public:
	typedef map<UINT32,CRXControl*>           CRXControlMap;
	typedef CRXControlMap::iterator           CRXControlMapItr;

	CRXControlMgr(void);
	~CRXControlMgr(void);

	BOOL             AddControl(CRXControl* poControl);
	CRXControl*      FindControl(const UINT32 dwID);
	CRXControl*      RemoveControl(const UINT32 dwID);
	CRXControl*      HitTest(POINT pt);
	VOID             SetVisible(const BOOL bIsVisible);
	BOOL			 OnClick(POINT pt);
	CRXControl*		 OnMouseMove(POINT pt);
	VOID             OnMouseLeave();
	BOOL			 OnSize(const UINT32 dwX,const UINT32 dwY);
	CRXControl* 	 OnLButtonDown(POINT pt);
	CRXControl*		 OnLButtonUp(POINT pt);
	CRXControl*      OnRButtonUp(POINT pt);
	CRXControl*		 OnLButtonDblClk(POINT pt);
	VOID             OnDraw(SkSurface* poSurface, HDC hDC);
	BOOL			 OnTimer(WPARAM wParam, LPARAM lParam);
	BOOL             OnCommand(WPARAM wParam,LPARAM pParam);
	BOOL             OnMessage(UINT32 dwMessageID,WPARAM wParam,LPARAM pParam);
	virtual HBRUSH   OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC);
	virtual LRESULT  OnEditChanged(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
	virtual BOOL     OnMouseWheel(UINT nFlags, short zDelta,POINT pt);
	virtual VOID     OnScrollBarChanged(EScrollType enType,INT32 nPercent);

	CRXControlMap& GetAllControl() { return m_oControlMap; }
private:
	VOID             _DeleteAll();
private:
	CRXControlMap    m_oControlMap;
};

class RXSKIN_API CRXRadioButton :public CRXControl
{
public:
	CRXRadioButton(void);
	~CRXRadioButton(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
	virtual VOID Enable(const BOOL bEnabled);
	virtual BOOL IsEnabled();
	VOID         SetChecked(const BOOL bChecked);
	BOOL         GetChecked();
	virtual void ModifyRect();
private:
	BOOL         m_bChecked;
};

class RXSKIN_API CRXRadioButtonList :public CRXControl
{
public:

	typedef list<CRXRadioButton*>           CRXRadioButtonLst;
	typedef CRXRadioButtonLst::iterator     CRXRadioButtonLstItr;

	CRXRadioButtonList(void);
	~CRXRadioButtonList(void);

	BOOL             AddRadioButton(CRXRadioButton* poControl);
	CRXRadioButton*  FindRadioButton(const UINT32 dwID);
	CRXRadioButton*  RemoveRadioButton(const UINT32 dwID);
	VOID             Select(const UINT32 dwID);
	UINT32           GetSelect();
	CRXRadioButton*  FindRadioByPosition(POINT pt);

	virtual VOID	 Enable(const BOOL bEnabled);
	virtual VOID	 OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID	 ReleaseControl();
	virtual BOOL	 PtInRect(POINT pt);
	virtual BOOL	 OnClick(POINT pt);
	virtual BOOL     OnLButtonDown(POINT pt);
	virtual BOOL	 OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL	 OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL	 OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL	 OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL	 OnMouseMove(POINT pt);
	virtual BOOL	 OnSize(const UINT32 dwX,const UINT32 dwY);
	virtual void     ModifyRect();
	virtual void     OffsetRect(int32_t nX, int32_t nY);
private:
	VOID             _DeleteAll();
private:
	CRXRadioButtonLst m_lstRadioButton;
	CRXRadioButton*  m_poSelected;
	CRXRadioButton*  m_poPressed;
};

class RXSKIN_API CRXListButton :public CRXControl
{
public:
	CRXListButton(void);
	~CRXListButton(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
	virtual VOID Enable(const BOOL bEnabled);
	virtual BOOL IsEnabled();
	VOID         SetSelected(const BOOL bSelected);
	BOOL         GetSelected();
	VOID         SetImage(const TCHAR* pszImageFile);
private:
	BOOL         m_bSelected;
	IRXImage*    m_poImage;
	tstring      m_strImageFile;
};


class RXSKIN_API CRXListButtonList :public CRXControl
{
public:

	typedef list<CRXListButton*>           CRXListButtonLst;
	typedef CRXListButtonLst::iterator     CRXListButtonLstItr;

	CRXListButtonList(void);
	~CRXListButtonList(void);

	BOOL             AddListButton(CRXListButton* poControl);
	CRXListButton*   FindListButton(const UINT32 dwID);
	CRXListButton*   RemoveListButton(const UINT32 dwID);
	VOID             Select(const UINT32 dwID);
	CRXListButton*   FindListByPosition(POINT pt);

	virtual VOID	 Enable(const BOOL bEnabled);
	virtual VOID	 OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID	 ReleaseControl();
	virtual BOOL	 PtInRect(POINT pt);
	virtual BOOL	 OnClick(POINT pt);
	virtual BOOL     OnLButtonDown(POINT pt);
	virtual BOOL	 OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL	 OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL	 OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL	 OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL	 OnMouseMove(POINT pt);
	virtual BOOL	 OnSize(const UINT32 dwX,const UINT32 dwY);
private:
	VOID             _DeleteAll();
private:
	CRXListButtonLst m_lstListButton;
	CRXListButton*  m_poSelected;
	CRXListButton*  m_poPressed;
	CRXListButton*  m_poMouseOn;
};


class RXSKIN_API CRXStatusButton:public CRXControl
{
public:
	struct SMenuItem
	{
	public:
		EMenuItemType m_enType;
		TCHAR         m_szName[256];
		TCHAR         m_szImagePath[256];
		UINT32        m_dwID;
		SMenuItem()
		{
			memset(m_szImagePath,0,sizeof(TCHAR)*256);
			memset(m_szName,0,sizeof(TCHAR)*256);
			m_enType = MENU_STRING;
			m_dwID = 0;
		}
	};
	typedef list<SMenuItem*>        CMenuItemList;
	typedef CMenuItemList::iterator CMenuItemListItr;
	CRXStatusButton(void);
	~CRXStatusButton(void);
	VOID         SetStatus(const UINT32 dwStatus,const BOOL bDraw = TRUE );
	EUserState   GetStatus();
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);
	VOID    AddMenuItem(EMenuItemType enType,const UINT32 dwID,const TCHAR* pszName,const TCHAR* pszImageFile=_T(""));
private:
	IRXImage*   m_poImage;
	EUserState  m_enStatus;
	VOID*       m_poMenu;
	CMenuItemList m_lstMenus;
};

class RXSKIN_API CRXCheckButton :public CRXControl
{
public:
	CRXCheckButton(void);
	~CRXCheckButton(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);
	VOID         SetChecked(const BOOL bChecked);
	BOOL         GetChecked();
private:
	BOOL         m_bChecked;
};

class RXSKIN_API CRXHyperLink :public CRXControl
{
public:
	CRXHyperLink(void);
	~CRXHyperLink(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);

	VOID         SetImageFile(const TCHAR* pszImageFile);
private:
	IRXImage*    m_poImage;
	TCHAR        m_szImageFile[256];      
};

class RXSKIN_API CRXButton :public CRXControl
{
public:
	CRXButton(void);
	~CRXButton(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);

	VOID    SetImage(const TCHAR* pszImageFile);
	TCHAR*  GetImageFile();

	VOID         SetBorderColor(COLORREF clrBorderColor);
	COLORREF     GetBorderColor();

	VOID         SetMouseOnFromColor(COLORREF clrMouseOnFromColor);
	COLORREF     GetMouseOnFromColor();

	VOID         SetMouseOnToColor(COLORREF clrNormalToColor);
	COLORREF     GetMouseOnToColor();

	VOID         SetMouseOnMiddleColor(COLORREF clrMouseOnMiddleColor);
	COLORREF     GetMouseOnMiddleColor();

	VOID         SetPressedFromColor(COLORREF clrPressedFromColor);
	COLORREF     GetPressedFromColor();

	VOID         SetPressedToColor(COLORREF clrPressedToColor);
	COLORREF     GetPressedToColor();

	VOID         SetPressedMiddleColor(COLORREF clrPressedMiddleColor);
	COLORREF     GetPressedMiddleColor();

	void         SetCheckBoxBtn(const bool& bCheck) { m_bCheckBoxBtn = bCheck; };
	void         SetSelect(const bool& bSelected);
	bool         IsSelected() { return m_bSelected; }
private:
	IRXImage*    m_poImage;
	TCHAR        m_szImage[256];
	COLORREF     m_clrBorder;
	COLORREF     m_clrMouseOnFrom;
	COLORREF     m_clrMouseOnMiddle;
	COLORREF     m_clrMouseOnTo;

	COLORREF     m_clrPressedFrom;
	COLORREF     m_clrPressedMiddle;
	COLORREF     m_clrPressedTo;

	bool         m_bCheckBoxBtn;
	bool         m_bSelected;
private: 
	void         _DrawCheckBoxButton(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	void         _DrawNormalButton(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
};

class RXSKIN_API CRXImageButton :public CRXControl
{
public:
	CRXImageButton(void);
	~CRXImageButton(void);
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
	VOID    SetNormalImage(const TCHAR* pszImageFile);
	VOID    SetNormalImage(IRXImage* poImage) { m_poImageNormal = poImage; };
	VOID    SetHotImage(const TCHAR* pszImageFile);
	VOID    SetHotImage(IRXImage* poImage) { m_poImageHot = poImage; };
	VOID    SetDownImage(const TCHAR* pszImageFile);
	VOID    SetDownImage(IRXImage* poImage) { m_poImageDown = poImage; };
	VOID    ResetNormalImage(const TCHAR* pszImageFile);
	VOID    ResetHotImage(const TCHAR* pszImageFile);
	VOID    ResetDownImage(const TCHAR* pszImageFile);
	COLORREF GetNormalBK();
	void     SetNormalBK(COLORREF clr);
	VOID    SetFont(HFONT hFont) { m_hFont = hFont; };
	void     SetTextColor(COLORREF clr) { m_clrTxt = clr; };
	void     SetAutoRecycle(const bool& bRecyCle) { m_bAutoRecycle = bRecyCle; }
private:
	bool           m_bAutoRecycle;
	IRXImage*      m_poImageNormal;
	tstring        m_strNormalImage;
	IRXImage*      m_poImageHot;
	tstring        m_strHotImage;
	IRXImage* m_poImageDown;
	tstring        m_strDownImage;
	COLORREF       m_clrNormalBk;
	COLORREF       m_clrTxt;
	HFONT          m_hFont;
};
class CRXSkinFont;
class RXSKIN_API CRXSkinStatic :public CRXControl
{
public:
	CRXSkinStatic(void);
	~CRXSkinStatic(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);
	VOID    SetTextColor(COLORREF clr) { m_clrText = clr; };
	VOID    SetFont(const LOGFONT& oFont);
	VOID    SetFontID(const uint32_t& dwFontID);
	VOID    SetCenter(const bool& bCenter) { m_bCenter = bCenter; }
	void    SetIsSingleLine(const bool& bIsSingle) { m_bIsSingleLine = bIsSingle; }
private:
	CRXSkinFont* m_poFont;
	COLORREF m_clrText;
	bool    m_bCenter;
	bool    m_bIsSingleLine;
};

class RXSKIN_API CRXTitle :public CRXControl
{
public:
	CRXTitle(void);
	~CRXTitle(void);
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
	VOID    SetImage(const TCHAR* pszImageFile);
private:
	IRXImage*    m_poImage;
	tstring      m_strImage;
};


class RXSKIN_API CRXMenuButton :public CRXControl
{
public:
	CRXMenuButton(void);
	~CRXMenuButton(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);
};

class RXSKIN_API CRXCheckImageButton :public CRXControl
{
public:
	CRXCheckImageButton(void);
	~CRXCheckImageButton(void);
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
	VOID             SetChecked(const BOOL bChecked);
	BOOL             GetChecked();
private:
	BOOL             m_bChecked;
};

class RXSKIN_API CRXTxtButton :public CRXControl
{
public:
	CRXTxtButton(void);
	~CRXTxtButton(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);
};

class RXSKIN_API CRXSperator :public CRXControl
{
public:
	CRXSperator(void);
	~CRXSperator(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);
};

class RXSKIN_API CRXGroupLine :public CRXControl
{
public:
	CRXGroupLine(void);
	~CRXGroupLine(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; }
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; };
	virtual BOOL OnMouseMove(POINT pt);
};

class RXSKIN_API CRXStaticImage :public CRXControl
{
public:
	CRXStaticImage(void);
	~CRXStaticImage(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);

	VOID    SetImage(const TCHAR* pszImageFile);
private:
	TCHAR   m_szImage[256];
	IRXImage*   m_poImage;
};

class RXSKIN_API CRXSkinPicture :public CRXControl
{
public:
	CRXSkinPicture(void);
	~CRXSkinPicture(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseMove(POINT pt);
	VOID    SetImage(const TCHAR* pszImageFile);
	VOID    SetImage(IRXImage* poImage);
private:
	TCHAR   m_szImage[256];
	IRXImage*   m_poImage;
};

class RXSKIN_API CRXSkinBG :public CRXControl
{
public:
	CRXSkinBG(void);
	~CRXSkinBG(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt) { return FALSE; }
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; }
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; };
	virtual BOOL OnMouseMove(POINT pt) { return FALSE; }
	void    SetFrom(const COLORREF& clr) { m_clrFrom = clr; }
	void    SetTo(const COLORREF& clr) { m_clrTo = clr; }
private:
	COLORREF m_clrFrom;
	COLORREF m_clrTo;
};


class RXSKIN_API CBorder
{
public:
	enum Type
	{
		BORDER_TOP = 0,
		BORDER_LEFT,
		BORDER_BOTTOM,
		BORDER_RIGHT
	};
	CBorder();
	~CBorder();
	VOID      SetImageFile(const TCHAR* pszImageFile);
	VOID      SetFirstOffSet(const UINT32 dwFirstOffSet);
	VOID      SetSecondOffSet(const UINT32 dwSecondOffSet);
	VOID      OnDraw(HDC hDC, CRXSkinRect rcClient, INT32 nTopOffSet, INT32 nBottomOffSet);
	VOID      SetType(Type enType);
	VOID      SetTrans(COLORREF clrTrans);
	INT32     GetWidth();
	INT32     GetHeight();
private:
	TCHAR     m_szFilename[256];
	IRXImage* m_poImage;
	INT32     m_nOffSetFirst;
	INT32     m_nOffSetSecond;
	Type      m_enType;
	COLORREF  m_clrTrans;
protected:
	VOID      _DrawTop(HDC hDC, CRXSkinRect rcClient);
	VOID      _DrawBottom(HDC hDC, CRXSkinRect rcClient);
	VOID      _DrawLeft(HDC hDC, CRXSkinRect rcClient, INT32 nTopOffSet, INT32 nBottomOffSet);
	VOID      _DrawRight(HDC hDC, CRXSkinRect rcClient, INT32 nTopOffSet, INT32 nBottomOffSet);
};