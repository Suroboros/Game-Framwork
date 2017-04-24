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

void Font::CreateText(TCHAR* text, TCHAR* font, FLOAT size, float r,float g,float b,float a)
{
	// Draw text
	D2DClass::GetInstance().GetDeviceContext()->DrawText(text, wcslen(text), DWClass::GetInstance().CreateTextFormat(font, size), D2D1::RectF(0, 0, 800, 600), D2DClass::GetInstance().CreateBrush(r, g, b, a));
	D2DClass::GetInstance().GetDeviceContext()->DrawText(text, wcslen(text), DWClass::GetInstance().CreateTextFormat(font, size), D2D1::RectF(100, 100, 800, 600), D2DClass::GetInstance().CreateBrush(0.0, 1.0, 0.0, 1.0));
}
