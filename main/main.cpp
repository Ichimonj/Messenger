#include <iostream>
#include "version.hpp" //generated
#include "Server.hpp"
using namespace std;

int main()
{
    cout << "BUILD VERSION - " << version << endl;
    shared_ptr<asio::io_context> context = make_shared<asio::io_context>();
    Server server(context);
    server.start_accept();
    context->run();
    return 0;
}
