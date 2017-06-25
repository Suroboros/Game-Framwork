#pragma once
/////////////////////////////////////
// Image2D.h
// Render image in screen(2D).
/////////////////////////////////////

#ifndef _IMAGE_2D_H_
#define _IMAGE_2D_H_

// Include
#include "D2DClass.h"
#include "GEUtility.h"

// Image2D class
class Image2D
{
public:
	Image2D();
	Image2D(Image2D& image);
	~Image2D();

	bool Initialize(LPCTSTR srcPath);
	void Shutdown();

	void DrawImage(Rect destination, Rect source, float opacity);

private:
	bool LoadImageFromFile(LPCTSTR srcPath);
	ID2D1Bitmap* m_Bitmap;

};

#endif // !_IMAGE_2D_H_
