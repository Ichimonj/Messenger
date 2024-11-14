#include "Server.hpp"

Server::Server(shared_ptr<asio::io_context> context) : context_(context),count(0)
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
    svDEBUG_LOG("DEBUG_Server", "start_accept()");
    cout << "Starting to accept connections..." << endl;

    auto socket = make_shared<asio::ip::tcp::socket>(*context_);

    acceptor_->async_accept(*socket, [this, socket](const error_code& ec)
        {
            if (!ec)
            {
                cout << "New client connected!" << endl;
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
    count += 1;
    userPtr newUser = AccountFactory::make_account(socket, count);
    if (newUser != nullptr) {
        accountBase.insert(newUser);
    }
    else {
        ERROR_LOG("DEBUG_Server", "AccountFactory rerutn nullptr");
    }
}
