//==============
// LogEntry.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Devices/Clock.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "LogEntry.h"

using namespace Devices;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Storage {
	namespace Collections {


//==================
// Con-/Destructors
//==================

LogEntry::LogEntry()
{}

LogEntry::LogEntry(Handle<TimePoint> htime, Handle<String> hmsg):
Message(hmsg),
Time(htime)
{
if(!Time)
	Time=Clock::Current->Now();
}


//========
// Common
//========

SIZE_T LogEntry::ReadFromStream(Handle<InputStream> hstream)
{
if(!hstream)
	return 0;
SIZE_T usize=0;
Time=new TimePoint();
usize+=Time->ReadFromStream(hstream);
if(Time->ToSeconds()==-1)
	return 0;
StreamReader reader(hstream);
Message=reader.ReadString('\377', &usize);
return usize;
}

SIZE_T LogEntry::WriteToStream(Handle<OutputStream> hstream)
{
if(!Message||!Time)
	return 0;
SIZE_T usize=0;
usize+=Time->WriteToStream(hstream);
StreamWriter writer(hstream);
usize+=writer.WriteString(Message);
return usize;
}

}}