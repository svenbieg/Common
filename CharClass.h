//=============
// CharClass.h
//=============

#pragma once


//=======
// Using
//=======

#include "CharHelper.h"
#include "Variable.h"


//======
// Char
//======

class Char: public Variable
{
public:
	// Con-/Destructors
	static inline Handle<Char> Create(Handle<String> Name, CHAR Value) { return new Char(Name, Value); }
	static inline Handle<Char> Create(Handle<String> Name, WCHAR Value) { return new Char(Name, Value); }

	// Access
	WCHAR Get();
	inline Handle<String> GetName()const override { return m_Name; }
	inline BOOL IsAlpha() { return CharHelper::IsAlpha(Get()); }
	inline BOOL IsBreak() { return CharHelper::IsBreak(Get()); }
	inline BOOL IsCapital() { return CharHelper::IsCapital(Get()); }
	inline BOOL IsDigit(UINT Base=10) { return CharHelper::IsDigit(Get(), Base); }
	inline BOOL IsSmall() { return CharHelper::IsSmall(Get()); }
	inline BOOL IsSpecial() { return CharHelper::IsSpecial(Get()); }
	Event<Variable, WCHAR&> Reading;
	Handle<String> ToString(LanguageCode Language)override { return String::Create("%c", Get()); }
	SIZE_T WriteToStream(IOutputStream* Stream)override;

	// Modification
	SIZE_T ReadFromStream(IInputStream* Stream, BOOL Notify=true)override;
	VOID Set(WCHAR Value, BOOL Notify=true);

private:
	// Con-/Destructors
	Char(Handle<String> Name, CHAR Value);
	Char(Handle<String> Name, WCHAR Value);

	// Common
	Handle<String> m_Name;
	WCHAR m_Value;
};
