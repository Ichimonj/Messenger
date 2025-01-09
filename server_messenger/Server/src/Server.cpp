#include "Server.hpp"
#include "AccountBase.hpp"
Server::Server(shared_ptr<asio::io_context> context, const char* ip, int port) : context_(context)
{
    svDEBUG_LOG("DEBUG_Server", "Server(shared_ptr<asio::io_context> context)");
    acceptor_ = make_shared<asio::ip::tcp::acceptor>(*context_, asio::ip::tcp::endpoint(asio::ip::make_address("127.0.0.1"), 8080));
}

Server::~Server()
{
    svDEBUG_LOG("DEBUG_Server", "~Server()");
}

void Server::start_accept()
{
    svDEBUG_LOG("DEBUG_Server", "Starting to accept connections...");

    auto socket = make_shared<asio::ip::tcp::socket>(*context_);

    acceptor_->async_accept(*socket, [this, socket](const error_code& ec)
        {
            if (!ec)
            {
                svDEBUG_LOG("DEBUG_Server", "New client connected!");
                thread([this, socket]() {add_client(socket); }).detach();
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
