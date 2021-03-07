//=================
// HttpResonse.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "HttpResponse.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//==================
// Con-/Destructors
//==================

HttpResponse::HttpResponse(HttpStatus status):
HttpMessage(status)
{}


//========
// Common
//========

SIZE_T HttpResponse::ReadFromStream(Handle<InputStream> hstream)
{
Clear();
StreamReader reader(hstream);
SIZE_T usize=0;
TCHAR pstr[256];
usize+=reader.ReadString(pstr, 256, '\r');
UINT uver0=0;
UINT uver1=0;
UINT ustatus=0;
if(StringScan(pstr, "HTTP/%u.%u %u", &uver0, &uver1, &ustatus)!=3)
	{
	Status=HttpStatus::InternalServerError;
	usize+=reader.Skip();
	return usize;
	}
Status=(HttpStatus)ustatus;
usize+=reader.SkipChar();
while(1)
	{
	usize+=reader.ReadString(pstr, 256, '\r');
	usize+=reader.SkipChar();
	if(pstr[0]==0)
		break;
	UINT uvalue=0;
	if(!StringFind(pstr, ":", &uvalue))
		{
		Status=HttpStatus::InternalServerError;
		usize+=reader.Skip();
		return usize;
		}
	pstr[uvalue]=0;
	uvalue++;
	if(pstr[uvalue]==' ')
		uvalue++;
	Handle<String> hkey=pstr;
	Handle<String> hvalue=&pstr[uvalue];
	Properties->Add(hkey, hvalue);
	}
ContentType=Properties->Get("Content-Type");
auto hcon=Properties->Get("Connection");
if(hcon=="Keep-Alive")
	KeepAlive=true;
return usize;
}

SIZE_T HttpResponse::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("HTTP/1.1 %u %s\r\n", (UINT)Status, HttpStatusToString(Status));
Properties->Set("Connection", KeepAlive? "Keep-Alive": "Close");
UINT64 ucontent=0;
if(Content)
	ucontent=Content->GetSize();
if(ucontent>0)
	{
	Properties->Set("Content-Length", new String("%u", ucontent));
	Properties->Set("Content-Type", ContentType? ContentType: "text/html; charset=ISO-8859-15");
	}
else
	{
	Properties->Remove("Content-Length");
	Properties->Remove("Content-Type");
	}
for(auto hit=Properties->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hkey=hit->GetCurrentId();
	auto hvalue=hit->GetCurrentItem();
	usize+=writer.Print(hkey);
	usize+=writer.Print(": ");
	usize+=writer.Print(hvalue);
	usize+=writer.Print("\r\n");
	}
usize+=writer.Print("\r\n");
return usize;
}

}}