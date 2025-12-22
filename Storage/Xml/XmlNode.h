//===========
// XmlNode.h
//===========

#pragma once


//=======
// Using
//=======

#include "Collections/list.hpp"
#include "Collections/map.hpp"
#include "Concurrency/Task.h"
#include "Storage/Streams/Stream.h"
#include "Event.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Xml {


//======================
// Forward-Declarations
//======================

class XmlNodeAttributeIterator;
class XmlNodeChildIterator;


//==========
// XML-Node
//==========

class XmlNode: public Object
{
public:
	// Using
	using InputStream=Storage::Streams::InputStream;
	using OutputStream=Storage::Streams::OutputStream;

	// Friends
	friend XmlNodeAttributeIterator;
	friend XmlNodeChildIterator;

	// Con-/Destructors
	static inline Handle<XmlNode> Create(Handle<String> Tag=nullptr) { return new XmlNode(Tag); }
	static inline Handle<XmlNode> Create(XmlNode* Parent, Handle<String> Tag=nullptr) { return new XmlNode(Parent, Tag); }

	// Common
	Handle<XmlNodeAttributeIterator> Attributes();
	Event<XmlNode> Changed;
	Handle<XmlNodeChildIterator> Children();
	BOOL Clear(BOOL Notify=true);
	VOID CopyFrom(XmlNode* Copy, BOOL Notify=true);
	Handle<String> GetAttribute(Handle<String> Name);
	BOOL GetAttribute(Handle<String> Name, UINT* Value);
	BOOL GetAttribute(Handle<String> Name, UINT64* Value);
	inline Handle<XmlNode> GetChild(Handle<String> Name)const { return m_Index.get(Name); }
	inline Handle<String> GetName()const { return m_Attributes.get("Name"); }
	SIZE_T GetSize();
	inline Handle<String> GetTag()const { return m_Tag; }
	inline Handle<String> GetValue()const { return m_Value; }
	inline BOOL HasAttribute(Handle<String> Name)const { return m_Attributes.contains(Name); }
	SIZE_T ReadFromStream(InputStream* Stream);
	BOOL SetAttribute(Handle<String> Name, INT Value, BOOL Notify=true);
	BOOL SetAttribute(Handle<String> Name, UINT Value, BOOL Notify=true);
	BOOL SetAttribute(Handle<String> Name, UINT64 Value, BOOL Notify=true);
	BOOL SetAttribute(Handle<String> Name, Handle<String> Value, BOOL Notify=true);
	BOOL SetName(Handle<String> Name, BOOL Notify=true);
	BOOL SetTag(Handle<String> Tag, BOOL Notify=true);
	BOOL SetValue(Handle<String> Value, BOOL Notify=true);
	SIZE_T WriteToStream(OutputStream* Stream, INT Level=-1);

protected:
	// Con-/Destructors
	XmlNode(Handle<String> Tag=nullptr): XmlNode(nullptr, Tag) {}
	XmlNode(XmlNode* Parent, Handle<String> Tag=nullptr);

	// Common
	Collections::map<Handle<String>, Handle<String>> m_Attributes;
	Collections::list<Handle<XmlNode>> m_Children;
	Collections::map<Handle<String>, XmlNode*> m_Index;
	Concurrency::Mutex m_Mutex;
	XmlNode* m_Parent;
	SIZE_T m_Size;
	Handle<String> m_Tag;
	Handle<String> m_Value;
};


//====================
// Attribute-Iterator
//====================

class XmlNodeAttributeIterator: public Object
{
public:
	// Using
	using AccessMode=Concurrency::AccessMode;

	// Friends
	friend XmlNode;

	// Access
	inline Handle<String> GetName()const { return m_It.get_key(); }
	inline Handle<String> GetValue()const { return m_It.get_value(); }
	inline BOOL HasCurrent()const { return m_It.has_current(); }

	// Navigation
	inline BOOL Begin() { return m_It.begin(); }
	inline BOOL End() { return m_It.rbegin(); }
	inline UINT GetPosition() { return m_It.get_position(); }
	inline BOOL MoveNext() { return m_It.move_next(); }
	inline BOOL MovePrevious() { return m_It.move_previous(); }

private:
	// Con-/Destructors
	XmlNodeAttributeIterator(XmlNode* Node): m_It(&Node->m_Attributes), m_Node(Node)
		{
		m_Node->m_Mutex.Lock(AccessMode::ReadOnly);
		}
	~XmlNodeAttributeIterator()
		{
		m_Node->m_Mutex.Unlock(AccessMode::ReadOnly);
		}

	// Common
	typename Collections::map<Handle<String>, Handle<String>>::iterator m_It;
	Handle<XmlNode> m_Node;
};


//================
// Child-Iterator
//================

class XmlNodeChildIterator: public Object
{
public:
	// Using
	using AccessMode=Concurrency::AccessMode;

	// Friends
	friend XmlNode;

	// Access
	inline Handle<XmlNode> GetCurrent()const { return m_It.get_current(); }
	inline BOOL HasCurrent()const { return m_It.has_current(); }

	// Navigation
	inline BOOL Begin() { return m_It.begin(); }
	inline BOOL End() { return m_It.rbegin(); }
	inline UINT GetPosition() { return m_It.get_position(); }
	inline BOOL MoveNext() { return m_It.move_next(); }
	inline BOOL MovePrevious() { return m_It.move_previous(); }

private:
	// Con-/Destructors
	XmlNodeChildIterator(XmlNode* Node): m_It(&Node->m_Children), m_Node(Node)
		{
		m_Node->m_Mutex.Lock(AccessMode::ReadOnly);
		}
	~XmlNodeChildIterator()
		{
		m_Node->m_Mutex.Unlock(AccessMode::ReadOnly);
		}

	// Common
	typename Collections::list<Handle<XmlNode>>::iterator m_It;
	Handle<XmlNode> m_Node;
};

}}