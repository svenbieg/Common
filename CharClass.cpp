//===============
// CharClass.cpp
//===============

#include "CharClass.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"

using namespace Storage::Streams;


//========
// Access
//========

WCHAR Char::Get()
{
WCHAR value=m_Value;
Reading(this, value);
return value;
}

SIZE_T Char::WriteToStream(IOutputStream* stream)
{
WCHAR value=Get();
StreamWriter writer(stream);
return writer.PrintChar(value);
}


//==============
// Modification
//==============

SIZE_T Char::ReadFromStream(IInputStream* stream, BOOL notify)
{
StreamReader reader(stream);
WCHAR value=0;
SIZE_T size=reader.ReadChar(&value);
if(size)
	Set(value, notify);
return size;
}

VOID Char::Set(WCHAR value, BOOL notify)
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

Char::Char(Handle<String> name, CHAR value):
m_Name(name),
m_Value(CharHelper::ToUnicode(value))
{}

Char::Char(Handle<String> name, WCHAR value):
m_Name(name),
m_Value(value)
{}
