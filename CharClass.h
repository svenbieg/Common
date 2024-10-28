//=============
// CharClass.h
//=============

#pragma once


//=======
// Using
//=======

#include "CharHelper.h"
#include "StringClass.h"
#include "Variable.h"


//======
// Char
//======

class Char: public TypedVariable<CHAR>
{
public:
	// Con-/Destructors
	Char(CHAR Value=0): TypedVariable(nullptr, Value) {}
	Char(WCHAR Value): TypedVariable(nullptr, CharToAnsi(Value)) {}
	Char(Handle<String> Name, CHAR Value=0): TypedVariable(Name, Value) {}
	Char(Handle<String> Name, WCHAR Value): TypedVariable(Name, CharToAnsi(Value)) {}

	// Access
	inline BOOL IsAlpha() { return CharIsAlpha(m_Value); }
	inline BOOL IsBreak() { return CharIsBreak(m_Value); }
	inline BOOL IsCapital() { return CharIsCapital(m_Value); }
	inline BOOL IsDigit(UINT Base=10) { return CharIsDigit(m_Value, Base); }
	inline BOOL IsSmall() { return CharIsSmall(m_Value); }
	inline BOOL IsSpecial() { return CharIsSpecial(m_Value); }
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override { return new String("%c", Get()); }
};
