//==========
// Update.h
//==========

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/OutputStream.h"


//===========
// Namespace
//===========

namespace Web {


//======================
// Forward-Declarations
//======================

class WebPage;


//========
// Update
//========

class Update: public Object
{
private:
	// Using
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Con-/Destructors
	Update(Handle<WebPage> WebPage, UINT Time);

	// Common
	SIZE_T WriteToStream(Handle<OutputStream> Stream);

private:
	// Common
	Handle<WebPage> hWebPage;
	UINT uTime;
	UINT uTimeNow;
};

}