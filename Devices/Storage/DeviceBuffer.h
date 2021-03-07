//================
// DeviceBuffer.h
//================

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/RandomAccessStream.h"
#include "BlockDevice.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Storage {


//===============
// Device-Buffer
//===============

class DeviceBuffer: public ::Storage::Streams::RandomAccessStream
{
public:
	// Con-/Destructors
	DeviceBuffer(Handle<BlockDevice> Device, UINT64 Offset, UINT Size=PAGE_SIZE);
	~DeviceBuffer();

	// Common
	BOOL Seek(UINT64 Offset);

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

private:
	// Common
	Handle<BlockDevice> hDevice;
	BYTE* pBuffer;
	UINT uBufPos;
	UINT uBufSize;
	UINT64 uOffset;
	UINT uSize;
};

}}