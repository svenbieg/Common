//=============
// AppData.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Database/Database.h"
#include "Storage/AppDataHelper.h"
#include "AppData.h"
#include "PathHelper.h"


//===========
// Namespace
//===========

namespace Storage {


//==================
// Con-/Destructors
//==================

AppData::AppData()
{
hVirtual=new Storage::Virtual::Directory("Virtual");
Directories->Append(hVirtual);
hVirtual->Add(hVirtual->GetName(), hVirtual);
auto hdb=new Database::Database();
auto hinternal=hdb->Root;
Directories->Append(hinternal);
hVirtual->Add(hinternal->GetName(), hinternal);
auto hlocal=GetAppDataLocal();
if(hlocal)
	{
	Directories->Append(hlocal);
	hVirtual->Add(hlocal->GetName(), hlocal);
	}
auto hnet=GetAppDataNetwork();
if(hnet)
	{
	Directories->Append(hnet);
	hVirtual->Add(hnet->GetName(), hnet);
	}
}


//========
// Common
//========

BOOL AppData::Add(Handle<String> hname, Handle<Convertible> hobj)
{
return hVirtual->Add(hname, hobj);
}

Handle<File> AppData::CreateFile(Handle<String> hpath, FileCreateMode create, FileAccessMode access, FileShareMode share)
{
if(!hpath||hpath->IsEmpty())
	return nullptr;
auto ppath=hpath->Begin();
UINT pos=0;
while(PathIsSeparator(ppath[pos]))
	pos++;
UINT uclen=PathGetComponentLength(&ppath[pos]);
if(!uclen)
	return nullptr;
Handle<String> hname=new String(uclen, &ppath[pos]);
pos+=uclen;
if(ppath[pos]==0)
	return nullptr;
auto hobj=hVirtual->GetChild(hname);
if(!hobj)
	return nullptr;
auto hdir=hobj->As<Storage::Directory>("Storage.Directory");
if(!hdir)
	return nullptr;
return hdir->CreateFile(&ppath[pos], create, access, share);
}

}