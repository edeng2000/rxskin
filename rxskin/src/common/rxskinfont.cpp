#include "common/rxskinfont.h"
#include "common/rxskinservice.h"
#include "common/rxskinresourcemgr.h"
#include "core/SkTypeface.h"
#include "core/SkCanvas.h"
#include "core/SkColor.h"
#include "skia/drawtext-skia.h"
#include "effects/SkGradientShader.h"
#include "json/cJSON.h"
#include "json/json.h"
static SkShader* CreateShader(const SkRect& skrc, 
	const GradientInfo* info, const GradientItem* pGradients, 
	int nCount, BYTE byAlpha, SkShader::TileMode tileMode);
#ifndef PI	//PI
#define PI         ((float)3.141592654f)
#endif

CRXSkinFont::CRXSkinFont()
{
	m_skFont = NULL;
}

CRXSkinFont::~CRXSkinFont()
{
	if (m_skFont)
	{
	//	CRXSkinFontMgr::Instance()->DeleteSkFont(m_skFont);
	}
}

bool CRXSkinFont::InitFont(const LOGFONT& lf)
{
	memcpy(&m_lf, &lf, sizeof(LOGFONT));
	string strFaceName = _RXTT2UTF8(lf.lfFaceName);
	BYTE style = SkTypeface::kNormal;
	if (lf.lfItalic) style |= SkTypeface::kItalic;
	if (lf.lfWeight == FW_BOLD) style |= SkTypeface::kBold;
	m_lf.lfCharSet = 1;
	m_lf.lfEscapement = 1;
	m_lf.lfQuality = 5;
	m_bLcdText = 1;
	m_skFont = SkTypeface::CreateFromName(strFaceName.c_str(), (SkTypeface::Style)style, m_lf.lfCharSet);
	return true;
}

/** 获取字体名
*/
tstring CRXSkinFont::FontName()
{
	return m_lf.lfFaceName;
}

/** 获取字体大小(字体高度)
*/
int CRXSkinFont::FontSize() const
{
	return m_lf.lfHeight;
}

/** 是否为粗体
*/
bool CRXSkinFont::IsBold() const
{
	return m_lf.lfWeight == FW_BOLD;
}

/** 字体下划线状态
*/
bool CRXSkinFont::IsUnderline() const
{
	return m_lf.lfUnderline;
}

/** 字体的斜体状态
*/
bool CRXSkinFont::IsItalic() const
{
	return m_lf.lfItalic;
}

/** 字体的删除线状态
*/
bool CRXSkinFont::IsStrikeOut() const
{
	return m_lf.lfStrikeOut;
}
CRXSkinFontMgr* g_poFontMgr = NULL;
CRXSkinFontMgr::CRXSkinFontMgr()
{
	//m_pSkFontMgr = SkFontMgr_New_DirectWrite();	
}

CRXSkinFontMgr::~CRXSkinFontMgr()
{
	UnInit();
}

CRXSkinFontMgr* CRXSkinFontMgr::Instance()
{
	if (g_poFontMgr == NULL)
	{
		g_poFontMgr = new CRXSkinFontMgr;
	}
	return g_poFontMgr;
}

BOOL CRXSkinFontMgr::Init(const TCHAR* pszConfig)
{

	if (CRXSkinService::Instance()->IsFromZipResource())
	{
		std::vector<unsigned char> vec;
		CRXSkinResourceMgr::Instance()->LoadResource(pszConfig, vec);
		if (vec.size() > 0)
		{
			tinyxml2::XMLDocument oDoc;
			char* pszData = (char*)malloc(vec.size() + 1);
			memset(pszData, 0, vec.size() + 1);
			memcpy(pszData, &vec[0], vec.size());
			if (tinyxml2::XML_SUCCESS == oDoc.Parse((const char*)pszData))
			{
				_ReadConfig(&oDoc);
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
	return TRUE;
}

BOOL CRXSkinFontMgr::InitResource(const TCHAR* pszResource)
{
	return TRUE;
}

void CRXSkinFontMgr::UnInit()
{
	DeleteAllFont();
}

void CRXSkinFontMgr::_ReadConfig(tinyxml2::XMLDocument* poDoc, 
	const EResourceType& enFromResource /* = RESOURCE_FROM_FILE */, 
	HMODULE hModel /* = NULL */)
{

}

void CRXSkinFontMgr::AddFont(const uint32_t& dwID, CRXSkinFont* poFont)
{
	CRXSkinFont* poFind = FindFont(dwID);
	if (NULL == poFind)
	{
		m_mapFont.insert(make_pair(dwID, poFont));
	}
}
void CRXSkinFontMgr::AddFontEx(const LOGFONT& oFont, CRXSkinFont* poFont)
{
	std::string strKey = GetFontKey(oFont);
	CRXSkinFont* poFind = FindFontEx(oFont);
	if (NULL == poFind)
	{
		m_mapFontStyle.insert(make_pair(strKey, poFont));
	}
}
void CRXSkinFontMgr::RemoveFont(const uint32_t& dwID)
{
	map<uint32_t, CRXSkinFont*>::iterator itr = m_mapFont.find(dwID);
	if (itr!=m_mapFont.end())
	{
		CRXSkinFont* poFont = itr->second;
		m_mapFont.erase(itr);
		delete poFont;
	}
}
void CRXSkinFontMgr::RemoveFontEx(const LOGFONT& oFont)
{
	std::string strKey = GetFontKey(oFont);

	map<std::string, CRXSkinFont*>::iterator itr = m_mapFontStyle.find(strKey);
	if (itr != m_mapFontStyle.end())
	{
		CRXSkinFont* poFont = itr->second;
		m_mapFontStyle.erase(itr);
		delete poFont;
	}
}
CRXSkinFont* CRXSkinFontMgr::FindFont(const uint32_t& dwID)
{
	map<uint32_t, CRXSkinFont*>::iterator itr = m_mapFont.find(dwID);
	if (itr != m_mapFont.end())
	{
		CRXSkinFont* poFont = itr->second;
		return poFont;
	}
	return NULL;
}
CRXSkinFont* CRXSkinFontMgr::FindFontEx(const LOGFONT& oFont)
{
	std::string strKey = GetFontKey(oFont);
	map<std::string, CRXSkinFont*>::iterator itr = m_mapFontStyle.find(strKey);
	if (itr != m_mapFontStyle.end())
	{
		CRXSkinFont* poFont = itr->second;
		return poFont;
	}
	return NULL;
}
void CRXSkinFontMgr::DeleteAllFont()
{
	map<uint32_t, CRXSkinFont*>::iterator itr = m_mapFont.begin();
	for (;itr != m_mapFont.end();itr++)
	{
		delete itr->second;
	}

	m_mapFont.clear();
}

void CRXSkinFontMgr::DeleteAllFontEx()
{
	map<std::string, CRXSkinFont*>::iterator itr = m_mapFontStyle.begin();
	for (; itr != m_mapFontStyle.end(); itr++)
	{
		delete itr->second;
	}

	m_mapFontStyle.clear();
}

std::string CRXSkinFontMgr::GetFontKey(const LOGFONT& oFont)
{
	cJSON* root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "name", cJSON_CreateString(_RXTT2UTF8(oFont.lfFaceName).c_str()));
	cJSON_AddBoolToObject(root, "size", oFont.lfHeight);
	cJSON_AddBoolToObject(root, "StrikeOut", oFont.lfStrikeOut);
	cJSON_AddBoolToObject(root, "Underline", oFont.lfStrikeOut);
	cJSON_AddBoolToObject(root, "Italic", oFont.lfItalic);
	cJSON_AddBoolToObject(root, "Bold", oFont.lfWeight);
	char* pszJson = cJSON_Print(root);
	return pszJson;
}

SkRect toSkRect(LPCRECT pRc)
{
	SkIRect rc = { (int)pRc->left,(int)pRc->top,(int)pRc->right,(int)pRc->bottom };
	return SkRect::Make(rc);
}
BOOL CRXSkinFontMgr::DrawText(SkCanvas* poCanvas,
	const tstring& strText,
	CRXSkinRect rc,
	const COLORREF clr,
	const uint32_t& dwFontID, UINT uFormat)
{
	if (strText.size() <= 0)
		return FALSE;
#if WCHAR_SIZE == 4
	m_paint.setTextEncoding(SkPaint::kUTF32_TextEncoding);
#else
	m_paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
#endif
	m_paint.setAntiAlias(true);
	m_paint.setLCDRenderText(true);
	m_paint.setStyle(SkPaint::kStroke_Style);
	m_paint.setARGB(255, GetRValue(clr), GetGValue(clr), GetBValue(clr));
	SkPaint     txtPaint = m_paint;	
	CRXSkinFont* pFont = FindFont(dwFontID);
	if (NULL == pFont)
	{
		return FALSE;
	}
	txtPaint.setTypeface(pFont->GetFont());
	txtPaint.setTextSize(SkIntToScalar(abs(pFont->FontSize())));
	txtPaint.setUnderlineText(!!pFont->IsUnderline());
	txtPaint.setStrikeThruText(!!pFont->IsStrikeOut());
	txtPaint.setStyle(SkPaint::Style(pFont->m_fillStyle));
	txtPaint.setLCDRenderText(!!pFont->m_bLcdText);
	txtPaint.setDevKernText(true);
	txtPaint.setDistanceFieldTextTEMP(true);
	if (uFormat & DT_CENTER)
		txtPaint.setTextAlign(SkPaint::kCenter_Align);
	else if (uFormat & DT_RIGHT)
		txtPaint.setTextAlign(SkPaint::kRight_Align);
	else
		txtPaint.setTextAlign(SkPaint::kLeft_Align);
	SkRect skrc = toSkRect(&rc);
	
	if (uFormat & DT_CALCRECT)
	{
		skrc = DrawText_Skia(poCanvas, strText.c_str(),
			strText.size(),skrc, txtPaint, uFormat);
		rc.left = (int)skrc.fLeft;
		rc.top = (int)skrc.fTop;
		rc.right = (int)skrc.fRight;
		rc.bottom = (int)skrc.fBottom;
	}
	else if (pFont->LogFont().lfEscapement != 0) {
		//calc draw size
		SkRect skrcContent = DrawText_Skia(poCanvas,
			strText.c_str(),
			strText.size(), 
			skrc,
			txtPaint, uFormat | DT_CALCRECT);
		if (uFormat & DT_CENTER) {
			skrc.fLeft += (skrc.width() - skrcContent.width()) / 2;
		}
		else if (uFormat & DT_RIGHT) {
			skrc.fLeft += skrc.width() - skrcContent.width();
		}
		skrc.fRight = skrc.fLeft + skrcContent.width();
		if (uFormat & DT_VCENTER) {
			skrc.fTop += (skrc.height() - skrcContent.height()) / 2;
		}
		else if (uFormat & DT_BOTTOM) {
			skrc.fTop += skrc.height() - skrcContent.height();
		}
		skrc.fBottom = skrc.fTop + skrcContent.height();

		SkMatrix oldMtx = poCanvas->getTotalMatrix();
		SkScalar fx = skrc.fLeft;
		SkScalar fy = skrc.fTop;
		SkMatrix mtx;
		float degree = (float)pFont->LogFont().lfEscapement;
		degree /= 10;
		degree = 360.f - degree;//change to clockwise.
		mtx.setRotate(degree);
		mtx.preTranslate(-fx, -fy);
		mtx.postTranslate(fx, fy);
		mtx.postConcat(oldMtx);
		poCanvas->setMatrix(mtx);
		skrc = DrawText_Skia(poCanvas, 
			strText.c_str(),
			strText.size(), skrc, txtPaint, uFormat);
		poCanvas->setMatrix(oldMtx);
	}
	else {
		DrawText_Skia(poCanvas, strText.c_str(),
			strText.size(), skrc, txtPaint, uFormat);
	}
	return S_OK;
}

BOOL CRXSkinFontMgr::DrawTextByFont(SkCanvas* poCanvas,
	const tstring& strText,
	CRXSkinRect rc,
	const COLORREF clr,
	CRXSkinFont* poFont,
	UINT uFormat)
{
	if (strText.size() <= 0)
		return FALSE;
#if WCHAR_SIZE == 4
	m_paint.setTextEncoding(SkPaint::kUTF32_TextEncoding);
#else
	m_paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
#endif
	m_paint.setAntiAlias(true);
	m_paint.setLCDRenderText(true);
	m_paint.setStyle(SkPaint::kStroke_Style);
	m_paint.setARGB(255, GetRValue(clr), GetGValue(clr), GetBValue(clr));
	SkPaint     txtPaint = m_paint;
	txtPaint.setTypeface(poFont->GetFont());
	txtPaint.setTextSize(SkIntToScalar(abs(poFont->FontSize())));
	txtPaint.setUnderlineText(!!poFont->IsUnderline());
	txtPaint.setStrikeThruText(!!poFont->IsStrikeOut());
	txtPaint.setStyle(SkPaint::Style(poFont->m_fillStyle));
	txtPaint.setLCDRenderText(!!poFont->m_bLcdText);
	txtPaint.setDevKernText(true);
	txtPaint.setDistanceFieldTextTEMP(true);
	if (uFormat & DT_CENTER)
		txtPaint.setTextAlign(SkPaint::kCenter_Align);
	else if (uFormat & DT_RIGHT)
		txtPaint.setTextAlign(SkPaint::kRight_Align);
	else
		txtPaint.setTextAlign(SkPaint::kLeft_Align);
	SkRect skrc = toSkRect(&rc);

	if (uFormat & DT_CALCRECT)
	{
		skrc = DrawText_Skia(poCanvas, strText.c_str(),
			strText.size(), skrc, txtPaint, uFormat);
		rc.left = (int)skrc.fLeft;
		rc.top = (int)skrc.fTop;
		rc.right = (int)skrc.fRight;
		rc.bottom = (int)skrc.fBottom;
	}
	else if (poFont->LogFont().lfEscapement != 0) {
		//calc draw size
		SkRect skrcContent = DrawText_Skia(poCanvas,
			strText.c_str(),
			strText.size(),
			skrc,
			txtPaint, uFormat | DT_CALCRECT);
		if (uFormat & DT_CENTER) {
			skrc.fLeft += (skrc.width() - skrcContent.width()) / 2;
		}
		else if (uFormat & DT_RIGHT) {
			skrc.fLeft += skrc.width() - skrcContent.width();
		}
		skrc.fRight = skrc.fLeft + skrcContent.width();
		if (uFormat & DT_VCENTER) {
			skrc.fTop += (skrc.height() - skrcContent.height()) / 2;
		}
		else if (uFormat & DT_BOTTOM) {
			skrc.fTop += skrc.height() - skrcContent.height();
		}
		skrc.fBottom = skrc.fTop + skrcContent.height();

		SkMatrix oldMtx = poCanvas->getTotalMatrix();
		SkScalar fx = skrc.fLeft;
		SkScalar fy = skrc.fTop;
		SkMatrix mtx;
		float degree = (float)poFont->LogFont().lfEscapement;
		degree /= 10;
		degree = 360.f - degree;//change to clockwise.
		mtx.setRotate(degree);
		mtx.preTranslate(-fx, -fy);
		mtx.postTranslate(fx, fy);
		mtx.postConcat(oldMtx);
		poCanvas->setMatrix(mtx);
		skrc = DrawText_Skia(poCanvas,
			strText.c_str(),
			strText.size(), skrc, txtPaint, uFormat);
		poCanvas->setMatrix(oldMtx);
	}
	else {
		DrawText_Skia(poCanvas, strText.c_str(),
			strText.size(), skrc, txtPaint, uFormat);
	}
	return S_OK;
}

BOOL CRXSkinFontMgr::TextOut(SkCanvas* poCanvas, int x,
	int y,
	const tstring& strTxt,
	const COLORREF clr,
	const uint32_t& dwFontID,
	UINT uFormat)
{
	CRXSkinFont* poFont = FindFont(dwFontID);
	if (NULL == poFont)
	{
		return S_FALSE;
	}
	m_paint.setARGB(255, GetRValue(clr), GetGValue(clr), GetBValue(clr));
#if WCHAR_SIZE == 4
	m_paint.setTextEncoding(SkPaint::kUTF32_TextEncoding);
#else
	m_paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
#endif
	m_paint.setAntiAlias(true);
	m_paint.setLCDRenderText(true);
	m_paint.setStyle(SkPaint::kStroke_Style);
	SkPaint     txtPaint = m_paint;
	txtPaint.setStyle(SkPaint::kStrokeAndFill_Style);
	txtPaint.setTypeface(poFont->GetFont());
	txtPaint.setTextSize(SkIntToScalar(abs(poFont->FontSize())));
	txtPaint.setUnderlineText(!!poFont->IsUnderline());
	txtPaint.setStrikeThruText(!!poFont->IsStrikeOut());

	SkPaint::FontMetrics metrics;
	txtPaint.getFontMetrics(&metrics);
	SkScalar fx = (SkScalar)x;
	SkScalar fy = (SkScalar)y;

	SkMatrix oldMtx = poCanvas->getTotalMatrix();
	if (poFont->LogFont().lfEscapement != 0) {
		SkMatrix mtx;
		float degree = (float)poFont->LogFont().lfEscapement;
		degree /= 10;
		degree = 360.f - degree;//change to clockwise.
		mtx.setRotate(degree);
		mtx.preTranslate(-fx, -fy);
		mtx.postTranslate(fx, fy);
		mtx.postConcat(oldMtx);
		poCanvas->setMatrix(mtx);
	}
	fy -= metrics.fTop;
	poCanvas->drawText((LPCWSTR)strTxt.c_str(),
		strTxt.size() * 2,
		fx, fy, txtPaint);
	if (poFont->LogFont().lfEscapement != 0) {
		poCanvas->setMatrix(oldMtx);
	}
	return S_OK;
}

BOOL CRXSkinFontMgr::TextOutByFont(SkCanvas* poCanvas,
	int x,
	int y,
	const tstring& strTxt,
	const COLORREF clr,
	CRXSkinFont* poFont,
	UINT uFormat)
{
	m_paint.setARGB(255, GetRValue(clr), GetGValue(clr), GetBValue(clr));
#if WCHAR_SIZE == 4
	m_paint.setTextEncoding(SkPaint::kUTF32_TextEncoding);
#else
	m_paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
#endif
	m_paint.setAntiAlias(true);
	m_paint.setLCDRenderText(true);
	m_paint.setStyle(SkPaint::kStroke_Style);
	SkPaint     txtPaint = m_paint;
	txtPaint.setStyle(SkPaint::kStrokeAndFill_Style);
	txtPaint.setTypeface(poFont->GetFont());
	txtPaint.setTextSize(SkIntToScalar(abs(poFont->FontSize())));
	txtPaint.setUnderlineText(!!poFont->IsUnderline());
	txtPaint.setStrikeThruText(!!poFont->IsStrikeOut());

	SkPaint::FontMetrics metrics;
	txtPaint.getFontMetrics(&metrics);
	SkScalar fx = (SkScalar)x;
	SkScalar fy = (SkScalar)y;

	SkMatrix oldMtx = poCanvas->getTotalMatrix();
	if (poFont->LogFont().lfEscapement != 0) {
		SkMatrix mtx;
		float degree = (float)poFont->LogFont().lfEscapement;
		degree /= 10;
		degree = 360.f - degree;//change to clockwise.
		mtx.setRotate(degree);
		mtx.preTranslate(-fx, -fy);
		mtx.postTranslate(fx, fy);
		mtx.postConcat(oldMtx);
		poCanvas->setMatrix(mtx);
	}
	fy -= metrics.fTop;
	poCanvas->drawText((LPCWSTR)strTxt.c_str(),
		strTxt.size() * 2,
		fx, fy, txtPaint);
	if (poFont->LogFont().lfEscapement != 0) {
		poCanvas->setMatrix(oldMtx);
	}
	return S_OK;
}

int32_t CRXSkinFontMgr::GetTextWidth(const uint32_t& dwFontID, const tstring& strTxt)
{
	int32_t nWidth = 0;
	CRXSkinFont* poFont = FindFont(dwFontID);
	if (poFont==NULL)
	{
		return nWidth;
	}
	SkPaint oPaint;
#if WCHAR_SIZE == 4
	oPaint.setTextEncoding(SkPaint::kUTF32_TextEncoding);
#else
	oPaint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
#endif
	oPaint.setAntiAlias(true);
	oPaint.setLCDRenderText(true);
	oPaint.setStyle(SkPaint::kStroke_Style);
	oPaint.setStyle(SkPaint::kStrokeAndFill_Style);
	oPaint.setTypeface(poFont->GetFont());
	oPaint.setTextSize(SkIntToScalar(abs(poFont->FontSize())));
	oPaint.setUnderlineText(!!poFont->IsUnderline());
	oPaint.setStrikeThruText(!!poFont->IsStrikeOut());
	string str = _RXTT2UTF8(strTxt.c_str());
	nWidth = (int32_t)oPaint.measureText(str.c_str(), str.size());
	return nWidth;
}
void CRXSkinFontMgr::GetTextSize(const uint32_t& dwFontID,
	const tstring& strTxt, 
	SIZE& sz)
{
	sz.cx = sz.cy = 0;
	CRXSkinFont* poFont = FindFont(dwFontID);
	if (poFont == NULL)
	{
		return;
	}
	SkPaint oPaint;
	oPaint.setTextEncoding(SkPaint::kUTF8_TextEncoding);
	oPaint.setAntiAlias(true);
	oPaint.setLCDRenderText(true);
	oPaint.setStyle(SkPaint::kStroke_Style);
	oPaint.setStyle(SkPaint::kStrokeAndFill_Style);
	oPaint.setTypeface(poFont->GetFont());
	oPaint.setTextSize(SkIntToScalar(abs(poFont->FontSize())));
	oPaint.setUnderlineText(!!poFont->IsUnderline());
	oPaint.setStrikeThruText(!!poFont->IsStrikeOut());
	string str = _RXTT2UTF8(strTxt.c_str());
	sz.cx = (int32_t)oPaint.measureText(str.c_str(), str.size());
	sz.cy = poFont->FontSize();
}
int32_t CRXSkinFontMgr::GetTextWidthByFont(const LOGFONT& oFont, const tstring& strTxt)
{
	int32_t nWidth = 0;
	CRXSkinFont* poFont = FindFontEx(oFont);
	if (poFont == NULL)
	{
		return nWidth;
	}
	SkPaint oPaint;
#if WCHAR_SIZE == 4
	oPaint.setTextEncoding(SkPaint::kUTF32_TextEncoding);
#else
	oPaint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
#endif
	oPaint.setAntiAlias(true);
	oPaint.setLCDRenderText(true);
	oPaint.setStyle(SkPaint::kStroke_Style);
	oPaint.setStyle(SkPaint::kStrokeAndFill_Style);
	oPaint.setTypeface(poFont->GetFont());
	oPaint.setTextSize(SkIntToScalar(abs(poFont->FontSize())));
	oPaint.setUnderlineText(!!poFont->IsUnderline());
	oPaint.setStrikeThruText(!!poFont->IsStrikeOut());
	nWidth = (int32_t)oPaint.measureText(strTxt.c_str(), strTxt.size());
	return nWidth;
}

void CRXSkinFontMgr::GetTextSizeByFont(const LOGFONT& oFont, 
	const tstring& strTxt,
	SIZE& sz)
{
	sz.cx = sz.cy = 0;
	CRXSkinFont* poFont = FindFontEx(oFont);
	if (poFont == NULL)
	{
		return ;
	}
	SkPaint oPaint;
#if WCHAR_SIZE == 4
	oPaint.setTextEncoding(SkPaint::kUTF32_TextEncoding);
#else
	oPaint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
#endif
	oPaint.setAntiAlias(true);
	oPaint.setLCDRenderText(true);
	oPaint.setStyle(SkPaint::kStroke_Style);
	oPaint.setStyle(SkPaint::kStrokeAndFill_Style);
	oPaint.setTypeface(poFont->GetFont());
	oPaint.setTextSize(SkIntToScalar(abs(poFont->FontSize())));
	oPaint.setUnderlineText(!!poFont->IsUnderline());
	oPaint.setStrikeThruText(!!poFont->IsStrikeOut());
	sz.cx = (int32_t)oPaint.measureText(strTxt.c_str(), strTxt.size());
	sz.cy = poFont->FontSize();
}


BOOL CRXSkinFontMgr::DrawGradientRect(SkCanvas* poCanvas,
	const CRXSkinRect oRect, 
	BOOL bVert, 
	POINT ptRoundCorner,
	const GradientItem* pGradients,
	int nCount, BYTE byAlpha) {
	GradientInfo info = { linear,{bVert ? 90.0f : 0.0f} };
	return DrawGradientRectEx(poCanvas,oRect, ptRoundCorner, pGradients, nCount, &info, byAlpha);
}
BOOL CRXSkinFontMgr::DrawGradientRectEx(SkCanvas* poCanvas,
	const CRXSkinRect oRect,
	POINT ptRoundCorner,
	const GradientItem* pGradients,
	int nCount,
	const GradientInfo* info, 
	BYTE byAlpha)
{
	SkRect skrc = toSkRect(&oRect);
	SkShader* pShader = CreateShader(skrc, info, pGradients, nCount, byAlpha, SkShader::kRepeat_TileMode);
	if (!pShader)
	{
		return E_INVALIDARG;
	}

	SkPaint paint = m_paint;
	paint.setShader(pShader)->unref();
	if (ptRoundCorner.x == 0 && ptRoundCorner.y == 0)
		poCanvas->drawRect(skrc, paint);
	else
		poCanvas->drawRoundRect(skrc, (SkScalar)ptRoundCorner.x, (SkScalar)ptRoundCorner.y, paint);
	return S_OK;
}

static bool fequal(float a, float b)
{
	return fabs(a - b) < 0.0000001f;
}
static SkShader* CreateShader(const SkRect& skrc, const GradientInfo* info, const GradientItem* pGradients, int nCount, BYTE byAlpha, SkShader::TileMode tileMode) 
{
	SkColor stack_colors[3];
	SkScalar stack_pos[3];
	SkColor* skColors = nCount <= 3 ? stack_colors : new SkColor[nCount];
	SkScalar* pos = nCount <= 3 ? stack_pos : new SkScalar[nCount];
	for (int i = 0; i < nCount; i++) {
		skColors[i] = SColor(pGradients[i].cr, byAlpha).toARGB();
		pos[i] = pGradients[i].pos;
	}

	SkScalar wid = skrc.width();
	SkScalar hei = skrc.height();
	SkScalar halfWid = wid / 2;
	SkScalar halfHei = hei / 2;
	SkShader* pShader = NULL;
	if (info->type == linear)
	{
		SkPoint skPts[2];
		if (fequal(info->angle, 90.0f) || fequal(info->angle, 270.0f))
		{//90度
			skPts[0].set(halfWid, 0.0f);
			skPts[1].set(halfWid, hei);
		}
		else if (fequal(info->angle, 0.0f) || fequal(info->angle, 180.0f))
		{//水平方向
			skPts[0].set(0.f, halfHei);
			skPts[1].set(wid, halfHei);
		}
		else
		{//其它角度

			float angleInRadians = PI * info->angle / 180;
			double tanAngle = tan(angleInRadians);

			SkPoint pt1a, pt2a;//与左右两条边相交的位置
			SkPoint pt1b, pt2b;//与上下两条边相关的位置

			pt1a.fX = -halfWid, pt2a.fX = halfWid;
			pt1b.fY = -halfHei, pt2b.fY = halfHei;

			pt1a.fY = (SkScalar)(-halfWid * tanAngle);
			pt2a.fY = -pt1a.fY;

			pt1b.fX = (SkScalar)(-halfHei / tanAngle);
			pt2b.fX = -pt1b.fX;

			if (pt2a.fY > halfHei)
			{//using pt1a,pt2a
				skPts[0] = pt1a;
				skPts[1] = pt2a;
			}
			else
			{//using pt1b,pt2b
				skPts[0] = pt1b;
				skPts[1] = pt2b;
			}
			skPts[0].offset(halfWid, halfHei);
			skPts[1].offset(halfWid, halfHei);
		}
		SkPoint::Offset(skPts, 2, skrc.fLeft, skrc.fTop);
		pShader = SkGradientShader::CreateLinear(skPts, skColors, pos, nCount, tileMode);
	}
	else if (info->type == radial)
	{
		SkPoint skCenter;
		skCenter.set(wid * info->radial.centerX, hei * info->radial.centerY);
		SkPoint::Offset(&skCenter, 1, skrc.fLeft, skrc.fTop);
		pShader = SkGradientShader::CreateRadial(skCenter, info->radial.radius, skColors, pos, nCount, tileMode);
	}
	else if (info->type == sweep)
	{
		SkScalar centerX = skrc.fLeft + skrc.width() * info->sweep.centerX;
		SkScalar centerY = skrc.fTop + skrc.height() * info->sweep.centerY;
		pShader = SkGradientShader::CreateSweep(centerX, centerY, skColors, pos, nCount);
	}
	if (nCount > 3) {
		delete[]skColors;
		delete[]pos;
	}
	return pShader;
}


CRXSkinSkiaBrush::CRXSkinSkiaBrush( const GradientItem* pGradients,
	int nCount, const GradientInfo* info, BYTE byAlpha, SkShader::TileMode tileMode)
{
	m_tileMode = (SkShader::TileMode)tileMode;
	m_byAlpha = byAlpha;
	m_gradInfo = *info;
	m_arrGradItem.resize(nCount);
	memcpy(&m_arrGradItem[0], pGradients, sizeof(GradientItem) * nCount);
}

CRXSkinSkiaBrush::CRXSkinSkiaBrush(SkBitmap bmp,
	SkShader::TileMode xtm, SkShader::TileMode ytm)
{
	m_bmp = bmp;
	m_xtm = (SkShader::TileMode)xtm;
	m_ytm = (SkShader::TileMode)ytm;
}

CRXSkinSkiaBrush::CRXSkinSkiaBrush(COLORREF cr)
{
	SColor tmp(cr);
	m_cr = tmp.toARGB();
}

CRXSkinSkiaBrush::~CRXSkinSkiaBrush()
{
}

void CRXSkinSkiaBrush::InitPaint(SkPaint& paint, const SkRect& skrc)
{
	if (m_brushType == Brush_Color)
	{
		paint.setFilterLevel(SkPaint::kNone_FilterLevel);
		paint.setColor(m_cr);
	}
	else if (m_brushType == Brush_Bitmap) {
		SkMatrix mtx;
		mtx.setTranslate(skrc.fLeft, skrc.fTop);
		paint.setShader(SkShader::CreateBitmapShader(m_bmp, m_xtm, m_ytm, &mtx))->unref();
	}
	else//if(m_brushType == Brush_Shader)
	{
		SkShader* pShader = CreateShader(skrc, &m_gradInfo, &m_arrGradItem[0], m_arrGradItem.size(), m_byAlpha, m_tileMode);
		if (!pShader)
			return;
		paint.setShader(pShader)->unref();
	}
}


BOOL CRXSkinSkiaBrush::IsFullArc() const
{
	if (m_gradInfo.type == sweep)
		return m_gradInfo.sweep.bFullArc;
	else
		return FALSE;
}