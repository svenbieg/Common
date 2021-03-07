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

class Char: public TypedVariable<TCHAR>
{
public:
	// Con-/Destructors
	#ifdef UNICODE
	Char(CHAR Value): TypedVariable(nullptr, CharToUnicode(Value)) {}
	Char(WCHAR Value=0): TypedVariable(nullptr, Value) {}
	Char(Chars Char): TypedVariable(nullptr, CharToUnicode((CHAR)Char)) {}
	Char(Handle<String> Name, CHAR Value): TypedVariable(Name, CharToUnicode(Value)) {}
	Char(Handle<String> Name, WCHAR Value=0): TypedVariable(Name, Value) {}
	Char(Handle<String> Name, Chars Char): TypedVariable(Name, CharToUnicode((CHAR)Char)) {}
	#else
	Char(CHAR Value=0): TypedVariable(nullptr, Value) {}
	Char(WCHAR Value): TypedVariable(nullptr, CharToAnsi(Value)) {}
	Char(Chars Char): TypedVariable(nullptr, (CHAR)Char) {}
	Char(Handle<String> Name, CHAR Value=0): TypedVariable(Name, Value) {}
	Char(Handle<String> Name, WCHAR Value): TypedVariable(Name, CharToAnsi(Value)) {}
	Char(Handle<String> Name, Chars Char): TypedVariable(Name, (CHAR)Char) {}
	#endif

	// Access
	inline BOOL IsAlpha() { return CharIsAlpha(tValue); }
	inline BOOL IsBreak() { return CharIsBreak(tValue); }
	inline BOOL IsCapital() { return CharIsCapital(tValue); }
	inline BOOL IsHex() { return CharIsHex(tValue); }
	inline BOOL IsNumber() { return CharIsNumber(tValue); }
	inline BOOL IsSmall() { return CharIsSmall(tValue); }
	inline BOOL IsSpecial() { return CharIsSpecial(tValue); }
	Handle<String> ToString()override { return new String("%c", Get()); }
};
