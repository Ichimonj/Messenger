#include "Account.hpp"
#include "Lenguage.hpp"
#include "Console.hpp"
#include "Error.hpp"
#include "Command.hpp"
#include "FileNames.hpp"
#include <fstream>

Account::Account(const uint64_t ID, const string& userName)
	:ID_	 (ID),		userName_	(userName), 
	email_	 (""),		phoneNumber_(""), 
	password_(""),		chatManager_(ID_, userName)
{
}

Account::Account(const uint64_t ID, const string& userName, const string& email, const string& phoneNumber)
	:ID_	 (ID),		userName_	(userName), 
	email_	 (email),	phoneNumber_(phoneNumber), 
	password_(""),		chatManager_(ID_, userName)
{
}

void Account::setSocket(const shared_ptr<asio::ip::tcp::socket> socket)
{
	this->socket_ = socket;
}

void Account::setPassword(const string& password)
{
	this->password_ = password;
}

void Account::read()
{
	socket_->async_read_some(asio::buffer(buf), [&](const error_code& ec, size_t length) {
		if (ec) {

		}
		else {
			if (length == 1) {
				cout << "Operation status - ";
				cout << funct_return::Error::message(static_cast<funct_return::message>(buf[0])) << endl;
				read();
			}
			else {
				chatManager_.readHandler(string(buf, length), socket_);
				read();
			}
		}
		});
}

void Account::write(const string& str)
{
	if (str == "--help" || str == "-h") {
		Console::help();
		return;
	}
	else if (str == "--account" || str == "-a") {
		accountOperation();
		Console::help();
		return;
	}
	else if (str == "--chats" || str == "-c") {
		chatsOperation();
		return;
	}
	else {
		chatManager_.writeHandler(str, this->socket_);
		return;
	}
}

void Account::accountOperation()
{
	Console::accountOperation();

	while (true) {
		string operation;
		safe_getline(operation);
		if (operation == "--ch name" || operation == "-chn") {
			changeUserName();
			return;
		}
		else if (operation == "--ch passwd" || operation == "-chp") {
			changePassword();
			return;
		}
		else if (operation == "--ch email" || operation == "-che") {
			changeEmail();
			return;
		}
		else if (operation == "--ch phone number" || operation == "-chpn") {
			changePhoneNumber();
			return;
		}
		else if (operation == "--line size" || operation == "-ls") {
			changeLineSize();
			return;
		}
		else if (operation == "--exit") {
			exitAccount();
			return;
		}
		else if (operation == "--delete") {
			deleteAccount();
			return;
		}
		else if (operation == "--back") {
			return;
		}
	}
}

void Account::chatsOperation()
{
	Console::chatsHelp(chatManager_.getChats());
	while (true) {
		string operation;
		safe_getline(operation);
		if (operation == "--make Schat" || operation == "-msc") {
			chatManager_.makeSoloChat(this->socket_);
			Console::help();
			return;
		}
		else if (operation == "--make Gchat" || operation == "-mgc") {
			chatManager_.makeGroupChat(this->socket_);
			Console::help();
			return;
		}
		else if (operation == "--invite user" || operation == "-iu") {
			chatManager_.inviteUser(this->socket_);
			Console::help();
			return;
		}
		else if (operation == "--select" || operation == "-s") {
			chatManager_.selectChat(this->socket_);
			return;
		}
		else if (operation == "--back") {
			Console::help();
			return;
		}
	}

}

void Account::changeUserName()
{
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "Введите ваше имя - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "Enter your new name - ";
#endif
	string name;
	safe_getline(name);
	vector<string> command = client_command::Command::createCommand(client_command::change_name);
	command.push_back(name);
	this->userName_ = name;

	error_code ec;
	for (auto& ex : command) {
		this->socket_->write_some(asio::buffer(ex.data(), ex.length()), ec);
		Sleep(50);
	}
	chatManager_.changeUrName(name);
	stop();
}

void Account::changePassword()
{
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "Введите старый пароль - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "Enter old password - ";
#endif
	string oldPassword;
	safe_getline(oldPassword);
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "Введите новый пароль - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "Enter new password - ";
#endif
	string newPassword;
	safe_getline(newPassword);

	vector<string> command = client_command::Command::createCommand(client_command::change_password);
	command.push_back(oldPassword);
	command.push_back(newPassword);

	error_code ec;
	for (auto& ex : command) {
		this->socket_->write_some(asio::buffer(ex.data(), ex.length()), ec);
		Sleep(50);
	}
	stop();
}

void Account::changeEmail()
{
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "Введите новую почту - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "Enter new email - ";
#endif
	string email;
	safe_getline(email);

	vector<string> command = client_command::Command::createCommand(client_command::change_email);
	command.push_back(email);

	error_code ec;
	for (auto& ex : command) {
		this->socket_->write_some(asio::buffer(ex.data(), ex.length()), ec);
		Sleep(50);
	}
	stop();
}

void Account::changePhoneNumber()
{
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "Введите новый номер телефона - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "Enter new phone number - ";
#endif
	string phoneNumber;
	safe_getline(phoneNumber);

	vector<string> command = client_command::Command::createCommand(client_command::change_phone_number);
	command.push_back(phoneNumber);

	error_code ec;
	for (auto& ex : command) {
		this->socket_->write_some(asio::buffer(ex.data(), ex.length()), ec);
		Sleep(50);
	}
	stop();
}

void Account::changeLineSize()
{
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "Введите количество строк - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "Enter line size - ";
#endif
	string _lineSize;
	size_t lineSize;
	do {
		safe_getline(_lineSize);
		while (!isDigit(_lineSize)) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Количество строк может быть только числом - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "The number of lines can only be a number - ";
#endif
			safe_getline(_lineSize);
		}
		lineSize = stoi(_lineSize);
		if (lineSize == 0) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "не может быть 0 строк в чате - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "there cannot be 0 lines in the chat - "; 
#endif
		}
	} while (lineSize == 0);
	chatManager_.changeLineSize(lineSize);
}

void Account::exitAccount()
{
	char ch;
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "1 - Выйти\n2 - Не выходить\n"; 
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "1 - Log out\n2 - Don't log out\n";
#endif
	cin.ignore();
	cin >> ch;
	while (!strchr("12", ch)) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "1 - Выйти\n2 - Не выходить\n"; 
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "1 - Log out\n2 - Don't log out\n";
#endif
		cin.ignore();
		cin >> ch;
	}
	if (ch == '1') {
		error_code ec;
		vector<string> command = client_command::Command::createCommand(client_command::commands::exit_account);
		for (auto& ex : command) {
			socket_->write_some(asio::buffer(ex.data(), ex.length()), ec);
			Sleep(50);
		}
		exit(0);
	}
	else {
		return;
	}
	return;
}

void Account::deleteAccount()
{
	char ch;
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "1 - Удалить аккаунт\n2 - Не удалять аккаунт\n"; 
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "1 - Delete account\n2 - Do not delete account\n";
#endif
	cin.ignore();
	cin >> ch;
	while (!strchr("12", ch)) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "1 - Удалить аккаунт\n2 - Не удалять аккаунт\n"; 
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "1 - Delete account\n2 - Do not delete account\n";
#endif
		cin.ignore();
		cin >> ch;
	}

	if (ch == '1') {
		error_code ec;
		vector<string> command = client_command::Command::createCommand(client_command::commands::delete_account);
		for (auto& ex : command) {
			socket_->write_some(asio::buffer(ex.data(), ex.length()), ec);
			if (ec) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
				cout << ec.message() << "\nОшибка удаления аккаунта";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
				cout << ec.message() << "\nFailed to delete account";
#endif
				return;
			}
			Sleep(50);
		}
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "Вы успешно удалили свой аккаунт.\n"; 
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "You have successfully deleted your account.\n";
#endif
		exit(0);
	}
	else {
		return;
	}
}

uint8_t Account::serializationLogin()
{
	ofstream file;
	file.open(login_file_name, ios::binary);
	if (!file.is_open()) {
		return 1;
	}
	size_t sizePassword = this->password_.size();

	file.write(reinterpret_cast<char*>(&this->ID_), sizeof(this->ID_));
	file.write(reinterpret_cast<char*>(&sizePassword),sizeof(size_t));
	file.write(password_.c_str(), password_.size());
	return 0;
}
