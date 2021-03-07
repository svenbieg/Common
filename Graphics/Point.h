//=========
// Point.h
//=========

#pragma once


//===========
// Namespace
//===========

namespace Graphics {


//=======
// Point
//=======

class POINT
{
public:
	// Con-/Destructors
	POINT(): Left(0), Top(0) {}
	POINT(INT Left, INT Top): Left(Left), Top(Top) {}

	// Comparison
	bool operator==(POINT const& Point) { return CompareMemory(this, &Point, sizeof(POINT))==0; }

	// Common
	INT Left;
	INT Top;
};

}