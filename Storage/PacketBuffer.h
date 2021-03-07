//================
// PacketBuffer.h
//================

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/RandomAccessStream.h"


//===========
// Namespace
//===========

namespace Storage {


//===============
// Packet-Buffer
//===============

class PacketBuffer: public Streams::RandomAccessStream
{
public:
	// Con-/Destructors
	PacketBuffer(UINT ChunkSize=512);
	~PacketBuffer();

	// Access
	SIZE_T Available()override;
	SIZE_T Consume(SIZE_T Size);
	BOOL Contains(VOID const* Buffer, SIZE_T Size);
	SIZE_T Copy(VOID* Buffer, SIZE_T Size);
	VOID const* GetChunk()const;
	SIZE_T GetChunkSize()const;
	UINT GetPacketCount()const { return uPacketCount; }
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;
	SIZE_T WriteToStream(Handle<OutputStream> Stream);

	// Modification
	VOID Clear();
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

private:
	// Types
	typedef struct Chunk
		{
		SIZE_T ChunkLength;
		SIZE_T TotalLength;
		Chunk* Previous;
		Chunk* Next;
		}BufferChunk;

	// Common
	Chunk* CreateChunk(VOID const* Buffer, SIZE_T Size, Chunk* Previous);
	VOID FreeChunk(Chunk* Chunk);
	SIZE_T ReadInternal(VOID* Buffer, SIZE_T Size, BOOL Consume);
	BOOL bFlush;
	Chunk* pFirst;
	Chunk* pLast;
	UINT uChunkSize;
	UINT uPacketCount;
	SIZE_T uPosition;
};

}
