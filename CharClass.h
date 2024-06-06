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
	inline BOOL IsAlpha() { return CharIsAlpha(tValue); }
	inline BOOL IsBreak() { return CharIsBreak(tValue); }
	inline BOOL IsCapital() { return CharIsCapital(tValue); }
	inline BOOL IsDigit(UINT Base=10) { return CharIsDigit(tValue, Base); }
	inline BOOL IsSmall() { return CharIsSmall(tValue); }
	inline BOOL IsSpecial() { return CharIsSpecial(tValue); }
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override { return new String("%c", Get()); }
};
