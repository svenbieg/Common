//============
// Pressure.h
//============

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//===========
// Namespace
//===========

namespace Physics {


//==========
// Pressure
//==========

class Pressure: public TypedVariable<SHORT>
{
public:
	// Con-/Destructors
	Pressure(SHORT hPa=0);
	Pressure(Handle<String> Name, SHORT hPa=0);

	// Common
	Handle<String> ToString()override;
};

}