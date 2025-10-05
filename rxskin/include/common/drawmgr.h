#pragma once

#include "rxskin.h"
#include <CommCtrl.h>
#include <map>
#include "fundation/tinyxml2.h"
#include "common/rximage.h"
#include "common/rxskinutil.h"
#include "controls/rxcontrol.h"
class CRXSkinDialog;
class CRXSkinWnd;

typedef BOOL (RXAPI *lpfnUpdateLayeredWindow)(
	HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc,
	POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags
	);

typedef BOOL (RXAPI *lpfnSetLayeredWindowAttributes)(
	HWND hwnd, COLORREF crKey, BYTE xAlpha, DWORD dwFlags
	);



class RXSKIN_API CRXSkinRenderClip
{
public:
	~CRXSkinRenderClip();
	RECT rcItem;
	HDC hDC;
	HRGN hRgn;
	HRGN hOldRgn;

	static void GenerateClip(HDC hDC, RECT rc, CRXSkinRenderClip& clip);
	static void GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRXSkinRenderClip& clip);
	static void UseOldClipBegin(HDC hDC, CRXSkinRenderClip& clip);
	static void UseOldClipEnd(HDC hDC, CRXSkinRenderClip& clip);
};


class RXSKIN_API CDrawMgr
{
public:

	enum ECursorType
	{
		CURSOR_ARROW = 0,
		CURSOR_HAND,
		CURSOR_NS,
		CURSOR_WE,
		CURSOR_NWSE,
		CURSOR_NESW
	};
	struct SControlBorder
	{
	public:
		UINT32      m_dwTotal;
		UINT32      m_dwFirstOffSet;
		UINT32      m_dwSecondOffSet;
		SControlBorder()
		{
			m_dwSecondOffSet= m_dwTotal = m_dwFirstOffSet = 0;
		}
		~SControlBorder()
		{

		}
	};
	CDrawMgr(void);
	~CDrawMgr(void);
	BOOL         Init(const TCHAR* pszConfig);
	BOOL         InitResource(const TCHAR* pszResource);
	VOID         UnInit();

	void         LoadSelfBmp();

	VOID         SetCursor(ECursorType enType);

	POINT        GetButtonSize();
	VOID         SetButtonSize(POINT pt);
	HFONT        GetEditFont();
	HFONT        GetNormalFont();
	HFONT        GetBoldFont();
	HFONT        GetTitleFont();
	HCURSOR      GetHandCursor();
	HCURSOR      GetArrowCursor();
	HCURSOR      GetNSCursor();
	HCURSOR      GetWECursor();
	HCURSOR      GetNWSECursor();
	HCURSOR      GetNESWCursor();
	double       DrawImage(RX::Image::IRXImage* poImage,RECT rcImage,HDC hDC);
	double       DrawImageGray(RX::Image::IRXImage* poImage,RECT rcImage,HDC hDC);
	VOID         SetLayeredWindowAttributes(HWND hWnd,COLORREF clr,INT32 nAlpha);
	COLORREF     GetTitleColor();
	VOID         SetTitleColor(COLORREF clrTitle);
	VOID         SetTitleFontSize(const INT32 nFontSize);
	VOID         SetTitleFontName(const TCHAR* pszFontName);

	VOID         SetCommonFontSize(const INT32 nFontSize);
	VOID         SetCommonFontName(const TCHAR* pszFontName);
	INT32        GetCommonFontSize() { return m_nCommonFontSize; }
	TCHAR*       GetCommonFontName() { return m_szCommonFontName; }
	COLORREF     GetCommonColor() { return m_clrCommon; }
	VOID         SetCommonColor(const COLORREF& clr) { m_clrCommon = clr; }
	VOID         SetButtonBorderColor(COLORREF clrBorderColor);
	COLORREF     GetButtonBorderColor();

	VOID         SetButtonNormalFromColor(COLORREF clrNormalFromColor);
	COLORREF     GetButtonNormalFromColor();
	COLORREF     GetButtonNormalTxt() { return m_clrButtonNormalTxt; }
	VOID         SetButtonNormalToColor(COLORREF clrNormalToColor);
	COLORREF     GetButtonNormalToColor();

	VOID         SetButtonNormalMiddleColor(COLORREF clrNormalMiddleColor);
	COLORREF     GetButtonNormalMiddleColor();

	SIZE         GetButtonRoundSize();
	VOID         SetButtonRoundSize(const SIZE szButtonRoundSize);

	VOID         SetButtonMouseOnFromColor(COLORREF clrMouseOnFromColor);
	COLORREF     GetButtonMouseOnFromColor();

	VOID         SetButtonMouseOnToColor(COLORREF clrNormalToColor);
	COLORREF     GetButtonMouseOnToColor();

	VOID         SetButtonMouseOnMiddleColor(COLORREF clrMouseOnMiddleColor);
	COLORREF     GetButtonMouseOnMiddleColor();
	COLORREF     GetButtonMouseOnTxt() { return m_clrButtonMouseOnTxt; }

	VOID         SetButtonPressedFromColor(COLORREF clrPressedFromColor);
	COLORREF     GetButtonPressedFromColor();

	VOID         SetButtonPressedToColor(COLORREF clrPressedToColor);
	COLORREF     GetButtonPressedToColor();

	VOID         SetButtonPressedMiddleColor(COLORREF clrPressedMiddleColor);
	COLORREF     GetButtonPressedMiddleColor();
	COLORREF     GetButtonPressedTxt() { return m_clrButtonPressedTxt; }

	VOID         SetCommonButtonBorderColor(COLORREF clrBorderColor);
	COLORREF     GetCommonButtonBorderColor();

	VOID         SetCommonButtonNormalFromColor(COLORREF clrNormalFromColor);
	COLORREF     GetCommonButtonNormalFromColor();

	VOID         SetCommonButtonNormalToColor(COLORREF clrNormalToColor);
	COLORREF     GetCommonButtonNormalToColor();

	VOID         SetCommonButtonNormalMiddleColor(COLORREF clrNormalMiddleColor);
	COLORREF     GetCommonButtonNormalMiddleColor();

	SIZE         GetCommonButtonRoundSize();
	VOID         SetCommonButtonRoundSize(const SIZE szButtonRoundSize);

	VOID         SetCommonButtonMouseOnFromColor(COLORREF clrMouseOnFromColor);
	COLORREF     GetCommonButtonMouseOnFromColor();

	VOID         SetCommonButtonMouseOnToColor(COLORREF clrNormalToColor);
	COLORREF     GetCommonButtonMouseOnToColor();

	VOID         SetCommonButtonMouseOnMiddleColor(COLORREF clrMouseOnMiddleColor);
	COLORREF     GetCommonButtonMouseOnMiddleColor();

	VOID         SetCommonButtonPressedFromColor(COLORREF clrPressedFromColor);
	COLORREF     GetCommonButtonPressedFromColor();

	VOID         SetCommonButtonPressedToColor(COLORREF clrPressedToColor);
	COLORREF     GetCommonButtonPressedToColor();

	VOID         SetCommonButtonPressedMiddleColor(COLORREF clrPressedMiddleColor);
	COLORREF     GetCommonButtonPressedMiddleColor();

	UINT32       GetComboBoxArrowWidth();
	UINT32       GetComboBoxArrowOffSet();

	SControlBorder& GetControlBorderLeft();
	SControlBorder& GetControlBorderRight();
	SControlBorder& GetControlBorderTop();
	SControlBorder& GetControlBorderBottom();
	UINT32       GetControlBorderSize();

	UINT32       GetComboBoxPopupOffSetX();
	UINT32       GetComboBoxPopupOffSetY();
	UINT32       GetComboBoxPopupItemHeight();
	COLORREF     GetComboBoxPopupColorBK();
	COLORREF     GetComboBoxPopupColorBorder();

	COLORREF     GetComboBoxPopupItemMouseOnBkColor();
	COLORREF     GetComboBoxPopupItemMouseOnTextColor();
	COLORREF     GetComboBoxPopupItemPressedBkColor();
	COLORREF     GetComboBoxPopupItemPressedTextColor();
	COLORREF     GetComboBoxPopupItemSelectedBkColor();
	COLORREF     GetComboBoxPopupItemSelectedTextColor();
	COLORREF     GetComboBoxPopupItemNormalTextColor();

	//  CRXSkinTabCtrl相关参数
	COLORREF     GetTabItemNormalFromBk();
	COLORREF     GetTabItemNormalToBk();
	COLORREF     GetTabItemNormalText();

	COLORREF     GetTabItemMouseOnFromBk();
	COLORREF     GetTabItemMouseOnToBk();
	COLORREF     GetTabItemMouseOnText();

	COLORREF     GetTabItemPressedFromBk();
	COLORREF     GetTabItemPressedToBk();
	COLORREF     GetTabItemPressedText();

	COLORREF     GetTabItemSelectedFromBk();
	COLORREF     GetTabItemSelectedToBk();
	COLORREF     GetTabItemSelectedText();
	COLORREF     GetTabItemBorder();

	SIZE         GetTabCtrlRound();
	UINT32       GetTabCtrlItemHeight();
	UINT32       GetTabItemOffSetX();

	// CRXSkinTabCtrl参数设定 结束

	// CRXSkinToolTip参数设定
	COLORREF     GetToolTipBk();
	VOID         SetToolTipBk(COLORREF clr);

	COLORREF     GetToolTipBorder();
	VOID         SetToolTipBorder(COLORREF clr);

	UINT32       GetToolTipOffSetX();
	VOID         SetToolTipOffSetX(UINT32 dwOffSetX);

	UINT32       GetToolTipOffSetY();
	VOID         SetToolTipOffSetY(UINT32 dwOffSetY);
	// CRXSkinToolTip参数设定 结束

	// CRXSkinMenu参数设定
	COLORREF     GetMenuItemNormalFromBk();
	VOID         SetMenuItemNormalFromBk(COLORREF clr);

	COLORREF     GetMenuItemNormalToBk();
	VOID         SetMenuItemNormalToBk(COLORREF clr);

	COLORREF     GetMenuItemNormalText();
	VOID         SetMenuItemNormalText(COLORREF clr);

	COLORREF     GetMenuItemMouseOnFromBk();
	VOID         SetMenuItemMouseOnFromBk(COLORREF clr);

	COLORREF     GetMenuItemMouseOnToBk();
	VOID         SetMenuItemMouseOnToBk(COLORREF clr);

	COLORREF     GetMenuItemMouseOnText();
	VOID         SetMenuItemMouseOnText(COLORREF clr);

	COLORREF     GetMenuItemPressedFromBk();
	VOID         SetMenuItemPressedFromBk(COLORREF clr);

	COLORREF     GetMenuItemPressedToBk();
	VOID         SetMenuItemPressedToBk(COLORREF clr);

	COLORREF     GetMenuItemPressedText();
	VOID         SetMenuItemPressedText(COLORREF clr);

	COLORREF     GetMenuSeperatorBk();
	VOID         SetMenuSeperatorBk(COLORREF clr);

	COLORREF     GetMenuBk();
	VOID         SetMenuBk(COLORREF clr);

	COLORREF     GetMenuBorder();
	VOID         SetMenuBorder(COLORREF clr);

	COLORREF     GetMenuSidebarBk();
	VOID         SetMenuSidebarBk(COLORREF clr);

	UINT32       GetMenuSidebarWidth();
	VOID         SetMenuSidebarWidth(UINT32 dwWidth);

	UINT32       GetMenuItemOffSetX();
	VOID         SetMenuItemOffSetX(const UINT32 dwOffSetX);

	UINT32       GetMenuItemOffSetY();
	VOID         SetMenuItemOffSetY(const UINT32 dwOffSetY);

	SIZE         GetMenuRound();
	VOID         SetMenuRound(SIZE sz);

	UINT32       GetMenuItemHeight();
	VOID         SetMenuItemHeight(const UINT32 dwItemHeight);

	UINT32       GetMenuItemArrowWidth();
	VOID         SetMenuItemArrowWidth(const UINT32 dwItemArrowWidth);

	UINT32       GetMenuSeperatorHeight();
	VOID         SetMenuSeperatorHeight(const UINT32 dwSeperatorHeight);
	// CRXSkinMenu参数设定 结束

	// CRXSkinDatePicker参数设定
	SIZE         GetDatePickerSize();
	// CRXSkinDatePicker参数设定 结束
	
	// CRXSkinDatePicker参数设定
	SIZE         GetLocationPickerSize();
	// CRXSkinDatePicker参数设定 结束
	
	// 全局相关参数
	COLORREF     GetCommonBk();
	COLORREF     GetCommonBorderColor();
	COLORREF     GetHyperLinkText();
	COLORREF     GetHyperLinkHotText();
	SIZE         GetCommonBorderSize();
	VOID         SetDiableText(const COLORREF clr);
	COLORREF     GetDisableText();
	VOID         SetNormalText(const COLORREF clr);
	COLORREF     GetNormalText();
	// 箭头
	COLORREF     GetArrowNormal();
	COLORREF     GetArrowMouseOn();
	COLORREF     GetArrowPressed();
	UINT32       GetArrowHeight();

	// CRXSkinOutbar参数设定
	SIZE         GetOutbarRound();
	VOID         SetOutbarRound(SIZE szRound);

	SIZE         GetOutbarOffSet();
	VOID         SetOutbarOffSet(SIZE szRound);

	COLORREF     GetOutbarItemSelectedNormalFrom();
	VOID         SetOutbarItemSelectedNormalFrom(COLORREF clr);

	COLORREF     GetOutbarItemSelectedMouseOnFrom();
	VOID         SetOutbarItemSelectedMouseOnFrom(COLORREF clr);

	COLORREF     GetOutbarItemSelectedNormalTo();
	VOID         SetOutbarItemSelectedNormalTo(COLORREF clr);

	COLORREF     GetOutbarItemSelectedMouseOnTo();
	VOID         SetOutbarItemSelectedMouseOnTo(COLORREF clr);

	COLORREF     GetOutbarItemSelectedText();
	VOID         SetOutbarItemSelectedText(COLORREF clr);

	COLORREF     GetOutbarItemNormalText();
	VOID         SetOutbarItemNormalText(COLORREF clr);

	COLORREF     GetOutbarItemMouseOnText();
	VOID         SetOutbarItemMouseOnText(COLORREF clr);

	INT32        GetOutbarItemIndentationX();
	VOID         SetOutbarItemIndentationX(INT32 nOutbarItemIndentationX);

	INT32        GetOutbarItemHeight();
	VOID         SetOutbarItemHeight(INT32 nOutbarItemHeight);

	INT32        GetOutbarFolderHeight();
	VOID         SetOutbarFolderHeight(INT32 nOutbarFolderHeight);

	SIZE         GetOutbarFolderOffSet();
	VOID         SetOutbarFolderOffSet(SIZE szRound);

	COLORREF     GetOutbarFolderNormalBorder();
	VOID         SetOutbarFolderNormalBorder(COLORREF clr);

	COLORREF     GetOutbarFolderNormalFrom();
	VOID         SetOutbarFolderNormalFrom(COLORREF clr);

	COLORREF     GetOutbarFolderNormalTo();
	VOID         SetOutbarFolderNormalTo(COLORREF clr);

	COLORREF     GetOutbarFolderNormalText();
	VOID         SetOutbarFolderNormalText(COLORREF clr);

	COLORREF     GetOutbarBKFrom();
	VOID         SetOutbarBKFrom(COLORREF clr);

	COLORREF     GetOutbarBKTo();
	VOID         SetOutbarBKTo(COLORREF clr);

	COLORREF     GetOutbarFolderPressedBorder();
	VOID         SetOutbarFolderPressedBorder(COLORREF clr);

	COLORREF     GetOutbarFolderPressedFrom();
	VOID         SetOutbarFolderPressedFrom(COLORREF clr);

	COLORREF     GetOutbarFolderPressedTo();
	VOID         SetOutbarFolderPressedTo(COLORREF clr);

	COLORREF     GetOutbarFolderPressedText();
	VOID         SetOutbarFolderPressedText(COLORREF clr);

	COLORREF     GetOutbarFolderMouseOnFirstBorder();
	VOID         SetOutbarFolderMouseOnFirstBorder(COLORREF clr);

	COLORREF     GetOutbarFolderMouseOnSecondBorder();
	VOID         SetOutbarFolderMouseOnSecondBorder(COLORREF clr);

	COLORREF     GetOutbarFolderMouseOnThirdBorder();
	VOID         SetOutbarFolderMouseOnThirdBorder(COLORREF clr);

	COLORREF     GetOutbarFolderMouseOnFrom();
	VOID         SetOutbarFolderMouseOnFrom(COLORREF clr);

	COLORREF     GetOutbarFolderMouseOnTo();
	VOID         SetOutbarFolderMouseOnTo(COLORREF clr);

	COLORREF     GetOutbarFolderMouseOnText();
	VOID         SetOutbarFolderMouseOnText(COLORREF clr);
	// CRXSkinOutbar参数设定 结束

	// CRXSkinButtonList参数设定
	COLORREF      GetButtonListSelectedNormalFrom();
	VOID          SetButtonListSelectedNormalFrom(COLORREF clr);

	COLORREF      GetButtonListSelectedNormalTo();
	VOID          SetButtonListSelectedNormalTo(COLORREF clr);

	COLORREF      GetButtonListSelectedNormalText();
	VOID          SetButtonListSelectedNormalText(COLORREF clr);

	COLORREF      GetButtonListSelectedMouseOnFrom();
	VOID          SetButtonListSelectedMouseOnFrom(COLORREF clr);

	COLORREF      GetButtonListSelectedMouseOnTo();
	VOID          SetButtonListSelectedMouseOnTo(COLORREF clr);

	COLORREF      GetButtonListSelectedMouseOnText();
	VOID          SetButtonListSelectedMouseOnText(COLORREF clr);

	COLORREF      GetButtonListSelectedPressedFrom();
	VOID          SetButtonListSelectedPressedFrom(COLORREF clr);

	COLORREF      GetButtonListSelectedPressedTo();
	VOID          SetButtonListSelectedPressedTo(COLORREF clr);

	COLORREF      GetButtonListSelectedPressedText();
	VOID          SetButtonListSelectedPressedText(COLORREF clr);

	INT32         GetButtonListOffSetX();
	VOID          SetButtonListOffSetX(INT32 nOffSetX);

	INT32         GetButtonListOffSetY();
	VOID          SetButtonListOffSetY(INT32 nOffSetY);

	COLORREF      GetButtonListNormalText();
	VOID          SetButtonListNormalText(COLORREF clr);

	COLORREF      GetButtonListMouseOnText();
	VOID          SetButtonListMouseOnText(COLORREF clr);

	COLORREF      GetButtonListPressedText();
	VOID          SetButtonListPressedText(COLORREF clr);

	SIZE          GetButtonListRound();
	VOID          SetButtonListRound(SIZE sz);
	// CRXSkinButtonList参数设定 结束

	//  CRXSkinSearch 相关参数

	UINT32         GetBrownserOffSetX();
	VOID          SetBrownserOffSetX(const UINT32 dwOffSetX);
	UINT32        GetBrownserButtonWidth() { return m_dwBrownserButtonWidth; }
	UINT32        GetBrownserButtonHeight() { return m_dwBrownserButtonHeight; }
	//  CRXSkinSearch 相关参数  结束

	//  CRXSkinImageSel 相关参数
	INT32         GetImageSelOffSetX();
	VOID          SetImageSelOffSetX(INT32 nOffSetX);

	INT32         GetImageSelOffSetY();
	VOID          SetImageSelOffSetY(INT32 nOffSetY);

	SIZE          GetImageSelItem();
	VOID          SetImageSelItem(SIZE sz);
	//  CRXSkinImageSel 相关参数  结束


	//  CRXSkinListCtrl 相关参数
	INT32         GetListCtrlOffSetX();
	VOID          SetListCtrlOffSetX(const INT32 nListCtrlOffSetX);

	INT32         GetListCtrlOffSetY();
	VOID          SetListCtrlOffSetY(const INT32 nListCtrlOffSetY);

	INT32         GetListCtrlHeaderHeight();
	VOID          SetListCtrlHeaderHeight(const INT32 nHeight);

	INT32         GetListCtrlItemHeight();
	VOID          SetListCtrlItemHeight(const INT32 nHeight);

	COLORREF      GetListCtrlBorder();
	VOID          SetListCtrlBorder(COLORREF clr);

	COLORREF      GetListCtrlBK();
	VOID          SetListCtrlBK(COLORREF clr);

	COLORREF      GetListCtrlHeaderFrom();
	VOID          SetListCtrlHeaderFrom(COLORREF clr);
	
	COLORREF      GetListCtrlHeaderTo();
	VOID          SetListCtrlHeaderTo(COLORREF clr);

	COLORREF      GetListCtrlItemSelecedFrom();
	VOID          SetListCtrlItemSelecedFrom(COLORREF clr);

	COLORREF      GetListCtrlItemSelecedTo();
	VOID          SetListCtrlItemSelecedTo(COLORREF clr);

	COLORREF      GetListCtrlItemMouseOnFrom();
	VOID          SetListCtrlItemMouseOnFrom(COLORREF clr);

	COLORREF      GetListCtrlItemMouseOnTo();
	VOID          SetListCtrlItemMouseOnTo(COLORREF clr);

	COLORREF      GetListCtrlItemSelecedText();
	VOID          SetListCtrlItemSelecedText(COLORREF clr);

	COLORREF      GetListCtrlHeaderNormal() { return m_clrListCtrlHeaderNormal; }
	COLORREF      GetListCtrlHeaderMouseOn() { return m_clrListCtrlHeaderMouseOn; }
	COLORREF      GetListCtrlHeaderPressed() { return m_clrListCtrlHeaderPressed; }

	COLORREF      GetListItemNormalTxtColor() { return m_clrListCtrlItemNormalText; }
	COLORREF      GetListItemMouseOnTxtColor() { return m_clrListCtrlItemMouseOnText; }
	COLORREF      GetListItemPressedTxtColor() { return m_clrListCtrlItemPressedText; }
	//  CRXSkinListCtrl 相关参数  结束


	//  CRXSkinTreeCtrl 相关参数 开始
	inline INT32  GetTreeCtrlOffSetX() { return m_nTreeCtrlOffSetX; };
	inline INT32  GetTreeCtrlOffSetY() { return m_nTreeCtrlOffSetY; }
	inline INT32  GetTreeCtrlItemHeight() { return m_nTreeCtrlItemHeight; };

	inline COLORREF GetTreeCtrlBorderColor() { return m_clrTreeCtrlBorder; }
	inline COLORREF GetTreeCtrlBKColor() { return m_clrTreeCtrlBK; };

	inline COLORREF GetTreeCtrlItemNormalText() { return m_clrTreeCtrlItemNormalText; };
	inline COLORREF GetTreeCtrlItemMouseOnFrom() { return m_clrTreeCtrlItemMouseOnFrom; };
	inline COLORREF GetTreeCtrlItemMouseOnTo() { return m_clrTreeCtrlItemMouseOnTo; };
	inline COLORREF GetTreeCtrlItemMouseOnText() { return m_clrTreeCtrlItemMouseOnText; };
	inline COLORREF GetTreeCtrlItemPressedFrom() { return m_clrTreeCtrlItemPressedFrom; };
	inline COLORREF GetTreeCtrlItemPressedTo() { return m_clrTreeCtrlItemPressedTo; };
	inline COLORREF GetTreeCtrlItemPressedText() { return m_clrTreeCtrlItemPressedText; };
	//  CRXSkinTreeCtrl 相关参数 结束
	int32_t         GetShellImageListIndex(const TCHAR* pszName);


	//  CRXSkinSpinEdit 相关参数
	INT32         GetSpinEditArrowHeight();
	VOID          SetSpinEditArrowHeight(INT32 nSpinEditArrowHeight);


	//  Slider参数
	UINT32        GetSliderHorLeftOffset() { return m_dwHorLeftOffSet; }
	UINT32        GetSliderHorRightOffset() { return m_dwHorRightOffSet; }
	UINT32        GetSliderVerTopOffset() { return m_dwVerTopOffSet; }
	UINT32        GetSliderVerBottomOffset() { return m_dwVerBottomOffSet; }
	COLORREF      GetSliderTrans() { return m_clrSliderTrans; }

	UINT32        GetSliderHorSelectedLeftOffset() { return m_dwHorSelectedLeftOffSet; }
	UINT32        GetSliderHorSelectedRightOffset() { return m_dwHorSelectedRightOffSet; }
	UINT32        GetSliderVerSelectedTopOffset() { return m_dwVerSelectedTopOffSet; }
	UINT32        GetSliderVerSelectedBottomOffset() { return m_dwVerSelectedBottomOffSet; }

	SIZE          GetProgressRound() { return m_szProgressRound; };
	void          SetProgressRound(const SIZE& sz) { m_szProgressRound = sz; }
	COLORREF      GetProgressBorder() { return m_clrProgressBorder; }
	void          SetProgressBorder(COLORREF clr) { m_clrProgressBorder = clr; }
	COLORREF      GetProgressTxt() { return m_clrProgressTxt; }
	void          SetProgressTxt(COLORREF clr) { m_clrProgressTxt = clr; }
	COLORREF      GetProgressHot() { return m_clrProgressHot; }
	void          SetProgressHot(COLORREF clr) { m_clrProgressHot = clr; }

	COLORREF      GetProgressBK() { return m_clrProgressBK; }
	void          SetProgressBK(COLORREF clr) { m_clrProgressBK = clr; }

	//  CRXSkinSpinEdit 相关参数 结束
    VOID         DrawControlBorder(HDC hDC,
        EControlState enState,
        RECT rcControl,
        BOOL bIsEnable = TRUE);
	VOID         DrawMultLineText(HDC hDC,
								  CRXSkinRect rect,
								  INT32 nRowDis,
								  UINT nFromat,
								  CRXSkinString strText);
    CRXControl* CreateControl(tinyxml2::XMLElement* poElementControl, CRXControlMgr* poControlMgr, CRXSkinWnd* poParent, const EResourceType& enFromResource = EResourceType::RESOURCE_FROM_FILE);
	BOOL  SaveBmp(HBITMAP hBitmap);
	static void DrawColor(HDC hDC, const RECT& rc, DWORD color);
	static HBITMAP GenerateBitmap(CRXControl* pManager, CRXControl* pControl, RECT rc, DWORD dwFilterColor = 0);
	static HBITMAP CopyDCToBitmap(HDC hScrDC, LPRECT lpRect);
private:
	HFONT        m_hNormalFont;
	HFONT        m_hBoldFont;
	HFONT        m_hTitleFont;
	HFONT        m_hEditFont;
	HCURSOR      m_hHand;
	HCURSOR      m_hArrow;
	HCURSOR      m_hNS;
	HCURSOR      m_hWE;
	HCURSOR      m_hNWSE;
	HCURSOR      m_hNESW;
	POINT        m_szButton;
	COLORREF     m_clrTitle;
	TCHAR        m_szTitleFontName[256];
	INT32        m_nTitleFontSize;

	TCHAR        m_szCommonFontName[256];
	INT32        m_nCommonFontSize;
	COLORREF     m_clrCommon;

	TCHAR        m_szBoldFontName[256];
	INT32        m_nBoldFontSize;
	COLORREF     m_clrButtonBorder;
	COLORREF     m_clrButtonNormalFrom;
	COLORREF     m_clrButtonNormalMiddle;
	COLORREF     m_clrButtonNormalTo;
	COLORREF     m_clrButtonNormalTxt;

	COLORREF     m_clrButtonMouseOnFrom;
	COLORREF     m_clrButtonMouseOnMiddle;
	COLORREF     m_clrButtonMouseOnTo;
	COLORREF     m_clrButtonMouseOnTxt;

	COLORREF     m_clrButtonPressedFrom;
	COLORREF     m_clrButtonPressedMiddle;
	COLORREF     m_clrButtonPressedTo;
	COLORREF     m_clrButtonPressedTxt;

	SIZE         m_szButtonRound;

	COLORREF     m_clrCommonButtonBorder;
	COLORREF     m_clrCommonButtonNormalFrom;
	COLORREF     m_clrCommonButtonNormalMiddle;
	COLORREF     m_clrCommonButtonNormalTo;

	COLORREF     m_clrCommonButtonMouseOnFrom;
	COLORREF     m_clrCommonButtonMouseOnMiddle;
	COLORREF     m_clrCommonButtonMouseOnTo;

	COLORREF     m_clrCommonButtonPressedFrom;
	COLORREF     m_clrCommonButtonPressedMiddle;
	COLORREF     m_clrCommonButtonPressedTo;

	SIZE         m_szCommonButtonRound;

	UINT32       m_dwComboBoxArrowWidth;
	UINT32       m_dwComboBoxArrowOffSet;

	SControlBorder   m_stLeft;
	SControlBorder   m_stRight;
	SControlBorder   m_stTop;
	SControlBorder   m_stBottom;

	UINT32           m_dwControlBoderSize;


	UINT32        m_dwComboBoxPopupOffSetX;
	UINT32        m_dwComboBoxPopupOffSetY;
	UINT32        m_dwComboBoxPopupItemHeight;
	COLORREF      m_clrComboBoxPopupBK;
	COLORREF      m_clrComboBoxPopupBorder;

	COLORREF      m_clrComboBoxPopupItemMouseonBK;
	COLORREF      m_clrComboBoxPopupItemMouseonText;
	COLORREF      m_clrComboBoxPopupItemPressedBK;
	COLORREF      m_clrComboBoxPopupItemPressedText;
	COLORREF      m_clrComboBoxPopupItemSelectedBK;
	COLORREF      m_clrComboBoxPopupItemSelectedText;
	COLORREF      m_clrComboBoxPopupItemNormalText;

	//  CRXSkinSearch 相关参数
	INT32         m_dwBrownserOffSetX;
	UINT32        m_dwBrownserButtonWidth;
	UINT32        m_dwBrownserButtonHeight;
	
	//  CRXSkinSearch 相关参数  结束

	//  CRXSkinTabCtrl相关参数
	COLORREF      m_clrTabItemNormalFromBk;
	COLORREF      m_clrTabItemNormalToBk;
	COLORREF      m_clrTabItemNormalText;

	COLORREF      m_clrTabItemMouseOnFromBk;
	COLORREF      m_clrTabItemMouseOnToBk;
	COLORREF      m_clrTabItemMouseOnText;

	COLORREF      m_clrTabItemPressedFromBk;
	COLORREF      m_clrTabItemPressedToBk;
	COLORREF      m_clrTabItemPressedText;

	COLORREF      m_clrTabItemSelectedFromBk;
	COLORREF      m_clrTabItemSelectedToBk;
	COLORREF      m_clrTabItemSelectedText;

	COLORREF      m_clrTabItemBorder;

	COLORREF	  m_clrHyperLinkText;
	COLORREF	  m_clrHyperLinkHotText;

	UINT32        m_dwTabItemHeight;
	UINT32        m_dwTabItemOffSetX;
	SIZE          m_szTabCtrlRound;

	// CRXSkinToolTip参数设定
	COLORREF      m_clrToolTipBk;
	COLORREF      m_clrToolTipBorder;
	UINT32        m_dwToolTipOffSetX;
	UINT32        m_dwToolTipOffSetY;
	// CRXSkinToolTip参数设定 结束

	// CRXSkinMenu参数设定
	COLORREF      m_clrMenuItemNormalFromBk;
	COLORREF      m_clrMenuItemNormalToBk;
	COLORREF      m_clrMenuItemNormalText;

	COLORREF      m_clrMenuItemMouseOnFromBk;
	COLORREF      m_clrMenuItemMouseOnToBk;
	COLORREF      m_clrMenuItemMouseOnText;

	COLORREF      m_clrMenuItemPressedFromBk;
	COLORREF      m_clrMenuItemPressedToBk;
	COLORREF      m_clrMenuItemPressedText;

	COLORREF      m_clrMenuSeperator;

	COLORREF      m_clrMenuBk;
	COLORREF      m_clrMenuBorder;
	COLORREF      m_clrSidebarBk;
	SIZE          m_szMenuRound;
	UINT32        m_dwSidebarWidth;
	UINT32        m_dwMenuSeperatorHeight;
	UINT32        m_dwMenuOffSetX;
	UINT32        m_dwMenuOffSetY;
	UINT32        m_dwMenuItemHeight;
	UINT32        m_dwMenuItemArrowWidth;
	// CRXSkinMenu参数设定  结束

	// CRXSkinDatePicker参数设定
	SIZE          m_szDatePicker;
	// CRXSkinDatePicker参数设定  结束

	// CRXSkinLocationPicker参数设定
	SIZE          m_szLocationPicker;
	// CRXSkinLocationPicker参数设定  结束

	// CRXSkinOutbar参数设定
	SIZE          m_szOutbarRound;
	COLORREF      m_clrOutbarItemSecletedNormalFrom;
	COLORREF      m_clrOutbarItemSecletedNormalTo;
	COLORREF      m_clrOutbarItemSecletedMouseOnFrom;
	COLORREF      m_clrOutbarItemSecletedMouseOnTo;
	COLORREF      m_clrOutbarItemSecletedText;
	COLORREF      m_clrOutbarItemNormalText;
	COLORREF      m_clrOutbarItemMouseOnText;
	SIZE          m_szOutbarOffSet;
	INT32         m_nOutbarItemIndentationX;
	INT32         m_nOutbarItemHeight;

	SIZE          m_szOutbarFolderOffSet;
	INT32         m_nOutbarFolderHeight;

	COLORREF      m_clrOutbarFolderNormalBorder;
	COLORREF      m_clrOutbarFolderNormalFrom;
	COLORREF      m_clrOutbarFolderNormalTo;
	COLORREF      m_clrOutbarFolderNormalText;

	COLORREF      m_clrOutbarFolderPressedBorder;
	COLORREF      m_clrOutbarFolderPressedFrom;
	COLORREF      m_clrOutbarFolderPressedTo;
	COLORREF      m_clrOutbarFolderPressedText;

	COLORREF      m_clrOutbarFolderMouseOnFirstBorder;
	COLORREF      m_clrOutbarFolderMouseOnSecondeBorder;
	COLORREF      m_clrOutbarFolderMouseOnThirdBorder;
	COLORREF      m_clrOutbarFolderMouseOnFrom;
	COLORREF      m_clrOutbarFolderMouseOnTo;
	COLORREF      m_clrOutbarFolderMouseOnText;

	COLORREF      m_clrOutbarBkFrom;
	COLORREF      m_clrOutbarBkTo;
	// CRXSkinOutbar参数设定  结束

	// CRXSkinButtonList参数设定
	COLORREF      m_clrButtonListSelectedNormalFrom;
	COLORREF      m_clrButtonListSelectedNormalTo;
	COLORREF      m_clrButtonListSelectedNormalText;

	COLORREF      m_clrButtonListSelectedMouseOnFrom;
	COLORREF      m_clrButtonListSelectedMouseOnTo;
	COLORREF      m_clrButtonListSelectedMouseOnText;

	COLORREF      m_clrButtonListSelectedPressedFrom;
	COLORREF      m_clrButtonListSelectedPressedTo;
	COLORREF      m_clrButtonListSelectedPressedText;

	INT32         m_nButtonListOffSetX;
	INT32         m_nButtonListOffSetY;

	SIZE          m_szButtonListRound;

	COLORREF      m_clrButtonListNormalText;
	COLORREF      m_clrButtonListMouseOnText;
	COLORREF      m_clrButtonListPressedText;

	// CRXSkinButtonList参数设定 结束

	//  全局参数
	COLORREF      m_clrCommonBk;
	COLORREF      m_clrCommonBorder;
	SIZE          m_szControlBorder;
	COLORREF      m_clrDisableText;
	COLORREF      m_clrNormalText;
	//  箭头参数
	COLORREF      m_clrArrowNormal;
	COLORREF      m_clrArrowMouseOn;
	COLORREF      m_clrArrowPressed;
	UINT32        m_dwArrowHeight;

	ECursorType   m_enCursorType;

	//  CRXSkinImageSel 相关参数
	INT32         m_nImageSelOffSetX;
	INT32         m_nImageSelOffSetY;
	SIZE          m_szImageSelItem;
	//  CRXSkinImageSel 相关参数  结束

	// CRXSkinListCtrl  相关参数
	INT32         m_nListCtrlOffSetX;
	INT32         m_nListCtrlOffSetY;
	INT32         m_nListCtrlItemHeight;
	INT32         m_nListCtrlHeaderHeight;

	COLORREF      m_clrListCtrlHeaderNormal;
	COLORREF      m_clrListCtrlHeaderMouseOn;
	COLORREF      m_clrListCtrlHeaderPressed;

	COLORREF      m_clrListCtrlHeaderTxtNormal;
	COLORREF      m_clrListCtrlHeaderTxtMouseOn;
	COLORREF      m_clrListCtrlHeaderTxtPressed;

	COLORREF      m_clrListCtrlBorder;
	COLORREF      m_clrListCtrlBK;

	COLORREF      m_clrListCtrlHeaderFrom;
	COLORREF      m_clrListCtrlHeaderTo;

	COLORREF      m_clrListCtrlRowSelectedFrom;
	COLORREF      m_clrListCtrlRowSelectedTo;
	COLORREF      m_clrListCtrlRowMouseOnFrom;
	COLORREF      m_clrListCtrlRowMouseOnTo;
	COLORREF      m_clrListCtrlRowSelectedText;
	COLORREF      m_clrListCtrlItemNormalText;
	COLORREF      m_clrListCtrlItemMouseOnText;
	COLORREF      m_clrListCtrlItemPressedText;

	// CRXSkinListCtrl  相关参数  结束

	// CRXSkinTreeCtrl  相关参数
	INT32         m_nTreeCtrlOffSetX;
	INT32         m_nTreeCtrlOffSetY;
	INT32         m_nTreeCtrlItemHeight;

	COLORREF      m_clrTreeCtrlBorder;
	COLORREF      m_clrTreeCtrlBK;

	COLORREF      m_clrTreeCtrlItemNormalText;
	COLORREF      m_clrTreeCtrlItemMouseOnFrom;
	COLORREF      m_clrTreeCtrlItemMouseOnTo;
	COLORREF      m_clrTreeCtrlItemMouseOnText;
	COLORREF      m_clrTreeCtrlItemPressedFrom;
	COLORREF      m_clrTreeCtrlItemPressedTo;
	COLORREF      m_clrTreeCtrlItemPressedText;

	// CRXSkinTreeCtrl  相关参数  结束
	
	// CRXSkinProgress  相关参数  
	SIZE          m_szProgressRound;
	COLORREF      m_clrProgressBorder;
	COLORREF      m_clrProgressTxt;
	COLORREF      m_clrProgressHot;
	COLORREF      m_clrProgressBK;
	// CRXSkinProgress  相关参数  结束

	// CRXSkinSpinEdit 相关参数 

	INT32         m_nSpinEditArrowHeight;
	// CRXSkinSpinEdit 相关参数 结束

	lpfnUpdateLayeredWindow m_lpfnUpdateLayeredWindow;
	lpfnSetLayeredWindowAttributes m_lpfnSetLayeredWindowAttributes;
	HMODULE		m_hUser32;


	//  Slider 相关参数
	UINT32     m_dwHorLeftOffSet;
	UINT32     m_dwHorRightOffSet;
	UINT32     m_dwVerTopOffSet;
	UINT32     m_dwVerBottomOffSet;

	UINT32     m_dwHorSelectedLeftOffSet;
	UINT32     m_dwHorSelectedRightOffSet;
	UINT32     m_dwVerSelectedTopOffSet;
	UINT32     m_dwVerSelectedBottomOffSet;

	COLORREF   m_clrSliderTrans;
	int m_trh;
private:
	BOOL		_InitLayeredWindows();
	void		_MovePos(const POINT &posStart,
						 POINT &posCurr,
						 int nColWidth,
						 int nRowHeight,
						 int nCharWidth,
						 int nCount);
	BOOL        _IsChineseChar(TCHAR* str);
	VOID        _ReadConfig(tinyxml2::XMLDocument* poDoc,const EResourceType& enFromResource = RESOURCE_FROM_FILE,HMODULE hModel = NULL);
	VOID        _DrawControlBorderLeft(HDC hDC,EControlState enState,RECT rcControl,IRXImage* poImage,BOOL bIsEnable = TRUE);
	VOID        _DrawControlBorderRight(HDC hDC,EControlState enState,RECT rcControl,IRXImage* poImage,BOOL bIsEnable = TRUE);
	VOID        _DrawControlBorderTop(HDC hDC,EControlState enState,RECT rcControl,IRXImage* poImage,BOOL bIsEnable = TRUE);
	VOID        _DrawControlBorderBottom(HDC hDC,EControlState enState,RECT rcControl,IRXImage* poImage,BOOL bIsEnable = TRUE);

public:
	COLORREF    GetColorFromElementAttri(VOID* poElement, const char* pszAtti);
	SIZE        GetSizeFromElementAttri(VOID* poElement, const char* pszAtti);
	INT32       GetIN32FromElementAttri(VOID* poElement,const char* pszAtti);
	HIMAGELIST  GetShellImageList(BOOL bLarge);
};
