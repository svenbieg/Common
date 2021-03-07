//=================
// Convertible.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Convertible.h"


//========
// Common
//========

BOOL Convertible::HasType(Handle<String> htype)
{
return hType->Contains(htype);
}


//============================
// Con-/Destructors Protected
//============================

Convertible::Convertible(Handle<String> htype):
hType(htype)
{}
