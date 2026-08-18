//============
// Variable.h
//============

#pragma once


//=======
// Using
//=======

#include "Culture/LanguageCode.h"
#include "Event.h"
#include "StringClass.h"


//==========
// Variable
//==========

class Variable: public Object
{
public:
	// Using
	using InputStream=Storage::Streams::InputStream;
	using LanguageCode=Culture::LanguageCode;
	using OutputStream=Storage::Streams::OutputStream;

	// Access
	virtual Handle<String> GetName()const=0;
	virtual SIZE_T WriteToStream(OutputStream* Stream)=0;

	// Common
	Event<Variable> Changed;
	virtual BOOL FromString(Handle<String> Value, BOOL Notify=true) { return false; }
	virtual SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify=true)=0;
	virtual Handle<String> ToString(LanguageCode Language=LanguageCode::None)=0;

protected:
	// Con-/Destructors
	Variable()=default;
};
