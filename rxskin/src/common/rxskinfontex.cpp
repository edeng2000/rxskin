#include "common/rxskinfont.h"
#include "common/rxskinservice.h"
#include "common/rxskinresourcemgr.h"
#include "core/SkTypeface.h"
#include "core/SkCanvas.h"
#include "core/SkColor.h"
/*#include "skia/drawtext-skia.h"*/
#include "effects/SkGradientShader.h"
#include "json/cJSON.h"
#include "json/json.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkFont.h"
#include "include/core/SkData.h"
#include "core/SkMatrix.h"
#include "core/SkFontMetrics.h"
#include "skia/SkTextBox.h"
#if defined(SK_BUILD_FOR_WIN)
#include "include/ports/SkTypeface_win.h"
#else
#include "include/ports/SkFontMgr_fontconfig.h"
#endif
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
	SkFontStyle fontStyle = SkFontStyle::Normal();
	if ((lf.lfWeight == FW_BOLD) && lf.lfItalic) {
		fontStyle = SkFontStyle::BoldItalic();
	}
	else if (lf.lfWeight == FW_BOLD) {
		fontStyle = SkFontStyle::Bold();
	}
	else if (lf.lfItalic) {
		fontStyle = SkFontStyle::Italic();
	}
	sk_sp<SkFontMgr> pSkFontMgr = CRXSkinFontMgr::Instance()->GetFontMgr();
	ASSERT(pSkFontMgr != nullptr);
	if (pSkFontMgr == nullptr) {
		return false;
	}
	m_skFont = pSkFontMgr->matchFamilyStyle(_RXTT2UTF8(m_lf.lfFaceName).c_str(), fontStyle);
	m_poFont = new SkFont();
	m_poFont->setTypeface(m_skFont);
	m_poFont->setSize(SkIntToScalar(std::abs(m_lf.lfHeight)));
	m_poFont->setEdging(SkFont::Edging::kSubpixelAntiAlias);
	m_poFont->setSubpixel(true);
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

RX_IMPLEMENT_SINGLETON(CRXSkinFontMgr)
CRXSkinFontMgr::CRXSkinFontMgr()
{
	m_pSkFontMgr = SkFontMgr_New_DirectWrite();
}

CRXSkinFontMgr::~CRXSkinFontMgr()
{
	UnInit();
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

SkTextEncoding CRXSkinFontMgr::GetTextEncoding() const
{
	constexpr const size_t nValueLen = 2;
	if constexpr (nValueLen == 1) {
		return SkTextEncoding::kUTF8;
	}
	else if constexpr (nValueLen == 2) {
		return SkTextEncoding::kUTF16;
	}
	else if constexpr (nValueLen == 4) {
		return SkTextEncoding::kUTF32;
	}
	else {
#ifdef DUILIB_UNICODE
		return SkTextEncoding::kUTF16;
#else
		return SkTextEncoding::kUTF8;
#endif
	}
}


BOOL CRXSkinFontMgr::DrawText(SkCanvas* poCanvas,
	const tstring& strText,
	CRXSkinRect rc,
	const COLORREF clr,
	const uint32_t& dwFontID, UINT uFormat)
{

	const SkTextEncoding textEncoding = GetTextEncoding();

	//获取字体接口    
	CRXSkinFont* pSkiaFont = FindFont(dwFontID);
	ASSERT(pSkiaFont != nullptr);
	if (pSkiaFont == nullptr) {
		return FALSE;
	}
	const SkFont* pSkFont = pSkiaFont->m_poFont;
	ASSERT(pSkFont != nullptr);
	if (pSkFont == nullptr) {
		return FALSE;
	}

	//绘制属性设置
	SkPaint skPaint = m_paint;
	skPaint.setARGB(
		255, GetRValue(clr), GetGValue(clr), GetBValue(clr));
	if (255 != 0xFF) {
		skPaint.setAlpha(255);
	}

	//绘制区域
	SkIRect rcSkDestI = { rc.left, rc.top, rc.right, rc.bottom };
	SkRect rcSkDest = SkRect::Make(rcSkDestI);

	//设置绘制属性
	SkTextBox skTextBox;
	skTextBox.setBox(rcSkDest);
	if (uFormat & DT_SINGLELINE) {
		//单行文本
		skTextBox.setLineMode(SkTextBox::kOneLine_Mode);
	}

	//绘制区域不足时，自动在末尾绘制省略号
	bool bEndEllipsis = false;
	if (uFormat & DT_END_ELLIPSIS) {
		bEndEllipsis = true;
	}
	skTextBox.setEndEllipsis(bEndEllipsis);

	bool bPathEllipsis = false;
	if (uFormat & DT_PATH_ELLIPSIS) {
		bPathEllipsis = true;
	}
	skTextBox.setPathEllipsis(bPathEllipsis);

	//绘制文字时，不使用裁剪区域（可能会导致文字绘制超出边界）
	if (uFormat & DT_NOCLIP) {
		skTextBox.setClipBox(false);
	}
	//删除线
	skTextBox.setStrikeOut(pSkiaFont->IsStrikeOut());
	//下划线
	skTextBox.setUnderline(pSkiaFont->IsUnderline());

	if (uFormat & DT_CENTER) {
		//横向对齐：居中对齐
		skTextBox.setTextAlign(SkTextBox::kCenter_Align);
	}
	else if (uFormat & DT_RIGHT) {
		//横向对齐：右对齐
		skTextBox.setTextAlign(SkTextBox::kRight_Align);
	}
	else {
		//横向对齐：左对齐
		skTextBox.setTextAlign(SkTextBox::kLeft_Align);
	}

	if (uFormat & DT_VCENTER) {
		//纵向对齐：居中对齐
		skTextBox.setSpacingAlign(SkTextBox::kCenter_SpacingAlign);
	}
	else if (uFormat & DT_BOTTOM) {
		//纵向对齐：下对齐
		skTextBox.setSpacingAlign(SkTextBox::kEnd_SpacingAlign);
	}
	else {
		//纵向对齐：上对齐
		skTextBox.setSpacingAlign(SkTextBox::kStart_SpacingAlign);
	}
	skTextBox.draw(poCanvas,
		(const char*)strText.c_str(),
		strText.size() * 2,
		textEncoding,
		*pSkFont,
		skPaint);
	return S_OK;
}

BOOL CRXSkinFontMgr::DrawTextByFont(SkCanvas* poCanvas,
	const tstring& strText,
	CRXSkinRect rc,
	const COLORREF clr,
	CRXSkinFont* poFont,
	UINT uFormat)
{
	const SkTextEncoding textEncoding = GetTextEncoding();

	//获取字体接口    
	CRXSkinFont* pSkiaFont = poFont;
	ASSERT(pSkiaFont != nullptr);
	if (pSkiaFont == nullptr) {
		return FALSE;
	}
	const SkFont* pSkFont = pSkiaFont->m_poFont;
	ASSERT(pSkFont != nullptr);
	if (pSkFont == nullptr) {
		return FALSE;
	}

	//绘制属性设置
	SkPaint skPaint = m_paint;
	skPaint.setARGB(
		255, GetRValue(clr), GetGValue(clr), GetBValue(clr));
	if (255 != 0xFF) {
		skPaint.setAlpha(255);
	}

	//绘制区域
	SkIRect rcSkDestI = { rc.left, rc.top, rc.right, rc.bottom };
	SkRect rcSkDest = SkRect::Make(rcSkDestI);

	//设置绘制属性
	SkTextBox skTextBox;
	skTextBox.setBox(rcSkDest);
	if (uFormat & DT_SINGLELINE) {
		//单行文本
		skTextBox.setLineMode(SkTextBox::kOneLine_Mode);
	}

	//绘制区域不足时，自动在末尾绘制省略号
	bool bEndEllipsis = false;
	if (uFormat & DT_END_ELLIPSIS) {
		bEndEllipsis = true;
	}
	skTextBox.setEndEllipsis(bEndEllipsis);

	bool bPathEllipsis = false;
	if (uFormat & DT_PATH_ELLIPSIS) {
		bPathEllipsis = true;
	}
	skTextBox.setPathEllipsis(bPathEllipsis);

	//绘制文字时，不使用裁剪区域（可能会导致文字绘制超出边界）
	if (uFormat & DT_NOCLIP) {
		skTextBox.setClipBox(false);
	}
	//删除线
	skTextBox.setStrikeOut(pSkiaFont->IsStrikeOut());
	//下划线
	skTextBox.setUnderline(pSkiaFont->IsUnderline());

	if (uFormat & DT_CENTER) {
		//横向对齐：居中对齐
		skTextBox.setTextAlign(SkTextBox::kCenter_Align);
	}
	else if (uFormat & DT_RIGHT) {
		//横向对齐：右对齐
		skTextBox.setTextAlign(SkTextBox::kRight_Align);
	}
	else {
		//横向对齐：左对齐
		skTextBox.setTextAlign(SkTextBox::kLeft_Align);
	}

	if (uFormat & DT_VCENTER) {
		//纵向对齐：居中对齐
		skTextBox.setSpacingAlign(SkTextBox::kCenter_SpacingAlign);
	}
	else if (uFormat & DT_BOTTOM) {
		//纵向对齐：下对齐
		skTextBox.setSpacingAlign(SkTextBox::kEnd_SpacingAlign);
	}
	else {
		//纵向对齐：上对齐
		skTextBox.setSpacingAlign(SkTextBox::kStart_SpacingAlign);
	}
	skTextBox.draw(poCanvas,
		(const char*)strText.c_str(),
		strText.size() * 2,
		textEncoding,
		*pSkFont,
		skPaint);
	return S_OK;
}

BOOL CRXSkinFontMgr::TextOut(SkCanvas* poCanvas, int x,
	int y,
	const tstring& strTxt,
	const COLORREF clr,
	const uint32_t& dwFontID,
	UINT uFormat)
{
	SIZE sz;
	GetTextSize(dwFontID, strTxt, sz, uFormat);
	CRXSkinRect rc;
	rc.left = x;
	rc.top = y;
	rc.right = x+sz.cx;
	rc.bottom = y + sz.cy;
	DrawText(poCanvas, strTxt,rc, clr, dwFontID, uFormat);
// 	CRXSkinFont* poFont = FindFont(dwFontID);
// 	if (NULL == poFont)
// 	{
// 		return S_FALSE;
// 	}
// 	m_paint.setARGB(255, GetRValue(clr), GetGValue(clr), GetBValue(clr));
// #if WCHAR_SIZE == 4
// 	m_paint.setTextEncoding(SkPaint::kUTF32_TextEncoding);
// #else
// 	m_paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
// #endif
// 	m_paint.setAntiAlias(true);
// 	m_paint.setLCDRenderText(true);
// 	m_paint.setStyle(SkPaint::kStroke_Style);
// 	SkPaint     txtPaint = m_paint;
// 	txtPaint.setStyle(SkPaint::kStrokeAndFill_Style);
// 	txtPaint.setTypeface(poFont->GetFont());
// 	txtPaint.setTextSize(SkIntToScalar(abs(poFont->FontSize())));
// 	txtPaint.setUnderlineText(!!poFont->IsUnderline());
// 	txtPaint.setStrikeThruText(!!poFont->IsStrikeOut());
// 
// 	SkPaint::FontMetrics metrics;
// 	txtPaint.getFontMetrics(&metrics);
// 	SkScalar fx = (SkScalar)x;
// 	SkScalar fy = (SkScalar)y;
// 
// 	SkMatrix oldMtx = poCanvas->getTotalMatrix();
// 	if (poFont->LogFont().lfEscapement != 0) {
// 		SkMatrix mtx;
// 		float degree = (float)poFont->LogFont().lfEscapement;
// 		degree /= 10;
// 		degree = 360.f - degree;//change to clockwise.
// 		mtx.setRotate(degree);
// 		mtx.preTranslate(-fx, -fy);
// 		mtx.postTranslate(fx, fy);
// 		mtx.postConcat(oldMtx);
// 		poCanvas->setMatrix(mtx);
// 	}
// 	fy -= metrics.fTop;
// 	poCanvas->drawText((LPCWSTR)strTxt.c_str(),
// 		strTxt.size() * 2,
// 		fx, fy, txtPaint);
// 	if (poFont->LogFont().lfEscapement != 0) {
// 		poCanvas->setMatrix(oldMtx);
// 	}
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
	SIZE sz;
	GetTextSizeByFont(poFont->m_lf, strTxt, sz, uFormat);
	CRXSkinRect rc;
	rc.left = x;
	rc.top = y;
	rc.right = x + sz.cx;
	rc.bottom = y + sz.cy;
	DrawTextByFont(poCanvas, strTxt, rc, clr, poFont, uFormat);
/*	m_paint.setARGB(255, GetRValue(clr), GetGValue(clr), GetBValue(clr));
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
	}*/
	return S_OK;
}

int32_t CRXSkinFontMgr::GetTextWidth(const uint32_t& dwFontID, const tstring& strTxt)
{
/*	int32_t nWidth = 0;
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
	return nWidth;*/
	return 0;
}
void CRXSkinFontMgr::GetTextSize(const uint32_t& dwFontID,
	const tstring& strTxt, 
	SIZE& sz,
	const uint32_t uFormat)
{
	sz.cx = sz.cy = 0;
	CRXSkinFont* poFont = FindFont(dwFontID);
	if (poFont == NULL)
	{
		return;
 	}

	//获取字体接口
	CRXSkinFont* pSkiaFont = FindFont(dwFontID);
	ASSERT(pSkiaFont != nullptr);
	if (pSkiaFont == nullptr) {
		return;
	}
	const SkFont* pSkFont = pSkiaFont->m_poFont;
	ASSERT(pSkFont != nullptr);
	if (pSkFont == nullptr) {
		return ;
	}

	//绘制属性设置
	SkPaint skPaint = m_paint;

	bool isSingleLineMode = false;
	if (uFormat & DT_SINGLELINE) {
		isSingleLineMode = true;
	}
	int width = -1;
	//计算行高
	SkFontMetrics fontMetrics;
	SkScalar fontHeight = pSkFont->getMetrics(&fontMetrics);

	if (isSingleLineMode ) {
		//单行模式, 或者没有限制宽度
		SkScalar textWidth = pSkFont->measureText(strTxt.c_str(),
			strTxt.size() * 2,
			GetTextEncoding(),
			nullptr,
			&skPaint);
		int textIWidth = SkScalarTruncToInt(textWidth + 0.5f);
		if (textWidth > textIWidth) {
			textIWidth += 1;
		}
		if (textIWidth <= 0) {
			return ;
		}
		RECT rc;
		rc.left = 0;
		if (width <= 0) {
			rc.right = textIWidth;
		}
		else if (textIWidth < width) {
			rc.right = textIWidth;
		}
		else {
			//返回限制宽度
			rc.right = width;
		}
		rc.top = 0;
		rc.bottom = SkScalarTruncToInt(fontHeight + 0.5f);
		if (fontHeight > rc.bottom) {
			rc.bottom += 1;
		}
		sz.cx = rc.right - rc.left;
		sz.cy = rc.bottom - rc.top;
		return ;
	}
	else {
		//多行模式，并且限制宽度width为有效值
		ASSERT(width > 0);
		int lineCount = SkTextLineBreaker::CountLines((const char*)strTxt.c_str(),
			strTxt.size() * sizeof(TCHAR),
			GetTextEncoding(),
			*pSkFont,
			skPaint,
			SkScalar(width),
			SkTextBox::kWordBreak_Mode);

		float spacingMul = 1.0f;//行间距倍数，暂不支持设置
		SkScalar scaledSpacing = fontHeight * spacingMul;
		SkScalar textHeight = fontHeight;
		if (lineCount > 0) {
			textHeight += scaledSpacing * (lineCount - 1);
		}
		RECT rc;
		rc.left = 0;
		rc.right = width;
		rc.top = 0;
		rc.bottom = SkScalarTruncToInt(textHeight + 0.5f);
		if (textHeight > rc.bottom) {
			rc.bottom += 1;
		}
		sz.cx = rc.right - rc.left;
		sz.cy = rc.bottom - rc.top;
		return ;
	}
}
int32_t CRXSkinFontMgr::GetTextWidthByFont(const LOGFONT& oFont, 
	const tstring& strTxt,
	const uint32_t uFormat)
{
	int32_t nWidth = 0;
	CRXSkinFont* poFont = FindFontEx(oFont);
	if (poFont == NULL)
	{
		return nWidth;
	}

	const SkFont* pSkFont = poFont->m_poFont;
	ASSERT(pSkFont != nullptr);
	if (pSkFont == nullptr) {
		return 0;
	}

	//绘制属性设置
	SkPaint skPaint = m_paint;

	bool isSingleLineMode = false;
	if (uFormat & DT_SINGLELINE) {
		isSingleLineMode = true;
	}
	int width = -1;
	//计算行高
	SkFontMetrics fontMetrics;
	SkScalar fontHeight = pSkFont->getMetrics(&fontMetrics);

	if (isSingleLineMode) {
		//单行模式, 或者没有限制宽度
		SkScalar textWidth = pSkFont->measureText(strTxt.c_str(),
			strTxt.size() * 2,
			GetTextEncoding(),
			nullptr,
			&skPaint);
		int textIWidth = SkScalarTruncToInt(textWidth + 0.5f);
		if (textWidth > textIWidth) {
			textIWidth += 1;
		}
		if (textIWidth <= 0) {
			return 0;
		}
		RECT rc;
		rc.left = 0;
		if (width <= 0) {
			rc.right = textIWidth;
		}
		else if (textIWidth < width) {
			rc.right = textIWidth;
		}
		else {
			//返回限制宽度
			rc.right = width;
		}
		rc.top = 0;
		rc.bottom = SkScalarTruncToInt(fontHeight + 0.5f);
		if (fontHeight > rc.bottom) {
			rc.bottom += 1;
		}
		return rc.right - rc.left;
	}
	else {
		//多行模式，并且限制宽度width为有效值
		ASSERT(width > 0);
		int lineCount = SkTextLineBreaker::CountLines((const char*)strTxt.c_str(),
			strTxt.size() * sizeof(TCHAR),
			GetTextEncoding(),
			*pSkFont,
			skPaint,
			SkScalar(width),
			SkTextBox::kWordBreak_Mode);

		float spacingMul = 1.0f;//行间距倍数，暂不支持设置
		SkScalar scaledSpacing = fontHeight * spacingMul;
		SkScalar textHeight = fontHeight;
		if (lineCount > 0) {
			textHeight += scaledSpacing * (lineCount - 1);
		}
		RECT rc;
		rc.left = 0;
		rc.right = width;
		rc.top = 0;
		rc.bottom = SkScalarTruncToInt(textHeight + 0.5f);
		if (textHeight > rc.bottom) {
			rc.bottom += 1;
		}
		return rc.right - rc.left;
	}
	return 0;
}

void CRXSkinFontMgr::GetTextSizeByFont(const LOGFONT& oFont, 
	const tstring& strTxt,
	SIZE& sz,
	const uint32_t uFormat)
{
	sz.cx = sz.cy = 0;
	CRXSkinFont* poFont = FindFontEx(oFont);
	if (poFont == NULL)
	{
		return;
	}

	const SkFont* pSkFont = poFont->m_poFont;
	ASSERT(pSkFont != nullptr);
	if (pSkFont == nullptr) {
		return;
	}

	//绘制属性设置
	SkPaint skPaint = m_paint;

	bool isSingleLineMode = false;
	if (uFormat & DT_SINGLELINE) {
		isSingleLineMode = true;
	}
	int width = -1;
	//计算行高
	SkFontMetrics fontMetrics;
	SkScalar fontHeight = pSkFont->getMetrics(&fontMetrics);

	if (isSingleLineMode) {
		//单行模式, 或者没有限制宽度
		SkScalar textWidth = pSkFont->measureText(strTxt.c_str(),
			strTxt.size() * 2,
			GetTextEncoding(),
			nullptr,
			&skPaint);
		int textIWidth = SkScalarTruncToInt(textWidth + 0.5f);
		if (textWidth > textIWidth) {
			textIWidth += 1;
		}
		if (textIWidth <= 0) {
			return;
		}
		RECT rc;
		rc.left = 0;
		if (width <= 0) {
			rc.right = textIWidth;
		}
		else if (textIWidth < width) {
			rc.right = textIWidth;
		}
		else {
			//返回限制宽度
			rc.right = width;
		}
		rc.top = 0;
		rc.bottom = SkScalarTruncToInt(fontHeight + 0.5f);
		if (fontHeight > rc.bottom) {
			rc.bottom += 1;
		}
		sz.cx = rc.right - rc.left;
		sz.cy = rc.bottom - rc.top;
		return;
	}
	else {
		//多行模式，并且限制宽度width为有效值
		ASSERT(width > 0);
		int lineCount = SkTextLineBreaker::CountLines((const char*)strTxt.c_str(),
			strTxt.size() * sizeof(TCHAR),
			GetTextEncoding(),
			*pSkFont,
			skPaint,
			SkScalar(width),
			SkTextBox::kWordBreak_Mode);

		float spacingMul = 1.0f;//行间距倍数，暂不支持设置
		SkScalar scaledSpacing = fontHeight * spacingMul;
		SkScalar textHeight = fontHeight;
		if (lineCount > 0) {
			textHeight += scaledSpacing * (lineCount - 1);
		}
		RECT rc;
		rc.left = 0;
		rc.right = width;
		rc.top = 0;
		rc.bottom = SkScalarTruncToInt(textHeight + 0.5f);
		if (textHeight > rc.bottom) {
			rc.bottom += 1;
		}
		sz.cx = rc.right - rc.left;
		sz.cy = rc.bottom - rc.top;
		return;
	}
}




