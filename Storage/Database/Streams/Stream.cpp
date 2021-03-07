//============
// Stream.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Database/Database.h"
#include "Stream.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {
		namespace Streams {


//========
// Common
//========

VOID Stream::Close(FileShareMode share)
{
ScopedLock lock(cCriticalSection);
switch(share)
	{
	case FileShareMode::Exclusive:
		{
		iShareRead++;
		iShareWrite++;
		break;
		}
	case FileShareMode::ShareRead:
		{
		iShareWrite++;
		break;
		}
	default:
		{
		break;
		}
	}
}

BOOL Stream::Open(FileAccessMode access, FileShareMode share)
{
ScopedLock lock(cCriticalSection);
if(iShareRead<0)
	return false;
if(access==FileAccessMode::ReadWrite)
	{
	if(iShareWrite<0)
		return false;
	}
switch(share)
	{
	case FileShareMode::Exclusive:
		{
		iShareRead--;
		iShareWrite--;
		break;
		}
	case FileShareMode::ShareRead:
		{
		iShareWrite--;
		break;
		}
	default:
		{
		break;
		}
	}
return true;
}


//============================
// Con-/Destructors Protected
//============================

Stream::Stream(Handle<Database> hdb, UINT64 offset):
hDatabase(hdb),
iShareRead(0),
iShareWrite(0),
uOffset(offset)
{}

}}}