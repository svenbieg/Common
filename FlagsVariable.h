//=================
// FlagsVariable.h
//=================

#pragma once


//=======
// Using
//=======

#include "Collections/index.hpp"
#include "Variable.h"


//================
// Flags-Variable
//================

class FlagsVariable: public Variable
{
public:
	// Con-/Destructors
	static inline Handle<FlagsVariable> Create(Handle<String> Name) { return new FlagsVariable(Name); }

	// Access
	inline BOOL Get(Handle<String> Flag) { return m_Flags.contains(Flag); }
	inline Handle<String> GetName()const override { return m_Name; }
	Handle<String> ToString(LanguageCode Language=LanguageCode::None)override;
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Modification
	VOID Clear(BOOL Notify=true);
	VOID Clear(Handle<String> Flag, BOOL Notify=true);
	BOOL FromString(Handle<String> Value, BOOL Notify=true)override;
	SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify=true)override;
	VOID Set(Handle<String> Flag, BOOL Notify=true);

private:
	// Con-/Destructors
	FlagsVariable(Handle<String> Name): m_Name(Name) {}

	// Common
	Collections::index<Handle<String>> m_Flags;
	Handle<String> m_Name;
};
