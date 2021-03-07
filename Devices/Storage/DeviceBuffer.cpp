//==================
// DeviceBuffer.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "DeviceBuffer.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Storage {


//==================
// Con-/Destructors
//==================

DeviceBuffer::DeviceBuffer(Handle<BlockDevice> device, UINT64 offset, UINT size):
hDevice(device),
uBufPos(0),
uBufSize(0),
uOffset(offset),
uSize(size)
{
pBuffer=(BYTE*)DeviceAlloc(size);
}

DeviceBuffer::~DeviceBuffer()
{
DeviceFree(pBuffer);
}


//========
// Common
//========

BOOL DeviceBuffer::Seek(UINT64 offset)
{
UINT64 size=hDevice->GetSize();
if(offset>=size)
	return false;
uOffset=offset;
uBufPos=0;
uBufSize=0;
return true;
}


//==============
// Input-Stream
//==============

SIZE_T DeviceBuffer::Available()
{
UINT64 size=hDevice->GetSize();
UINT64 available=size-uOffset-uBufPos;
if(available>MAX_SIZE_T)
	return MAX_SIZE_T;
return (SIZE_T)available;
}

SIZE_T DeviceBuffer::Read(VOID* bufv, SIZE_T size)
{
if(!size)
	return 0;
SIZE_T available=Available();
size=MIN(size, available);
if(!bufv)
	return size;
BYTE* buf=(BYTE*)bufv;
SIZE_T read=0;
while(read<size)
	{
	if(uBufPos==uBufSize)
		{
		hDevice->Read(uOffset, pBuffer, uSize);
		uBufPos=0;
		uBufSize=uSize;
		}
	SIZE_T copy=size-read;
	copy=MIN(copy, uBufSize-uBufPos);
	CopyMemory(&buf[read], &pBuffer[uBufPos], copy);
	uBufPos+=copy;
	read+=copy;
	if(uBufPos==uBufSize)
		uOffset+=uSize;
	}
return read;
}


//===============
// Output-Stream
//===============

VOID DeviceBuffer::Flush()
{
if(!uBufSize)
	return;
UINT block_size=hDevice->GetBlockSize();
if(uOffset%block_size==0)
	hDevice->Erase(uOffset, block_size);
hDevice->Write(uOffset, pBuffer, uBufSize);
}

SIZE_T DeviceBuffer::Write(VOID const* bufv, SIZE_T size)
{
if(!size)
	return 0;
SIZE_T available=Available();
size=MIN(size, available);
BYTE const* buf=(BYTE const*)bufv;
SIZE_T written=0;
while(written<size)
	{
	if(uBufPos==uSize)
		{
		Flush();
		uOffset+=uSize;
		uBufPos=0;
		uBufSize=0;
		}
	SIZE_T copy=size-written;
	copy=MIN(copy, uSize-uBufPos);
	CopyMemory(&pBuffer[uBufPos], &buf[written], copy);
	uBufPos+=copy;
	uBufSize=MAX(uBufSize, uBufPos);
	written+=copy;
	}
return written;
}

}}