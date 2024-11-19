#include "UserAccount.hpp"

//constructors
UserAccount::UserAccount(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string &userName, const string &password, const string &emale, const PhoneNumber &phoneNumber)
    : Account(socket, ID, userName, password), emale_(emale), phoneNumber_(phoneNumber)
{
    acDEBUG_LOG("DEBUG_User_account", "UserAccount(const uint64_t ID, const string& userName, const string& password, const string& emale, const PhoneNumber& phoneNumber)");
    reading();
}
UserAccount::UserAccount(UserAccount &&other) noexcept
    : Account(move(other)), password_(move(other.password_)), emale_(move(emale_)), phoneNumber_(move(phoneNumber_))
{
    acDEBUG_LOG("DEBUG_User_account", "UserAccount(UserAccount&& other)");
    reading();
}

//destructors
UserAccount::~UserAccount()
{
    acDEBUG_LOG("DEBUG_User_account", "~UserAccount()");
}

//other member functions
void UserAccount::reading()
{
    socket_->async_read_some(asio::buffer(buf),
        [&](const error_code& ec, size_t length)
        {
            if (!ec) {
                thread([&]() {read_handler(buf, length); }).detach();
            }
            else {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                socket_->close();
                this->status_ = offline;
            }
        });
}
void UserAccount::read_handler(const char* buf, const size_t length)
{
    string msg(buf, length);
    USER_MESSAGE(this->getUserName(), msg);
    //Change user name
    if (msg == "__chName") {
        error_code ec;
        char buf[1024];

        socket_->cancel();
        size_t length = socket_->read_some(asio::buffer(buf), ec);
        if (ec) {
            ERROR_LOG("ERROR_Temp_account", "error reading");
            socket_->close();
            this->status_ = offline;
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
            socket_->close();
            this->status_ = offline;
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
            socket_->close();
            this->status_ = offline;
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
            socket_->close();
            this->status_ = offline;
            return;
        }

        if (string(buf, length) == "__Y") {
            ERROR_LOG("ERROR_Temp_account", "delete account");
            socket_->close();
            this->status_ = offline;
        }
        else {
            ERROR_LOG("ERROR_Temp_account", "don't delete account");
        }
    }
}

void UserAccount::print() const
{
    cout << "\033[38;5;250;48;5;23m____User account____\n";
    cout << dynamic_cast<const Account &>(*this);
    cout << *this;
}

void UserAccount::info() const
{
    cout << "\033[38;5;250;48;5;23m____User account____\n";
    Account::info();
}

//operators
ostream &operator<<(ostream &os, const UserAccount &ex)
{
    os << setw(16) << left << setfill('.') << "Emale" << ex.emale_ << '\n';
    os << setw(16) << left << setfill('.') << "Phone number" << ex.phoneNumber_;
    os << "\033[0m\n";
    return os;
}