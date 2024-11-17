#include "TempAccount.hpp"

//constructors
TempAccount::TempAccount(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string& userName, const string& password)
    : Account(socket, ID, userName, password)
{
    acDEBUG_LOG("DEBUG_Temp_account", "TempAccount(const uint64_t ID, const string userName)");
    reading();
}
TempAccount::TempAccount(TempAccount &&other) noexcept
    : Account(move(other))
{
    acDEBUG_LOG("DEBUG_Temp_account", "Account(Account&& other)");
    reading();
}
//destructors
TempAccount::~TempAccount()
{
    acDEBUG_LOG("DEBUG_Temp_account", "~TempAccount()");
}

//other member functions
void TempAccount::reading()
{
    socket_->async_read_some(asio::buffer(buf),
        [&](const error_code& ec, size_t length)
        {
            if (!ec) {
                USER_MESSAGE(this->getUserName(), string(buf, length));
                reading();
            }
            else {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
            }
        });
}
void TempAccount::print() const
{
    cout << "\033[38;5;250;48;5;23m____Temp account____\n";
    cout << dynamic_cast<const Account &>(*this);
    cout << *this;
}
void TempAccount::info() const
{
    cout << "\033[38;5;250;48;5;23m____Temp account____\n";
    Account::info();
}

//operators
ostream &operator<<(ostream &os, const TempAccount &ex)
{
    os << "\033[0m";
    return os;
}