//=======================
// DirectoryIterator.cpp
//=======================

#include "pch.h"


//=======
// Using
//=======

#include "Directory.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Virtual {


//==================
// Con-/Destructors
//==================

DirectoryIterator::DirectoryIterator(Handle<Directory> hdir):
cIt(&hdir->cList),
hDirectory(hdir)
{
hDirectory->cCriticalSection.Lock();
}

DirectoryIterator::DirectoryIterator(Handle<Directory> hdir, SIZE_T upos):
DirectoryIterator(hdir)
{
cIt.set_position(upos);
}

DirectoryIterator::~DirectoryIterator()
{
hDirectory->cCriticalSection.Unlock();
}


//==============
// Modification
//==============

BOOL DirectoryIterator::Last()
{
SIZE_T ucount=hDirectory->cList.get_count();
if(!ucount)
	return false;
return cIt.set_position(ucount-1);
}

}}