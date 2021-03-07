//=============
// EntryType.h
//=============

#pragma once


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {
		namespace Entries {


//============
// Entry-Type
//============

enum class EntryType: UINT
{
None=0,
Database=0x4244, // DB
Directory=0x524944, // DIR
File=0x454C4946, // FILE
Update=0x54445055, // UPDT
New=0xFFFFFFFF
};

}}}