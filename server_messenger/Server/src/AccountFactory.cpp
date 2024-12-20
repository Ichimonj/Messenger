#include "AccountFactory.hpp"
#include "ServerError.hpp"
//important: version without encryption
atomic<uint64_t> AccountFactory::count = 0;
vector<uint64_t> AccountFactory::free_id{};
mutex AccountFactory::mtAssignmentId;
mutex AccountFactory::mtAccountInsert;

void AccountFactory::make_account(shared_ptr<asio::ip::tcp::socket> socket)
{
    afDEBUG_LOG("DEBUG_account_factory", "make_account");

    error_code ec;
    char _type[10];
    string type;
    size_t length = socket->read_some(asio::buffer(_type), ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    else    { type = string(_type, length); }

    if      (type == "temp")    { make_temp_account(socket, ec); }
    else if (type == "no_temp") { make_user_account(socket, ec); }
    else if (type == "login")   { login_account    (socket, ec); }
    else    { EXCEPTIONS_LOG("Account_factory", "undefined connection type"); return; }
}

//Accounts/include/TempAccoutn.hpp
void AccountFactory::make_temp_account(shared_ptr<asio::ip::tcp::socket> socket, error_code& ec)
{
    afDEBUG_LOG("DEBUG_account_factory", "make_temp_account");

    char buf[1024];

    size_t lenght = socket->read_some(asio::buffer(buf), ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    string _userName(buf, lenght);

    lenght = socket->read_some(asio::buffer(buf), ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    string _password(buf, lenght);
    _password = Hash(_password);

    uint64_t ID;
    mtAssignmentId.lock();
    if (free_id.empty()) { ID = count++; }
    else {
        ID = free_id.back();
        free_id.pop_back();
    }
    mtAssignmentId.unlock();

    mtAccountInsert.lock();
    accountBase.insert(make_shared<TempAccount>(socket, ID, _userName,_password));
    mtAccountInsert.unlock();

    afDEBUG_LOG("DEBUG_account_factory", string("successful creation temp account ID - "+to_string(ID)));
}

//Accounts/include/UserAccount.hpp
void AccountFactory::make_user_account(shared_ptr<asio::ip::tcp::socket> socket, error_code &ec)
{
    afDEBUG_LOG("DEBUG_account_factory", "make_temp_account");

    char buf[1024];

    size_t lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    string _userName(buf, lenght);

    lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    string _password(buf, lenght);
    _password = Hash(_password);

    lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    string _emale(buf, lenght);

    lenght = socket->read_some(asio::buffer(buf),ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }

    PhoneNumber _phoneNumber(string(buf, lenght));

    uint64_t ID;
    mtAssignmentId.lock();
    if (free_id.empty()) { ID = count++; }
    else {
        ID = free_id.back();
        free_id.pop_back();
    }
    mtAssignmentId.unlock();

    mtAccountInsert.lock();
    accountBase.insert(make_shared<UserAccount>(socket, ID, _userName, _password, _emale, _phoneNumber));
    mtAccountInsert.unlock();

    afDEBUG_LOG("DEBUG_account_factory", string("successful creation user account ID - " + to_string(ID)));
}

void AccountFactory::login_account(shared_ptr<asio::ip::tcp::socket> socket, error_code& ec)
{
    afDEBUG_LOG("DEBUG_account_factory", "login");
    char buf[1024];

    size_t lenght = socket->read_some(asio::buffer(buf), ec);
    if (ec) { EXCEPTIONS_LOG("EXCEPTIONS_account_factory", ec.message()); return; }

    for (int i = 0; i < lenght; i++) {
        if (!isdigit(buf[i])) {
            return;
        }
    }

    uint64_t ID = stoi(string(buf, lenght));
    auto user = accountBase.findUser(ID);
    if (user == nullptr) {
        EXCEPTIONS_LOG("Account_factory", "account not found");
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::noUser)}), ec);
        return; 
    }
    else {
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
    }

    lenght = socket->read_some(asio::buffer(buf), ec);        
    if (ec) { EXCEPTIONS_LOG("Account_factory - ", ec.message()); return; }

    string _password(buf, lenght);
    _password = Hash(_password);

    if (user->getPassword() == _password) {
        if (user->login(socket) == 1) { EXCEPTIONS_LOG("Account_factory", "account is already taken"); }
        else { user->outBuffer(); }

        afDEBUG_LOG("DEBUG_account_factory", string("successful login account ID - " + to_string(ID)));
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
        string accountData = user->getAccountData();
        socket->write_some(asio::buffer(accountData.data(), accountData.size()), ec);
    }
    else {
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongPassword) }), ec);
    }
}