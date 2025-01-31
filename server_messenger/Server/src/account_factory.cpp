#include "account_factory.hpp"
#include "server_error.hpp"
#include "data_base_work.hpp"

//important: version without encryption
atomic<uint64_t> AccountFactory::count = 0;
vector<uint64_t> AccountFactory::free_id{};
mutex AccountFactory::mt_assignment_id;
mutex AccountFactory::mt_account_insert;

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

    size_t length = socket->read_some(asio::buffer(buf), ec);   //user name
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    string _userName(buf, length);

    socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }

    length = socket->read_some(asio::buffer(buf), ec);          //password
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    string _password(buf, length);
    _password = Hash(_password);

    socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }

    // Создаем ID клиенту
    uint64_t ID;
    mt_assignment_id.lock();
    if (free_id.empty()) { ID = count++; }
    else {
        ID = free_id.back();
        free_id.pop_back();
    }
    mt_assignment_id.unlock();

    mt_account_insert.lock();
    account_base.insert(make_shared<TempAccount>(socket, ID, _userName,_password)); // Добавляем нового клиента в базу
    mt_account_insert.unlock();

    WorkWithFiles::createUserDir(ID); //создаем папку для клиента 

    afDEBUG_LOG("DEBUG_account_factory", string("successful creation temp account ID - "+to_string(ID)));
    string accountData = to_string(ID) + '\n' + _userName;
    socket->write_some(asio::buffer(accountData.data(), accountData.size()), ec); // Отправляем клиенту его данные 
}

//Accounts/include/UserAccount.hpp
void AccountFactory::make_user_account(shared_ptr<asio::ip::tcp::socket> socket, error_code& ec)
{
    afDEBUG_LOG("DEBUG_account_factory", "make_no_temp_account");

    char buf[1024];

    size_t length = socket->read_some(asio::buffer(buf), ec);    //User name
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    string _userName(buf, length);

    socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }

    length = socket->read_some(asio::buffer(buf), ec);           //Password
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    string _password(buf, length);
    _password = Hash(_password);

    socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }

    length = socket->read_some(asio::buffer(buf), ec);           //emale
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    string _email(buf, length);

    socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }

    length = socket->read_some(asio::buffer(buf), ec);           //Phone number
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }

    PhoneNumber _phoneNumber(string(buf, length));
    while (!_phoneNumber.isValid()) {
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongPhoneNumber) }), ec);
        if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }

        length = socket->read_some(asio::buffer(buf), ec);      //Phone number
        if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }

        _phoneNumber.setNumber(string(buf, length));
    }

    socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
    if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }

    // Создаем ID клиенту
    uint64_t ID;
    mt_assignment_id.lock();
    if (free_id.empty()) { ID = count++; }
    else {
        ID = free_id.back();
        free_id.pop_back();
    }
    mt_assignment_id.unlock();

    mt_account_insert.lock();
    account_base.insert(make_shared<UserAccount>(socket, ID, _userName, _password, _email, _phoneNumber)); // Добавляем нового клиента в базу
    mt_account_insert.unlock();

    WorkWithFiles::createUserDir(ID); //создаем папку для клиента 

    afDEBUG_LOG("DEBUG_account_factory", string("successful creation user account ID - " + to_string(ID)));
    string accountData = to_string(ID) + '\n' + _userName + '\n' + _email + '\n' + _phoneNumber.getNumber(); 
    socket->write_some(asio::buffer(accountData.data(), accountData.size()), ec); // Отправляе клиенту его данные
}

void AccountFactory::login_account(shared_ptr<asio::ip::tcp::socket> socket, error_code& ec)
{
    afDEBUG_LOG("DEBUG_account_factory", "login");
    char buf[1024];

    size_t length = socket->read_some(asio::buffer(buf), ec);   //ID
    if (ec) { EXCEPTIONS_LOG("EXCEPTIONS_account_factory", ec.message()); return; }

    for (size_t i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            return;
        }
    }

    uint64_t ID = stoi(string(buf, length));
    auto user = account_base.findUser(ID);
    if (user == nullptr) {
        EXCEPTIONS_LOG("Account_factory", "account not found");
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::noUser)}), ec);
        return;
    }
    else {
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
        if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    }

    length = socket->read_some(asio::buffer(buf), ec);        //password
    if (ec) { EXCEPTIONS_LOG("Account_factory - ", ec.message()); return; }

    string _password(buf, length);
    _password = Hash(_password);

    if (user->getPassword() == _password) {
        if (user->login(socket) == 1) { EXCEPTIONS_LOG("Account_factory", "account is already taken"); return; }
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);

        string accountData = user->getAccountData();
        socket->write_some(asio::buffer(accountData.data(), accountData.size()), ec);
        if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
        else { user->outBuffer(); }

        afDEBUG_LOG("DEBUG_account_factory", string("successful login account ID - " + to_string(ID)));
        if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    }
    else {
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongPassword) }), ec);
        if (ec) { EXCEPTIONS_LOG("Account_factory", ec.message()); return; }
    }
}