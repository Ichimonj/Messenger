#include "chat_manager.hpp"
#include "account_base.hpp"
#include "preprocessor.hpp"
#include "server_error.hpp"
#include "rand.hpp"

/*CHAT MANAGER*/
uint8_t ChatManager::printChat(string&& msg, const shared_ptr<asio::ip::tcp::socket> socket)
{
    if (correspondence == nullptr) {
        msg = msg.substr(msg.find_first_of('['), msg.back());
        favorite_messages.push_back(msg);
    }
    else {
        correspondence->printChat("#(" + correspondence->chat_UID + ")" + msg, socket);
    }                             
    return funct_return::message::successful;
}

uint8_t ChatManager::setChatIndex(const string& chatUID)
{
    if (chatUID.size() == 1) {
        correspondence = nullptr;
    }
    auto chat = chats.find(chatUID);
    if (chat == chats.end()) {
        return funct_return::message::noChat;
    }
    else {
        correspondence = chat->second;
    }
    return funct_return::message::successful;
}

uint8_t ChatManager::createSoloChat(const uint64_t ID, const shared_ptr<Account> creator)
{
    cmDEBUG_LOG("ChatManager", "addSoloChat");
    
    error_code ec;
    auto user = account_base.findUser(ID);
    if (user == nullptr) {
        creator->getSocket()->write_some(asio::buffer({ static_cast<unsigned char>(funct_return::message::noUser) }), ec);
        return funct_return::message::noUser;
    }
    else {
        error_code ec;
        creator->getSocket()->write_some(asio::buffer({static_cast<unsigned char>(funct_return::message::successful)}), ec);
        Sleep(100);
        shared_ptr<SoloChat> chat = make_shared<SoloChat>(user);
        chat->generateUID(creator->getId());
        string chatInformMsg = string("#(" + chat->chat_UID + ")${" + to_string(user->getId()) + "}[" + user->getUserName() + ']');
        creator->getSocket()->write_some(asio::buffer(chatInformMsg.data(), chatInformMsg.length()));       //message to sender

        chatInformMsg = string("#(" + chat->chat_UID + ")${" + to_string(creator->getId()) + "}[" + creator->getUserName() + ']');
        user->getSocket()->write_some(asio::buffer(chatInformMsg.data(), chatInformMsg.length()));          //message to the interlocutor
        chats.insert({ chat->chat_UID,chat });

        return funct_return::message::successful;
    }
}

uint8_t ChatManager::createGroupChat(const uint64_t ID, const shared_ptr<Account> creator)
{
    cmDEBUG_LOG("ChatManager", "addGroupChat");

    auto user = account_base.findUser(ID);
    if (user == nullptr) {
        return funct_return::message::noUser;
    }
    else { 
        shared_ptr<GroupChat> chat = make_shared<GroupChat>(user);
        chat->generateUID(creator->getId());

        string chatInformMsg = string("#(" + chat->chat_UID + ")${" + to_string(user->getId()) + "}[" + user->getUserName() + ']');
        creator->getSocket()->write_some(asio::buffer(chatInformMsg.data(), chatInformMsg.length()));      //message to sender             
        chatInformMsg = string("#(" + chat->chat_UID + ")${" + to_string(creator->getId()) + "}[" + creator->getUserName() + ']');
        user->getSocket()->write_some(asio::buffer(chatInformMsg.data(), chatInformMsg.length()));  //message to the interlocutor
        chats.insert({ chat->chat_UID,chat });        
        
        return funct_return::message::successful;
    }
}

uint8_t ChatManager::inviteUserGroupChat(const uint64_t ID, const string& chatUID, const shared_ptr<Account> creator)
{
    cmDEBUG_LOG("ChatManager", "inviteUserGroupChat");

    auto user = account_base.findUser(ID);
    if (user == nullptr) {
        return funct_return::message::noUser;
    }
    auto chat = chats.find(chatUID);
    if (chat == chats.end()) {
        return funct_return::message::noChat;
    }
    uint8_t result = chat->second->addUserGroupChat(user);

    if (result == funct_return::message::successful) {
        string chatInformMsg = string("#(" + chat->second->chat_UID + ")");
        string newUserInform = chatInformMsg + string("@{" + to_string(user->getId()) + "}[" + user->getUserName() + ']');
        
        creator->getSocket()->write_some(asio::buffer(newUserInform.data(), newUserInform.length())); //message to sender

        shared_ptr<GroupChat> groupChat = static_pointer_cast<GroupChat>(chat->second);
        for (size_t i = 0; i < chat->second->getCountUser(); i++) {
            if (groupChat->getCorrespondent(i)->getId() != ID) {
                auto chatUser = groupChat->getCorrespondent(i);
                chatInformMsg += string("${" + to_string(chatUser->getId()) + "}[" + chatUser->getUserName() + ']');

                chatUser->getSocket()->write_some(asio::buffer(newUserInform.data(), newUserInform.length()));
            }
        }
        chatInformMsg += string("${" + to_string(creator->getId()) + "}[" + creator->getUserName() + ']');
        user->getSocket()->write_some(asio::buffer(chatInformMsg.data(), chatInformMsg.length()));
    }

    return result;
}

uint8_t ChatManager::addUserGroupChat(const uint64_t ID, const string& chatUID, const shared_ptr<Account> creator)
{

    cmDEBUG_LOG("ChatManager", "inviteUserGroupChat");

    auto user = account_base.findUser(ID);
    if (user == nullptr) {
        cout << "no user" << endl;
        return funct_return::message::noUser;
    }
    auto chat = chats.find(chatUID);
    if (chat == chats.end()) {
        cout << "no chat" << endl;
        return funct_return::message::noChat;
    }
    uint8_t result = chat->second->addUserGroupChat(user);
    
    return result;
}

uint8_t ChatManager::addSoloChat(const string& chatUID, const uint64_t ID)
{
    if (chats.find(chatUID) != chats.end()) {
        cmDEBUG_LOG("ChatManager", "already a chat");
        return funct_return::message::alreadyChat;
    }

    auto user = account_base.findUser(ID);
    if (user == nullptr) {
        return funct_return::message::noUser;
    }

    shared_ptr<SoloChat> chat = make_shared<SoloChat>(chatUID, user);
    chats.insert({ chat->chat_UID,chat });
    return funct_return::message::successful;
}

uint8_t ChatManager::addGroupChat(const string& chatUID, const vector<uint64_t> IDs)
{
    if (chats.find(chatUID) != chats.end()) { 
        cmDEBUG_LOG("ChatManager","already a chat")
        return funct_return::message::alreadyChat; 
    }

    vector<shared_ptr<Account>> users;
    for (auto& ID : IDs) {
        auto user = account_base.findUser(ID);
        if (user != nullptr) {
            users.push_back(user);
        }
    }
    if (users.empty()) {
        return funct_return::message::noUser;
    }

    shared_ptr<GroupChat> chat = make_shared<GroupChat>(chatUID, users);
    chats.insert({ chat->chat_UID,chat });
    return funct_return::message::successful;
}
void ChatManager::bufferingMsg(const string& msg)
{
    this->msg_buffer.push_back(msg);
}

vector<string> ChatManager::getBuffer()
{
    return msg_buffer;
}

/*SOLO CHAT*/
void SoloChat::printChat(const string&& msg, const shared_ptr<asio::ip::tcp::socket> socket) {
    cmDEBUG_LOG("SoloChat", "printChat");

    error_code ec;
    correspondent->getSocket()->write_some(asio::buffer(msg.data(), msg.length()), ec);
    if (ec) {
        if (correspondent->getStatus() == status::deleted) {
            string errorMsg = string("%{" + to_string(correspondent->getId()) + "}[" + correspondent->getUserName() + "]Del");
            socket->async_write_some(asio::buffer(errorMsg.data(), errorMsg.length()), [&](const error_code& ec, size_t) {});
        }
        else if (correspondent->getStatus() == status::offline) {
            correspondent->bufferingMsg(msg);
        }
        else {
            string errorMsg = string("%{" + to_string(correspondent->getId()) + "}[" + correspondent->getUserName() + "]IOerror");
            socket->async_write_some(asio::buffer(errorMsg.data(), errorMsg.length()), [&](const error_code& ec, size_t) {});
        }
    }
}

void SoloChat::generateUID(const uint64_t userID)
{
    chat_UID.push_back('0');
    string _userID = to_string(userID);
    string hashID = Hash(_userID);

    chat_UID += (string(hashID, 33));
    chat_UID+=(SSLrand(96));
    for (size_t i = 1; i < 128; i++) {
        if (chat_UID[i] == '[' || chat_UID[i] == ']' || chat_UID[i] == '{' || chat_UID[i] == '}') {
            chat_UID[i] += 1; 
        }
    }
}
uint8_t SoloChat::addUserGroupChat(const shared_ptr<Account> user)
{
    return funct_return::message::addUserToSoloChat;
}
/*GROUP CHAT*/
void GroupChat::printChat(const string&& msg, const shared_ptr<asio::ip::tcp::socket> socket){
    cmDEBUG_LOG("GroupChat", "printChat");

    error_code ec;
    for (auto& ex : correspondents) {
        ex->getSocket()->write_some(asio::buffer(msg.data(), msg.length()), ec);
        if (ec) {
            if (ex->getStatus() == status::deleted) {
                string errorMsg = string("%{" + to_string(ex->getId()) + "}[" + ex->getUserName() + "]Del");
                socket->async_write_some(asio::buffer(errorMsg.data(), errorMsg.length()), [&](const error_code& ec, size_t) {});
                auto deletedUser = find(correspondents.begin(), correspondents.end(), ex);
                if (deletedUser != correspondents.end()) {
                    correspondents.erase(deletedUser);
                }
            }
            else if (ex->getStatus() == status::offline) {
                ex->bufferingMsg(msg);
            }
            else {
                string errorMsg = string("%{" + to_string(ex->getId()) + "}[" + ex->getUserName() + "]IOerror");
                socket->async_write_some(asio::buffer(errorMsg.data(), errorMsg.length()), [&](const error_code& ec, size_t) {});
            }
        }
    }
}

void GroupChat::generateUID(const uint64_t userID)
{
    chat_UID.push_back('1');
    string _userID = to_string(userID);
    string hashID = Hash(_userID);

    chat_UID += (string(hashID, 33));
    chat_UID += (SSLrand(96));
    for (size_t i = 1; i < 128; i++) {
        if (chat_UID[i] == '[' || chat_UID[i] == ']' || chat_UID[i] == '{' || chat_UID[i] == '}') {
            chat_UID[i] += 1;
        }
    }
}

uint8_t GroupChat::addUserGroupChat(const shared_ptr<Account> user)
{
    cmDEBUG_LOG("GroupChat", "addUserGroupChat");
    auto _user = find(correspondents.begin(), correspondents.end(), user);
    if (_user != correspondents.end()) {
        return funct_return::message::alreadyUser;
    }
    correspondents.push_back(user);
    return funct_return::message::successful;
}

const size_t GroupChat::getCountUser() const
{
    return this->correspondents.size();
}

const shared_ptr<Account> GroupChat::getCorrespondent(const size_t num)
{
    if (num > correspondents.size() - 1) {
        return nullptr;
    }
    return correspondents[num];
}