#include "pch.h"
#include "common/rxskinutil.h"
#include "common/RXSkinService.h"
#include "main/rxskinwaiting.h"
#include "main/rxskinmessagebox.h"
#include "main/rxskinfileselect.h"
#include "main/rxskinfolderselect.h"
#include <windows.h> 
#include <Commdlg.h>
#include <tchar.h>
#include <time.h>

#include "fundation/rxfile.h"
#include "fundation/rxdir.h"
#include "ximage.h"

#define WND_RXSKIN_MESSAGE_BOX               _T("RXSkinMessageBox")
#define WND_RXSKIN_FILE_SELECT               _T("RXSkinFileSelect")

RXSKIN_API COLORREF RXAPI GetColor(const CHAR* pszColor)
{
	COLORREF clr = RGB(0,0,0);
	INT32 nR = -1,nG = -1,nB = -1;
	string strValue = pszColor;
	INT32 nPos = (int)strValue.find("(");
	if (nPos>=0)
	{
		strValue = strValue.substr((size_t)nPos+1,strValue.size()-1-nPos);
		nPos = (int)strValue.find(",");
		string strR = strValue.substr(0,nPos);
		nR = atoi(strR.c_str());
		strValue = strValue.substr((size_t)nPos+1,strValue.size()-1-nPos);
		nPos = (int)strValue.find(",");
		string strG = strValue.substr(0,nPos);
		nG = atoi(strG.c_str());
		strValue = strValue.substr((size_t)nPos+1,strValue.size()-2-nPos);
		nB = atoi(strValue.c_str());
	}
	clr = RGB(nR,nG,nB);
	return clr;
}

RXSKIN_API SIZE RXAPI GetSize(const CHAR* pszSize)
{
	SIZE sz;
	string strValue = pszSize;
	INT32 nPos = (int)strValue.find(",");
	if (nPos>=0)
	{
		string strX = strValue.substr(0,nPos);
		sz.cx = atoi(strX.c_str());
		strValue = strValue.substr((size_t)nPos+1,strValue.size()-1-nPos);
		sz.cy = atoi(strValue.c_str());
	}
	return sz;
}

RXSKIN_API RECT RXAPI RXSkinGetRect(const CHAR* pszRect)
{
	RECT rc;
	INT32 nL = -1,nT = -1,nR = -1,nB = -1;
	string strValue = pszRect;
	INT32 nPos = (int)strValue.find("(");
	if (nPos>=0)
	{
		strValue = strValue.substr((size_t)nPos+1,strValue.size()-1-nPos);
		nPos = (int)strValue.find(",");
		string strL = strValue.substr(0,nPos);
		nL = atoi(strL.c_str());

		strValue = strValue.substr((size_t)nPos+1,strValue.size()-1-nPos);
		nPos = (int)strValue.find(",");
		string strT = strValue.substr(0,nPos);
		nT = atoi(strT.c_str());

		strValue = strValue.substr((size_t)nPos+1,strValue.size()-1-nPos);
		nPos = (int)strValue.find(",");
		string strR = strValue.substr(0,nPos);
		nR = atoi(strR.c_str());
		strValue = strValue.substr((size_t)nPos+1,strValue.size()-2-nPos);
		nB = atoi(strValue.c_str());
	}
	rc.left = nL;
	rc.top = nT;
	rc.right = nR;
	rc.bottom = nB;
	return rc;
}

RXSKIN_API INT32 RXAPI RXSkinMessageBox(HWND hParentWnd,
										const TCHAR* pszTitle,
										const TCHAR* pszText,
										EMessageBoxType enType,
										EMessageBoxTipType enTipType)
{
	CRXSkinMessageBox oWnd;	
	oWnd.SetContent(pszText);
	oWnd.SetType(enType);
	oWnd.SetTipType(enTipType);
	oWnd.Create(WND_RXSKIN_MESSAGE_BOX, pszTitle,WS_POPUP|WS_VISIBLE,hParentWnd,::LoadIcon(NULL, IDI_APPLICATION));
	CRXSkinString strTitle = pszTitle;
	if (strTitle.GetLength() > 0)
	{
		oWnd.SetTitle(pszTitle);
	}
	INT32 nResult = oWnd.ShowModal();
	return nResult;
}

RXSKIN_API TCHAR* RXAPI RXSkinFileSelect(HWND hParentWnd,
	const TCHAR* pszTitle ,
	const TCHAR* pszText,
	const TCHAR* pszDefaultPath,
	const EFileSelectType enType)
{
	CRXSkinFileSelect oWnd;
	oWnd.SetType(enType);
	oWnd.SetDefaultPath(pszDefaultPath);
	oWnd.Create(WND_RXSKIN_FILE_SELECT, pszTitle, WS_POPUP | WS_VISIBLE, hParentWnd, ::LoadIcon(NULL, IDI_APPLICATION));
	CRXSkinString strTitle = pszTitle;
	if (strTitle.GetLength() > 0)
	{
		oWnd.SetTitle(pszTitle);
	}
	static TCHAR szPath[MAX_PATH] = {0};
	memset(szPath, 0, sizeof(TCHAR)*MAX_PATH);
	INT32 nResult = oWnd.ShowModal();
	_RXTStrcpy(szPath, oWnd.GetResult().c_str());
	return szPath;
}

RXSKIN_API tstring RXAPI RXSkinFolderSelect(HWND hParentWnd,
	const TCHAR* pszTitle,
	const TCHAR* pszText,
	const TCHAR* pszDefaultPath,
	const EFileSelectType enType)
{

	CRXSkinFolderSelect oWnd;
	oWnd.Create(WND_RXSKIN_FILE_SELECT, pszTitle, WS_POPUP | WS_VISIBLE, hParentWnd, ::LoadIcon(NULL, IDI_APPLICATION));
	CRXSkinString strTitle = pszTitle;
	if (strTitle.GetLength() > 0)
	{
		oWnd.SetTitle(pszTitle);
	}

	INT32 nResult = oWnd.ShowModal();
	if (nResult == IDOK)
	{
		return oWnd.GetCurPath().c_str();
	}
	return pszDefaultPath;
}


HWND  g_hWaitingWnd = NULL;

RXSKIN_API INT32 RXAPI RXSkinWaiting(HWND hParentWnd,
	const TCHAR* pszTitle,
	const TCHAR* pszText)
{
	CRXSkinMessageBox oWnd;
	oWnd.SetContent(pszText);
	oWnd.Create(WND_RXSKIN_MESSAGE_BOX, pszTitle, WS_POPUP | WS_VISIBLE, hParentWnd, ::LoadIcon(NULL, IDI_APPLICATION));
	CRXSkinString strTitle = pszTitle;
	if (strTitle.GetLength() > 0)
	{
		oWnd.SetTitle(pszTitle);
	}
	g_hWaitingWnd = oWnd.GetHWnd();
	INT32 nResult = oWnd.ShowModal();
	return nResult;
}

RXSKIN_API INT32 RXAPI CloseWaiting()
{
	if (g_hWaitingWnd)
	{
		::SendMessage(g_hWaitingWnd, WM_COMMAND, IDOK, NULL);
		g_hWaitingWnd = NULL;
	}
	return 0;
}

RXSKIN_API BOOL RXAPI RegisterWindowClass(const TCHAR* pszClaasName,
										  WNDPROC lpfnWndProc,
										  HICON hIcon)
{
	WNDCLASS  wc;
	if (GetClassInfo(CRXSkinService::Instance()->GetInstance(),pszClaasName,&wc))
	{
		return TRUE;
	}
	
	wc.style         = CS_DBLCLKS;
	wc.lpfnWndProc   = (WNDPROC) lpfnWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = CRXSkinService::Instance()->GetInstance();
	wc.hIcon         = hIcon;
	wc.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = pszClaasName;
	if (RegisterClass(&wc) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

RXSKIN_API TCHAR* RXAPI SelectFile(const TCHAR* pszFilters)
{
	static TCHAR  szPath[MAX_PATH] = {0};
	memset(szPath, 0, sizeof(TCHAR)*MAX_PATH);

	OPENFILENAME open;
	ZeroMemory(&open, sizeof(OPENFILENAME));
	open.lStructSize = sizeof(OPENFILENAME);
	open.lpstrFile = szPath;
	open.lpstrFile[0] = _T('\0');
	open.nMaxFile = MAX_PATH;
	open.lpstrFilter =(LPCTSTR)pszFilters;
	open.nFilterIndex = 1;
	open.lpstrFileTitle = NULL;
	open.nMaxFileTitle = 0;
	open.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileName(&open)) {
		_RXTStrcpy(szPath, open.lpstrFile);		
		return szPath;
	}
	else {
		return szPath;
	}
}


RXSKIN_API TCHAR* RXAPI SelectSaveFile(const TCHAR* pszFilters)
{
	static TCHAR  szPath[MAX_PATH] = { 0 };
	memset(szPath, 0, sizeof(TCHAR) * MAX_PATH);

	OPENFILENAME open;
	ZeroMemory(&open, sizeof(OPENFILENAME));
	open.lStructSize = sizeof(OPENFILENAME);
	open.lpstrFile = szPath;
	open.lpstrFile[0] = _T('\0');
	open.nMaxFile = MAX_PATH;
	open.lpstrFilter = (LPCTSTR)pszFilters;
	open.nFilterIndex = 1;
	open.lpstrFileTitle = NULL;
	open.nMaxFileTitle = 0;
	open.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetSaveFileName(&open)) {
		_RXTStrcpy(szPath, open.lpstrFile);
		return szPath;
	}
	else {
		return szPath;
	}
}


typedef       struct       tagTABLE_YUV2RGB  
{  
	unsigned       short       YtoR[256];  
	unsigned       short       YtoG[256];  
	unsigned       short       YtoB[256];    
	unsigned       short       UtoG[256];  
	unsigned       short       UtoB[256];  
	unsigned       short       VtoR[256];  
	unsigned       short       VtoG[256];    
}TABLE_YUV2RGB;  

TABLE_YUV2RGB       table_yuv2rgb;  

RXSKIN_API void RXAPI RXSkinInitTable()
{
	for (unsigned short  j  = 0; j < 256; ++j)  
	{    
		table_yuv2rgb.YtoR[j]       =       table_yuv2rgb.YtoG[j]  
		=  table_yuv2rgb.YtoB[j]  = (unsigned       short)(j << 7);  
		table_yuv2rgb.VtoR[j]       =       j       *       180;  
		table_yuv2rgb.VtoG[j]       =       j       *       91;  
		table_yuv2rgb.UtoG[j]       =       j       *       44;  
		table_yuv2rgb.UtoB[j]       =       j       *       226;  
	}  
}

RXSKIN_API void RXAPI RXSkinConvertYUY2toRGB24(BYTE* pDst, BYTE* pSrc, int w, int h)  
{  

	long       m       =       0;  
	long       k       =       0;  
	int   n=w/2;  
	int   dec=w*4;  

	int       tmpR0       =       0;  
	int       tmpG0       =       0;  
	int       tmpB0       =       0;  
	int       tmpR1       =       0;  
	int       tmpG1       =       0;  
	int       tmpB1       =       0;  
	k=(h-1)*w<<1;  
	for(   int   i=h-1;i>-1;i--)  
	{  

		for(int   j=0;j<n;j++)  
		{  
			tmpR0       =       (table_yuv2rgb.YtoR[pSrc[k       +       0]]       +       table_yuv2rgb.VtoR[pSrc[k       +       3]]       -       22906)       >>       7;    
			tmpG0       =       (table_yuv2rgb.YtoG[pSrc[k       +       0]]       -       table_yuv2rgb.VtoG[pSrc[k       +       3]]       -       table_yuv2rgb.UtoG[pSrc[k       +       1]]       +       17264)       >>       7;        
			tmpB0       =       (table_yuv2rgb.YtoB[pSrc[k       +       0]]       +       table_yuv2rgb.UtoB[pSrc[k       +       1]]       -       28928)       >>       7;  

			tmpR1       =       (table_yuv2rgb.YtoR[pSrc[k       +       2]]       +       table_yuv2rgb.VtoR[pSrc[k       +       3]]       -       22906)       >>       7;    
			tmpG1       =       (table_yuv2rgb.YtoG[pSrc[k       +       2]]       -       table_yuv2rgb.VtoG[pSrc[k       +       3]]       -       table_yuv2rgb.UtoG[pSrc[k       +       1]]       +       17264)       >>       7;    
			tmpB1       =       (table_yuv2rgb.YtoB[pSrc[k       +       2]]       +       table_yuv2rgb.UtoB[pSrc[k       +       1]]       -       28928)       >>       7;    

			if       (tmpR0       >       255)       tmpR0       =       255;  
			if       (tmpG0       >       255)       tmpG0       =       255;  
			if       (tmpB0       >       255)       tmpB0       =       255;  
			if       (tmpR1       >       255)       tmpR1       =       255;  
			if       (tmpG1       >       255)       tmpG1       =       255;  
			if       (tmpB1       >       255)       tmpB1       =       255;  

			if       (tmpR0       <       0)       tmpR0       =       0;  
			if       (tmpG0       <       0)       tmpG0       =       0;  
			if       (tmpB0       <       0)       tmpB0       =       0;  
			if       (tmpR1       <       0)       tmpR1       =       0;  
			if       (tmpG1       <       0)       tmpG1       =       0;  
			if       (tmpB1       <       0)       tmpB1       =       0;  

			pDst[m       +       0]       =     tmpB0;  
			pDst[m       +       1]       =       tmpG0;  
			pDst[m       +       2]       =       tmpR0;  
			pDst[m       +       3]       =       tmpB1;  
			pDst[m       +       4]       =       tmpG1;  
			pDst[m       +       5]       =       tmpR1;  

			k       +=       4;  
			m       +=       6;  
		}  
		k=k-dec;  
	}
}

RXSKIN_API void RXAPI RXSkinSaveRgb2Bmp(char* rgbbuf, unsigned int width, unsigned int height,const TCHAR* pszImagePath)
{
	BITMAPINFO bitmapinfo;
	ZeroMemory(&bitmapinfo,sizeof(BITMAPINFO));
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biWidth = width;
	bitmapinfo.bmiHeader.biHeight = height;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount =24;
	bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biSizeImage = width*height;
	bitmapinfo.bmiHeader.biClrUsed = 0;        
	bitmapinfo.bmiHeader.biClrImportant = 0;

	BITMAPFILEHEADER bmpHeader;
	ZeroMemory(&bmpHeader,sizeof(BITMAPFILEHEADER));
	bmpHeader.bfType = 0x4D42;
	bmpHeader.bfOffBits = sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
	bmpHeader.bfSize = bmpHeader.bfOffBits + width*height*3;

	FILE* fp = _tfopen(pszImagePath, _T("wb"));
	if (fp)
	{
		fwrite(&bmpHeader, 1, sizeof(BITMAPFILEHEADER), fp);
		fwrite(&(bitmapinfo.bmiHeader), 1, sizeof(BITMAPINFOHEADER), fp);
		fwrite(rgbbuf, 1, width*height*3, fp);
		fclose(fp);
	}
}


CRXSkinRect::CRXSkinRect()
{
	memset(m_szString, 0, 64);
}

CRXSkinRect::CRXSkinRect(const CRXSkinRect& oRect)
{
	left = oRect.left;
	right = oRect.right;
	top = oRect.top;
	bottom = oRect.bottom;
	memset(m_szString, 0, 64);
}

CRXSkinRect::CRXSkinRect(const RECT& oRect)
{
	left = oRect.left;
	right = oRect.right;
	top = oRect.top;
	bottom = oRect.bottom;
	memset(m_szString, 0, 64);
}

CRXSkinRect::CRXSkinRect(const LONG dwLeft,const LONG dwTop,const LONG dwRight,const LONG dwBottom)
{
	left = dwLeft;
	right = dwRight;
	top = dwTop;
	bottom = dwBottom;
	memset(m_szString, 0, 64);
}

VOID CRXSkinRect::OffsetRect(POINT pt)
{
	left -= pt.x;
	right -= pt.x;
	top -= pt.y;
	bottom -= pt.y;
}

CRXSkinRect::~CRXSkinRect()
{

}

void CRXSkinRect::ResetOffset()
{
	::OffsetRect(this, -left, -top);
}

INT32 CRXSkinRect::Height()
{
	return bottom - top;
}

INT32 CRXSkinRect::Width()
{
	return right - left;
}

VOID CRXSkinRect::SetRectEmpty()
{
	left = right = top = bottom = 0;
}

CRXSkinRect& CRXSkinRect::operator =(RECT &temp)
{
	left = temp.left;
	right = temp.right;
	top = temp.top;
	bottom = temp.bottom;
	return *this;
}

POINT CRXSkinRect::TopLeft()
{
	POINT pt;
	pt.x = left;
	pt.y = top;
	return pt;
}
POINT CRXSkinRect::Center()
{
	POINT pt;
	pt.x = left + (right-left)/2;
	pt.y = top+ (bottom-top)/2;
	return pt;
}
BOOL CRXSkinRect::PtInRect(POINT pt)
{
	if (pt.x>=left&&pt.x<=right
		&& pt.y>=top && pt.y <= bottom)
	{
		return TRUE;
	}
	return FALSE;
}

void CRXSkinRect::DeflateRect(int nLeft, int nTop, int nRight, int nBottom)
{
	left -= nLeft;
	top -= nTop;
	right -= nRight;
	bottom -= nBottom;
}

char* CRXSkinRect::ToString()
{
	static  char szData[128] = {0};
	memset(szData, 0, 128);
	sprintf(szData, "RECT(%d,%d,%d,%d)",this->left, this->top, this->right, this->bottom);
	return szData;
}

/////   CRXSkinPoint
CRXSkinPoint::CRXSkinPoint()
{
	x = y = 0;
}

CRXSkinPoint::CRXSkinPoint(const POINT& src)
{
	x = src.x;
	y = src.y;
}

CRXSkinPoint::CRXSkinPoint(int _x, int _y)
{
	x = _x;
	y = _y;
}

CRXSkinPoint::CRXSkinPoint(LPARAM lParam)
{
	x = GET_X_LPARAM(lParam);
	y = GET_Y_LPARAM(lParam);
}

/////   CRXSkinPoint  结束


/////////////////////CRXSkinSize  ////////////////////////////////////////////////////////////////
//
//

CRXSkinSize::CRXSkinSize()
{
	cx = cy = 0;
}

CRXSkinSize::CRXSkinSize(const SIZE& src)
{
	cx = src.cx;
	cy = src.cy;
}

CRXSkinSize::CRXSkinSize(const RECT rc)
{
	cx = rc.right - rc.left;
	cy = rc.bottom - rc.top;
}

CRXSkinSize::CRXSkinSize(int _cx, int _cy)
{
	cx = _cx;
	cy = _cy;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CRXSkinStdPtrArray::CRXSkinStdPtrArray(int iPreallocSize) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(iPreallocSize)
{
	if( iPreallocSize > 0 ) m_ppVoid = static_cast<LPVOID*>(malloc(iPreallocSize * sizeof(LPVOID)));
}

CRXSkinStdPtrArray::CRXSkinStdPtrArray(const CRXSkinStdPtrArray& src) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(0)
{
	for(int i=0; i<src.GetSize(); i++)
		Add(src.GetAt(i));
}

CRXSkinStdPtrArray::~CRXSkinStdPtrArray()
{
	if( m_ppVoid != NULL ) free(m_ppVoid);
}

void CRXSkinStdPtrArray::Empty()
{
	if( m_ppVoid != NULL ) free(m_ppVoid);
	m_ppVoid = NULL;
	m_nCount = m_nAllocated = 0;
}

void CRXSkinStdPtrArray::Resize(int iSize)
{
	Empty();
	m_ppVoid = static_cast<LPVOID*>(malloc(iSize * sizeof(LPVOID)));
	::ZeroMemory(m_ppVoid, iSize * sizeof(LPVOID));
	m_nAllocated = iSize;
	m_nCount = iSize;
}

bool CRXSkinStdPtrArray::IsEmpty() const
{
	return m_nCount == 0;
}

bool CRXSkinStdPtrArray::Add(LPVOID pData)
{
	if( ++m_nCount >= m_nAllocated) {
		m_nAllocated *= 2;
		if( m_nAllocated == 0 ) m_nAllocated = 11;
		m_ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, m_nAllocated * sizeof(LPVOID)));
		if( m_ppVoid == NULL ) return false;
	}
	m_ppVoid[m_nCount - 1] = pData;
	return true;
}

bool CRXSkinStdPtrArray::InsertAt(int iIndex, LPVOID pData)
{
	if( iIndex == m_nCount ) return Add(pData);
	if( iIndex < 0 || iIndex > m_nCount ) return false;
	if( ++m_nCount >= m_nAllocated) {
		m_nAllocated *= 2;
		if( m_nAllocated == 0 ) m_nAllocated = 11;
		m_ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, m_nAllocated * sizeof(LPVOID)));
		if( m_ppVoid == NULL ) return false;
	}
	memmove(&m_ppVoid[iIndex + 1], &m_ppVoid[iIndex], (m_nCount - iIndex - 1) * sizeof(LPVOID));
	m_ppVoid[iIndex] = pData;
	return true;
}

bool CRXSkinStdPtrArray::SetAt(int iIndex, LPVOID pData)
{
	if( iIndex < 0 || iIndex >= m_nCount ) return false;
	m_ppVoid[iIndex] = pData;
	return true;
}

bool CRXSkinStdPtrArray::Remove(int iIndex)
{
	if( iIndex < 0 || iIndex >= m_nCount ) return false;
	if( iIndex < --m_nCount ) ::CopyMemory(m_ppVoid + iIndex, m_ppVoid + iIndex + 1, (m_nCount - iIndex) * sizeof(LPVOID));
	return true;
}

int CRXSkinStdPtrArray::Find(LPVOID pData) const
{
	for( int i = 0; i < m_nCount; i++ ) if( m_ppVoid[i] == pData ) return i;
	return -1;
}

int CRXSkinStdPtrArray::GetSize() const
{
	return m_nCount;
}

LPVOID* CRXSkinStdPtrArray::GetData()
{
	return m_ppVoid;
}

LPVOID CRXSkinStdPtrArray::GetAt(int iIndex) const
{
	if( iIndex < 0 || iIndex >= m_nCount ) return NULL;
	return m_ppVoid[iIndex];
}

LPVOID CRXSkinStdPtrArray::operator[] (int iIndex) const
{
	return m_ppVoid[iIndex];
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CRXSkinString::CRXSkinString() : m_pstr(m_szBuffer)
{
	m_szBuffer[0] = '\0';
}

CRXSkinString::CRXSkinString(const TCHAR ch) : m_pstr(m_szBuffer)
{
	m_szBuffer[0] = ch;
	m_szBuffer[1] = '\0';
}

CRXSkinString::CRXSkinString(LPCTSTR lpsz, int nLen) : m_pstr(m_szBuffer)
{      
	ASSERT(!::IsBadStringPtr(lpsz,-1) || lpsz==NULL);
	m_szBuffer[0] = '\0';
	Assign(lpsz, nLen);
}

CRXSkinString::CRXSkinString(const CRXSkinString& src) : m_pstr(m_szBuffer)
{
	m_szBuffer[0] = '\0';
	Assign(src.m_pstr);
}

CRXSkinString::~CRXSkinString()
{
	if( m_pstr != m_szBuffer ) free(m_pstr);
}

int CRXSkinString::GetLength() const
{ 
	return (int) _tcslen(m_pstr); 
}

CRXSkinString::operator LPCTSTR() const 
{ 
	return m_pstr; 
}

void CRXSkinString::Append(LPCTSTR pstr)
{
	int nNewLength = GetLength() + (int) _tcslen(pstr);
	if( nNewLength >= MAX_LOCAL_STRING_LEN ) {
		if( m_pstr == m_szBuffer ) {
			m_pstr = static_cast<LPTSTR>(malloc((nNewLength + 1) * sizeof(TCHAR)));
			_tcscpy(m_pstr, m_szBuffer);
			_tcscat(m_pstr, pstr);
		}
		else {
			m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (nNewLength + 1) * sizeof(TCHAR)));
			_tcscat(m_pstr, pstr);
		}
	}
	else {
		if( m_pstr != m_szBuffer ) {
			free(m_pstr);
			m_pstr = m_szBuffer;
		}
		_tcscat(m_szBuffer, pstr);
	}
}

void CRXSkinString::Assign(LPCTSTR pstr, int cchMax)
{
	if( pstr == NULL ) pstr = _T("");
	cchMax = (cchMax < 0 ? (int) _tcslen(pstr) : cchMax);
	if( cchMax < MAX_LOCAL_STRING_LEN ) {
		if( m_pstr != m_szBuffer ) {
			free(m_pstr);
			m_pstr = m_szBuffer;
		}
	}
	else if( cchMax > GetLength() || m_pstr == m_szBuffer ) {
		if( m_pstr == m_szBuffer ) m_pstr = NULL;
		m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (cchMax + 1) * sizeof(TCHAR)));
	}
	_tcsncpy(m_pstr, pstr, cchMax);
	m_pstr[cchMax] = '\0';
}

bool CRXSkinString::IsEmpty() const 
{ 
	return m_pstr[0] == '\0'; 
}

void CRXSkinString::Empty() 
{ 
	if( m_pstr != m_szBuffer ) free(m_pstr);
	m_pstr = m_szBuffer;
	m_szBuffer[0] = '\0'; 
}

LPCTSTR CRXSkinString::GetData()
{
	return m_pstr;
}


CRXSkinString& CRXSkinString::TrimLeft()
{
	// find first non-space character

	LPTSTR psz = this->m_pstr;

	while (::_istspace(*psz))
	{
		psz = ::CharNext(psz);
	}

	if (psz != this->m_pstr)
	{
		int iFirst = int(psz - this->m_pstr);
		Assign(psz, this->GetLength() - iFirst);
	}

	return(*this);
}

CRXSkinString& CRXSkinString::TrimRight(TCHAR ch)
{
	LPTSTR psz = this->m_pstr;
	LPTSTR pszLast = NULL;

	while (*psz != 0)
	{
		if ((*psz) == ch)
		{
			if (pszLast == NULL)
				pszLast = psz;
		}
		else
		{
			pszLast = NULL;
		}
		psz = ::CharNext(psz);
	}

	if (pszLast != NULL)
	{
		// truncate at trailing space start
		int iLast = int(pszLast - this->GetData());

		this->SetAt(iLast, 0);
	}

	return(*this);
}

CRXSkinString& CRXSkinString::TrimRight()
{
	LPTSTR psz = this->m_pstr;
	LPTSTR pszLast = NULL;

	while (*psz != 0)
	{
		if (::_istspace(*psz))
		{
			if (pszLast == NULL)
				pszLast = psz;
		}
		else
		{
			pszLast = NULL;
		}
		psz = ::CharNext(psz);
	}

	if (pszLast != NULL)
	{
		// truncate at trailing space start
		int iLast = int(pszLast - this->GetData());

		this->SetAt(iLast, 0);
	}

	return(*this);
}

CRXSkinString& CRXSkinString::Trim()
{
	TrimLeft();
	TrimRight();
	return(*this);
}

TCHAR CRXSkinString::GetAt(int nIndex) const
{
	return m_pstr[nIndex];
}

TCHAR CRXSkinString::operator[] (int nIndex) const
{ 
	return m_pstr[nIndex];
}   

const CRXSkinString& CRXSkinString::operator=(const CRXSkinString& src)
{      
	Assign(src);
	return *this;
}

const CRXSkinString& CRXSkinString::operator=(LPCTSTR lpStr)
{      
	ASSERT(!::IsBadStringPtr(lpStr,-1));
	Assign(lpStr);
	return *this;
}

#ifndef _UNICODE

const CRXSkinString& CRXSkinString::operator=(LPCWSTR lpwStr)
{      
	ASSERT(!::IsBadStringPtrW(lpwStr,-1));
	int cchStr = ((int) wcslen(lpwStr) * 2) + 1;
	LPSTR pstr = (LPSTR) _alloca(cchStr);
	if( pstr != NULL ) ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
	Assign(pstr);
	return *this;
}

#endif // _UNICODE

const CRXSkinString& CRXSkinString::operator=(const TCHAR ch)
{
	Empty();
	m_szBuffer[0] = ch;
	m_szBuffer[1] = '\0';
	return *this;
}

CRXSkinString CRXSkinString::operator+(const CRXSkinString& src)
{
	CRXSkinString sTemp = *this;
	sTemp.Append(src);
	return sTemp;
}

CRXSkinString CRXSkinString::operator+(LPCTSTR lpStr)
{
	ASSERT(!::IsBadStringPtr(lpStr,-1));
	CRXSkinString sTemp = *this;
	sTemp.Append(lpStr);
	return sTemp;
}

const CRXSkinString& CRXSkinString::operator+=(const CRXSkinString& src)
{      
	Append(src);
	return *this;
}

const CRXSkinString& CRXSkinString::operator+=(LPCTSTR lpStr)
{      
	ASSERT(!::IsBadStringPtr(lpStr,-1));
	Append(lpStr);
	return *this;
}

const CRXSkinString& CRXSkinString::operator+=(const TCHAR ch)
{      
	TCHAR str[] = { ch, '\0' };
	Append(str);
	return *this;
}

bool CRXSkinString::operator == (LPCTSTR str) const { return (Compare(str) == 0); };
bool CRXSkinString::operator != (LPCTSTR str) const { return (Compare(str) != 0); };
bool CRXSkinString::operator <= (LPCTSTR str) const { return (Compare(str) <= 0); };
bool CRXSkinString::operator <  (LPCTSTR str) const { return (Compare(str) <  0); };
bool CRXSkinString::operator >= (LPCTSTR str) const { return (Compare(str) >= 0); };
bool CRXSkinString::operator >  (LPCTSTR str) const { return (Compare(str) >  0); };

void CRXSkinString::SetAt(int nIndex, TCHAR ch)
{
	ASSERT(nIndex>=0 && nIndex<GetLength());
	m_pstr[nIndex] = ch;
}

int CRXSkinString::Compare(LPCTSTR lpsz) const 
{ 
	return _tcscmp(m_pstr, lpsz); 
}

int CRXSkinString::CompareNoCase(LPCTSTR lpsz) const 
{ 
	return _tcsicmp(m_pstr, lpsz); 
}

void  CRXSkinString::MakeUpper() 
{ 
	_tcsupr(m_pstr); 	
}

void  CRXSkinString::MakeLower()
{ 
	_tcslwr(m_pstr); 	
}

CRXSkinString CRXSkinString::Left(int iLength) const
{
	if( iLength < 0 ) iLength = 0;
	if( iLength > GetLength() ) iLength = GetLength();
	return CRXSkinString(m_pstr, iLength);
}

CRXSkinString CRXSkinString::Mid(int iPos, int iLength) const
{
	if( iLength < 0 ) iLength = GetLength() - iPos;
	if( iPos + iLength > GetLength() ) iLength = GetLength() - iPos;
	if( iLength <= 0 ) return CRXSkinString();
	return CRXSkinString(m_pstr + iPos, iLength);
}

CRXSkinString CRXSkinString::Right(int iLength) const
{
	int iPos = GetLength() - iLength;
	if( iPos < 0 ) {
		iPos = 0;
		iLength = GetLength();
	}
	return CRXSkinString(m_pstr + iPos, iLength);
}

int CRXSkinString::Find(TCHAR ch, int iPos /*= 0*/) const
{
	ASSERT(iPos>=0 && iPos<=GetLength());
	if( iPos != 0 && (iPos < 0 || iPos >= GetLength()) ) return -1;
	LPCTSTR p = _tcschr(m_pstr + iPos, ch);
	if( p == NULL ) return -1;
	return (int)(p - m_pstr);
}

int CRXSkinString::Find(LPCTSTR pstrSub, int iPos /*= 0*/) const
{
	ASSERT(!::IsBadStringPtr(pstrSub,-1));
	ASSERT(iPos>=0 && iPos<=GetLength());
	if( iPos != 0 && (iPos < 0 || iPos > GetLength()) ) return -1;
	LPCTSTR p = _tcsstr(m_pstr + iPos, pstrSub);
	if( p == NULL ) return -1;
	return (int)(p - m_pstr);
}

int CRXSkinString::ReverseFind(TCHAR ch) const
{
	LPCTSTR p = _tcsrchr(m_pstr, ch);
	if( p == NULL ) return -1;
	return (int)(p - m_pstr);
}

int CRXSkinString::Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo)
{
	CRXSkinString sTemp;
	int nCount = 0;
	int iPos = Find(pstrFrom);
	if( iPos < 0 ) return 0;
	int cchFrom = (int) _tcslen(pstrFrom);
	int cchTo = (int) _tcslen(pstrTo);
	while( iPos >= 0 ) {
		sTemp = Left(iPos);
		sTemp += pstrTo;
		sTemp += Mid(iPos + cchFrom);
		Assign(sTemp);
		iPos = Find(pstrFrom, iPos + cchTo);
		nCount++;
	}
	return nCount;
}

int CRXSkinString::Format(LPCTSTR pstrFormat, ...)
{
	CRXSkinString sFormat = pstrFormat;
	// Do ordinary printf replacements
	// NOTE: Documented max-length of wvsprintf() is 1024
	TCHAR szBuffer[1025] = { 0 };
	va_list argList;
	va_start(argList, pstrFormat);
	int iRet = ::wvsprintf(szBuffer, sFormat, argList);
	va_end(argList);
	Assign(szBuffer);
	return iRet;
}

int CRXSkinString::SmallFormat(LPCTSTR pstrFormat, ...)
{
	CRXSkinString sFormat = pstrFormat;
	TCHAR szBuffer[64] = { 0 };
	va_list argList;
	va_start(argList, pstrFormat);
	int iRet = ::wvsprintf(szBuffer, sFormat, argList);
	va_end(argList);
	Assign(szBuffer);
	return iRet;
}




///


static UINT HashKey(LPCTSTR Key)
{
	UINT i = 0;
	SIZE_T len = _tcslen(Key);
	while (len-- > 0) i = (i << 5) + i + Key[len];
	return i;
}

static UINT HashKey(const CRXSkinString& Key)
{
	return HashKey((LPCTSTR)Key);
};

CStdStringPtrMap::CStdStringPtrMap(int nSize) : m_nCount(0)
{
	if (nSize < 16) nSize = 16;
	m_nBuckets = nSize;
	m_aT = new TITEM * [nSize];
	memset(m_aT, 0, nSize * sizeof(TITEM*));
}

CStdStringPtrMap::~CStdStringPtrMap()
{
	if (m_aT) {
		int len = m_nBuckets;
		while (len--) {
			TITEM* pItem = m_aT[len];
			while (pItem) {
				TITEM* pKill = pItem;
				pItem = pItem->pNext;
				delete pKill;
			}
		}
		delete[] m_aT;
		m_aT = NULL;
	}
}

void CStdStringPtrMap::RemoveAll()
{
	this->Resize(m_nBuckets);
}

void CStdStringPtrMap::Resize(int nSize)
{
	if (m_aT) {
		int len = m_nBuckets;
		while (len--) {
			TITEM* pItem = m_aT[len];
			while (pItem) {
				TITEM* pKill = pItem;
				pItem = pItem->pNext;
				delete pKill;
			}
		}
		delete[] m_aT;
		m_aT = NULL;
	}

	if (nSize < 0) nSize = 0;
	if (nSize > 0) {
		m_aT = new TITEM * [nSize];
		memset(m_aT, 0, nSize * sizeof(TITEM*));
	}
	m_nBuckets = nSize;
	m_nCount = 0;
}

LPVOID CStdStringPtrMap::Find(LPCTSTR key, bool optimize) const
{
	if (m_nBuckets == 0 || GetSize() == 0) return NULL;

	UINT slot = HashKey(key) % m_nBuckets;
	for (TITEM* pItem = m_aT[slot]; pItem; pItem = pItem->pNext) {
		if (pItem->Key == key) {
			if (optimize && pItem != m_aT[slot]) {
				if (pItem->pNext) {
					pItem->pNext->pPrev = pItem->pPrev;
				}
				pItem->pPrev->pNext = pItem->pNext;
				pItem->pPrev = NULL;
				pItem->pNext = m_aT[slot];
				pItem->pNext->pPrev = pItem;
				//将item移动至链条头部
				m_aT[slot] = pItem;
			}
			return pItem->Data;
		}
	}

	return NULL;
}

bool CStdStringPtrMap::Insert(LPCTSTR key, LPVOID pData)
{
	if (m_nBuckets == 0) return false;
	if (Find(key)) return false;

	// Add first in bucket
	UINT slot = HashKey(key) % m_nBuckets;
	TITEM* pItem = new TITEM;
	pItem->Key = key;
	pItem->Data = pData;
	pItem->pPrev = NULL;
	pItem->pNext = m_aT[slot];
	if (pItem->pNext)
		pItem->pNext->pPrev = pItem;
	m_aT[slot] = pItem;
	m_nCount++;
	return true;
}

LPVOID CStdStringPtrMap::Set(LPCTSTR key, LPVOID pData)
{
	if (m_nBuckets == 0) return pData;

	if (GetSize() > 0) {
		UINT slot = HashKey(key) % m_nBuckets;
		// Modify existing item
		for (TITEM* pItem = m_aT[slot]; pItem; pItem = pItem->pNext) {
			if (pItem->Key == key) {
				LPVOID pOldData = pItem->Data;
				pItem->Data = pData;
				return pOldData;
			}
		}
	}

	Insert(key, pData);
	return NULL;
}

bool CStdStringPtrMap::Remove(LPCTSTR key)
{
	if (m_nBuckets == 0 || GetSize() == 0) return false;

	UINT slot = HashKey(key) % m_nBuckets;
	TITEM** ppItem = &m_aT[slot];
	while (*ppItem) {
		if ((*ppItem)->Key == key) {
			TITEM* pKill = *ppItem;
			*ppItem = (*ppItem)->pNext;
			if (*ppItem)
				(*ppItem)->pPrev = pKill->pPrev;
			delete pKill;
			m_nCount--;
			return true;
		}
		ppItem = &((*ppItem)->pNext);
	}

	return false;
}

int CStdStringPtrMap::GetSize() const
{
#if 0//def _DEBUG
	int nCount = 0;
	int len = m_nBuckets;
	while (len--) {
		for (const TITEM* pItem = m_aT[len]; pItem; pItem = pItem->pNext) nCount++;
	}
	ASSERT(m_nCount == nCount);
#endif
	return m_nCount;
}

LPCTSTR CStdStringPtrMap::GetAt(int iIndex) const
{
	if (m_nBuckets == 0 || GetSize() == 0) return NULL;

	int pos = 0;
	int len = m_nBuckets;
	while (len--) {
		for (TITEM* pItem = m_aT[len]; pItem; pItem = pItem->pNext) {
			if (pos++ == iIndex) {
				return pItem->Key.GetData();
			}
		}
	}

	return NULL;
}

LPCTSTR CStdStringPtrMap::operator[] (int nIndex) const
{
	return GetAt(nIndex);
}


//


CRXSkinStringArray::CRXSkinStringArray()
{
}

CRXSkinStringArray::~CRXSkinStringArray()
{
}


CRXSkinString CRXSkinStringArray::GetHead()
{
	if (m_vecValues.size()>0)
	{
		return m_vecValues[0];
	}
	return _T("");
}


RXSKIN_API void RXAPI  RXGetAllImageFiles(const TCHAR* pszPath, vector<tstring>& vecAll)
{
	vector<tstring> vecTmp;
	RXGetAllFiles((TCHAR*)pszPath, vecTmp);
	for (int i = 0; i < (int)vecTmp.size(); i++)
	{
		tstring strExt = RXFileExtractExt(vecTmp[i].c_str());
		if (CxImage::IsImage(strExt.c_str()))
		{
			vecAll.push_back(vecTmp[i]);
		}
	}
}


void RXCreateDesktopShortcut(const TCHAR* pszFileName,
	const uint32_t& dwResourceID,
	const TCHAR* pszResourceType,
	const TCHAR* pszExePath)
{
	HRESULT hr;
	IShellLink* pLink;
	IPersistFile* ppf;

	// 创建IShellLink对象
	hr = CoCreateInstance(CLSID_ShellLink,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IShellLink, (void**)&pLink);
	if (FAILED(hr)) return;

	// 获取IPersistFile接口
	hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if (FAILED(hr)) {
		pLink->Release();
		return;
	}
	tstring strPath = pszExePath;
	// 设置快捷方式的属性
	wchar_t* path;
	_get_wpgmptr(&path);

	pLink->SetPath(strPath.c_str());
	pLink->SetArguments(strPath.c_str());

	uint32_t dwLen = 0;
	TCHAR* pszResource = MAKEINTRESOURCE(dwResourceID);
	BYTE* byData = CRXSkinResourceMgr::Instance()->GetResource(pszResource,
		pszResourceType,
		dwLen);
	if (byData && dwLen)
	{
		TCHAR szAppPath[MAX_PATH] = { 0 };
		_RXTsprintf(szAppPath, _T("%s\\app.ico"), RX::RXFileExtractPath(pszExePath).c_str());
		FILE* hFile = _tfopen(szAppPath, _T("wb"));
		if (hFile)
		{
			fwrite(byData, 1, dwLen, hFile);
			fclose(hFile);
			pLink->SetIconLocation(szAppPath, 0);
		}
	}

	LPITEMIDLIST pidl;
	if (FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl)))
	{
		int32_t error = GetLastError();
		return;
	}
	TCHAR szPath[MAX_PATH] = { 0 };
	SHGetPathFromIDList(pidl, szPath);
	strPath = szPath;
	if (strPath.size() > 0)
	{
		if (strPath[strPath.size() - 1] != _T('\\'))
		{
			strPath += _T("\\");
		}
	}
	strPath += pszFileName;
	strPath += _T(".lnk");
	// 保存快捷方式到磁盘
	hr = ppf->Save(strPath.c_str(), TRUE);
	ppf->Release();
	pLink->Release();

}


void RXCreateStartMenuShortcut(const TCHAR* pszFileName,
	const uint32_t& dwResourceID,
	const TCHAR* pszResourceType,
	const TCHAR* pszExePath,
	const TCHAR* pszFolderName)
{
	HRESULT hr;
	IShellLink* pLink;
	IPersistFile* ppf;

	// 创建IShellLink对象
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
	if (FAILED(hr)) return;

	// 获取IPersistFile接口
	hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if (FAILED(hr)) {
		pLink->Release();
		return;
	}
	tstring strPath = pszExePath;
	// 设置快捷方式的属性
	wchar_t* path;
	_get_wpgmptr(&path);

	pLink->SetPath(strPath.c_str());
	pLink->SetArguments(strPath.c_str());

	uint32_t dwLen = 0;
	TCHAR* pszResource = MAKEINTRESOURCE(dwResourceID);
	BYTE* byData = CRXSkinResourceMgr::Instance()->GetResource(pszResource, pszResourceType, dwLen);
	if (byData && dwLen)
	{
		TCHAR szAppPath[MAX_PATH] = { 0 };
		_RXTsprintf(szAppPath, _T("%sapp.ico"), RX::RXFileExtractPath(pszExePath).c_str());
		FILE* hFile = _tfopen(szAppPath, _T("wb"));
		if (hFile)
		{
			fwrite(byData, 1, dwLen, hFile);
			fclose(hFile);
			pLink->SetIconLocation(szAppPath, 0);
		}
	}

	LPITEMIDLIST pidl;
	if (FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pidl)))
	{
		int32_t error = GetLastError();
		LOG_INFO("SHGetSpecialFolderLocation:%d", error);
		return;
	}
	TCHAR szPath[MAX_PATH] = { 0 };
	SHGetPathFromIDList(pidl, szPath);
	strPath = szPath;
	if (strPath.size() > 0)
	{
		if (strPath[strPath.size() - 1] != _T('\\'))
		{
			strPath += _T("\\");
		}
	}

	strPath += pszFolderName;
	RXCreateDirectory(strPath.c_str());
	strPath += _T("\\");

	strPath += pszFileName;
	strPath += _T(".lnk");
	// 保存快捷方式到磁盘
	hr = ppf->Save(strPath.c_str(), TRUE);
	ppf->Release();
	pLink->Release();

}