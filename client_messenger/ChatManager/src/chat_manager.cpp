#include "chat_manager.hpp"
#include "lenguage.hpp"
#include "console.hpp"
#include "file_work.hpp"
#include "file_names.hpp"
#include "error.hpp"
#include "command.hpp"
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

void User::serialization(ofstream& file)
{
	try
	{
		record(file, ID);
		record(file, name);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

void User::deserialization(ifstream& file)
{
	try
	{
		reading(file, &ID);
		reading(file, &name);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

Message::Message(string msg, shared_ptr<User> user)
	:message(msg), user(user) {
}

void Message::serialization(ofstream& file)
{
	try
	{
		record(file, message);
		user->serialization(file);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}
void Message::deserialization(ifstream& file)
{
	try
	{
		reading(file, &message);
		user = make_shared<User>();
		user->deserialization(file);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

ostream& operator<<(ostream& os, const Message& ex){
	size_t FILL = 15;
	os << setw(FILL) << left << setfill('_') << to_string(ex.user->ID) + " - " + ex.user->name << ex.message.c_str();
	return os;
}

Chat::Chat(const string& chatUID)
	:chat_UID(chatUID) {
}
void Chat::setChatName(const string& chatName){ 
	this->chat_name = chatName; 
}
const vector<Message> Chat::getMsgBuffer() const{ 
	return this->msg_buffer; 
}
const bool Chat::getIsActiveChat() const{
	return this->is_active_chat;
}
void Chat::setIsActiveChat(const bool isActive){
	this->is_active_chat = isActive; 
}
const string Chat::getChatName() const{
	return this->chat_name;
}
const string Chat::getChatUID() const{
	return this->chat_UID;
}
const uint32_t Chat::getNotViewedMessage() const{
	return this->not_viewed_message;
}
void Chat::clearNotViewedMessage(){
	this->not_viewed_message = 0;
}

void Chat::printChat(size_t lineSize){
	system("cls");
	this->line_size = lineSize;
	size_t i = 0;
	size_t msgBufferSize = msg_buffer.size();
	if (line_size < msgBufferSize) {
		i = msgBufferSize - line_size;
	}
	for (; i < msgBufferSize; i++) {
		cout << msg_buffer[i] << endl;
	}
}

void Chat::msgBuffering(const Message msg){
	this->msg_buffer.push_back(msg);
	if (is_active_chat != true) {
		not_viewed_message++;
	}
	else {
		printChat(line_size);
	}
}

void Chat::chatSerialization(ofstream& file)
{
	try
	{
		/*chat name*/
		record(file, chat_name);

		/*Not Viewed Message*/
		record(file, not_viewed_message);

		/*Msg Buffer(size)*/
		uint64_t msgBufferSize = msg_buffer.size();
		record(file, msgBufferSize);
		/*Msg Buffer(message)*/
		for (auto& message : msg_buffer)
		{
			message.serialization(file);
		}

		/*chat uid*/
		record(file, chat_UID);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

void Chat::chatDeserialization(ifstream& file, shared_ptr<User> you, shared_ptr<User> correspondent)
{
	try
	{
		/*chat name*/
		reading(file, &chat_name);

		/*Not Viewed Message*/
		reading(file, (uint64_t*)&not_viewed_message);

		/*chat users*/
		map<uint64_t, shared_ptr<User>> users;
		users.insert({ you->ID, you });
		users.insert({ correspondent->ID, correspondent });

		/*Msg Buffer(size)*/
		uint64_t msgBufferSize;
		reading(file, &msgBufferSize);
		msg_buffer.resize(msgBufferSize);
		/*Msg Buffer(message)*/
		for (int i = 0; i < msgBufferSize; i++)
		{
			Message msg;
			msg.deserialization(file);
			if (users.find(msg.user->ID) == users.end())
			{
				throw exception("Файл поврежден");
				return;
			}
			msg_buffer[i] = msg;
		}

		/*chat uid*/
		reading(file, &chat_UID);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

void Chat::chatDeserialization(ifstream& file, shared_ptr<User> you, map<uint64_t, shared_ptr<User>> correspondents)
{
	try
	{
		/*chat name*/
		reading(file, &chat_name);

		/*Not Viewed Message*/
		reading(file, (uint64_t*)&not_viewed_message);

		/*chat users*/
		map<uint64_t, shared_ptr<User>> users;
		users = correspondents;
		users.insert({ you->ID, you });

		/*Msg Buffer(size)*/
		uint64_t msgBufferSize;
		reading(file, &msgBufferSize);
		msg_buffer.resize(msgBufferSize);
		/*Msg Buffer(message)*/
		for (int i = 0; i < msgBufferSize; i++)
		{
			Message msg;
			msg.deserialization(file);
			if (users.find(msg.user->ID) == users.end())
			{
				throw exception("Файл поврежден");
				return;
			}
			msg_buffer[i] = msg;
		}

		/*chat uid*/
		reading(file, &chat_UID);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

SoloChat::SoloChat(const string& chatUID, const User& user)
	:Chat(chatUID) {
	this->correspondent = make_shared<User>(user);
}

const string SoloChat::chatName() const{
	if (this->getChatName().size() == 0) {
		return this->correspondent->name;
	}
	else {
		return this->getChatName();
	}
}

const bool SoloChat::isUserAvailable(uint64_t ID) const{
	return true;
}

void SoloChat::serialization(ofstream& file)
{
	try
	{
		/*chat type*/
		char ch = '0';
		record(file, &ch);

		/*correspondent*/
		correspondent->serialization(file);

		/*char*/
		this->chatSerialization(file);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}
void SoloChat::deserialization(ifstream& file, shared_ptr<User> you)
{
	try
	{
		/*correspondent*/
		correspondent = make_shared<User>();
		correspondent->deserialization(file);

		/*char*/
		this->chatDeserialization(file, you, correspondent);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

GroupChat::GroupChat(const string& chatUID, const shared_ptr<User> user)
	:Chat(chatUID),correspondents() {
	this->correspondents.insert({ user->ID,user });
}

GroupChat::GroupChat(const string& chatUID, const vector<shared_ptr<User>> users)
	:Chat(chatUID),correspondents() { 
	for (auto& user : users) {
		correspondents.insert({ user->ID,user });
	}
}

void GroupChat::addUser(shared_ptr<User> user){
	this->correspondents.insert({ user->ID,user });
}

const string GroupChat::chatName() const{
	if (this->getChatName().size() == 0) {
		string chatName;
		for (auto& user : this->correspondents) {
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
	if (correspondents.find(ID) == correspondents.end()) {
		return false;
	}
	return false;
}
void GroupChat::serialization(ofstream& file)
{
	try
	{
		/*chat type*/
		char ch = '1';
		record(file, &ch);

		/*correspondents size*/
		uint64_t correspondentsSize = correspondents.size();
		record(file, correspondentsSize);

		/*correspondents*/
		for (auto& user : correspondents)
		{
			user.second->serialization(file);
		}

		/*Chat*/
		this->chatSerialization(file);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}
void GroupChat::deserialization(ifstream& file, shared_ptr<User> you)
{
	try
	{
		/*correspondents size*/
		uint64_t correspondentsSize;
		reading(file, &correspondentsSize);

		/*correspondents*/
		for (int i = 0; i < correspondentsSize; i++)
		{
			shared_ptr<User> user = make_shared<User>();
			user->deserialization(file);
			correspondents.insert({ user->ID, user });
		}

		/*Chat*/
		this->chatDeserialization(file, you, correspondents);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}
ChatManager::ChatManager()
{
}
ChatManager::ChatManager(uint64_t ID, const string& name)
{
	this->you = make_shared<User>(ID, name);
	try
	{
		ifstream file;
		file.open(chat_mager_file_name, ios::binary);
		this->deserialization(file);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		stop();
	}
};
ChatManager::~ChatManager()
{
	if (save_chat_manager) {
		ofstream file;
		file.open(chat_mager_file_name, ios::binary);
		if (!file.is_open()) {
			return;
		}
		this->serialization(file);
	}
	else {
		return;
	}
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
		Console::chats(chats);
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
	} while (chatID == 0 || chatID > chats.size());
	string chatUID = UID_base.find(chatID)->second;

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
		Console::chats(chats);
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
	} while (chatID > UID_base.size());
	string chatUID;
	if (chatID == 0) {
		chatUID = "0";
		this->active_chat = nullptr;
	}
	else {
		chatUID = UID_base.find(chatID)->second;
		this->active_chat = chats.find(chatUID)->second;
		this->active_chat->setIsActiveChat(true);
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
	if (active_chat != nullptr) {
		active_chat->setIsActiveChat(false);
	}
}
void ChatManager::changeLineSize(size_t lineSize)
{
	this->line_size = lineSize;
}
void ChatManager::changeSaveChatManager(bool saveChatManager)
{
	this->save_chat_manager = saveChatManager;
}
void ChatManager::readHandler(string msg, shared_ptr<asio::ip::tcp::socket> socket){
	if (msg[0] == '#') {
		size_t startUID = msg.find_first_of('(');
		size_t endUID = msg.find_first_of(')');
		string chatUID = msg.substr(startUID + 1, endUID - startUID - 1);

		auto chat = chats.find(chatUID);
		if (chat == chats.end()) {
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
	if (active_chat == nullptr) {
		favorite_messages.push_back(msg);
	}
	else {
		active_chat->msgBuffering(Message(msg, you));
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
	chats.insert({ UID,chat });
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
	chats.insert({ UID,chat });
	updateUIDbase();
}

void ChatManager::printChat(){
	if (active_chat == nullptr) {
		system("cls");
		size_t i = 0;
		size_t msgBufferSize = favorite_messages.size();;
		if (line_size < msgBufferSize) {
			i = msgBufferSize - line_size;
		}
		for (; i < msgBufferSize; i++) {
			cout << favorite_messages[i] << endl;
		}
	}
	else {
		active_chat->printChat(line_size);
	}
	active_chat->clearNotViewedMessage();
}

//not yet optimized
void ChatManager::updateUIDbase(){
	uint32_t i = 1;
	UID_base.clear();
	for (auto chat : chats) {
		UID_base.insert({ i,chat.first });
		i++;
	}
}

void ChatManager::serialization(ofstream& file)
{
	try
	{
		/*you*/
		you->serialization(file);

		/*chats size*/
		uint64_t chatsSize = chats.size();
		record(file, chatsSize);
		/*chats*/
		for (auto& chat : chats)
		{
			chat.second->serialization(file);
		}
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

void ChatManager::deserialization(ifstream& file)
{
	try
	{
		if (file.tellg() == 0) {
			throw exception("нет сохранных переписок для вашего аккаунта");
			return;
		}

		/*you*/
		shared_ptr<User> you_ = make_shared<User>();
		you_->deserialization(file);

		if (you_->ID == you->ID) {
			throw exception("нет сохранных переписок для вашего аккаунта");
			return;
		}

		/*chats size*/
		uint64_t chatsSize;
		reading(file, &chatsSize);
		/*chats*/
		for (int i = 0; i < chatsSize; i++)
		{
			char type;
			reading(file, &type);
			if (type == '0')
			{
				shared_ptr<SoloChat> chat = make_shared<SoloChat>();
				chat->deserialization(file, you);
				chats.insert({ chat->getChatUID(), chat });
			}
			else if (type == '1')
			{
				shared_ptr<GroupChat> chat = make_shared<GroupChat>();
				chat->deserialization(file, you);
				chats.insert({ chat->getChatUID(), chat });
			}
			else
			{
				throw exception("Файл поврежден");
			}
		}
	}
	catch (const std::exception& e)
	{
		throw e;
	}

	updateUIDbase();
}