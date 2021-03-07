//==========
// Grid.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Grid.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//============================
// Con-/Destructors Protected
//============================

Grid::Grid():
Grid(Container::Current)
{}

Grid::Grid(Container* pparent):
Window(pparent),
Control(pparent),
Container(pparent),
Columns(this, 0),
Lines(this, 0)
{
Children->Added.Add(this, &Grid::OnChildAdded);
Columns.Changed.Add(this, &Grid::OnRasterChanged);
Columns.Reading.Add(this, &Grid::OnColumnsReading);
Lines.Changed.Add(this, &Grid::OnRasterChanged);
Lines.Reading.Add(this, &Grid::OnLinesReading);
}


//========
// Common
//========

VOID Grid::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
UpdateRaster(hdc);
StretchRaster(rc);
AbsoluteRaster();
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	if(!hchild->Visible.GetInternal())
		continue;
	UINT ucol=hchild->Column.GetInternal();
	UINT uline=hchild->Line.GetInternal();
	RECT const& margin=hchild->Margin.GetInternal();
	Graphics::RECT rcc({ rc.Left, rc.Top, 0, 0 });
	if(ucol>0)
		rcc.Left=cWidths.GetAt(ucol-1);
	rcc.Left+=margin.Left;
	rcc.Right=cWidths.GetAt(ucol);
	rcc.Right-=margin.Right;
	if(uline>0)
		rcc.Top=cHeights.GetAt(uline-1);
	rcc.Top+=margin.Top;
	rcc.Bottom=cHeights.GetAt(uline);
	rcc.Bottom-=margin.Bottom;
	hchild->Move(rcc);
	}
}


//==================
// Common Protected
//==================

Graphics::SIZE Grid::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Control::GetMinSizeInternal(hdc);
UpdateRaster(hdc);
UINT ucols=cWidths.GetCount();
for(UINT u=0; u<ucols; u++)
	minsize.Width+=cWidths[u];
UINT ulines=cHeights.GetCount();
for(UINT u=0; u<ulines; u++)
	minsize.Height+=cHeights[u];
return minsize;
}


//================
// Common Private
//================

VOID Grid::AbsoluteRaster()
{
RECT const& padding=Padding.GetInternal();
UINT ucols=cWidths.GetCount();
for(UINT u=1; u<ucols; u++)
	{
	UINT uwidth=cWidths.GetAt(u-1);
	uwidth+=cWidths.GetAt(u)+padding.Left;
	cWidths.SetAt(u, uwidth);
	}
UINT ulines=cHeights.GetCount();
for(UINT u=1; u<ulines; u++)
	{
	UINT uheight=cHeights.GetAt(u-1);
	uheight+=cHeights.GetAt(u)+padding.Top;
	cHeights.SetAt(u, uheight);
	}
}

Graphics::SIZE Grid::GetRaster()
{
UINT ucols=Columns.GetInternal();
UINT ulines=Lines.GetInternal();
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	if(!hchild->Visible.GetInternal())
		continue;
	ucols=MAX(ucols, hchild->Column.GetInternal()+1);
	ulines=MAX(ulines, hchild->Line.GetInternal()+1);
	}
ucols=MAX(ucols, 1);
ulines=MAX(ulines, 1);
return { ucols, ulines };
}

VOID Grid::OnChildAdded(Handle<ControlList> hlist, Handle<Control> hchild)
{
hchild->Column.Changed.Add(this, &Grid::OnRasterChanged);
hchild->Line.Changed.Add(this, &Grid::OnRasterChanged);
InvalidateArrangement();
}

VOID Grid::OnColumnsReading(UINT& ucols)
{
SIZE raster=GetRaster();
ucols=raster.Width;
}

VOID Grid::OnLinesReading(UINT& ulines)
{
SIZE raster=GetRaster();
ulines=raster.Height;
}

VOID Grid::OnRasterChanged(UINT ulines)
{
InvalidateArrangement();
}

VOID Grid::StretchRaster(RECT const& rc)
{
RECT const& padding=Padding.GetInternal();
UINT uwidth=rc.Right-rc.Left;
uwidth-=padding.Left+padding.Right;
UINT uminwidth=0;
UINT ucols=cWidths.GetCount();
UINT ufreecols=0;
for(UINT u=0; u<ucols; u++)
	{
	UINT uwidth=cWidths.GetAt(u);
	if(!uwidth)
		ufreecols++;
	uminwidth+=uwidth;
	}
if(uminwidth<uwidth)
	{
	UINT uspace=uwidth-uminwidth;
	if(ufreecols>0)
		{
		for(UINT u=0; u<ucols; u++)
			{
			if(cWidths.GetAt(u))
				continue;
			UINT uwidth=uspace/ufreecols;
			cWidths.SetAt(u, uwidth);
			uspace-=uwidth;
			if(--ufreecols==0)
				break;
			}
		}
	else
		{
		UINT uwidth=cWidths.GetAt(ucols-1);
		cWidths.SetAt(ucols-1, uwidth+uspace);
		}
	}
UINT uheight=rc.Bottom-rc.Top;
uheight-=padding.Top+padding.Bottom;
UINT uminheight=0;
UINT ulines=cHeights.GetCount();
UINT ufreelines=0;
for(UINT u=0; u<ulines; u++)
	{
	UINT uheight=cHeights.GetAt(u);
	if(!uheight)
		ufreelines++;
	uminheight+=uheight;
	}
if(uminheight<uheight)
	{
	UINT uspace=uheight-uminheight;
	if(ufreelines>0)
		{
		for(UINT u=0; u<ulines; u++)
			{
			if(cHeights.GetAt(u))
				continue;
			UINT uheight=uspace/ufreelines;
			cHeights.SetAt(u, uheight);
			uspace-=uheight;
			if(--ufreelines==0)
				break;
			}
		}
	else
		{
		UINT uheight=cHeights.GetAt(ulines-1);
		cHeights.SetAt(ulines-1, uheight+uspace);
		}
	}
}

VOID Grid::UpdateRaster(Handle<DeviceContext> hdc)
{
SIZE raster=GetRaster();
if(cWidths.GetCount()<raster.Width)
	{
	cWidths.Clear();
	cWidths.SetCount(raster.Width);
	}
cWidths.Zero();
if(cHeights.GetCount()<raster.Height)
	{
	cHeights.Clear();
	cHeights.SetCount(raster.Height);
	}
cHeights.Zero();
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	if(!hchild->Visible.GetInternal())
		continue;
	SIZE minsize=hchild->GetMinSize(hdc);
	UINT ucol=hchild->Column.GetInternal();
	RECT const& margin=hchild->Margin.GetInternal();
	minsize.Width+=margin.Left+margin.Right;
	UINT uwidth=cWidths.GetAt(ucol);
	cWidths.SetAt(ucol, MAX(uwidth, minsize.Width));
	UINT uline=hchild->Line.GetInternal();
	minsize.Height+=margin.Top+margin.Bottom;
	UINT uheight=cHeights.GetAt(uline);
	cHeights.SetAt(uline, MAX(uheight, minsize.Height));
	}
}

}}