//========
// Grid.h
//========

#pragma once


//=======
// Using
//=======

#include "User/Collections/Vector.h"
#include "Container.h"


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//======
// Grid
//======

class Grid: public virtual Container
{
public:
	// Using
	using CoreGrid=Grid;

	// Con-/Destructors
	Grid();
	Grid(Container* Parent);

	// Common
	Property<Grid, UINT> Columns;
	VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect)override;
	Property<Grid, UINT> Lines;

protected:
	// Common
	SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext)override;

private:
	// Using
	using SizeVector=User::Collections::Vector<UINT>;

	// Common
	VOID AbsoluteRaster();
	SIZE GetRaster();
	VOID OnChildAdded(Handle<ControlList> List, Handle<Control> Child);
	VOID OnColumnsReading(UINT& Columns);
	VOID OnLinesReading(UINT& Lines);
	VOID OnRasterChanged(UINT Value);
	VOID StretchRaster(RECT const& Rect);
	VOID UpdateRaster(Handle<DeviceContext> DeviceContext);
	SizeVector cHeights;
	SizeVector cWidths;
};

}}