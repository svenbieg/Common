//===============
// BlockDevice.h
//===============

#pragma once


//=======
// Using
//=======

#include "Storage/Container.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Storage {


//==============
// Block-Device
//==============

class BlockDevice: public Object
{
public:
	// Common
	static Handle<BlockDevice> Current;
	virtual VOID Erase(UINT64 Offset, UINT Size)=0;
	virtual VOID Flush()=0;
	virtual UINT GetBlockSize()=0;
	static Handle<BlockDevice> GetDefault();
	virtual UINT64 GetSize()=0;
	virtual SIZE_T Read(UINT64 Offset, VOID* Buffer, SIZE_T Size)=0;
	virtual BOOL SetSize(UINT64 Size)=0;
	virtual SIZE_T Write(UINT64 Offset, VOID const* Buffer, SIZE_T Size)=0;

protected:
	// Con-/Destructors
	BlockDevice();
};

}}