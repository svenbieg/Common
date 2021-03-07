//==============
// TimeSpan.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "TimeSpan.h"


//===========
// Namespace
//===========

namespace Physics {


//==================
// Con-/Destructors
//==================

TimeSpan::TimeSpan(UINT usec):
TimeSpan(nullptr, usec)
{}

TimeSpan::TimeSpan(Handle<String> hname, UINT usec):
TypedVariable(hname, usec)
{}


//========
// Access
//========

Handle<String> TimeSpan::ToString()
{
ScopedLock lock(cCriticalSection);
return ToString(tValue);
}

Handle<String> TimeSpan::ToString(UINT us)
{
TCHAR pstr[16];
ToString(us, pstr, 16);
return pstr;
}

UINT TimeSpan::ToString(UINT us, LPTSTR pstr, UINT usize)
{
UINT udays=us/24/60/60;
us-=udays*24*60*60;
UINT uhours=us/60/60;
us-=uhours*60*60;
UINT umins=us/60;
us-=umins*60;
if(udays>0)
	return StringPrint(pstr, usize, "%ud %uh", udays, uhours);
if(uhours>0)
	return StringPrint(pstr, usize, "%u:%02uh", uhours, umins);
if(umins>0)
	return StringPrint(pstr, usize, "%um", umins);
return StringPrint(pstr, usize, "%us", us);
}

}