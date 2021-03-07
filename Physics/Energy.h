//==========
// Energy.h
//==========

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//===========
// Namespace
//===========

namespace Physics {


//========
// Energy
//========

class Energy: public TypedVariable<INT>
{
public:
	// Con-/Destructors
	Energy(INT Wh=0);
	Energy(Handle<String> Name, INT Wh=0);

	// Common
	Handle<String> ToString()override;
	static Handle<String> ToString(INT Wh);
};

}