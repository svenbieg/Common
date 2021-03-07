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

StringVariable::StringVariable(Handle<String> hname, Handle<String> hvalue):
Variable(hname),
hValue(hvalue)
{}


//========
// Access
//========

Handle<String> StringVariable::Get()
{
ScopedLock lock(cCriticalSection);
Handle<String> hvalue=hValue;
lock.Release();
Reading(this, hvalue);
return hvalue;
}

SIZE_T StringVariable::WriteToStream(Handle<OutputStream> hstream)
{
ScopedLock lock(cCriticalSection);
Handle<String> hvalue=hValue;
lock.Release();
Reading(this, hvalue);
StreamWriter writer(hstream);
return writer.WriteString(hvalue);
}


//==============
// Modification
//==============

BOOL StringVariable::FromString(Handle<String> hvalue)
{
Set(hvalue);
return true;
}

SIZE_T StringVariable::ReadFromStream(Handle<InputStream> hstream)
{
StreamReader reader(hstream);
SIZE_T usize=0;
auto hstr=reader.ReadString('\000', &usize);
Set(hstr);
return usize;
}

BOOL StringVariable::Set(Handle<String> hvalue)
{
ScopedLock lock(cCriticalSection);
if(hValue==hvalue)
	return true;
hValue=hvalue;
lock.Release();
Changed(this);
return true;
}
