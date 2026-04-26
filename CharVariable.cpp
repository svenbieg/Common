//==================
// CharVariable.cpp
//==================

#include "CharVariable.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"

using namespace Storage::Streams;


//========
// Access
//========

WCHAR CharVariable::Get()
{
WCHAR value=m_Value;
Reading(this, value);
return value;
}

SIZE_T CharVariable::WriteToStream(OutputStream* stream)
{
WCHAR value=Get();
StreamWriter writer(stream);
return writer.PrintChar(value);
}


//==============
// Modification
//==============

SIZE_T CharVariable::ReadFromStream(InputStream* stream, BOOL notify)
{
StreamReader reader(stream);
WCHAR value=0;
SIZE_T size=reader.ReadChar(&value);
if(size)
	Set(value, notify);
return size;
}

VOID CharVariable::Set(WCHAR value, BOOL notify)
{
if(m_Value==value)
	return;
m_Value=value;
if(notify)
	Changed(this);
}


//==========================
// Con-/Destructors Private
//==========================

CharVariable::CharVariable(Handle<String> name, CHAR value):
m_Name(name),
m_Value(CharHelper::ToUnicode(value))
{}

CharVariable::CharVariable(Handle<String> name, WCHAR value):
m_Name(name),
m_Value(value)
{}
