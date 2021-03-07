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

Bool::Bool(BOOL bvalue):
Bool(nullptr, bvalue)
{}

Bool::Bool(Handle<String> hname, BOOL bvalue):
TypedVariable(hname, bvalue)
{}


//==============
// Modification
//==============

BOOL Bool::FromString(Handle<String> hvalue)
{
BOOL b=false;
if(FromString(hvalue, b))
	{
	Set(b);
	return true;
	}
return false;
}

BOOL Bool::FromString(Handle<String> hvalue, BOOL& b)
{
if(!hvalue)
	return false;
if(StringCompare(hvalue->Begin(), "0", 0, false)==0)
	{
	b=false;
	return true;
	}
if(StringCompare(hvalue->Begin(), "1", 0, false)==0)
	{
	b=true;
	return true;
	}
if(StringCompare(hvalue->Begin(), "FALSE", 0, false)==0)
	{
	b=false;
	return true;
	}
if(StringCompare(hvalue->Begin(), "TRUE", 0, false)==0)
	{
	b=true;
	return true;
	}
if(StringCompare(hvalue->Begin(), "NO", 0, false)==0)
	{
	b=false;
	return true;
	}
if(StringCompare(hvalue->Begin(), "YES", 0, false)==0)
	{
	b=true;
	return true;
	}
if(StringCompare(hvalue->Begin(), "NEIN", 0, false)==0)
	{
	b=false;
	return true;
	}
if(StringCompare(hvalue->Begin(), "JA", 0, false)==0)
	{
	b=true;
	return true;
	}
return false;
}
