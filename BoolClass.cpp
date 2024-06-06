//===============
// BoolClass.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "BoolClass.h"


//==================
// Con-/Destructors
//==================

Bool::Bool(BOOL value):
Bool(nullptr, value)
{}

Bool::Bool(Handle<String> name, BOOL value):
TypedVariable(name, value)
{}


//==============
// Modification
//==============

BOOL Bool::FromString(Handle<String> value, BOOL notify)
{
BOOL b=false;
if(FromString(value, &b))
	{
	Set(b, notify);
	return true;
	}
return false;
}

BOOL Bool::FromString(Handle<String> value, BOOL* ptr)
{
if(!value)
	return false;
if(StringCompare(value->Begin(), "0", false)==0)
	{
	*ptr=false;
	return true;
	}
if(StringCompare(value->Begin(), "1", false)==0)
	{
	*ptr=true;
	return true;
	}
if(StringCompare(value->Begin(), "FALSE", false)==0)
	{
	*ptr=false;
	return true;
	}
if(StringCompare(value->Begin(), "TRUE", false)==0)
	{
	*ptr=true;
	return true;
	}
if(StringCompare(value->Begin(), "NO", false)==0)
	{
	*ptr=false;
	return true;
	}
if(StringCompare(value->Begin(), "YES", false)==0)
	{
	*ptr=true;
	return true;
	}
if(StringCompare(value->Begin(), "NEIN", false)==0)
	{
	*ptr=false;
	return true;
	}
if(StringCompare(value->Begin(), "JA", false)==0)
	{
	*ptr=true;
	return true;
	}
return false;
}
