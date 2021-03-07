//============
// Bitmap.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Bitmap.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Emulation {


//==================
// Con-/Destructors
//==================

Bitmap::Bitmap(UINT uwidth, UINT uheight, BitmapFormat uformat)
{
WORD ubpp=BitmapFormatToBits(uformat);
uFormat=uformat;
uHeight=uheight;
uPitch=uwidth*ubpp/8;
uSize=uPitch*uHeight;
uWidth=uwidth;
if(uSize==0)
	return;
pBuffer=(BYTE*)Alloc(uSize);
}

Bitmap::~Bitmap()
{
if(pBuffer)
	Free(pBuffer);
}

}}