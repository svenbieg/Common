//=================
// HttpMessage.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "HttpMessage.h"

using namespace Storage;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//========
// Common
//========

VOID HttpMessage::Clear()
{
KeepAlive=false;
Properties->Clear();
Status=HttpStatus::Ok;
}


//==============
// Input-Stream
//==============

SIZE_T HttpMessage::Available()
{
UpdateHeader();
UINT64 usize=hHeader->GetSize();
if(Content)
	usize+=Content->GetSize();
return (SIZE_T)MIN(usize-uPosition, 0xFFFFFFFF);
}

SIZE_T HttpMessage::Read(VOID* pbufv, SIZE_T usize)
{
UpdateHeader();
UINT64 uheader=hHeader->GetSize();
SIZE_T upos=0;
if(uPosition<uheader)
	{
	hHeader->Seek(uPosition);
	upos=hHeader->Read(pbufv, usize);
	uPosition+=upos;
	}
if(upos==usize)
	return upos;
if(!Content)
	return upos;
BYTE* pbuf=(BYTE*)pbufv;
UINT64 ucontent=uPosition-uheader;
Content->Seek(ucontent);
SIZE_T uread=Content->Read(&pbuf[upos], usize-upos);
upos+=uread;
uPosition+=uread;
return upos;
}


//===============
// Output-Stream
//===============

VOID HttpMessage::Flush()
{}

SIZE_T HttpMessage::Write(VOID const* pbuf, SIZE_T usize)
{
return 0;
}


//===========
// Container
//===========

FileSize HttpMessage::GetSize()
{
UpdateHeader();
UINT64 usize=hHeader->GetSize();
if(Content)
	usize+=Content->GetSize();
return usize;
}

BOOL HttpMessage::Seek(UINT64 upos)
{
UINT64 usize=GetSize();
if(upos>=usize)
	return false;
uPosition=upos;
return true;
}


//============================
// Con-/Destructors Protected
//============================

HttpMessage::HttpMessage(HttpStatus status):
KeepAlive(false),
Status(status)
{
Properties=new PropertyMap();
}


//================
// Common Private
//================

VOID HttpMessage::UpdateHeader()
{
if(hHeader)
	return;
hHeader=new Intermediate();
this->WriteToStream(hHeader);
}

}}