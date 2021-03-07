//========
// Font.h
//========

#pragma once


//=======
// Using
//=======

#include "Convertible.h"


//===========
// Namespace
//===========

namespace Graphics {


//======
// Font
//======

class Font: public Convertible
{
public:
	// Common
	virtual WORD GetHeight()const=0;

protected:
	// Con-/Destructors
	Font(Handle<String> Type): Convertible(Type+";Graphics.Font") {}
};

}