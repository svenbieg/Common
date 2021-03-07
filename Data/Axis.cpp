//==========
// Axis.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Axis.h"


//===========
// Namespace
//===========

namespace Data {


//==================
// Con-/Destructors
//==================

Axis::Axis(Handle<String> hname, Handle<String> hunit):
Name(hname),
Unit(hunit)
{
Points=new PointMap();
}

}