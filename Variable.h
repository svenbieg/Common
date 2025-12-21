//============
// Variable.h
//============

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/Stream.h"
#include "Event.h"
#include "StringClass.h"


//==========
// Variable
//==========

class Variable: public Object
{
public:
	// Using
	using IInputStream=Storage::Streams::IInputStream;
	using IOutputStream=Storage::Streams::IOutputStream;

	// Access
	virtual Handle<String> GetName()const=0;
	virtual SIZE_T WriteToStream(IOutputStream* Stream)=0;

	// Common
	Event<Variable> Changed;
	virtual BOOL FromString(Handle<String> Value, BOOL Notify=true) { return false; }
	virtual SIZE_T ReadFromStream(IInputStream* Stream, BOOL Notify=true)=0;

protected:
	// Con-/Destructors
	Variable()=default;
};
