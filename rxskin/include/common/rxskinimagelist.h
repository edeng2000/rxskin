#pragma once
#include "rxskin.h"
#include "rximage.h"
#include "fundation/rxstring.h"
#include <map>
using namespace std;
using namespace RX;
using namespace RX::Image;
#pragma warning(disable: 4251)

class RXSKIN_API CRXSkinImageList
{
public:
	CRXSkinImageList(void);
	~CRXSkinImageList(void);
	bool OpenFile(const char* pszName,const TCHAR* pszFilePath,const uint32_t& dwCX,const uint32_t& dwCY);
	IRXImage* GetImage();
	uint32_t GetX();
	uint32_t GetY();
	uint32_t GetCount();

	bool     CheckImageIndex(const uint32_t& dwImageIndex);
private:
	IRXImage* m_poImage;
	string m_strName;
	tstring   m_strFilePath;
	uint32_t  m_dwX;
	uint32_t  m_dwY;
};
