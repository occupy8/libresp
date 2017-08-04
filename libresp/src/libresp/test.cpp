#include "resp_decode.h"
#include "resp_encode.h"

#include <iostream>

using namespace resp;
using namespace std;

int main(void){
    char *str = "*1\r\n$4\r\nPING\r\n";

    resp_decode decode;
    resp_encode encode;
    resp_decode_result result;

    decode.inprocessBuffer(str, strlen(str), result);

    cout << "used: "<<result.used;
    vector<Message>::iterator it;
    for(it = result.cmds.begin(); it != result.cmds.end(); it++){
        Message m = (*it);
        switch(m.type){
            case msg_type::ty_single:
                switch(m.bptr->get_type()){
                    case msg_type::ty_integer:
                        cout<<"msg: "<<m.bptr->get_integer()<<endl;
                        break;
                    case msg_type::ty_string:
                        cout<<"msg: "<<m.bptr->get_str()<<endl;
                        break;
                    default:
                        break;
                }
                
                break;
            case msg_type::ty_array:
                vector<BasicMessagePtr>::iterator itor;
                for(itor = m.barray_ptr.begin(); itor != m.barray_ptr.end(); itor++){
                    switch((*itor)->get_type()){
                        case msg_type::ty_integer:
                            cout<<"msg: "<<(*itor)->bptr->get_integer()<<endl;
                            break;
                        case msg_type::ty_string:
                            cout<<"msg: "<<(*itor)->bptr->get_str()<<endl;
                            break;
                        default:
                            break;
                    }
                }
                
                break;
            default:
                break;
        }
    }
    

    return 0;
}