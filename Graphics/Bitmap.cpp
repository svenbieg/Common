//============
// Bitmap.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Bitmap.h"


//===========
// Namespace
//===========

namespace Graphics {


//========
// Common
//========

VOID Bitmap::Clear(COLOR c)
{
if(!pBuffer)
	return;
UINT* pbuf=(UINT*)pBuffer;
UINT usize=uSize/(UINT)sizeof(UINT);
switch(uFormat)
	{
	case BitmapFormat::Monochrome:
		{
		UINT ucolor=0;
		if(c.GetMonochrome())
			ucolor=0xFFFFFFFF;
		for(UINT u=0; u<usize; u++)
			pbuf[u]=ucolor;
		break;
		}
	case BitmapFormat::RGB:
		{
		UINT r=c.GetRed();
		UINT g=c.GetGreen();
		UINT b=c.GetBlue();
		UINT ucolor1=(b<<24)|(r<<16)|(g<<8)|b;
		UINT ucolor2=(g<<24)|(b<<16)|(r<<8)|g;
		UINT ucolor3=(r<<24)|(g<<16)|(b<<8)|r;
		for(UINT u=0; u<usize; u+=3)
			pbuf[u]=ucolor1;
		for(UINT u=1; u<usize; u+=3)
			pbuf[u]=ucolor2;
		for(UINT u=2; u<usize; u+=3)
			pbuf[u]=ucolor3;
		break;
		}
	case BitmapFormat::RGBA:
		{
		UINT ucolor=c.Reverse();
		for(UINT u=0; u<usize; u++)
			pbuf[u]=ucolor;
		break;
		}
	default:
		break;
	}
}

VOID Bitmap::FillRect(RECT const& rc, COLOR c)
{
if(c<=0xFFFFFF)
	return;
RECT rcc(rc);
rcc.Left=MAX(rcc.Left, 0);
rcc.Top=MAX(rcc.Top, 0);
rcc.Right=MIN(rcc.Right, (INT)uWidth);
rcc.Bottom=MIN(rcc.Bottom, (INT)uHeight);
for(INT y=rcc.Top; y<rcc.Bottom; y++)
	{
	for(INT x=rcc.Left; x<rcc.Right; x++)
		SetPixel(x, y, c);
	}
}

COLOR Bitmap::GetPixel(UINT uleft, UINT utop)
{
COLOR c=Colors::Black;
switch(uFormat)
	{
	case BitmapFormat::Monochrome:
		{
		UINT upos=(utop/8)*uWidth+uleft;
		BYTE umod=(BYTE)(1<<(utop&7));
		c.SetMonochrome(pBuffer[upos]&umod);
		break;
		}
	case BitmapFormat::RGB:
		{
		UINT upos=(utop*uWidth+uleft)*3;
		CopyMemory(&pBuffer[upos], &c, 3);
		break;
		}
	case BitmapFormat::RGBA:
		{
		COLOR* pbuf=(COLOR*)pBuffer;
		UINT upos=utop*uWidth+uleft;
		c=pbuf[upos];
		break;
		}
	default:
		break;
	}
return c;
}

VOID Bitmap::SetPixel(UINT uleft, UINT utop, COLOR c)
{
switch(uFormat)
	{
	case BitmapFormat::Monochrome:
		{
		UINT upos=(utop/8)*uWidth+uleft;
		BYTE umod=(BYTE)(1<<(utop&7));
		if(c.GetMonochrome())
			{
			pBuffer[upos]|=umod;
			}
		else
			{
			pBuffer[upos]&=(BYTE)(~umod);
			}
		break;
		}
	case BitmapFormat::RGB:
		{
		UINT upos=(utop*uWidth+uleft)*3;
		pBuffer[upos++]=c.GetBlue();
		pBuffer[upos++]=c.GetGreen();
		pBuffer[upos++]=c.GetRed();
		break;
		}
	case BitmapFormat::RGBA:
		{
		COLOR* pbuf=(COLOR*)pBuffer;
		UINT upos=utop*uWidth+uleft;
		pbuf[upos]+=c.Reverse();
		break;
		}
	default:
		break;
	}
}


//============================
// Con-/Destructors Protected
//============================

Bitmap::Bitmap():
pBuffer(nullptr),
uFormat(BitmapFormat::Unknown),
uHeight(0),
uPitch(0),
uSize(0),
uWidth(0)
{}


//==================
// Common Protected
//==================

WORD Bitmap::BitmapFormatToBits(BitmapFormat format)
{
switch(format)
	{
	case BitmapFormat::Monochrome:
		return 1;
	case BitmapFormat::RGB:
		return 24;
	case BitmapFormat::RGBA:
		return 32;
	default:
		break;
	}
return 0;
}

BitmapFormat Bitmap::BitmapFormatFromBits(WORD ubpp)
{
switch(ubpp)
	{
	case 1:
		return BitmapFormat::Monochrome;
	case 24:
		return BitmapFormat::RGB;
	case 32:
		return BitmapFormat::RGBA;
	default:
		break;
	}
return BitmapFormat::RGB;
}

}