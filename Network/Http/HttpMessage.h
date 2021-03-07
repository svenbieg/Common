//===============
// HttpMessage.h
//===============

#pragma once


//=======
// Using
//=======

#include "Collections/Map.h"
#include "Storage/Intermediate.h"
#include "HttpHelper.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//==============
// Http-Message
//==============

class HttpMessage: public Storage::Container
{
protected:
	// Using
	using Container=Storage::Container;
	using InputStream=Storage::Streams::InputStream;
	using Intermediate=Storage::Intermediate;
	using FileSize=Storage::FileSize;
	using OutputStream=Storage::Streams::OutputStream;
	using PropertyMap=Collections::Map<Handle<String>, Handle<String>, 8>;

public:
	// Common
	virtual VOID Clear();
	Handle<Container> Content;
	Handle<String> ContentType;
	BOOL KeepAlive;
	Handle<PropertyMap> Properties;
	virtual SIZE_T ReadFromStream(Handle<InputStream> Stream)=0;
	HttpStatus Status;
	virtual SIZE_T WriteToStream(Handle<OutputStream> Stream)=0;

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

	// Container
	FileSize GetSize()override;
	BOOL Seek(UINT64 Position)override;

protected:
	// Con-/Destructors
	HttpMessage(HttpStatus Status=HttpStatus::Ok);

private:
	// Common
	VOID UpdateHeader();
	Handle<Intermediate> hHeader;
	UINT64 uPosition;
};

}}
