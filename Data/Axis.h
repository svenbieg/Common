//========
// Axis.h
//========

#pragma once


//=======
// Using
//=======

#include "Collections/Map.h"


//===========
// Namespace
//===========

namespace Data {


//======
// Axis
//======

class Axis: public Object
{
private:
	// Using
	using PointMap=Collections::Map<INT, Handle<String>, 8>;

public:
	// Con-/Destructors
	Axis(Handle<String> Name=nullptr, Handle<String> Unit=nullptr);

	// Common
	Handle<String> Name;
	Handle<PointMap> Points;
	Handle<String> Unit;
};

}