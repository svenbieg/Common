//===========
// XmlNode.h
//===========

#pragma once


//=======
// Using
//=======

#include "Collections/Cluster.h"
#include "Collections/Map.h"
#include "Storage/Streams/OutputStream.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Xml {


//==========
// Xml-Node
//==========

class XmlNode: public ::Object
{
private:
	// Using
	using AttributeMap=::Collections::Map<Handle<String>, Handle<String>, 8>;
	using NodeCluster=::Collections::Cluster<Handle<XmlNode>, 8>;
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Con-/Destructors
	XmlNode(Handle<String> Tag=nullptr, Handle<String> Content=nullptr);
	XmlNode(Handle<XmlNode> Parent, Handle<String> Tag=nullptr, Handle<String> Content=nullptr);
	template <class... _args_t> XmlNode(Handle<String> Tag, LPCSTR Format, _args_t... Arguments): XmlNode(Tag, new String(Format, Arguments...)) {}
	template <class... _args_t> XmlNode(Handle<XmlNode> Parent, Handle<String> Tag, LPCSTR Format, _args_t... Arguments): XmlNode(Parent, Tag, new String(Format, Arguments...)) {}

	// Common
	Handle<AttributeMap> Attributes;
	Handle<NodeCluster> Children;
	Handle<String> Content;
	Handle<XmlNode> GetChild(LPCSTR Tag, UINT Id=0, UINT Length=0);
	SIZE_T GetLength(UINT Level=0);
	Handle<String> Tag;
	SIZE_T WriteToStream(Handle<OutputStream> Stream, UINT Level=0);

	// Modification
	VOID Clear();
	UINT ReadFrom(LPCSTR String, UINT Length);

private:
	// Common
	UINT DoAssign(LPCSTR String, UINT Length);
	SIZE_T DoWriteToStream(Handle<OutputStream> Stream, UINT Level);
};

}}
