#include "ChatManager.hpp"
#include "AccountBase.hpp"
#include "Preprocessor.hpp"
ChatManager::ChatManager()
{
}

int ChatManager::addSoloChat(uint64_t ID,shared_ptr<asio::ip::tcp::socket> socket)
{
    shared_ptr<Account> user = accountBase.findUser(ID);
    if (user != nullptr) {
        cmDEBUG_LOG("DEBUG_Chat_manager", "add user in chat");
        soloChat.insert({ user->getId(),user });

        string msg = string("@{" + to_string(user->getId()) + '}' + '[' + user->getUserName() + ']');
        socket->write_some(asio::buffer(msg.data(), msg.length()));
        return 0;
    }
    else {
        cmDEBUG_LOG("DEBUG_Chat_manager", "user not find");
        return 1;
    }
}

int ChatManager::printChat(string&& msg)
{
    if (�orrespondent != nullptr) {
        error_code ec;
        �orrespondent->getSocket()->write_some(asio::buffer(msg.data(), msg.length()), ec);
        if (ec) {
            ERROR_LOG("ERROR_Chat_manager", ec.message());

            if (�orrespondent->getStatus() == deleted) {
                ERROR_LOG("ERROR_Chat_manager", "Account deleted");
                soloChat.erase(�orrespondent->getId());
                this->�orrespondent = nullptr;
                return 2;
            }
            else if(�orrespondent->getStatus() == offline){
                cmDEBUG_LOG("DEBUG_Chat_manager", "buffering msg");
                �orrespondent->bufferingMsg(msg);
                return 0;
            }
            return 1;
        }
    }
    else {
        return 2;
    }
    return 0;
}

int ChatManager::setChatIndex(uint64_t index)
{
    auto user = soloChat.find(index);
    if (user == soloChat.end()) {
        cmDEBUG_LOG("DEBUG_Chat_manager", "error chat index");
        return 1;
    }
    else {
        �orrespondent = user->second;
    }
    return 0;
}

void ChatManager::bufferingMsg(string& msg)
{
    cmDEBUG_LOG("DEBUG_Chat_manager", "buffering msg");
    this->bufferMsg.push_back(msg);
}

vector<string> ChatManager::getBuffer()
{
    return this->bufferMsg;
}
