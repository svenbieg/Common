//===========
// Flags.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Collections/StringList.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "Flags.h"

using namespace Collections;
using namespace Storage::Streams;


//==================
// Con-/Destructors
//==================

Flags::Flags(Handle<String> name):
Variable(name)
{}


//========
// Access
//========

BOOL Flags::Get(Handle<String> flag)
{
SharedLock lock(cMutex);
return cFlags.contains(flag);
}

Handle<String> Flags::ToString(LanguageCode lng)
{
SharedLock lock(cMutex);
Handle<String> flags;
UINT count=0;
for(auto it=cFlags.cbegin(); it.has_current(); it.move_next())
	{
	if(count>0)
		flags=flags+"|";
	auto flag=it.get_current();
	flags=flags+flag;
	count++;
	}
return flags;
}

SIZE_T Flags::WriteToStream(OutputStream* stream)
{
auto flags=ToString();
SIZE_T size=0;
StreamWriter writer(stream);
size+=writer.Print(flags);
size+=writer.PrintChar('\0');
return size;
}


//==============
// Modification
//==============

VOID Flags::Clear(BOOL notify)
{
UniqueLock lock(cMutex);
if(cFlags.get_count()==0)
	return;
cFlags.clear();
if(notify)
	Changed(this);
}

VOID Flags::Clear(Handle<String> flag, BOOL notify)
{
UniqueLock lock(cMutex);
if(!cFlags.contains(flag))
	return;
cFlags.remove(flag);
if(notify)
	Changed(this);
}

BOOL Flags::FromString(Handle<String> str, BOOL notify)
{
if(!str)
	return false;
if(str=="0")
	{
	Clear(notify);
	return true;
	}
UniqueLock lock(cMutex);
Handle<StringList> list=new StringList(str);
BOOL changed=false;
for(auto it=list->First(); it->HasCurrent(); it->MoveNext())
	{
	auto flag=it->GetCurrent();
	auto p=flag->Begin();
	if(CharCompare(p[0], '!')==0)
		{
		Handle<String> clear=new String(&p[1]);
		if(cFlags.contains(clear))
			{
			cFlags.remove(clear);
			changed=true;
			}
		}
	else
		{
		if(!cFlags.contains(flag))
			{
			cFlags.set(flag);
			changed=true;
			}
		}
	}
lock.Unlock();
if(changed&&notify)
	Changed(this);
return true;
}

SIZE_T Flags::ReadFromStream(InputStream* stream, BOOL notify)
{
SIZE_T size=0;
StreamReader reader(stream);
auto str=reader.ReadString(&size);
FromString(str, notify);
return size;
}

VOID Flags::Set(Handle<String> flag, BOOL notify)
{
UniqueLock lock(cMutex);
if(cFlags.contains(flag))
	return;
cFlags.set(flag);
lock.Unlock();
if(notify)
	Changed(this);
}
