//=======
// Log.h
//=======

#pragma once


//=======
// Using
//=======

#include "Collections/List.h"
#include "Storage/Streams/Stream.h"
#include "LogEntry.h"


//===========
// Namespace
//===========

namespace Collections {


//=====
// Log
//=====

class Log: public Object
{
public:
	// Using
	using EntryList=List<Handle<LogEntry>>;
	using IInputStream=Storage::Streams::IInputStream;
	using IOutputStream=Storage::Streams::IOutputStream;
	using Sentence=Culture::Sentence;
	using TimePoint=Timing::TimePoint;

	// Con-/Destructors
	static inline Handle<Log> Create() { return new Log(); }

	// Common
	VOID Clear();
	Handle<EntryList> Entries;
	SIZE_T ReadFromStream(IInputStream* Stream);
	VOID Write(Handle<Sentence> Message) { Write(nullptr, Message); }
	VOID Write(Handle<TimePoint> TimePoint, Handle<Sentence> Message);
	SIZE_T WriteToStream(IOutputStream* Stream);
	Event<Log> Changed;

private:
	// Con-/Destructors
	Log();

	// Common
	VOID OnTimePointChanged(Variable* Sender);
};

}