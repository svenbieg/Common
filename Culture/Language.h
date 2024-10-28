//============
// Language.h
//============

#pragma once


//===========
// Namespace
//===========

namespace Culture {


//===============
// Language-Code
//===============

enum class LanguageCode: UINT
{
None,
DE,
EN,
Unknown=-1
};

LPCSTR LanguageCodeToString(LanguageCode Language);
LanguageCode LanguageCodeFromString(LPCSTR Language);
LanguageCode LanguageCodeFromString(LPCWSTR Language);


//=========
// Globals
//=========

extern LanguageCode CurrentLanguage;


//==========
// Language
//==========

class Language: public TypedVariable<LanguageCode>
{
public:
	// Con-/Destructors
	Language(Handle<String> Name, LanguageCode Language);

	// Common
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override;
};

}
