//===============
// Directory.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Network/Connection.h"
#include "Storage/Streams/StreamWriter.h"
#include "Storage/File.h"
#include "Directory.h"

using namespace Storage;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {


//=============
// Java-Script
//=============

LPCSTR DirectoryScript=
	"const chunk_size=1024*1024;\r\n\r\n"
	"function sendChunk(file, start)\r\n"
	"{\r\n"
	"var name=encodeURIComponent(file.name);\r\n"
	"var end=start+chunk_size;\r\n"
	"if(end>file.size)\r\n"
	"\tend=file.size;\r\n"
	"var progress=document.getElementById('UploadProgress');\r\n"
	"var request=new XMLHttpRequest();\r\n"
	"request.open('POST', '?name='.concat(name));\r\n"
	"request.setRequestHeader('Content-Range', start+'-'+end+'/'+file.size);\r\n"
	"request.onload=function(e)\r\n"
	"\t{\r\n"
	"\tif(request.status!=200)\r\n"
	"\t\tend=file.size+1\r\n"
	"\tif(end>=file.size)\r\n"
	"\t\t{\r\n"
	"\t\tlocation.reload();\r\n"
	"\t\treturn;\r\n"
	"\t\t}\r\n"
	"\tsendChunk(file, end);\r\n"
	"\t}\r\n"
	"request.upload.onprogress=function(e)\r\n"
	"\t{\r\n"
	"\tprogress.value=(start+e.loaded)*100/file.size\r\n"
	"\t}\r\n"
	"request.send(file.slice(start, end));\r\n"
	"}\r\n\r\n"
	"function onUploadInputChanged(e)\r\n"
	"{\r\n"
	"var file=e.target.files[0];\r\n"
	"var sel=document.getElementById('UploadInput');"
	"sel.value='';\r\n"
	"var progress=document.getElementById('UploadProgress');\r\n"
	"progress.style='visibility:visible';\r\n"
	"sendChunk(file, 0)\r\n"
	"}\r\n\r\n"
	"function onDragOver(e)\r\n"
	"{\r\n"
	"e.stopPropagation();\r\n"
	"e.preventDefault();\r\n"
	"e.dataTransfer.dropEffect='copy';\r\n"
	"}\r\n\r\n"
	"document.addEventListener(\"DOMContentLoaded\", function ()\r\n"
	"{\r\n"
	"document.getElementById('UploadInput').addEventListener('change', onUploadInputChanged, false);\r\n"
	"document.addEventListener('dragover', onDragOver, false);"
	"document.addEventListener('drop', onUploadInputChanged, false);"
	"});\r\n\r\n";


//==================
// Con-/Destructors
//==================

Directory::Directory(Handle<Storage::Directory> hdir):
hDirectory(hdir)
{}


//========
// Common
//========

SIZE_T Directory::WriteToStream(Handle<OutputStream> hstream)
{
StreamWriter writer(hstream);
SIZE_T usize=0;
usize+=writer.Print("<!DOCTYPE html>\r\n");
usize+=writer.Print("<html>\r\n<head>\r\n<title>");
auto hhost=Network::Connection::Current->HostName;
auto hpath=hDirectory->GetPath();
Handle<String> hfullpath=new String("%s/%s", hhost->Begin(), hpath->Begin());
usize+=writer.Print(hfullpath);
usize+=writer.Print("</title>\r\n");
usize+=writer.Print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\r\n");
usize+=writer.Print("<style type=\"text/css\">\r\n");
usize+=writer.Print("a { color:blue; text-decoration:none; }\r\n");
usize+=writer.Print("a:link { color:blue; }\r\n");
usize+=writer.Print("a:visited { color:blue; }\r\n");
usize+=writer.Print("td.name { padding-right:8px; text-align:left; }\r\n");
usize+=writer.Print("td.size { text-align:right; }\r\n");
usize+=writer.Print("th { text-align:left; }\r\n");
usize+=writer.Print("</style>\r\n\r\n");
usize+=writer.Print("<script language=\"javascript\" type=\"text/javascript\">\r\n\r\n");
usize+=writer.Print(DirectoryScript);
usize+=writer.Print("</script>\r\n\r\n");
usize+=writer.Print("</head>\r\n\r\n<body>\r\n\r\n<h1>");
usize+=writer.Print(hfullpath);
usize+=writer.Print("</h1>\r\n\r\n<p>\r\n<table>\r\n");
auto hparent=hDirectory->Parent;
if(hparent)
	{
	usize+=writer.Print("  <tr><th><a href=\"/");
	usize+=writer.Print(hparent->GetPath());
	usize+=writer.Print("\">..</a></th></tr>\r\n");
	}
else
	{
	usize+=writer.Print("  <tr><th><a href=\"index.html\">..</a></th></tr>\r\n");
	}
for(auto hit=hDirectory->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hitem=hit->GetCurrent();
	auto hdir=hitem->As<Storage::Directory>("Storage.Directory");
	if(!hdir)
		continue;
	usize+=writer.Print("  <tr>");
	usize+=writer.Print("<th><a href=\"");
	usize+=writer.Print(hdir->GetPath());
	usize+=writer.Print("\">");
	usize+=writer.Print(hdir->GetName());
	usize+=writer.Print("</a></th>");
	usize+=writer.Print("</tr>\r\n");
	}
usize+=writer.Print("</table>\r\n</p>\r\n\r\n");
usize+=writer.Print("<p>\r\n<table>\r\n");
for(auto hit=hDirectory->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hitem=hit->GetCurrent();
	if(hitem->HasType("Storage.Directory"))
		continue;
	auto hfile=hitem->As<File>("Storage.File");
	if(!hfile)
		continue;
	if(!hfile->Create())
		continue;
	usize+=writer.Print("  <tr>");
	usize+=writer.Print("<td class=\"name\"><a href=\"");
	usize+=writer.Print(hpath);
	usize+=writer.Print("/");
	auto hname=hfile->GetName();
	usize+=writer.Print(hname);
	usize+=writer.Print("\" download>");
	usize+=writer.Print(hname);
	usize+=writer.Print("</a></td>");
	usize+=writer.Print("<td class=\"size\">");
	auto size=hfile->GetSize();
	usize+=writer.Print(size.ToString());
	usize+=writer.Print("</td>");
	usize+=writer.Print("</tr>\r\n");
	}
usize+=writer.Print("</table><br />\r\n");
usize+=writer.Print("<input type=\"file\" id=\"UploadInput\" name=\"files[]\" style=\"display: none;\" /><br />");
usize+=writer.Print("<input type=\"button\" value=\"Datei hinzufügen...\" onclick=\"document.getElementById('UploadInput').click();\" />");
usize+=writer.Print("<br /><br />");
usize+=writer.Print("<progress id=\"UploadProgress\" min=\"0\" max=\"100\" value=\"0\" style=\"visibility:hidden\" />");
usize+=writer.Print("</p>\r\n");
usize+=writer.Print("</body>\r\n</html>\r\n");
return usize;
}

}