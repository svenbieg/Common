//=========
// Graph.h
//=========

#pragma once


//=======
// Using
//=======

#include "Collections/Map.h"
#include "Data/Axis.h"
#include "Data/Row.h"
#include "Graphics/Color.h"
#include "Container.h"


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//=======
// Graph
//=======

class Graph: public Control
{
private:
	// Using
	using Axis=Data::Axis;
	using COLOR=Graphics::COLOR;
	using RowMap=Collections::Map<Handle<String>, Handle<Data::Row>>;

public:
	// Con-/Destructors
	Graph(Handle<String> Id): Graph(Container::Current, Id) {}
	Graph(Handle<Container> Parent, Handle<String> Id);
	~Graph();

	// Common
	Handle<Axis> AxisX;
	Handle<Axis> AxisY;
	UINT Divisor;
	COLOR FontColor;
	Handle<String> FontFamily;
	UINT FontSize;
	COLOR GridColor;
	Handle<String> Id;
	INT Offset;
	Handle<RowMap> Rows;
	SIZE_T WriteScriptToStream(Handle<OutputStream> Stream)override;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
};

}}