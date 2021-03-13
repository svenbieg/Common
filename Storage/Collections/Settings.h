//============
// Settings.h
//============

#pragma once


//=======
// Using
//=======

#include "Storage/Buffer.h"
#include "Collections/Map.h"
#include "Storage/Streams/OutputStream.h"
#include "Storage/File.h"
#include "Variable.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Collections {


//==========
// Settings
//==========

class Settings: public Object
{
private:
	// Using
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Con-/Destructors
	Settings(Handle<String> Path="Internal/Settings.dat");
	~Settings();

	// Common
	VOID Add(Handle<Variable> Variable);
	Event<Settings> Loaded;

private:
	// Using
	using SettingsMap=::Collections::Map<Handle<String>, Handle<Variable>>;

	// Common
	VOID Load();
	VOID OnVariableChanged(Handle<Variable> Variable);
	VOID Save();
	SIZE_T WriteVariable(Handle<OutputStream> Stream, Handle<Variable> Variable);
	BOOL bChanged;
	Handle<File> hFile;
	Handle<SettingsMap> hSettingsMap;
};

}}