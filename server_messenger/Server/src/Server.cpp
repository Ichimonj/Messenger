#include "Server.hpp"
#include "AccountBase.hpp"
#include "ThreadPool.hpp"
Server::Server(shared_ptr<asio::io_context> context, const char* ip, int port) : context(context)
{
    svDEBUG_LOG("DEBUG_Server", "Server(shared_ptr<asio::io_context> context)");
    acceptor = make_shared<asio::ip::tcp::acceptor>(*context, asio::ip::tcp::endpoint(asio::ip::make_address("127.0.0.1"), 8080));
}

Server::~Server()
{
    svDEBUG_LOG("DEBUG_Server", "~Server()");
}

void Server::start_accept()
{
    svDEBUG_LOG("DEBUG_Server", "Starting to accept connections...");

    auto socket = make_shared<asio::ip::tcp::socket>(*context);

    acceptor->async_accept(*socket, [this, socket](const error_code& ec)
        {
            if (!ec)
            {
                svDEBUG_LOG("DEBUG_Server", "New client connected!");
                threadPool->addTask([this, socket]() {add_client(socket); });
            }
            else
            {
                ERROR_LOG("Accept error: ", ec.message());   
            }
            start_accept();
        });
}
void Server::add_client(shared_ptr<asio::ip::tcp::socket> socket)
{
    svDEBUG_LOG("DEBUG_Server", "add_client(shared_ptr<asio::ip::tcp::socket> socket)");
    AccountFactory::make_account(socket);
}
