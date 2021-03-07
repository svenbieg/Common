//=============
// HyperLink.h
//=============

#pragma once


//=======
// Using
//=======

#include "Container.h"


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//============
// Hyper-Link
//============

class HyperLink: public Container
{
public:
	// Con-/Destructors
	HyperLink(Handle<String> Text, Handle<String> Link): HyperLink(Container::Current, Text, Link) {}
	HyperLink(Handle<Container> Parent, Handle<String> Text, Handle<String> Link);

	// Common
	Handle<String> Link;
	Handle<String> Text;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
};

}}
