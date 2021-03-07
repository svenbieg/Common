//========
// Size.h
//========

#pragma once


//===========
// Namespace
//===========

namespace Graphics {


//======
// Size
//======

class SIZE
{
public:
	// Con-/Destructors
	SIZE(): Width(0), Height(0) {}
	SIZE(UINT Width, UINT Height): Width(Width), Height(Height) {}

	// Comparison
	bool operator==(SIZE const& Size) { return CompareMemory(this, &Size, sizeof(SIZE))==0; }

	// Common
	UINT Width;
	UINT Height;
};

}