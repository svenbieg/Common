//===========
// Graph.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Web/WebPage.h"
#include "Graph.h"

using namespace Graphics;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Graph::Graph(Handle<Container> hparent, Handle<String> hid):
Control(hparent),
Divisor(1),
FontColor(Colors::Black),
FontFamily("Segoe UI"),
FontSize(15),
GridColor(Colors::LightGray),
Id(hid),
Offset(0)
{
WebPage::Current->Controls->Add(Id, this);
AxisX=new Axis();
AxisY=new Axis();
Rows=new RowMap();
}

Graph::~Graph()
{
WebPage::Current->Controls->Remove(Id);
}


//========
// Common
//========

SIZE_T Graph::WriteScriptToStream(Handle<OutputStream> hstream)
{
StreamWriter writer(hstream);
SIZE_T size=0;
size+=writer.Print("function draw%s() {\r\n", Id->Begin());
SIZE_T uaxisxcount=AxisX->Points->GetCount();
INT minx=0;
INT maxx=0;
if(uaxisxcount>0)
	{
	size+=writer.Print("var AxisX=[ ");
	UINT uaxisx=0;
	for(auto hit=AxisX->Points->First(); hit->HasCurrent(); hit->MoveNext())
		{
		auto x=hit->GetCurrentId();
		auto hstr=hit->GetCurrentItem();
		if(uaxisx++>0)
			size+=writer.Print(", ");
		size+=writer.Print("[ %i, '%s']", x, hstr->Begin());
		minx=MIN(minx, x);
		maxx=MAX(maxx, x);
		}
	size+=writer.Print("];\r\n");
	}
SIZE_T uaxisycount=AxisY->Points->GetCount();
INT miny=0;
INT maxy=0;
if(uaxisycount>0)
	{
	size+=writer.Print("var AxisY=[ ");
	UINT uaxisy=0;
	for(auto hit=AxisY->Points->First(); hit->HasCurrent(); hit->MoveNext())
		{
		auto y=hit->GetCurrentId();
		auto hstr=hit->GetCurrentItem();
		if(uaxisy++>0)
			size+=writer.Print(", ");
		size+=writer.Print("[ %i, '%s']", y, hstr->Begin());
		miny=MIN(miny, y);
		maxy=MAX(maxy, y);
		}
	size+=writer.Print("];\r\n");
	}
SIZE_T urowcount=Rows->GetCount();
if(urowcount>0)
	{
	for(auto hrows=Rows->First(); hrows->HasCurrent(); hrows->MoveNext())
		{
		auto hid=hrows->GetCurrentId();
		auto hrow=hrows->GetCurrentItem();
		size+=writer.Print("var %s=", hid->Begin());
		size+=hrow->PrintToStream(hstream, Divisor, miny, maxy, Offset);
		size+=writer.Print(";\r\n");
		}
	size+=writer.Print("var Rows=[ ");
	UINT urow=0;
	for(auto hit=Rows->First(); hit->HasCurrent(); hit->MoveNext())
		{
		if(urow++>0)
			size+=writer.Print(", ");
		auto hid=hit->GetCurrentId();
		size+=writer.Print(hid->Begin());
		}
	size+=writer.Print(" ];\r\n");
	size+=writer.Print("var RowColors=[ ");
	urow=0;
	for(auto hit=Rows->First(); hit->HasCurrent(); hit->MoveNext())
		{
		if(urow++>0)
			size+=writer.Print(", ");
		size+=writer.Print("'");
		auto hrow=hit->GetCurrentItem();
		auto hcolor=hrow->Color.ToString();
		size+=writer.Print(hcolor);
		size+=writer.Print("'");
		}
	size+=writer.Print(" ];\r\n");
	size+=writer.Print("var RowNames=[ ");
	urow=0;
	for(auto hit=Rows->First(); hit->HasCurrent(); hit->MoveNext())
		{
		if(urow++>0)
			size+=writer.Print(", ");
		size+=writer.Print("'");
		auto hrow=hit->GetCurrentItem();
		size+=writer.Print(hrow->Name);
		size+=writer.Print("'");
		}
	size+=writer.Print(" ];\r\n");
	}
size+=writer.Print("var canvas=document.getElementById('%s');\r\n", Id->Begin());
//size+=writer.Print("var rc=canvas.getBoundingClientRect();\r\n");
size+=writer.Print("canvas.width=800;\r\n");
size+=writer.Print("canvas.height=500;\r\n");
size+=writer.Print("var width=canvas.width-1;\r\n");
size+=writer.Print("var height=canvas.height-1;\r\n");
size+=writer.Print("var left=0;\r\n");
size+=writer.Print("var top=0;\r\n");
size+=writer.Print("var minx=%i;\r\n", minx);
size+=writer.Print("var maxx=%i;\r\n", maxx);
size+=writer.Print("var div=%i;\r\n", Divisor);
size+=writer.Print("var miny=%i;\r\n", miny);
size+=writer.Print("var maxy=%i;\r\n", maxy);
size+=writer.Print("var fontsize=%u;\r\n", FontSize);
size+=writer.Print("var marginx=8;");
size+=writer.Print("var marginy=8;\r\n");
auto hfontcolor=FontColor.ToString();
size+=writer.Print("var fontcolor='%s';\r\n", hfontcolor->Begin());
auto hgridcolor=GridColor.ToString();
size+=writer.Print("var gridcolor='%s';\r\n", hgridcolor->Begin());
size+=writer.Print("var context=canvas.getContext('2d');\r\n");
size+=writer.Print("context.clearRect(0, 0, width, height);\r\n");
size+=writer.Print("context.font='%upx %s';\r\n", FontSize, FontFamily->Begin());
if(urowcount>0)
	{
	size+=writer.Print("var namecount=RowNames.length;\r\n");
	size+=writer.Print("var maxnamewidth=0;\r\n");
	size+=writer.Print("RowNames.forEach(function(name) {\r\n");
	size+=writer.Print("\tvar tm=context.measureText(name);\r\n");
	size+=writer.Print("\tif(tm.width>maxnamewidth)\r\n");
	size+=writer.Print("\t\tmaxnamewidth=tm.width\r\n");
	size+=writer.Print("\t});\r\n");
	size+=writer.Print("width-=fontsize+marginx+maxnamewidth;\r\n");
	size+=writer.Print("var urow=0;\r\n");
	size+=writer.Print("var y=top+(height-namecount*fontsize-(namecount-1)*marginy)/2;\r\n");
	size+=writer.Print("RowNames.forEach(function(name) {\r\n");
	size+=writer.Print("\tcontext.fillStyle=RowColors[urow];\r\n");
	size+=writer.Print("\tcontext.fillRect(width, y, fontsize, fontsize);\r\n");
	size+=writer.Print("\tcontext.fillStyle=fontcolor;\r\n");
	size+=writer.Print("\tcontext.fillText(name, width+fontsize+marginx, y+fontsize-fontsize/8);\r\n");
	size+=writer.Print("\ty+=fontsize+marginy\r\n");
	size+=writer.Print("\turow++\r\n");
	size+=writer.Print("\t});\r\n");
	}
size+=writer.Print("context.fillStyle=fontcolor;\r\n");
size+=writer.Print("context.strokeStyle=gridcolor;\r\n");
if(AxisX->Name)
	{
	if(AxisX->Unit)
		{
		size+=writer.Print("var namex='%s/%s';\r\n", AxisX->Name->Begin(), AxisX->Unit->Begin());
		}
	else
		{
		size+=writer.Print("var namex='%s';\r\n", AxisX->Name->Begin());
		}
	size+=writer.Print("var tmnamex=context.measureText(namex);\r\n");
	size+=writer.Print("width-=marginx+tmnamex.width;\r\n");
	size+=writer.Print("context.fillText(namex, width+marginx, height);\r\n");
	}
if(AxisY->Name)
	{
	if(AxisY->Unit)
		{
		size+=writer.Print("var namey='%s/%s';\r\n", AxisY->Name->Begin(), AxisY->Unit->Begin());
		}
	else
		{
		size+=writer.Print("var namey='%s';\r\n", AxisY->Name->Begin());
		}
	size+=writer.Print("var tmnamey=context.measureText(namey);\r\n");
	size+=writer.Print("left+=tmnamey.width+marginx;\r\n");
	size+=writer.Print("width-=tmnamey.width+marginx;\r\n");
	size+=writer.Print("context.fillText(namey, 0, fontsize);\r\n");
	}
if(uaxisxcount>0)
	{
	size+=writer.Print("var pt=AxisX[AxisX.length-1];\r\n");
	size+=writer.Print("var tm=context.measureText(pt[1]);");
	size+=writer.Print("width-=tm.width/2;\r\n");
	size+=writer.Print("height-=fontsize+marginy;\r\n");
	}
if(uaxisycount>0)
	{
	size+=writer.Print("var maxtextwidth=0;\r\n");
	size+=writer.Print("AxisY.forEach(function(pt) {\r\n");
	size+=writer.Print("\tvar tm=context.measureText(pt[1]);\r\n");
	size+=writer.Print("\tif(tm.width>maxtextwidth)\r\n");
	size+=writer.Print("\t\tmaxtextwidth=tm.width;\r\n");
	size+=writer.Print("\t});\r\n");
	size+=writer.Print("width-=maxtextwidth+marginx;\r\n");
	size+=writer.Print("height-=fontsize/2;\r\n");
	size+=writer.Print("left+=maxtextwidth+marginx;\r\n");
	size+=writer.Print("top+=fontsize/2;\r\n");
	}
if(uaxisxcount>0)
	{
	size+=writer.Print("context.beginPath();\r\n");
	size+=writer.Print("AxisX.forEach(function(pt) {\r\n");
	size+=writer.Print("\tvar x=left+pt[0]/(maxx-minx)*width;\r\n");
	size+=writer.Print("\tcontext.moveTo(x, top);\r\n");
	size+=writer.Print("\tcontext.lineTo(x, top+height);\r\n");
	size+=writer.Print("\tvar tm=context.measureText(pt[1]);");
	size+=writer.Print("\tcontext.fillText(pt[1], x-tm.width/2, top+height+fontsize+marginy);\r\n");
	size+=writer.Print("\t});\r\n");
	size+=writer.Print("context.stroke();\r\n");
	}
if(uaxisycount>0)
	{
	size+=writer.Print("context.beginPath();\r\n");
	size+=writer.Print("AxisY.forEach(function(pt) {\r\n");
	size+=writer.Print("\tvar y=top+height-(pt[0]-miny)/(maxy-miny)*height;\r\n");
	size+=writer.Print("\tcontext.moveTo(left, y);\r\n");
	size+=writer.Print("\tcontext.lineTo(left+width, y);\r\n");
	size+=writer.Print("\tvar tm=context.measureText(pt[1]);");
	size+=writer.Print("\tcontext.fillText(pt[1], left-tm.width-marginx, y+fontsize/2);\r\n");
	size+=writer.Print("\t});\r\n");
	size+=writer.Print("context.stroke();\r\n");
	}
size+=writer.Print("var urow=0;\r\n");
size+=writer.Print("Rows.forEach(function(row) {\r\n");
size+=writer.Print("\tcontext.fillStyle=RowColors[urow];\r\n");
size+=writer.Print("\tcontext.strokeStyle=RowColors[urow];\r\n");
size+=writer.Print("\tcontext.beginPath();\r\n");
size+=writer.Print("\tvar wasoutside=true;\r\n");
size+=writer.Print("\tvar upos=0;\r\n");
size+=writer.Print("\trow.forEach(function(value) {\r\n");
size+=writer.Print("\t\tvar x=left+upos*div/(maxx-minx)*width;\r\n");
size+=writer.Print("\t\tvar y=top+height-(value-miny)/(maxy-miny)*height;\r\n");
size+=writer.Print("\t\tvar outside=(value<miny||value>maxy);\r\n");
size+=writer.Print("\t\tif(wasoutside||outside)\r\n");
size+=writer.Print("\t\t\t{\r\n");
size+=writer.Print("\t\t\tif(!outside){ context.fillRect(x, y, 1, 1); }\r\n");
size+=writer.Print("\t\t\tcontext.moveTo(x, y);\r\n");
size+=writer.Print("\t\t\t}\r\n");
size+=writer.Print("\t\telse { context.lineTo(x, y); }\r\n");
size+=writer.Print("\t\twasoutside=outside;\r\n");
size+=writer.Print("\t\tupos++;\r\n");
size+=writer.Print("\t\t});\r\n");
size+=writer.Print("\tcontext.stroke();\r\n");
size+=writer.Print("\turow++;\r\n");
size+=writer.Print("\t});\r\n");
size+=writer.Print("}\r\n");
size+=writer.Print("window.addEventListener(\"DOMContentLoaded\", function(e) { draw%s(); });\r\n\r\n", Id->Begin());
return size;
}

SIZE_T Graph::WriteToStream(Handle<OutputStream> hstream)
{
StreamWriter writer(hstream);
SIZE_T usize=0;
usize+=writer.Print("<canvas id=\"");
usize+=writer.Print(Id);
usize+=writer.Print("\"");
usize+=WriteControlAttributes(hstream);
usize+=writer.Print("></canvas>");
usize+=WriteBreaks(hstream);
return usize;
}

}}