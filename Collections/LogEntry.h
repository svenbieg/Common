//============
// LogEntry.h
//============

#pragma once


//=======
// Using
//=======

#include "Culture/Sentence.h"
#include "Timing/TimePoint.h"


//===========
// Namespace
//===========

namespace Collections {


//======================
// Forward-Declarations
//======================

class Log;


//===========
// Log-Entry
//===========

class LogEntry: public Object
{
public:
	// Friends
	friend Log;
	friend Object;

	// Using
	using InputStream=Storage::Streams::InputStream;
	using OutputStream=Storage::Streams::OutputStream;
	using Sentence=Culture::Sentence;
	using TimePoint=Timing::TimePoint;

	// Common
	SIZE_T ReadFromStream(InputStream* Stream);
	SIZE_T WriteToStream(OutputStream* Stream);
	Handle<Sentence> Message;
	Handle<TimePoint> Time;

private:
	// Con-/Destructors
	LogEntry();
	LogEntry(Handle<TimePoint> Time, Handle<Sentence> Message);
	static inline Handle<LogEntry> Create()
		{
		return Object::Create<LogEntry>();
		}
	static inline Handle<LogEntry> Create(Handle<TimePoint> Time, Handle<Sentence> Message)
		{
		return Object::Create<LogEntry>(Time, Message);
		}
};

}