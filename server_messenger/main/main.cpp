#include <iostream>
#include "version.hpp" //generated
#include "Server.hpp"
using namespace std;

int main(int argc,char* argv[])
{
    setlocale(LC_ALL, "ru");
    cout << "BUILD VERSION - " << version << endl;

    uint8_t threadSize = 12;
    if (argc > 1) {
        threadSize = stoi(argv[1]);
    }
    shared_ptr<asio::io_context> context = make_shared<asio::io_context>();
    
    const char* ip = "127.0.0.1";
    int port = 8080;

    if (argc == 4) {
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

    Server server(context,ip,port);
    server.start_accept();

    vector<thread> threads;
    for (size_t i = 0; i < threadSize; i++) {
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
