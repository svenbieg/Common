//========
// Flow.h
//========

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//===========
// Namespace
//===========

namespace Physics {


//===========
// Flow-Unit
//===========

enum class FlowUnits
{
LitersPerHour,
MillisPerMin,
MillisPerSec
};


//======
// Flow
//======

class Flow: public TypedVariable<INT>
{
public:
	// Con-/Destructors
	Flow(INT MillisPerSec=0);
	Flow(Handle<String> Name, INT MillisPerSec=0);

	// Access
	Handle<String> ToString()override;
	static Handle<String> ToString(INT MillisPerSec, FlowUnits Unit);
};

}
