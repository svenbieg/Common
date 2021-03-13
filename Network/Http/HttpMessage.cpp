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
Content=nullptr;
ContentType=nullptr;
KeepAlive=false;
Properties->Clear();
Status=HttpStatus::Ok;
hHeader=nullptr;
uPosition=0;
}


//==============
// Input-Stream
//==============

SIZE_T HttpMessage::Available()
{
UpdateHeader();
UINT64 size=hHeader->GetSize();
if(Content)
	size+=Content->GetSize();
UINT64 available=size-uPosition;
if(available>MAX_SIZE_T)
	return MAX_SIZE_T;
return (SIZE_T)available;
}

SIZE_T HttpMessage::Read(VOID* pbufv, SIZE_T size)
{
UpdateHeader();
UINT64 header=hHeader->GetSize();
SIZE_T pos=0;
if(uPosition<header)
	{
	hHeader->Seek(uPosition);
	pos=hHeader->Read(pbufv, size);
	uPosition+=pos;
	}
if(pos==size)
	return pos;
if(!Content)
	return pos;
BYTE* pbuf=(BYTE*)pbufv;
UINT64 content_size=Content->GetSize();
UINT64 content_pos=uPosition-header;
UINT64 available=content_size-content_pos;
SIZE_T copy=(SIZE_T)MIN(available, size-pos);
if(!copy)
	return pos;
Content->Seek(content_pos);
SIZE_T read=Content->Read(&pbuf[pos], copy);
pos+=read;
uPosition+=read;
return pos;
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
UINT64 size=hHeader->GetSize();
if(Content)
	size+=Content->GetSize();
return size;
}

BOOL HttpMessage::Seek(UINT64 pos)
{
UINT64 size=GetSize();
if(pos>=size)
	return false;
uPosition=pos;
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