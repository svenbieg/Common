//=========
// Log.cpp
//=========

#include "pch.h"


//=======
// Using
//=======

#include "Runtime/Application.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "Log.h"
#include "PathHelper.h"

using namespace Runtime;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Storage {
	namespace Collections {


//==================
// Con-/Destructors
//==================

Log::Log(Handle<String> hpath):
bChanged(false)
{
auto happ=Application::Current;
hFile=happ->AppData->CreateFile(hpath, FileCreateMode::OpenAlways, FileAccessMode::ReadWrite);
if(!hFile)
	return;
StreamReader reader(hFile);
while(1)
	{
	hLastEntry=new LogEntry();
	SIZE_T entry_size=hLastEntry->ReadFromStream(hFile);
	if(!entry_size)
		break;
	Append(hLastEntry);
	}
Changed.Add(this, &Log::OnChanged);
}


//========
// Common
//========

VOID Log::Clear()
{
List::Clear();
if(hFile)
	hFile->SetSize(0);
hLastEntry=nullptr;
Changed(this);
}

VOID Log::Write(Handle<TimePoint> htime, Handle<String> hmsg)
{
Handle<LogEntry> hentry=new LogEntry(htime, hmsg);
if(!hentry->Time->IsAbsolute())
	hentry->Time->Changed.Add(this, &Log::OnTimePointChanged);
Append(hentry);
Changed(this);
}


//================
// Common Private
//================

VOID Log::OnChanged()
{
if(bChanged)
	return;
if(!hFile)
	return;
bChanged=true;
Application::Current->Loop.AddProc(this, &Log::OnLoop);
}

VOID Log::OnLoop()
{
bChanged=false;
Handle<LogEntry> hentry=GetFirst();
if(hLastEntry)
	hentry=hLastEntry->GetNext();
UINT64 size=hFile->GetSize();
SIZE_T entry_size=WriteEntries(nullptr, hentry);
if(size+entry_size>2048)
	{
	hFile->SetSize(0);
	hentry=GetFirst();
	while(size+entry_size>2048)
		{
		for(UINT u=0; u<5; u++)
			{
			Remove(hentry);
			hentry=GetFirst();
			}
		entry_size=WriteEntries(nullptr, hentry);
		}
	}
WriteEntries(hFile, hentry);
hFile->Flush();
hLastEntry=GetLast();
}

VOID Log::OnTimePointChanged(Handle<Variable> hvar)
{
hvar->Changed.Remove(this);
Changed(this);
}

SIZE_T Log::WriteEntries(Handle<OutputStream> hstream, Handle<LogEntry> hentry)
{
SIZE_T size=0;
while(hentry)
	{
	if(!hentry||!hentry->Time->IsAbsolute())
		break;
	UINT entry_size=(UINT)hentry->WriteToStream(nullptr);
	if(hstream)
		{
		size+=hstream->Write(&entry_size, sizeof(UINT));
		size+=hentry->WriteToStream(hstream);
		}
	else
		{
		size+=sizeof(UINT)+entry_size;
		}
	hentry=hentry->GetNext();
	}
return size;
}

}}