//=========
// Label.h
//=========

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


//=======
// Label
//=======

class Label: public virtual Control
{
public:
	// Using
	using COLOR=Graphics::COLOR;
	using CoreLabel=Label;

	// Con-/Destructors
	Label(Handle<Variable> Variable);
	Label(Container* Parent, Handle<Variable> Variable);
	~Label();

	// Common
	Property<Label, COLOR> Color;
	BOOL Render(Handle<DeviceContext> DeviceContext)override;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;
	Handle<Variable> hVariable;

private:
	// Common
	VOID OnColorChanged(COLOR Color);
	VOID OnVariableChanged(Handle<Variable> Variable);
};

}}