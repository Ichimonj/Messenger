#include "UserAccount.hpp"

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

UserAccount::~UserAccount()
{
    acDEBUG_LOG("DEBUG_User_account", "~UserAccount()");
}

void UserAccount::reading()
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
                socket_->close();
            }
        });
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

ostream &operator<<(ostream &os, const UserAccount &ex)
{
    os << setw(16) << left << setfill('.') << "Emale" << ex.emale_ << '\n';
    os << setw(16) << left << setfill('.') << "Phone number" << ex.phoneNumber_;
    os << "\033[0m\n";
    return os;
}