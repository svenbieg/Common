//==========
// Canvas.h
//==========

#pragma once


//=======
// Using
//=======

#include "Container.h"


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//========
// Canvas
//========

class Canvas: public virtual Container
{
public:
	// Using
	using CoreCanvas=Canvas;

	// Con-/Destructors
	Canvas(Container* Parent);
	VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect)override;

protected:
	// Common
	virtual SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;
};

}}