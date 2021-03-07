//===========
// Caption.h
//===========

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


//=========
// Caption
//=========

class Caption: public Control
{
public:
	// Con-/Destructors
	Caption(Handle<String> Text, UINT Level=1): Caption(Container::Current, Text, Level) {}
	Caption(Handle<Container> Parent, Handle<String> Text, UINT Level);

	// Common
	Handle<String> Text;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;

private:
	// Common
	CHAR pTag[6];
	UINT uLevel;
};

}}
