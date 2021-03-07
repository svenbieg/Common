//=================
// DeviceContext.h
//=================

#pragma once


//=======
// Using
//=======

#include "Bitmap.h"
#include "Color.h"
#include "Font.h"
#include "Point.h"
#include "Rect.h"
#include "Size.h"


//===========
// Namespace
//===========

namespace Graphics {


//================
// Device-Context
//================

class DeviceContext: public Object
{
public:
	// Con-/Destructors
	DeviceContext();

	// Using
	using SIZE=Graphics::SIZE;

	// Common
	virtual VOID BeginPaint(Handle<Bitmap> Target) { hTarget=Target; }
	RECT Clip;
	VOID Clear(COLOR Color=0);
	virtual VOID DrawText(Handle<String> Text, COLOR Color);
	virtual VOID EndPaint() {}
	VOID FillRect(RECT const& Rect, COLOR Color);
	Handle<Graphics::Font> Font;
	virtual SIZE MeasureText(Handle<Graphics::Font> Font, Handle<String> Text);
	POINT Offset;
	VOID SetPixel(INT Left, INT Top, COLOR Color);

protected:
	// Common
	BOOL AdjustRect(RECT& Rect);
	Handle<Bitmap> hTarget;
};

}