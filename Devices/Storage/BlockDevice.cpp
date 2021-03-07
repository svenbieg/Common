//=================
// BlockDevice.cpp
//=================

#include "pch.h"


//========
// Common
//========

#include "BlockDevice.h"


//===========
// Namespace
//===========

namespace Devices {
	namespace Storage {


//========
// Common
//========

Handle<BlockDevice> BlockDevice::Current;


//============================
// Con-/Destructors Protected
//============================

BlockDevice::BlockDevice()
{
Current=this;
}

}}