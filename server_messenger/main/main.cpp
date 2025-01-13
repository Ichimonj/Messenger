#include <iostream>
#include "version.hpp" //generated
#include "Server.hpp"
#include "ThreadPool.hpp"
using namespace std;


int main(int argc,char* argv[])
{
    setlocale(LC_ALL, "ru");
    cout << "BUILD VERSION - " << version << endl;

    shared_ptr<asio::io_context> context = make_shared<asio::io_context>();

    const char* ip = "127.0.0.1";
    int port = 8080;

    uint8_t IOthreadSize            = 12;
    uint8_t threadPoolThreadsSize   = 12;

    if (argc == 2) {
        IOthreadSize = stoi(argv[1]);
    }
    else if (argc == 3) {
        IOthreadSize            = stoi(argv[1]);
        threadPoolThreadsSize   = stoi(argv[2]);
    }
    else if (argc == 4) {
        try {
            ip = argv[2];
            asio::ip::make_address_v4(ip);
        }
        catch (const std::exception&) {
            system("cls");
            cerr << "Wrong ip\nenter ip in the format: 0.0.0.0" << endl;
            return 1; // Завершение программы при ошибке
        }
        try {
            port = stoi(argv[3]);
            if (port < 0 || port > 65535) {
                system("cls");
                cerr << "The port must be in the range from 0 to 65535" << endl;
                return 1; // Завершение программы при ошибке
            }
        }
        catch (const std::exception&) {
            system("cls");
            cerr << "Wrong port" << endl;
            return 1; // Завершение программы при ошибке
        }
    }
#ifdef _DEBUG
    string initInform = "ip - " + string(ip)+" port - " + to_string(port) + 
        "\nIOthreadSize - " + to_string(IOthreadSize) + "\nthreadPoolThreadsSize - " + to_string(threadPoolThreadsSize);
    cout << initInform << endl;
#endif // _DEBUG


    Server server(context,ip,port);
    server.start_accept();

    vector<thread> threads;
    for (size_t i = 0; i < IOthreadSize; i++) {
        threads.push_back(thread([context]() {context->run(); }));
    }
    threadPool = make_shared<ThreadPool>(threadPoolThreadsSize);

    for (;;) {
        string command;
        getline(cin, command);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    return 0;
}
