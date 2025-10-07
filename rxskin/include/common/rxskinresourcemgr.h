#pragma once
#include "RXSkin.h"
#include "drawmgr.h"
#include "common/rxskinutil.h"
#include "common/imagemgr.h"
#include <map>
#include <unordered_map>
#include "mkmutex.h"
#include "rxsingleton.h"
#include "tinyxml2.h"
#include "fundation/ZipArchive.h"
using namespace std;

#pragma warning(disable: 4251)

class RXSKIN_API CRXSkinResourceMgr
{
public:
	static CRXSkinResourceMgr* Instance();

	tinyxml2::XMLDocument* GetXml(const TCHAR* pszResoureName);
	BYTE* GetRTF(const TCHAR* pszResoureName,uint32_t&dwLen);
	BYTE* GetExe(const TCHAR* pszResoureName, uint32_t& dwLen);
	BYTE* GetResource(const TCHAR* pszResoureName, const TCHAR* pszType,  uint32_t& dwLen);

	tstring  GetResource(const char*  pszValue);
	BYTE*    GetPNG(const TCHAR* pszResoureName, uint32_t& dwLen);
	BYTE* GetGif(const TCHAR* pszResoureName, uint32_t& dwLen);

	bool     OpenZipResource(const TCHAR* pszPath, const char* pszPassword);
	bool     LoadResource(const TCHAR* pszPath, std::vector<unsigned char>& vec);
	bool     LoadResourceFromZip(const TCHAR* pszPath, std::vector<unsigned char>& vec);
	tinyxml2::XMLDocument* LoadXmlFromResource(const TCHAR* pszResoureName);
private:
//	zipper::Unzipper* m_poUnzip;
//	std::vector<zipper::ZipEntry>  m_vecItems;
	CZipArchive  m_oZip;

private:
	CRXSkinResourceMgr();
	~CRXSkinResourceMgr();
};