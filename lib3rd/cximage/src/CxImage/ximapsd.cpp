/*
 * File:	ximapsd.cpp
 * Purpose:	Platform Independent PSD Image Class Loader
 * Dec/2010 Davide Pizzolato - www.xdp.it
 * CxImage version 7.0.2 07/Feb/2011
 *
 * libpsd (c) 2004-2007 Graphest Software
 *
 * Based on MyPSD class by Iosif Hamlatzis
 * Details: http://www.codeproject.com/KB/graphics/MyPSD.aspx
 * Cleaned up a bit and ported to CxImage by Vitaly Ovchinnikov
 * Send feedback to vitaly(dot)ovchinnikov(at)gmail.com
 */

#include "ximapsd.h"

bool CxImagePSD::Decode(CxFile *hFile)
{
	if (hFile==NULL)
		return false;
	cx_try
	{
// 		m_oImage.load(m_strFileName.c_str());
// 		Create(m_oImage.getWidth(), m_oImage.getHeight(),24,CXIMAGE_FORMAT_PSD);
// 		uint8_t* alpha = NULL;
// 		if (m_oImage.isTransparent())
// 			alpha = (uint8_t*)m_oImage.getTransparencyTable();
// 		if (alpha)
// 			AlphaCreate();
// 		
// 		int32_t x,y;
// 		RGBQUAD c;
// 		c.rgbReserved = 0;		
// 		for (y = m_oImage.getHeight() - 1; y--;) {
// 			for (x = 0; x < m_oImage.getWidth(); x++) {
// 				m_oImage.getPixelColor(x, y, &c);
// 				SetPixelColor(x, y, c);
// 				if (alpha) AlphaSet(x, y, *alpha++);
// 			}
// 		}

	}
	cx_catch {
		if (strcmp(message,"")) strncpy(info.szLastError,message,255);
		if (info.nEscape == -1 && info.dwType == CXIMAGE_FORMAT_PSD) return true;
		return false;
	}
	return true;
}

bool CxImagePSD::Encode(CxFile * hFile)
{
	if (hFile == NULL) return false;
	strcpy(info.szLastError, "Save PSD not supported");
	return false;
}
