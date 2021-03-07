//===================
// DeviceContext.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#include "Emulation/Font.h"
#include "DeviceContext.h"


//===========
// Namespace
//===========

namespace Graphics {


//==================
// Con-/Destructors
//==================

DeviceContext::DeviceContext():
Clip(0, 0, 0, 0),
Offset(0, 0)
{}


//========
// Common
//========

VOID DeviceContext::Clear(COLOR c)
{
if(hTarget)
	hTarget->Clear(c);
}

VOID DeviceContext::DrawText(Handle<String> htext, COLOR c)
{
if((UINT)c<=0xFFFFFF)
	return;
auto hfont=Font->As<Graphics::Emulation::Font>("Graphics.Emulation.Font");
if(!hTarget||!htext||!hfont)
	return;
UINT ulen=htext->GetLength();
if(ulen==0)
	return;
LPCTSTR pstr=htext->Begin();
UINT ufontsize=hfont->GetHeight();
UINT uwidth=hTarget->GetWidth();
UINT uheight=hTarget->GetHeight();
RECT rc(0, 0, uwidth, uheight);
if(!AdjustRect(rc))
	return;
INT ileft=Offset.Left;
for(UINT u=0; u<ulen; u++)
	{
	if(ileft>=Clip.Right)
		break;
	BYTE uchar=(BYTE)pstr[u];
	auto pcharinfo=hfont->GetCharInfo(uchar);
	BYTE const* pchar=pcharinfo->Data;
	UINT ucharwidth=pcharinfo->Size/ufontsize;
	INT icharleft=ileft;
	INT icharright=ileft+ucharwidth;
	ileft=icharright;
	if(icharright<Clip.Left)
		continue;
	for(UINT ucy=0; ucy<ufontsize; ucy++)
		{
		INT y=Offset.Top+ucy;
		if(y<Clip.Top)
			continue;
		if(y>=Clip.Bottom)
			break;
		for(UINT ucx=0; ucx<ucharwidth; ucx++)
			{
			INT x=icharleft+ucx;
			if(x<Clip.Left)
				continue;
			if(x>=Clip.Right)
				continue;
			if(pchar[ucy*ucharwidth+ucx])
				hTarget->SetPixel(x, y, c);
			}
		}
	}
}

VOID DeviceContext::FillRect(RECT const& rc, COLOR c)
{
if(c<=0xFFFFFF)
	return;
if(!hTarget)
	return;
RECT rcc(rc);
if(!AdjustRect(rcc))
	return;
hTarget->FillRect(rcc, c);
}

SIZE DeviceContext::MeasureText(Handle<Graphics::Font> hfont, Handle<String> htext)
{
SIZE size(0, 0);
auto hefont=hfont->As<Graphics::Emulation::Font>("Graphics.Emulation.Font");
if(!hefont)
	return size;
size.Height=hefont->GetHeight();
if(!htext)
	return size;
UINT ulen=htext->GetLength();
if(ulen==0)
	return size;
LPCTSTR pstr=htext->Begin();
for(UINT u=0; u<ulen; u++)
	{
	auto pci=hefont->GetCharInfo((BYTE)pstr[u]);
	UINT ucharwidth=pci->Size/size.Height;
	size.Width+=ucharwidth;
	}
return size;
}

VOID DeviceContext::SetPixel(INT ileft, INT itop, COLOR c)
{
if(!hTarget)
	return;
RECT rc(0, 0, 1, 1);
if(!AdjustRect(rc))
	return;
hTarget->SetPixel(rc.Left, rc.Top, c);
}


//==================
// Common Protected
//==================

BOOL DeviceContext::AdjustRect(RECT& rc)
{
rc.Left+=Offset.Left;
rc.Right+=Offset.Left;
rc.Top+=Offset.Top;
rc.Bottom+=Offset.Top;
rc.Left=MAX(rc.Left, Clip.Left);
rc.Top=MAX(rc.Top, Clip.Top);
rc.Right=MIN(rc.Right, Clip.Right);
rc.Bottom=MIN(rc.Bottom, Clip.Bottom);
return rc.Right>rc.Left&&rc.Bottom>rc.Top;
}

}