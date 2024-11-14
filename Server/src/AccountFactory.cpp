#include "AccountFactory.hpp"

void AccountFactory::make_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID)
{
    afDEBUG_LOG("DEBUG_account_factory", "make_account");

    error_code ec;
    char _type[10];
    string type;
    size_t length = socket->read_some(asio::buffer(_type), ec);
    if (ec) { return; }
    else    { type = string(_type, length); }

    if      (type == "temp")    { make_temp_account(socket, ID, ec); }
    else if (type == "no_temp") { make_user_account(socket, ID, ec); }
    else    { return; }
}

void AccountFactory::make_temp_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID, error_code &ec)
{
    afDEBUG_LOG("DEBUG_account_factory", "make_temp_account");

    char buf[1024];

    size_t lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { return ; }
    string _userName(buf, lenght);

    accountBase.insert(make_shared<TempAccount>(socket, ID, _userName));
}

void AccountFactory::make_user_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID, error_code &ec)
{
    afDEBUG_LOG("DEBUG_account_factory", "make_user_account");

    char buf[1024];

    size_t lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { return ; }
    string _userName(buf, lenght);

    lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { return ; }
    string _password(buf, lenght);

    lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { return ; }
    string _emale(buf, lenght);

    lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { return ; }
    PhoneNumber _phoneNumber(string(buf, lenght));

    accountBase.insert(make_shared<UserAccount>(socket, ID, _userName, _password, _emale, _phoneNumber));
}

