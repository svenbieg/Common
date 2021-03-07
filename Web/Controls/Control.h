//===========
// Control.h
//===========

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/OutputStream.h"


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//======================
// Forward-Declarations
//======================

class Container;


//=========
// Control
//=========

class Control: public Object
{
protected:
	// Using
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Common
	VOID Break(UINT Count=1) { uBreaks=Count; }
	Handle<String> Class;
	virtual VOID OnNotified(Handle<String> Params) {}
	Handle<Container> Parent;
	Handle<String> Style;
	virtual SIZE_T WriteScriptToStream(Handle<OutputStream> Stream) { return 0; }
	virtual SIZE_T WriteToStream(Handle<OutputStream> Stream);
	virtual SIZE_T WriteUpdateToStream(Handle<OutputStream> Stream, UINT LastTime) { return 0; }

protected:
	// Con-/Destructors
	Control();
	Control(Handle<Container> Parent, Handle<String> Class=nullptr, Handle<String> Style=nullptr);

	// Common
	SIZE_T WriteBreaks(Handle<OutputStream> Stream);
	SIZE_T WriteControlAttributes(Handle<OutputStream> Stream);
	UINT uBreaks;
};

}}
