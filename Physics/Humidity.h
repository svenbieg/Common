//============
// Humidity.h
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
// Humidity
//==========

class Humidity: public TypedVariable<CHAR>
{
public:
	// Con-/Destructors
	Humidity(CHAR Percent=0);
	Humidity(Handle<String> Name, CHAR Percent=0);

	// Common
	Handle<String> ToString()override;
};

}