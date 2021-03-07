//==============
// FileHelper.h
//==============

#pragma once


//===========
// Namespace
//===========

namespace Storage {


//=============
// Create-Mode
//=============

enum class FileCreateMode
{
OpenAlways,
OpenExisting,
CreateAlways,
CreateNew
};


//=============
// Access-Mode
//=============

enum class FileAccessMode
{
ReadOnly,
ReadWrite
};


//============
// Share-Mode
//============

enum class FileShareMode
{
None,
Exclusive,
ShareRead,
ShareWrite
};

}