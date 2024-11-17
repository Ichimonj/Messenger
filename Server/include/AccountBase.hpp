#pragma once
#include "Accounts.hpp"
#include "map"
typedef shared_ptr<Account> userPtr;
typedef map<uint64_t, userPtr> userSet;

//CLASS RESPONSIBLE FOR THE CLIENT DATABASE
class AccountBase
{
public:
    AccountBase()   { abDEBUG_LOG("DEBUG_Account_base", "AccountBase()") };
    ~AccountBase()  { abDEBUG_LOG("DEBUG_Account_base", "~AccountBase()") };

    void    insert(userPtr&& user);
    void    erase(uint64_t ID);
    userPtr findUser(uint64_t ID);

    void    print();

private:
    userSet user_base;
};
extern AccountBase accountBase;