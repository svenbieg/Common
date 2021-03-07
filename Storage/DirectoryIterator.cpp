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


//==================
// Con-/Destructors
//==================

DirectoryIterator::DirectoryIterator(Handle<Directory> hdir):
hDirectory(hdir)
{
hDirectory->FindFirst();
}

DirectoryIterator::~DirectoryIterator()
{
hDirectory->FindClose();
}


//========
// Access
//========

Handle<Convertible> DirectoryIterator::GetCurrent()const
{
return hDirectory->FindCurrent();
}

BOOL DirectoryIterator::HasCurrent()const
{
return hDirectory->FindCurrent()!=nullptr;
}


//==============
// Modification
//==============

BOOL DirectoryIterator::MoveNext()const
{
auto hnext=hDirectory->FindNext();
return hnext!=nullptr;
}

}