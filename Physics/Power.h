//=========
// Power.h
//=========

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//===========
// Namespace
//===========

namespace Physics {


//=======
// Power
//=======

class Power: public TypedVariable<INT>
{
public:
	// Con-/Destructors
	Power(INT Watt=0);
	Power(Handle<String> Name, INT Watt=0);

	// Common
	Handle<String> ToString()override;
	static Handle<String> ToString(INT Watt);
};

}