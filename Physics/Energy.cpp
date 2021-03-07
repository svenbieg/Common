//============
// Energy.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Energy.h"


//===========
// Namespace
//===========

namespace Physics {


//==================
// Con-/Destructors
//==================

Energy::Energy(INT wh):
Energy(nullptr, wh)
{}

Energy::Energy(Handle<String> hname, INT wh):
TypedVariable(hname, wh)
{}


//========
// Access
//========

Handle<String> Energy::ToString()
{
ScopedLock lock(cCriticalSection);
return ToString(tValue);
}

Handle<String> Energy::ToString(INT wh)
{
if(wh<0)
	return "-";
if(wh<1000)
	return new String("%iWh", wh);
return new String("%.2fkWh", (FLOAT)wh/1000.f);
}

}
