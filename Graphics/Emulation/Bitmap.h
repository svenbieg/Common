//==========
// Bitmap.h
//==========

#pragma once


//=======
// Using
//=======

#include "Graphics/Bitmap.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Emulation {


//========
// Bitmap
//========

class Bitmap: public Graphics::Bitmap
{
public:
	// Con-/Destructors
	Bitmap(UINT Width, UINT Height, BitmapFormat Format=BitmapFormat::RGB);
	~Bitmap();
};

}}