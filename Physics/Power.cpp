//===========
// Power.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Power.h"


//===========
// Namespace
//===========

namespace Physics {


//==================
// Con-/Destructors
//==================

Power::Power(INT w):
Power(nullptr, w)
{}

Power::Power(Handle<String> hname, INT w):
TypedVariable(hname, w)
{}


//========
// Access
//========

Handle<String> Power::ToString()
{
ScopedLock lock(cCriticalSection);
return ToString(tValue);
}

Handle<String> Power::ToString(INT w)
{
if(w<0)
	return "-";
if(w<1000)
	return new String("%iW", w);
return new String("%.2fkW", (FLOAT)w/1000.f);
}

}
