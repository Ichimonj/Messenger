#pragma once
#include"Accounts.hpp"
#include"AccountBase.hpp"

class AccountFactory
{
public:
    static void make_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID);

private:
    static void make_temp_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID,error_code& ec);
    static void make_user_account(shared_ptr<asio::ip::tcp::socket> socket, uint64_t ID,error_code& ec);
};