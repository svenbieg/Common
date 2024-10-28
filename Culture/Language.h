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
Unknown
};

LPCSTR LanguageCodeToString(LanguageCode Language);
LanguageCode LanguageCodeFromString(LPCSTR Language);
LanguageCode LanguageCodeFromString(LPCWSTR Language);


//=========
// Globals
//=========

extern LanguageCode CurrentLanguage;

}
