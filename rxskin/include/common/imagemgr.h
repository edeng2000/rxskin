#pragma once
#include "rxskin.h"
#include "rximage.h"
#include "fundation/rxstring.h"
#include "rxskinimagelist.h"
#include <map>
using namespace std;
using namespace RX;
using namespace RX::Image;
#pragma warning(disable: 4251)
class RXSKIN_API CImageMgr
{
public:
	CImageMgr(void);
	~CImageMgr(void);

	typedef map<tstring,IRXImage*>         CImageMap;
	typedef CImageMap::iterator            CImageMapItr;

	typedef map<UINT32,IRXImage*>          CImageIDMap;
	typedef CImageIDMap::iterator          CImageIDMapItr;

	typedef map<string, CRXSkinImageList*>         CRXSkinImageListMap;
	typedef CRXSkinImageListMap::iterator          CRXSkinImageListMapItr;

	BOOL            Init(const TCHAR* pszConfig);
	BOOL            InitResource(const TCHAR* pszResource);
	VOID            UnInit();
	BOOL            SaveJpg(const TCHAR* pszFrom,const TCHAR* psTo);
	IRXImage*       OpenImage(const TCHAR* pszFilename);
	IRXImage*       OpenDirectImage(const TCHAR* pszFilename);

	IRXImage* OpenImageFromResource(const TCHAR* pszResource);
	IRXImage* OpenDirectImageFromResource(const TCHAR* pszResource);

	IRXImage*		LoadIconDirectImage(const TCHAR* pszFilename);
	IRXImage*       FindImage(const UINT32 dwID);
	VOID            RemoveImage(const TCHAR* pszFilename);

	CRXSkinImageList* AddImageList(const char* pszName, const TCHAR* pszFilePath, const uint32_t& dwCX, const uint32_t& dwCY);
	CRXSkinImageList* FindImageList(const char* pszName);

	TCHAR*          GetFileTypes(BOOL bOpenFileDialog);
	int             GetIndexFromType(int nDocType, BOOL bOpenFileDialog);
	tstring         GetExtFromType(int nDocType);
	int             GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
private:
	IRXImageModule* m_poImageModule;
	CImageMap       m_oImageMap;
	CImageIDMap     m_oImageIDMap;
	CImageIDMap     m_mapIcons;
	CRXSkinImageListMap m_mapImageList;
private:
	VOID            _DeleteAll();
};
