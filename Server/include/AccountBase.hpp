#pragma once
#include "Accounts.hpp"
#include "map"
typedef shared_ptr<Account> userPtr;
typedef map<uint64_t, userPtr> userSet;

class AccountBase
{
public:
    AccountBase()   { abDEBUG_LOG("DEBUG_Account_base", "AccountBase()") };
    ~AccountBase()  { abDEBUG_LOG("DEBUG_Account_base", "~AccountBase()") };

    void insert(userPtr&& user);
    void erase(uint64_t ID);
    userPtr findUser(uint64_t ID);

public:
    void print();
private:
    userSet user_base;
};
extern AccountBase accountBase;