#include <iostream>
//#include "version.hpp" //generated
#include "Server.hpp"
using namespace std;

int main(int argc,char* argv[])
{
    setlocale(LC_ALL, "ru");
    //cout << "BUILD VERSION - " << version << endl;

    uint8_t threadSize = 12;
    if (argc > 1) {
        threadSize = stoi(argv[1]);
    }

    shared_ptr<asio::io_context> context = make_shared<asio::io_context>();
    Server server(context);
    server.start_accept();

    vector<thread> threads;
    for (int i = 0; i < threadSize; i++) {
        threads.push_back(thread([context]() {context->run(); }));
    }
    for (;;) {
        string command;
        getline(cin, command);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    return 0;
}
