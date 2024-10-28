//==========
// Months.h
//==========

#pragma once


//=======
// Using
//=======

#include "String.h"


//===========
// Namespace
//===========

namespace Resources {
	namespace Strings {


//========
// Months
//========

constexpr STRING STR_JANUARY[]=
	{
	{ LNG::DE, "Jan" },
	{ LNG::EN, "Jan" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_FEBRUARY[]=
	{
	{ LNG::DE, "Feb" },
	{ LNG::EN, "Feb" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_MARCH[]=
	{
	{ LNG::DE, "Mär" },
	{ LNG::EN, "Mar" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_APRIL[]=
	{
	{ LNG::DE, "Apr" },
	{ LNG::EN, "Apr" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_MAY[]=
	{
	{ LNG::DE, "Mai" },
	{ LNG::EN, "May" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_JUNE[]=
	{
	{ LNG::DE, "Jun" },
	{ LNG::EN, "Jun" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_JULY[]=
	{
	{ LNG::DE, "Jul" },
	{ LNG::EN, "Jul" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_AUGUST[]=
	{
	{ LNG::DE, "Aug" },
	{ LNG::EN, "Aug" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_SEPTEMBER[]=
	{
	{ LNG::DE, "Sep" },
	{ LNG::EN, "Sep" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_OCTOBER[]=
	{
	{ LNG::DE, "Okt" },
	{ LNG::EN, "Oct" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_NOVEMBER[]=
	{
	{ LNG::DE, "Nov" },
	{ LNG::EN, "Nov" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_DECEMBER[]=
	{
	{ LNG::DE, "Dez" },
	{ LNG::EN, "Dec" },
	{ LNG::None, nullptr }
	};

constexpr STRING const* STRS_MONTHS[]=
	{
	STR_JANUARY,
	STR_FEBRUARY,
	STR_MARCH,
	STR_APRIL,
	STR_MAY,
	STR_JUNE,
	STR_JULY,
	STR_AUGUST,
	STR_SEPTEMBER,
	STR_OCTOBER,
	STR_NOVEMBER,
	STR_DECEMBER,
	nullptr
	};

}}