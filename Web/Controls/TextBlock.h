//=============
// TextBlock.h
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
// Text-Block
//============

class TextBlock: public Control
{
public:
	// Con-/Destructors
	TextBlock(Handle<String> Text=nullptr): TextBlock(Container::Current, Text) {}
	TextBlock(Handle<Container> Parent, Handle<String> Text);
	template <class... _tParams> TextBlock(LPCTSTR Format, _tParams... Params): TextBlock(Container::Current, new String(Format, Params...)) {}
	template <class... _tParams> TextBlock(Handle<Container> Parent, LPCTSTR Format, _tParams... Params): TextBlock(Parent, new String(Format, Params...)) {}

	// Common
	Handle<String> Text;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
};

}}
