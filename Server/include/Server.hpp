#pragma once
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "AccountFactory.hpp"
#include "AccountBase.hpp"
using namespace std;

//CLASS RESPONSIBLE FOR CONNECTING CLIENTS
class Server
{
public:
    Server()                = delete;
    Server(const Server&)   = delete;    
    Server(Server&&)        = delete;
    Server(shared_ptr<asio::io_context> context);

    ~Server();
    void start_accept();

private:
    void add_client(shared_ptr<asio::ip::tcp::socket> socket);
private:
    shared_ptr<asio::io_context> context_;
    shared_ptr<asio::ip::tcp::acceptor> acceptor_;
};