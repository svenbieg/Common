//============
// FileSize.h
//============

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//===========
// Namespace
//===========

namespace Storage {


//===========
// File-Size
//===========

class FileSize: public Variable
{
public:
	// Con-/Destructors
	static inline Handle<FileSize> Create(UINT64 Value=0)
		{
		return new FileSize(nullptr, Value);
		}
	static inline Handle<FileSize> Create(Handle<String> Name, UINT64 Value=0)
		{
		return new FileSize(Name, Value);
		}

	// Access
	inline UINT64 Get()const { return m_Value; }
	static inline UINT64 Get(FileSize* Value) { return Value? Value->Get(): 0; }
	inline Handle<String> GetName()const override { return m_Name; }
	inline Handle<String> ToString(LanguageCode Language=LanguageCode::None)override { return ToString(Get()); }
	static Handle<String> ToString(UINT64 Size);
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Modification
	SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify=true)override;
	BOOL Set(UINT64 Value, BOOL Notify=true);

private:
	// Con-/Destructors
	FileSize(Handle<String> Name, UINT64 Value): m_Name(Name), m_Value(Value) {}

	// Common
	Handle<String> m_Name;
	UINT64 m_Value;
};

}