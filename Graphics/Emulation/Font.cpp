//==========
// Font.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Font.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Emulation {


//============================
// Con-/Destructors Protected
//============================

Font::Font(WORD uheight):
Graphics::Font("Graphics.Emulation.Font"),
uHeight(uheight)
{
ZeroMemory(pCharacters, sizeof(pCharacters));
}

}}