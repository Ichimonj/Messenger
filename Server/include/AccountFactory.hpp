#pragma once
#include"Accounts.hpp"
#include"AccountBase.hpp"
#include"atomic"
#include"Hash.hpp"
//factory design pattern
//CLASS RESPONSIBLE FOR CREATION OF ACCOUNTS
class AccountFactory
{
public:
    static void make_account(shared_ptr<asio::ip::tcp::socket> socket);
    static vector<uint64_t> free_id;

private:
    static mutex mtAccountInsert;
    static mutex mtAssignmentId;

    static atomic<uint64_t> count;
    //Accounts/include/TempAccoutn.hpp
    static void make_temp_account(shared_ptr<asio::ip::tcp::socket> socket, error_code& ec);
    //Accounts/include/UserAccount.hpp
    static void make_user_account(shared_ptr<asio::ip::tcp::socket> socket, error_code& ec);
    static void login_account    (shared_ptr<asio::ip::tcp::socket> socket, error_code& ec);
};