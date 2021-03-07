//===========
// WebPage.h
//===========

#pragma once


//=======
// Using
//=======

#include "Collections/Index.h"
#include "Collections/Map.h"
#include "Storage/Streams/OutputStream.h"
#include "Web/Controls/Form.h"
#include "Convertible.h"


//===========
// Namespace
//===========

namespace Web {


//==========
// Web-Page
//==========

class WebPage: public Convertible
{
private:
	// Using
	using Control=Web::Controls::Control;
	using ControlMap=::Collections::Map<Handle<String>, Handle<Control>>;
	using Form=Web::Controls::Form;
	using Index=::Collections::Index<Handle<String>>;
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Con-/Destructors
	WebPage(Handle<String> Title);

	// Common
	Handle<ControlMap> Controls;
	static Handle<WebPage> Current;
	Handle<Form> GetForm()const { return hForm; }
	Handle<String> GetTitle()const { return hTitle; }
	Handle<Index> Scripts;
	VOID Select();
	Handle<Index> Styles;
	SIZE_T WriteToStream(Handle<OutputStream> Stream);

protected:
	// Common
	Handle<Form> hForm;
	Handle<String> hTitle;
};

}