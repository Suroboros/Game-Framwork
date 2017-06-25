#include "Font.h"
#include "D2DClass.h"
#include "DWClass.h"
#include <AtlConv.h>

Font::Font()
{
}

Font::Font(const Font&)
{
}

Font::~Font()
{
}

Font& Font::operator=(const Font&)
{
	return *this;
}

bool Font::Initialize()
{
	
	return true;
}

void Font::Shutdown()
{

}

Font & Font::GetInstance()
{
	static Font instance;
	return instance;
}

void Font::CreateText(char* text, TCHAR* font, FLOAT size, Color color, Rect box)
{
	USES_CONVERSION;
	TCHAR temp[256];
	_tcscpy_s(temp, A2T(text));
	// Draw text
	D2DClass::GetInstance().GetDeviceContext()->DrawText(temp, wcslen(temp), DWClass::GetInstance().CreateTextFormat(font, size), D2D1::RectF(box.left, box.top, box.left + box.width, box.top + box.height), D2DClass::GetInstance().CreateBrush(color.r, color.g, color.b, color.a));
}
