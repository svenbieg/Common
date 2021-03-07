//===============
// Workspace.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Workspace.h"


//===========
// Namespace
//===========

namespace Storage {


//==================
// Con-/Destructors
//==================

Workspace::Workspace()
{
Directories=new DirectoryList();
}


//========
// Access
//========

Handle<Convertible> Workspace::Get(Handle<String> hpath)
{
for(auto hit=Directories->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hdir=hit->GetCurrent();
	auto hitem=hdir->Get(hpath);
	if(hitem)
		return hitem;
	}
return nullptr;
}

}