//========
// Days.h
//========

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


//======
// Days
//======

constexpr STRING STR_MONDAY[]=
	{
	{ LNG::DE, "Mo" },
	{ LNG::EN, "Mon" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_TUESDAY[]=
	{
	{ LNG::DE, "Di" },
	{ LNG::EN, "Tue" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_WEDNESDAY[]=
	{
	{ LNG::DE, "Mi" },
	{ LNG::EN, "Wed" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_THURSDAY[]=
	{
	{ LNG::DE, "Do" },
	{ LNG::EN, "Thu" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_FRIDAY[]=
	{
	{ LNG::DE, "Fr" },
	{ LNG::EN, "Fri" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_SATURDAY[]=
	{
	{ LNG::DE, "Sa" },
	{ LNG::EN, "Sat" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_SUNDAY[]=
	{
	{ LNG::DE, "So" },
	{ LNG::EN, "Sun" },
	{ LNG::None, nullptr }
	};

constexpr STRING const* STRS_DAYS[]=
	{
	STR_MONDAY,
	STR_TUESDAY,
	STR_WEDNESDAY,
	STR_THURSDAY,
	STR_FRIDAY,
	STR_SATURDAY,
	STR_SUNDAY,
	nullptr
	};

}}