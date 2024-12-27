#include "UserAccount.hpp"
#include "ServerError.hpp"
//constructors
UserAccount::UserAccount(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string &userName, const string &password, const string &emale, const PhoneNumber &phoneNumber)
    : Account(socket, ID, userName, password), emale_(emale), phoneNumber_(phoneNumber)
{
    acDEBUG_LOG("DEBUG_User_account", "UserAccount(const uint64_t ID, const string& userName, const string& password, const string& emale, const PhoneNumber& phoneNumber)");
    reading();
}
UserAccount::UserAccount(UserAccount &&other) noexcept
    : Account(move(other)), emale_(move(emale_)), phoneNumber_(move(phoneNumber_))
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
                thread([&]() {read_handler(buf, length); }).join();
            }
            else {
                ERROR_LOG("ERROR_User_account", "error reading");
                socket_->close();
                this->status_ = offline;
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

        size_t length = socket_->read_some(asio::buffer(subBuf), ec);
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

        size_t length = socket_->read_some(asio::buffer(subBuf), ec);
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
        chatManager.printChat(string("${" + to_string(this->getId()) + '}' + '[' + this->getUserName() + ']' + msg), this->getSocket());
        reading();
    }
}

bool UserAccount::checkError(const error_code& ec)
{
    if (ec) {
        ERROR_LOG("ERROR_User_account", "error reading");
        this->socket_->close();
        this->status_ = offline;
        return true;
    }
    return false;
}

void UserAccount::change_name(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    string msg = string(buf, length);
    this->userName_ = msg;

    reading();
    return;
}
void UserAccount::change_password(error_code& ec)
{
    char buf[1024];
    //old password
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    string msg = string(buf, length);
    if (this->password_ == Hash(msg)) {
        //new password
        length = socket_->read_some(asio::buffer(buf), ec);
        if (checkError(ec)) return;

        msg = string(buf, length);
        this->password_ = Hash(msg);

        socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
        if (checkError(ec)) return;

        reading();
        return;
    }
    else {
        socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongPassword) }), ec);
        if (checkError(ec)) return;

        reading();
        return;
    }
}
void UserAccount::change_email(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    string msg = string(buf, length);
    this->emale_ = msg;

    reading();
    return;
}
void UserAccount::change_phone_number(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    PhoneNumber phoneNumber(string(buf, length));
    if (phoneNumber.isValid()) {
        acDEBUG_LOG("DEBUG_User_account", "Phone number valid");

        this->phoneNumber_ = phoneNumber;
        socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
        if (checkError(ec)) return;

    }
    else {
        acDEBUG_LOG("DEBUG_User_account", "Phone number not valid");

        socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongPhoneNumber) }), ec);
        if (checkError(ec)) return;

    }
    reading();
    return;
}
void UserAccount::exit_account(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (string(buf, length) == "__Y") {
        acDEBUG_LOG("ERROR_Temp_account", "log out of account");
        this->socket_->close();
        this->status_ = offline;
        return;
    }
    acDEBUG_LOG("ERROR_Temp_account", "don't log out of account");

    reading();
    return;
}
void UserAccount::delete_account(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (string(buf, length) == "__Y") {
        acDEBUG_LOG("ERROR_Temp_account", "delete account");
        this->socket_->close();
        this->status_ = deleted;
        AccountFactory::free_id.push_back(this->getId());
        accountBase.erase(this->getId());
        return;
    }
    reading();
    return;
}

void UserAccount::create_solo_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    for (int i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

            socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongID) }), ec);
            reading();
            return;
        }
    }

    uint64_t ID(stoi(string(buf, length)));
    chatManager.createSoloChat(ID, accountBase.findUser(this->ID_));

    reading();
    return;
}
void UserAccount::add_solo_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    for (int i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

            reading();
            return;
        }
    }
    uint64_t ID(stoi(string(buf, length)));

    length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (length != 128) {
        acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
        socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongUID) }), ec);
        reading();
        return;
    }
    string chatUID = (string(buf, length));

    uint8_t result = chatManager.addSoloChat(chatUID, ID);

    socket_->write_some(asio::buffer({ static_cast<unsigned char>(result) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
}
void UserAccount::create_group_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    for (int i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

            reading();
            return;
        }
    }
    uint64_t ID(stoi(string(buf, length)));

    uint8_t result = chatManager.createGroupChat(ID, accountBase.findUser(this->ID_));
    socket_->write_some(asio::buffer({ static_cast<unsigned char>(result) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
}
void UserAccount::add_group_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
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
    length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (length != 128) {
        acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
        socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongUID) }), ec);
        reading();
        return;
    }
    string chatUID = (string(buf, length));

    uint8_t result = chatManager.addGroupChat(chatUID, IDs);

    socket_->write_some(asio::buffer({ static_cast<unsigned char>(result) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
}
void UserAccount::invite_user(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    for (int i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

            reading();
            return;
        }
    }
    uint64_t ID(stoi(string(buf, length)));

    length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (length != 128) {
        acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
        socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongUID) }), ec);
        reading();
        return;
    }
    string chatUID = (string(buf, length));

    uint8_t addUserResult = chatManager.inviteUserGroupChat(ID, chatUID, accountBase.findUser(this->ID_));
    socket_->write_some(asio::buffer({ static_cast<unsigned char>(addUserResult) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
}
void UserAccount::add_user_group_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    for (int i = 0; i < length; i++) {
        if (!isdigit(buf[i])) {
            acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

            reading();
            return;
        }
    }
    uint64_t ID(stoi(string(buf, length)));

    length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (length != 128) {
        acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
        socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongUID) }), ec);
        reading();
        return;
    }
    string chatUID = (string(buf, length));

    uint8_t addUserResult = chatManager.addUserGroupChat(ID, chatUID, accountBase.findUser(this->ID_));
    socket_->write_some(asio::buffer({ static_cast<unsigned char>(addUserResult) }), ec);
    if (checkError(ec)) return;

    reading();
    return;
}
void UserAccount::select_chat(error_code& ec)
{
    char buf[1024];
    size_t length = socket_->read_some(asio::buffer(buf), ec);
    if (checkError(ec)) return;

    if (length == 1) {
        chatManager.setChatIndex("0");
        socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
        reading();
        return;
    }
    if (length != 128) {
        acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
        socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::wrongUID) }), ec);
        reading();
        return;
    }
    string chatUID = (string(buf, length));

    uint8_t result = chatManager.setChatIndex(chatUID);
    socket_->write_some(asio::buffer({ static_cast<unsigned char>(result) }), ec);
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
    this->chatManager.bufferingMsg(msg);
}

void UserAccount::outBuffer()
{
    for (auto& ex : chatManager.getBuffer()) {
        this->socket_->write_some(asio::buffer(ex.data(), ex.length()));
    }
}

const string UserAccount::getAccountData() const
{
    string data = Account::getAccountData();
    data += this->emale_ + '\n' + this->phoneNumber_.getNumber();
    return data;
}

//operators
ostream &operator<<(ostream &os, const UserAccount &ex)
{
    os << setw(16) << left << setfill('.') << "Emale" << ex.emale_ << '\n';
    os << setw(16) << left << setfill('.') << "Phone number" << ex.phoneNumber_;
    os << "\033[0m\n";
    return os;
}