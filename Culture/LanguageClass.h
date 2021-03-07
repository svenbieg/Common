//=================
// LanguageClass.h
//=================

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//===========
// Namespace
//===========

namespace Culture {


//===============
// Language-Code
//===============

enum class LanguageCode: UINT
{
DE,
US,
None
};


//==========
// Language
//==========

class Language: public TypedVariable<LanguageCode>
{
public:
	// Con-/Destructors
	Language(LanguageCode Language=LanguageCode::DE);
	Language(Handle<String> Name, LanguageCode Language=LanguageCode::DE);

	// Access
	static LanguageCode Current;
	Handle<String> ToString()override;
	static Handle<String> ToString(LanguageCode Code, LanguageCode Language=LanguageCode::None);
	static UINT ToString(LanguageCode Code, LPTSTR Buffer, UINT Size, LanguageCode Language=LanguageCode::None);
};

}
