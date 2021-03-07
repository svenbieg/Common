//================
// MemoryStream.h
//================

#pragma once


//=======
// Using
//=======

#include "Storage/Container.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Streams {


//===============
// Memory-Stream
//===============

class MemoryStream: public Container
{
public:
	// Con-/Destructors
	MemoryStream(VOID* Buffer, SIZE_T Size, SIZE_T Position=0);

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

	// Container
	FileSize GetSize()override;
	BOOL Seek(UINT64 Position)override;

private:
	// Common
	BYTE* pBuffer;
	SIZE_T uPosition;
	SIZE_T uSize;
};

}}