//================
// UdpMessage.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "UdpMessage.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Udp {


//==================
// Con-/Destructors
//==================

UdpMessage::UdpMessage(VOID const* pbuf, WORD usize):
pBuffer(nullptr),
uSize(0)
{
if(usize==0)
	return;
pBuffer=Alloc(usize);
CopyMemory(pBuffer, pbuf, usize);
uSize=usize;
}

UdpMessage::~UdpMessage()
{
if(pBuffer)
	Free(pBuffer);
}

}}
