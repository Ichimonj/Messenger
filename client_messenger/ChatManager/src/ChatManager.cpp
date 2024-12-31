#include "ChatManager.hpp"
#include "Lenguage.hpp"
#include "Console.hpp"
#include "Error.hpp"
#include "Command.hpp"
#include <iomanip>

bool isDigit(string& str)
{
	for (char& ch : str) {
		if (!isdigit(ch)) {
			return false;
		}
	}
	return true;
}
void safe_getline(string& in)
{
	getline(cin, in);
	while (in.empty()) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "Вы ничего не ввели - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "You have not entered anything - ";
#endif
		getline(cin, in);
	}

}
void stop()
{
	cin.clear();
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "Нажмите на любую клавишу что бы продолжить";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "Press any key to continue";
#endif
	cin.get();
}


User::User(const uint64_t ID, const string name)
	:ID(ID), name(name) {
}

User::User(const shared_ptr<User> user)
{
	this->ID	= user->ID;
	this->name	= user->name;
}

Message::Message(string msg, shared_ptr<User> user)
	:message(msg), user(user) {
};
ostream& operator<<(ostream& os, const Message& ex){
	size_t FILL = 15;
	os << setw(FILL) << left << setfill('_') << to_string(ex.user->ID) + " - " + ex.user->name << ex.message.c_str();
	return os;
}

Chat::Chat(const string& chatUID)
	:chatUID_(chatUID) {
}
void Chat::setChatName(const string& chatName){ 
	this->chatName_ = chatName; 
}
const vector<Message> Chat::getMsgBuffer() const{ 
	return this->msgBuffer_; 
}
const bool Chat::getIsActiveChat() const{
	return this->isActiveChat_;
}
void Chat::setIsActiveChat(const bool isActive){
	this->isActiveChat_ = isActive; 
}
const string Chat::getChatName() const{
	return this->chatName_;
}
const string Chat::getChatUID() const{
	return this->chatName_;
}
const uint32_t Chat::getNotViewedMessage() const{
	return this->notViewedMessage_;
}
void Chat::clearNotViewedMessage(){
	this->notViewedMessage_ = 0;
}

void Chat::printChat(size_t lineSize){
	system("cls");
	this->lineSize_ = lineSize;
	size_t i = 0;
	size_t msgBufferSize = msgBuffer_.size();
	if (lineSize_ < msgBufferSize) {
		i = msgBufferSize - lineSize_;
	}
	for (; i < msgBufferSize; i++) {
		cout << msgBuffer_[i] << endl;
	}
}

void Chat::msgBuffering(const Message msg){
	this->msgBuffer_.push_back(msg);
	if (isActiveChat_ != true) {
		notViewedMessage_++;
	}
	else {
		printChat(lineSize_);
	}
}

SoloChat::SoloChat(const string& chatUID, const User& user)
	:Chat(chatUID) {
	this->correspondent_ = make_shared<User>(user);
}

const string SoloChat::chatName() const{
	if (this->getChatName().size() == 0) {
		return this->correspondent_->name;
	}
	else {
		return this->getChatName();
	}
}

const bool SoloChat::isUserAvailable(uint64_t ID) const{
	return true;
}

GroupChat::GroupChat(const string& chatUID, const shared_ptr<User> user)
	:Chat(chatUID),correspondents_() {
	this->correspondents_.insert({ user->ID,user });
}

GroupChat::GroupChat(const string& chatUID, const vector<shared_ptr<User>> users)
	:Chat(chatUID),correspondents_() { 
	for (auto& user : users) {
		correspondents_.insert({ user->ID,user });
	}
}

void GroupChat::addUser(shared_ptr<User> user){
	this->correspondents_.insert({ user->ID,user });
}

const string GroupChat::chatName() const{
	if (this->getChatName().size() == 0) {
		string chatName;
		for (auto& user : this->correspondents_) {
			chatName += user.second->name;
			chatName += ", ";
		}
		chatName.resize(chatName.size() - 2);
		return chatName;
	}
	else {
		return this->getChatName();
	}
}

const bool GroupChat::isUserAvailable(uint64_t ID) const{
	if (correspondents_.find(ID) == correspondents_.end()) {
		return false;
	}
	return false;
}
void ChatManager::makeSoloChat(shared_ptr<asio::ip::tcp::socket> socket){
	system("cls");
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "Введите id собеседника - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "Enter the id of the interlocutor - ";
#endif
	string ID;
	safe_getline(ID);
	while (!isDigit(ID)) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "Id может быть только числом - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "Id can only be a number - ";
#endif
		safe_getline(ID);
	}
	error_code ec;
	vector<string> command = client_command::Command::createCommand(client_command::commands::create_solo_chat);
	command.push_back(ID);

	for (auto& ex : command) {
		socket->write_some(asio::buffer(ex.data(), ex.length()), ec);
		if (ec) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << ec.message() << "\nОшибка создания нового чата\n";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << ec.message() << "\nFailed to add new chat\n";
#endif
			return;
		}
		Sleep(50);
	}
	updateUIDbase();
	stop(); 
}

void ChatManager::makeGroupChat(shared_ptr<asio::ip::tcp::socket> socket){
	system("cls");
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "Введите id собеседника - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "Enter the id of the interlocutor - ";
#endif
	string ID;
	safe_getline(ID);
	while (!isDigit(ID)) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "Id может быть только числом - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "Id can only be a number - ";
#endif
		safe_getline(ID);
	}
	error_code ec;
	vector<string> command = client_command::Command::createCommand(client_command::commands::create_group_chat);
	command.push_back(ID);

	for (auto& ex : command) {
		socket->write_some(asio::buffer(ex.data(), ex.length()), ec);
		if (ec) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << ec.message() << "\nОшибка создания нового чата\n";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << ec.message() << "\nFailed to add new chat\n";
#endif
			return;
		}
		Sleep(50);
	}
	updateUIDbase();
	stop();
}

void ChatManager::inviteUser(shared_ptr<asio::ip::tcp::socket> socket){
	system("cls");
	uint32_t chatID;
	do {
		string _chatID;
		Console::chats(chats_);
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "exit - чтобы выйти\nВыберите чат из вышеуказанных - "; 
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "exit - to go out\nSelect a chat from the above - ";
#endif
		safe_getline(_chatID);
		if (_chatID == "exit") {
			return;
		}
		while (!isDigit(_chatID)) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Id может быть только числом - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "ID can only be a number - ";
#endif
			safe_getline(_chatID);
		}
		chatID = stoi(_chatID);
		if (chatID == 0) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Вы не можете добавить собеседника в избранные сообщения - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "You cannot add a person to your favorite messages - "; 
#endif
			stop();
		}
	} while (chatID == 0 || chatID > chats_.size());
	string chatUID = UIDbase_.find(chatID)->second;

	if (chatUID[0] == '1') {
		system("cls");
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "Невозможно добавить человека в индивидуальный чат"; 
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "It is not possible to add a person with an individual chat";
#endif
		stop();
		return;
	}

#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "Введите id нового собеседника - "; 
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "Enter the id of the new interlocutor - ";
#endif
	string ID;
	safe_getline(ID);
	while (!isDigit(ID)) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "Id может быть только числом - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "Id can only be a number - ";
#endif
		safe_getline(ID);
	}

	error_code ec;
	vector<string> command = client_command::Command::createCommand(client_command::commands::invite_user);
	command.push_back(ID);
	command.push_back(chatUID);
	for (auto& ex : command) {
		socket->write_some(asio::buffer(ex.data(), ex.length()), ec);
		if (ec) {
			cout << ec.message() << endl;
			return;
		}
		Sleep(50);
	}
}

void ChatManager::addUser(const string& chatUID, const uint64_t ID, shared_ptr<asio::ip::tcp::socket> socket){
	error_code ec;
	vector<string> command = client_command::Command::createCommand(client_command::commands::add_user_group_chat);
	command.push_back(to_string(ID));
	command.push_back(chatUID);
	for (auto& ex : command) {
		socket->write_some(asio::buffer(ex.data(), ex.length()), ec);
		if (ec) {
			cout << ec.message() << endl;
			return;
		}
		Sleep(50);
	}
	stop();
}

void ChatManager::selectChat(shared_ptr<asio::ip::tcp::socket> socket){
	system("cls");
	uint32_t chatID;
	do {
		string _chatID;
		Console::chats(chats_);
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "Выберете чат из вышеперечисленных - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "Select a chat from the above - ";
#endif
		safe_getline(_chatID);
		while (!isDigit(_chatID)) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "ID может быть только числом - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "ID can only be a number - ";
#endif
			safe_getline(_chatID);
		}
		chatID = stoi(_chatID);
	} while (chatID > UIDbase_.size());
	string chatUID;
	if (chatID == 0) {
		chatUID = "0";
		this->activeChat_ = nullptr;
	}
	else {
		chatUID = UIDbase_.find(chatID)->second;
		this->activeChat_ = chats_.find(chatUID)->second;
		this->activeChat_->setIsActiveChat(true);
		printChat();
	}

	error_code ec;
	vector<string> command = client_command::Command::createCommand(client_command::commands::select_chat);
	command.push_back(chatUID);
	for (auto& ex : command) {
		socket->write_some(asio::buffer(ex.data(), ex.length()),ec);
		if (ec) {
			cout << ec.message() << endl;
			return;
		}
		Sleep(50);
	}

	stop();
}

void ChatManager::changeUrName(const string& name) { you->name = name; }

void ChatManager::exitChat(){
	if (activeChat_ != nullptr) {
		activeChat_->setIsActiveChat(false);
	}
}
void ChatManager::changeLineSize(size_t lineSize)
{
	this->lineSize = lineSize;
}
void ChatManager::readHandler(string msg, shared_ptr<asio::ip::tcp::socket> socket){
	if (msg[0] == '#') {
		size_t startUID = msg.find_first_of('(');
		size_t endUID = msg.find_first_of(')');
		string chatUID = msg.substr(startUID + 1, endUID - startUID - 1);

		auto chat = chats_.find(chatUID);
		if (chat == chats_.end()) {
			if (chatUID[0] == '0') {
				addSoloChat(chatUID, msg, socket);
			}
			else if (chatUID[0] == '1') {
				addGroupChat(chatUID, msg, socket);
			}
			else {
				return;
			}
		}
		else {
			size_t startID = msg.find_first_of('{');
			size_t endID = msg.find_first_of('}');
			uint64_t ID = stoi(msg.substr(startID + 1, endID - startID - 1));

			size_t startName = msg.find_first_of('[');
			size_t endName = msg.find_first_of(']');
			string name = msg.substr(startName + 1, endName - startName - 1);

			if (chatUID[0] == '0') {
				chat->second->receivingMsg(msg.substr(endName + 1, msg.back()), ID, name);
			}
			else {
				if (!chat->second->isUserAvailable(ID)) {
					if (msg[endUID + 1] == '@') {
						addUser(chatUID, ID, socket);
					}
				}
				else {
					chat->second->receivingMsg(msg.substr(endName + 1, msg.back()), ID, name);
				}
			}
		}
	}
}
void ChatManager::writeHandler(string msg, shared_ptr<asio::ip::tcp::socket> socket){
	error_code ec;
	socket->write_some(asio::buffer(msg.data(), msg.length()), ec);
	if (activeChat_ == nullptr) {
		favoriteMessages_.push_back(msg);
	}
	else {
		activeChat_->msgBuffering(Message(msg, you));
	}
}
void ChatManager::addSoloChat(const string& UID, string& msg, shared_ptr<asio::ip::tcp::socket> socket){
	size_t startID = msg.find_first_of('{');
	size_t endID = msg.find_first_of('}');
	uint64_t ID = stoi(msg.substr(startID + 1, endID - startID - 1));

	size_t startName = msg.find_first_of('[');
	size_t endName = msg.find_first_of(']');
	string name = msg.substr(startName + 1, endName - startName - 1);

	error_code ec;
	vector<string> command = client_command::Command::createCommand(client_command::commands::add_solo_chat);
	command.push_back(to_string(ID));
	command.push_back(UID);
	for (auto& ex : command) {
		socket->write_some(asio::buffer(ex.data(), ex.length()), ec);
		if (ec) {
			return;
		}
		Sleep(50);
	}
	shared_ptr<Chat> chat = make_shared<SoloChat>(UID, User(ID, name));
	chats_.insert({ UID,chat });
	updateUIDbase();
}

void ChatManager::addGroupChat(const string& UID, string& msg, shared_ptr<asio::ip::tcp::socket> socket){
	vector<shared_ptr<User>> users;
	string IDs;
	while (msg.find_last_of('$') != string::npos) {
		size_t startID = msg.find_last_of('{');
		size_t endID = msg.find_last_of('}');
		uint64_t ID = stoi(msg.substr(startID + 1, endID - startID - 1));

		size_t startName = msg.find_last_of('[');
		size_t endName = msg.find_last_of(']');
		string name = msg.substr(startName + 1, endName - startName - 1);
		users.push_back(make_shared<User>(ID, name));
		IDs += to_string(ID);
		IDs += ' ';
		msg.resize(msg.find_last_of('$'));
	}
	IDs.resize(IDs.size() - 1);

	error_code ec;
	vector<string> command = client_command::Command::createCommand(client_command::commands::add_group_chat);
	command.push_back(IDs);
	command.push_back(UID);
	for (auto& ex : command) {
		socket->write_some(asio::buffer(ex.data(), ex.length()), ec);
		if (ec) {
			return;
		}
		Sleep(50);
	}
	shared_ptr<Chat> chat = make_shared<GroupChat>(UID, users);
	chats_.insert({ UID,chat });
	updateUIDbase();
}

void ChatManager::printChat(){
	if (activeChat_ == nullptr) {
		system("cls");
		size_t i = 0;
		size_t msgBufferSize = favoriteMessages_.size();;
		if (lineSize < msgBufferSize) {
			i = msgBufferSize - lineSize;
		}
		for (; i < msgBufferSize; i++) {
			cout << favoriteMessages_[i] << endl;
		}
	}
	else {
		activeChat_->printChat(lineSize);
	}
	activeChat_->clearNotViewedMessage();
}

//not yet optimized
void ChatManager::updateUIDbase(){
	uint32_t i = 1;
	UIDbase_.clear();
	for (auto chat : chats_) {
		UIDbase_.insert({ i,chat.first });
		i++;
	}
}
