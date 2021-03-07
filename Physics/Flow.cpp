//==========
// Flow.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Flow.h"


//===========
// Namespace
//===========

namespace Physics {


//==================
// Con-/Destructors
//==================

Flow::Flow(INT ml):
Flow(nullptr, ml)
{}

Flow::Flow(Handle<String> hname, INT ml):
TypedVariable(hname, ml)
{}


//========
// Access
//========

Handle<String> Flow::ToString()
{
ScopedLock lock(cCriticalSection);
return ToString(tValue, FlowUnits::MillisPerMin);
}

Handle<String> Flow::ToString(INT ml, FlowUnits unit)
{
if(ml==-1)
	return "-";
switch(unit)
	{
	case FlowUnits::LitersPerHour:
		return new String("%.2f l/h", (FLOAT)ml*3.6f);
	case FlowUnits::MillisPerMin:
		return new String("%i ml/min", ml*60);
	default:
	case FlowUnits::MillisPerSec:
		return new String("%i ml/s", ml);
	}
return nullptr;
}

}