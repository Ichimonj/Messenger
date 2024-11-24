#include "ChatManager.hpp"
#include"AccountBase.hpp"
#include "Preprocessor.hpp"
ChatManager::ChatManager()
{
}

int ChatManager::addSoloChat(uint64_t ID)
{
    shared_ptr<Account> user = accountBase.findUser(ID);
    if (user != nullptr) {
        cmDEBUG_LOG("DEBUG_Chat_manager", "add user in chat");
        soloChat.push_back(user);
        
        return 0;
    }
    else {
        cmDEBUG_LOG("DEBUG_Chat_manager", "user not find");
        return 1;
    }
}

int ChatManager::printChat(string&& msg)
{
    if (chatIndex > soloChat.size() || chatIndex <= 0) {
        cmDEBUG_LOG("DEBUG_Chat_manager", "error chat index");
        return 1;
    }
    else {
        error_code ec;
        soloChat[chatIndex - 1]->getSocket()->write_some(asio::buffer(msg.data(), msg.length()), ec);
        if (ec) {
            ERROR_LOG("ERROR_Chat_manager", ec.message());

            if (soloChat[chatIndex - 1]->getStatus() == deleted) {
                ERROR_LOG("ERROR_Chat_manager", "Account deleted");
                soloChat.erase(soloChat.begin() + (chatIndex - 1));
                chatIndex = 0;
                return 2;
            }

            return 1;
        }
        return 0;
    }
}

int ChatManager::setChatIndex(uint32_t index)
{
    if (chatIndex > soloChat.size() || chatIndex < 0) {
        cmDEBUG_LOG("DEBUG_Chat_manager", "error chat index");
        return 1;
    }
    else {
        chatIndex = index;
        return 0;
    }
}
