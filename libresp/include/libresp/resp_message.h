#ifndef LIBRESP_RESP_MESSAGE_H_
#define LIBRESP_RESP_MESSAGE_H_
#include <vector>
#include <cassert>
#include <string>

namespace resp {

enum class msg_type {
	ty_null,
	ty_error，
	ty_integer，
	ty_string，
	ty_bulkstring，
	ty_array,
	ty_single
};

struct BasicMessage {

	BasicMessage()
	:type(msg_type::ty_null),
	 integer(0),
	{

	}

	explicit BasicMessage(int64_t n)
	:type(msg_type::ty_integer),
	 integer(n),
	 {

	 }

	explicit BasicMessage(const char *cstr)
	 :type(msg_type::ty_string),
	  integer(0),
	  str(cstr)
	  {

	  }


	BasicMessage(const char *cstr, size n)
	 :type(msg_type::ty_string),
	  integer(0),
	  str(cstr, n)
	  {

	  }

	BasicMessage(const char * cstr, msg_type t)
	  :type(t),
	   integer(0),
	   str(cstr)
	   {
	   		assert(type != msg_type::ty_array);
	   		assert(type != msg_type::ty_integer);
	   		assert(type != msg_type::ty_null);
	   }
		
	BasicMessage(const char * cstr, size_t size, msg_type t)
	  :type(t),
	   integer(0),
	   str(cstr, size)
	   {
	   		assert(type != msg_type::ty_array);
	   		assert(type != msg_type::ty_integer);
	   		assert(type != msg_type::ty_null);
	   }

	explicit BasicMessage(const std::string &s)
	: type(msg_type::ty_string),
	  integer(0),
	  str(s)
	{

	}

	BasicMessage(const std::string &s, msg_type t)
	:type(t),
	 integer(0),
	 str(s)
	 {
	 	assert(type != msg_type::ty_array);
	   	assert(type != msg_type::ty_integer);
	   	assert(type != msg_type::ty_null);

	 }

	operator bool()
	{
		return type != msg_type::ty_null;
	}

	bool operator!()
	{
		return type == msg_type::ty_null;
	}

	msg_type get_type(){
		return type;
	}

	int64_t get_integer(){
		return integer;
	}

	std::string get_str(){
		return str;
	}

	msg_type     type;
	int64_t      integer;
	std::string  str;
};

typedef std::shared_ptr<BasicMessage> BasicMessagePtr;
typedef std::vector<BasicMessagePtr> BasicMessagePtrArray;

struct Message{
	msg_type type;  //single or array
	BasicMessagePtr bptr;  
	BasicMessagePtrArray barray_ptr;
};

}
#endif