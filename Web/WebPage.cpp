//=============
// WebPage.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Controls/Form.h"
#include "Network/Http/HttpServer.h"
#include "Storage/Streams/StreamWriter.h"
#include "WebPage.h"

using namespace Network::Http;
using namespace Storage;
using namespace Storage::Streams;
using namespace Web::Controls;


//===========
// Namespace
//===========

namespace Web {


//=========
// Scripts
//=========

const CHAR WebPageScript[]=
"var lastupdate=0;\r\n"
"function updateWebPage() {\r\n"
"var request=new XMLHttpRequest();\r\n"
"request.open('GET', '?update='.concat(lastupdate));\r\n"
"request.addEventListener('load', function(e) {\r\n"
"\tif(request.status==200)\r\n"
"\t\tdocument.dispatchEvent(new CustomEvent('eventUpdateWebPage', { detail: request.responseText }));\r\n"
"\tsetTimeout(updateWebPage, 1000);\r\n"
"\t});\r\n"
"request.send();\r\n"
"}\r\n\r\n"
"function updateWebPageControls(arg) {\r\n"
"if(arg==\"\")\r\n"
"\treturn;\r\n"
"var params=arg.split(';');\r\n"
"for(var i=0; i<params.length; i++) {\r\n"
"\tvar param=params[i];\r\n"
"\tif(param==\"\")\r\n"
"\t\tcontinue;\r\n"
"\tvar op=\"=\";\r\n"
"\tif(param.indexOf(\"+=\")>0)\r\n"
"\t\top=\"+=\";\r\n"
"\tvar pair=param.split(op);\r\n"
"\tvar key=pair[0];\r\n"
"\tvar value=pair[1];\r\n"
"\tif(key=='time')\r\n"
"\t\t{\r\n"
"\t\tlastupdate=value;\r\n"
"\t\tcontinue;\r\n"
"\t\t}\r\n"
"\tvar element=document.getElementById(key);\r\n"
"\tif(element==null)\r\n"
"\t\tcontinue;\r\n"
"\tif(element.tagName.toLocaleLowerCase()=='input') {\r\n"
"\t\tvar edit=element;\r\n"
"\t\tif(op==\"=\")\r\n"
"\t\t\tedit.value=\"\";\r\n"
"\t\tedit.value+=value;\r\n"
"\t\tcontinue;\r\n"
"\t\t}\r\n"
"\tif(element.tagName.toLowerCase()=='table')\r\n"
"\t\telement=element.lastElementChild;\r\n"
"\tif(op==\"=\")\r\n"
"\t\telement.innerHTML=\"\";\r\n"
"\telement.innerHTML+=value;\r\n"
"\t}\r\n"
"}\r\n\r\n"
"var eventWebPageUpdate=new CustomEvent('eventUpdateWebPage', { detail: String });\r\n"
"document.addEventListener('eventUpdateWebPage', function(e) { updateWebPageControls(e.detail); }, false);\r\n"
"window.addEventListener('DOMContentLoaded', function(e) { setTimeout(updateWebPage, 1000); });\r\n\r\n";


//==================
// Con-/Destructors
//==================

WebPage::WebPage(Handle<String> htitle):
Convertible("Web.WebPage"),
hTitle(htitle)
{
Current=this;
Controls=new ControlMap();
Scripts=new Index();
Styles=new Index();
Styles->Add("h2 { margin-top:40px; }\r\n");
Styles->Add("h3 { margin-bottom:10px; }\r\n");
Styles->Add("input.num { text-align:right; }\r\n");
Styles->Add("table { border-collapse:collapse; }\r\n");
Styles->Add("table.blank { border:0; }\r\n");
Styles->Add("table.btn { border:0; margin-top:8px; }\r\n");
Styles->Add("table.btn td { padding-left:0; padding-right:8px; }\r\n");
Styles->Add("table.simple { border:1px solid #808080; }\r\n");
Styles->Add("table.simple td { border:1px solid #A0A0A0; padding-left:4px; padding-right:4px; vertical-align:middle; }\r\n");
Styles->Add("table.simple td.static { background-color:#F0F0F0; padding-right:20px; }\r\n");
Styles->Add("table.simple td.text { padding-right:20px; }\r\n");
Styles->Add("table.simple td.num { padding-left:20px; text-align:right; }\r\n");
Styles->Add("td.right { text-align:right }\r\n");
hForm=new Form();
auto hserver=HttpServer::Current;
if(!hserver)
	{
	hserver=new HttpServer();
	hserver->Listen(80);
	}
}


//========
// Common
//========

Handle<WebPage> WebPage::Current;

VOID WebPage::Select()
{
hForm->Select();
}

SIZE_T WebPage::WriteToStream(Handle<OutputStream> hstream)
{
StreamWriter writer(hstream);
SIZE_T usize=0;
usize+=writer.Print("<!DOCTYPE html>\r\n");
usize+=writer.Print("<html>\r\n<head>\r\n<title>");
usize+=writer.Print(hTitle);
usize+=writer.Print("</title>\r\n");
usize+=writer.Print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n");
usize+=writer.Print("<link rel=\"icon\" type=\"image/x-icon\" href=\"favicon.ico\">\r\n");
usize+=writer.Print("<style type=\"text/css\">\r\n");
for(auto hit=Styles->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hstyle=hit->GetCurrent();
	usize+=writer.Print(hstyle);
	}
usize+=writer.Print("</style>\r\n");
usize+=writer.Print("<script language=\"javascript\" type=\"text/javascript\">\r\n\r\n");
usize+=writer.Print(WebPageScript);
usize+=writer.Print("\r\n");
for(auto hit=Scripts->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hscript=hit->GetCurrent();
	usize+=writer.Print(hscript);
	}
for(auto hit=Controls->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hcontrol=hit->GetCurrentItem();
	usize+=hcontrol->WriteScriptToStream(hstream);
	}
usize+=hForm->WriteScriptToStream(hstream);
usize+=writer.Print("</script>\r\n");
usize+=writer.Print("</head>\r\n\r\n<body>\r\n");
usize+=hForm->WriteToStream(hstream);
usize+=writer.Print("</body>\r\n</html>\r\n");
return usize;
}

}