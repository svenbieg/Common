//================
// HttpResponse.h
//================

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


//===============
// Http-Response
//===============

class HttpResponse: public HttpMessage
{
public:
	// Con-/Destructors
	HttpResponse(HttpStatus Status=HttpStatus::Ok);

	// Common
	SIZE_T ReadFromStream(Handle<InputStream> Stream)override;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
};

}}