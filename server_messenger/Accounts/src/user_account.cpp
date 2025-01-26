#include "user_account.hpp"
#include "server_error.hpp"
#include "thread_pool.hpp"

//constructors
UserAccount::UserAccount(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string &userName, const string &password, const string &emale, const PhoneNumber &phoneNumber)
    : Account(socket, ID, userName, password), email(emale), phone_number(phoneNumber)
{
    acDEBUG_LOG("DEBUG_User_account", "UserAccount(const uint64_t ID, const string& userName, const string& password, const string& emale, const PhoneNumber& phoneNumber)");
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
    socket->async_read_some(asio::buffer(buf),
        [&](const error_code& ec, size_t length)
        {
            if (!ec) {
                threadPool->addTask([&]() {read_handler(buf, length); });
            }
            else {
                ERROR_LOG("ERROR_User_account", "error reading");
                socket->close();
                this->status = offline;
                return;
            }
        });
}
void UserAccount::read_handler(const char* buf, const size_t length)
{
    string msg(buf, length);
    USER_MESSAGE(this->getUserName(), msg);

    /*account operations*/
    if (msg == "__chAccount") {
        error_code ec;
        char subBuf[1024];

        size_t length = socket->read_some(asio::buffer(subBuf), ec);
        if (checkError(ec)) return;

        msg = string(subBuf, length);

        //Change user name
        if (msg == "__chName") {
            change_name(ec);
            return;
        }
        //Change password
        else if (msg == "__chPaswd") {
            change_password(ec);
            return;
        }
        //Change emale
        else if (msg == "__chEmail") {
            change_email(ec);
            return;
        }
        //Change phone number
        else if (msg == "__chPhoneNumber") {
            change_phone_number(ec);
            return;
        }
        //exit
        else if (msg == "__exit") {
            exit_account(ec);
            return;
        }
        //delete
        else if (msg == "__delete") {
            delete_account(ec);
            return;
        }
        else {
            reading();
            return;
        }
    }
    /*chat operations*/
    else if (msg == "__chat") {
        error_code ec;
        char subBuf[1024];

        size_t length = socket->read_some(asio::buffer(subBuf), ec);
        if (checkError(ec)) return;

        msg = string(subBuf, length);

        //add chat
        if (msg == "__createSoloChat") {
            create_solo_chat(ec);
            return;
        }
        else if (msg == "__addSoloChat") {
            add_solo_chat(ec);
            return;
        }
        else if (msg == "__createGroupChat") {
            create_group_chat(ec);
            return;
        }
        else if (msg == "__addGroupChat") {
            add_group_chat(ec);
            return;
        }
        else if (msg == "__inviteUserGroupChat") {
            invite_user(ec);
            return;
        }
        else if (msg == "__addUserGroupChat") {
            add_user_group_chat(ec);
            return;
        }
        else if (msg == "__selectChat") {
            select_chat(ec);
            return;
        }
        else {
            reading();
            return;
        }
    }
    /*msg output*/
    else {
        chat_manager.printChat(string("${" + to_string(this->getId()) + '}' + '[' + this->getUserName() + ']' + msg), this->getSocket());
        reading();
    }
}

bool UserAccount::checkError(const error_code& ec)
{
    if (ec) {
        ERROR_LOG("ERROR_User_account", "error reading");
        this->socket->close();
        this->status = offline;
        return true;
    }
    return false;
}

void UserAccount::change_name(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    string msg = string(buf, length);
    this->user_name = msg;

    reading();
    return;
}
void UserAccount::change_password(error_code& ec)
{
    char buf[1024];
    //old password
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    string msg = string(buf, length);
    if (this->password == Hash(msg)) {
        //new password
        length = socket->read_some(asio::buffer(buf), ec);
        if (checkError(ec)) return;

        msg = string(buf, length);
        this->password = Hash(msg);

        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
        if (checkError(ec)) return;

        reading();
        return;
    }
    else {
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongPassword) }), ec);
        if (checkError(ec)) return;

        reading();
        return;
    }
}
void UserAccount::change_email(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    string msg = string(buf, length);
    this->email = msg;

    reading();
    return;
}
void UserAccount::change_phone_number(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    PhoneNumber phoneNumber(string(buf, length));
    if (phoneNumber.isValid()) {
        acDEBUG_LOG("DEBUG_User_account", "Phone number valid");

        this->phone_number = phoneNumber;
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
        if (checkError(ec)) return;

    }
    else {
        acDEBUG_LOG("DEBUG_User_account", "Phone number not valid");

        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongPhoneNumber) }), ec);
        if (checkError(ec)) return;

    }
    reading();
    return;
}
void UserAccount::exit_account(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (string(buf, length) == "__Y") {
        acDEBUG_LOG("ERROR_Temp_account", "log out of account");
        this->socket->close();
        this->status = offline;
        return;
    }
    acDEBUG_LOG("ERROR_Temp_account", "don't log out of account");

    reading();
    return;
}
void UserAccount::delete_account(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (string(buf, length) == "__Y") {
        acDEBUG_LOG("ERROR_Temp_account", "delete account");
        this->socket->close();
        this->status = deleted;
        AccountFactory::free_id.push_back(this->getId());
        account_base.erase(this->getId());
        return;
    }
    reading();
    return;
}

void UserAccount::create_solo_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    for (size_t i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

            socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongID) }), ec);
            reading();
            return;
        }
    }

    uint64_t ID(stoi(string(buf, length)));
    chat_manager.createSoloChat(ID, account_base.findUser(this->ID));

    reading();
    return;
}
void UserAccount::add_solo_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    for (size_t i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

            reading();
            return;
        }
    }
    uint64_t ID(stoi(string(buf, length)));

    length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (length != 128) {
        acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongUID) }), ec);
        reading();
        return;
    }
    string chatUID = (string(buf, length));

    uint8_t result = chat_manager.addSoloChat(chatUID, ID);

    socket->write_some(asio::buffer({ static_cast<unsigned char>(result) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
}
void UserAccount::create_group_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    for (size_t i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

            reading();
            return;
        }
    }
    uint64_t ID(stoi(string(buf, length)));

    uint8_t result = chat_manager.createGroupChat(ID, account_base.findUser(this->ID));
    socket->write_some(asio::buffer({ static_cast<unsigned char>(result) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
}
void UserAccount::add_group_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    istringstream iss(string(buf, length));
    vector<uint64_t> IDs;

    string ID;
    while (iss >> ID) {
        bool isNum = true;
        for (auto& ex : ID) {
            if (!isdigit(ex)) {
                isNum = false;
            }
        }
        if (isNum) { IDs.push_back(stoi(ID)); }
    }
    length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (length != 128) {
        acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongUID) }), ec);
        reading();
        return;
    }
    string chatUID = (string(buf, length));

    uint8_t result = chat_manager.addGroupChat(chatUID, IDs);

    socket->write_some(asio::buffer({ static_cast<unsigned char>(result) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
}
void UserAccount::invite_user(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    for (size_t i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

            reading();
            return;
        }
    }
    uint64_t ID(stoi(string(buf, length)));

    length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (length != 128) {
        acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongUID) }), ec);
        reading();
        return;
    }
    string chatUID = (string(buf, length));

    uint8_t addUserResult = chat_manager.inviteUserGroupChat(ID, chatUID, account_base.findUser(this->ID));
    socket->write_some(asio::buffer({ static_cast<unsigned char>(addUserResult) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
}
void UserAccount::add_user_group_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    for (size_t i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

            reading();
            return;
        }
    }
    uint64_t ID(stoi(string(buf, length)));

    length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (length != 128) {
        acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongUID) }), ec);
        reading();
        return;
    }
    string chatUID = (string(buf, length));

    uint8_t addUserResult = chat_manager.addUserGroupChat(ID, chatUID, account_base.findUser(this->ID));
    socket->write_some(asio::buffer({ static_cast<unsigned char>(addUserResult) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
}
void UserAccount::select_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (length == 1) {
        chat_manager.setChatIndex("0");
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
        reading();
        return;
    }
    if (length != 128) {
        acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
        socket->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongUID) }), ec);
        reading();
        return;
    }
    string chatUID = (string(buf, length));

    uint8_t result = chat_manager.setChatIndex(chatUID);
    socket->write_some(asio::buffer({ static_cast<unsigned char>(result) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
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

void UserAccount::bufferingMsg(const string& msg)
{
    this->chat_manager.bufferingMsg(msg);
}

void UserAccount::outBuffer()
{
    for (auto& ex : chat_manager.getBuffer()) {
        this->socket->write_some(asio::buffer(ex.data(), ex.length()));
    }
}

const string UserAccount::getAccountData() const
{
    string data = Account::getAccountData();
    data += this->email + '\n' + this->phone_number.getNumber();
    return data;
}

//operators
ostream &operator<<(ostream &os, const UserAccount &ex)
{
    os << setw(16) << left << setfill('.') << "Emale" << ex.email << '\n';
    os << setw(16) << left << setfill('.') << "Phone number" << ex.phone_number;
    os << "\033[0m\n";
    return os;
}