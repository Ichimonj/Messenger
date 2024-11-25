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
        if (ec) {
            ERROR_LOG("ERROR_User_account", "error reading");
            this->socket_->close();
            this->status_ = offline;
            return;
        }
        msg = string(subBuf, length);

        //Change user name
        if (msg == "__chName") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_User_account", "error reading");
                this->socket_->close();
                this->status_ = offline;
                return;
            }

            msg = string(subBuf, length);
            this->userName_ = msg;

            reading();
        }
        //Change password
        else if (msg == "__chPaswd") {
            //old password
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_User_account", "error reading");
                this->socket_->close();
                this->status_ = offline;
                return;
            }

            msg = string(subBuf, length);
            if (this->password_ == Hash(msg)) {
                //new password
                length = socket_->read_some(asio::buffer(subBuf), ec);
                if (ec) {
                    ERROR_LOG("ERROR_User_account", "error reading");
                    this->socket_->close();
                    this->status_ = offline;
                    return;
                }

                msg = string(subBuf, length);
                this->password_ = Hash(msg);

                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
                if (ec) {
                    ERROR_LOG("ERROR_User_account", "error reading");
                    this->socket_->close();
                    this->status_ = offline;
                    return;
                }
                reading();
                return;
            }
            else {
                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::IOerror) }), ec);
                if (ec) {
                    ERROR_LOG("ERROR_User_account", "error reading");
                    this->socket_->close();
                    this->status_ = offline;
                    return;
                }
                reading();
                return;
            }
        }
        //Change emale
        else if (msg == "__chEmale") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_User_account", "error reading");
                this->socket_->close();
                this->status_ = offline;
                return;
            }

            msg = string(subBuf, length);
            this->emale_ = msg;

            reading();
        }
        //Change phone number
        else if (msg == "__chPhoneNumber") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_User_account", "error reading");
                this->socket_->close();
                this->status_ = offline;
                return;
            }

            PhoneNumber phoneNumber(string(subBuf, length));
            if (phoneNumber.isValid()) {
                acDEBUG_LOG("DEBUG_User_account", "Phone number valid");

                this->phoneNumber_ = phoneNumber;
                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
            }
            else {
                acDEBUG_LOG("DEBUG_User_account", "Phone number not valid");

                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::IOerror) }), ec);
            }
            reading();
        }
        //exit
        else if (msg == "__exit") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_User_account", "error reading");
                this->socket_->close();
                this->status_ = offline;
                return;
            }

            if (string(subBuf, length) == "__Y") {
                acDEBUG_LOG("ERROR_Temp_account", "log out of account");
                this->socket_->close();
                this->status_ = offline;
                return;
            }
            else {
                acDEBUG_LOG("ERROR_Temp_account", "don't log out of account");

                reading();
                return;
            }
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
        if (ec) {
            ERROR_LOG("ERROR_User_account", "error reading");
            this->socket_->close();
            this->status_ = offline;
            return;
        }
        msg = string(subBuf, length);

        //add chat
        if (msg == "__addChat") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_User_account", "error reading");
                this->socket_->close();
                this->status_ = offline;
                return;
            }

            for (int i = 0; i < length; i++) {
                if (!isdigit(subBuf[i])) {
                    acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

                    reading();
                    return;
                }
            }

            uint64_t ID(stoi(string(subBuf, length)));
            if (chatManager.addSoloChat(ID) == 1) {
                acDEBUG_LOG("DEBUG_Temp_account", "invalid ID");

                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::IOerror) }), ec);
                if (ec) {
                    ERROR_LOG("ERROR_User_account", "error reading");
                    this->socket_->close();
                    this->status_ = offline;
                    return;
                }

                reading();
                return;
            }
            acDEBUG_LOG("DEBUG_Temp_account", "user added");
            socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
            if (ec) {
                ERROR_LOG("ERROR_User_account", "error reading");
                this->socket_->close();
                this->status_ = offline;
                return;
            }

            reading();
        }
        else if (msg == "__selectChat") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            for (int i = 0; i < length; i++) {
                if (!isdigit(subBuf[i])) {
                    acDEBUG_LOG("DEBUG_Temp_account", "not a number entered");

                    reading();
                    return;
                }
            }

            uint32_t index(stoi(string(subBuf, length)));
            if (chatManager.setChatIndex(index) == 1) {
                acDEBUG_LOG("DEBUG_Temp_account", "error chat index");

                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::IOerror) }), ec);
                if (ec) {
                    ERROR_LOG("ERROR_User_account", "error reading");
                    this->socket_->close();
                    this->status_ = offline;
                    return;
                }
                reading();
                return;
            }
            socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
            if (ec) {
                ERROR_LOG("ERROR_User_account", "error reading");
                this->socket_->close();
                this->status_ = offline;
                return;
            }
            reading();
        }
        else {
            reading();
            return;
        }
    }
    /*msg output*/
    else {
        if (chatManager.printChat(string(this->getUserName() + " - " + msg + '\n')) == 2) {
            error_code ec;
            socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::msgInDeleteACcount) }), ec);
            if (ec) {
                ERROR_LOG("ERROR_User_account", "error reading");
                this->socket_->close();
                this->status_ = offline;
                return;
            }
        }
        reading();
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