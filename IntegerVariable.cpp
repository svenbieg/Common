//=====================
// IntegerVariable.cpp
//=====================

#include "IntegerVariable.h"


//========
// Access
//========

INT IntegerVariable::Get()
{
INT value=m_Value;
Reading(this, value);
return value;
}

SIZE_T IntegerVariable::WriteToStream(OutputStream* Stream)
{
if(!Stream)
	return sizeof(INT);
INT value=Get();
return Stream->Write(&value, sizeof(INT));
}


//==============
// Modification
//==============

BOOL IntegerVariable::FromString(Handle<String> str, BOOL notify)
{
INT value;
if(FromString(str, &value))
	{
	Set(value, notify);
	return true;
	}
return false;
}

BOOL IntegerVariable::FromString(Handle<String> str, INT* value_ptr)
{
if(!str)
	return false;
INT value;
if(str->Scan("%i", &value)!=1)
	return false;
if(value_ptr)
	*value_ptr=value;
return true;
}

SIZE_T IntegerVariable::ReadFromStream(InputStream* stream, BOOL notify)
{
if(!stream)
	return sizeof(INT);
INT value;
SIZE_T size=stream->Read(&value, sizeof(INT));
if(size==sizeof(INT))
	Set(value, notify);
return size;
}

BOOL IntegerVariable::Set(INT value, BOOL notify)
{
if(m_Value==value)
	return false;
m_Value=value;
if(notify)
	Changed(this);
return true;
}
