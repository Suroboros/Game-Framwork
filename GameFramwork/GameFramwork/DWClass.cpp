#include "DWClass.h"

bool DWClass::Initialize()
{
	HRESULT hr;

	// Create direct write factory
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_factory), (IUnknown**)&m_factory);
	if(FAILED(hr))
		return false;

	return true;
}

void DWClass::Shutdown()
{
	// Release text format
	if(m_textFormat)
	{
		m_textFormat->Release();
		m_textFormat = nullptr;
	}

	// Release factory
	if(m_factory)
	{
		m_factory->Release();
		m_factory = nullptr;
	}
}

DWClass & DWClass::GetInstance()
{
	static DWClass instance;
	return instance;
}

IDWriteTextFormat * DWClass::CreateTextFormat(TCHAR * font, FLOAT size)
{
	// Create text format
	m_factory->CreateTextFormat(font, nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, _T(""), &m_textFormat);
	return m_textFormat;
}
