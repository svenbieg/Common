//===============
// HttpRequest.h
//===============

#pragma once


//=======
// Using
//=======

#include "HttpMessage.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//==============
// Http-Request
//==============

class HttpRequest: public HttpMessage
{
public:
	// Con-/Destructors
	HttpRequest();

	// Common
	VOID Clear()override;
	static Handle<HttpRequest> FromUrl(Handle<String> Url);
	HttpMethod Method;
	Handle<String> Params;
	Handle<String> Path;
	SIZE_T ReadFromStream(Handle<InputStream> Stream)override;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
};

}}
