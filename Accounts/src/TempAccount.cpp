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
                thread([&]() {read_handler(buf, length); }).detach();
                reading();
            }
            else if (ec == asio::error::operation_aborted) {
                acDEBUG_LOG("DEBUG_Temp_account", "stop async reading");
            }
            else {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
            }
        });
}
void TempAccount::read_handler(const char* buf, const size_t length)
{
    string msg(buf, length);
    USER_MESSAGE(this->getUserName(), msg);
    //Change user name
    if (msg == "__chName") {
        error_code ec;
        char buf[1024];
    
        socket_->cancel();
        size_t length = socket_->read_some(asio::buffer(buf),ec);
        if(ec){
            ERROR_LOG("ERROR_Temp_account", "error reading");
            AccountFactory::free_id.push_back(this->getId());
            accountBase.erase(this->getId());
            return;
        }

        string name_(buf, length);
        this->userName_ = name_;

        reading();
    }
    //Change password
    else if (msg == "__chPaswd") {
        error_code ec;
        char buf[1024];

        socket_->cancel();
        //old password
        size_t length = socket_->read_some(asio::buffer(buf), ec);
        if (ec) {
            ERROR_LOG("ERROR_Temp_account", "error reading");
            AccountFactory::free_id.push_back(this->getId());
            accountBase.erase(this->getId());
            return;
        }

        string _password(buf, length);
        _password = Hash(_password);
        if (this->password_ != _password) {
            reading();
            return;
        }

        length = socket_->read_some(asio::buffer(buf), ec);
        if (ec) {
            ERROR_LOG("ERROR_Temp_account", "error reading");
            AccountFactory::free_id.push_back(this->getId());
            accountBase.erase(this->getId());
            return;
        }

        _password = string(buf, length);
        this->password_ = Hash(_password);

        reading();
    }
    //exit
    else if (msg == "__exit") {
        error_code ec;
        char buf[10];

        socket_->cancel();
        size_t length = socket_->read_some(asio::buffer(buf), ec);
        if (ec) {
            ERROR_LOG("ERROR_Temp_account", "error reading");
            AccountFactory::free_id.push_back(this->getId());
            accountBase.erase(this->getId());
            return;
        }

        if (string(buf, length) == "__Y") {
            ERROR_LOG("ERROR_Temp_account", "delete account");
            AccountFactory::free_id.push_back(this->getId());
            accountBase.erase(this->getId());
        }
        else {
            ERROR_LOG("ERROR_Temp_account", "don't delete account");
        }
    }
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