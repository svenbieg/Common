//====================
// StringVariable.cpp
//====================

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "StringVariable.h"

using namespace Storage::Streams;


//==================
// Con-/Destructors
//==================

StringVariable::StringVariable(Handle<String> name, Handle<String> value):
Variable(name),
hValue(value)
{}


//========
// Access
//========

Handle<String> StringVariable::Get()
{
SharedLock lock(cMutex);
Handle<String> hvalue=hValue;
lock.Unlock();
Reading(this, hvalue);
return hvalue;
}

SIZE_T StringVariable::WriteToStream(OutputStream* stream)
{
SharedLock lock(cMutex);
Handle<String> value=hValue;
lock.Unlock();
Reading(this, value);
SIZE_T size=0;
StreamWriter writer(stream);
size+=writer.Print(value);
size+=writer.PrintChar('\0');
return size;
}


//==============
// Modification
//==============

BOOL StringVariable::FromString(Handle<String> value, BOOL notify)
{
return Set(value, notify);
}

SIZE_T StringVariable::ReadFromStream(InputStream* stream, BOOL notify)
{
if(!stream)
	return 0;
SIZE_T size=0;
StreamReader reader(stream);
auto str=reader.ReadString();
Set(str, notify);
return size;
}

BOOL StringVariable::Set(Handle<String> value, BOOL notify)
{
UniqueLock lock(cMutex);
if(hValue==value)
	return true;
hValue=value;
lock.Unlock();
if(notify)
	Changed(this);
return true;
}
