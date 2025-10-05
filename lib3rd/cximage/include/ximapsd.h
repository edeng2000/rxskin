#ifndef __XIMAGE_PSD_INCLUDE___
#define __XIMAGE_PSD_INCLUDE___

#include "ximage.h"
#include "../libpsd/libpsd.h"

class CxImagePSD: public CxImage
{
public:
	CxImagePSD(): CxImage(CXIMAGE_FORMAT_PSD) 
	{
	}
	~CxImagePSD()
	{

	}
	bool Decode(CxFile * hFile);
	bool Decode(FILE *hFile) { CxIOFile file(hFile); return Decode(&file); }
	bool Encode(CxFile * hFile);
	bool Encode(FILE *hFile) { CxIOFile file(hFile); return Encode(&file); }

protected:
};

#endif
