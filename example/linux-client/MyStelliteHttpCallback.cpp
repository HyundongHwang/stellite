#include "MyStelliteHttpCallback.h"

MyStelliteHttpCallback::MyStelliteHttpCallback()
    : stellite::HttpResponseDelegate()
{
}

void
MyStelliteHttpCallback::OnHttpResponse(int request_id, const stellite::HttpResponse &response,
                                       const char *body,
                                       size_t body_len) {

    printf("OnHttpResponse.OnHttpResponse request_id[%d] body_len[%d] connection_info_desc[%s] connection_info[%d] \n",
         request_id,
         body_len,
         response.connection_info_desc.c_str(),
         response.connection_info
    );

    // printf("OnHttpResponse.OnHttpResponse body[%s]  \n",
    //      body
    // );

    pCv->notify_one();

}

void
MyStelliteHttpCallback::OnHttpStream(int request_id, const stellite::HttpResponse &response,
                                     const char *stream,
                                     size_t stream_len, bool is_last) {

    printf("OnHttpResponse.OnHttpStream request_id[%d], stream_len[%d], is_last[%d] connection_info_desc[%s] connection_info[%d] \n",
         request_id,
         stream_len,
         is_last,
         response.connection_info_desc.c_str(),
         response.connection_info
    );

    // printf("OnHttpResponse.OnHttpStream stream[%s] \n",
    //      stream);

    pCv->notify_one();
}

void
MyStelliteHttpCallback::OnHttpError(int request_id, int error_code,
                                    const std::string &error_message) {

    printf("OnHttpResponse.OnHttpError request_id[%d] error_code[%d] error_message[%s] \n",
         request_id,
         error_code,
         error_message.c_str()
    );

    pCv->notify_one();
}