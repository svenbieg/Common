//=================
// Temperature.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Temperature.h"


//===========
// Namespace
//===========

namespace Physics {


//==================
// Con-/Destructors
//==================

Temperature::Temperature(FLOAT degc):
Temperature(nullptr, degc)
{}

Temperature::Temperature(Handle<String> hname, FLOAT degc):
TypedVariable(hname, degc)
{}


//========
// Common
//========

Handle<String> Temperature::ToString()
{
ScopedLock lock(cCriticalSection);
if(tValue==-300.f)
	return "-";
return new String("%i%cC", (INT)tValue, 176);
}

Handle<String> Temperature::Unit=new String("%cC", 176);

}
