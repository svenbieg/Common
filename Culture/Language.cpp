//==============
// Language.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Resources/Strings/Language.h"
#include "Language.h"
#include "Sentence.h"

using namespace Resources::Strings;


//===========
// Namespace
//===========

namespace Culture {


//===============
// Language-Code
//===============

LPCSTR LanguageCodeToString(LanguageCode lng)
{
return Translate(STR_LANGUAGE_CODE, lng);
}

template <class _char_t> LanguageCode LanguageCodeFromString(_char_t const* lng)
{
auto string=STR_LANGUAGE_CODE;
while(1)
	{
	if(StringCompare(string->Value, lng, 2, false)==0)
		return string->Language;
	if(string->Language==LNG::None)
		break;
	string++;
	}
return LanguageCode::Unknown;
}

LanguageCode LanguageCodeFromString(LPCSTR lng)
{
return LanguageCodeFromString<CHAR>(lng);
}

LanguageCode LanguageCodeFromString(LPCWSTR lng)
{
return LanguageCodeFromString<WCHAR>(lng);
}


//=========
// Globals
//=========

LanguageCode CurrentLanguage=LanguageCode::None;


//==================
// Con-/Destructors
//==================

Language::Language(Handle<String> name, LanguageCode lng):
TypedVariable(name, lng)
{}


//========
// Common
//========

typedef struct
{
LNG Language;
STRING const* String;
}TRANSLATION;

constexpr TRANSLATION TRANS_LANGUAGE[]=
	{
	{ LNG::DE, STR_GERMAN },
	{ LNG::EN, STR_ENGLISH },
	{ LNG::None, nullptr }
	};

Handle<String> Language::ToString(LanguageCode lng_str)
{
auto lng=Get();
TRANSLATION const* trans=TRANS_LANGUAGE;
while(trans->Language!=LNG::None)
	{
	if(trans->Language==lng)
		return Translate(trans->String, lng_str);
	trans++;
	}
return "STR_LANGUAGE";
}

}