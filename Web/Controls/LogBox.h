//==========
// LogBox.h
//==========

#pragma once


//=======
// Using
//=======

#include "Storage/Collections/Log.h"
#include "Button.h"
#include "Container.h"


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//=========
// Log-Box
//=========

class LogBox: public Control
{
private:
	// Using
	using Log=Storage::Collections::Log;

public:
	// Con-/Destructors
	LogBox(Handle<String> Id, Handle<Log> Log): LogBox(Container::Current, Id, Log) {}
	LogBox(Handle<Container> Parent, Handle<String> Id, Handle<Log> Log);

	// Common
	Handle<String> Id;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
	SIZE_T WriteUpdateToStream(Handle<OutputStream> Stream, UINT LastTime)override;

private:
	// Common
	VOID OnClearButtonClick(Handle<Button> Button);
	VOID OnLogChanged(Handle<Log> Log);
	Handle<Button> hClearButton;
	Handle<Log> hLog;
	UINT uTimeChanged;
};

}}
