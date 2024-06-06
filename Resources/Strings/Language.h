//============
// Language.h
//============

#pragma once


//=======
// Using
//=======

#include "Resources/Strings/String.h"


//===========
// Namespace
//===========

namespace Resources {
	namespace Strings {


//===============
// Language-Code
//===============

constexpr STRING STR_LANGUAGE_CODE[]=
	{
	{ LNG::DE, "DE" },
	{ LNG::EN, "EN" },
	{ LNG::None, "__" }
	};


//===========
// Languages
//===========

constexpr STRING STR_GERMAN[]=
	{
	{ LNG::DE, "Deutsch" },
	{ LNG::EN, "German" },
	{ LNG::None, "GERMAN" }
	};

constexpr STRING STR_ENGLISH[]=
	{
	{ LNG::DE, "Englisch" },
	{ LNG::EN, "English" },
	{ LNG::None, "ENGLISH" }
	};

}}