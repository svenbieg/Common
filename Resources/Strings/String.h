//==========
// String.h
//==========

#pragma once


//=======
// Using
//=======

#include "Culture/LanguageCode.h"


//===========
// Namespace
//===========

namespace Resources {
	namespace Strings {


//==========
// Language
//==========

typedef Culture::LanguageCode LNG;


//========
// String
//========

typedef struct
{
LNG Language;
LPCWSTR Value;
}STRING;

}}