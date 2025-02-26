#include "abstract_account.hpp"

//constructors
Account::Account(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string &userName,const string& password)
    : socket(move(socket)), ID(ID), user_name(userName),password(password), status(online)
{
    acDEBUG_LOG("DEBUG_Virtual_account", "Account(const uint64_t ID, const string userName)");
}

//destructors
Account::~Account()
{
    acDEBUG_LOG("DEBUG_Virtual_account", "~Account()");
}

//hetaeras
const uint64_t Account::getId()     const { return this->ID; }
const string Account::getUserName() const { return this->user_name; }
const string Account::getPassword() const { return this->password; }
const status Account::getStatus()   const { return this->status; }

const string Account::getAccountData() const
{
    string data = to_string(this->ID) + '\n' + this->user_name  + '\n';
    return data;
}

const shared_ptr<asio::ip::tcp::socket> Account::getSocket()     const { return this->socket; }

//other member functions
void Account::print() const { cout << *this; }
void Account::info() const
{
    cout << "\033[38;5;250;48;5;22m";
    cout << setw(16) << left << setfill('.') << "ID" << ID << '\n';
    cout << setw(16) << left << setfill('.') << "User name" << user_name << '\n';
    cout << "\033[0m";
}
uint8_t Account::login(shared_ptr<asio::ip::tcp::socket> socket)
{
    acDEBUG_LOG("DEBUG_Virtual_account", "login");
    if (socket->is_open()) { return 1; }
    this->socket = socket;
    reading();
    this->status = online;
    return 0;
}

//operators
ostream &operator<<(ostream &os, const Account &ex)
{
    cout << "\033[38;5;250;48;5;22m";
    os << setw(16) << left << setfill('.') << "ID"          << ex.ID       << '\n';
    os << setw(16) << left << setfill('.') << "User name"   << ex.user_name << '\n';
    os << setw(16) << left << setfill('.') << "Password"    << ex.password << '\n';
    os << setw(16) << left << setfill('.') << "Status";
    switch (ex.status)
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
    return this->ID < ex.ID;
}
