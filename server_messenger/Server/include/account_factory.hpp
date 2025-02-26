#pragma once
#include"accounts.hpp"
#include"account_base.hpp"
#include"atomic"
#include"hash.hpp"

//factory design pattern
//CLASS RESPONSIBLE FOR CREATION OF ACCOUNTS
class AccountFactory
{
public:
    static void make_account(shared_ptr<asio::ip::tcp::socket> socket);
    static vector<uint64_t> free_id;

private:
    //Accounts/include/TempAccoutn.hpp
    static void make_temp_account(shared_ptr<asio::ip::tcp::socket> socket, error_code& ec);
    //Accounts/include/UserAccount.hpp
    static void make_user_account(shared_ptr<asio::ip::tcp::socket> socket, error_code& ec);
    static void login_account    (shared_ptr<asio::ip::tcp::socket> socket, error_code& ec);

private:
    static atomic<uint64_t> count;

    static mutex mt_account_insert;
    static mutex mt_assignment_id;
};