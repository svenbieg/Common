//=======
// Row.h
//=======

#pragma once


//=======
// Using
//=======

#include "Collections/Vector.h"
#include "Graphics/Color.h"


//===========
// Namespace
//===========

namespace Data {


//=====
// Row
//=====

class Row: public Object
{
private:
	// Using
	using COLOR=Graphics::COLOR;
	using OutputStream=Storage::Streams::OutputStream;
	using ValueVector=Collections::Vector<INT>;

public:
	// Con-/Destructors
	Row(Handle<String> Name=nullptr);

	// Common
	static FLOAT Calculate(FLOAT X, FLOAT const* Xs, FLOAT const* Ys, SIZE_T Count, FLOAT Error=0.f);
	COLOR Color;
	Handle<String> Name;
	SIZE_T PrintToStream(Handle<OutputStream> Stream, UINT Divisor, INT Min, INT Max, INT Offset);
	Handle<ValueVector> Values;
};

}