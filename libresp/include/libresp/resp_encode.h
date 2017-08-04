#ifndef LIBRESP_RESP_ENCODE_H_
#define LIBRESP_RESP_ENCODE_H_

#include "resp_message.h"

namespace resp{

class resp_encode{
public:
    resp_encode(Message msg);
    ~resp_encode();

    int encode(std::string &out);

private:
    Message m_msg;
};



}


#endif 