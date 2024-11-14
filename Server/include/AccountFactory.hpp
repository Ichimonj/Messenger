#pragma once
#include"Accounts.hpp"

class AccountFactory
{
public:
    static unique_ptr<Account> make_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID);

private:
    static unique_ptr<Account> make_temp_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID,error_code& ec);
    static unique_ptr<Account> make_user_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID,error_code& ec);
};