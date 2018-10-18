// whattotest.cpp
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <gtest/gtest.h>
#include "MyStelliteHttpCallback.h"
#include "stellite/include/http_client.h"
#include "stellite/include/http_client_context.h"
#include "stellite/include/http_request.h"
#include "stellite/include/http_response.h"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// TEST(stellite, get_quic)
// {
//     std::condition_variable cv;
//     std::mutex mutex;

//     static stellite::HttpClientContext::Params client_params;
//     client_params.using_quic = true;
//     client_params.using_http2 = true;
//     client_params.ignore_certificate_errors = true;
//     client_params.enable_http2_alternative_service_with_different_host = true;
//     client_params.enable_quic_alternative_service_with_different_host = true;
//     client_params.origins_to_force_quic_on.push_back("www.google.co.kr");
//     static stellite::HttpClientContext ctx(client_params);

//     ctx.Initialize();
//     MyStelliteHttpCallback callback;
//     callback.pCv = &cv;
//     auto pClient = ctx.CreateHttpClient(&callback);

//     stellite::HttpRequest request;
//     request.request_type = stellite::HttpRequest::RequestType::GET;
//     request.url = "https://www.google.co.kr:443";
//     printf("request.url[%s] \n", request.url.c_str());
//     pClient->Request(request, 3 * 1000);

//     std::unique_lock<std::mutex> lk(mutex);
//     cv.wait(lk);

//     // ctx.CancelAll();
//     // ctx.ReleaseHttpClient(pClient);
// }

TEST(stellite, get_tcp)
{
    std::condition_variable cv;
    std::mutex mutex;

    static stellite::HttpClientContext::Params client_params;
    client_params.using_quic = false;
    client_params.using_http2 = true;
    client_params.ignore_certificate_errors = true;
    client_params.enable_http2_alternative_service_with_different_host = true;
    client_params.enable_quic_alternative_service_with_different_host = false;
    client_params.origins_to_force_quic_on.clear();
    static stellite::HttpClientContext ctx(client_params);

    ctx.Initialize();
    MyStelliteHttpCallback callback;
    callback.pCv = &cv;
    auto pClient = ctx.CreateHttpClient(&callback);

    stellite::HttpRequest request;
    request.request_type = stellite::HttpRequest::RequestType::GET;
    request.url = "https://www.google.co.kr:443";
    printf("request.url[%s] \n", request.url.c_str());
    pClient->Request(request, 3 * 1000);

    std::unique_lock<std::mutex> lk(mutex);
    cv.wait(lk);

    ctx.CancelAll();
    ctx.ReleaseHttpClient(pClient);
}