//==========
// Vector.h
//==========

#pragma once


//=======
// Using
//=======

#include "Collections/Vector.h"
#include "Runtime/Application.h"
#include "Storage/Database/Database.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Collections {


//========
// Vector
//========

template <class _item_t>
class Vector: public ::Collections::Vector<_item_t>
{
private:
	// Using
	using _vector_t=::Collections::Vector<_item_t>;
	using Application=Runtime::Application;

public:
	// Con-/Destructors
	Vector(Handle<String> Path, SIZE_T Count):
		_vector_t(Count)
		{
		hFile=Application::Current->AppData->CreateFile(Path, FileCreateMode::OpenAlways, FileAccessMode::ReadWrite);
		if(!hFile)
			return;
		if(!hFile->SetSize(Count*sizeof(_item_t)))
			{
			hFile=nullptr;
			return;
			}
		hFile->Read(this->pItems, Count*sizeof(_item_t));
		}

	// Modification
	VOID Clear()override { Fill(-1); }
	VOID Fill(_item_t const& Item)override
		{
		_vector_t::Fill(Item);
		if(hFile)
			{
			hFile->SetSize(0);
			hFile->Write(this->pItems, this->uCount*sizeof(_item_t));
			hFile->Flush();
			}
		}
	BOOL SetAt(SIZE_T Position, _item_t const& Item)override
		{
		if(!_vector_t::SetAt(Position, Item))
			return false;
		if(hFile)
			{
			hFile->Seek(Position*sizeof(_item_t));
			hFile->Write(&Item, sizeof(_item_t));
			hFile->Flush();
			}
		return true;
		}

private:
	// Common
	Handle<File> hFile;
};

}}