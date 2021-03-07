//==================
// PacketBuffer.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "PacketBuffer.h"


//===========
// Namespace
//===========

namespace Storage {


//==================
// Con-/Destructors
//==================

PacketBuffer::PacketBuffer(UINT uchunksize):
bFlush(false),
pFirst(nullptr),
pLast(nullptr),
uChunkSize(uchunksize),
uPacketCount(0),
uPosition(0)
{}

PacketBuffer::~PacketBuffer()
{
FreeChunk(pFirst);
}


//========
// Access
//========

SIZE_T PacketBuffer::Available()
{
if(!pFirst)
	return 0;
return pFirst->TotalLength-uPosition;
}

BOOL PacketBuffer::Contains(VOID const* pbufv, SIZE_T usize)
{
BYTE const* pbuf=(BYTE const*)pbufv;
Chunk* chunk=pFirst;
while(chunk)
	{
	BYTE* pchunk=(BYTE*)chunk;
	pchunk+=sizeof(Chunk);
	SIZE_T chunk_len=chunk->ChunkLength;
	for(SIZE_T chunk_pos=0; chunk_pos<chunk_len; chunk_pos++)
		{
		UINT available=chunk_len-chunk_pos;
		UINT compare=MIN(usize, available);
		if(CompareMemory(&pchunk[chunk_pos], pbuf, compare)==0)
			{
			if(compare==usize)
				return true;
			Chunk* next=chunk->Next;
			if(!next)
				return false;
			BYTE* pnext=(BYTE*)next;
			pnext+=sizeof(Chunk);
			return CompareMemory(pnext, &pbuf[compare], usize-compare)==0;
			}
		}
	chunk=chunk->Next;
	}
return false;
}

SIZE_T PacketBuffer::Copy(VOID* pbuf, SIZE_T usize)
{
return ReadInternal(pbuf, usize, false);
}

SIZE_T PacketBuffer::Consume(SIZE_T usize)
{
if(pFirst==nullptr)
	return 0;
SIZE_T upos=0;
while(usize>0)
	{
	Chunk* pchunk=pFirst;
	if(usize<pchunk->ChunkLength-uPosition)
		{
		uPosition+=usize;
		upos+=usize;
		return upos;
		}
	Chunk* pnext=pchunk->Next;
	if(pnext==nullptr)
		{
		upos+=pchunk->ChunkLength;
		Clear();
		return upos;
		}
	SIZE_T uconsume=pchunk->ChunkLength-uPosition;
	if(pchunk->ChunkLength==pchunk->TotalLength)
		uPacketCount--;
	operator delete(pchunk);
	pFirst=pnext;
	pFirst->Previous=nullptr;
	uPosition=0;
	upos+=uconsume;
	usize-=uconsume;
	}
return upos;
}

VOID const* PacketBuffer::GetChunk()const
{
if(pFirst==nullptr)
	return nullptr;
BYTE* pbuf=(BYTE*)pFirst;
pbuf+=sizeof(Chunk)+uPosition;
return pbuf;
}

SIZE_T PacketBuffer::GetChunkSize()const
{
if(pFirst==nullptr)
	return 0;
return pFirst->ChunkLength-uPosition;
}

SIZE_T PacketBuffer::Read(VOID* pbuf, SIZE_T usize)
{
return ReadInternal(pbuf, usize, true);
}

SIZE_T PacketBuffer::WriteToStream(Handle<OutputStream> hstream)
{
UINT upackets=GetPacketCount();
if(upackets==0)
	return 0;
SIZE_T utotal=Available();
for(SIZE_T upos=0; upos<utotal;)
	{
	VOID const* pchunk=GetChunk();
	SIZE_T uchunk=GetChunkSize();
	hstream->Write(pchunk, uchunk);
	Consume(uchunk);
	upos+=uchunk;
	}
return utotal;
}


//==============
// Modification
//==============

VOID PacketBuffer::Clear()
{
FreeChunk(pFirst);
pFirst=nullptr;
pLast=nullptr;
uPacketCount=0;
uPosition=0;
}

VOID PacketBuffer::Flush()
{
if(pFirst==nullptr)
	return;
bFlush=true;
uPacketCount++;
}

SIZE_T PacketBuffer::Write(VOID const* pbuf, SIZE_T usize)
{
ASSERT(pbuf!=nullptr);
ASSERT(usize>0);
if(pFirst==nullptr)
	{
	pFirst=CreateChunk(pbuf, usize, nullptr);
	pLast=pFirst;
	bFlush=false;
	return usize;
	}
if(bFlush)
	{
	Chunk* pchunk=CreateChunk(pbuf, usize, nullptr);
	pLast->Next=pchunk;
	pLast=pchunk;
	bFlush=false;
	return usize;
	}
BYTE const* psrc=(BYTE const*)pbuf;
Chunk* pchunk=pLast;
SIZE_T uchunklen=pchunk->ChunkLength;
SIZE_T uchunksize=BlockAlign(sizeof(Chunk)+uchunklen, uChunkSize);
SIZE_T ufree=uchunksize-uchunklen-sizeof(Chunk);
SIZE_T ucopy=MIN(ufree, usize);
if(ucopy>0)
	{
	BYTE* pdst=(BYTE*)pchunk;
	pdst+=sizeof(Chunk)+uchunklen;
	CopyMemory(pdst, psrc, ucopy);
	pchunk->ChunkLength+=ucopy;
	psrc+=ucopy;
	}
if(ucopy<usize)
	pLast=CreateChunk(psrc, usize-ucopy, pchunk);
while(pchunk!=nullptr)
	{
	pchunk->TotalLength+=usize;
	pchunk=pchunk->Previous;
	}
return usize;
}


//================
// Common Private
//================

PacketBuffer::Chunk* PacketBuffer::CreateChunk(VOID const* pbuf, SIZE_T usize, Chunk* pprev)
{
SIZE_T uchunksize=BlockAlign(sizeof(Chunk)+usize, uChunkSize);
Chunk* pchunk=(Chunk*)operator new(uchunksize);
pchunk->ChunkLength=usize;
pchunk->TotalLength=usize;
pchunk->Previous=pprev;
pchunk->Next=nullptr;
BYTE* pdst=(BYTE*)pchunk;
pdst+=sizeof(Chunk);
CopyMemory(pdst, pbuf, usize);
if(pprev!=nullptr)
	pprev->Next=pchunk;
return pchunk;
}

VOID PacketBuffer::FreeChunk(Chunk* pchunk)
{
while(pchunk!=nullptr)
	{
	Chunk* pnext=pchunk->Next;
	operator delete(pchunk);
	pchunk=pnext;
	}
}

SIZE_T PacketBuffer::ReadInternal(VOID* pbuf, SIZE_T usize, BOOL bconsume)
{
if(pFirst==nullptr)
	return 0;
BYTE* pdst=(BYTE*)pbuf;
SIZE_T utotal=pFirst->TotalLength-uPosition;
SIZE_T ucopy=MIN(usize, utotal);
SIZE_T upos=0;
while(ucopy>0)
	{
	SIZE_T uchunk=pFirst->ChunkLength-uPosition;
	SIZE_T uread=MIN(uchunk, ucopy);
	BYTE* psrc=(BYTE*)pFirst;
	psrc+=sizeof(Chunk)+uPosition;
	if(pdst)
		{
		CopyMemory(pdst, psrc, uread);
		pdst+=uread;
		}
	if(bconsume)
		Consume(uread);
	upos+=uread;
	ucopy-=uread;
	}
return upos;
}

}
