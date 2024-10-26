//==========
// Enum.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "Enum.h"

using namespace Concurrency;
using namespace Storage::Streams;


//==================
// Con-/Destructors
//==================

Enum::Enum(Handle<String> name):
Variable(name)
{}


//========
// Access
//========

Handle<EnumIterator> Enum::First()
{
auto it=new EnumIterator(this);
it->First();
return it;
}

Handle<Sentence> Enum::Get()
{
SharedLock lock(cMutex);
return hValue;
}

Handle<String> Enum::ToString(LanguageCode lng)
{
SharedLock lock(cMutex);
return hValue->Begin(lng);
}

SIZE_T Enum::WriteToStream(OutputStream* stream)
{
SharedLock lock(cMutex);
SIZE_T size=0;
StreamWriter writer(stream);
size+=writer.Print(hValue->Begin(LanguageCode::None));
size+=writer.PrintChar('\0');
return size;
}


//==============
// Modification
//==============

VOID Enum::Add(Handle<Sentence> value)
{
ScopedLock lock(cMutex);
cEnum.set(value);
if(!hValue)
	hValue=value;
}

BOOL Enum::FromString(Handle<String> str, BOOL notify)
{
if(!str)
	return false;
for(auto it=cEnum.cbegin(); it.has_current(); it.move_next())
	{
	auto sentence=it.get_current();
	if(sentence->Compare(str->Begin())==0)
		return Set(sentence);
	}
return false;
}

SIZE_T Enum::ReadFromStream(InputStream* stream, BOOL notify)
{
SIZE_T size=0;
StreamReader reader(stream);
auto str=reader.ReadString(&size);
FromString(str, notify);
return size;
}

BOOL Enum::Set(Handle<Sentence> value, BOOL notify)
{
ScopedLock lock(cMutex);
if(!cEnum.contains(value))
	return false;
if(hValue==value)
	return true;
hValue=value;
lock.Unlock();
if(notify)
	Changed(this);
return true;
}


//===========================
// Iterator Con-/Destructors
//===========================

EnumIterator::EnumIterator(Handle<Enum> henum):
cIt(&henum->cEnum),
hEnum(henum)
{
hEnum->cMutex.Lock();
}

EnumIterator::~EnumIterator()
{
hEnum->cMutex.Unlock();
}
