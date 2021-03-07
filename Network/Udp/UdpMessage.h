//==============
// UdpMessage.h
//==============

#pragma once


//=======
// Using
//=======

#include "Storage/Buffer.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Udp {


//=============
// Udp-Message
//=============

class UdpMessage: public Object
{
public:
	// Con-/Destructors
	UdpMessage(VOID const* Buffer, WORD Size);
	~UdpMessage();

	// Common
	VOID const* Begin()const { return pBuffer; }
	WORD GetSize()const { return uSize; }

private:
	// Common
	VOID* pBuffer;
	WORD uSize;
};

}}
