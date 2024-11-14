#include "AccountBase.hpp"

void AccountBase::insert(userPtr&& user)
{
    abDEBUG_LOG("DEBUG_Account_base", "insert(userPtr &user)");
    user_base.insert({ user->getId(), user });
}

void AccountBase::erase(uint64_t ID)
{
    abDEBUG_LOG("DEBUG_Account_base", "erase(uint64_t ID)");
    user_base.erase(ID);
}

userPtr AccountBase::findUser(uint64_t ID)
{
    abDEBUG_LOG("DEBUG_Account_base", "findUser(uint64_t ID)");
    auto account = user_base.find(ID);
    if (account == user_base.end()) { return nullptr; }
    return account->second;
}

void AccountBase::print()
{
    abDEBUG_LOG("DEBUG_Account_base", "print()");

    for (auto& user : user_base)
    {
        user.second->print();
    }
}
AccountBase accountBase;
