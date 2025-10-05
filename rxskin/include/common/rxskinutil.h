#ifndef _RXSKIN_UTIL_INCLUDE_
#define _RXSKIN_UTIL_INCLUDE_
#include "RXSkin.h"
#include <windows.h>
#include "tchar.h"
#include "OAIdl.h"
#include <vector>
#include <string.h>
#include <string>
using namespace std;

#include "fundation/rxstring.h"

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

#ifndef UINT32 
#define UINT32				unsigned int
#endif


#ifndef ASSERT
#define ASSERT(expr)		_ASSERTE(expr)
#endif

enum EMessageBoxType
{

	RXSKIN_MB_ABORTRETRYIGNORE = 0,
	RXSKIN_MB_OK,
	RXSKIN_MB_OKCANCEL,
	RXSKIN_MB_RETRYCANCEL,
	RXSKIN_MB_YESNO,
	RXSKIN_MB_YESNOCANCEL,
	RXSKIN_MB_TIP,
};

enum EMessageBoxTipType
{
	RXSKIN_MB_ERROR = 0,
	RXSKIN_MB_WARN,
	RXSKIN_MB_INFO,
	RXSKIN_MB_HELP,
};

enum EFileSelectType
{
	RXSKIN_FILE_SELECT_FILE = 0,
	RXSKIN_FILE_SELECT_DIRECTORY,
};


RXSKIN_API COLORREF RXAPI GetColor(const CHAR* pszColor);
RXSKIN_API RECT     RXAPI RXSkinGetRect(const CHAR* pszRect);

RXSKIN_API SIZE     RXAPI GetSize(const CHAR* pszRect);

RXSKIN_API void     RXAPI RXSkinInitTable();
RXSKIN_API void     RXAPI RXSkinConvertYUY2toRGB24(BYTE* pDst, BYTE* pSrc, int w, int h);
RXSKIN_API void     RXAPI RXSkinSaveRgb2Bmp(char* rgbbuf, unsigned int width, unsigned int height,const TCHAR* pszImagePath);
RXSKIN_API INT32    RXAPI RXSkinMessageBox(HWND hParentWnd = NULL,
										   const TCHAR* pszTitle=_T(""),
										   const TCHAR* pszText=_T(""),
										   EMessageBoxType enType=RXSKIN_MB_OK,
										   EMessageBoxTipType enTipType = RXSKIN_MB_HELP);

RXSKIN_API TCHAR*  RXAPI RXSkinFileSelect(HWND hParentWnd = NULL,
	const TCHAR* pszTitle = _T(""),
	const TCHAR* pszText = _T(""),
	const TCHAR* pszDefaultPath = _T(""),
	const EFileSelectType enType = RXSKIN_FILE_SELECT_DIRECTORY);
RXSKIN_API tstring RXAPI RXSkinFolderSelect(HWND hParentWnd = NULL,
	const TCHAR* pszTitle = _T(""),
	const TCHAR* pszText = _T(""),
	const TCHAR* pszDefaultPath = _T(""),
	const EFileSelectType enType = RXSKIN_FILE_SELECT_DIRECTORY);

RXSKIN_API INT32 RXAPI RXSkinWaiting(HWND hParentWnd,
	const TCHAR* pszTitle,
	const TCHAR* pszText);

RXSKIN_API INT32 RXAPI CloseWaiting();

RXSKIN_API BOOL RXAPI RegisterWindowClass(const TCHAR* pszClaasName,
										  WNDPROC lpfnWndProc,
										  HICON hIcon = NULL);
RXSKIN_API TCHAR* RXAPI SelectFile(const TCHAR* pszFilters);
RXSKIN_API TCHAR* RXAPI SelectSaveFile(const TCHAR* pszFilters);
RXSKIN_API TCHAR* RXAPI  RXGetFileTypes(BOOL bOpenFileDialog);

RXSKIN_API void RXAPI  RXGetAllImageFiles(const TCHAR* pszPath,vector<tstring>& vecAll);

class RXSKIN_API CRXSkinRect : public RECT
{
public:
	CRXSkinRect();
	CRXSkinRect(const CRXSkinRect& oRect);
	CRXSkinRect(const RECT& oRect);
	CRXSkinRect(const LONG dwLeft,const LONG dwTop,const LONG dwRight,const LONG dwBottom);
	~CRXSkinRect();
	INT32  Width();
	INT32  Height();
	VOID   SetRectEmpty();
	VOID   OffsetRect(POINT pt);
	void   ResetOffset();
	
	POINT  TopLeft();
	POINT  Center();
	BOOL   IsEmpty() { return Height() <= 0 || Width() <= 0; }
	BOOL   PtInRect(POINT pt);
	void   DeflateRect(int nLeft, int nTop, int nRight, int nBottom);
	CRXSkinRect& operator =(RECT &temp);

public:
	char* ToString();
	char  m_szString[64];
	
};


class RXSKIN_API CRXSkinPoint : public tagPOINT
{
public:
	CRXSkinPoint();
	CRXSkinPoint(const POINT& src);
	CRXSkinPoint(int x, int y);
	CRXSkinPoint(LPARAM lParam);
};


class RXSKIN_API CRXSkinSize : public tagSIZE
{
public:
	CRXSkinSize();
	CRXSkinSize(const SIZE& src);
	CRXSkinSize(const RECT rc);
	CRXSkinSize(int cx, int cy);
	void SetSize(const int& _cx, const int & _cy) { cx = _cx; cy = _cy; }
};


/////////////////////////////////////////////////////////////////////////////////////
//

class RXSKIN_API CRXSkinStdPtrArray
{
public:
	CRXSkinStdPtrArray(int iPreallocSize = 0);
	CRXSkinStdPtrArray(const CRXSkinStdPtrArray& src);
	~CRXSkinStdPtrArray();

	void Empty();
	void Resize(int iSize);
	bool IsEmpty() const;
	int Find(LPVOID iIndex) const;
	bool Add(LPVOID pData);
	bool SetAt(int iIndex, LPVOID pData);
	bool InsertAt(int iIndex, LPVOID pData);
	bool Remove(int iIndex);
	int GetSize() const;
	LPVOID* GetData();

	LPVOID GetAt(int iIndex) const;
	LPVOID operator[] (int nIndex) const;

protected:
	LPVOID* m_ppVoid;
	int m_nCount;
	int m_nAllocated;
};


class RXSKIN_API CRXSkinString
{
public:
	enum { MAX_LOCAL_STRING_LEN = 63 };

	CRXSkinString();
	CRXSkinString(const TCHAR ch);
	CRXSkinString(const CRXSkinString& src);
	CRXSkinString(LPCTSTR lpsz, int nLen = -1);
	~CRXSkinString();

	void Empty();
	int GetLength() const;
	bool IsEmpty() const;
	TCHAR GetAt(int nIndex) const;
	void Append(LPCTSTR pstr);
	void Assign(LPCTSTR pstr, int nLength = -1);
	LPCTSTR GetData() const;

	void SetAt(int nIndex, TCHAR ch);
	operator LPCTSTR() const;

	TCHAR operator[] (int nIndex) const;
	const CRXSkinString& operator=(const CRXSkinString& src);
	const CRXSkinString& operator=(const TCHAR ch);
	const CRXSkinString& operator=(LPCTSTR pstr);
#ifdef _UNICODE
	const CRXSkinString& operator=(LPCSTR lpStr);
	const CRXSkinString& operator+=(LPCSTR lpStr);
#else
	const CRXSkinString& operator=(LPCWSTR lpwStr);
	const CRXSkinString& operator+=(LPCWSTR lpwStr);
#endif
	CRXSkinString operator+(const CRXSkinString& src) const;
	CRXSkinString operator+(LPCTSTR pstr) const;
	const CRXSkinString& operator+=(const CRXSkinString& src);
	const CRXSkinString& operator+=(LPCTSTR pstr);
	const CRXSkinString& operator+=(const TCHAR ch);
	CRXSkinString operator+(const CRXSkinString& src);
	CRXSkinString operator+(LPCTSTR lpStr);
	
	bool operator == (LPCTSTR str) const;
	bool operator != (LPCTSTR str) const;
	bool operator <= (LPCTSTR str) const;
	bool operator <  (LPCTSTR str) const;
	bool operator >= (LPCTSTR str) const;
	bool operator >  (LPCTSTR str) const;

	int Compare(LPCTSTR pstr) const;
	int CompareNoCase(LPCTSTR pstr) const;

	void MakeUpper();
	void MakeLower();

	CRXSkinString Left(int nLength) const;
	CRXSkinString Mid(int iPos, int nLength = -1) const;
	CRXSkinString Right(int nLength) const;
	CRXSkinString& TrimLeft();
	CRXSkinString& TrimRight();
	CRXSkinString& TrimRight(TCHAR ch);
	CRXSkinString& Trim();
	LPCTSTR GetData();
	int Find(TCHAR ch, int iPos = 0) const;
	int Find(LPCTSTR pstr, int iPos = 0) const;
	int ReverseFind(TCHAR ch) const;
	int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

	int __cdecl Format(LPCTSTR pstrFormat, ...);
	int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

protected:
	int __cdecl InnerFormat(LPCTSTR pstrFormat, va_list Args);

protected:
	LPTSTR m_pstr;
	TCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
};

static std::vector<CRXSkinString> StrSplit(CRXSkinString text, CRXSkinString sp)
{
	std::vector<CRXSkinString> vResults;
	int pos = text.Find(sp, 0);
	while (pos >= 0)
	{
		CRXSkinString t = text.Left(pos);
		vResults.push_back(t);
		text = text.Right(text.GetLength() - pos - sp.GetLength());
		pos = text.Find(sp);
	}
	vResults.push_back(text);
	return vResults;
}

struct TITEM
{
	CRXSkinString Key;
	LPVOID Data;
	struct TITEM* pPrev;
	struct TITEM* pNext;
};

class RXSKIN_API CStdStringPtrMap
{
public:
	CStdStringPtrMap(int nSize = 83);
	~CStdStringPtrMap();

	void Resize(int nSize = 83);
	LPVOID Find(LPCTSTR key, bool optimize = true) const;
	bool Insert(LPCTSTR key, LPVOID pData);
	LPVOID Set(LPCTSTR key, LPVOID pData);
	bool Remove(LPCTSTR key);
	void RemoveAll();
	int GetSize() const;
	LPCTSTR GetAt(int iIndex) const;
	LPCTSTR operator[] (int nIndex) const;

protected:
	TITEM** m_aT;
	int m_nBuckets;
	int m_nCount;
};

class CRXSkinStringArray
{
public:

	typedef vector<CRXSkinString>    StringArray;
	typedef StringArray::iterator    POSITION;

	CRXSkinStringArray();
	~CRXSkinStringArray();
	BOOL  IsEmpty() { return m_vecValues.size() == 0; }
	CRXSkinString GetHead();
	void  InsertAfter(POSITION& pos, CRXSkinString str) { m_vecValues.insert(pos, str); }
	CRXSkinString RemoveTail() {
		if (GetCount()>0)
		{
			CRXSkinString str = m_vecValues[GetCount() - 1];
			m_vecValues.pop_back();
			return str;
		}
		return _T("");
	}
	POSITION GetHeadPosition() { return m_vecValues.begin(); }
	BOOL  IsEnd(POSITION& pos) { return pos == m_vecValues.end(); }
	UINT32 GetCount() { return (UINT32)m_vecValues.size(); }
	CRXSkinString GetNext(POSITION& pos) { return *(pos++); }

	CRXSkinString GetAt(const UINT32& dwPos)
	{
		if (dwPos<GetCount())
		{
			return m_vecValues[dwPos];
		}
		return (_T(""));
	}
	void Add(const CRXSkinString& str) { m_vecValues.push_back(str); }
	void RemoveAt(const UINT32& dwPos)
	{
		if (dwPos < GetCount())
		{
			UINT32 m = 0;
			for (vector<CRXSkinString>::iterator itr = m_vecValues.begin();itr!=m_vecValues.end();itr++)
			{
				if (m == dwPos)
				{
					m_vecValues.erase(itr);
					return;
				}
				m++;
			}
		}
	}

private:
	vector<CRXSkinString>   m_vecValues;
};
void RXCreateDesktopShortcut(const TCHAR* pszFileName,
	const uint32_t& dwResourceID,
	const TCHAR* pszResourceType,
	const TCHAR* pszExePath);
void RXCreateStartMenuShortcut(const TCHAR* pszFileName,
	const uint32_t& dwResourceID,
	const TCHAR* pszResourceType,
	const TCHAR* pszExePath,
	const TCHAR* pszFolderName);
#endif