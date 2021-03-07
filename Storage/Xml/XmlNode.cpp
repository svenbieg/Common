//=============
// XmlNode.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "XmlNode.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Storage {
	namespace Xml {


//==================
// Con-/Destructors
//==================

XmlNode::XmlNode(Handle<String> htag, Handle<String> hcontent):
XmlNode(nullptr, htag, hcontent)
{}

XmlNode::XmlNode(Handle<XmlNode> hparent, Handle<String> htag, Handle<String> hcontent)
{
Attributes=new AttributeMap();
Children=new NodeCluster();
Content=hcontent;
Tag=htag;
if(hparent)
	hparent->Children->Append(this);
}


//========
// Access
//========

Handle<XmlNode> XmlNode::GetChild(LPCSTR ptag, UINT uid, UINT ulen)
{
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hcurrent=hit->GetCurrent();
	auto htag=hcurrent->Tag;
	if(!htag)
		continue;
	if(StringCompare(htag->Begin(), ptag, ulen, false)==0)
		{
		if(uid==0)
			return hcurrent;
		uid--;
		}
	}
return nullptr;
}

SIZE_T XmlNode::GetLength(UINT ulevel)
{
return DoWriteToStream(nullptr, ulevel);
}

SIZE_T XmlNode::WriteToStream(Handle<OutputStream> hstream, UINT ulevel)
{
return DoWriteToStream(hstream, ulevel);
}


//==============
// Modification
//==============

VOID XmlNode::Clear()
{
Attributes->Clear();
Children->Clear();
Content=nullptr;
Tag=nullptr;
}

UINT XmlNode::ReadFrom(LPCSTR pstr, UINT ulen)
{
Clear();
return DoAssign(pstr, ulen);
}


//================
// Common Private
//================

UINT XmlNode::DoAssign(LPCSTR pstr, UINT ulen)
{
UINT utag=0;
if(!StringFind(pstr, "<", &utag, ulen))
	return 0;
utag++;
if(utag>=ulen)
	return 0;
UINT unext=0;
UINT utagend=utag;
UINT uatt=0;
UINT ucontent=0;
for(; utagend<ulen; utagend++)
	{
	if(pstr[utagend]=='>')
		{
		ucontent=utagend+1;
		break;
		}
	if(pstr[utagend]=='/')
		{
		if(utagend+1>=ulen)
			return 0;
		if(pstr[utagend+1]=='>')
			{
			unext=utagend+2;
			break;
			}
		}
	if(CharIsAlpha(pstr[utagend]))
		continue;
	uatt=utagend+1;
	break;
	}
if(utagend>=ulen)
	return 0;
UINT utaglen=utagend-utag;
if(!utaglen)
	return 0;
Tag=new String(utaglen, &pstr[utag]);
if(unext)
	return unext;
while(uatt)
	{
	BOOL bclosed=false;
	for(; uatt<ulen-2; uatt++)
		{
		if(CharIsAlpha(pstr[uatt]))
			break;
		if(pstr[uatt]=='>')
			{
			bclosed=true;
			ucontent=uatt+1;
			break;
			}
		if(pstr[uatt]=='/')
			{
			if(uatt+1>=ulen)
				return 0;
			if(pstr[uatt+1]=='>')
				return uatt+2;
			}
		}
	if(bclosed)
		break;
	if(uatt+2>=ulen)
		return 0;
	UINT uattlen=0;
	if(!StringFind(&pstr[uatt], "=", &uattlen))
		return 0;
	UINT uattend=uatt+uattlen;
	if(pstr[uattend+1]!='\"')
		return 0;
	UINT uvalue=uattend+2;
	UINT uvaluelen=0;
	if(!StringFind(&pstr[uvalue], "\"", &uvaluelen))
		return 0;
	UINT uvalueend=uvalue+uvaluelen;
	Handle<String> hname=new String(uattlen, &pstr[uatt]);
	Handle<String> hvalue=new String(uvaluelen, &pstr[uvalue]);
	Attributes->Add(hname, hvalue);
	uatt=uvalueend+1;
	}
if(!ucontent)
	return 0;
UINT ucontentlen=0;
if(!StringFind(&pstr[ucontent], Tag->Begin(), &ucontentlen, 0, false))
	return 0;
ucontentlen-=2;
UINT ucontentend=ucontent+ucontentlen;
if(pstr[ucontentend]!='<')
	return 0;
if(pstr[ucontentend+1]!='/')
	return 0;
if(pstr[ucontentend+2+utaglen]!='>')
	return 0;
unext=ucontentend+utaglen+3;
while(ucontentlen)
	{
	Handle<XmlNode> hnode=new XmlNode();
	UINT ulen=hnode->ReadFrom(&pstr[ucontent], ucontentlen);
	if(!ulen)
		{
		Children->Clear();
		Content=new String(ucontentlen, &pstr[ucontent]);
		break;
		}
	Children->Append(hnode);
	ucontentlen-=ulen;
	ucontent+=ulen;
	}
return unext;
}

SIZE_T XmlNode::DoWriteToStream(Handle<OutputStream> hstream, UINT ulevel)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
if(Tag)
	{
	for(UINT u=0; u<ulevel; u++)
		usize+=writer.Print("\t");
	usize+=writer.Print("<");
	usize+=writer.Print(Tag);
	for(auto hit=Attributes->First(); hit->HasCurrent(); hit->MoveNext())
		{
		auto hname=hit->GetCurrentId();
		auto hvalue=hit->GetCurrentItem();
		usize+=writer.Print(" ");
		usize+=writer.Print(hname);
		if(hvalue)
			{
			usize+=writer.Print("=\"");
			usize+=writer.Print(hvalue);
			usize+=writer.Print("\"");
			}
		}
	}
if(Children->GetCount()>0)
	{
	if(Tag)
		usize+=writer.Print(">\r\n");
	for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
		{
		auto hcurrent=hit->GetCurrent();
		usize+=hcurrent->WriteToStream(hstream, ulevel+1);
		}
	if(Tag)
		{
		for(UINT u=0; u<ulevel; u++)
			usize+=writer.Print("\t");
		}
	}
else if(Content)
	{
	if(Tag)
		usize+=writer.Print(">");
	usize+=writer.Print(Content);
	}
else
	{
	if(Tag)
		{
		usize+=writer.Print(" />\r\n");
		return usize;
		}
	}
if(Tag)
	{
	usize+=writer.Print("</");
	usize+=writer.Print(Tag);
	usize+=writer.Print(">\r\n");
	}
return usize;
}

}}
