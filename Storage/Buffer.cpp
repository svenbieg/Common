//============
// Buffer.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/BufferHelper.h"
#include "Buffer.h"


//===========
// Namespace
//===========

namespace Storage {


//==================
// Con-/Destructors
//==================

Buffer::Buffer(SIZE_T usize):
pBuffer(nullptr),
uPosition(0),
uSize(0),
uType(BufferType::Dynamic)
{
BufferSetSize(&pBuffer, &uSize, usize);
}

Buffer::Buffer(VOID const* pbuf, SIZE_T usize, BufferType utype):
pBuffer(nullptr),
uPosition(0),
uSize(0),
uType(utype)
{
switch(uType)
	{
	case BufferType::Dynamic:
		{
		BufferAssign(&pBuffer, &uSize, pbuf, usize);
		break;
		}
	case BufferType::Static:
		{
		pBuffer=(BYTE*)pbuf;
		uSize=usize;
		break;
		}
	}
}

Buffer::~Buffer()
{
if(uType==BufferType::Dynamic)
	BufferClear(&pBuffer, &uSize);
}


//========
// Access
//========

SIZE_T Buffer::Available()
{
return uSize-uPosition;
}

SIZE_T Buffer::Read(VOID* pbuf, SIZE_T usize)
{
SIZE_T ucopy=MIN(uSize-uPosition, usize);
if(pbuf)
	CopyMemory(pbuf, &pBuffer[uPosition], ucopy);
uPosition+=ucopy;
return ucopy;
}

BOOL Buffer::Seek(UINT64 upos)
{
if(upos>=uSize)
	return false;
uPosition=(SIZE_T)upos;
return true;
}

SIZE_T Buffer::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T ucopy=uSize-uPosition;
if(!hstream)
	return ucopy;
return hstream->Write(&pBuffer[uPosition], ucopy);
}


//==============
// Modification
//==============

SIZE_T Buffer::Fill(SIZE_T value, SIZE_T size)
{
if(size==0)
	size=uSize;
SIZE_T fill=MIN(uSize-uPosition, size);
FillMemory(&pBuffer[uPosition], fill, value);
uPosition+=fill;
return fill;
}

SIZE_T Buffer::ReadFromStream(Handle<InputStream> hstream)
{
if(uType==BufferType::Static)
	return 0;
if(!hstream)
	return 0;
SIZE_T ucopy=uSize-uPosition;
return hstream->Read(&pBuffer[uPosition], ucopy);
}

SIZE_T Buffer::Write(VOID const* pbuf, SIZE_T usize)
{
if(uType==BufferType::Static)
	return 0;
SIZE_T ucopy=MIN(uSize-uPosition, usize);
CopyMemory(&pBuffer[uPosition], pbuf, ucopy);
uPosition+=ucopy;
return ucopy;
}

SIZE_T Buffer::Zero()
{
if(uType==BufferType::Static)
	return 0;
ZeroMemory(pBuffer, uSize);
uPosition=0;
return uSize;
}

SIZE_T Buffer::Zero(SIZE_T size)
{
if(uType==BufferType::Static)
	return 0;
if(uPosition>=uSize)
	return 0;
SIZE_T zero=MIN(size, uSize-uPosition);
ZeroMemory(&pBuffer[uPosition], zero);
uPosition+=zero;
return zero;
}

}
