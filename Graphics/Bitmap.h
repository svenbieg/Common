//==========
// Bitmap.h
//==========

#pragma once


//=======
// Using
//=======

#include "Graphics/Color.h"
#include "Graphics/Rect.h"


//===========
// Namespace
//===========

namespace Graphics {


//===============
// Bitmap-Format
//===============

enum class BitmapFormat: UINT
{
Monochrome,
RGB,
RGBA,
Unknown
};


//========
// Bitmap
//========

class Bitmap: public Object
{
public:
	// Using
	using COLOR=Graphics::COLOR;
	using RECT=Graphics::RECT;

	// Common
	BYTE* Begin()const { return pBuffer; }
	VOID Clear(COLOR Color);
	VOID FillRect(RECT const& Rect, COLOR Color);
	BitmapFormat GetFormat()const { return uFormat; }
	UINT GetHeight()const { return uHeight; }
	UINT GetPitch()const { return uPitch; }
	COLOR GetPixel(UINT Left, UINT Top);
	UINT GetSize()const { return uSize; }
	UINT GetWidth()const { return uWidth; }
	VOID SetPixel(UINT Left, UINT Top, COLOR Color);

protected:
	// Con-/Destructors
	Bitmap();

	// Common
	static WORD BitmapFormatToBits(BitmapFormat Format);
	static BitmapFormat BitmapFormatFromBits(WORD BitsPerPixel);
	BYTE* pBuffer;
	BitmapFormat uFormat;
	UINT uHeight;
	UINT uPitch;
	UINT uSize;
	UINT uWidth;
};

}