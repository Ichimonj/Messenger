#include "AccountFactory.hpp"

unique_ptr<Account> AccountFactory::make_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID)
{
    afDEBUG_LOG("DEBUG_account_factory", "make_account");

    error_code ec;
    char _type[10];
    string type;
    size_t length = socket->read_some(asio::buffer(_type), ec);
    if (ec) { return nullptr; }
    else    { type = string(_type, length); }

    if      (type == "temp")    { return make_temp_account(socket, ID, ec); }
    else if (type == "no_temp") { return make_user_account(socket, ID, ec); }
    else    { return nullptr; }
}

unique_ptr<Account> AccountFactory::make_temp_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID, error_code &ec)
{
    afDEBUG_LOG("DEBUG_account_factory", "make_temp_account");

    char buf[1024];

    size_t lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { return nullptr; }
    string userName(buf, lenght);

    return make_unique<TempAccount>(socket, ID, userName);
}

unique_ptr<Account> AccountFactory::make_user_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID, error_code &ec)
{
    afDEBUG_LOG("DEBUG_account_factory", "make_user_account");

    char buf[1024];

    size_t lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { return nullptr; }
    string _userName(buf, lenght);

    lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { return nullptr; }
    string _password(buf, lenght);

    lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { return nullptr; }
    string _emale(buf, lenght);

    lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { return nullptr; }
    PhoneNumber _phoneNumber(string(buf, lenght));

    return make_unique<UserAccount>(socket, ID, _userName, _password, _emale, _phoneNumber);
}