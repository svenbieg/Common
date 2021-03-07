//==============
// Settings.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Runtime/Application.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "PathHelper.h"
#include "Settings.h"

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

Settings::Settings(Handle<String> hpath)
{
auto happ=Application::Current;
hFile=happ->AppData->CreateFile(hpath, FileCreateMode::OpenAlways, FileAccessMode::ReadWrite);
if(!hFile)
	return;
hSettingsMap=new SettingsMap();
happ->Loop.AddProc(this, &Settings::Load);
}

Settings::~Settings()
{
if(!hSettingsMap)
	return;
for(auto hit=hSettingsMap->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hvar=hit->GetCurrentItem();
	hvar->Changed.Remove(this);
	}
}


//========
// Common
//========

VOID Settings::Add(Handle<Variable> hvar)
{
if(!hvar)
	return;
if(!hSettingsMap)
	return;
hSettingsMap->Add(hvar->GetName(), hvar);
}


//================
// Common Private
//================

VOID Settings::Load()
{
StreamReader reader(hFile);
while(1)
	{
	UINT entry_size=0;
	SIZE_T read=reader.ReadUInt(&entry_size);
	if(!read)
		break;
	read=0;
	auto hname=reader.ReadString('\377', &read);
	if(hname)
		{
		auto hvar=hSettingsMap->Get(hname);
		if(hvar)
			read+=hvar->ReadFromStream(hFile);
		}
	if(read<entry_size)
		reader.Skip(entry_size-read);
	}
for(auto hit=hSettingsMap->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hvar=hit->GetCurrentItem();
	hvar->Changed.Add(this, &Settings::OnVariableChanged);
	}
Loaded(this);
}

VOID Settings::OnVariableChanged(Handle<Variable> hvar)
{
UINT64 size=hFile->GetSize();
SIZE_T entry_size=WriteVariable(nullptr, hvar);
if(size+entry_size<=2048)
	{
	WriteVariable(hFile, hvar);
	hFile->Flush();
	return;
	}
hFile->SetSize(0);
for(auto hit=hSettingsMap->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hvar=hit->GetCurrentItem();
	WriteVariable(hFile, hvar);
	}
hFile->Flush();
}

SIZE_T Settings::WriteVariable(Handle<OutputStream> hstream, Handle<Variable> hvar)
{
StreamWriter writer(nullptr);
UINT entry_size=0;
entry_size+=(UINT)writer.WriteString(hvar->GetName());
entry_size+=(UINT)hvar->WriteToStream(nullptr);
if(!hstream)
	return sizeof(UINT)+entry_size;
writer.SetStream(hstream);
SIZE_T size=0;
size+=writer.WriteUInt(entry_size);
size+=writer.WriteString(hvar->GetName());
size+=hvar->WriteToStream(hstream);
return size;
}

}}