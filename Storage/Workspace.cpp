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

Workspace::Workspace(Handle<String> name):
Directory(name)
{
hDirectories=new DirectoryList();
hVirtual=new Storage::Virtual::Directory("Virtual");
AddDirectory(hVirtual);
}


//========
// Common
//========

BOOL Workspace::Add(Handle<String> name, Handle<Object> obj)
{
return hVirtual->Add(name, obj);
}

VOID Workspace::AddDirectory(Handle<Storage::Directory> dir)
{
if(!dir)
	return;
hDirectories->Append(dir);
hVirtual->Add(dir->GetName(), dir);
}

Handle<File> Workspace::CreateFile(Handle<String> path, FileCreateMode create, FileAccessMode access, FileShareMode share)
{
if(!path||path->IsEmpty())
	return nullptr;
auto ppath=path->Begin();
UINT pos=0;
while(PathIsSeparator(ppath[pos]))
	pos++;
UINT uclen=PathGetComponentLength(&ppath[pos]);
if(!uclen)
	return nullptr;
Handle<String> name=new String(uclen, &ppath[pos]);
pos+=uclen;
if(ppath[pos]==0)
	return nullptr;
auto obj=hVirtual->GetChild(name);
if(!obj)
	return nullptr;
auto dir=Convert<Storage::Directory>(obj);
if(!dir)
	return nullptr;
return dir->CreateFile(&ppath[pos], create, access, share);
}

Handle<DirectoryIterator> Workspace::First()
{
return hVirtual->First();
}

Handle<Object> Workspace::Get(Handle<String> path)
{
for(auto it=hDirectories->First(); it->HasCurrent(); it->MoveNext())
	{
	auto dir=it->GetCurrent();
	auto obj=dir->Get(path);
	if(obj)
		return obj;
	}
return nullptr;
}

}