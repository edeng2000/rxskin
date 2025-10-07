#pragma once
#include "rxskin.h"
#include <CommCtrl.h>
#include <map>
#include "fundation/tinyxml2.h"
#include "common/rximage.h"
#include "common/rxskinutil.h"
#include "controls/rxcontrol.h"
#include "core/SkFont.h"
#include "core/SkShader.h"
#include "ports/SkFontMgr.h"
#include "fundation/rxsingleton.h"
#include <unordered_map>
using namespace std;

typedef enum _FillStyle
{
	kFill_Style = 0,
	kStroke_Style = 1,
	kStrokeAndFill_Style = 2,
} FillStyle;

#define CR_INVALID 0x00FFFFFF
#ifndef GetAValue
#define GetAValue(rgb) (LOBYTE(rgb >> 24))
#endif // GetAValue

#ifndef RGBA
#define RGBA(r, g, b, a) (RGB(r, g, b) | (a << 24))
#endif // RGBA

//以ARGB格式存储颜色值
class SColor {
public:
	SColor(BYTE r, BYTE g, BYTE b, BYTE a = 0xFF)
		: b(b)
		, g(g)
		, r(r)
		, a(a)
	{
	}

	SColor(COLORREF cr, BYTE alpha)
	{
		r = GetRValue(cr);
		g = GetGValue(cr);
		b = GetBValue(cr);
		a = GetAValue(cr);
		updateAlpha(alpha);
	}

	SColor(COLORREF cr)
	{
		r = GetRValue(cr);
		g = GetGValue(cr);
		b = GetBValue(cr);
		a = GetAValue(cr);
	}

	const COLORREF toCOLORREF() const
	{
		return RGB(r, g, b) | (a << 24);
	}

	const DWORD toARGB() const
	{
		DWORD crRet;
		memcpy(&crRet, this, 4);
		return crRet;
	}

	void setRGB(COLORREF cr)
	{
		r = GetRValue(cr);
		g = GetGValue(cr);
		b = GetBValue(cr);
		a = GetAValue(cr);
	}

	void setRGB(COLORREF cr, BYTE alpha)
	{
		r = GetRValue(cr);
		g = GetGValue(cr);
		b = GetBValue(cr);
		a = GetAValue(cr);
		updateAlpha(alpha);
	}

	void updateAlpha(BYTE alpha)
	{
		if (alpha != 0xFF)
		{
			if (a == 0xFF)
				a = alpha;
			else
				a = (a * alpha) / 0xFF;
		}
	}

	DWORD b : 8;
	DWORD g : 8;
	DWORD r : 8;
	DWORD a : 8;
};
class RXSKIN_API CRXSkinFont
{
public:

    CRXSkinFont();
    ~CRXSkinFont();

    /** 初始化字体(内部未对字体大小做DPI自适应)
     */
    virtual bool InitFont(const LOGFONT& lf) ;

    /** 获取字体名
    */
    virtual tstring FontName();

    /** 获取字体大小(字体高度)
    */
    virtual int FontSize() const;

    /** 是否为粗体
    */
    virtual bool IsBold() const;

    /** 字体下划线状态
    */
    virtual bool IsUnderline() const ;

    /** 字体的斜体状态
    */
    virtual bool IsItalic() const ;

    /** 字体的删除线状态
    */
    virtual bool IsStrikeOut() const;
    SkTypeface* GetFont() { return m_skFont; }
    const LOGFONT& LogFont() { return m_lf; }
    public:
//字体句柄
	SkTypeface* m_skFont;   //定义字体
	SkPaint     m_skPaint;  //定义文字绘制属性
	LOGFONT     m_lf;
	FillStyle	m_fillStyle;
	BOOL		m_bLcdText;
};
struct SFontFromFile
{
	//字体名称
	TCHAR m_szFontFamilyName[MAX_PATH];

	//字体风格 -> 字体数据
	std::vector<SkTypeface*> m_vecFontTypeface;
    SFontFromFile()
    {
        memset(m_szFontFamilyName, 0, MAX_PATH * sizeof(TCHAR));
        m_vecFontTypeface.clear();
    }
    SFontFromFile(const SFontFromFile& oFont)
    {
		memset(m_szFontFamilyName, 0, MAX_PATH * sizeof(TCHAR));
		m_vecFontTypeface.clear();
        _RXTStrncpy(m_szFontFamilyName, oFont.m_szFontFamilyName, MAX_PATH);
        for (uint32_t t=0;t<(uint32_t)oFont.m_vecFontTypeface.size();t++)
        {
            m_vecFontTypeface.push_back(oFont.m_vecFontTypeface[t]);
        }
    }
};
typedef struct _GradientItem
{
	COLORREF cr;
	float pos;
} GradientItem;

typedef enum _BrushType
{
	Brush_Color = 0, //颜色画刷
	Brush_Bitmap,    //位图画刷
	Brush_Shader,    //渐变画刷
} BrushType;
class RXSKIN_API CRXSkinSkiaPen
{
public:
	CRXSkinSkiaPen(int iStyle = PS_SOLID,
		COLORREF cr = 0xFF000000,
		int cWidth = 1) 
	{
		m_style = iStyle;
		m_nWidth = cWidth;
		m_cr = cr;
	};
	~CRXSkinSkiaPen() {};
	int GetWidth() { return m_nWidth; };
	void SetWidth(int nWid) { m_nWidth = nWid; };

	int GetStyle() { return m_style; };

	void SetStyle(int style) { m_style = style; };

	COLORREF GetColor() { return m_cr; };

	void SetColor(COLORREF cr){m_cr = cr; };

protected:
	int		m_style;
	int		m_nWidth;
	COLORREF	m_cr;
};
typedef enum GradientType
{
	linear = 0, /*<线性渐变*/
	radial,     /*<辐射渐变*/
	sweep       /*<扫描渐变*/
} GradientType;
typedef struct _GradientInfo
{
	GradientType type;
	union {
		float angle; // for linear
		struct
		{
			float radius;  // radical
			float centerX; // 0.0 -> 1.0, 0.5 is center of x
			float centerY; // 0.0 -> 1.0, 0.5 is center of y
		} radial;
		struct
		{
			BOOL bFullArc; // true(default) indicator gradient is rendered for full circle.
			float centerX; // 0.0 -> 1.0, 0.5 is center of x
			float centerY; // 0.0 -> 1.0, 0.5 is center of y
		} sweep;           // sweep;
	};
} GradientInfo;

class RXSKIN_API CRXSkinSkiaBrush
{
public:
	CRXSkinSkiaBrush( COLORREF cr);
	CRXSkinSkiaBrush(SkBitmap bmp, SkShader::TileMode xtm, SkShader::TileMode ytm);
	CRXSkinSkiaBrush(const GradientItem* pGradients,
		int nCount, const GradientInfo* info, BYTE byAlpha,SkShader::TileMode  tileMode);

	~CRXSkinSkiaBrush();

public:
	BrushType GetBrushType(){
		return m_brushType;
	}
	void InitPaint(SkPaint& paint, const SkRect& skrc);
	BOOL IsFullArc() const;
protected:
	BrushType m_brushType;

	SkColor m_cr;		//颜色画刷
	SkBitmap  m_bmp;	//位图画刷
	SkShader::TileMode m_xtm, m_ytm;
	//gradient info
	int m_nWid, m_nHei;
	GradientInfo m_gradInfo;
	BYTE	m_byAlpha;
	vector<GradientItem> m_arrGradItem;

	SkShader::TileMode  m_tileMode;
};




class RXSKIN_API CRXSkinFontMgr
{
public:
	static CRXSkinFontMgr* Instance();

	BOOL         Init(const TCHAR* pszConfig);
	BOOL         InitResource(const TCHAR* pszResource);
	VOID         UnInit();
    BOOL         DrawText(SkCanvas* poCanvas,
        const tstring& strText,
        CRXSkinRect rc,
        const COLORREF clr,
        const uint32_t& dwFontID, 
        UINT uFormat);

	BOOL         DrawTextByFont(SkCanvas* poCanvas,
		const tstring& strText,
		CRXSkinRect rc,
		const COLORREF clr,
		CRXSkinFont*poFont,
		UINT uFormat);
	BOOL TextOut(SkCanvas* poCanvas,
		int x,
		int y,
		const tstring& strTxt,
		const COLORREF clr,
		const uint32_t& dwFontID,
		UINT uFormat);

	BOOL TextOutByFont(SkCanvas* poCanvas,
		int x,
		int y,
		const tstring& strTxt,
		const COLORREF clr,
		CRXSkinFont* poFont,
		UINT uFormat);

	int32_t GetTextWidth(const uint32_t& dwFontID, const tstring& strTxt);
	int32_t GetTextWidthByFont(const LOGFONT& oFont, const tstring& strTxt);
	VOID    GetTextSize(const uint32_t& dwFontID, const tstring& strTxt,SIZE& sz);
	void    GetTextSizeByFont(const LOGFONT& oFont, const tstring& strTxt,SIZE& sz);

    void    AddFont(const uint32_t& dwID, CRXSkinFont* poFont);
    void    RemoveFont(const uint32_t& dwID);
    CRXSkinFont* FindFont(const uint32_t& dwID);
    void    DeleteAllFont();

	void    AddFontEx(const LOGFONT& oFont, CRXSkinFont* poFont);
	void    RemoveFontEx(const LOGFONT& oFont);
	CRXSkinFont* FindFontEx(const LOGFONT& oFont);
	void    DeleteAllFontEx();

	std::string  GetFontKey(const LOGFONT& oFont);

	BOOL DrawGradientRectEx(SkCanvas* poCanvas,
		const CRXSkinRect oRect,
		POINT ptRoundCorner,
		const GradientItem* pGradients,
		int nCount,
		const GradientInfo* info,
		BYTE byAlpha);
	BOOL DrawGradientRect(SkCanvas* poCanvas,
		const CRXSkinRect oRect,
		BOOL bVert,
		POINT ptRoundCorner,
		const GradientItem* pGradients,
		int nCount, BYTE byAlpha);
private:
	CRXSkinFontMgr();
	~CRXSkinFontMgr();
    bool         AddFontTypeface(const SkTypeface* spTypeface);
	VOID        _ReadConfig(tinyxml2::XMLDocument* poDoc, const EResourceType& enFromResource = RESOURCE_FROM_FILE, HMODULE hModel = NULL);
private:
    SkFontMgr* m_pSkFontMgr;
    std::vector<SFontFromFile> m_vecFontFamilies;
    /** 默认的字体名称
*/
    SkPaint			m_paint;
    tstring m_strDefaultFontName;
    std::map<string, CRXSkinFont*> m_mapFontStyle;
    map<uint32_t, CRXSkinFont*> m_mapFont;
};

