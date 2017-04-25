#include "Font.h"
#include "D2DClass.h"
#include "DWClass.h"

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

void Font::CreateText(TCHAR* text, TCHAR* font, FLOAT size, Color color, Rect box)
{
	// Draw text
	D2DClass::GetInstance().GetDeviceContext()->DrawText(text, wcslen(text), DWClass::GetInstance().CreateTextFormat(font, size), D2D1::RectF(box.left, box.top, box.right, box.bottom), D2DClass::GetInstance().CreateBrush(color.r, color.g, color.b, color.a));
}
