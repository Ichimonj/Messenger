#include "TempAccount.hpp"
#include "ServerError.hpp"
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
                thread([&]() {read_handler(buf, length); }).join();
            }
            else {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
            }
        });
}
void TempAccount::read_handler(const char* buf, const size_t length)
{
    string msg(buf, length);
    USER_MESSAGE(this->getUserName(), msg);

    /*account operations*/
    if (msg == "__chAccount") {
        error_code ec;
        char subBuf[1024];

        size_t length = socket_->read_some(asio::buffer(subBuf), ec);
        if (ec) {
            ERROR_LOG("ERROR_Temp_account", "error reading");
            this->status_ = deleted;
            AccountFactory::free_id.push_back(this->getId());
            accountBase.erase(this->getId());
            return;
        }
        msg = string(subBuf, length);

        //Change user name
        if (msg == "__chName") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
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
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
                return;
            }

            msg = string(subBuf, length);
            if (this->password_ == Hash(msg)) {
                //new password
                length = socket_->read_some(asio::buffer(subBuf), ec);
                if (ec) {
                    ERROR_LOG("ERROR_Temp_account", "error reading");
                    this->status_ = deleted;
                    AccountFactory::free_id.push_back(this->getId());
                    accountBase.erase(this->getId());
                    return;
                }

                msg = string(subBuf, length);
                this->password_ = Hash(msg);

                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
                if (ec) {
                    ERROR_LOG("ERROR_Temp_account", "error reading");
                    this->status_ = deleted;
                    AccountFactory::free_id.push_back(this->getId());
                    accountBase.erase(this->getId());
                    return;
                }
                reading();
                return;
            }
            else {
                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::IOerror) }), ec);
                if (ec) {
                    ERROR_LOG("ERROR_Temp_account", "error reading");
                    this->status_ = deleted;
                    AccountFactory::free_id.push_back(this->getId());
                    accountBase.erase(this->getId());
                    return;
                }
                reading();
                return;
            }
        }
        //exit
        else if (msg == "__exit") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
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
            ERROR_LOG("ERROR_Temp_account", "error reading");
            this->status_ = deleted;
            AccountFactory::free_id.push_back(this->getId());
            accountBase.erase(this->getId());
            return;
        }
        msg = string(subBuf, length);

        //add chat
        if (msg == "__createSoloChat") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
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
            if (chatManager.createSoloChat(ID, accountBase.findUser(this->getId())) == 1) {
                acDEBUG_LOG("DEBUG_Temp_account", "invalid ID");

                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::IOerror) }), ec);
                if (ec) {
                    ERROR_LOG("ERROR_Temp_account", "error reading");
                    this->status_ = deleted;
                    AccountFactory::free_id.push_back(this->getId());
                    accountBase.erase(this->getId());
                    return;
                }

                reading();
                return;
            }
            acDEBUG_LOG("DEBUG_Temp_account", "user added");
            socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
                return;
            }

            reading();
        }
        else if (msg == "__addSoloChat") {
            acDEBUG_LOG("DEBUG_User_account", "add solo chat");
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
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

            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
                return;
            }
            if (length != 128) {
                acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
                reading();
                return;
            }
            string chatUID = (string(subBuf, length));

            uint8_t result = chatManager.addSoloChat(chatUID, ID);

            socket_->write_some(asio::buffer({ static_cast<unsigned char>(result) }), ec);
            reading();
        }
        else if (msg == "__createGroupChat") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
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

            if (chatManager.createGroupChat(ID, accountBase.findUser(this->getId())) == 1) {
                acDEBUG_LOG("DEBUG_Temp_account", "invalid ID");

                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::IOerror) }), ec);
                if (ec) {
                    ERROR_LOG("ERROR_Temp_account", "error reading");
                    this->status_ = deleted;
                    AccountFactory::free_id.push_back(this->getId());
                    accountBase.erase(this->getId());
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
        else if (msg == "__addGroupChat") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
                return;
            }
            istringstream iss(string(subBuf, length));
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
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
                return;
            }
            if (length != 128) {
                acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
                reading();
                return;
            }
            string chatUID = (string(subBuf, length));

            uint8_t result = chatManager.addGroupChat(chatUID, IDs);

            socket_->write_some(asio::buffer({ static_cast<unsigned char>(result) }), ec);
            reading();

        }
        else if (msg == "__addUserGroupChat") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
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

            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
                return;
            }
            if (length != 128) {
                acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
                reading();
                return;
            }
            string chatUID = (string(subBuf, length));

            uint8_t addUserResult = chatManager.addUserGroupChat(ID, chatUID, accountBase.findUser(this->getId()));
            socket_->write_some(asio::buffer({ static_cast<unsigned char>(addUserResult) }), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
                return;
            }
            reading();
        }
        else if (msg == "__selectChat") {
            length = socket_->read_some(asio::buffer(subBuf), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
                return;
            }

            if (length != 128) {
                acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");
                reading();
                return;
            }
            string chatUID = (string(subBuf, length));

            if (chatManager.setChatIndex(chatUID) == 1) {
                acDEBUG_LOG("DEBUG_Temp_account", "wrong UID");

                socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::IOerror) }), ec);
                if (ec) {
                    ERROR_LOG("ERROR_Temp_account", "error reading");
                    this->status_ = deleted;
                    AccountFactory::free_id.push_back(this->getId());
                    accountBase.erase(this->getId());
                    return;
                }
                reading();
                return;
            }
            socket_->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::successful) }), ec);
            if (ec) {
                ERROR_LOG("ERROR_Temp_account", "error reading");
                this->status_ = deleted;
                AccountFactory::free_id.push_back(this->getId());
                accountBase.erase(this->getId());
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
        chatManager.printChat(string("${" + to_string(this->getId()) + '}' + '[' + this->getUserName() + ']' + msg), this->getSocket());
        reading();
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
//The temporary account will be deleted after logging out, there is no need to archive messages
void TempAccount::bufferingMsg(const string& msg){ }
void TempAccount::outBuffer(){ }


//operators
ostream &operator<<(ostream &os, const TempAccount &ex)
{
    os << "\033[0m";
    return os;
}