//==================
// MemoryStream.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "MemoryStream.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Streams {


//==================
// Con-/Destructors
//==================

MemoryStream::MemoryStream(VOID* pbuf, SIZE_T usize, SIZE_T upos):
pBuffer((BYTE*)pbuf),
uPosition(upos),
uSize(usize)
{}


//==============
// Input-Stream
//==============

SIZE_T MemoryStream::Available()
{
return uSize-uPosition;
}

SIZE_T MemoryStream::Read(VOID* pbuf, SIZE_T usize)
{
SIZE_T ucopy=MIN(usize, uSize-uPosition);
CopyMemory(pbuf, &pBuffer[uPosition], ucopy);
uPosition+=ucopy;
return ucopy;
}


//===============
// Output-Stream
//===============

VOID MemoryStream::Flush()
{}

SIZE_T MemoryStream::Write(VOID const* pbuf, SIZE_T usize)
{
SIZE_T ucopy=MIN(usize, uSize-uPosition);
CopyMemory(&pBuffer[uPosition], pbuf, ucopy);
uPosition+=ucopy;
return ucopy;
}


//===========
// Container
//===========

FileSize MemoryStream::GetSize()
{
return uSize;
}

BOOL MemoryStream::Seek(UINT64 upos)
{
if(upos>=uSize)
	return false;
uPosition=(SIZE_T)upos;
return true;
}

}}