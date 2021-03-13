//===========
// Entry.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Database/Database.h"
#include "Entry.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {
		namespace Entries {


//============================
// Con-/Destructors Protected
//============================

Entry::Entry(Handle<String> htype, Handle<Database> hdb, UINT64 offset):
Convertible(htype),
hDatabase(hdb),
uOffset(offset),
// Common Private
uAccessCount(0)
{}


//================
// Common Private
//================

VOID Entry::Close()
{
if(uAccessCount>0)
	uAccessCount--;
if(uAccessCount==0)
	hDatabase->hEntryMap->Remove(uOffset);
}

VOID Entry::Open()
{
if(uAccessCount==0)
	hDatabase->hEntryMap->Add(uOffset, this);
uAccessCount++;
}

}}}