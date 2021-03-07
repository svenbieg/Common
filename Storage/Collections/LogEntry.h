//============
// LogEntry.h
//============

#pragma once


//=======
// Using
//=======

#include "Collections/ListItem.h"
#include "Physics/TimePoint.h"
#include "Storage/Streams/InputStream.h"
#include "Storage/Streams/OutputStream.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Collections {


//===========
// Log-Entry
//===========

class LogEntry: public ::Collections::ListItem<LogEntry>
{
private:
	// Using
	using InputStream=::Storage::Streams::InputStream;
	using OutputStream=::Storage::Streams::OutputStream;
	using TimePoint=Physics::TimePoint;

public:
	// Con-/Destructors
	LogEntry();
	LogEntry(Handle<TimePoint> Time, Handle<String> Message);

	// Common
	SIZE_T ReadFromStream(Handle<InputStream> Stream);
	SIZE_T WriteToStream(Handle<OutputStream> Stream);
	Handle<String> Message;
	Handle<TimePoint> Time;
};

}}