#include "pch.h"
#include "common/RXSkinService.h"
#include "common/drawmgr.h"
#include "common/ImageMgr.h"
#include "common/rxskinutil.h"
#include "fundation/rxstring.h"
#include "common/rxskindefines.h"
#include "fundation/tinyxml2.h"
#include "main\rxskindialog.h"
#include "rxfile.h"
#include "rxdir.h"
#include "rxstring.h"
#include <iostream>
#include <fstream>
#include "common/rximagemodel.h"
#include "controls/rxskinsearch.h"
#include "common/rxskinresourcemgr.h"
#include "common/rxskinfont.h"
#include "rxskinres.h"
CRXSkinRenderClip::~CRXSkinRenderClip()
{
	ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
	ASSERT(::GetObjectType(hRgn) == OBJ_REGION);
	ASSERT(::GetObjectType(hOldRgn) == OBJ_REGION);
	::SelectClipRgn(hDC, hOldRgn);
	::DeleteObject(hOldRgn);
	::DeleteObject(hRgn);
}

void CRXSkinRenderClip::GenerateClip(HDC hDC, RECT rc, CRXSkinRenderClip& clip)
{
	RECT rcClip = { 0 };
	::GetClipBox(hDC, &rcClip);
	clip.hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	clip.hRgn = ::CreateRectRgnIndirect(&rc);
	::ExtSelectClipRgn(hDC, clip.hRgn, RGN_AND);
	clip.hDC = hDC;
	clip.rcItem = rc;
}

void CRXSkinRenderClip::GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRXSkinRenderClip& clip)
{
	RECT rcClip = { 0 };
	::GetClipBox(hDC, &rcClip);
	clip.hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	clip.hRgn = ::CreateRectRgnIndirect(&rc);
	HRGN hRgnItem = ::CreateRoundRectRgn(rcItem.left, rcItem.top, rcItem.right + 1, rcItem.bottom + 1, width, height);
	::CombineRgn(clip.hRgn, clip.hRgn, hRgnItem, RGN_AND);
	::ExtSelectClipRgn(hDC, clip.hRgn, RGN_AND);
	clip.hDC = hDC;
	clip.rcItem = rc;
	::DeleteObject(hRgnItem);
}

void CRXSkinRenderClip::UseOldClipBegin(HDC hDC, CRXSkinRenderClip& clip)
{
	::SelectClipRgn(hDC, clip.hOldRgn);
}

void CRXSkinRenderClip::UseOldClipEnd(HDC hDC, CRXSkinRenderClip& clip)
{
	::SelectClipRgn(hDC, clip.hRgn);
}



CDrawMgr::CDrawMgr(void)
{
	m_lpfnSetLayeredWindowAttributes = NULL;
	m_lpfnUpdateLayeredWindow = NULL;
	m_hNormalFont = NULL;
	m_hEditFont = NULL;
	m_hHand = NULL;
	m_hBoldFont = NULL;
	m_hArrow = NULL;
	m_hNS = NULL;
	m_hWE = NULL;
	m_hNESW = NULL;
	m_hNWSE = NULL;
	m_hUser32 = NULL;
	m_szButton.x = m_szButton.y = 0;
	m_hTitleFont =NULL;
	memset(m_szTitleFontName,0,sizeof(TCHAR)*256);
	_tcscpy(m_szTitleFontName,_T("幼圆"));
	m_nTitleFontSize = 14;

	memset(m_szCommonFontName, 0, sizeof(TCHAR) * 256);
	_tcscpy(m_szCommonFontName, _T("仿宋"));
	m_nCommonFontSize = 14;
	m_clrCommon = RGB(30, 30, 30);

	memset(m_szBoldFontName, 0, sizeof(TCHAR) * 256);
	_tcscpy(m_szBoldFontName, _T("仿宋"));
	m_nBoldFontSize = 14;

	m_clrButtonBorder = RGB(178,178,178);
	m_clrButtonNormalFrom = RGB(250,250,250);
	m_clrButtonNormalTo = RGB(250,250,250);
	m_clrButtonNormalMiddle = RGB(220,220,220);
	m_clrButtonNormalTxt = RGB(0,0,0);

	m_clrButtonMouseOnFrom = RGB(250,250,250);
	m_clrButtonMouseOnTo = RGB(250,250,250);
	m_clrButtonMouseOnMiddle = RGB(220,220,220);
	m_clrButtonMouseOnTxt = RGB(0, 0, 0);

	m_clrButtonPressedFrom = RGB(250,250,250);
	m_clrButtonPressedTo = RGB(250,250,250);
	m_clrButtonPressedMiddle = RGB(220,220,220);
	m_clrButtonPressedTxt = RGB(0, 0, 0);

	m_szButtonRound.cx = m_szButtonRound.cy = 3;

	m_clrCommonButtonBorder = RGB(178,178,178);
	m_clrCommonButtonNormalFrom = RGB(250,250,250);
	m_clrCommonButtonNormalTo = RGB(250,250,250);
	m_clrCommonButtonNormalMiddle = RGB(220,220,220);

	m_clrCommonButtonMouseOnFrom = RGB(250,250,250);
	m_clrCommonButtonMouseOnTo = RGB(250,250,250);
	m_clrCommonButtonMouseOnMiddle = RGB(220,220,220);

	m_clrCommonButtonPressedFrom = RGB(250,250,250);
	m_clrCommonButtonPressedTo = RGB(250,250,250);
	m_clrCommonButtonPressedMiddle = RGB(220,220,220);
	m_szCommonButtonRound.cx = m_szCommonButtonRound.cy = 3;

	m_dwComboBoxArrowWidth = 23;
	m_dwComboBoxArrowOffSet = 2;
	m_dwControlBoderSize = 4;

	m_dwComboBoxPopupItemHeight = 16;
	m_dwComboBoxPopupOffSetX = m_dwComboBoxPopupOffSetY = 3;
	m_clrComboBoxPopupBK = RGB(250,250,250);
	m_clrComboBoxPopupBorder = RGB(2,82,150);

	m_clrComboBoxPopupItemNormalText = RGB(50,40,30);
	m_clrComboBoxPopupItemMouseonBK = RGB(54,112,170);
	m_clrComboBoxPopupItemMouseonText = RGB(254,252,250);
	m_clrComboBoxPopupItemPressedBK = RGB(94,162,190);
	m_clrComboBoxPopupItemPressedText = RGB(254,252,250);
	m_clrComboBoxPopupItemSelectedBK = RGB(94,162,190);
	m_clrComboBoxPopupItemSelectedText = RGB(254,252,250);


	/// CRXSkinTabCtrl相关参数
	m_clrTabItemNormalFromBk = RGB(139,194,225);
	m_clrTabItemNormalToBk = RGB(139,194,225);
	m_clrTabItemNormalText = RGB(54,52,50);

	m_clrTabItemMouseOnFromBk = RGB(182,216,238);
	m_clrTabItemMouseOnToBk = RGB(168,209,232);
	m_clrTabItemNormalText = RGB(54,52,50);

	m_clrTabItemPressedFromBk = RGB(205,227,240);
	m_clrTabItemPressedToBk = RGB(237,245,249);
	m_clrTabItemPressedText = RGB(54,52,50);

	m_clrTabItemSelectedFromBk = RGB(205,227,240);
	m_clrTabItemSelectedToBk = RGB(237,245,249);
	m_clrTabItemSelectedText = RGB(54,52,50);
	m_clrTabItemBorder = RGB(0,60,117);

	m_dwTabItemHeight = 20;
	m_szTabCtrlRound.cx = m_szTabCtrlRound.cy = 3;
	m_dwTabItemOffSetX = 3;
	// CRXSkinTabCtrl相关参数  结束

	// CRXSkinToolTip参数设定
	m_clrToolTipBk = RGB(206,192,193);
	m_clrToolTipBorder = RGB(106,162,193);
	m_dwToolTipOffSetX = m_dwToolTipOffSetY = 3;
	// CRXSkinToolTip参数设定 结束

	// CRXSkinMenu参数设定
	m_clrMenuItemNormalFromBk = RGB(253,253,253);
	m_clrMenuItemNormalToBk = RGB(253,253,253);
	m_clrMenuItemNormalText = RGB(0,20,35);

	m_clrMenuItemMouseOnFromBk = RGB(63,152,217);
	m_clrMenuItemMouseOnToBk = RGB(48,134,198);
	m_clrMenuItemMouseOnText = RGB(253,253,253);

	m_clrMenuItemPressedFromBk = RGB(63,152,217);
	m_clrMenuItemPressedToBk = RGB(48,134,198);
	m_clrMenuItemPressedText = RGB(253,253,253);

	m_clrMenuSeperator = RGB(183,195,204);

	m_clrMenuBk = RGB(253,253,253);
	m_clrMenuBorder = RGB(40,62,93);
	m_clrSidebarBk = RGB(209,228,236);
	m_szMenuRound.cx = m_szMenuRound.cy = 3;
	m_dwSidebarWidth = 25;
	m_dwMenuOffSetX = 3;
	m_dwMenuOffSetY = 3;
	m_dwMenuItemHeight = 20;
	m_dwMenuItemArrowWidth = 15;
	m_dwMenuSeperatorHeight = 10;
	// CRXSkinMenu参数设定  结束

	// CRXSkinDatePicker参数设定
	m_szDatePicker.cx = 120;
	m_szDatePicker.cy = 30;
	// CRXSkinDatePicker参数设定 结束


	// CRXSkinDatePicker参数设定
	m_szLocationPicker.cx = 120;
	m_szLocationPicker.cy = 30;
	// CRXSkinDatePicker参数设定 结束
	
	// 全局相关参数
	m_clrCommonBk = RGB(250,250,250);
	m_clrCommonBorder = RGB(2,82,150);
	m_clrHyperLinkText = RGB(20,50,250);
	m_clrHyperLinkHotText = RGB(220, 50, 50);
	m_clrNormalText = RGB(20,30,40);
	m_szControlBorder.cx = m_szControlBorder.cy = 3;
	// 箭头
	m_clrArrowNormal = RGB(50,50,50);
	m_clrArrowMouseOn = RGB(92,82,100);
	m_clrArrowPressed = RGB(109,102,100);
	m_dwArrowHeight = 8;
	// 全局相关参数 结束

	// CRXSkinOutbar 参数
	m_szOutbarOffSet.cx = 3;
	m_szOutbarOffSet.cy = 2;
	m_szOutbarRound.cx = m_szOutbarRound.cy = 3;
	m_nOutbarItemIndentationX = 10;
	m_nOutbarItemHeight = 20;
	m_clrOutbarItemSecletedMouseOnFrom = RGB(64,136,181);
	m_clrOutbarItemSecletedMouseOnTo = RGB(79,153,200);
	m_clrOutbarItemSecletedNormalFrom = RGB(79,153,200);
	m_clrOutbarItemSecletedNormalTo = RGB(64,136,181);
	m_clrOutbarItemSecletedText = RGB(255,255,255);
	m_clrOutbarItemNormalText = RGB(0,28,48);
	m_clrOutbarItemMouseOnText = RGB(3,112,218);

	m_szOutbarFolderOffSet.cx = m_szOutbarFolderOffSet.cy = 3;
	m_nOutbarFolderHeight = 25;

	m_clrOutbarFolderNormalBorder = RGB(188,206,223);
	m_clrOutbarFolderNormalFrom = RGB(244,246,247);
	m_clrOutbarFolderNormalTo = RGB(225,232,236);
	m_clrOutbarFolderNormalText = RGB(0,28,48);

	m_clrOutbarFolderPressedBorder = RGB(164,174,185);
	m_clrOutbarFolderPressedFrom = RGB(217,227,230);
	m_clrOutbarFolderPressedTo = RGB(252,252,252);
	m_clrOutbarFolderPressedText =RGB(0,28,48);

	m_clrOutbarFolderMouseOnFirstBorder =RGB(80,129,163);
	m_clrOutbarFolderMouseOnSecondeBorder = RGB(102,205,235);
	m_clrOutbarFolderMouseOnThirdBorder = RGB(200,234,245);
	m_clrOutbarFolderMouseOnFrom = RGB(246,248,249);
	m_clrOutbarFolderMouseOnTo = RGB(231,238,240);
	m_clrOutbarFolderMouseOnText =RGB(0,28,48);
	m_clrOutbarBkFrom = m_clrOutbarBkTo = RGB(240,245,248);
	// CRXSkinOutbar 参数 结束

	// CRXSkinButtonList参数设定
	m_clrButtonListSelectedNormalFrom = RGB(79,153,200);
	m_clrButtonListSelectedNormalTo = RGB(64,136,181);
	m_clrButtonListSelectedNormalText = RGB(255,255,255);

	m_clrButtonListSelectedMouseOnFrom = RGB(79,153,200);
	m_clrButtonListSelectedMouseOnTo = RGB(64,136,181);
	m_clrButtonListSelectedMouseOnText = RGB(255,255,255);

	m_clrButtonListSelectedPressedFrom = RGB(64,136,181);
	m_clrButtonListSelectedPressedTo = RGB(79,153,200);
	m_clrButtonListSelectedPressedText = RGB(255,255,255);

	m_nButtonListOffSetX = 4;
	m_nButtonListOffSetY = 2;

	m_clrButtonListNormalText = RGB(0,28,48);
	m_clrButtonListMouseOnText = RGB(3,112,218);
	m_clrButtonListPressedText = RGB(0,28,48);

	m_szButtonListRound.cx = m_szButtonListRound.cy = 3;

	// CRXSkinButtonList参数设定 结束

	//  CRXSkinSearch 相关参数
	m_dwBrownserOffSetX = 5;
	m_dwBrownserButtonWidth = 40;
	m_dwBrownserButtonHeight = 40;
	//  CRXSkinSearch 相关参数  结束

	//  CRXSkinImageSel 相关参数
	m_nImageSelOffSetX = 3;
	m_nImageSelOffSetY = 3;
	m_szImageSelItem.cx = m_szImageSelItem.cy = 45;
	//  CRXSkinImageSel 相关参数  结束

	// CRXSkinListCtrl  相关参数
	m_nListCtrlOffSetX = 3;
	m_nListCtrlOffSetY = 2;
	m_nListCtrlItemHeight = 23;
	m_nListCtrlHeaderHeight = 25;

	m_clrListCtrlBorder = RGB(191,216,233);
	m_clrListCtrlBK = RGB(232,245,251);

	m_clrListCtrlHeaderFrom = RGB(209,230,243);
	m_clrListCtrlHeaderTo = RGB(222,238,247);

	m_clrListCtrlHeaderNormal = RGB(100,100,100);
	m_clrListCtrlHeaderMouseOn = RGB(100, 100, 100);
	m_clrListCtrlHeaderPressed = RGB(100, 100, 100);

	m_clrListCtrlRowSelectedFrom = RGB(63,152,217);
	m_clrListCtrlRowSelectedTo= RGB(49,136,199);

	m_clrListCtrlRowMouseOnFrom = RGB(83, 172, 217);
	m_clrListCtrlRowMouseOnTo = RGB(69, 156, 199);

	m_clrListCtrlRowSelectedText = RGB(250,240,230);

	m_clrListCtrlItemNormalText = RGB(3,45,65);
	m_clrListCtrlItemMouseOnText = RGB(3, 45, 65);
	m_clrListCtrlItemPressedText = RGB(3, 45, 65);

	// CRXSkinListCtrl  相关参数  结束

	// CRXSkinTreeCtrl  相关参数
	m_nTreeCtrlOffSetX = 3;
	m_nTreeCtrlOffSetY = 2;
	m_nTreeCtrlItemHeight = 23;

	m_clrTreeCtrlBorder = RGB(191, 216, 233);;
	m_clrTreeCtrlBK = RGB(232, 245, 251);

	m_clrTreeCtrlItemNormalText = RGB(21, 16, 33);;
	m_clrTreeCtrlItemMouseOnFrom =  RGB(232, 245, 251);
	m_clrTreeCtrlItemMouseOnTo = RGB(222, 215, 221);
	m_clrTreeCtrlItemMouseOnText  = RGB(21, 16, 33);
	m_clrTreeCtrlItemPressedFrom = RGB(212, 225, 231);
	m_clrTreeCtrlItemPressedTo = RGB(192, 205, 211);
	m_clrTreeCtrlItemPressedText = RGB(21, 16, 33);;

	// CRXSkinSPinEdit  相关参数
	m_nSpinEditArrowHeight = 6;
	// CRXSkinSpinEdit  相关参数 结束
	m_enCursorType = CURSOR_ARROW;


	m_dwHorLeftOffSet = 10;
	m_dwHorRightOffSet = 10;
	m_dwVerTopOffSet = 10;
	m_dwVerBottomOffSet = 10;
	m_clrSliderTrans = RGB(255, 0, 255);

	m_dwHorSelectedLeftOffSet = 5;
	m_dwHorSelectedRightOffSet = 5;
	m_dwVerSelectedTopOffSet = 5;
	m_dwVerSelectedBottomOffSet = 5;
	m_clrTitle = RGB(250, 250, 250);
	m_szProgressRound.cx = m_szProgressRound.cy = 4;
	m_clrProgressBorder = RGB(192, 205, 211);
	m_clrProgressTxt = RGB(92, 125, 171);
	m_clrProgressHot = RGB(192, 225, 171);
	m_clrProgressBK = RGB(235, 225, 225);
}

CDrawMgr::~CDrawMgr(void)
{
	UnInit();
}

BOOL  CDrawMgr::InitResource(const TCHAR* pszResource)
{
	
	m_hNormalFont = CreateFont(
		m_nCommonFontSize,
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		m_szCommonFontName);
	m_hEditFont = CreateFont(
		20,
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		m_szCommonFontName);
	m_hBoldFont = CreateFont(
		m_nBoldFontSize,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		m_szBoldFontName);

	m_hTitleFont = CreateFont(
		m_nTitleFontSize,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		m_szTitleFontName);

	m_hArrow = ::LoadCursor(NULL, IDC_ARROW);
	m_hHand = ::LoadCursor(NULL, IDC_HAND);
	m_hNS = ::LoadCursor(NULL, IDC_SIZENS);
	m_hWE = ::LoadCursor(NULL, IDC_SIZEWE);

	m_hNESW = ::LoadCursor(NULL, IDC_SIZENESW);
	m_hNWSE = ::LoadCursor(NULL, IDC_SIZENWSE);
	_InitLayeredWindows();
	tinyxml2::XMLDocument* poDoc = CRXSkinResourceMgr::Instance()->GetXml(pszResource);
	if (poDoc)
	{
		_ReadConfig(poDoc, EResourceType::RESOURCE_FROM_EXE);
		delete poDoc;
	}
	return TRUE;
}

BOOL CDrawMgr::Init(const TCHAR* pszConfig)
{
	if (CRXSkinService::Instance()->IsFromZipResource())
	{
		std::vector<unsigned char> vec;
		CRXSkinResourceMgr::Instance()->LoadResource(pszConfig,vec);
		if (vec.size()>0)
		{
			tinyxml2::XMLDocument oDoc;
			char* pszData = (char*)malloc(vec.size() + 1);
			memset(pszData, 0, vec.size() + 1);
			memcpy(pszData, &vec[0], vec.size());
			if (tinyxml2::XML_SUCCESS == oDoc.Parse((const char*)pszData))
			{
				_ReadConfig(&oDoc,EResourceType::RESOURCE_FROM_ZIP);
			}
			free(pszData);
		}
	}
	if (CRXSkinService::Instance()->IsFromFileResource())
	{
		tinyxml2::XMLDocument oDoc;
		tstring strPath = CRXSkinService::Instance()->GetResourceRootPath() + pszConfig;
		if (tinyxml2::XML_SUCCESS == oDoc.LoadFile(_RXTT2A(strPath.c_str()).c_str()))
		{
			_ReadConfig(&oDoc);
		}
	}


	m_hNormalFont = CreateFont(
								m_nCommonFontSize,                      
								0,                       
								0,                       
								0,                       
								FW_NORMAL,                 
								FALSE,                   
								FALSE,                   
								0,                       
								ANSI_CHARSET,            
								OUT_DEFAULT_PRECIS,      
								CLIP_DEFAULT_PRECIS,     
								DEFAULT_QUALITY,         
								DEFAULT_PITCH | FF_SWISS,
								m_szCommonFontName);
	m_hEditFont = CreateFont(
								20,                      
								0,                       
								0,                       
								0,                       
								FW_NORMAL,                 
								FALSE,                   
								FALSE,                   
								0,                       
								ANSI_CHARSET,            
								OUT_DEFAULT_PRECIS,      
								CLIP_DEFAULT_PRECIS,     
								DEFAULT_QUALITY,         
								DEFAULT_PITCH | FF_SWISS,
								m_szCommonFontName);
	m_hBoldFont = CreateFont(
								m_nBoldFontSize,
								0,                       
								0,                       
								0,                       
								FW_BOLD,                 
								FALSE,                   
								FALSE,                   
								0,                       
								ANSI_CHARSET,            
								OUT_DEFAULT_PRECIS,      
								CLIP_DEFAULT_PRECIS,     
								DEFAULT_QUALITY,         
								DEFAULT_PITCH | FF_SWISS,
							m_szBoldFontName);

		m_hTitleFont = CreateFont(
								m_nTitleFontSize,                      
								0,                       
								0,                       
								0,                       
								FW_BOLD,                 
								FALSE,                   
								FALSE,                   
								0,                       
								ANSI_CHARSET,            
								OUT_DEFAULT_PRECIS,      
								CLIP_DEFAULT_PRECIS,     
								DEFAULT_QUALITY,         
								DEFAULT_PITCH | FF_SWISS,
								m_szTitleFontName);

	m_hArrow = ::LoadCursor(NULL, IDC_ARROW);
	m_hHand  = ::LoadCursor(NULL, IDC_HAND);
	m_hNS  = ::LoadCursor(NULL,IDC_SIZENS);
	m_hWE = ::LoadCursor(NULL,IDC_SIZEWE);

	m_hNESW  = ::LoadCursor(NULL,IDC_SIZENESW);
	m_hNWSE = ::LoadCursor(NULL,IDC_SIZENWSE);
	
	_InitLayeredWindows();

	return TRUE;
}

void CDrawMgr::LoadSelfBmp()
{
	HIMAGELIST hShellImageList = GetShellImageList(FALSE);
	UINT32 dwCount = ImageList_GetImageCount(hShellImageList);
	INT32 cx = 0;
	INT32 cy = 0;
	ImageList_GetIconSize(hShellImageList, &cx, &cy);
	tstring strImage = RXGetModulePath();
	strImage += _RXT("\\output");
	RXCreateDirectory(strImage.c_str(), TRUE);

	HDC hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	HDC	hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBmp = CreateCompatibleBitmap(hDC, cx, cy * dwCount);
	HGDIOBJ	hOld = SelectObject(hMemDC, hBmp);
	for (UINT32 t = 0; t < dwCount; t++)
	{
		ImageList_Draw(hShellImageList, t, hMemDC, 0, cy * t, ILD_TRANSPARENT | ILD_NORMAL);
	}
	hBmp = (HBITMAP)SelectObject(hMemDC, hOld);

	DeleteDC(hDC);
	DeleteDC(hMemDC);
	SaveBmp(hBmp);
	DeleteObject(hBmp);
	ImageList_Destroy(hShellImageList);
	TCHAR szFullPath[MAX_PATH] = { 0 };
	_RXTsprintf(szFullPath, _T("%s\\output\\previews\\self_icons.bmp"), RXGetModulePath());
	CRXSkinService::Instance()->GetImageMgr().AddImageList("self", szFullPath, cx, cy);
}

BOOL CDrawMgr::SaveBmp(HBITMAP hBitmap)
{
	HDC   hDC;
	//当前分辨率下每象素所占字节数     
	int   iBits;
	//位图中每象素所占字节数     
	WORD  wBitCount;
	//定义调色板大小，位图中像素字节大小，位图文件大小，写入文件字节数  
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//位图属性结构       
	BITMAP   Bitmap;
	//位图文件头结构     
	BITMAPFILEHEADER   bmfHdr;
	//位图信息头结构       
	BITMAPINFOHEADER   bi;
	//指向位图信息头结构         
	LPBITMAPINFOHEADER lpbi;
	//定义文件，分配内存句柄，调色板句柄       
	HANDLE  hDib, hPal, hOldPal = NULL;

	//计算位图文件每个像素所占字节数       
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)wBitCount = 1;
	else if (iBits <= 4) wBitCount = 4;
	else if (iBits <= 8) wBitCount = 8;
	else wBitCount = 24;

	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	//   为位图内容分配内存       
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	//   处理调色板         
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		//hDC   =   m_pDc->GetSafeHdc();     
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	//   获取该调色板下新的像素值       
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	//   恢复调色板         
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	tstring strImage = RXGetModulePath();
	strImage += _RXT("\\output");
	RXCreateDirectory(strImage.c_str(), TRUE);
	strImage += _RXT("\\previews");
	RXCreateDirectory(strImage.c_str(), TRUE);
	TCHAR szPath[MAX_PATH] = { 0 };
	_RXTsprintf(szPath, _RXT("%s\\%s.bmp"), strImage.c_str(), _T("self_icons"));
	//   创建位图文件   
	FILE* hFile = fopen(_RXTT2A(szPath).c_str(), ("wb"));
	if (hFile == NULL) return FALSE;
	//   设置位图文件头       
	bmfHdr.bfType = 0x4D42;   //   ;BM;  
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	fwrite((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), 1, hFile);
	fwrite((LPSTR)lpbi, dwDIBSize, 1, hFile);
	//   写入位图文件其余内容  
	//   清除  
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	fclose(hFile);
	return TRUE;
}

VOID CDrawMgr::UnInit()
{
	::DeleteObject(m_hBoldFont);
	::DeleteObject(m_hEditFont);
	::DeleteObject(m_hTitleFont);
	if (m_hHand)
	{
		::DestroyCursor(m_hHand);
		m_hHand = NULL;
	}
	if (m_hArrow)
	{
		::DestroyCursor(m_hArrow);
		m_hArrow = NULL;
	}
	if (m_hNS)
	{
		::DestroyCursor(m_hNS);
		m_hNS = NULL;
	}
	if (m_hWE)
	{
		::DestroyCursor(m_hWE);
		m_hWE = NULL;
	}
	if (m_hNWSE)
	{
		::DestroyCursor(m_hNWSE);
		m_hNWSE = NULL;
	}
	if (m_hNESW)
	{
		::DestroyCursor(m_hNESW);
		m_hNESW = NULL;
	}

	if (m_hUser32)
	{
		FreeLibrary(m_hUser32);
		m_hUser32 = NULL;
	}
}

VOID CDrawMgr::SetCursor(ECursorType enType)
{
// 	if (enType == m_enCursorType)
// 	{
// 		return;
// 	}
	m_enCursorType = enType;
	switch(enType)
	{
	case CURSOR_ARROW:
		{
			::SetCursor(m_hArrow);
		}
		break;
	case CURSOR_HAND:
		{
			::SetCursor(m_hHand);
		}
		break;
	case CURSOR_NS:
		{
			::SetCursor(m_hNS);
		}
		break;
	case CURSOR_WE:
		{
			::SetCursor(m_hWE);
		}
		break;
	case CURSOR_NESW:
		{
			::SetCursor(m_hNESW);
		}	
		break;
	case CURSOR_NWSE:
		{
			::SetCursor(m_hNWSE);
		}
		break;
	default:
		break;
	}
}

UINT32 CDrawMgr::GetComboBoxArrowWidth()
{
	return m_dwComboBoxArrowWidth;
}
UINT32 CDrawMgr::GetComboBoxArrowOffSet()
{
	return m_dwComboBoxArrowOffSet;
}
CDrawMgr::SControlBorder& CDrawMgr::GetControlBorderLeft()
{
	return m_stLeft;
}

CDrawMgr::SControlBorder& CDrawMgr::GetControlBorderRight()
{
	return m_stRight;
}

CDrawMgr::SControlBorder& CDrawMgr::GetControlBorderTop()
{
	return m_stTop;
}

CDrawMgr::SControlBorder& CDrawMgr::GetControlBorderBottom()
{
	return m_stBottom;
}

UINT32 CDrawMgr::GetControlBorderSize()
{
	return m_dwControlBoderSize;
}

UINT32 CDrawMgr::GetComboBoxPopupItemHeight()
{
	return m_dwComboBoxPopupItemHeight;
}

UINT32 CDrawMgr::GetComboBoxPopupOffSetX()
{
	return m_dwComboBoxPopupOffSetX;
}

UINT32 CDrawMgr::GetComboBoxPopupOffSetY()
{
	return m_dwComboBoxPopupOffSetY;
}

COLORREF CDrawMgr::GetComboBoxPopupColorBK()
{
	return m_clrComboBoxPopupBK;
}

COLORREF CDrawMgr::GetComboBoxPopupColorBorder()
{
	return m_clrComboBoxPopupBorder;
}

COLORREF CDrawMgr::GetComboBoxPopupItemMouseOnBkColor()
{
	return m_clrComboBoxPopupItemMouseonBK;
}

COLORREF CDrawMgr::GetComboBoxPopupItemMouseOnTextColor()
{
	return m_clrComboBoxPopupItemMouseonText;
}

COLORREF CDrawMgr::GetComboBoxPopupItemNormalTextColor()
{
	return m_clrComboBoxPopupItemNormalText;
}

COLORREF CDrawMgr::GetComboBoxPopupItemPressedBkColor()
{
	return m_clrComboBoxPopupItemPressedBK;
}

COLORREF CDrawMgr::GetComboBoxPopupItemPressedTextColor()
{
	return m_clrComboBoxPopupItemPressedText;
}

COLORREF CDrawMgr::GetComboBoxPopupItemSelectedBkColor()
{
	return m_clrComboBoxPopupItemSelectedBK;
}
// CRXSkinTabCtrl 相关参数设定
COLORREF CDrawMgr::GetComboBoxPopupItemSelectedTextColor()
{
	return m_clrComboBoxPopupItemSelectedText;
}

COLORREF CDrawMgr::GetTabItemNormalFromBk()
{
	return m_clrTabItemNormalFromBk;
}

COLORREF CDrawMgr::GetTabItemNormalToBk()
{
	return m_clrTabItemNormalToBk;
}

COLORREF CDrawMgr::GetTabItemNormalText()
{
	return m_clrTabItemNormalText;
}

COLORREF CDrawMgr::GetTabItemMouseOnFromBk()
{
	return m_clrTabItemMouseOnFromBk;
}

COLORREF CDrawMgr::GetTabItemMouseOnToBk()
{
	return m_clrTabItemMouseOnToBk;
}

COLORREF CDrawMgr::GetTabItemMouseOnText()
{
	return m_clrTabItemMouseOnText;
}

COLORREF CDrawMgr::GetTabItemPressedFromBk()
{
	return m_clrTabItemPressedFromBk;
}

COLORREF CDrawMgr::GetTabItemPressedToBk()
{
	return m_clrTabItemPressedToBk;
}

COLORREF CDrawMgr::GetTabItemPressedText()
{
	return m_clrTabItemPressedText;
}

COLORREF CDrawMgr::GetTabItemSelectedFromBk()
{
	return m_clrTabItemSelectedFromBk;
}

COLORREF CDrawMgr::GetTabItemSelectedToBk()
{
	return m_clrTabItemSelectedToBk;
}

COLORREF CDrawMgr::GetTabItemSelectedText()
{
	return m_clrTabItemSelectedText;
}

COLORREF CDrawMgr::GetTabItemBorder()
{
	return m_clrTabItemBorder;
}

UINT32 CDrawMgr::GetTabCtrlItemHeight()
{
	return m_dwTabItemHeight;
}

UINT32 CDrawMgr::GetTabItemOffSetX()
{
	return m_dwTabItemOffSetX;
}

SIZE CDrawMgr::GetTabCtrlRound()
{
	return m_szTabCtrlRound;
}
// CRXSkinTabCtrl 相关参数设定  结束


VOID CDrawMgr::_ReadConfig(tinyxml2::XMLDocument* poDoc, 
	const EResourceType& enFromResource, 
	HMODULE hModel)
{
	tinyxml2::XMLElement* poRoot = poDoc->RootElement();
	if (poRoot)
	{
		tinyxml2::XMLElement* poFonts = poRoot->FirstChildElement("Fonts");
		if (poFonts)
		{
			tinyxml2::XMLElement* poTitle = poFonts->FirstChildElement("Title");
			if (poTitle)
			{
				CHAR* pszName = (CHAR*)poTitle->Attribute("name");
				if (pszName)
				{
					SetTitleFontName(_RXTUTF82T(pszName).c_str());
				}
				CHAR* pszColor = (CHAR*)poTitle->Attribute("color");
				if (pszColor)
				{
					SetTitleColor(GetColor(pszColor));
				}				
				CHAR* pszSize = (CHAR*)poTitle->Attribute("size");
				if (pszSize)
				{
					SetTitleFontSize(atoi(pszSize));
				}				
			}

			tinyxml2::XMLElement* poCommon = poFonts->FirstChildElement("Common");
			if (poCommon)
			{
				CHAR* pszName = (CHAR*)poCommon->Attribute("name");
				if (pszName)
				{
					SetCommonFontName(_RXTUTF82T(pszName).c_str());
				}
				CHAR* pszColor = (CHAR*)poCommon->Attribute("color");
				if (pszColor)
				{
					SetCommonColor(GetColor(pszColor));
				}
				CHAR* pszSize = (CHAR*)poCommon->Attribute("size");
				if (pszSize)
				{
					SetCommonFontSize(atoi(pszSize));
				}
			}
			tinyxml2::XMLElement* poFilesElement = poFonts->FirstChildElement("Files");
			if (poFilesElement)
			{
				tinyxml2::XMLElement* poFileElement = poFilesElement->FirstChildElement("File");
				while (poFileElement)
				{
					const char* pszSrc = poFileElement->Attribute("src");
					if (pszSrc)
					{
						switch (enFromResource)
						{
						case RESOURCE_FROM_FILE:
						{
							TCHAR szPath[MAX_PATH] = {0};
							_RXTsnprintf(szPath, MAX_PATH, _T("%s%s"),
								CRXSkinService::Instance()->GetResourceRootPath().c_str(),
								_RXTUTF82T(pszSrc).c_str());
					//		CRXSkinFontMgr::Instance()->LoadFontFile(szPath);
						}
						break;
						case RESOURCE_FROM_EXE:
						{
							TCHAR * pszResource = MAKEINTRESOURCE(atoi(pszSrc));
							if (pszResource)
							{
								uint32_t dwLen = 0;
								BYTE* lpData = CRXSkinResourceMgr::Instance()->GetResource(pszResource, _T("FONTFILE"),dwLen);
								if (lpData && dwLen>0)
								{
							//		CRXSkinFontMgr::Instance()->LoadFontFileData(lpData, dwLen);
								}
							}						
						}
						break;
						case RESOURCE_FROM_ZIP:
						{
							std::vector<unsigned char> vec;
							CRXSkinResourceMgr::Instance()->LoadResourceFromZip(_RXTUTF82T(pszSrc).c_str(), vec);
							if (vec.size()>0)
							{
						//		CRXSkinFontMgr::Instance()->LoadFontFileData(&vec[0], vec.size());
								vec.clear();
							}
						}
						break;
						default:
							break;
						}
					}
					poFileElement = poFileElement->NextSiblingElement("File");
				}

			}
			tinyxml2::XMLElement* poSkiaFont = poFonts->FirstChildElement("SkiaFont");
			if (poSkiaFont)
			{
				tinyxml2::XMLElement* poFontElement = poSkiaFont->FirstChildElement("Font");
				while (poFontElement)
				{
					LOGFONT oFont;
					memset(&oFont,0,sizeof(LOGFONT));
					CHAR* pszName = (CHAR*)poFontElement->Attribute("name");
					if (pszName)
					{
						_RXTStrcpy(oFont.lfFaceName, _RXTUTF82T(pszName).c_str());
					}
					CHAR* pszSize = (CHAR*)poFontElement->Attribute("size");
					if (pszSize)
					{
						oFont.lfHeight = (atoi(pszSize));
					}
					CHAR* pszItalic = (CHAR*)poFontElement->Attribute("Italic");
					if (pszItalic && strcmp(pszItalic,"true")==0)
					{
						oFont.lfItalic = true;
					}

					CHAR* pszUnderline = (CHAR*)poFontElement->Attribute("Underline");
					if (pszUnderline && strcmp(pszUnderline, "true") == 0)
					{
						oFont.lfUnderline = true;
					}

					CHAR* pszStrikeOut = (CHAR*)poFontElement->Attribute("StrikeOut");
					if (pszStrikeOut && strcmp(pszStrikeOut, "true") == 0)
					{
						oFont.lfStrikeOut = true;
					}
					CHAR* pszBold = (CHAR*)poFontElement->Attribute("Bold");
					if (pszBold && strcmp(pszBold, "true") == 0)
					{
						oFont.lfWeight = FW_BOLD;
					}

					uint32_t dwID = 0;
					CHAR* pszID = (CHAR*)poFontElement->Attribute("id");
					if (pszID )
					{
						dwID = atoi(pszID);
					}
					CRXSkinFont* poFont = new CRXSkinFont;
					if (poFont->InitFont(oFont))
					{
						CRXSkinFontMgr::Instance()->AddFont(dwID, poFont);
					}
					else
					{
						delete poFont;
					}

					poFontElement = poFontElement->NextSiblingElement("Font");
				}

			}
		}
		tinyxml2::XMLElement* poSizeElement = poRoot->FirstChildElement("Sizes");
		if (poSizeElement)
		{
			tinyxml2::XMLElement* poControlElement = poSizeElement->FirstChildElement("Control");
			if (poControlElement)
			{
				CHAR* pszBorder = (CHAR*)poControlElement->Attribute("border");
				if(pszBorder)
				{
					m_szControlBorder = GetSize(pszBorder);
				}
			}
			tinyxml2::XMLElement* poArrowElement = poSizeElement->FirstChildElement("Arrow");
			if (poArrowElement)
			{
				CHAR* pszHeight = (CHAR*)poArrowElement->Attribute("height");
				if(pszHeight)
				{
					m_dwArrowHeight = atoi(pszHeight);
				}
			}
		}
		tinyxml2::XMLElement* poProgress = poRoot->FirstChildElement("Progress");
		if (poProgress)
		{
			CHAR* pszBordercolort = (CHAR*)poProgress->Attribute("Text");
			if (pszBordercolort)
			{
				SetProgressTxt(GetColor(pszBordercolort));
			}
			pszBordercolort = (CHAR*)poProgress->Attribute("Hot");
			if (pszBordercolort)
			{
				SetProgressHot(GetColor(pszBordercolort));
			}
			pszBordercolort = (CHAR*)poProgress->Attribute("Round");
			if (pszBordercolort)
			{
				SetProgressRound(GetSize(pszBordercolort));
			}
		}
		tinyxml2::XMLElement* poButtons = poRoot->FirstChildElement("Buttons");
		if (poButtons)
		{
			tinyxml2::XMLElement* poText = poButtons->FirstChildElement("Text");
			if (poText)
			{
				CHAR* pszBordercolor = (CHAR*)poText->Attribute("bordercolor");
				if (pszBordercolor)
				{
					SetButtonBorderColor(GetColor(pszBordercolor));
				}
				CHAR* pszNormalfrom = (CHAR*)poText->Attribute("normalfrom");
				if (pszNormalfrom)
				{
					SetButtonNormalFromColor(GetColor(pszNormalfrom));
				}
				CHAR* pszNormalto = (CHAR*)poText->Attribute("normalto");
				if (pszNormalto)
				{
					SetButtonNormalToColor(GetColor(pszNormalto));
				}
				CHAR* pszNormalMiddle = (CHAR*)poText->Attribute("normalmiddle");
				if (pszNormalMiddle)
				{
					SetButtonNormalMiddleColor(GetColor(pszNormalMiddle));
				}

				CHAR* pszNormalTxt = (CHAR*)poText->Attribute("normaltxt");
				if (pszNormalTxt)
				{
					m_clrButtonNormalTxt = GetColor(pszNormalTxt);
				}
				CHAR* pszRound = (CHAR*)poText->Attribute("round");
				if (pszRound)
				{
					SetButtonRoundSize(GetSize(pszRound));
				}

				CHAR* pszMouseOnfrom = (CHAR*)poText->Attribute("mouseonfrom");
				if (pszMouseOnfrom)
				{
					SetButtonMouseOnFromColor(GetColor(pszMouseOnfrom));
				}
				CHAR* pszMouseOnTo = (CHAR*)poText->Attribute("mouseonto");
				if (pszMouseOnTo)
				{
					SetButtonMouseOnToColor(GetColor(pszMouseOnTo));
				}
				CHAR* pszMouseOnMiddle = (CHAR*)poText->Attribute("mouseonmiddle");
				if (pszMouseOnMiddle)
				{
					SetButtonMouseOnMiddleColor(GetColor(pszMouseOnMiddle));
				}

				CHAR* pszMouseOnTxt = (CHAR*)poText->Attribute("mouseontxt");
				if (pszMouseOnTxt)
				{
					m_clrButtonMouseOnTxt = GetColor(pszMouseOnTxt);
				}

				CHAR* pszPressedFrom = (CHAR*)poText->Attribute("pressedfrom");
				if (pszPressedFrom)
				{
					SetButtonPressedFromColor(GetColor(pszPressedFrom));
				}
				CHAR* pszPressedTo = (CHAR*)poText->Attribute("pressedto");
				if (pszPressedTo)
				{
					SetButtonPressedToColor(GetColor(pszPressedTo));
				}
				CHAR* pszPressedMiddle = (CHAR*)poText->Attribute("pressedmiddle");
				if (pszPressedMiddle)
				{
					SetButtonPressedMiddleColor(GetColor(pszPressedMiddle));
				}

				CHAR* pszPressedTxt = (CHAR*)poText->Attribute("pressedtxt");
				if (pszPressedTxt)
				{
					m_clrButtonPressedTxt = GetColor(pszPressedTxt);
				}
			}

			tinyxml2::XMLElement* poCommon = poButtons->FirstChildElement("Common");
			if (poCommon)
			{
				CHAR* pszBordercolor = (CHAR*)poCommon->Attribute("bordercolor");
				if (pszBordercolor)
				{
					SetCommonButtonBorderColor(GetColor(pszBordercolor));
				}
				CHAR* pszNormalfrom = (CHAR*)poCommon->Attribute("normalfrom");
				if (pszNormalfrom)
				{
					SetCommonButtonNormalFromColor(GetColor(pszNormalfrom));
				}
				CHAR* pszNormalto = (CHAR*)poCommon->Attribute("normalto");
				if (pszNormalto)
				{
					SetCommonButtonNormalToColor(GetColor(pszNormalto));
				}
				CHAR* pszNormalMiddle = (CHAR*)poCommon->Attribute("normalmiddle");
				if (pszNormalMiddle)
				{
					SetCommonButtonNormalMiddleColor(GetColor(pszNormalMiddle));
				}
				CHAR* pszRound = (CHAR*)poCommon->Attribute("round");
				if (pszRound)
				{
					SetCommonButtonRoundSize(GetSize(pszRound));
				}

				CHAR* pszMouseOnfrom = (CHAR*)poCommon->Attribute("mouseonfrom");
				if (pszMouseOnfrom)
				{
					SetCommonButtonMouseOnFromColor(GetColor(pszMouseOnfrom));
				}
				CHAR* pszMouseOnTo = (CHAR*)poCommon->Attribute("mouseonto");
				if (pszMouseOnTo)
				{
					SetCommonButtonMouseOnToColor(GetColor(pszMouseOnTo));
				}
				CHAR* pszMouseOnMiddle = (CHAR*)poCommon->Attribute("mouseonmiddle");
				if (pszMouseOnMiddle)
				{
					SetCommonButtonMouseOnMiddleColor(GetColor(pszMouseOnMiddle));
				}

				CHAR* pszPressedFrom = (CHAR*)poCommon->Attribute("pressedfrom");
				if (pszPressedFrom)
				{
					SetCommonButtonPressedFromColor(GetColor(pszPressedFrom));
				}
				CHAR* pszPressedTo = (CHAR*)poCommon->Attribute("pressedto");
				if (pszPressedTo)
				{
					SetCommonButtonPressedToColor(GetColor(pszPressedTo));
				}
				CHAR* pszPressedMiddle = (CHAR*)poCommon->Attribute("pressedmiddle");
				if (pszPressedMiddle)
				{
					SetCommonButtonPressedMiddleColor(GetColor(pszPressedMiddle));
				}
			}
			tinyxml2::XMLElement* poHyperLink = poButtons->FirstChildElement("HyperLink");
			if (poHyperLink)
			{
				CHAR* pszHyperLinkText = (CHAR*)poHyperLink->Attribute("textcolor");
				if (pszHyperLinkText)
				{
					m_clrHyperLinkText = GetColor(pszHyperLinkText);
				}
				pszHyperLinkText = (CHAR*)poHyperLink->Attribute("hottextcolor");
				if (pszHyperLinkText)
				{
					m_clrHyperLinkHotText = GetColor(pszHyperLinkText);
				}
			}
		}
		tinyxml2::XMLElement* poComboBoxs = poRoot->FirstChildElement("ComboBoxs");
		if (poComboBoxs)
		{
			tinyxml2::XMLElement* poRXSkinComboBox = poComboBoxs->FirstChildElement("RXSkinComboBox");
			if (poRXSkinComboBox)
			{
				CHAR* pszFirst = (CHAR*)poRXSkinComboBox->Attribute("arrowwidth");
				if (pszFirst)
				{
					m_dwComboBoxArrowWidth = atoi(pszFirst);
				}
				pszFirst = (CHAR*)poRXSkinComboBox->Attribute("arrowOffSet");
				if (pszFirst)
				{
					m_dwComboBoxArrowOffSet = atoi(pszFirst);
				}
			}

			tinyxml2::XMLElement* poPopupElement = poComboBoxs->FirstChildElement("Popup");
			if (poPopupElement)
			{
				CHAR* pszOffSetX = (CHAR*)poPopupElement->Attribute("offsetx");
				if (pszOffSetX)
				{
					m_dwComboBoxPopupOffSetX = atoi(pszOffSetX);
				}
				CHAR* pszOffSetY = (CHAR*)poPopupElement->Attribute("offsety");
				if (pszOffSetY)
				{
					m_dwComboBoxPopupOffSetY = atoi(pszOffSetY);
				}
				CHAR* pszItemHeight = (CHAR*)poPopupElement->Attribute("itemheight");
				if (pszItemHeight)
				{
					m_dwComboBoxPopupItemHeight = atoi(pszItemHeight);
				}
				CHAR* pszBKColor = (CHAR*)poPopupElement->Attribute("bkcolor");
				if (pszBKColor)
				{
					m_clrComboBoxPopupBK = GetColor(pszBKColor);
				}
				CHAR* pszBorderColor = (CHAR*)poPopupElement->Attribute("bordercolor");
				if (pszBorderColor)
				{
					m_clrComboBoxPopupBorder = GetColor(pszBorderColor);
				}
			}

			tinyxml2::XMLElement* poPopupItemElement = poComboBoxs->FirstChildElement("Item");
			if (poPopupItemElement)
			{
				CHAR* pszMouseOnBK = (CHAR*)poPopupItemElement->Attribute("mouseonbk");
				if (pszMouseOnBK)
				{
					m_clrComboBoxPopupItemMouseonBK = GetColor(pszMouseOnBK);
				}
				CHAR* pszMouseOnText = (CHAR*)poPopupItemElement->Attribute("mouseontext");
				if (pszMouseOnText)
				{
					m_clrComboBoxPopupItemMouseonText = GetColor(pszMouseOnText);
				}
				CHAR* pszPressedBK = (CHAR*)poPopupItemElement->Attribute("pressedbk");
				if (pszPressedBK)
				{
					m_clrComboBoxPopupItemPressedBK = GetColor(pszPressedBK);
				}
				CHAR* pszPressedText = (CHAR*)poPopupItemElement->Attribute("pressedtext");
				if (pszPressedText)
				{
					m_clrComboBoxPopupItemPressedText = GetColor(pszPressedText);
				}

				CHAR* pszSelectedBK = (CHAR*)poPopupItemElement->Attribute("selectedbk");
				if (pszSelectedBK)
				{
					m_clrComboBoxPopupItemSelectedBK = GetColor(pszSelectedBK);
				}
				CHAR* pszSelectedText = (CHAR*)poPopupItemElement->Attribute("selectedtext");
				if (pszSelectedText)
				{
					m_clrComboBoxPopupItemSelectedText = GetColor(pszSelectedText);
				}
				CHAR* pszNormalText = (CHAR*)poPopupItemElement->Attribute("normaltext");
				if (pszNormalText)
				{
					m_clrComboBoxPopupItemNormalText = GetColor(pszNormalText);
				}
			}
		}

		tinyxml2::XMLElement* poToolTip = poRoot->FirstChildElement("ToolTip");
		if (poToolTip)
		{
			CHAR* pszBk = (CHAR*)poToolTip->Attribute("bkcolor");
			if (pszBk)
			{
				m_clrToolTipBk = GetColor(pszBk);
			}
			CHAR* pszBorder = (CHAR*)poToolTip->Attribute("bordercolor");
			if (pszBorder)
			{
				m_clrToolTipBorder = GetColor(pszBorder);
			}
			CHAR* pszOffSetX = (CHAR*)poToolTip->Attribute("offsetx");
			if (pszOffSetX)
			{
				m_dwToolTipOffSetX = atoi(pszOffSetX);
			}
			CHAR* pszOffSetY = (CHAR*)poToolTip->Attribute("offsety");
			if (pszOffSetY)
			{
				m_dwToolTipOffSetY = atoi(pszOffSetY);
			}
		}

		tinyxml2::XMLElement* poControlBoder = poRoot->FirstChildElement("ControlBoder");
		if (poControlBoder)
		{
			tinyxml2::XMLElement* poControlBorderLeft = poControlBoder->FirstChildElement("Left");
			if (poControlBorderLeft)
			{
				CHAR* pszWidth = (CHAR*)poControlBorderLeft->Attribute("width");
				if (pszWidth)
				{
					m_stLeft.m_dwTotal = atoi(pszWidth);
				}
				CHAR* pszFirst = (CHAR*)poControlBorderLeft->Attribute("firstoffset");
				if (pszFirst)
				{
					m_stLeft.m_dwFirstOffSet = atoi(pszFirst);
				}
				CHAR* pszSecond = (CHAR*)poControlBorderLeft->Attribute("secondoffset");
				if (pszSecond)
				{
					m_stLeft.m_dwSecondOffSet = atoi(pszSecond);
				}
			}
			tinyxml2::XMLElement* poControlBorderRight = poControlBoder->FirstChildElement("Right");
			if (poControlBorderRight)
			{
				CHAR* pszWidth = (CHAR*)poControlBorderRight->Attribute("width");
				if (pszWidth)
				{
					m_stRight.m_dwTotal = atoi(pszWidth);
				}
				CHAR* pszFirst = (CHAR*)poControlBorderRight->Attribute("firstoffset");
				if (pszFirst)
				{
					m_stRight.m_dwFirstOffSet = atoi(pszFirst);
				}
				CHAR* pszSecond = (CHAR*)poControlBorderRight->Attribute("secondoffset");
				if (pszSecond)
				{
					m_stRight.m_dwSecondOffSet = atoi(pszSecond);
				}
			}
			tinyxml2::XMLElement* poControlBorderTop = poControlBoder->FirstChildElement("Top");
			if (poControlBorderTop)
			{
				CHAR* pszHeight = (CHAR*)poControlBorderTop->Attribute("height");
				if (pszHeight)
				{
					m_stTop.m_dwTotal = atoi(pszHeight);
				}
				CHAR* pszFirst = (CHAR*)poControlBorderTop->Attribute("firstoffset");
				if (pszFirst)
				{
					m_stTop.m_dwFirstOffSet = atoi(pszFirst);
				}
				CHAR* pszSecond = (CHAR*)poControlBorderTop->Attribute("secondoffset");
				if (pszSecond)
				{
					m_stTop.m_dwSecondOffSet = atoi(pszSecond);
				}
			}
			tinyxml2::XMLElement* poControlBorderBottom = poControlBoder->FirstChildElement("Bottom");
			if (poControlBorderBottom)
			{
				CHAR* pszHeight = (CHAR*)poControlBorderBottom->Attribute("height");
				if (pszHeight)
				{
					m_stBottom.m_dwTotal = atoi(pszHeight);
				}
				CHAR* pszFirst = (CHAR*)poControlBorderBottom->Attribute("firstoffset");
				if (pszFirst)
				{
					m_stBottom.m_dwFirstOffSet = atoi(pszFirst);
				}
				CHAR* pszSecond = (CHAR*)poControlBorderBottom->Attribute("secondoffset");
				if (pszSecond)
				{
					m_stBottom.m_dwSecondOffSet = atoi(pszSecond);
				}
			}
		}
		// 读取CRXSkinBrownser相关参数
		tinyxml2::XMLElement* poBrownser = poRoot->FirstChildElement("Brownser");
		if (poBrownser)
		{
			SetBrownserOffSetX(GetIN32FromElementAttri(poBrownser,"offsetx"));				
			m_dwBrownserButtonWidth = GetIN32FromElementAttri(poBrownser, "buttonwidth");
			m_dwBrownserButtonHeight = GetIN32FromElementAttri(poBrownser, "buttonheight");
		}
		// 读取CRXSkinBrownser相关参数  结束
			
		// 读取CRXSkinTabCtrl相关参数
		tinyxml2::XMLElement* poTabCtrl = poRoot->FirstChildElement("TabCtrl");
		if (poTabCtrl)
		{
			CHAR* pszRound = (CHAR*)poTabCtrl->Attribute("round");
			if (pszRound)
			{
				m_szTabCtrlRound = GetSize(pszRound);
			}
			tinyxml2::XMLElement* poItem = poTabCtrl->FirstChildElement("Item");
			if (poItem)
			{
				CHAR* pszHeight = (CHAR*)poItem->Attribute("height");
				if (pszHeight)
				{
					m_dwTabItemHeight = atoi(pszHeight);
				}
				CHAR* pszNormalFromBk = (CHAR*)poItem->Attribute("normalbkfrom");
				if (pszNormalFromBk)
				{
					m_clrTabItemNormalFromBk = GetColor(pszNormalFromBk);
				}
				CHAR* pszNormalToBk = (CHAR*)poItem->Attribute("normalbkto");
				if (pszNormalToBk)
				{
					m_clrTabItemNormalToBk = GetColor(pszNormalToBk);
				}
				CHAR* pszNormalText = (CHAR*)poItem->Attribute("normaltext");
				if (pszNormalText)
				{
					m_clrTabItemNormalText = GetColor(pszNormalText);
				}

				CHAR* pszMouseOnFromBk = (CHAR*)poItem->Attribute("mouseonbkfrom");
				if (pszMouseOnFromBk)
				{
					m_clrTabItemMouseOnFromBk = GetColor(pszMouseOnFromBk);
				}
				CHAR* pszMouseOnToBk = (CHAR*)poItem->Attribute("mouseonbkto");
				if (pszMouseOnToBk)
				{
					m_clrTabItemMouseOnToBk = GetColor(pszMouseOnToBk);
				}
				CHAR* pszMouseOnText = (CHAR*)poItem->Attribute("mouseontext");
				if (pszMouseOnText)
				{
					m_clrTabItemMouseOnText = GetColor(pszMouseOnText);
				}

				CHAR* pszPressedFromBk = (CHAR*)poItem->Attribute("pressedbkfrom");
				if (pszPressedFromBk)
				{
					m_clrTabItemPressedFromBk = GetColor(pszPressedFromBk);
				}
				CHAR* pszPressedToBk = (CHAR*)poItem->Attribute("pressedbkto");
				if (pszPressedToBk)
				{
					m_clrTabItemPressedToBk = GetColor(pszPressedToBk);
				}
				CHAR* pszPressedText = (CHAR*)poItem->Attribute("pressedtext");
				if (pszPressedText)
				{
					m_clrTabItemPressedText = GetColor(pszPressedText);
				}

				CHAR* pszSelectedFromBk = (CHAR*)poItem->Attribute("selectedbkfrom");
				if (pszSelectedFromBk)
				{
					m_clrTabItemSelectedFromBk = GetColor(pszSelectedFromBk);
				}
				CHAR* pszSelectedToBk = (CHAR*)poItem->Attribute("selectedbkto");
				if (pszSelectedToBk)
				{
					m_clrTabItemSelectedToBk = GetColor(pszSelectedToBk);
				}
				CHAR* pszSelectedText = (CHAR*)poItem->Attribute("selectedtext");
				if (pszSelectedText)
				{
					m_clrTabItemSelectedText = GetColor(pszSelectedText);
				}

				CHAR* pszBorder = (CHAR*)poItem->Attribute("bordercolor");
				if (pszBorder)
				{
					m_clrTabItemBorder = GetColor(pszBorder);
				}
					
				CHAR* pszOffSetX = (CHAR*)poItem->Attribute("offsetx");
				if (pszOffSetX)
				{
					m_dwTabItemOffSetX = atoi(pszOffSetX);
				}
			}
		}
		//  读取CRXSkintabCtrl相关参数  结束

		// 读取CRXSkinButtonList相关参数
		tinyxml2::XMLElement* poButtonList = poRoot->FirstChildElement("ButtonList");
		if (poButtonList)
		{
			tinyxml2::XMLElement* poItemElement = poButtonList->FirstChildElement("Item");
			if (poItemElement)
			{
				SetButtonListOffSetX(GetIN32FromElementAttri(poItemElement,"offsetx"));
				SetButtonListOffSetY(GetIN32FromElementAttri(poItemElement,"offsety"));
				SetButtonListRound(GetSizeFromElementAttri(poItemElement,"round"));
				SetButtonListSelectedNormalFrom(GetColorFromElementAttri(poItemElement,"selectednormalfrom"));
				SetButtonListSelectedNormalTo(GetColorFromElementAttri(poItemElement,"selectednormalto"));
				SetButtonListSelectedNormalText(GetColorFromElementAttri(poItemElement,"selectednormaltext"));

				SetButtonListSelectedMouseOnFrom(GetColorFromElementAttri(poItemElement,"selectedmouseonfrom"));
				SetButtonListSelectedMouseOnTo(GetColorFromElementAttri(poItemElement,"selectedmouseonto"));
				SetButtonListSelectedMouseOnText(GetColorFromElementAttri(poItemElement,"selectedmouseontext"));
				
				SetButtonListSelectedPressedFrom(GetColorFromElementAttri(poItemElement,"selectedpressedfrom"));
				SetButtonListSelectedPressedTo(GetColorFromElementAttri(poItemElement,"selectedpressedto"));
				SetButtonListSelectedPressedText(GetColorFromElementAttri(poItemElement,"selectedpressedtext"));

				SetButtonListNormalText(GetColorFromElementAttri(poItemElement,"normaltext"));
				SetButtonListMouseOnText(GetColorFromElementAttri(poItemElement,"mouseontext"));
				SetButtonListPressedText(GetColorFromElementAttri(poItemElement,"pressedtext"));

			}
		}
		// CRXSkinButtonList 参数   结束
		// 读取CRXSkinMenu相关参数
		tinyxml2::XMLElement* poMenu = poRoot->FirstChildElement("Menu");
		if (poMenu)
		{
			CHAR* pszBorderColor = (CHAR*)poMenu->Attribute("bordercolor");
			if (pszBorderColor)
			{
				SetMenuBorder(GetColor(pszBorderColor));
			}
			CHAR* pszBkColor = (CHAR*)poMenu->Attribute("bkcolor");
			if (pszBkColor)
			{
				SetMenuBk(GetColor(pszBkColor));
			}
			CHAR* pszSidebarColor = (CHAR*)poMenu->Attribute("sidebarcolor");
			if (pszSidebarColor)
			{
				SetMenuSidebarBk(GetColor(pszSidebarColor));
			}
			CHAR* pszRound = (CHAR*)poMenu->Attribute("round");
			if (pszRound)
			{
				SetMenuRound(GetSize(pszRound));
			}
			CHAR* pszSidebarWidth = (CHAR*)poMenu->Attribute("sidebarwidth");
			if (pszSidebarWidth)
			{
				SetMenuSidebarWidth(atoi(pszSidebarWidth));
			}

			tinyxml2::XMLElement* poItem = poMenu->FirstChildElement("Item");
			if (poItem)
			{
				CHAR* pszHeight = (CHAR*)poItem->Attribute("height");
				if (pszHeight)
				{
					SetMenuItemHeight(atoi(pszHeight));
				}
				CHAR* pszOffSetX = (CHAR*)poItem->Attribute("offsetx");
				if (pszOffSetX)
				{
					SetMenuItemOffSetX(atoi(pszOffSetX));
				}
				CHAR* pszOffSetY = (CHAR*)poItem->Attribute("offsety");
				if (pszOffSetY)
				{
					SetMenuItemOffSetY(atoi(pszOffSetY));
				}
				CHAR* pszNormalBkFrom = (CHAR*)poItem->Attribute("normalbkfrom");
				if (pszNormalBkFrom)
				{
					SetMenuItemNormalFromBk(GetColor(pszNormalBkFrom));
				}
				CHAR* pszNormalBkTo = (CHAR*)poItem->Attribute("normalbkto");
				if (pszNormalBkTo)
				{
					SetMenuItemNormalToBk(GetColor(pszNormalBkTo));
				}
				CHAR* pszNormalText = (CHAR*)poItem->Attribute("normaltext");
				if (pszNormalText)
				{
					SetMenuItemNormalText(GetColor(pszNormalText));
				}

				CHAR* pszMouseOnBkFrom = (CHAR*)poItem->Attribute("mouseonbkfrom");
				if (pszMouseOnBkFrom)
				{
					SetMenuItemMouseOnFromBk(GetColor(pszMouseOnBkFrom));
				}
				CHAR* pszMouseOnBkTo = (CHAR*)poItem->Attribute("mouseonbkto");
				if (pszMouseOnBkTo)
				{
					SetMenuItemMouseOnToBk(GetColor(pszMouseOnBkTo));
				}
				CHAR* pszMouseOnText = (CHAR*)poItem->Attribute("mouseontext");
				if (pszMouseOnText)
				{
					SetMenuItemMouseOnText(GetColor(pszMouseOnText));
				}

				CHAR* pszPressedBkFrom = (CHAR*)poItem->Attribute("pressedbkfrom");
				if (pszPressedBkFrom)
				{
					SetMenuItemPressedFromBk(GetColor(pszPressedBkFrom));
				}
				CHAR* pszPressedBkTo = (CHAR*)poItem->Attribute("pressedbkto");
				if (pszPressedBkTo)
				{
					SetMenuItemPressedToBk(GetColor(pszPressedBkTo));
				}
				CHAR* pszPressedText = (CHAR*)poItem->Attribute("pressedtext");
				if (pszPressedText)
				{
					SetMenuItemMouseOnText(GetColor(pszPressedText));
				}
					
				CHAR* pszSeperatorColor = (CHAR*)poItem->Attribute("seperatorcolor");
				if (pszSeperatorColor)
				{
					SetMenuSeperatorBk(GetColor(pszSeperatorColor));
				}
					
				CHAR* pszArrowWidth = (CHAR*)poItem->Attribute("arrowwidth");
				if (pszArrowWidth)
				{
					SetMenuItemArrowWidth(atoi(pszArrowWidth));
				}
					
				CHAR* pszSeperatorHeight = (CHAR*)poItem->Attribute("seperatorheight");
				if (pszSeperatorHeight)
				{
					SetMenuSeperatorHeight(atoi(pszSeperatorHeight));
				}
			}
		}
		// 读取CRXSkinMenu相关参数 结束

		// 读取CRXSkinDatePicker相关参数
		tinyxml2::XMLElement* poDatePicker = poRoot->FirstChildElement("DatePicker");
		if (poDatePicker)
		{
			CHAR* pszSize = (CHAR*)poDatePicker->Attribute("size");
			if (pszSize)
			{
				m_szDatePicker = GetSize(pszSize);
			}
		}
		// 读取CRXSkinDatePicker相关参数 结束

		// 读取CRXSkinImageSel相关参数
		tinyxml2::XMLElement* poImageSel = poRoot->FirstChildElement("ImageSel");
		if (poImageSel)
		{
			SetImageSelOffSetX(GetIN32FromElementAttri(poImageSel,"offsetx"));
			SetImageSelOffSetY(GetIN32FromElementAttri(poImageSel,"offsety"));
			tinyxml2::XMLElement* poImageSelItem = poImageSel->FirstChildElement("Item");
			if (poImageSelItem)
			{
				SetImageSelItem(GetSizeFromElementAttri(poImageSelItem,"size"));
			}
		}
		// 读取CRXSkinImageSel相关参数 结束

		// 读取CRXSkinLocationPicker相关参数
		tinyxml2::XMLElement* poLocationPicker = poRoot->FirstChildElement("LocationPicker");
		if (poLocationPicker)
		{
			CHAR* pszSize = (CHAR*)poLocationPicker->Attribute("size");
			if (pszSize)
			{
				m_szLocationPicker = GetSize(pszSize);
			}
		}
		// 读取CRXSkinLocationPicker相关参数 结束

		// 读取CRXSkinListCtrl相关参数
		tinyxml2::XMLElement* poListCtrl = poRoot->FirstChildElement("ListCtrl");
		if (poListCtrl)
		{
			SetListCtrlOffSetX(GetIN32FromElementAttri(poListCtrl,"offsetx"));
			SetListCtrlOffSetY(GetIN32FromElementAttri(poListCtrl,"offsety"));
			SetListCtrlBK(GetColorFromElementAttri(poListCtrl,"bkcolor"));
			SetListCtrlBorder(GetColorFromElementAttri(poListCtrl,"bordercolor"));
			tinyxml2::XMLElement* poHeader = poListCtrl->FirstChildElement("Header");
			if (poHeader)
			{
				SetListCtrlHeaderHeight(GetIN32FromElementAttri(poHeader,"height"));
				SetListCtrlHeaderFrom(GetColorFromElementAttri(poHeader,"fromcolor"));
				SetListCtrlHeaderTo(GetColorFromElementAttri(poHeader,"tocolor"));
				m_clrListCtrlHeaderNormal = GetColorFromElementAttri(poHeader, "normaltxtcolor");
				m_clrListCtrlHeaderMouseOn = GetColorFromElementAttri(poHeader, "mouseontxtcolor");
				m_clrListCtrlHeaderPressed = GetColorFromElementAttri(poHeader, "pressedtxtcolor");
			}

			tinyxml2::XMLElement* poItem = poListCtrl->FirstChildElement("Item");
			if (poItem)
			{
				SetListCtrlItemHeight(GetIN32FromElementAttri(poItem,"height"));
				SetListCtrlItemSelecedFrom(GetColorFromElementAttri(poItem,"selectedfromcolor"));
				SetListCtrlItemSelecedTo(GetColorFromElementAttri(poItem,"selectedtocolor"));
				SetListCtrlItemSelecedText(GetColorFromElementAttri(poItem,"selectedtextcolor"));
				m_clrListCtrlItemNormalText = GetColorFromElementAttri(poItem, "normaltextcolor");
				m_clrListCtrlItemMouseOnText = GetColorFromElementAttri(poItem, "mouseontxtcolor");
				m_clrListCtrlItemPressedText = GetColorFromElementAttri(poItem, "pressedtxtcolor");
			}
		}
		// 读取CRXSkinListCtrl相关参数 结束
			
		// 读取CRXSkinTreeCtrl相关参数
		tinyxml2::XMLElement* poTreeCtrl = poRoot->FirstChildElement("TreeCtrl");
		if (poTreeCtrl)
		{
			m_nTreeCtrlOffSetX = GetIN32FromElementAttri(poTreeCtrl, "offsetx");
			m_nTreeCtrlOffSetY = GetIN32FromElementAttri(poTreeCtrl, "offsety");
			m_clrTreeCtrlBorder = GetColorFromElementAttri(poTreeCtrl, "bordercolor");
			m_clrTreeCtrlBK = GetColorFromElementAttri(poTreeCtrl, "bkcolor");
			tinyxml2::XMLElement* poItem = poTreeCtrl->FirstChildElement("Item");
			if (poItem)
			{
				m_nTreeCtrlItemHeight = GetIN32FromElementAttri(poItem, "height");
				m_clrTreeCtrlItemNormalText = GetColorFromElementAttri(poItem, "normaltextcolor");
				m_clrTreeCtrlItemMouseOnFrom = GetColorFromElementAttri(poItem, "mouseonfromcolor");
				m_clrTreeCtrlItemMouseOnTo = GetColorFromElementAttri(poItem, "mouseontocolor");
				m_clrTreeCtrlItemMouseOnText = GetColorFromElementAttri(poItem, "mouseontextcolor");
										
				m_clrTreeCtrlItemPressedFrom = GetColorFromElementAttri(poItem, "pressedfromcolor");
				m_clrTreeCtrlItemPressedTo = GetColorFromElementAttri(poItem, "pressedtocolor");
				m_clrTreeCtrlItemPressedText = GetColorFromElementAttri(poItem, "pressedtextcolor");
			}
		}
		// 读取CRXSkinOutbar相关参数
		tinyxml2::XMLElement* poOutbar = poRoot->FirstChildElement("Outbar");
		if (poOutbar)
		{
			SetOutbarBKFrom(GetColorFromElementAttri(poOutbar,"bkfromcolor"));
			SetOutbarBKTo(GetColorFromElementAttri(poOutbar,"bktocolor"));
			SetOutbarRound(GetSizeFromElementAttri(poOutbar,"round"));
			tinyxml2::XMLElement* poItem = poOutbar->FirstChildElement("Item");
			if (poItem)
			{
				INT32 nOffSetX = GetIN32FromElementAttri(poItem,"offsetx");
				INT32 nOffSetY = GetIN32FromElementAttri(poItem,"offsety");
				SIZE sz;
				sz.cx = nOffSetX;
				sz.cy = nOffSetY;
				SetOutbarOffSet(sz);
				SetOutbarItemIndentationX(GetIN32FromElementAttri(poItem,"indentationx"));
				SetOutbarItemMouseOnText(GetColorFromElementAttri(poItem,"mouseontext"));
				SetOutbarItemSelectedNormalFrom(GetColorFromElementAttri(poItem,"selectednormalfrom"));
				SetOutbarItemSelectedNormalTo(GetColorFromElementAttri(poItem,"selectednormalto"));
				SetOutbarItemSelectedText(GetColorFromElementAttri(poItem,"selectedtext"));
				SetOutbarItemSelectedMouseOnFrom(GetColorFromElementAttri(poItem,"selectedmouseonfrom"));
				SetOutbarItemSelectedMouseOnTo(GetColorFromElementAttri(poItem,"selectedmouseonto"));
				SetOutbarItemNormalText(GetColorFromElementAttri(poItem,"normaltext"));
				SetOutbarItemHeight(GetIN32FromElementAttri(poItem,"height"));
			}
			tinyxml2::XMLElement* poFolder = poOutbar->FirstChildElement("Folder");
			if (poFolder)
			{
				INT32 nOffSetX = GetIN32FromElementAttri(poFolder,"offsetx");
				INT32 nOffSetY = GetIN32FromElementAttri(poFolder,"offsety");
				SIZE sz;
				sz.cx = nOffSetX;
				sz.cy = nOffSetY;
				SetOutbarFolderOffSet(sz);
				SetOutbarFolderHeight(GetIN32FromElementAttri(poFolder,"height"));
				SetOutbarFolderNormalBorder(GetColorFromElementAttri(poFolder,"normalborder"));
				SetOutbarFolderNormalFrom(GetColorFromElementAttri(poFolder,"noramlfrom"));
				SetOutbarFolderNormalTo(GetColorFromElementAttri(poFolder,"noramlto"));
				SetOutbarFolderNormalText(GetColorFromElementAttri(poFolder,"normaltext"));

				SetOutbarFolderPressedBorder(GetColorFromElementAttri(poFolder,"pressedborder"));
				SetOutbarFolderPressedFrom(GetColorFromElementAttri(poFolder,"pressedfrom"));
				SetOutbarFolderPressedTo(GetColorFromElementAttri(poFolder,"pressedto"));
				SetOutbarFolderPressedText(GetColorFromElementAttri(poFolder,"pressedtext"));

				SetOutbarFolderMouseOnFirstBorder(GetColorFromElementAttri(poFolder,"mouseonfirstborder"));
				SetOutbarFolderMouseOnSecondBorder(GetColorFromElementAttri(poFolder,"mouseonsecondborder"));
				SetOutbarFolderMouseOnThirdBorder(GetColorFromElementAttri(poFolder,"mouseonthirdborder"));

				SetOutbarFolderMouseOnFrom(GetColorFromElementAttri(poFolder,"mouseonfrom"));
				SetOutbarFolderMouseOnTo(GetColorFromElementAttri(poFolder,"mouseonto"));
				SetOutbarFolderMouseOnText(GetColorFromElementAttri(poFolder,"mouseontext"));
			}
		}
		// 读取CRXSkinOutbar相关参数 结束
		//  CRXSkinSpinEdit 相关参数
		tinyxml2::XMLElement* poElementSpinEdit = poRoot->FirstChildElement("SpinEdit");
		if (poElementSpinEdit)
		{
			m_nSpinEditArrowHeight = GetIN32FromElementAttri(poElementSpinEdit,"arrowheight");
		}
		//  CRXSkinSpinEdit 相关参数  结束

		//  CRXSkinSlider 相关参数
		tinyxml2::XMLElement* poElementSlider = poRoot->FirstChildElement("Slider");
		if (poElementSlider)
		{
			m_dwHorLeftOffSet = GetIN32FromElementAttri(poElementSlider, "leftoffset");
			m_dwHorRightOffSet = GetIN32FromElementAttri(poElementSlider, "rightoffset");
			m_dwVerTopOffSet = GetIN32FromElementAttri(poElementSlider, "topoffset");
			m_dwVerBottomOffSet = GetIN32FromElementAttri(poElementSlider, "bottomoffset");
			m_clrSliderTrans = GetColorFromElementAttri(poElementSlider, "trans");

			m_dwHorSelectedLeftOffSet = GetIN32FromElementAttri(poElementSlider, "selectedleftoffset");
			m_dwHorSelectedRightOffSet = GetIN32FromElementAttri(poElementSlider, "selectedrightoffset");

			m_dwVerSelectedTopOffSet = GetIN32FromElementAttri(poElementSlider, "selectedtopoffset");
			m_dwVerSelectedBottomOffSet = GetIN32FromElementAttri(poElementSlider, "selectedbottomoffset");
		}
		//  CRXSkinSlider 相关参数  结束

		//  读取全局相关参数
		tinyxml2::XMLElement* poColors = poRoot->FirstChildElement("Colors");
		if (poColors)
		{
			tinyxml2::XMLElement* poCommon = poColors->FirstChildElement("Common");
			if (poCommon)
			{
				m_clrNormalText = GetColorFromElementAttri(poCommon,"normaltext");
				CHAR* pszBkColor = (CHAR*)poCommon->Attribute("bkcolor");
				if (pszBkColor)
				{
					m_clrCommonBk = GetColor(pszBkColor);
				}
				CHAR* pszBorderColor = (CHAR*)poCommon->Attribute("bordercolor");
				if (pszBorderColor)
				{
					m_clrCommonBorder = GetColor(pszBorderColor);
				}
				m_clrDisableText = GetColorFromElementAttri(poCommon,"disabletext");
			}

			tinyxml2::XMLElement* poArrow = poColors->FirstChildElement("Arrow");
			if (poArrow)
			{
				CHAR* pszNormalColor = (CHAR*)poArrow->Attribute("normalcolor");
				if (pszNormalColor)
				{
					m_clrArrowNormal = GetColor(pszNormalColor);
				}
				CHAR* pszMouseOnColor = (CHAR*)poArrow->Attribute("mouseoncolor");
				if (pszMouseOnColor)
				{
					m_clrArrowMouseOn = GetColor(pszMouseOnColor);
				}
				CHAR* pszzPressedColor = (CHAR*)poArrow->Attribute("pressedcolor");
				if (pszzPressedColor)
				{
					m_clrArrowPressed = GetColor(pszzPressedColor);
				}
			}
		}
		//  读取全局相关参数  结束

	}
}

POINT CDrawMgr::GetButtonSize()
{
	return m_szButton;
}

VOID CDrawMgr::SetButtonSize(POINT pt)
{
	m_szButton = pt;
}

COLORREF CDrawMgr::GetTitleColor()
{
	return m_clrTitle;
}

VOID CDrawMgr::SetTitleColor(COLORREF clrTitle)
{
	m_clrTitle = clrTitle;
}

VOID CDrawMgr::SetTitleFontSize(const INT32 nFontSize)
{
	m_nTitleFontSize = nFontSize;
}

VOID CDrawMgr::SetTitleFontName(const TCHAR* pszFontName)
{
	memset(m_szTitleFontName,0,sizeof(TCHAR)*256);
	_tcscpy(m_szTitleFontName,pszFontName);
}

VOID CDrawMgr::SetCommonFontSize(const INT32 nFontSize)
{
	m_nCommonFontSize = nFontSize;
}

VOID CDrawMgr::SetCommonFontName(const TCHAR* pszFontName)
{
	memset(m_szCommonFontName, 0, sizeof(TCHAR) * 256);
	_tcscpy(m_szCommonFontName, pszFontName);
}


HFONT CDrawMgr::GetNormalFont()
{
	return m_hNormalFont;
}
HFONT CDrawMgr::GetEditFont()
{
	return m_hNormalFont;
}
HFONT CDrawMgr::GetBoldFont()
{
	return m_hBoldFont;
}

HFONT CDrawMgr::GetTitleFont()
{
	return m_hTitleFont;
}

HCURSOR CDrawMgr::GetArrowCursor()
{
	return m_hArrow;
}

HCURSOR CDrawMgr::GetHandCursor()
{
	return m_hHand;
}

HCURSOR CDrawMgr::GetNSCursor()
{
	return m_hNS;
}

HCURSOR CDrawMgr::GetWECursor()
{
	return m_hWE;
}

HCURSOR CDrawMgr::GetNWSECursor()
{
	return m_hNWSE;
}

HCURSOR CDrawMgr::GetNESWCursor()
{
	return m_hNESW;
}

VOID CDrawMgr::SetButtonBorderColor(COLORREF clrBorderColor)
{
	m_clrButtonBorder = clrBorderColor;
}

COLORREF CDrawMgr::GetButtonBorderColor()
{
	return m_clrButtonBorder;
}

VOID CDrawMgr::SetButtonNormalFromColor(COLORREF clrNormalFromColor)
{
	m_clrButtonNormalFrom = clrNormalFromColor;
}

COLORREF CDrawMgr::GetButtonNormalFromColor()
{
	return m_clrButtonNormalFrom;
}

VOID CDrawMgr::SetButtonNormalToColor(COLORREF clrNormalToColor)
{
	m_clrButtonNormalTo = clrNormalToColor;
}

COLORREF CDrawMgr::GetButtonNormalToColor()
{
	return m_clrButtonNormalTo;
}

VOID CDrawMgr::SetButtonNormalMiddleColor(COLORREF clrNormalMiddleColor)
{
	m_clrButtonNormalMiddle = clrNormalMiddleColor;
}

COLORREF CDrawMgr::GetButtonNormalMiddleColor()
{
	return m_clrButtonNormalMiddle;
}

VOID CDrawMgr::SetButtonMouseOnMiddleColor(COLORREF clrNormalMouseOnColor)
{
	m_clrButtonMouseOnMiddle = clrNormalMouseOnColor;
}

COLORREF CDrawMgr::GetButtonMouseOnMiddleColor()
{
	return m_clrButtonMouseOnMiddle;
}

VOID CDrawMgr::SetButtonMouseOnFromColor(COLORREF clrMouseOnFromColor)
{
	m_clrButtonMouseOnFrom = clrMouseOnFromColor;
}

COLORREF CDrawMgr::GetButtonMouseOnFromColor()
{
	return m_clrButtonMouseOnFrom;
}

VOID CDrawMgr::SetButtonMouseOnToColor(COLORREF clrMouseOnToColor)
{
	m_clrButtonMouseOnTo = clrMouseOnToColor;
}

COLORREF CDrawMgr::GetButtonMouseOnToColor()
{
	return m_clrButtonMouseOnTo;
}

VOID CDrawMgr::SetButtonPressedToColor(COLORREF clrPressedToColor)
{
	m_clrButtonPressedTo = clrPressedToColor;
}

COLORREF CDrawMgr::GetButtonPressedToColor()
{
	return m_clrButtonPressedTo;
}

VOID CDrawMgr::SetButtonPressedFromColor(COLORREF clrPressedFromColor)
{
	m_clrButtonPressedFrom = clrPressedFromColor;
}

COLORREF CDrawMgr::GetButtonPressedFromColor()
{
	return m_clrButtonPressedFrom;
}

VOID CDrawMgr::SetButtonPressedMiddleColor(COLORREF clrPressedMiddleColor)
{
	m_clrButtonPressedMiddle = clrPressedMiddleColor;
}

COLORREF CDrawMgr::GetButtonPressedMiddleColor()
{
	return m_clrButtonPressedMiddle;
}


SIZE CDrawMgr::GetButtonRoundSize()
{
	return m_szButtonRound;
}

VOID CDrawMgr::SetButtonRoundSize(const SIZE szButtonRoundSize)
{
	m_szButtonRound = szButtonRoundSize;
}


/// Common Button 参数
VOID CDrawMgr::SetCommonButtonBorderColor(COLORREF clrBorderColor)
{
	m_clrCommonButtonBorder = clrBorderColor;
}

COLORREF CDrawMgr::GetCommonButtonBorderColor()
{
	return m_clrCommonButtonBorder;
}

VOID CDrawMgr::SetCommonButtonNormalFromColor(COLORREF clrNormalFromColor)
{
	m_clrCommonButtonNormalFrom = clrNormalFromColor;
}

COLORREF CDrawMgr::GetCommonButtonNormalFromColor()
{
	return m_clrCommonButtonNormalFrom;
}

VOID CDrawMgr::SetCommonButtonNormalToColor(COLORREF clrNormalToColor)
{
	m_clrCommonButtonNormalTo = clrNormalToColor;
}

COLORREF CDrawMgr::GetCommonButtonNormalToColor()
{
	return m_clrCommonButtonNormalTo;
}

VOID CDrawMgr::SetCommonButtonNormalMiddleColor(COLORREF clrNormalMiddleColor)
{
	m_clrCommonButtonNormalMiddle = clrNormalMiddleColor;
}

COLORREF CDrawMgr::GetCommonButtonNormalMiddleColor()
{
	return m_clrCommonButtonNormalMiddle;
}

VOID CDrawMgr::SetCommonButtonMouseOnMiddleColor(COLORREF clrNormalMouseOnColor)
{
	m_clrCommonButtonMouseOnMiddle = clrNormalMouseOnColor;
}

COLORREF CDrawMgr::GetCommonButtonMouseOnMiddleColor()
{
	return m_clrCommonButtonMouseOnMiddle;
}

VOID CDrawMgr::SetCommonButtonMouseOnFromColor(COLORREF clrMouseOnFromColor)
{
	m_clrCommonButtonMouseOnFrom = clrMouseOnFromColor;
}

COLORREF CDrawMgr::GetCommonButtonMouseOnFromColor()
{
	return m_clrCommonButtonMouseOnFrom;
}

VOID CDrawMgr::SetCommonButtonMouseOnToColor(COLORREF clrMouseOnToColor)
{
	m_clrCommonButtonMouseOnTo = clrMouseOnToColor;
}

COLORREF CDrawMgr::GetCommonButtonMouseOnToColor()
{
	return m_clrCommonButtonMouseOnTo;
}

VOID CDrawMgr::SetCommonButtonPressedToColor(COLORREF clrPressedToColor)
{
	m_clrCommonButtonPressedTo = clrPressedToColor;
}

COLORREF CDrawMgr::GetCommonButtonPressedToColor()
{
	return m_clrCommonButtonPressedTo;
}

VOID CDrawMgr::SetCommonButtonPressedFromColor(COLORREF clrPressedFromColor)
{
	m_clrCommonButtonPressedFrom = clrPressedFromColor;
}

COLORREF CDrawMgr::GetCommonButtonPressedFromColor()
{
	return m_clrCommonButtonPressedFrom;
}

VOID CDrawMgr::SetCommonButtonPressedMiddleColor(COLORREF clrPressedMiddleColor)
{
	m_clrCommonButtonPressedMiddle = clrPressedMiddleColor;
}

COLORREF CDrawMgr::GetCommonButtonPressedMiddleColor()
{
	return m_clrCommonButtonPressedMiddle;
}


SIZE CDrawMgr::GetCommonButtonRoundSize()
{
	return m_szCommonButtonRound;
}

VOID CDrawMgr::SetCommonButtonRoundSize(const SIZE szButtonRoundSize)
{
	m_szCommonButtonRound = szButtonRoundSize;
}
/// Common Button 参数

// CRXSkinToolTip参数设定

COLORREF CDrawMgr::GetToolTipBk()
{
	return m_clrToolTipBk;
}

VOID CDrawMgr::SetToolTipBk(COLORREF clr)
{
	m_clrToolTipBk = clr;
}

COLORREF CDrawMgr::GetToolTipBorder()
{
	return m_clrToolTipBorder;
}

VOID CDrawMgr::SetToolTipBorder(COLORREF clr)
{
	m_clrToolTipBorder = clr;
}

UINT32 CDrawMgr::GetToolTipOffSetX()
{
	return m_dwToolTipOffSetX;
}

VOID CDrawMgr::SetToolTipOffSetX(UINT32 dwOffSetX)
{
	m_dwToolTipOffSetX = dwOffSetX;
}

UINT32 CDrawMgr::GetToolTipOffSetY()
{
	return m_dwToolTipOffSetY;
}

VOID CDrawMgr::SetToolTipOffSetY(UINT32 dwOffSetY)
{
	m_dwToolTipOffSetY = dwOffSetY;
}

// CRXSkinToolTip参数设定 结束

// CRXSkinMenu参数设定
COLORREF CDrawMgr::GetMenuBk()
{
	return m_clrMenuBk;
}

VOID CDrawMgr::SetMenuBk(COLORREF clr)
{
	m_clrMenuBk = clr;
}

COLORREF CDrawMgr::GetMenuBorder()
{
	return m_clrMenuBorder;
}

VOID CDrawMgr::SetMenuBorder(COLORREF clr)
{
	m_clrMenuBorder = clr;
}

COLORREF CDrawMgr::GetMenuItemNormalFromBk()
{
	return m_clrMenuItemNormalFromBk;
}

VOID CDrawMgr::SetMenuItemNormalFromBk(COLORREF clr)
{
	m_clrMenuItemNormalFromBk = clr;
}

COLORREF CDrawMgr::GetMenuItemNormalToBk()
{
	return m_clrMenuItemNormalToBk;
}

VOID CDrawMgr::SetMenuItemNormalToBk(COLORREF clr)
{
	m_clrMenuItemNormalToBk = clr;
}

COLORREF CDrawMgr::GetMenuItemNormalText()
{
	return m_clrMenuItemNormalText;
}

VOID CDrawMgr::SetMenuItemNormalText(COLORREF clr)
{
	m_clrMenuItemNormalText = clr;
}

COLORREF CDrawMgr::GetMenuItemMouseOnFromBk()
{
	return m_clrMenuItemMouseOnFromBk;
}

VOID CDrawMgr::SetMenuItemMouseOnFromBk(COLORREF clr)
{
	m_clrMenuItemMouseOnFromBk = clr;
}

COLORREF CDrawMgr::GetMenuItemMouseOnToBk()
{
	return m_clrMenuItemMouseOnToBk;
}

VOID CDrawMgr::SetMenuItemMouseOnToBk(COLORREF clr)
{
	m_clrMenuItemMouseOnToBk = clr;
}

COLORREF CDrawMgr::GetMenuItemMouseOnText()
{
	return m_clrMenuItemMouseOnText;
}

VOID CDrawMgr::SetMenuItemMouseOnText(COLORREF clr)
{
	m_clrMenuItemMouseOnText = clr;
}

COLORREF CDrawMgr::GetMenuItemPressedFromBk()
{
	return m_clrMenuItemPressedFromBk;
}

VOID CDrawMgr::SetMenuItemPressedFromBk(COLORREF clr)
{
	m_clrMenuItemPressedFromBk = clr;
}

COLORREF CDrawMgr::GetMenuItemPressedToBk()
{
	return m_clrMenuItemPressedToBk;
}

VOID CDrawMgr::SetMenuItemPressedToBk(COLORREF clr)
{
	m_clrMenuItemPressedToBk = clr;
}

COLORREF CDrawMgr::GetMenuItemPressedText()
{
	return m_clrMenuItemPressedText;
}

VOID CDrawMgr::SetMenuItemPressedText(COLORREF clr)
{
	m_clrMenuItemPressedText = clr;
}


COLORREF CDrawMgr::GetMenuSeperatorBk()
{
	return m_clrMenuSeperator;
}

VOID CDrawMgr::SetMenuSeperatorBk(COLORREF clr)
{
	m_clrMenuSeperator = clr;
}

COLORREF CDrawMgr::GetMenuSidebarBk()
{
	return m_clrSidebarBk;
}

VOID CDrawMgr::SetMenuSidebarBk(COLORREF clr)
{
	m_clrSidebarBk = clr;
}

UINT32 CDrawMgr::GetMenuItemHeight()
{
	return m_dwMenuItemHeight;
}

VOID CDrawMgr::SetMenuItemHeight(const UINT32 dwItemHeight)
{
	m_dwMenuItemHeight = dwItemHeight;
}

UINT32 CDrawMgr::GetMenuItemArrowWidth()
{
	return m_dwMenuItemArrowWidth;
}

VOID CDrawMgr::SetMenuItemArrowWidth(const UINT32 dwItemArrowWidth)
{
	m_dwMenuItemArrowWidth = dwItemArrowWidth;
}

UINT32 CDrawMgr::GetMenuSeperatorHeight()
{
	return m_dwMenuSeperatorHeight;
}

VOID CDrawMgr::SetMenuSeperatorHeight(const UINT32 dwSeperatorHeight)
{
	m_dwMenuSeperatorHeight = dwSeperatorHeight;
}
UINT32 CDrawMgr::GetMenuItemOffSetX()
{
	return m_dwMenuOffSetX;
}

VOID CDrawMgr::SetMenuItemOffSetX(const UINT32 dwOffSetX)
{
	m_dwMenuOffSetX = dwOffSetX; 
}

UINT32 CDrawMgr::GetMenuItemOffSetY()
{
	return m_dwMenuOffSetY;
}

VOID CDrawMgr::SetMenuItemOffSetY(const UINT32 dwOffSetY)
{
	m_dwMenuOffSetY = dwOffSetY; 
}

SIZE CDrawMgr::GetMenuRound()
{
	return m_szMenuRound;
}

VOID CDrawMgr::SetMenuRound(SIZE sz)
{
	m_szMenuRound = sz;
}

UINT32 CDrawMgr::GetMenuSidebarWidth()
{
	return m_dwSidebarWidth;
}

VOID CDrawMgr::SetMenuSidebarWidth(UINT32 dwWidth)
{
	m_dwSidebarWidth = dwWidth;
}

// CRXSkinMenu参数设定  结束

// CRXSkinDatePicker参数设定

SIZE CDrawMgr::GetDatePickerSize()
{
	return m_szDatePicker;
}

// CRXSkinDatePicker参数设定  结束

// CRXSkinLocationPicker参数设定

SIZE CDrawMgr::GetLocationPickerSize()
{
	return m_szLocationPicker;
}

// CRXSkinLocationPicker参数设定  结束

// CRXSkinOutbar 参数
SIZE CDrawMgr::GetOutbarOffSet()
{
	return m_szOutbarOffSet;
}

VOID CDrawMgr::SetOutbarOffSet(SIZE szRound)
{
	m_szOutbarOffSet = szRound;
}

SIZE CDrawMgr::GetOutbarRound()
{
	return m_szOutbarRound;
}

VOID CDrawMgr::SetOutbarRound(SIZE szRound)
{
	m_szOutbarRound = szRound;
}

INT32 CDrawMgr::GetOutbarItemIndentationX()
{
	return m_nOutbarItemIndentationX;
}

VOID CDrawMgr::SetOutbarItemIndentationX(INT32 nIndentationX)
{
	m_nOutbarItemIndentationX = nIndentationX;
}

INT32 CDrawMgr::GetOutbarItemHeight()
{
	return m_nOutbarItemHeight;
}

VOID CDrawMgr::SetOutbarItemHeight(INT32 nOutbarItemHeight)
{
	m_nOutbarItemHeight = nOutbarItemHeight;
}

COLORREF CDrawMgr::GetOutbarItemMouseOnText()
{
	return m_clrOutbarItemMouseOnText;
}

VOID CDrawMgr::SetOutbarItemMouseOnText(COLORREF clr)
{
	m_clrOutbarItemMouseOnText = clr;
}

COLORREF CDrawMgr::GetOutbarItemNormalText()
{
	return m_clrOutbarItemNormalText;
}

VOID CDrawMgr::SetOutbarItemNormalText(COLORREF clr)
{
	m_clrOutbarItemNormalText = clr;
}

COLORREF CDrawMgr::GetOutbarItemSelectedText()
{
	return m_clrOutbarItemSecletedText;
}

VOID CDrawMgr::SetOutbarItemSelectedText(COLORREF clr)
{
	m_clrOutbarItemSecletedText = clr;
}

COLORREF CDrawMgr::GetOutbarItemSelectedMouseOnFrom()
{
	return m_clrOutbarItemSecletedMouseOnFrom;
}

VOID CDrawMgr::SetOutbarItemSelectedMouseOnFrom(COLORREF clr)
{
	m_clrOutbarItemSecletedMouseOnFrom = clr;
}

COLORREF CDrawMgr::GetOutbarItemSelectedMouseOnTo()
{
	return m_clrOutbarItemSecletedMouseOnTo;
}

VOID CDrawMgr::SetOutbarItemSelectedMouseOnTo(COLORREF clr)
{
	m_clrOutbarItemSecletedMouseOnTo = clr;
}

COLORREF CDrawMgr::GetOutbarItemSelectedNormalFrom()
{
	return m_clrOutbarItemSecletedNormalFrom;
}

VOID CDrawMgr::SetOutbarItemSelectedNormalFrom(COLORREF clr)
{
	m_clrOutbarItemSecletedNormalFrom = clr;
}

COLORREF CDrawMgr::GetOutbarItemSelectedNormalTo()
{
	return m_clrOutbarItemSecletedNormalTo;
}

VOID CDrawMgr::SetOutbarItemSelectedNormalTo(COLORREF clr)
{
	m_clrOutbarItemSecletedNormalTo = clr;
}

INT32 CDrawMgr::GetOutbarFolderHeight()
{
	return m_nOutbarFolderHeight;
}
VOID CDrawMgr::SetOutbarFolderHeight(INT32 nOutbarFolderHeight)
{
	m_nOutbarFolderHeight = nOutbarFolderHeight;
}

SIZE CDrawMgr::GetOutbarFolderOffSet()
{
	return m_szOutbarFolderOffSet;
}

VOID CDrawMgr::SetOutbarFolderOffSet(SIZE szRound)
{
	m_szOutbarFolderOffSet = szRound;
}

COLORREF CDrawMgr::GetOutbarFolderNormalBorder()
{
	return m_clrOutbarFolderNormalBorder;
}
VOID CDrawMgr::SetOutbarFolderNormalBorder(COLORREF clr)
{
	m_clrOutbarFolderNormalBorder = clr;
}

COLORREF CDrawMgr::GetOutbarFolderNormalFrom()
{
	return m_clrOutbarFolderNormalFrom;
}
VOID CDrawMgr::SetOutbarFolderNormalFrom(COLORREF clr)
{
	m_clrOutbarFolderNormalFrom = clr;
}

COLORREF CDrawMgr::GetOutbarFolderNormalTo()
{
	return m_clrOutbarFolderNormalTo;
}

VOID CDrawMgr::SetOutbarFolderNormalTo(COLORREF clr)
{
	m_clrOutbarFolderNormalTo = clr;
}

COLORREF CDrawMgr::GetOutbarFolderNormalText()
{
	return m_clrOutbarFolderNormalText;
}
VOID CDrawMgr::SetOutbarFolderNormalText(COLORREF clr)
{
	m_clrOutbarFolderNormalText = clr;
}

COLORREF CDrawMgr::GetOutbarBKFrom()
{
	return m_clrOutbarBkFrom;
}

VOID CDrawMgr::SetOutbarBKFrom(COLORREF clr)
{
	m_clrOutbarBkFrom = clr;
}

COLORREF CDrawMgr::GetOutbarBKTo()
{
	return m_clrOutbarBkTo;
}

VOID CDrawMgr::SetOutbarBKTo(COLORREF clr)
{
	m_clrOutbarBkTo = clr;
}

COLORREF CDrawMgr::GetOutbarFolderPressedBorder()
{
	return m_clrOutbarFolderPressedBorder;
}
VOID CDrawMgr::SetOutbarFolderPressedBorder(COLORREF clr)
{
	m_clrOutbarFolderPressedBorder = clr;
}

COLORREF CDrawMgr::GetOutbarFolderPressedFrom()
{
	return m_clrOutbarFolderPressedFrom;
}
VOID CDrawMgr::SetOutbarFolderPressedFrom(COLORREF clr)
{
	m_clrOutbarFolderPressedFrom = clr;
}

COLORREF CDrawMgr::GetOutbarFolderPressedTo()
{
	return m_clrOutbarFolderPressedTo;
}
VOID CDrawMgr::SetOutbarFolderPressedTo(COLORREF clr)
{
	m_clrOutbarFolderPressedTo = clr;
}

COLORREF CDrawMgr::GetOutbarFolderPressedText()
{
	return m_clrOutbarFolderPressedText;
}
VOID CDrawMgr::SetOutbarFolderPressedText(COLORREF clr)
{
	m_clrOutbarFolderPressedText = clr;
}

COLORREF CDrawMgr::GetOutbarFolderMouseOnFirstBorder()
{
	return m_clrOutbarFolderMouseOnFirstBorder;
}
VOID CDrawMgr::SetOutbarFolderMouseOnFirstBorder(COLORREF clr)
{
	m_clrOutbarFolderMouseOnFirstBorder = clr;
}

COLORREF CDrawMgr::GetOutbarFolderMouseOnSecondBorder()
{
	return m_clrOutbarFolderMouseOnSecondeBorder;
}
VOID CDrawMgr::SetOutbarFolderMouseOnSecondBorder(COLORREF clr)
{
	m_clrOutbarFolderMouseOnSecondeBorder = clr;
}

COLORREF CDrawMgr::GetOutbarFolderMouseOnThirdBorder()
{
	return m_clrOutbarFolderMouseOnThirdBorder;
}
VOID  CDrawMgr::SetOutbarFolderMouseOnThirdBorder(COLORREF clr)
{
	m_clrOutbarFolderMouseOnThirdBorder = clr;
}

COLORREF CDrawMgr::GetOutbarFolderMouseOnFrom()
{
	return m_clrOutbarFolderMouseOnFrom;
}	
VOID CDrawMgr::SetOutbarFolderMouseOnFrom(COLORREF clr)
{
	m_clrOutbarFolderMouseOnFrom = clr;
}


COLORREF CDrawMgr::GetOutbarFolderMouseOnTo()
{
	return m_clrOutbarFolderMouseOnTo;
}	
VOID CDrawMgr::SetOutbarFolderMouseOnTo(COLORREF clr)
{
	m_clrOutbarFolderMouseOnTo = clr;
}

COLORREF CDrawMgr::GetOutbarFolderMouseOnText()
{
	return m_clrOutbarFolderMouseOnText;
}

VOID CDrawMgr::SetOutbarFolderMouseOnText(COLORREF clr)
{
	m_clrOutbarFolderMouseOnText = clr;
}

// CRXSkinOutbar 参数  结束

// 全局相关参数
COLORREF CDrawMgr::GetCommonBk()
{
	return m_clrCommonBk;
}
COLORREF CDrawMgr::GetCommonBorderColor()
{
	return m_clrCommonBorder;
}

COLORREF CDrawMgr::GetHyperLinkText()
{
	return m_clrHyperLinkText;
}
COLORREF CDrawMgr::GetHyperLinkHotText()
{
	return m_clrHyperLinkHotText;
}
SIZE CDrawMgr::GetCommonBorderSize()
{
	return m_szControlBorder;
}

COLORREF CDrawMgr::GetArrowNormal()
{
	return m_clrArrowNormal;
}

COLORREF CDrawMgr::GetArrowMouseOn()
{
	return m_clrArrowMouseOn;
}

COLORREF CDrawMgr::GetArrowPressed()
{
	return m_clrArrowPressed;
}

UINT32 CDrawMgr::GetArrowHeight()
{
	return m_dwArrowHeight;
}

COLORREF CDrawMgr::GetDisableText()
{
	return m_clrDisableText;
}

VOID CDrawMgr::SetDiableText(const COLORREF clr)
{
	m_clrDisableText = clr;
}

COLORREF CDrawMgr::GetNormalText()
{
	return m_clrNormalText;
}

VOID CDrawMgr::SetNormalText(const COLORREF clr)
{
	m_clrNormalText = clr;
}

// 全局相关参数 结束

// CRXSkinButtonList参数设定
COLORREF CDrawMgr::GetButtonListSelectedNormalFrom()
{
	return m_clrButtonListSelectedNormalFrom;
}

VOID CDrawMgr::SetButtonListSelectedNormalFrom(COLORREF clr)
{
	m_clrButtonListSelectedNormalFrom = clr;
}

COLORREF CDrawMgr::GetButtonListSelectedNormalTo()
{
	return m_clrButtonListSelectedNormalTo;
}

VOID CDrawMgr::SetButtonListSelectedNormalTo(COLORREF clr)
{
	m_clrButtonListSelectedNormalTo = clr;
}

COLORREF CDrawMgr::GetButtonListSelectedNormalText()
{
	return m_clrButtonListSelectedNormalText;
}

VOID CDrawMgr::SetButtonListSelectedNormalText(COLORREF clr)
{
	m_clrButtonListSelectedNormalText = clr;
}


COLORREF CDrawMgr::GetButtonListSelectedMouseOnFrom()
{
	return m_clrButtonListSelectedMouseOnFrom;
}

VOID CDrawMgr::SetButtonListSelectedMouseOnFrom(COLORREF clr)
{
	m_clrButtonListSelectedMouseOnFrom = clr;
}


COLORREF CDrawMgr::GetButtonListSelectedMouseOnTo()
{
	return m_clrButtonListSelectedMouseOnTo;
}

VOID CDrawMgr::SetButtonListSelectedMouseOnTo(COLORREF clr)
{
	m_clrButtonListSelectedMouseOnTo = clr;
}

COLORREF CDrawMgr::GetButtonListSelectedMouseOnText()
{
	return m_clrButtonListSelectedMouseOnText;
}

VOID CDrawMgr::SetButtonListSelectedMouseOnText(COLORREF clr)
{
	m_clrButtonListSelectedMouseOnText = clr;
}

COLORREF CDrawMgr::GetButtonListSelectedPressedFrom()
{
	return m_clrButtonListSelectedPressedFrom;
}

VOID CDrawMgr::SetButtonListSelectedPressedFrom(COLORREF clr)
{
	m_clrButtonListSelectedPressedFrom = clr;
}


COLORREF CDrawMgr::GetButtonListSelectedPressedTo()
{
	return m_clrButtonListSelectedPressedTo;
}

VOID CDrawMgr::SetButtonListSelectedPressedTo(COLORREF clr)
{
	m_clrButtonListSelectedPressedTo = clr;
}

COLORREF CDrawMgr::GetButtonListSelectedPressedText()
{
	return m_clrButtonListSelectedPressedText;
}

VOID CDrawMgr::SetButtonListSelectedPressedText(COLORREF clr)
{
	m_clrButtonListSelectedPressedText = clr;
}

INT32 CDrawMgr::GetButtonListOffSetX()
{
	return m_nButtonListOffSetX;
}

VOID  CDrawMgr::SetButtonListOffSetX(INT32 nOffSetX)
{
	m_nButtonListOffSetX = nOffSetX;
}

INT32 CDrawMgr::GetButtonListOffSetY()
{
	return m_nButtonListOffSetY;
}

VOID  CDrawMgr::SetButtonListOffSetY(INT32 nOffSetY)
{
	m_nButtonListOffSetY = nOffSetY;
}

COLORREF CDrawMgr::GetButtonListNormalText()
{
	return m_clrButtonListNormalText;
}

VOID CDrawMgr::SetButtonListNormalText(COLORREF clr)
{
	m_clrButtonListNormalText = clr;
}

COLORREF CDrawMgr::GetButtonListMouseOnText()
{
	return m_clrButtonListMouseOnText;
}

VOID CDrawMgr::SetButtonListMouseOnText(COLORREF clr)
{
	m_clrButtonListMouseOnText = clr;
}

COLORREF CDrawMgr::GetButtonListPressedText()
{
	return m_clrButtonListPressedText;
}
VOID CDrawMgr::SetButtonListPressedText(COLORREF clr)
{
	m_clrButtonListPressedText = clr;
}

SIZE CDrawMgr::GetButtonListRound()
{
	return m_szButtonListRound;
}

VOID CDrawMgr::SetButtonListRound(SIZE sz)
{
	m_szButtonListRound = sz;
}

// CRXSkinButtonList参数设定 结束

//  CRXSkinSearch 相关参数

UINT32 CDrawMgr::GetBrownserOffSetX()
{
	return m_dwBrownserOffSetX;
}

VOID CDrawMgr::SetBrownserOffSetX(const UINT32 dwOffSetX)
{
	m_dwBrownserOffSetX = dwOffSetX;
}


//  CRXSkinSearch 相关参数  结束

//  CRXSkinImageSel 相关参数
INT32 CDrawMgr::GetImageSelOffSetX()
{
	return m_nImageSelOffSetX;
}
VOID CDrawMgr::SetImageSelOffSetX(INT32 nOffSetX)
{
	m_nImageSelOffSetX = nOffSetX;
}

INT32 CDrawMgr::GetImageSelOffSetY()
{
	return m_nImageSelOffSetY;
}
VOID CDrawMgr::SetImageSelOffSetY(INT32 nOffSetY)
{
	m_nImageSelOffSetY = nOffSetY;
}

SIZE CDrawMgr::GetImageSelItem()
{
	return m_szImageSelItem;
}

VOID CDrawMgr::SetImageSelItem(SIZE sz)
{
	m_szImageSelItem = sz;
}
//  CRXSkinImageSel 相关参数  结束

// CRXSkinListCtrl  相关参数
INT32 CDrawMgr::GetListCtrlHeaderHeight()
{
	return m_nListCtrlHeaderHeight;
}

VOID CDrawMgr::SetListCtrlHeaderHeight(const INT32 nHeight)
{
	m_nListCtrlHeaderHeight = nHeight;
}

INT32 CDrawMgr::GetListCtrlItemHeight()
{
	return m_nListCtrlItemHeight;
}

VOID CDrawMgr::SetListCtrlItemHeight(const INT32 nHeight)
{
	m_nListCtrlItemHeight = nHeight;
}

INT32 CDrawMgr::GetListCtrlOffSetX()
{
	return m_nListCtrlOffSetX;
}

VOID CDrawMgr::SetListCtrlOffSetX(const INT32 nListCtrlOffSetX)
{
	m_nListCtrlOffSetX = nListCtrlOffSetX;
}

INT32 CDrawMgr::GetListCtrlOffSetY()
{
	return m_nListCtrlOffSetY;
}

VOID CDrawMgr::SetListCtrlOffSetY(const INT32 nListCtrlOffSetY)
{
	m_nListCtrlOffSetY = nListCtrlOffSetY;
}

COLORREF CDrawMgr::GetListCtrlBK()
{
	return m_clrListCtrlBK;
}

VOID CDrawMgr::SetListCtrlBK(COLORREF clr)
{
	m_clrListCtrlBK = clr;
}

COLORREF CDrawMgr::GetListCtrlBorder()
{
	return m_clrListCtrlBorder;
}

VOID CDrawMgr::SetListCtrlBorder(COLORREF clr)
{
	m_clrListCtrlBorder = clr;
}

COLORREF CDrawMgr::GetListCtrlHeaderFrom()
{
	return m_clrListCtrlHeaderFrom;
}

VOID CDrawMgr::SetListCtrlHeaderFrom(COLORREF clr)
{
	m_clrListCtrlHeaderFrom = clr;
}

COLORREF CDrawMgr::GetListCtrlHeaderTo()
{
	return m_clrListCtrlHeaderTo;
}

VOID CDrawMgr::SetListCtrlHeaderTo(COLORREF clr)
{
	m_clrListCtrlHeaderTo = clr;
}

COLORREF CDrawMgr::GetListCtrlItemSelecedTo()
{
	return m_clrListCtrlRowSelectedTo;
}

VOID CDrawMgr::SetListCtrlItemSelecedTo(COLORREF clr)
{
	m_clrListCtrlRowSelectedTo = clr;
}

COLORREF CDrawMgr::GetListCtrlItemSelecedText()
{
	return m_clrListCtrlRowSelectedText;
}

VOID CDrawMgr::SetListCtrlItemSelecedText(COLORREF clr)
{
	m_clrListCtrlRowSelectedText = clr;
}
COLORREF CDrawMgr::GetListCtrlItemSelecedFrom()
{
	return m_clrListCtrlRowSelectedFrom;
}

VOID CDrawMgr::SetListCtrlItemSelecedFrom(COLORREF clr)
{
	m_clrListCtrlRowSelectedFrom = clr;
}



COLORREF CDrawMgr::GetListCtrlItemMouseOnTo()
{
	return m_clrListCtrlRowMouseOnTo;
}

VOID CDrawMgr::SetListCtrlItemMouseOnTo(COLORREF clr)
{
	m_clrListCtrlRowMouseOnTo = clr;
}


COLORREF CDrawMgr::GetListCtrlItemMouseOnFrom()
{
	return m_clrListCtrlRowMouseOnFrom;
}

VOID CDrawMgr::SetListCtrlItemMouseOnFrom(COLORREF clr)
{
	m_clrListCtrlRowMouseOnFrom = clr;
}


// CRXSkinListCtrl  相关参数  结束

//  CRXSkinSpinEdit 相关参数
INT32 CDrawMgr::GetSpinEditArrowHeight()
{
	return m_nSpinEditArrowHeight;
}

VOID CDrawMgr::SetSpinEditArrowHeight(INT32 nSpinEditArrowHeight)
{
	m_nSpinEditArrowHeight = nSpinEditArrowHeight;
}
//  CRXSkinSpinEdit 相关参数  结束


double CDrawMgr::DrawImage(IRXImage* poImage,RECT rcImage,HDC hDC)
{
	double  dbScale = 1.0;
	double  dbX = (double)poImage->GetWidth() / (double)(rcImage.right - rcImage.left);
	double  dbY = (double)poImage->GetHeight() / (double)(rcImage.bottom - rcImage.top);
	if (dbX>dbScale)
	{
		dbScale = dbX;
	}

	if (dbY>dbScale)
	{
		dbScale = dbY;
	}

	INT32 nWidth = (INT32)((double)poImage->GetWidth() / dbScale);
	INT32 nHeight = (INT32)((double)poImage->GetHeight() / dbScale);
	INT32 nTop = rcImage.top  + ((rcImage.bottom - rcImage.top) - nHeight)/2;
	INT32 nLeft = rcImage.left  + ((rcImage.right - rcImage.left) - nWidth)/2;
	poImage->Stretch(hDC,nLeft,nTop,nWidth,nHeight);
	return dbScale;
}

VOID CDrawMgr::SetLayeredWindowAttributes(HWND hWnd,COLORREF clr,INT32 nAlpha)
{
	if (m_lpfnSetLayeredWindowAttributes)
	{
		m_lpfnSetLayeredWindowAttributes(hWnd,clr,nAlpha,ULW_ALPHA);
	}
}

BOOL CDrawMgr::_InitLayeredWindows()
{
	if( m_lpfnUpdateLayeredWindow == NULL )
	{
		m_hUser32 = GetModuleHandle(_T("USER32.DLL"));
		if (m_hUser32)
		{
			m_lpfnUpdateLayeredWindow =
										(lpfnUpdateLayeredWindow)GetProcAddress
										(
										m_hUser32,
										("UpdateLayeredWindow")
										);

			m_lpfnSetLayeredWindowAttributes =
										(lpfnSetLayeredWindowAttributes)GetProcAddress
										(
										m_hUser32,
										("SetLayeredWindowAttributes")
										);
			return m_lpfnUpdateLayeredWindow != NULL ? TRUE:FALSE;
		}
	}

	return TRUE;
}

VOID CDrawMgr::DrawControlBorder(HDC hDC,
								 EControlState enState,
								 RECT rcControl,
								 BOOL bIsEnable)
{
	IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().FindImage(ID_CONTROL_BORDER);
	if (poImage)
	{
		_DrawControlBorderLeft(hDC,enState,rcControl,poImage,bIsEnable);
		_DrawControlBorderTop(hDC,enState,rcControl,poImage,bIsEnable);
		_DrawControlBorderBottom(hDC,enState,rcControl,poImage,bIsEnable);
		_DrawControlBorderRight(hDC,enState,rcControl,poImage,bIsEnable);
	}
}

VOID CDrawMgr::_DrawControlBorderLeft(HDC hDC,
									  EControlState enState,
									  RECT rcControl,
									  IRXImage* poImage,
									  BOOL bIsEnable)
{
	UINT32 dwImageHeight = poImage->GetHeight() / 3;
	UINT32 dwOffSetY = 0;
	switch(enState)
	{
	case CONTROL_STATE_NORMAL:
		{
			dwOffSetY = 0;
		}
		break;
	case CONTROL_STATE_MOUSEON:
		{
			dwOffSetY = dwImageHeight;
		}
		break;
	case CONTROL_STATE_PRESSED:
		{
			dwOffSetY = 2 * dwImageHeight;
		}
	default:
		break;
	}
	RECT rcImage = rcControl;
	rcImage.right = rcControl.left + m_stLeft.m_dwTotal;
	rcImage.bottom = rcImage.top + m_stLeft.m_dwFirstOffSet;
	if (bIsEnable)
	{
		poImage->Draw(hDC,rcImage,0,dwOffSetY);
	}
	else
	{
		poImage->DrawGray(hDC,rcImage,0,dwOffSetY);
	}
	
	rcImage.top = rcControl.bottom - (dwImageHeight-m_stLeft.m_dwSecondOffSet);
	rcImage.bottom = rcControl.bottom;
	if (bIsEnable)
	{
		poImage->Draw(hDC,rcImage,0,m_stLeft.m_dwSecondOffSet+dwOffSetY);
	}
	else
	{
		poImage->DrawGray(hDC,rcImage,0,m_stLeft.m_dwSecondOffSet+dwOffSetY);
	}
	UINT32 dwMiddleWidth = m_stLeft.m_dwSecondOffSet - m_stLeft.m_dwFirstOffSet;
	UINT32 dwLessHeight = (rcControl.bottom - rcControl.top) - m_stLeft.m_dwFirstOffSet - (dwImageHeight-m_stLeft.m_dwSecondOffSet);
	INT32 nCount = dwLessHeight / dwMiddleWidth;
	INT32 nTop = rcControl.top + m_stLeft.m_dwFirstOffSet;
	for (INT32 n = 0; n < nCount; n ++)
	{
		rcImage.top = nTop;
		rcImage.bottom = rcImage.top + dwMiddleWidth;
		if (bIsEnable)
		{
			poImage->Draw(hDC,rcImage,0,m_stLeft.m_dwFirstOffSet+dwOffSetY);
		}
		else
		{
			poImage->DrawGray(hDC,rcImage,0,m_stLeft.m_dwFirstOffSet+dwOffSetY);
		}
		nTop += dwMiddleWidth;
	}
	if (dwLessHeight%dwMiddleWidth)
	{
		rcImage.top = rcControl.bottom - (dwImageHeight-m_stLeft.m_dwSecondOffSet) - dwMiddleWidth;
		rcImage.bottom = rcImage.top + dwMiddleWidth;
		if (bIsEnable)
			poImage->Draw(hDC,rcImage,0,m_stLeft.m_dwFirstOffSet+dwOffSetY);
		else
			poImage->DrawGray(hDC,rcImage,0,m_stLeft.m_dwFirstOffSet+dwOffSetY);
	}
}

VOID CDrawMgr::_DrawControlBorderRight(HDC hDC,
									   EControlState enState,
									   RECT rcControl,
									   IRXImage* poImage,
									   BOOL bIsEnable)
{
	UINT32 dwImageHeight = poImage->GetHeight() / 3;
	UINT32 dwOffSetY = 0;
	switch(enState)
	{
	case CONTROL_STATE_NORMAL:
		{
			dwOffSetY = 0;
		}
		break;
	case CONTROL_STATE_MOUSEON:
		{
			dwOffSetY = dwImageHeight;
		}
		break;
	case CONTROL_STATE_PRESSED:
		{
			dwOffSetY = 2 * dwImageHeight;
		}
	default:
		break;
	}
	RECT rcImage = rcControl;
	rcImage.left = rcControl.right - m_stRight.m_dwTotal;
	rcImage.bottom = rcImage.top + m_stRight.m_dwFirstOffSet;
	if (bIsEnable)
		poImage->Draw(hDC,rcImage,poImage->GetWidth() - m_stRight.m_dwTotal,dwOffSetY);
	else
		poImage->DrawGray(hDC,rcImage,poImage->GetWidth() - m_stRight.m_dwTotal,dwOffSetY);
	rcImage.top = rcControl.bottom - (dwImageHeight-m_stRight.m_dwSecondOffSet);
	rcImage.bottom = rcControl.bottom;
	if (bIsEnable)
		poImage->Draw(hDC,rcImage,poImage->GetWidth() - m_stRight.m_dwTotal,m_stRight.m_dwSecondOffSet+dwOffSetY);
	else
		poImage->DrawGray(hDC,rcImage,poImage->GetWidth() - m_stRight.m_dwTotal,m_stRight.m_dwSecondOffSet+dwOffSetY);

	UINT32 dwMiddleWidth = m_stRight.m_dwSecondOffSet - m_stRight.m_dwFirstOffSet;
	UINT32 dwLessHeight = (rcControl.bottom - rcControl.top) - m_stRight.m_dwFirstOffSet - (dwImageHeight-m_stRight.m_dwSecondOffSet);
	INT32 nCount = dwLessHeight / dwMiddleWidth;
	INT32 nTop = rcControl.top + m_stRight.m_dwFirstOffSet;
	for (INT32 n = 0; n < nCount; n ++)
	{
		rcImage.top = nTop;
		rcImage.bottom = rcImage.top + dwMiddleWidth;
		if (bIsEnable)
			poImage->Draw(hDC,rcImage,poImage->GetWidth() - m_stRight.m_dwTotal,m_stRight.m_dwFirstOffSet+dwOffSetY);
		else
			poImage->DrawGray(hDC,rcImage,poImage->GetWidth() - m_stRight.m_dwTotal,m_stRight.m_dwFirstOffSet+dwOffSetY);
		nTop += dwMiddleWidth;
	}
	if (dwLessHeight%dwMiddleWidth)
	{
		rcImage.top = rcControl.bottom - (dwImageHeight-m_stRight.m_dwSecondOffSet) - dwMiddleWidth;
		rcImage.bottom = rcImage.top + dwMiddleWidth;
		if (bIsEnable)
			poImage->Draw(hDC,rcImage,poImage->GetWidth() - m_stRight.m_dwTotal,m_stRight.m_dwFirstOffSet+dwOffSetY);
		else
			poImage->DrawGray(hDC,rcImage,poImage->GetWidth() - m_stRight.m_dwTotal,m_stRight.m_dwFirstOffSet+dwOffSetY);
	}
}

VOID CDrawMgr::_DrawControlBorderTop(HDC hDC,
									 EControlState enState,
									 RECT rcControl,
									 IRXImage* poImage,
									 BOOL bIsEnable)
{
	UINT32 dwImageHeight = poImage->GetHeight() / 3;
	UINT32 dwOffSetY = 0;
	switch(enState)
	{
	case CONTROL_STATE_NORMAL:
		{
			dwOffSetY = 0;
		}
		break;
	case CONTROL_STATE_MOUSEON:
		{
			dwOffSetY = dwImageHeight;
		}
		break;
	case CONTROL_STATE_PRESSED:
		{
			dwOffSetY = 2 * dwImageHeight;
		}
	default:
		break;
	}
	RECT rcImage = rcControl;
	rcImage.right = rcControl.left + m_stTop.m_dwFirstOffSet;
	rcImage.bottom = rcImage.top + m_stTop.m_dwTotal;
	if (bIsEnable)
		poImage->Draw(hDC,rcImage,0,dwOffSetY);
	else
		poImage->DrawGray(hDC,rcImage,0,dwOffSetY);
	rcImage.left = rcControl.right - (poImage->GetWidth()-m_stTop.m_dwSecondOffSet);
	rcImage.right = rcControl.right;
	if (bIsEnable)
		poImage->Draw(hDC,rcImage,m_stTop.m_dwSecondOffSet,dwOffSetY);
	else
		poImage->DrawGray(hDC,rcImage,m_stTop.m_dwSecondOffSet,dwOffSetY);

	UINT32 dwMiddleWidth = m_stTop.m_dwSecondOffSet - m_stTop.m_dwFirstOffSet;
	UINT32 dwLessWidth = (rcControl.right - rcControl.left) - m_stTop.m_dwFirstOffSet - (poImage->GetWidth()-m_stTop.m_dwSecondOffSet);
	INT32 nCount = dwLessWidth / dwMiddleWidth;
	INT32 nLeft = rcControl.left + m_stTop.m_dwFirstOffSet;
	for (INT32 n = 0; n < nCount; n ++)
	{
		rcImage.left = nLeft;
		rcImage.right = rcImage.left + dwMiddleWidth;
		if (bIsEnable)
			poImage->Draw(hDC,rcImage,m_stTop.m_dwFirstOffSet,dwOffSetY);
		else
			poImage->DrawGray(hDC,rcImage,m_stTop.m_dwFirstOffSet,dwOffSetY);
		nLeft += dwMiddleWidth;
	}
	if (dwLessWidth%dwMiddleWidth)
	{
		rcImage.left = rcControl.right - (poImage->GetWidth()-m_stTop.m_dwSecondOffSet) - dwMiddleWidth;
		rcImage.right = rcImage.left + dwMiddleWidth;
		if (bIsEnable)
			poImage->Draw(hDC,rcImage,poImage->GetWidth() - m_stTop.m_dwFirstOffSet,dwOffSetY);
		else
			poImage->DrawGray(hDC,rcImage,poImage->GetWidth() - m_stTop.m_dwFirstOffSet,dwOffSetY);
	}
}

VOID CDrawMgr::_DrawControlBorderBottom(HDC hDC,
										EControlState enState,
										RECT rcControl,
										IRXImage* poImage,
										BOOL bIsEnable)
{
	UINT32 dwImageHeight = poImage->GetHeight() / 3;
	UINT32 dwOffSetY = 0;
	switch(enState)
	{
	case CONTROL_STATE_NORMAL:
		{
			dwOffSetY = 0;
		}
		break;
	case CONTROL_STATE_MOUSEON:
		{
			dwOffSetY = dwImageHeight;
		}
		break;
	case CONTROL_STATE_PRESSED:
		{
			dwOffSetY = 2 * dwImageHeight;
		}
	default:
		break;
	}
	RECT rcImage = rcControl;
	rcImage.right = rcControl.left + m_stBottom.m_dwFirstOffSet;
	rcImage.top = rcControl.bottom - m_stBottom.m_dwTotal;
	if (bIsEnable)
		poImage->Draw(hDC,rcImage,0,dwOffSetY + dwImageHeight - m_stBottom.m_dwTotal);
	else
		poImage->DrawGray(hDC,rcImage,0,dwOffSetY + dwImageHeight - m_stBottom.m_dwTotal);
	rcImage.left = rcControl.right - (poImage->GetWidth()-m_stRight.m_dwSecondOffSet);
	rcImage.right = rcControl.right;
	if (bIsEnable)
		poImage->Draw(hDC,rcImage,m_stBottom.m_dwSecondOffSet,dwOffSetY + dwImageHeight - m_stBottom.m_dwTotal);
	else
		poImage->DrawGray(hDC,rcImage,m_stBottom.m_dwSecondOffSet,dwOffSetY + dwImageHeight - m_stBottom.m_dwTotal);

	UINT32 dwMiddleWidth = m_stBottom.m_dwSecondOffSet - m_stBottom.m_dwFirstOffSet;
	UINT32 dwLessWidth = (rcControl.right - rcControl.left) - m_stBottom.m_dwFirstOffSet - (poImage->GetWidth()-m_stBottom.m_dwSecondOffSet);
	INT32 nCount = dwLessWidth / dwMiddleWidth;
	INT32 nLeft = rcControl.left + m_stTop.m_dwFirstOffSet;
	for (INT32 n = 0; n < nCount; n ++)
	{
		rcImage.left = nLeft;
		rcImage.right = rcImage.left + dwMiddleWidth;
		if (bIsEnable)
			poImage->Draw(hDC,rcImage,m_stBottom.m_dwFirstOffSet,dwOffSetY + dwImageHeight - m_stBottom.m_dwTotal);
		else
			poImage->DrawGray(hDC,rcImage,m_stBottom.m_dwFirstOffSet,dwOffSetY + dwImageHeight - m_stBottom.m_dwTotal);
		nLeft += dwMiddleWidth;
	}
	if (dwLessWidth%dwMiddleWidth)
	{
		rcImage.left = rcControl.right - (poImage->GetWidth()-m_stTop.m_dwSecondOffSet) - dwMiddleWidth;
		rcImage.right = rcImage.left + dwMiddleWidth;
		if (bIsEnable)
			poImage->Draw(hDC,rcImage,poImage->GetWidth() - m_stBottom.m_dwFirstOffSet,dwOffSetY + dwImageHeight - m_stBottom.m_dwTotal);
		else
			poImage->DrawGray(hDC,rcImage,poImage->GetWidth() - m_stBottom.m_dwFirstOffSet,dwOffSetY + dwImageHeight - m_stBottom.m_dwTotal);
	}
}

void CDrawMgr::_MovePos(const POINT &posStart , POINT &posCurr , int nColWidth , int nRowHeight , int nCharWidth , int nCount)  
{
	int i; 
	if(nColWidth < nCharWidth)  
		return;  
	if(posStart.x > posCurr.x)  
		posCurr.x = posStart.x; 
	if(posStart.y > posCurr.y)  
		posCurr.y = posStart.y; 
	for(i = 0 ; i < nCount ; i++) 
	{  
		posCurr.x += nCharWidth;  
		if(posCurr.x > nColWidth)  
		{  
			posCurr.x = posStart.x;  
			posCurr.y += nRowHeight;  
		}  
	}  

}

void CDrawMgr::DrawMultLineText(HDC hDC, CRXSkinRect rect, INT32 nRowDis, UINT nFromat, CRXSkinString strText)
{
	if( strText.GetLength() <= 0 )
		return;

	TCHAR* pText = (TCHAR*)strText.GetData();
	INT32 nCount = strText.GetLength();
	CRXSkinRect rtChar;
	SIZE szTxt;
	GetTextExtentPoint32(hDC,pText + 0,1,&szTxt);
	int nRowHeight = szTxt.cy + nRowDis;
	rtChar.top = rect.top;
	rtChar.left = rect.left;
	rtChar.bottom  = rtChar.top + nRowDis + szTxt.cy;
	rtChar.right  = rtChar.left + szTxt.cx; 
	CRXSkinString strChar;
	for (int nCharIndex = 0; nCharIndex < nCount; nCharIndex++)
	{
		if( rtChar.right > rect.right )
		{
			rtChar.top = rtChar.bottom;
			rtChar.bottom += nRowHeight;
			GetTextExtentPoint32(hDC,pText + nCharIndex,1,&szTxt);
			rtChar.left = rect.left;
			rtChar.right = rtChar.left + szTxt.cx;
			if( rtChar.bottom > rect.bottom )
				break;
		}
		strChar = pText[nCharIndex];
		DrawText(hDC,strChar,1, &rtChar, nFromat);
		GetTextExtentPoint32(hDC,pText + nCharIndex + 1,1,&szTxt);
		rtChar.left = rtChar.right;
		rtChar.right += szTxt.cx;
	}
}


BOOL CDrawMgr::_IsChineseChar(TCHAR* str)  
{  
	return *str < 0 && (*(str + 1)) < 0;

}

COLORREF CDrawMgr::GetColorFromElementAttri(VOID* poElement, const char* pszAtti)
{
	COLORREF clr = RGB(0,0,0);
	CHAR* pszColor = (CHAR*)((tinyxml2::XMLElement*)poElement)->Attribute(pszAtti);
	if (pszColor)
	{
		clr = GetColor(pszColor);
	}
	return clr;
}

SIZE CDrawMgr::GetSizeFromElementAttri(VOID* poElement, const char* pszAtti)
{
	SIZE sz;
	sz.cx = sz.cy = 0;
	CHAR* pszSize = (CHAR*)((tinyxml2::XMLElement*)poElement)->Attribute(pszAtti);
	if (pszSize)
	{
		sz = GetSize(pszSize);
	}
	return sz;
}

INT32 CDrawMgr::GetIN32FromElementAttri(VOID* poElement, const char* pszAtti)
{
	INT32 nValue;
	nValue = 0;
	CHAR* pszSize = (CHAR*)((tinyxml2::XMLElement*)poElement)->Attribute(pszAtti);
	if (pszSize)
	{
		nValue = atoi(pszSize);
	}
	return nValue;
}

CRXControl* CDrawMgr::CreateControl(tinyxml2::XMLElement* poElementControl, CRXControlMgr* poControlMgr, CRXSkinWnd* poParent, const EResourceType& enFromResource)
{
	CHAR* pszType = (CHAR*)poElementControl->Attribute("type");
	if (pszType)
	{
		CRXControl* poControl = NULL;
		EControlType enType = RXSkinGetControlType(pszType);
		if (CONTROL_NONE == enType)
		{
			poControl = poParent->OnCreateControl(poElementControl);
			if (NULL == poControl)
			{
				return NULL;
			}
		}
		switch (enType)
		{
		case CONTROL_STATIC:
		{
			poControl = new CRXSkinStatic;
			poControl->SetParentWnd(poParent->GetHWnd());
			CHAR* pszColor = (CHAR*)poElementControl->Attribute("textcolor");
			if (pszColor)
			{
				((CRXSkinStatic*)poControl)->SetTextColor(GetColor(pszColor));
			}

			CHAR* pszIsBold = (CHAR*)poElementControl->Attribute("isBold");
			if (pszIsBold)
			{
				if (strcmp(pszIsBold,"true") == 0)
				{
					((CRXSkinStatic*)poControl)->SetFontID(ID_FONT_COMMON_BOLD);
				}				
			}
			CHAR* pszIsCenter = (CHAR*)poElementControl->Attribute("isCenter");
			if (pszIsCenter)
			{
				if (strcmp(pszIsCenter, "true") == 0)
				{
					((CRXSkinStatic*)poControl)->SetCenter(true);
				}
			}
		}
		break;
		case CONTROL_HYPERLINK:
		{
			poControl = new CRXHyperLink;
			poControl->SetParentWnd(poParent->GetHWnd());
			switch (enFromResource)
			{
			case EResourceType::RESOURCE_FROM_FILE:
			{
				tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
				CHAR* pszSrc = (CHAR*)poElementControl->Attribute("src");
				if (pszSrc)
				{
					strPath += _RXTUTF82T(pszSrc);
				}
				((CRXHyperLink*)poControl)->SetImageFile(strPath.c_str());
			}
			break;
			case EResourceType::RESOURCE_FROM_EXE:
			{
				CHAR* pszSrc = (CHAR*)poElementControl->Attribute("src");
				if (pszSrc)
				{
					TCHAR* pszResource = MAKEINTRESOURCE(atoi(pszSrc));
					((CRXHyperLink*)poControl)->SetImageFile(pszResource);
				}				
			}
			break;
			default:
				break;
			}

		}
		break;
		case CONTROL_BG:
		{
			poControl = new CRXSkinBG;
			poControl->SetParentWnd(poParent->GetHWnd());
			
			CHAR* pszValue = (CHAR*)poElementControl->Attribute("fromcolor");
			if (pszValue)
			{
				((CRXSkinBG*)poControl)->SetFrom(GetColor(pszValue));
			}
			pszValue = (CHAR*)poElementControl->Attribute("tocolor");
			if (pszValue)
			{
				((CRXSkinBG*)poControl)->SetTo(GetColor(pszValue));
			}
		}
		break;
		case CONTROL_BUTTON_LIST:
		{
			poControl = new CRXListButtonList;
			poControl->SetParentWnd(poParent->GetHWnd());
			tinyxml2::XMLElement* poItem = poElementControl->FirstChildElement("Item");
			while (poItem)
			{
				CRXSkinString strItemname = _T("");
				tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
				INT32 nItemID = 0;
				CHAR* pszItemname = (CHAR*)poItem->Attribute("text");
				if (pszItemname)
				{
					strItemname = _RXTUTF82T(pszItemname).c_str();
				}
				CHAR* pszItemID = (CHAR*)poItem->Attribute("id");
				if (pszItemID)
				{
					nItemID = atoi(pszItemID);
				}
				CHAR* pszItemSrc = (CHAR*)poItem->Attribute("src");
				if (pszItemSrc)
				{
					switch (enFromResource)
					{
					case EResourceType::RESOURCE_FROM_FILE:
					{
						strPath += _RXTUTF82T(pszItemSrc).c_str();
					}
					break;
					case EResourceType::RESOURCE_FROM_EXE:
					{
						strPath = CRXSkinResourceMgr::Instance()->GetResource(pszItemSrc).c_str();
					}
					break;
					default:
						break;
					}
					
				}
				CRXSkinRect rcItem;
				CHAR* pszArea = (CHAR*)poItem->Attribute("area");
				if (pszArea)
				{
					rcItem = RXSkinGetRect(pszArea);
				}
				CRXListButton* poBtn = new CRXListButton;
				poBtn->SetID(nItemID);
				poBtn->SetParentWnd(poParent->GetHWnd());
				poBtn->SetRect(rcItem);
				poBtn->SetImage(strPath.c_str());
				poBtn->SetText(strItemname.GetData());
				CRXSkinResizePoint ptTopLeft, ptBottomRight;

				CHAR* pszAlignTopLeft = (CHAR*)poElementControl->Attribute("aligntopleft");
				if (pszAlignTopLeft)
				{
					ptTopLeft = RXSkinGetAlignType(pszAlignTopLeft);
				}
				CHAR* pszAlignBottomRight = (CHAR*)poElementControl->Attribute("alignbottomright");
				if (pszAlignBottomRight)
				{
					ptBottomRight = RXSkinGetAlignType(pszAlignBottomRight);
				}
				poBtn->SetResize(ptTopLeft, ptBottomRight);
				((CRXListButtonList*)poControl)->AddListButton(poBtn);
				poItem = poItem->NextSiblingElement("Item");
			}
			
		}
		break;
		case CONTROL_OUTBAR:
		{
			poControl = new CRXSkinOutbar;
			poControl->SetParentWnd(poParent->GetHWnd());
			tinyxml2::XMLElement* poFoleder = poElementControl->FirstChildElement("Folder");
			while (poFoleder)
			{
				CRXSkinString strFoldername = _T("");
				INT32 nFolderID = 0;
				CHAR* pszFoldername = (CHAR*)poFoleder->Attribute("name");
				if (pszFoldername)
				{
					strFoldername = _RXTUTF82T(pszFoldername).c_str();
				}
				CHAR* pszFolderID = (CHAR*)poFoleder->Attribute("id");
				if (pszFolderID)
				{
					nFolderID = atoi(pszFolderID);
				}
				if (((CRXSkinOutbar*)poControl)->AddFolder(nFolderID, strFoldername.GetData()))
				{
					tinyxml2::XMLElement* poItem = poFoleder->FirstChildElement("Item");
					while (poItem)
					{
						CRXSkinString strItemname = _T("");
						tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
						INT32 nItemID = 0;
						CHAR* pszItemname = (CHAR*)poItem->Attribute("name");
						if (pszItemname)
						{
							strItemname = _RXTUTF82T(pszItemname).c_str();
						}
						CHAR* pszItemID = (CHAR*)poItem->Attribute("id");
						if (pszItemID)
						{
							nItemID = atoi(pszItemID);
						}
						CHAR* pszItemSrc = (CHAR*)poItem->Attribute("src");
						if (pszItemSrc)
						{
							switch (enFromResource)
							{
							case EResourceType::RESOURCE_FROM_FILE:
							{
								strPath += _RXTUTF82T(pszItemSrc).c_str();
							}
							break;
							case EResourceType::RESOURCE_FROM_EXE:
							{
								strPath = CRXSkinResourceMgr::Instance()->GetResource(pszItemSrc).c_str();
							}
							break;
							default:
								break;
							}
						}
						((CRXSkinOutbar*)poControl)->AddItem(nFolderID, nItemID, strItemname.GetData(), strPath.c_str());
						poItem = poItem->NextSiblingElement("Item");
					}
					CHAR* pszIsSelected = (CHAR*)poFoleder->Attribute("isselected");
					if (pszIsSelected && strcmp("TRUE", pszIsSelected) == 0)
					{
						((CRXSkinOutbar*)poControl)->SelectFolder(nFolderID);
					}
				}
				poFoleder = poFoleder->NextSiblingElement("Folder");
			}
		}
		break;
		case CONTROL_CHECKBOX_BUTTON:
		{
			poControl = new CRXCheckButton;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_RICHEDIT:
		{
			poControl = new CRXSkinRichEdit;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_PROGRESS:
		{
			poControl = new CRXSkinProgress;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		//  WebBrowser
		case CONTROL_WEB_BROWSER:
		{
			poControl = new CRXSkinWebBrowser(poParent->GetHWnd());
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_BROWNSER:
		{
			poControl = new CRXSkinBrowser;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_SEARCH_EDIT:
		{
			poControl = new CRXSkinSearch;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_STATUS_BUTTON:
		{
			poControl = new CRXStatusButton;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_TXT_BUTTON:
		{
			poControl = new CRXTxtButton;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_MENUBAR:
		{
			poControl = new CRXSkinMenubar;
			CRXSkinMenubar* poBar = (CRXSkinMenubar*)poControl;
			poControl->SetParentWnd(poParent->GetHWnd());
			char* pszAttri = (char*)poElementControl->Attribute("BkFrom");
			if (pszAttri)
			{
				poBar->SetBkFrom(GetColor(pszAttri));
			}
			pszAttri = (char*)poElementControl->Attribute("BkMiddle");
			if (pszAttri)
			{
				poBar->SetBkMiddle(GetColor(pszAttri));
			}
			pszAttri = (char*)poElementControl->Attribute("BkTo");
			if (pszAttri)
			{
				poBar->SetBkTo(GetColor(pszAttri));
			}

			pszAttri = (char*)poElementControl->Attribute("ItemNormalTxtColor");
			if (pszAttri)
			{
				poBar->SetItemNormalTextColor(GetColor(pszAttri));
			}
			pszAttri = (char*)poElementControl->Attribute("ItemMouseOnTxtColor");
			if (pszAttri)
			{
				poBar->SetItemMouseOnTextColor(GetColor(pszAttri));
			}

			pszAttri = (char*)poElementControl->Attribute("ItemPressedTxtColor");
			if (pszAttri)
			{
				poBar->SetItemPressedTextColor(GetColor(pszAttri));
			}
			pszAttri = (char*)poElementControl->Attribute("ItemPressedBkColor");
			if (pszAttri)
			{
				poBar->SetItemPressedBkColor(GetColor(pszAttri));
			}

			pszAttri = (char*)poElementControl->Attribute("ItemMouseOnBkColor");
			if (pszAttri)
			{
				poBar->SetItemMouseOnBkColor(GetColor(pszAttri));
			}	
			pszAttri = (char*)poElementControl->Attribute("area");
			if (pszAttri)
			{
				CRXSkinRect rc = RXSkinGetRect(pszAttri);
				rc.right = poParent->GetRect().right - CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderSize().cx;
				poBar->SetRect(rc);
			}

			tinyxml2::XMLElement* poChildElement = poElementControl->FirstChildElement("Item");
			while (poChildElement)
			{
				tstring strName;
				UINT32 dwID = 0;
				CRXSkinRect rcItem;
				char* pszName = (char*)poChildElement->Attribute("name");
				if (pszName)
				{
					strName = _RXTUTF82T(pszName);
				}
				char* pszID = (char*)poChildElement->Attribute("id");
				if (pszID)
				{
					dwID = atoi(pszID);
				}
				char* pszArea = (char*)poChildElement->Attribute("area");
				if (pszArea)
				{
					rcItem = RXSkinGetRect(pszArea);
				}
				CRXSkinMenubarItem* poBarItem = poBar->AddSubMenu(dwID, strName.c_str(), rcItem);
				if (poBarItem)
				{
					tinyxml2::XMLElement* poMenubarItem = poChildElement->FirstChildElement("MenuItem");
					while (poMenubarItem)
					{
						UINT32 dwID = 0;
						EMenuItemType enType = MENU_STRING;
						tstring strName;
						tstring strtImageFile;
						char* pszValue = (char*)poMenubarItem->Attribute("id");
						if (pszValue)
						{
							dwID = atoi(pszValue);
						}
						pszValue = (char*)poMenubarItem->Attribute("type");
						if (pszValue)
						{
							enType = (EMenuItemType)atoi(pszValue);
						}
						pszValue = (char*)poMenubarItem->Attribute("name");
						if (pszValue)
						{
							strName = _RXTUTF82T(pszValue);
						}
						pszValue = (char*)poMenubarItem->Attribute("img");
						if (pszValue)
						{				
							switch (enFromResource)
							{
							case EResourceType::RESOURCE_FROM_FILE:
							{
								strtImageFile = CRXSkinService::Instance()->GetResourceRootPath();
								strtImageFile += _RXTUTF82T(pszValue);
							}
							break;
							case EResourceType::RESOURCE_FROM_EXE:
							{
								strtImageFile = CRXSkinResourceMgr::Instance()->GetResource(pszValue);
							}
							break;
							default:
								break;
							}
						}
						poBarItem->AppendMenu(enType, dwID, strName.c_str(), strtImageFile.c_str());
						poMenubarItem = poMenubarItem->NextSiblingElement("MenuItem");
					}
				}
				poChildElement = poChildElement->NextSiblingElement("Item");
			}
		}
		break;
		case CONTROL_SLIDER:
		{
			poControl = new CRXSkinSlider;
			poControl->SetParentWnd(poParent->GetHWnd());
			char* pszSliderType = (char*)poElementControl->Attribute("Slider");
			if (pszSliderType)
			{
				((CRXSkinSlider*)poControl)->SetType((ESliderType)atoi(pszSliderType));
			}
		}
		break;

		case CONTROL_TREE:
		{
			poControl = new CRXSkinTree;
			poControl->SetParentWnd(poParent->GetHWnd());
			CHAR* pszBorderColorValue = (CHAR*)poElementControl->Attribute("bordercolor");
			if (pszBorderColorValue)
			{
				((CRXSkinTree*)poControl)->SetBorderColor(GetColor(pszBorderColorValue));
			}

			CHAR* pszTextColorValue = (CHAR*)poElementControl->Attribute("textcolor");
			if (pszTextColorValue)
			{
				((CRXSkinTree*)poControl)->SetTextColor(GetColor(pszTextColorValue));
			}

			CHAR* pszBKColorValue = (CHAR*)poElementControl->Attribute("bkcolor");
			if (pszBKColorValue)
			{
				((CRXSkinTree*)poControl)->SetBkColor(GetColor(pszBKColorValue));
			}
	//		((CRXSkinTree*)poControl)->CreateBody();
		}
		break;
		case CONTROL_COMBOBOX:
		{
			poControl = new CRXSkinComboBox;
			poControl->SetParentWnd(poParent->GetHWnd());
			CHAR* pszTipValue = (CHAR*)poElementControl->Attribute("tipvalue");
			if (pszTipValue)
			{
				((CRXSkinComboBox*)poControl)->SetTipValue(_RXTUTF82T(pszTipValue).c_str());
			}

			CHAR* pszTipTextColor = (CHAR*)poElementControl->Attribute("tiptextcolor");
			if (pszTipTextColor)
			{
				((CRXSkinComboBox*)poControl)->SetTipTextColor(GetColor(pszTipTextColor));
			}
			CHAR* pszTipBGColor = (CHAR*)poElementControl->Attribute("tipbgcolor");
			if (pszTipBGColor)
			{
				((CRXSkinComboBox*)poControl)->SetTipTextBG(GetColor(pszTipBGColor));
			}
			CHAR* pszFontSize = (CHAR*)poElementControl->Attribute("FontSize");
			if (pszFontSize)
			{
				((CRXSkinComboBox*)poControl)->SetFontSize(atoi(pszFontSize));
			}
			CHAR* pszFontName = (CHAR*)poElementControl->Attribute("FontName");
			if (pszFontName)
			{
				((CRXSkinComboBox*)poControl)->SetFontName(_RXTUTF82T(pszFontName).c_str());
			}
		}
		break;
		case CONTROL_EDIT:
		{
			poControl = new CRXSkinEdit;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_SPIN_EDIT:
		{
			poControl = new CRXSkinSpinEdit;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_GROUP_LINE:
		{
			poControl = new CRXGroupLine;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_STATIC_IMAGE:
		{
			poControl = new CRXStaticImage;
			poControl->SetParentWnd(poParent->GetHWnd());
			tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
		
			CHAR* pszSrc = (CHAR*)poElementControl->Attribute("src");
			if (pszSrc)
			{
				switch (enFromResource)
				{
				case EResourceType::RESOURCE_FROM_FILE:
				{
					strPath += _RXTUTF82T(pszSrc);
				}
				break;
				case EResourceType::RESOURCE_FROM_EXE:
				{
					strPath = CRXSkinResourceMgr::Instance()->GetResource(pszSrc);
				}
				break;
				default:
					break;
				}				
			}
			((CRXStaticImage*)poControl)->SetImage(strPath.c_str());
		}
		break;
		case CONTROL_GIF:
		{
			poControl = new CRXSkinGif;
			poControl->SetParentWnd(poParent->GetHWnd());
			tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
			CHAR* pszSrc = (CHAR*)poElementControl->Attribute("src");
			if (pszSrc)
			{				
				switch (enFromResource)
				{
				case EResourceType::RESOURCE_FROM_FILE:
				{
					strPath += _RXTUTF82T(pszSrc);
					((CRXSkinGif*)poControl)->SetImage(strPath.c_str());
				}
				break;
				case EResourceType::RESOURCE_FROM_EXE:
				{
					uint32_t dwLen = 0;
					TCHAR* pszResource = MAKEINTRESOURCE(atoi(pszSrc));
					if (pszResource)
					{
						BYTE* byData = CRXSkinResourceMgr::Instance()->GetGif(pszResource, dwLen);
						if (byData && dwLen > 0)
						{
							((CRXSkinGif*)poControl)->SetImageData(byData, dwLen);
						}
					}
				}
					
				break;
				default:
					break;
				}
			}
			
		}
		break;
		case  CONTROL_RADIOLIST:
		{
			poControl = new CRXRadioButtonList;
			poControl->SetParentWnd(poParent->GetHWnd());
			tinyxml2::XMLElement* poItem = poElementControl->FirstChildElement("Item");
			while (poItem)
			{
				UINT32 dwItemID = 0;
				CHAR* pszItemID = (CHAR*)poItem->Attribute("id");
				if (pszItemID)
				{
					dwItemID = atoi(pszItemID);
				}
				CRXSkinRect rcItem;
				CHAR* pszArea = (CHAR*)poItem->Attribute("area");
				if (pszArea)
				{
					rcItem = RXSkinGetRect(pszArea);
				}
				CRXSkinString strTxt;
				CHAR* pszText = (CHAR*)poItem->Attribute("text");
				if (pszText)
				{
					strTxt = _RXTUTF82T(pszText).c_str();
				}
				CRXSkinString strTooltip;
				CHAR* pszTooltip = (CHAR*)poItem->Attribute("tooltip");
				if (pszTooltip)
				{
					strTooltip = _RXTUTF82T(pszTooltip).c_str();
				}
				CRXSkinResizePoint ptTopLeft, ptBottomRight;

				CHAR* pszAlignTopLeft = (CHAR*)poElementControl->Attribute("aligntopleft");
				if (pszAlignTopLeft)
				{
					ptTopLeft = RXSkinGetAlignType(pszAlignTopLeft);
				}
				CHAR* pszAlignBottomRight = (CHAR*)poElementControl->Attribute("alignbottomright");
				if (pszAlignBottomRight)
				{
					ptBottomRight = RXSkinGetAlignType(pszAlignBottomRight);
				}
				CRXRadioButton* poBtn = new CRXRadioButton;
				poBtn->SetID(dwItemID);
				poBtn->SetParentWnd(poParent->GetHWnd());
				poBtn->SetText(strTxt.GetData());
				poBtn->SetTooltip(strTooltip.GetData());
				poBtn->SetRect(rcItem);
				poBtn->SetResize(ptTopLeft, ptBottomRight);
				if (FALSE == ((CRXRadioButtonList*)poControl)->AddRadioButton(poBtn))
				{
					delete poBtn;
				}
				poItem = poItem->NextSiblingElement("Item");
			}
		}
		break;
		case CONTROL_DATE_PICKER:
		{
			poControl = new CRXSkinDatePicker;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_LOCATION_PICKER:
		{
			poControl = new CRXSkinLocationPicker;
			poControl->SetParentWnd(poParent->GetHWnd());
			CHAR* pszTipValue = (CHAR*)poElementControl->Attribute("tipvalue");
			if (pszTipValue)
			{
				((CRXSkinLocationPicker*)poControl)->SetTipValue(_RXTUTF82T(pszTipValue).c_str());
			}

			CHAR* pszTipTextColor = (CHAR*)poElementControl->Attribute("tiptextcolor");
			if (pszTipTextColor)
			{
				((CRXSkinLocationPicker*)poControl)->SetTipTextColor(GetColor(pszTipTextColor));
			}
			CHAR* pszTipBGColor = (CHAR*)poElementControl->Attribute("tipbgcolor");
			if (pszTipBGColor)
			{
				((CRXSkinLocationPicker*)poControl)->SetTipTextBG(GetColor(pszTipBGColor));
			}
		}
		break;
		case CONTROL_PICTURE:
		{
			poControl = new CRXSkinPicture;
			poControl->SetParentWnd(poParent->GetHWnd());
		}
		break;
		case CONTROL_LISTCTRL:
		{
			poControl = new CRXSkinListCtrl;
			poControl->SetParentWnd(poParent->GetHWnd());
			((CRXSkinListCtrl*)poControl)->CreateBody();
			((CRXSkinListCtrl*)poControl)->CreateHeader();
			CRXSkinListCtrl* poListCtrl = (CRXSkinListCtrl*)poControl;
			CHAR* pszStyle = (CHAR*)poElementControl->Attribute("style");
			if (pszStyle && strcmp(pszStyle, "box") == 0)
			{
				((CRXSkinListCtrl*)poControl)->SetType(LIST_CTRL_BOX);
			}
			tinyxml2::XMLElement* poHeaderElement = poElementControl->FirstChildElement("Header");
			if (poHeaderElement)
			{
				tinyxml2::XMLElement* poElementColumn = poHeaderElement->FirstChildElement("Column");
				while (poElementColumn)
				{
					UINT32 dwPercent = 0;
					CHAR* pszPercent = (CHAR*)poElementColumn->Attribute("percent");
					if (pszPercent)
					{
						dwPercent = atoi(pszPercent);
					}
					CRXSkinString strText;
					CHAR* pszText = (CHAR*)poElementColumn->Attribute("text");
					if (pszText)
					{
						strText = _RXTUTF82T(pszText).c_str();
					}
					((CRXSkinListCtrl*)poControl)->AddColumn(strText.GetData(), dwPercent);
					poElementColumn = poElementColumn->NextSiblingElement("Column");
				}
			}
			tinyxml2::XMLElement* poRowsElement = poElementControl->FirstChildElement("Rows");
			if (poRowsElement)
			{
				const char* pszValue = poRowsElement->Attribute("group_num");
				if (pszValue)
				{
					poListCtrl->SetGroupNum(atoi(pszValue));
					tinyxml2::XMLElement* poRowElement = poRowsElement->FirstChildElement("row");
					while (poRowElement)
					{
						pszValue = poRowElement->Attribute("group_id");
						if (NULL == pszValue)
						{
							poRowElement = poRowElement->NextSiblingElement("row");
							continue;
						}
						uint32_t dwGroupID = atoi(pszValue);
						UINT32 dwRow = poListCtrl->AddRow(dwGroupID);
						UINT32 dwItemCount = 0;
						tinyxml2::XMLElement* poItemElement = poRowElement->FirstChildElement("item");
						while (poItemElement)
						{
							EListItemType enType = LIST_ITEM_TEXT;
							char* pszValue = (char*)poItemElement->Attribute("type");
							if (pszValue)
							{
								enType = (EListItemType)atoi(pszValue);
							}

							tstring strTxt = _T("");
							pszValue = (char*)poItemElement->Attribute("text");
							if (pszValue)
							{
								strTxt = _RXTUTF82T(pszValue);
							}
							poListCtrl->SetItemText(dwRow, dwItemCount, strTxt.c_str(), enType);
							dwItemCount++;
							poItemElement = poItemElement->NextSiblingElement("item");
						}
						poRowElement = poRowElement->NextSiblingElement("row");
					}
				}
				
			}
			poListCtrl->UpdateScrollBar();
			poListCtrl->Finished();
		}
		break;
		case CONTROL_IMAGE_BUTTON:
		{
			poControl = new CRXImageButton;
			poControl->SetParentWnd(poParent->GetHWnd());
			CHAR* pszSrc = (CHAR*)poElementControl->Attribute("normalsrc");
			if (pszSrc)
			{
				switch (enFromResource)
				{
				case EResourceType::RESOURCE_FROM_FILE:
				{
					tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
					strPath += _RXTUTF82T(pszSrc);
					((CRXImageButton*)poControl)->SetNormalImage(strPath.c_str());
				}
				break;
				case EResourceType::RESOURCE_FROM_EXE:
				{
					TCHAR* pszResource = MAKEINTRESOURCE(atoi(pszSrc));
					IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().OpenDirectImageFromResource(pszResource);
					((CRXImageButton*)poControl)->SetNormalImage(poImage);
				}
				break;
				default:
					break;
				}
				
			}
			pszSrc = (CHAR*)poElementControl->Attribute("normalbk");
			if (pszSrc)
			{			
				((CRXImageButton*)poControl)->SetNormalBK(GetColor(pszSrc));
			}
			pszSrc = (CHAR*)poElementControl->Attribute("hotsrc");
			if (pszSrc)
			{
				switch (enFromResource)
				{
				case EResourceType::RESOURCE_FROM_FILE:
				{
					tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
					strPath += _RXTUTF82T(pszSrc);
					((CRXImageButton*)poControl)->SetHotImage(strPath.c_str());
				}
				break;
				case EResourceType::RESOURCE_FROM_EXE:
				{
					TCHAR* pszResource = MAKEINTRESOURCE(atoi(pszSrc));
					IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().OpenDirectImageFromResource(pszResource);
					((CRXImageButton*)poControl)->SetHotImage(poImage);
				}
				break;
				default:
					break;
				}
			}
			pszSrc = (CHAR*)poElementControl->Attribute("downsrc");
			if (pszSrc)
			{
				switch (enFromResource)
				{
				case EResourceType::RESOURCE_FROM_FILE:
				{
					tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
					strPath += _RXTUTF82T(pszSrc);
					((CRXImageButton*)poControl)->SetDownImage(strPath.c_str());
				}
				break;
				case EResourceType::RESOURCE_FROM_EXE:
				{
					TCHAR* pszResource = MAKEINTRESOURCE(atoi(pszSrc));
					IRXImage* poImage = CRXSkinService::Instance()->GetImageMgr().OpenDirectImageFromResource(pszResource);
					((CRXImageButton*)poControl)->SetDownImage(poImage);
				}
				break;
				default:
					break;
				}
			}
		}
		break;
		case CONTROL_TAB_CTRL:
		{
			poControl = new CRXSkinTabCtrl;
			poControl->SetParentWnd(poParent->GetHWnd());
			tinyxml2::XMLElement* poTabItem = poElementControl->FirstChildElement("TabItem");
			while (poTabItem)
			{
				UINT32 dwItemID = 0;
				CHAR* pszID = (CHAR*)poTabItem->Attribute("id");
				if (pszID)
				{
					dwItemID = atoi(pszID);
				}
				tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
				CHAR* pszSrc = (CHAR*)poTabItem->Attribute("src");
				if (pszSrc)
				{					
					switch (enFromResource)
					{
					case EResourceType::RESOURCE_FROM_FILE:
					{
						strPath += _RXTUTF82T(pszSrc);
					}
					break;
					case EResourceType::RESOURCE_FROM_EXE:
					{
						strPath = CRXSkinResourceMgr::Instance()->GetResource(pszSrc);
					}
					break;
					default:
						break;
					}
				}
				tstring strName = _T("");
				CHAR* pszText = (CHAR*)poTabItem->Attribute("text");
				if (pszText)
				{
					strName += _RXTUTF82T(pszText);
				}
				tstring strToolTip = _T("");
				CHAR* pszToolTip = (CHAR*)poTabItem->Attribute("tooltip");
				if (pszToolTip)
				{
					strToolTip += _RXTUTF82T(pszToolTip);
				}
				((CRXSkinTabCtrl*)poControl)->AddTab(
					strName.c_str(),
					pszText == NULL?"": pszText,
					strToolTip.c_str(), strPath.c_str());
				CHAR* pszIsSelected = (CHAR*)poTabItem->Attribute("IsSelected");
				if (pszIsSelected && strcmp(pszIsSelected, "TRUE") == 0)
				{
					((CRXSkinTabCtrl*)poControl)->SelectTab(dwItemID);
				}
				poTabItem = poTabItem->NextSiblingElement("TabItem");
			}
		}
		break;
		case CONTROL_BUTTON:
		{
			poControl = new CRXButton;
			poControl->SetParentWnd(poParent->GetHWnd());
			CHAR* pszSrc = (CHAR*)poElementControl->Attribute("src");
			if (pszSrc)
			{
				switch (enFromResource)
				{
				case EResourceType::RESOURCE_FROM_FILE:
				{
					tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
					strPath += _RXTUTF82T(pszSrc);
					((CRXButton*)poControl)->SetImage(strPath.c_str());
				}
				break;
				case EResourceType::RESOURCE_FROM_EXE:
				{
					tstring	strPath = CRXSkinResourceMgr::Instance()->GetResource(pszSrc);
					((CRXButton*)poControl)->SetImage(strPath.c_str());
				}
				break;
				default:
					break;
				}
			}
			CHAR* pszColor = (CHAR*)poElementControl->Attribute("mouseonfrom");
			if (pszColor)
			{
				((CRXButton*)poControl)->SetMouseOnFromColor(GetColor(pszColor));
			}
			pszColor = (CHAR*)poElementControl->Attribute("mouseonmiddle");
			if (pszColor)
			{
				((CRXButton*)poControl)->SetMouseOnMiddleColor(GetColor(pszColor));
			}
			pszColor = (CHAR*)poElementControl->Attribute("mouseonto");
			if (pszColor)
			{
				((CRXButton*)poControl)->SetMouseOnToColor(GetColor(pszColor));
			}

			pszColor = (CHAR*)poElementControl->Attribute("pressedfrom");
			if (pszColor)
			{
				((CRXButton*)poControl)->SetPressedFromColor(GetColor(pszColor));
			}
			pszColor = (CHAR*)poElementControl->Attribute("pressedmiddle");
			if (pszColor)
			{
				((CRXButton*)poControl)->SetPressedMiddleColor(GetColor(pszColor));
			}
			pszColor = (CHAR*)poElementControl->Attribute("pressedto");
			if (pszColor)
			{
				((CRXButton*)poControl)->SetPressedToColor(GetColor(pszColor));
			}
			pszColor = (CHAR*)poElementControl->Attribute("bordercolor");
			if (pszColor)
			{
				((CRXButton*)poControl)->SetBorderColor(GetColor(pszColor));
			}
			pszColor = (CHAR*)poElementControl->Attribute("ischeck");
			if (pszColor && strcmp(pszColor,"true")==0)
			{
				((CRXButton*)poControl)->SetCheckBoxBtn(true);
			}
		}
		break;
		default:
			break;
		}
		if (NULL == poControl) return NULL;
		CHAR* pszText = (CHAR*)poElementControl->Attribute("text");
		if (pszText)
		{
			poControl->SetText(_RXTUTF82T(pszText).c_str());
		}

		CHAR* pszBorder = (CHAR*)poElementControl->Attribute("border");
		if (pszBorder)
		{
			if (strcmp(pszBorder,"true") == 0)
			{
				poControl->SetBorder(true);
			}
		}

		CHAR* pszIsVisible = (CHAR*)poElementControl->Attribute("isvisible");
		if (pszIsVisible && strcmp(pszIsVisible, "false") == 0)
		{
			poControl->SetVisible(FALSE);
		}
		CHAR* pszTooltip = (CHAR*)poElementControl->Attribute("tooltip");
		if (pszTooltip)
		{
			poControl->SetTooltip(_RXTUTF82T(pszTooltip).c_str());
		}
		CHAR* pszArea = (CHAR*)poElementControl->Attribute("area");
		if (pszArea)
		{
			poControl->SetRect(RXSkinGetRect(pszArea));
		}

		CRXSkinResizePoint ptTopLeft, ptBottomRight;

		CHAR* pszAlignTopLeft = (CHAR*)poElementControl->Attribute("aligntopleft");
		if (pszAlignTopLeft)
		{
			ptTopLeft = RXSkinGetAlignType(pszAlignTopLeft);
		}
		CHAR* pszAlignBottomRight = (CHAR*)poElementControl->Attribute("alignbottomright");
		if (pszAlignBottomRight)
		{
			ptBottomRight = RXSkinGetAlignType(pszAlignBottomRight);
		}
		poControl->SetResize(ptTopLeft, ptBottomRight);


		CHAR* pszId = (CHAR*)poElementControl->Attribute("id");
		if (pszId)
		{
			poControl->SetID(atoi(pszId));
		}
		if (CONTROL_COMBOBOX == enType)
		{
			CHAR* pszLeftImage = (CHAR*)poElementControl->Attribute("leftsrc");
			if (pszLeftImage)
			{
				tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
				strPath += tstring(_RXTUTF82T(pszLeftImage));
				((CRXSkinComboBox*)poControl)->SetLeftImage(strPath.c_str());
			}


			char* pszValue = (char*)poElementControl->Attribute("normalborder");
			if (pszValue)
			{
				((CRXSkinComboBox*)poControl)->SetNormalBorder(GetColor(pszValue));
			}
			pszValue = (char*)poElementControl->Attribute("hotborder");
			if (pszValue)
			{
				((CRXSkinComboBox*)poControl)->SetHotBorder(GetColor(pszValue));
			}
			pszValue = (char*)poElementControl->Attribute("textcolor");
			if (pszValue)
			{
				((CRXSkinComboBox*)poControl)->SetTextColor(GetColor(pszValue));
			}
			BOOL bIsBold = FALSE;
			CHAR* pszIsBold = (CHAR*)poElementControl->Attribute("IsBold");
			if (pszIsBold && strcmp(pszIsBold, "TRUE") == 0)
			{
				bIsBold = TRUE;
			}
			((CRXSkinComboBox*)poControl)->SetIsBold(bIsBold);

			pszIsBold = (CHAR*)poElementControl->Attribute("FontSize");
			if (pszIsBold)
			{
				((CRXSkinComboBox*)poControl)->SetFontSize(atoi(pszIsBold));
			}
			pszIsBold = (CHAR*)poElementControl->Attribute("FontName");
			if (pszIsBold)
			{
				((CRXSkinComboBox*)poControl)->SetFontName(_RXTUTF82T(pszIsBold).c_str());
			}
			CHAR* pszStyle = (CHAR*)poElementControl->Attribute("style");
			if (pszStyle)
			{
				((CRXSkinComboBox*)poControl)->Create(poControl->GetID(), poControl->GetRect(), (EComboBoxType)atoi(pszStyle));
				if (atoi(pszStyle) == COMBOBOX_DROPDOWN)
				{
					poParent->AddAnchor(((CRXSkinComboBox*)poControl)->GetEditWnd(),
						RXSkinGetAlignType(pszAlignTopLeft),
						RXSkinGetAlignType(pszAlignBottomRight));
				}
			}
			tinyxml2::XMLElement* poItem = poElementControl->FirstChildElement("Item");
			while (poItem)
			{
				CRXSkinString strTxt;
				CHAR* pszTxt = (CHAR*)poItem->Attribute("text");
				if (pszTxt)
				{
					strTxt = _RXTUTF82T(pszTxt).c_str();
				}
				UINT32 dwData = 0;
				CHAR* pszData = (CHAR*)poItem->Attribute("data");
				if (pszData)
				{
					dwData = atoi(pszData);
				}
				((CRXSkinComboBox*)poControl)->AddString(strTxt, dwData);
				poItem = poItem->NextSiblingElement("Item");
			}
			CHAR* pszIsEnable = (CHAR*)poElementControl->Attribute("isenabled");
			if (pszIsEnable && strcmp(pszIsEnable, "FALSE") == 0)
			{
				((CRXSkinComboBox*)poControl)->Enable(FALSE);
			}

			CHAR* pszFontSize = (CHAR*)poElementControl->Attribute("FontSize");
			if (pszFontSize)
			{
				((CRXSkinComboBox*)poControl)->SetFontSize(atoi(pszFontSize));
			}
		}		

		if (CONTROL_RICHEDIT == enType)
		{
			BOOL bIsPassword = FALSE;
			CHAR* pszIsPassword = (CHAR*)poElementControl->Attribute("ispassword");
			if (pszIsPassword && strcmp(pszIsPassword, "TRUE") == 0)
			{
				bIsPassword = TRUE;
			}
			CHAR* pszIsEnable = (CHAR*)poElementControl->Attribute("isenabled");
			if (pszIsEnable && strcmp(pszIsEnable, "FALSE") == 0)
			{
				((CRXSkinRichEdit*)poControl)->Enable(FALSE);
			}
			CHAR* pszisMultLine = (CHAR*)poElementControl->Attribute("isMultLine");
			if (pszisMultLine)
			{
				if (strcmp(pszisMultLine, "true") == 0)
					((CRXSkinRichEdit*)poControl)->SetIsMultLine(true);
			}
			CHAR* pszTipValue = (CHAR*)poElementControl->Attribute("tipvalue");
			if (pszTipValue)
			{
				((CRXSkinRichEdit*)poControl)->SetTipValue(_RXTUTF82T(pszTipValue).c_str());
			}
			CHAR* pszBKColor = (CHAR*)poElementControl->Attribute("bkcolor");
			if (pszBKColor)
			{
				((CRXSkinRichEdit*)poControl)->SetBkColor(GetColor(pszBKColor));
			}

			CHAR* pszTextColor = (CHAR*)poElementControl->Attribute("textcolor");
			if (pszTextColor)
			{
				((CRXSkinRichEdit*)poControl)->SetTextColor(GetColor(pszTextColor));
			}

			CHAR* pszBorderColor = (CHAR*)poElementControl->Attribute("bordercolor");
			if (pszBorderColor)
			{
				((CRXSkinRichEdit*)poControl)->SetBorderColor(GetColor(pszBorderColor));
			}
			CHAR* pszTipTextColor = (CHAR*)poElementControl->Attribute("tiptextcolor");
			if (pszTipTextColor)
			{
				((CRXSkinRichEdit*)poControl)->SetTipTextColor(GetColor(pszTipTextColor));
			}
			CHAR* pszTipBGColor = (CHAR*)poElementControl->Attribute("tipbgcolor");
			if (pszTipBGColor)
			{
				((CRXSkinRichEdit*)poControl)->SetTipTextBG(GetColor(pszTipBGColor));
			}
			CHAR* pszReadOnly = (CHAR*)poElementControl->Attribute("readonly");
			if (pszReadOnly)
			{
				if (strcmp(pszReadOnly, "true") == 0)
				{
					((CRXSkinRichEdit*)poControl)->SetReadOnly(true);
				}
				else
				{
					((CRXSkinRichEdit*)poControl)->SetReadOnly(false);
				}
			}
			((CRXSkinRichEdit*)poControl)->Create(poControl->GetID(), poControl->GetRect(), bIsPassword);
			poParent->GetControlMgr().AddControl(poControl);
			poParent->AddAnchor(((CRXSkinRichEdit*)poControl)->GetEditWnd(),
				RXSkinGetAlignType(pszAlignTopLeft),
				RXSkinGetAlignType(pszAlignBottomRight));
		}

		if (CONTROL_EDIT == enType)
		{
			BOOL bIsPassword = FALSE;
			BOOL bIsBold = FALSE;
			CHAR* pszIsPassword = (CHAR*)poElementControl->Attribute("ispassword");
			if (pszIsPassword && strcmp(pszIsPassword, "TRUE") == 0)
			{
				bIsPassword = TRUE;
			}
			CHAR* pszIsBold = (CHAR*)poElementControl->Attribute("IsBold");
			if (pszIsBold && strcmp(pszIsBold, "TRUE") == 0)
			{
				bIsBold = TRUE;
			}
			((CRXSkinEdit*)poControl)->SetIsBold(bIsBold);

			pszIsBold = (CHAR*)poElementControl->Attribute("FontSize");
			if (pszIsBold)
			{
				((CRXSkinEdit*)poControl)->SetFontSize(atoi(pszIsBold));
			}
			pszIsBold = (CHAR*)poElementControl->Attribute("FontName");
			if (pszIsBold)
			{
				((CRXSkinEdit*)poControl)->SetFontName(_RXTUTF82T(pszIsBold).c_str());
			}
			((CRXSkinEdit*)poControl)->SetIsBold(bIsBold);
			CHAR* pszIsEnable = (CHAR*)poElementControl->Attribute("isenabled");
			if (pszIsEnable && strcmp(pszIsEnable, "FALSE") == 0)
			{
				((CRXSkinEdit*)poControl)->Enable(FALSE);
			}
			CHAR* pszisMultLine = (CHAR*)poElementControl->Attribute("isMultLine");
			if (pszisMultLine)
			{
				if (strcmp(pszisMultLine, "true") == 0)
					((CRXSkinEdit*)poControl)->SetIsMultLine(true);
			}
			CHAR* pszReadOnly = (CHAR*)poElementControl->Attribute("readonly");
			if (pszReadOnly)
			{
				if (strcmp(pszReadOnly, "true") == 0)
					((CRXSkinEdit*)poControl)->SetReadOnly(true);
			}
			CHAR* pszTipValue = (CHAR*)poElementControl->Attribute("tipvalue");
			if (pszTipValue)
			{
				((CRXSkinEdit*)poControl)->SetTipValue(_RXTUTF82T(pszTipValue).c_str());
			}
			CHAR* pszBKColor = (CHAR*)poElementControl->Attribute("bkcolor");
			if (pszBKColor)
			{
				((CRXSkinEdit*)poControl)->SetBkColor(GetColor(pszBKColor));
			}

			CHAR* pszTextColor = (CHAR*)poElementControl->Attribute("textcolor");
			if (pszTextColor)
			{
				((CRXSkinEdit*)poControl)->SetTextColor(GetColor(pszTextColor));
			}

			CHAR* pszBorderColor = (CHAR*)poElementControl->Attribute("bordercolor");
			if (pszBorderColor)
			{
				((CRXSkinEdit*)poControl)->SetBorderColor(GetColor(pszBorderColor));
			}
			CHAR* pszTipTextColor = (CHAR*)poElementControl->Attribute("tiptextcolor");
			if (pszTipTextColor)
			{
				((CRXSkinEdit*)poControl)->SetTipTextColor(GetColor(pszTipTextColor));
			}
			CHAR* pszTipBGColor = (CHAR*)poElementControl->Attribute("tipbgcolor");
			if (pszTipBGColor)
			{
				((CRXSkinEdit*)poControl)->SetTipTextBG(GetColor(pszTipBGColor));
			}

			((CRXSkinEdit*)poControl)->Create(poControl->GetID(), poControl->GetRect(), bIsPassword);
			poParent->AddAnchor(((CRXSkinEdit*)poControl)->GetEditWnd(),
				RXSkinGetAlignType(pszAlignTopLeft),
				RXSkinGetAlignType(pszAlignBottomRight));
		}

		if (CONTROL_SEARCH_EDIT == enType)
		{
			BOOL bIsPassword = FALSE;
			CHAR* pszIsPassword = (CHAR*)poElementControl->Attribute("ispassword");
			if (pszIsPassword && strcmp(pszIsPassword, "TRUE") == 0)
			{
				bIsPassword = TRUE;
			}
			CHAR* pszIsEnable = (CHAR*)poElementControl->Attribute("isenabled");
			if (pszIsEnable && strcmp(pszIsEnable, "FALSE") == 0)
			{
				((CRXSkinSearch*)poControl)->Enable(FALSE);
			}
			CHAR* pszisMultLine = (CHAR*)poElementControl->Attribute("isMultLine");
			if (pszisMultLine)
			{
				if (strcmp(pszisMultLine, "true") == 0)
					((CRXSkinSearch*)poControl)->SetIsMultLine(true);
			}
			CHAR* pszReadOnly = (CHAR*)poElementControl->Attribute("readonly");
			if (pszReadOnly)
			{
				if (strcmp(pszReadOnly, "true") == 0)
					((CRXSkinSearch*)poControl)->SetReadOnly(true);
			}
			CHAR* pszTipValue = (CHAR*)poElementControl->Attribute("tipvalue");
			if (pszTipValue)
			{
				((CRXSkinSearch*)poControl)->SetTipValue(_RXTUTF82T(pszTipValue).c_str());
			}
			CHAR* pszBKColor = (CHAR*)poElementControl->Attribute("bkcolor");
			if (pszBKColor)
			{
				((CRXSkinSearch*)poControl)->SetBkColor(GetColor(pszBKColor));
			}

			CHAR* pszTextColor = (CHAR*)poElementControl->Attribute("textcolor");
			if (pszTextColor)
			{
				((CRXSkinSearch*)poControl)->SetTextColor(GetColor(pszTextColor));
			}

			CHAR* pszBorderColor = (CHAR*)poElementControl->Attribute("bordercolor");
			if (pszBorderColor)
			{
				((CRXSkinSearch*)poControl)->SetBorderColor(GetColor(pszBorderColor));
			}
			CHAR* pszTipTextColor = (CHAR*)poElementControl->Attribute("tiptextcolor");
			if (pszTipTextColor)
			{
				((CRXSkinSearch*)poControl)->SetTipTextColor(GetColor(pszTipTextColor));
			}
			CHAR* pszTipBGColor = (CHAR*)poElementControl->Attribute("tipbgcolor");
			if (pszTipBGColor)
			{
				((CRXSkinSearch*)poControl)->SetTipTextBG(GetColor(pszTipBGColor));
			}

			((CRXSkinSearch*)poControl)->Create(poControl->GetID(), poControl->GetRect(), bIsPassword);
			poParent->AddAnchor(((CRXSkinSearch*)poControl)->GetEditWnd(),
				RXSkinGetAlignType(pszAlignTopLeft),
				RXSkinGetAlignType(pszAlignBottomRight));
		}

		if (CONTROL_BROWNSER == enType)
		{
			BOOL bIsPassword = FALSE;
			CHAR* pszIsPassword = (CHAR*)poElementControl->Attribute("ispassword");
			if (pszIsPassword && strcmp(pszIsPassword, "TRUE") == 0)
			{
				bIsPassword = TRUE;
			}
			CHAR* pszIsEnable = (CHAR*)poElementControl->Attribute("isenabled");
			if (pszIsEnable && strcmp(pszIsEnable, "FALSE") == 0)
			{
				((CRXSkinBrowser*)poControl)->Enable(FALSE);
			}
			CHAR* pszisMultLine = (CHAR*)poElementControl->Attribute("isMultLine");
			if (pszisMultLine)
			{
				if (strcmp(pszisMultLine, "true") == 0)
					((CRXSkinBrowser*)poControl)->SetIsMultLine(true);
			}
			CHAR* pszReadOnly = (CHAR*)poElementControl->Attribute("readonly");
			if (pszReadOnly)
			{
				if (strcmp(pszReadOnly, "true") == 0)
					((CRXSkinBrowser*)poControl)->SetReadOnly(true);
			}
			CHAR* pszTipValue = (CHAR*)poElementControl->Attribute("tipvalue");
			if (pszTipValue)
			{
				((CRXSkinBrowser*)poControl)->SetTipValue(_RXTUTF82T(pszTipValue).c_str());
			}
			CHAR* pszBKColor = (CHAR*)poElementControl->Attribute("bkcolor");
			if (pszBKColor)
			{
				((CRXSkinBrowser*)poControl)->SetBkColor(GetColor(pszBKColor));
			}

			CHAR* pszTextColor = (CHAR*)poElementControl->Attribute("textcolor");
			if (pszTextColor)
			{
				((CRXSkinBrowser*)poControl)->SetTextColor(GetColor(pszTextColor));
			}

			CHAR* pszBorderColor = (CHAR*)poElementControl->Attribute("bordercolor");
			if (pszBorderColor)
			{
				((CRXSkinBrowser*)poControl)->SetBorderColor(GetColor(pszBorderColor));
			}
			CHAR* pszTipTextColor = (CHAR*)poElementControl->Attribute("tiptextcolor");
			if (pszTipTextColor)
			{
				((CRXSkinBrowser*)poControl)->SetTipTextColor(GetColor(pszTipTextColor));
			}
			CHAR* pszTipBGColor = (CHAR*)poElementControl->Attribute("tipbgcolor");
			if (pszTipBGColor)
			{
				((CRXSkinBrowser*)poControl)->SetTipTextBG(GetColor(pszTipBGColor));
			}

			((CRXSkinBrowser*)poControl)->Create(poControl->GetID(), poControl->GetRect(), bIsPassword);
			poParent->AddAnchor(((CRXSkinBrowser*)poControl)->GetEditWnd(),
				RXSkinGetAlignType(pszAlignTopLeft),
				RXSkinGetAlignType(pszAlignBottomRight));
		}
		if (CONTROL_SPIN_EDIT == enType)
		{
			BOOL bIsPassword = FALSE;
			CHAR* pszIsPassword = (CHAR*)poElementControl->Attribute("ispassword");
			if (pszIsPassword && strcmp(pszIsPassword, "TRUE") == 0)
			{
				bIsPassword = TRUE;
			}
			CHAR* pszIsEnable = (CHAR*)poElementControl->Attribute("isenabled");
			if (pszIsEnable && strcmp(pszIsEnable, "FALSE") == 0)
			{
				((CRXSkinSpinEdit*)poControl)->Enable(FALSE);
			}
			((CRXSkinSpinEdit*)poControl)->Create();
			poParent->AddAnchor(((CRXSkinEdit*)poControl)->GetEditWnd(),
				RXSkinGetAlignType(pszAlignTopLeft),
				RXSkinGetAlignType(pszAlignBottomRight));
		}
		if (CONTROL_PROGRESS == enType)
		{
			BOOL bIsPassword = FALSE;
			CHAR* txtColor = (CHAR*)poElementControl->Attribute("textcolor");
			if (txtColor)
			{
				((CRXSkinProgress*)poControl)->SetTextColor(GetColor(txtColor));
			}
			txtColor = (CHAR*)poElementControl->Attribute("bordercolor");
			if (txtColor)
			{
				((CRXSkinProgress*)poControl)->SetBorderColor(GetColor(txtColor));
			}
			txtColor = (CHAR*)poElementControl->Attribute("bkcolor");
			if (txtColor)
			{
				((CRXSkinProgress*)poControl)->SetBKColor(GetColor(txtColor));
			}
			txtColor = (CHAR*)poElementControl->Attribute("round");
			if (txtColor)
			{
				((CRXSkinProgress*)poControl)->SetRound(GetSize(txtColor));
			}


		}
		//  浏览器
		if (enType == CONTROL_WEB_BROWSER)
		{
			((CRXSkinWebBrowser*)poControl)->CreateBrowser();
			CHAR* pszBorderColor = (CHAR*)poElementControl->Attribute("bordercolor");
			if (pszBorderColor)
			{
				((CRXSkinWebBrowser*)poControl)->SetBorderColor(GetColor(pszBorderColor));
			}
		}
		CHAR* pszIsEnable = (CHAR*)poElementControl->Attribute("isenabled");
		if (pszIsEnable && strcmp(pszIsEnable, "FALSE") == 0)
		{
			poControl->Enable(FALSE);
		}
		if (poControl)
		{
			poControlMgr->AddControl(poControl);
		}
		return poControl;
	}
	return NULL;
}

double CDrawMgr::DrawImageGray(IRXImage* poImage, RECT rcImage, HDC hDC)
{
    double  dbScale = 1.0;
    double  dbX = (double)poImage->GetWidth() / (double)(rcImage.right - rcImage.left);
    double  dbY = (double)poImage->GetHeight() / (double)(rcImage.bottom - rcImage.top);
    if (dbX > dbScale)
    {
        dbScale = dbX;
    }
    if (dbY > dbScale)
    {
        dbScale = dbY;
    }
    INT32 nWidth = (INT32)((double)poImage->GetWidth() / dbScale);
    INT32 nHeight = (INT32)((double)poImage->GetHeight() / dbScale);
    INT32 nTop = rcImage.top + ((rcImage.bottom - rcImage.top) - nHeight) / 2;
    INT32 nLeft = rcImage.left + ((rcImage.right - rcImage.left) - nWidth) / 2;
    poImage->StretchGray(hDC, nLeft, nTop, nWidth, nHeight);
    return dbScale;
}

int32_t CDrawMgr::GetShellImageListIndex(const TCHAR* pszName)
{
	int iIcon = 0;
	SHFILEINFO sfi;
	if (::SHGetFileInfo(pszName, 0, &sfi, sizeof(SHFILEINFO),
		SHGFI_ICON | SHGFI_SHELLICONSIZE | SHGFI_SMALLICON))
	{
		iIcon = sfi.iIcon;
	}

	return iIcon;
}

HIMAGELIST CDrawMgr::GetShellImageList(BOOL bLarge)
{
	TCHAR szWinDir[MAX_PATH + 1];
	if (GetWindowsDirectory(szWinDir, MAX_PATH) == 0)
	{
		return NULL;
	}

	SHFILEINFO sfi;
	HIMAGELIST hImageList = (HIMAGELIST)SHGetFileInfo(szWinDir,
		0,
		&sfi,
		sizeof(SHFILEINFO),
		SHGFI_SYSICONINDEX | (bLarge ? 0 : SHGFI_SMALLICON));
	return hImageList;
}


void CDrawMgr::DrawColor(HDC hDC, const RECT& rc, DWORD color)
{
	if (color <= 0x00FFFFFF) return;

}

HBITMAP CDrawMgr::GenerateBitmap(CRXControl* pManager, CRXControl* pControl, RECT rc, DWORD dwFilterColor)
{
	if (pManager == NULL || pControl == NULL) return NULL;
	int cx = rc.right - rc.left;
	int cy = rc.bottom - rc.top;

	HDC hPaintDC = ::CreateCompatibleDC(pManager->GetPaintDC());
	HBITMAP hPaintBitmap = ::CreateCompatibleBitmap(pManager->GetPaintDC(), rc.right, rc.bottom);
	ASSERT(hPaintDC);
	ASSERT(hPaintBitmap);
	HBITMAP hOldPaintBitmap = (HBITMAP) ::SelectObject(hPaintDC, hPaintBitmap);
	pControl->OnDraw(nullptr,hPaintDC, rc);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = cx;
	bmi.bmiHeader.biHeight = cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = cx * cy * sizeof(DWORD);
	LPDWORD pDest = NULL;
	HDC hCloneDC = ::CreateCompatibleDC(pManager->GetPaintDC());
	HBITMAP hBitmap = ::CreateDIBSection(pManager->GetPaintDC(), &bmi, DIB_RGB_COLORS, (LPVOID*)&pDest, NULL, 0);
	ASSERT(hCloneDC);
	ASSERT(hBitmap);
	if (hBitmap != NULL)
	{
		HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hCloneDC, hBitmap);
		::BitBlt(hCloneDC, 0, 0, cx, cy, hPaintDC, rc.left, rc.top, SRCCOPY);
		RECT rcClone = { 0, 0, cx, cy };
		if (dwFilterColor > 0x00FFFFFF) DrawColor(hCloneDC, rcClone, dwFilterColor);
		::SelectObject(hCloneDC, hOldBitmap);
		::DeleteDC(hCloneDC);
		::GdiFlush();
	}

	// Cleanup
	::SelectObject(hPaintDC, hOldPaintBitmap);
	::DeleteObject(hPaintBitmap);
	::DeleteDC(hPaintDC);

	return hBitmap;
}

HBITMAP CDrawMgr::CopyDCToBitmap(HDC hScrDC, LPRECT lpRect)
{
	if (hScrDC == NULL || lpRect == NULL || IsRectEmpty(lpRect))
	{
		return NULL;
	}

	HDC        hMemDC;
	// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;
	// 位图句柄
	int       nX, nY, nX2, nY2;
	// 选定区域坐标
	int       nWidth, nHeight;
	// 位图宽度和高度

	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;

	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	//为指定设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 创建一个与指定设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	StretchBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, nWidth, nHeight, SRCCOPY);
	//BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY);
	//得到屏幕位图的句柄

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//清除 

	DeleteDC(hMemDC);
	DeleteObject(hOldBitmap);
	// 返回位图句柄
	return hBitmap;
}