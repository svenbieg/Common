//=============
// TextBlock.h
//=============

#pragma once


//=======
// Using
//=======

#include "Graphics/Color.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//============
// Text-Block
//============

class TextBlock: public virtual Control
{
public:
	// Using
	using COLOR=Graphics::COLOR;
	using CoreTextBlock=TextBlock;

	// Con-/Destructors
	TextBlock(Handle<String> Text);
	TextBlock(Container* Parent, Handle<String> Text);
	template <class... _args_t> TextBlock(LPCSTR Format, _args_t... Arguments):
		TextBlock(new String(Format, Arguments...)) {}

	// Common
	Property<TextBlock, COLOR> Color;
	BOOL Render(Handle<DeviceContext> DeviceContext)override;
	VariableHandle<TextBlock, String> Text;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Common
	VOID OnColorChanged(COLOR Color);
	VOID OnTextChanged(Handle<String> Text);
};

}}