//=========
// Email.h
//=========

#pragma once


//===========
// Namespace
//===========

namespace Network {
	namespace Smtp {


//========
// E-Mail
//========

class Email: public Object
{
public:
	// Con-/Destructors
	Email() {}

	// Common
	Handle<String> From;
	Handle<String> Subject;
	Handle<String> Text;
	Handle<String> To;
};

}}