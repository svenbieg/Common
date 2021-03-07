//========
// Rect.h
//========

#pragma once


//===========
// Namespace
//===========

namespace Graphics {


//======
// Rect
//======

class RECT
{
public:
	// Con-/Destructors
	RECT(): Left(0), Top(0), Right(0), Bottom(0) {}
	RECT(INT Left, INT Top, INT Right, INT Bottom): Left(Left), Top(Top), Right(Right), Bottom(Bottom) {}

	// Comparison
	bool operator==(RECT const& Rect) { return CompareMemory(this, &Rect, sizeof(RECT))==0; }

	// Common
	INT Left;
	INT Top;
	INT Right;
	INT Bottom;
};

}