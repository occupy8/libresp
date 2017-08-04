#include "resp_decode.h"



resp_decode::resp_decode()
{
   
}

resp_decode::~resp_decode()
{

}
//return 
// protocol error
// ok
int resp_decode::inprocessBuffer(char *s, int len, resp_decode_result &result)
{
    if(NULL == s || 0 == len){
        printf("inprocessBuffer s=%p len=%d\n", s, len);
        return -1;
    }      
    
    m_pstr = s;
    m_len = len;

    while(m_len){
        if(m_pstr[0] == '*'){
            if(processMultibulkBuffer(result) != 0)
                break;
        }else{
            if(processInlineBuffer(result) != 0)
                break;
        }
    }

    result.used = len - m_len;

    return 0;
}

int resp_decode::processInlineBuffer(resp_decode_result &result)
{
    char *newline;
    size_t querylen;
    char *aux;

    newline = strchr(m_pstr, '\n');
    if(newline == NULL){
        printf("processInlineBuffer can't find '\n' \n");
        return -1;
    }

    if(newline && newline != m_pstr && *(newline-1) == '\r')
        newline--;

    querylen = newline-m_pstr;
    
    std::shared_ptr<BasicMessage>  bptr;
    switch(m_pstr[0]){
        case '-':
            bptr = new BasicMessage(m_pstr+1, querylen-1， msg_type::ty_error);
            break;
        case '+': 
            bptr = new BasicMessage(m_pstr+1, querylen-1, msg_type::ty_string);
            break;
        case ':':
            long long value;
            string2ll(m_pstr+1, querylen-1, &value);
            bptr = new BasicMessage(value);
            break;
        case '$':
            long long value;
            string2ll(m_pstr+1, querylen-1, &value);
            m_pstr += querylen +2;
            m_len -= (querylen+2);
            newline = strchr(m_pstr, '\n');
            if(newline == NULL){
                printf("protocol not enough data\n");
                m_pstr -= querylen+2;
                m_len += querylen+2;
                return 0;
            }

            if(newline && newline != m_pstr && *(newline-1) == '\r')
                newline--;
            else{
                m_pstr -= querylen+2;
                m_len += querylen+2;
                return 0;
            }
            querylen = newline-m_pstr;
            bptr = new BasicMessage(m_pstr, querylen, msg_type::ty_string);            
            m_pstr += querylen+2;
            m_len -= (querylen+2);

            break;

    }
    
    Message m;
    m.type = ty_single;
    m.bptr = bptr;
    result.cmds.push_back(m);
  
    return 0;
}

int resp_decode::processMultibulkBuffer(resp_decode_result &result)
{
    char *newline = NULL;
    int pos = 0, ok;
    long long ll;
    Message p_msg;
    char *tmp_ptr = m_pstr;
    int tmp_len = m_len;
    int bulklen = 0;
    int multibulklen = 0;

    newline = strchr(tmp_ptr, '\r');
    if(newline == NULL) {
        printf("can't find '\r'...\n");
        return -1;
    }

    if(newline-tmp_ptr > (tmp_len-2))
        return -1;
    ok = string2ll(tmp_ptr+1, newline-(tmp_ptr+1), &ll);
    if(!ok || ll > 1024*1024){
        printf("Protocol error: invalid multibulk length \n");
        return -1;
    }

    pos = (newline-tmp_ptr) + 2;
    if(ll <= 0){
        printf("length error\n");
        return 0;
    }
    multibulklen = ll;

    // tmp_ptr += pos;
    // tmp_len -= pos;

    while(multibulklen){
        if(0 == bulklen){
            newline = strchr(tmp_ptr+pos, '\r');
            if(newline == NULL){
                printf("Protocol error: too big bulk count string\n");
                break;
            }
            if(newline-tmp_pstr > tmp_len-2){
                break;
            }

            if(tmp_ptr[pos] != '$'){
                return -1;
            }

            ok = string2ll(tmp_ptr+pos+1, newline-(tmp_ptr+pos+1), &ll);
            if(!ok || ll <0 || ll > 512*1024*1024){
                return -1;
            }

            pos += newline-(tmp_ptr+pos)+2;
            if(ll >= PROTO_MBULK_BIG_ARG){
                size_t qblen;

                pos = 0;
                printf("proto buffer is too large for read\n");
                return -1;
            }
            bulklen = ll;

        }

        if(tmp_len - pos < (unsigned)(bulklen+2)){
            break;
        }else {
            if(pos == 0 && bulklen >= PROTO_MBULK_BIG_ARG && 
                (signed) m_len == bulklen+2){                   
                    tmp_ptr += pos;
                    tmp_len -= pos;
                    pos = 0;
                    printf("can't understand \n'");
                    return -1;
                }else{
                    p_msg.type = ty_array;
                    std::shared_ptr<BasicMessage> bptr(new BasicMessage(tmp_ptr+pos, bulklen));
                    p_msg.barray_ptr.push_back(bptr);

                    pos += bulklen+2;
                }

                bulklen = 0;
                multibulklen--;
        }
    }

    if(pos){
        m_pstr += pos;
        m_len -= pos;
    }

    if(multibulklen == 0){
        result.cmds.push_back(p_msg);

        return 0;
    }
        
    return -1;
}