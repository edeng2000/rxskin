/*
 * File:	ximapng.h
 * Purpose:	PNG Image Class Loader and Writer
 */
/* ==========================================================
 * CxImagePNG (c) 07/Aug/2001 Davide Pizzolato - www.xdp.it
 * For conditions of distribution and use, see copyright notice in ximage.h
 *
 * Special thanks to Troels Knakkergaard for new features, enhancements and bugfixes
 *
 * original CImagePNG  and CImageIterator implementation are:
 * Copyright:	(c) 1995, Alejandro Aguilar Sierra <asierra(at)servidor(dot)unam(dot)mx>
 *
 * libpng Copyright (c) 1998-2003 Glenn Randers-Pehrson
 * ==========================================================
 */
#if !defined(__ximaPNG_h)
#define __ximaPNG_h

#include "ximage.h"

#if CXIMAGE_SUPPORT_PNG

extern "C" {
#ifdef _LINUX
 #undef _DLL
 #include <png.h>
 #include <pngstruct.h>
 #include <pnginfo.h>
#else
 #include "../png/png.h"
 #include "../png/pngstruct.h"
 #include "../png/pnginfo.h"
#endif
}

class CxImagePNG: public CxImage
{
public:
	CxImagePNG(): CxImage(CXIMAGE_FORMAT_PNG) 
	{
	}
	~CxImagePNG()
	{

	}
	bool DecodeData(const uint8_t* pszData, const uint32_t& dwSize);
	bool Decode(CxFile * hFile);
	bool Decode(FILE *hFile) { CxIOFile file(hFile); return Decode(&file); }

#if CXIMAGE_SUPPORT_ENCODE
	bool Encode(CxFile * hFile);
	bool Encode(FILE *hFile) { CxIOFile file(hFile); return Encode(&file); }
#endif // CXIMAGE_SUPPORT_ENCODE

	enum CODEC_OPTION
	{
		ENCODE_INTERLACE = 0x01,
		// Exclusive compression types : 3 bit wide field
		ENCODE_COMPRESSION_MASK = 0x0E,
		ENCODE_NO_COMPRESSION =      1 << 1,
		ENCODE_BEST_SPEED =          2 << 1,
		ENCODE_BEST_COMPRESSION =    3 << 1,
		ENCODE_DEFAULT_COMPRESSION = 4 << 1
	}; 

protected:
	void ima_png_error(png_struct *png_ptr, char *message);
	void expand2to4bpp(uint8_t* prow);	

	static void PNGAPI user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
	{		
		CxFile* hFile = (CxFile*)png_get_io_ptr(png_ptr);
		if (hFile)
		{
			hFile->Read(data, 1, length);
		}
	}

	static void PNGAPI user_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		CxFile* hFile = (CxFile*)png_get_io_ptr(png_ptr);
		if (hFile)
		{
			hFile->Write(data, 1, length);
		}
	}

	static void PNGAPI user_flush_data(png_structp png_ptr)
	{
		CxFile* hFile = (CxFile*)png_get_io_ptr(png_ptr);
		if (hFile)
		{
			hFile->Flush();
		}
	}

    static void PNGAPI user_error_fn(png_structp png_ptr,png_const_charp error_msg)
	{
		//strncpy((char*)png_ptr->error_ptr,error_msg,255);
		longjmp(png_ptr->jmp_buf_local, 1);
	}
private:
	uint8_t* m_poImageData;
	uint32_t m_dwSize;
};

#endif

#endif
