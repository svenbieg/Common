//============
// Variable.h
//============

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/InputStream.h"
#include "Storage/Streams/OutputStream.h"
#include "Event.h"
#include "ScopedLock.h"
#include "StringClass.h"
#include "VariableHelper.h"


//==========
// Variable
//==========

class Variable: public Object
{
protected:
	// Using
	using InputStream=Storage::Streams::InputStream;
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Access
	Handle<String> GetName()const { return hName; }
	virtual Handle<String> ToString() { return nullptr; }
	virtual SIZE_T WriteToStream(Handle<OutputStream> Stream) { return 0; }

	// Modification
	Event<Variable> Changed;
	virtual BOOL FromString(Handle<String> Value) { return false; }
	virtual SIZE_T ReadFromStream(Handle<InputStream> Stream) { return 0; }

protected:
	// Con-/Destructors
	Variable(Handle<String> Name): hName(Name) {}

	// Common
	CriticalSection cCriticalSection;
	Handle<String> hName;
};


//================
// Typed Variable
//================

template <class _value_t>
class TypedVariable: public Variable
{
public:
	// Access
	_value_t Get()
		{
		ScopedLock lock(cCriticalSection);
		_value_t tvalue=tValue;
		lock.Release();
		Reading(this, tvalue);
		return tvalue;
		}
	Event<Variable, _value_t&> Reading;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override
		{
		if(!Stream)
			return sizeof(_value_t);
		ScopedLock lock(cCriticalSection);
		_value_t value=tValue;
		lock.Release();
		Reading(this, value);
		return Stream->Write(&value, sizeof(_value_t));
		}

	// Modification
	SIZE_T ReadFromStream(Handle<InputStream> Stream)override
		{
		if(!Stream)
			return sizeof(_value_t);
		_value_t value;
		SIZE_T usize=Stream->Read(&value, sizeof(_value_t));
		if(usize==sizeof(_value_t))
			Set(value);
		return usize;
		}
	virtual VOID Set(_value_t const& Value)
		{
		ScopedLock lock(cCriticalSection);
		if(tValue==Value)
			return;
		tValue=Value;
		lock.Release();
		Changed(this);
		}

protected:
	// Con-/Destructors
	TypedVariable(Handle<String> Name, _value_t Value): Variable(Name), tValue(Value) {}
	
	// Common
	_value_t tValue;
};
