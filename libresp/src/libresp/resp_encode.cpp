#include "resp_encode.h"

#include <string>
#include <sstream>
#include <iostream>

class resp_encode{
public:
    resp_encode(Message msg);
    ~resp_encode();

    int encode(std::string &out);

private:
    Message m_msg;
};


resp_encode::resp_encode(Message msg){
    m_msg = msg;
}

resp_encode::~resp_encode(){

}

int resp_encode::encode(std::string &out){
    int num;
    char tmp[16] = {0};
    int tmplen = 0;
    std::stringstream stream;
    std::vector<BasicMessagePtr>::iterator it;
    num = m_msg.barray_ptr.capacity();
    
    if(m_msg.type == msg_type::ty_array){
        stream << "*";
        stream << num;
        stream << "\r\n";

        for(it = m_msg.barray_ptr.begin(); it != m_msg.barray_ptr.end(); it++){
            stream << "$";
            switch((*it)->type){
                case msg_type::ty_integer:
                    snprintf(tmp, sizeof(tmp), "%lld", (*it)->get_integer());
                    tmplen = strlen(tmp);
                    stream << tmplen;
                    stream << tmp;
                    stream << "\r\n";
                    break;
                case msg_type::ty_string:
                    stream << (*it)->get_str().size();
                    stream << (*it)->get_str();
                    stream << "\r\n";
                    break;
                default:
                    break;
            }
        }
    }else if(m_msg.type == msg_type::ty_single){
        std::shared_ptr<BasicMessage> sptr = m_msg.bptr;
        switch(sptr->get_type()){
            case msg_type::ty_error:
                stream<<"-";
                stream<<sptr->get_str();
                stream<<"\r\n";
                break;
            case msg_type::ty_string:
                stream<<"+";
                stream<<sptr->get_str();
                stream<<"\r\n";
                break;
            case msg_type::ty_error:
                stream<<"-";
                stream<<sptr->get_str();
                stream<<"\r\n";
                break;
            case msg_type::ty_integer:
                stream<<":";
                stream<<sptr->get_integer();
                stream<<"\r\n";
                break;
            case msg_type::ty_bulkstring:
                stream<<"$";
                stream<<sptr->get_str().size();
                stream<<"\r\n";
                stream<<sptr->get_str();
                stream<<"\r\n";
                break;
            default:
                printf("message type error!\n");
                break;

        }
    }

    stream >> out;
}