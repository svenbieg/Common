//====================
// HttpConnection.cpp
//====================

#include "pch.h"


//=======
// Using
//=======

#include "HttpConnection.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//==================
// Con-/Destructors
//==================

HttpConnection::HttpConnection(Handle<TcpConnection> hcon):
hTcpConnection(hcon),
uPosition(0),
uStatus(HttpConnectionStatus::Request)
{
hTcpConnection->Closed.Add(this, &HttpConnection::OnTcpConnectionClosed);
hTcpConnection->DataReceived.Add(this, &HttpConnection::OnTcpConnectionDataReceived);
hTcpConnection->DataSent.Add(this, &HttpConnection::OnTcpConnectionDataSent);
hTcpConnection->Send.Add(this, &HttpConnection::OnTcpConnectionSend);
}

HttpConnection::~HttpConnection()
{
CloseInternal();
}


//========
// Common
//========

VOID HttpConnection::Close()
{
if(uStatus==HttpConnectionStatus::Closed)
	return;
CloseInternal();
uStatus=HttpConnectionStatus::Closed;
Closed(this);
}

VOID HttpConnection::Send(Handle<HttpRequest> hrequest)
{
hMessage=hrequest;
uPosition=0;
uStatus=HttpConnectionStatus::Requesting;
}

VOID HttpConnection::Send(Handle<HttpResponse> hresponse)
{
hMessage=hresponse;
uPosition=0;
uStatus=HttpConnectionStatus::Responding;
}

VOID HttpConnection::SetTimeout(UINT utimeout)
{
if(!hTcpConnection)
	return;
hTcpConnection->SetTimeout(utimeout);
}


//==============
// Input-Stream
//==============

SIZE_T HttpConnection::Available()
{
if(!hTcpConnection)
	return 0;
return hTcpConnection->Available();
}

SIZE_T HttpConnection::Read(VOID* pbuf, SIZE_T usize)
{
if(!hTcpConnection)
	return 0;
return hTcpConnection->Read(pbuf, usize);
}


//================
// Common Private
//================

VOID HttpConnection::CloseInternal()
{
hMessage=nullptr;
uPosition=0;
if(hTcpConnection)
	{
	hTcpConnection->Closed.Remove(this);
	hTcpConnection->DataReceived.Remove(this);
	hTcpConnection->DataSent.Remove(this);
	hTcpConnection->Send.Remove(this);
	hTcpConnection=nullptr;
	}
}

VOID HttpConnection::OnTcpConnectionClosed(Handle<TcpConnection> hcon)
{
Close();
}

VOID HttpConnection::OnTcpConnectionDataReceived(Handle<TcpConnection> hcon)
{
switch(uStatus)
	{
	case HttpConnectionStatus::Content:
		{
		DataReceived(this);
		hcon->SetTimeout(10000);
		break;
		}
	case HttpConnectionStatus::Request:
		{
		if(!hcon->InputBuffer->Contains("\r\n\r\n", 4))
			{
			hcon->SetTimeout(10000);
			return;
			}
		Handle<HttpRequest> hrequest=new HttpRequest();
		hrequest->ReadFromStream(hcon);
		if(hrequest->Status==HttpStatus::InternalServerError)
			{
			Close();
			return;
			}
		Request(this, hrequest);
		if(uStatus==HttpConnectionStatus::Closing)
			break;
		if(hcon->Available())
			DataReceived(this);
		hcon->SetTimeout(10000);
		uStatus=HttpConnectionStatus::Content;
		break;
		}
	case HttpConnectionStatus::Response:
		{
		if(!hcon->InputBuffer->Contains("\r\n\r\n", 4))
			{
			hcon->SetTimeout(10000);
			return;
			}
		Handle<HttpResponse> hresponse=new HttpResponse();
		hresponse->ReadFromStream(hcon);
		if(hresponse->Status==HttpStatus::InternalServerError)
			{
			Close();
			return;
			}
		Response(this, hresponse);
		if(uStatus==HttpConnectionStatus::Closing)
			break;
		if(hcon->Available())
			DataReceived(this);
		hcon->SetTimeout(10000);
		uStatus=HttpConnectionStatus::Content;
		break;
		}
	default:
		break;
	}
}

VOID HttpConnection::OnTcpConnectionDataSent(Handle<TcpConnection> hcon)
{
if(uStatus==HttpConnectionStatus::Closing)
	Close();
}

VOID HttpConnection::OnTcpConnectionSend(Handle<TcpConnection> hcon)
{
if(!hMessage)
	return;
CHAR pbuf[512];
while(1)
	{
	if(!hMessage->Seek(uPosition))
		{
		hcon->Flush();
		BOOL bkeepalive=hMessage->KeepAlive;
		hMessage=nullptr;
		if(uStatus==HttpConnectionStatus::Requesting)
			{
			uStatus=HttpConnectionStatus::Response;
			return;
			}
		uStatus=bkeepalive? HttpConnectionStatus::Request: HttpConnectionStatus::Closing;
		return;
		}
	SIZE_T uread=hMessage->Read(pbuf, 512);
	SIZE_T uwritten=hcon->Write(pbuf, uread);
	uPosition+=uwritten;
	if(uwritten<uread)
		break;
	}
}

}}