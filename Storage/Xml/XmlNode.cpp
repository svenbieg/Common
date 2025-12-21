//=============
// XmlNode.cpp
//=============

#include "XmlNode.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"

using namespace Concurrency;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Storage {
	namespace Xml {


//========
// Common
//========

Handle<XmlNodeAttributeIterator> XmlNode::Attributes()
{
return new XmlNodeAttributeIterator(this);
}

Handle<XmlNodeChildIterator> XmlNode::Children()
{
return new XmlNodeChildIterator(this);
}

BOOL XmlNode::Clear(BOOL notify)
{
WriteLock lock(m_Mutex);
bool cleared=false;
cleared|=m_Attributes.clear();
cleared|=m_Children.clear();
cleared|=m_Index.clear();
cleared|=m_Tag;
cleared|=m_Value;
m_Tag=nullptr;
m_Value=nullptr;
if(!cleared)
	return false;
if(!notify)
	return true;
lock.Unlock();
Changed(this);
return true;
}

VOID XmlNode::CopyFrom(XmlNode* node, BOOL notify)
{
if(!node)
	return;
WriteLock lock(m_Mutex);
ReadLock node_lock(node->m_Mutex);
m_Tag=node->m_Tag;
m_Attributes.copy_from(node->m_Attributes);
m_Children.clear();
m_Index.clear();
if(node->m_Value)
	{
	m_Value=node->m_Value;
	}
else
	{
	for(auto it=node->m_Children.cbegin(); it.has_current(); it.move_next())
		{
		auto child=XmlNode::Create();
		child->CopyFrom(it.get_current(), false);
		m_Children.append(child);
		child->m_Parent=this;
		auto name=child->GetName();
		if(name)
			m_Index.set(name, child);
		}
	}
if(!notify)
	return;
lock.Unlock();
node_lock.Unlock();
Changed(this);
}

Handle<String> XmlNode::GetAttribute(Handle<String> name)
{
ReadLock lock(m_Mutex);
return m_Attributes.get(name);
}

BOOL XmlNode::GetAttribute(Handle<String> name, UINT* value_ptr)
{
ReadLock lock(m_Mutex);
auto att=m_Attributes.get(name);
if(!att)
	return false;
return att->Scan("%u", value_ptr)==1;
}

BOOL XmlNode::GetAttribute(Handle<String> name, UINT64* value_ptr)
{
ReadLock lock(m_Mutex);
auto att=m_Attributes.get(name);
if(!att)
	return false;
return att->Scan("%u", value_ptr)==1;
}

SIZE_T XmlNode::GetSize()
{
WriteLock lock(m_Mutex);
if(m_Size)
	return m_Size;
if(!m_Tag)
	return 0;
StreamWriter writer;
SIZE_T size=0;
size+=writer.Print("<");
size+=writer.Print(m_Tag);
for(auto it=m_Attributes.cbegin(); it.has_current(); it.move_next())
	{
	auto name=it.get_key();
	auto value=it.get_value();
	size+=writer.Print(" ");
	size+=writer.Print(name);
	if(value)
		{
		size+=writer.Print("=\"");
		size+=writer.Print(value);
		size+=writer.Print("\"");
		}
	}
if(m_Value)
	{
	size+=writer.Print(m_Value);
	size+=writer.Print("</");
	size+=writer.Print(m_Tag);
	size+=writer.Print(">");
	m_Size=size;
	return m_Size;
	}
if(!m_Children.get_count())
	{
	size+=writer.Print(" />");
	m_Size=size;
	return m_Size;
	}
size+=writer.Print(">");
for(auto it=m_Children.cbegin(); it.has_current(); it.move_next())
	{
	auto child=it.get_current();
	size+=child->GetSize();
	}
size+=writer.Print("</");
size+=writer.Print(m_Tag);
size+=writer.Print(">");
m_Size=size;
return m_Size;
}

SIZE_T XmlNode::ReadFromStream(InputStream* stream)
{
WriteLock lock(m_Mutex);
m_Attributes.clear();
m_Children.clear();
m_Index.clear();
m_Parent=nullptr;
m_Size=0;
m_Value=nullptr;
StreamReader reader(stream);
SIZE_T read=0;
auto value=reader.ReadString(&read, "<", "\r\n\t ");
if(value)
	{
	m_Size=read;
	m_Value=value;
	return read;
	}
if(!CharHelper::Equal(reader.LastChar, '<'))
	return 0;
auto tag=reader.ReadString(&read, " />", " ");
if(!tag)
	return 0;
if(m_Tag)
	{
	if(StringHelper::Compare(m_Tag, tag, 0, false)!=0)
		return 0;
	}
while(CharHelper::Equal(reader.LastChar, ' '))
	{
	auto att_name=reader.ReadString(&read, " =/>", " ");
	if(!att_name)
		break;
	Handle<String> att_value;
	if(CharHelper::Equal(reader.LastChar, '='))
		att_value=reader.ReadString(&read, "\"", " \"");
	m_Attributes.set(att_name, att_value);
	}
if(CharHelper::Equal(reader.LastChar, '/'))
	{
	read+=reader.ReadChar();
	if(!CharHelper::Equal(reader.LastChar, '>'))
		return 0;
	m_Size=read;
	return read;
	}
while(1)
	{
	auto child=XmlNode::Create();
	read+=child->ReadFromStream(stream);
	if(!child->m_Tag)
		{
		m_Value=child->m_Value;
		break;
		}
	m_Children.append(child);
	child->m_Parent=this;
	auto name=child->GetName();
	if(!name)
		continue;
	if(!m_Index.add(name, child))
		return 0;
	}
read+=reader.ReadChar();
if(!CharHelper::Equal(reader.LastChar, '/'))
	return 0;
auto close=reader.ReadString(&read, ">");
if(!CharHelper::Equal(reader.LastChar, '>'))
	return 0;
if(StringHelper::Compare(close, tag, 0, false)!=0)
	return 0;
m_Size=read;
m_Tag=tag;
return read;
}

BOOL XmlNode::SetAttribute(Handle<String> name, INT value, BOOL notify)
{
WriteLock lock(m_Mutex);
if(!m_Attributes.set(name, String::Create("%i", value)))
	return false;
m_Size=0;
if(!notify)
	return true;
lock.Unlock();
Changed(this);
return true;
}

BOOL XmlNode::SetAttribute(Handle<String> name, UINT value, BOOL notify)
{
WriteLock lock(m_Mutex);
if(!m_Attributes.set(name, String::Create("%u", value)))
	return false;
m_Size=0;
if(!notify)
	return true;
lock.Unlock();
Changed(this);
return true;
}

BOOL XmlNode::SetAttribute(Handle<String> name, UINT64 value, BOOL notify)
{
WriteLock lock(m_Mutex);
if(!m_Attributes.set(name, String::Create("%u", value)))
	return false;
m_Size=0;
if(!notify)
	return true;
lock.Unlock();
Changed(this);
return true;
}

BOOL XmlNode::SetAttribute(Handle<String> name, Handle<String> value, BOOL notify)
{
WriteLock lock(m_Mutex);
if(!m_Attributes.set(name, value))
	return false;
m_Size=0;
if(!notify)
	return true;
lock.Unlock();
Changed(this);
return true;
}

BOOL XmlNode::SetName(Handle<String> name, BOOL notify)
{
WriteLock lock(m_Mutex);
if(m_Parent)
	{
	auto old_name=m_Attributes.get("Name");
	if(old_name==name)
		return true;
	WriteLock parent_lock(m_Parent->m_Mutex);
	if(m_Parent->m_Index.contains(name))
		return false;
	if(old_name)
		m_Parent->m_Index.remove(old_name);
	if(name)
		m_Parent->m_Index.set(name, this);
	}
if(!m_Attributes.set("Name", name))
	return false;
if(!notify)
	return true;
lock.Unlock();
Changed(this);
return true;
}

BOOL XmlNode::SetTag(Handle<String> tag, BOOL notify)
{
WriteLock lock(m_Mutex);
if(m_Tag==tag)
	return false;
m_Tag=tag;
if(!notify)
	return true;
lock.Unlock();
Changed(this);
return true;
}

BOOL XmlNode::SetValue(Handle<String> value, BOOL notify)
{
WriteLock lock(m_Mutex);
if(m_Value==value)
	return false;
m_Children.clear();
m_Index.clear();
m_Size=0;
m_Value=value;
if(!notify)
	return true;
lock.Unlock();
Changed(this);
return true;
}

SIZE_T XmlNode::WriteToStream(OutputStream* stream, INT level)
{
ReadLock lock(m_Mutex);
if(!m_Tag)
	return 0;
StreamWriter writer(stream);
SIZE_T written=0;
INT next_level=-1;
if(level>=0)
	{
	next_level=level+1;
	written+=writer.Print("\r\n");
	}
if(level>0)
	written+=writer.PrintChar('\t', level);
written+=writer.Print("<");
written+=writer.Print(m_Tag);
for(auto it=m_Attributes.cbegin(); it.has_current(); it.move_next())
	{
	auto name=it.get_key();
	auto value=it.get_value();
	written+=writer.Print(" ");
	written+=writer.Print(name);
	if(value)
		{
		written+=writer.Print("=\"");
		written+=writer.Print(value);
		written+=writer.Print("\"");
		}
	}
if(m_Value)
	{
	written+=writer.Print(m_Value);
	written+=writer.Print("</");
	written+=writer.Print(m_Tag);
	written+=writer.Print(">");
	return written;
	}
if(!m_Children.get_count())
	{
	written+=writer.Print(" />");
	return written;
	}
written+=writer.Print(">");
for(auto it=m_Children.cbegin(); it.has_current(); it.move_next())
	{
	auto child=it.get_current();
	written+=child->WriteToStream(stream, next_level);
	}
if(level>=0)
	written+=writer.Print("\r\n");
if(level>0)
	written+=writer.PrintChar('\t', level);
written+=writer.Print("</");
written+=writer.Print(m_Tag);
written+=writer.Print(">");
return written;
}


//============================
// Con-/Destructors Protected
//============================

XmlNode::XmlNode(XmlNode* parent, Handle<String> tag):
m_Parent(parent),
m_Size(0),
m_Tag(tag)
{
if(!m_Parent)
	return;
WriteLock lock(m_Parent->m_Mutex);
m_Parent->m_Children.append(this);
lock.Unlock();
m_Parent->Changed(m_Parent);
}

}}