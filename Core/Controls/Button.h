//==========
// Button.h
//==========

#pragma once


//=======
// Using
//=======

#include "Interactive.h"


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//========
// Button
//========

class Button: public virtual Interactive
{
public:
	// Using
	using CoreButton=Button;

	// Con-/Destructors
	Button(Container* Parent);

	// Common
	Property<Button, BOOL> Down;
	VariableHandle<Button, String> Text;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	VOID OnTextChanged(Handle<String> Text);
};

}}