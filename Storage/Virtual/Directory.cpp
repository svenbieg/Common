//===============
// Directory.cpp
//===============

#include "Storage/Virtual/Directory.h"


//=======
// Using
//=======

#include "PathHelper.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Virtual {


//========
// Common
//========

BOOL Directory::Add(Handle<String> name, Handle<Object> obj, BOOL notify)
{
if(!obj)
	return false;
if(!m_Map.add(name, obj))
	return false;
if(notify)
	Changed(this);
return true;
}

VOID Directory::Clear()
{
m_Map.clear();
}

Handle<Object> Directory::GetChild(Handle<String> name)
{
if(!name||name->IsEmpty())
	return nullptr;
return m_Map.get(name);
}

BOOL Directory::Remove(Handle<String> name, BOOL notify)
{
if(!m_Map.remove(name))
	return false;
if(notify)
	Changed(this);
return true;
}

VOID Directory::RemoveAll()
{
if(m_Map.clear())
	Changed(this);
}


//===================
// Storage.Directory
//===================

Handle<Storage::DirectoryIterator> Directory::Begin()
{
return new DirectoryIterator(this);
}

Handle<Storage::File> Directory::CreateFile(Handle<String> path, FileCreateMode create, FileAccessMode access, FileShareMode share)
{
return nullptr;
}

Handle<Object> Directory::Get(Handle<String> path)
{
if(!path)
	return nullptr;
auto path_ptr=path->Begin();
while(CharHelper::Equal(path_ptr[0], "\\/"))
	path_ptr++;
UINT uclen=PathHelper::GetComponentLength(path_ptr);
if(!uclen)
	return nullptr;
auto name=String::Create(uclen, path_ptr);
path_ptr+=uclen;
auto obj=m_Map.get(name);
if(!obj)
	return nullptr;
if(path_ptr[0]==0)
	return obj;
auto sub=obj.As<Storage::Directory>();
if(sub)
	return sub->Get(path_ptr);
return nullptr;
}


//==========================
// Con-/Destructors Private
//==========================

Directory::Directory(Directory* parent, Handle<String> name):
m_Name(name),
m_Parent(parent)
{}


//==========
// Iterator
//==========

DirectoryIterator::DirectoryIterator(Handle<Directory> dir):
m_Directory(dir),
m_It(&dir->m_Map)
{
First();
}

}}