#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
#include <chrono>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

int main(int argc, char* argv[])
{
    int runningNo = 0;
    json::value jsonResponse;

    // Define URI and create listener
    uri_builder uri(L"http://127.0.0.1:1234");
    http_listener listener(uri.to_uri());
    
    // Open the listener, i.e. start accepting requests. Listener runs asynchronously uses a threadpool to launch the task
    listener.open().wait();
    cout << "HTTP LISTENER STARTED \n";

    // Listen to and handle GET requests
    listener.support(methods::GET, [&](http_request request) {       
            // Prepare HTTP response
            http_response response(status_codes::OK);
            response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            response.set_body(jsonResponse);
            request.reply(response);
    });

    // TODO: Provide a way to safely terminate this loop
    cout << "Waiting for incoming connection..." << endl;
    while (true) {
        // Prepare JSON response
        jsonResponse[L"key1"] = json::value::boolean(false);
        jsonResponse[L"key2"] = json::value::number(runningNo);
        jsonResponse[L"key3"] = json::value::number(43.6121);
        jsonResponse[L"key4"] = json::value::string(U("str"));
        
        runningNo++;
        this_thread::sleep_for(chrono::milliseconds(2000));
    }

    // Nothing left to do but commit suicide.
    cout << "Terminating JSON listener." << endl;
    listener.close();
    return 0;
}