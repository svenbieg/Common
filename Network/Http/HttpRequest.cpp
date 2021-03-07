//=================
// HttpRequest.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "HttpRequest.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//==================
// Con-/Destructors
//==================

HttpRequest::HttpRequest():
Method(HttpMethod::Get)
{}


//========
// Common
//========

VOID HttpRequest::Clear()
{
HttpMessage::Clear();
Method=HttpMethod::Unknown;
Params=nullptr;
Path=nullptr;
}

Handle<HttpRequest> HttpRequest::FromUrl(Handle<String> hurl)
{
Handle<HttpRequest> hrequest=new HttpRequest();
if(!hurl)
	return hrequest;
LPCTSTR purl=hurl->Begin();
UINT uhost=0;
if(StringCompare(purl, "http://", 7, false)==0)
	uhost+=7;
Handle<String> hhost;
LPCTSTR phost=&purl[uhost];
UINT upath=0;
if(StringFind(phost, "/", &upath))
	{
	hhost=new String(upath, phost);
	LPCTSTR ppath=&phost[upath+1];
	UINT uparams=0;
	if(StringFind(ppath, "?", &uparams))
		{
		LPCTSTR pparams=&ppath[uparams+1];
		hrequest->Path=new String(uparams, ppath);
		hrequest->Params=pparams;
		}
	else
		{
		hrequest->Path=ppath;
		}
	}
else
	{
	hhost=phost;
	}
hrequest->Properties->Add("Host", hhost);
return hrequest;
}

SIZE_T HttpRequest::ReadFromStream(Handle<InputStream> hstream)
{
Clear();
Status=HttpStatus::InternalServerError;
StreamReader reader(hstream);
SIZE_T usize=0;
TCHAR pstr[256];
usize+=reader.ReadString(pstr, 256, ' ');
Method=HttpMethodFromString(pstr);
if(Method==HttpMethod::Unknown)
	{
	usize+=reader.Skip();
	return usize;
	}
UINT upath=reader.ReadString(pstr, 256, ' ');
usize+=upath;
UINT uparams=0;
if(StringFind(pstr, "?", &uparams))
	{
	Path=HttpPathToString(pstr, uparams);
	Params=new String(&pstr[uparams+1]);
	}
else
	{
	Path=HttpPathToString(pstr, upath);
	}
usize+=reader.GoTo('\n');
while(1)
	{
	usize+=reader.ReadString(pstr, 256, '\r');
	usize+=reader.SkipChar();
	if(pstr[0]==0)
		break;
	UINT uvalue=0;
	if(!StringFind(pstr, ":", &uvalue))
		{
		Properties->Add(pstr, nullptr);
		continue;
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
Status=HttpStatus::Ok;
return usize;
}

SIZE_T HttpRequest::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print(HttpMethodToString(Method));
usize+=writer.Print(" /");
usize+=writer.Print(Path);
if(Params)
	{
	usize+=writer.Print("?");
	usize+=writer.Print(Params);
	}
usize+=writer.Print(" HTTP/1.1\r\n");
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
