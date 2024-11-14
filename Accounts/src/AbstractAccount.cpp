#include "AbstractAccount.hpp"

Account::Account(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string &userName,const string& password)
    : socket_(move(socket)), ID_(ID), userName_(userName),password_(password), status_(online)
{
    acDEBUG_LOG("DEBUG_Virtual_account", "Account(const uint64_t ID, const string userName)");
}
Account::Account(Account &&other) noexcept
    : socket_(move(other.socket_)), ID_(other.ID_), userName_(move(other.userName_)),password_(move(other.password_)), status_(other.status_)
{
    acDEBUG_LOG("DEBUG_Virtual_account", "Account(Account&& other)");
}

Account::~Account()
{
    acDEBUG_LOG("DEBUG_Virtual_account", "~Account()");
}

const uint64_t Account::getId()     const { return this->ID_; }
const string Account::getUserName() const { return this->userName_; }
const status Account::getStatus()   const { return this->status_; }

void Account::print() const { cout << *this; }

void Account::info() const
{
    cout << "\033[38;5;250;48;5;22m";
    cout << setw(16) << left << setfill('.') << "ID" << ID_ << '\n';
    cout << setw(16) << left << setfill('.') << "User name" << userName_ << '\n';
    cout << "\033[0m";
}

ostream &operator<<(ostream &os, const Account &ex)
{
    cout << "\033[38;5;250;48;5;22m";
    os << setw(16) << left << setfill('.') << "ID"          << ex.ID_       << '\n';
    os << setw(16) << left << setfill('.') << "User name"   << ex.userName_ << '\n';
    os << setw(16) << left << setfill('.') << "Password"    << ex.password_ << '\n';
    os << setw(16) << left << setfill('.') << "Status";
    switch (ex.status_)
    {
    case offline:
        os << "offline";
        break;
    case online:
        os << "online";
        break;
    case dont_disturb:
        os << "dont disturb";
        break;
    case invisible:
        os << "invisible";
        break;
    default:
        os << "unknown";
        break;
    }
    os << '\n';
    return os;
}

bool Account::operator<(const Account &ex)
{
    return this->ID_ < ex.ID_;
}