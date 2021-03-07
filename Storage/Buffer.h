//==========
// Buffer.h
//==========

#pragma once


//=======
// Using
//=======

#include "Storage/Container.h"


//===========
// Namespace
//===========

namespace Storage {


//=============
// Buffer-Type
//=============

enum class BufferType: UINT
{
Dynamic,
Static
};


//========
// Buffer
//========

class Buffer: public Container
{
private:
	// Using
	using InputStream=Storage::Streams::InputStream;
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Con-/Destructors
	Buffer(SIZE_T Size);
	Buffer(VOID const* Buffer, SIZE_T Size, BufferType Type=BufferType::Dynamic);
	~Buffer();

	// Access
	SIZE_T Available()override;
	BYTE* Begin()const { return pBuffer; }
	SIZE_T GetPosition()const { return uPosition; }
	FileSize GetSize()override { return uSize; }
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;
	BOOL Seek(UINT64 Position)override;
	SIZE_T WriteToStream(Handle<OutputStream> Stream);

	// Modification
	SIZE_T Fill(SIZE_T Value, SIZE_T Size=0);
	inline VOID Flush()override {}
	SIZE_T ReadFromStream(Handle<InputStream> Stream);
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;
	SIZE_T Zero();
	SIZE_T Zero(SIZE_T Size);

private:
	// Common
	BYTE* pBuffer;
	SIZE_T uPosition;
	SIZE_T uSize;
	BufferType uType;
};

}
