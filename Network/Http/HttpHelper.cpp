//================
// HttpHelper.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "CharHelper.h"
#include "HttpHelper.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//=============
// HTTP-Status
//=============

HttpStatus HttpStatusFromString(LPCTSTR pstatus)
{
if(StringCompare(pstatus, "Ok", 0, false)==0)
	return HttpStatus::Ok;
if(StringCompare(pstatus, "No Content", 0, false)==0)
	return HttpStatus::NoContent;
if(StringCompare(pstatus, "Moved Permanently", 0, false)==0)
	return HttpStatus::MovedPermanently;
if(StringCompare(pstatus, "Permanent Redirect", 0, false)==0)
	return HttpStatus::PermanentRedirect;
if(StringCompare(pstatus, "Bad Request", 0, false)==0)
	return HttpStatus::BadRequest;
if(StringCompare(pstatus, "Site Not Found", 0, false)==0)
	return HttpStatus::SiteNotFound;
if(StringCompare(pstatus, "Request Timeout", 0, false)==0)
	return HttpStatus::RequestTimeout;
if(StringCompare(pstatus, "Length Required", 0, false)==0)
	return HttpStatus::LengthRequired;
if(StringCompare(pstatus, "Internal Server Error", 0, false)==0)
	return HttpStatus::InternalServerError;
return HttpStatus::None;
}

LPCSTR HttpStatusToString(HttpStatus status)
{
switch(status)
	{
	case HttpStatus::Ok:
		return "Ok";
	case HttpStatus::NoContent:
		return "No Content";
	case HttpStatus::MovedPermanently:
		return "Moved Permanently";
	case HttpStatus::PermanentRedirect:
		return "Permanent Redirect";
	case HttpStatus::BadRequest:
		return "Bad Request";
	case HttpStatus::SiteNotFound:
		return "Site Not Found";
	case HttpStatus::RequestTimeout:
		return "Request Timeout";
	case HttpStatus::LengthRequired:
		return "Length Required";
	case HttpStatus::InternalServerError:
		return "Internal Server Error";
	default:
		return nullptr;
	}
return nullptr;
}


//=============
// HTTP-Method
//=============

HttpMethod HttpMethodFromString(LPCTSTR pmethod)
{
if(StringCompare(pmethod, "NOTIFY", 0, false)==0)
	return HttpMethod::Notify;
if(StringCompare(pmethod, "GET", 0, false)==0)
	return HttpMethod::Get;
if(StringCompare(pmethod, "SET", 0, false)==0)
	return HttpMethod::Set;
if(StringCompare(pmethod, "POST", 0, false)==0)
	return HttpMethod::Post;
return HttpMethod::Unknown;
}

LPCSTR HttpMethodToString(HttpMethod method)
{
switch(method)
	{
	case HttpMethod::Notify:
		return "NOTIFY";
	case HttpMethod::Get:
		return "GET";
	case HttpMethod::Set:
		return "SET";
	case HttpMethod::Post:
		return "POST";
	case HttpMethod::Unknown:
		return nullptr;
	}
return nullptr;
}


//===========
// HTTP-Path
//===========

UINT HttpPathLength(LPCTSTR pstr, UINT ulen)
{
UINT uret=0;
for(UINT u=0; u<ulen; u++)
	{
	if(pstr[u]=='%')
		u+=2;
	uret++;
	}
return uret;
}

Handle<String> HttpPathToString(LPCTSTR pstr, UINT ulen)
{
UINT upath=HttpPathLength(pstr, ulen);
Handle<String> hstr=new String(upath+1, nullptr);
LPTSTR pdst=(LPTSTR)hstr->Begin();
UINT udst=0;
for(UINT u=0; u<ulen; u++)
	{
	TCHAR c=pstr[u];
	if(c=='%')
		{
		c=(BYTE)((CharToHex(pstr[u+1])<<8)|CharToHex(pstr[u+2]));
		u+=2;
		if(c==0xC3)
			{
			TCHAR c1=pstr[u+1];
			if(c1=='%')
				{
				c1=(BYTE)((CharToHex(pstr[u+2])<<8)|CharToHex(pstr[u+3]));
				if(c1==0x84)
					{
					c='Ä';
					}
				else if(c1==0x96)
					{
					c='Ö';
					}
				else if(c1==0x9C)
					{
					c='Ü';
					}
				else if(c1==0x9F)
					{
					c='ß';
					}
				else if(c1==0xA4)
					{
					c='ä';
					}
				else if(c1==0xB6)
					{
					c='ö';
					}
				else if(c1==0xBC)
					{
					c='ü';
					}
				else
					{
					c='_';
					}
				u+=3;
				}
			}
		}
	pdst[udst++]=c;
	}
pdst[udst]=0;
return hstr;
}

}}
