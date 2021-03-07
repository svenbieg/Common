//================
// HttpServer.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "Network/Connection.h"
#include "Runtime/Application.h"
#include "Storage/Streams/StreamWriter.h"
#include "Storage/File.h"
#include "Storage/Intermediate.h"
#include "Web/Directory.h"
#include "Web/Update.h"
#include "Web/WebPage.h"
#include "HttpServer.h"

using namespace Network;
using namespace Runtime;
using namespace Storage;
using namespace Storage::Streams;
using namespace Web;
using namespace Web::Controls;


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//============================
// Con-/Destructors Protected
//============================

HttpServer::HttpServer():
uFileEnd(0),
uFileSize(0),
uFileStart(0),
uStatus(HttpServerStatus::Closed)
{
Current=this;
}

HttpServer::~HttpServer()
{
Close();
}


//========
// Common
//========

VOID HttpServer::Close()
{
if(uStatus==HttpServerStatus::Closed)
	return;
hTcpServer=nullptr;
uStatus=HttpServerStatus::Closed;
}

Handle<HttpServer> HttpServer::Current;

VOID HttpServer::Listen(WORD uport)
{
Close();
hTcpServer=new TcpServer();
hTcpServer->ConnectionReceived.Add(this, &HttpServer::OnConnectionReceived);
hTcpServer->Listen(uport);
uStatus=HttpServerStatus::Listening;
}


//================
// Common Private
//================

VOID HttpServer::OnConnectionClosed(Handle<HttpConnection> hcon)
{
Connections->Remove(hcon);
}

VOID HttpServer::OnConnectionDataReceived(Handle<HttpConnection> hcon)
{
if(!hFile)
	{
	Handle<HttpResponse> hresponse=new HttpResponse(HttpStatus::InternalServerError);
	hcon->Send(hresponse);
	return;
	}
SIZE_T available=hcon->Available();
Handle<Buffer> hbuf=new Buffer(available);
SIZE_T read=hcon->Read(hbuf->Begin(), available);
SIZE_T written=hFile->Write(hbuf->Begin(), read);
if(written!=available)
	{
	hFile=nullptr;
	Handle<HttpResponse> hresponse=new HttpResponse(HttpStatus::InternalServerError);
	hcon->Send(hresponse);
	return;
	}
uFileStart+=written;
if(uFileStart>=uFileSize)
	{
	hFile=nullptr;
	uFileStart=0;
	uFileEnd=0;
	uFileSize=0;
	}
if(uFileStart==uFileEnd)
	{
	Handle<HttpResponse> hresponse=new HttpResponse(HttpStatus::Ok);
	hcon->Send(hresponse);
	}
}

VOID HttpServer::OnConnectionReceived(Handle<TcpServer> hserver, Handle<TcpConnection> htcpcon)
{
Handle<HttpConnection> hcon=new HttpConnection(htcpcon);
hcon->Closed.Add(this, &HttpServer::OnConnectionClosed);
hcon->Request.Add(this, &HttpServer::OnConnectionRequest);
if(!Connections)
	Connections=new ConnectionList();
Connections->Add(hcon);
ConnectionReceived(this, hcon);
}

VOID HttpServer::OnConnectionRequest(Handle<HttpConnection> hcon, Handle<HttpRequest> hrequest)
{
Handle<HttpResponse> hresponse=new HttpResponse(HttpStatus::Ok);
if(hrequest->Path=="/"||hrequest->Path=="/gen_204")
	{
	auto hnetcon=Connection::Current;
	if(hnetcon)
		{
		Handle<String> hhostname=hnetcon->HostName;
		if(hhostname)
			{
			Handle<String> hurl=new String("http://%s/index.html", hhostname->Begin());
			hresponse->Status=HttpStatus::MovedPermanently;
			hresponse->Properties->Add("Location", hurl);
			}
		}
	hcon->Send(hresponse);
	return;
	}
auto method=hrequest->Method;
if(method==HttpMethod::Get||method==HttpMethod::Notify)
	{
	auto hobj=Application::Current->AppData->Get(hrequest->Path);
	if(!hobj)
		{
		hresponse->Status=HttpStatus::SiteNotFound;
		hcon->Send(hresponse);
		return;
		}
	auto hpage=hobj->As<WebPage>("Web.WebPage");
	if(hpage)
		{
		auto hparams=hrequest->Params;
		Handle<String> hupdate;
		UINT uupdate=0;
		if(hparams)
			{
			hupdate=hparams->Get("update");
			if(hupdate)
				hupdate->Scan("%u", &uupdate);
			}
		if(method==HttpMethod::Notify)
			{
			auto hid=hparams->Get("id");
			if(hid)
				{
				auto hcontrol=hpage->Controls->Get(hid);
				if(hcontrol)
					hcontrol->OnNotified(hparams);
				}
			}
		Handle<Intermediate> hbuf=new Intermediate();
		if(method==HttpMethod::Notify||hupdate)
			{
			Handle<Update> hupdate=new Update(hpage, uupdate);
			hupdate->WriteToStream(hbuf);
			}
		else
			{
			hpage->WriteToStream(hbuf);
			}
		hresponse->Content=hbuf;
		hcon->Send(hresponse);
		return;
		}
	auto hdir=hobj->As<Storage::Directory>("Storage.Directory");
	if(hdir)
		{
		Handle<Intermediate> hbuf=new Intermediate();
		Handle<Web::Directory> hwebdir=new Web::Directory(hdir);
		hwebdir->WriteToStream(hbuf);
		hresponse->Content=hbuf;
		hcon->Send(hresponse);
		return;
		}
	auto hfile=hobj->As<Storage::File>("Storage.File");
	if(hfile)
		{
		if(!hfile->Create(FileCreateMode::OpenExisting, FileAccessMode::ReadOnly, FileShareMode::ShareRead))
			{
			hresponse->Status=HttpStatus::InternalServerError;
			hcon->Send(hresponse);
			return;
			}
		hresponse->Content=hfile;
		Handle<String> hdisp=new String("attachment; filename=%s", hfile->GetName()->Begin());
		hresponse->Properties->Add("Content-Disposition", hdisp);
		hcon->Send(hresponse);
		return;
		}
	}
if(method==HttpMethod::Post)
	{
	Handle<String> hrange=hrequest->Properties->Get("Content-Range");
	if(!hrange)
		{
		hresponse->Status=HttpStatus::BadRequest;
		hcon->Send(hresponse);
		return;
		}
	if(hrange->Scan("%u-%u/%u", &uFileStart, &uFileEnd, &uFileSize)!=3)
		{
		hresponse->Status=HttpStatus::BadRequest;
		hcon->Send(hresponse);
		return;
		}
	if(uFileStart==0)
		{
		Handle<String> hname=hrequest->Params->Get("name");
		Handle<String> hpath=new String("%s/%s", hrequest->Path->Begin(), hname->Begin());
		hFile=Application::Current->AppData->CreateFile(hpath, FileCreateMode::CreateNew, FileAccessMode::ReadWrite);
		if(hFile)
			{
			if(!hFile->SetSize(uFileSize))
				hFile=nullptr;
			}
		}
	if(!hFile)
		{
		Handle<HttpResponse> hresponse=new HttpResponse(HttpStatus::InternalServerError);
		hcon->Send(hresponse);
		return;
		}
	hcon->DataReceived.Add(this, &HttpServer::OnConnectionDataReceived);
	return;
	}
hresponse->Status=HttpStatus::InternalServerError;
hcon->Send(hresponse);
}

}}