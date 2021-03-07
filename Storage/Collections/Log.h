//=======
// Log.h
//=======

#pragma once


//=======
// Using
//=======

#include "Collections/List.h"
#include "Storage/Streams/OutputStream.h"
#include "Storage/File.h"
#include "LogEntry.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Collections {


//=====
// Log
//=====

class Log: public ::Collections::List<LogEntry>
{
private:
	// Using
	using OutputStream=Storage::Streams::OutputStream;
	using TimePoint=Physics::TimePoint;

public:
	// Con-/Destructors
	Log(Handle<String> Path="Internal/Log.dat");

	// Common
	VOID Clear()override;
	VOID Write(Handle<String> Message) { Write(nullptr, Message); }
	VOID Write(Handle<TimePoint> TimePoint, Handle<String> Message);
	Event<Log> Changed;

private:
	// Common
	VOID OnChanged();
	VOID OnLoop();
	VOID OnTimePointChanged(Handle<Variable> TimePoint);
	SIZE_T WriteEntries(Handle<OutputStream> Stream, Handle<LogEntry> FirstEntry);
	BOOL bChanged;
	Handle<File> hFile;
	Handle<LogEntry> hLastEntry;
};

}}