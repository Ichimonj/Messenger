#include"Account.hpp"
#include"Console.hpp"
#include"stack"
#include"Error.hpp"
#include<iostream>
asio::io_context context;

shared_ptr<asio::ip::tcp::socket> socket_;
asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1"), 8080);

shared_ptr<Account> login() {
	error_code ec;
	socket_ = make_shared<asio::ip::tcp::socket>(context);
	socket_->connect(endpoint, ec);
	while (ec) {
		cout << ec.message() << endl;
		socket_->connect(endpoint, ec);
	}
	Console::loginHelp();

	string command;
	getline(cin, command);
	while (command != "--login" && command != "--registry" && command != "-l" && command != "-r") {
		Console::loginHelp();
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "Пожалуйста, введите правильную команду" << endl; 
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "Please enter the correct command" << endl;
#endif 
		getline(cin, command);
	}

	if (command == "--login" || command == "-l") {
		system("cls");
		error_code ec;
		char buf[1024];
		socket_->write_some(asio::buffer(string("login").data(), 5), ec);
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "Введите ваше id - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "Enter your id - ";
#endif
		string ID;
		safe_getline(ID);

		socket_->write_some(asio::buffer(ID.data(), ID.size()), ec);
		size_t length = socket_->read_some(asio::buffer(buf), ec);
		if (static_cast<funct_return::message>(buf[0]) != funct_return::message::successful) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Такого пользователя не существует\n";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "This user does not exist\n";
#endif
			return nullptr;
		}

#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
		cout << "Введите пароль - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "Enter password - ";
#endif
		string password;
		safe_getline(password);

		socket_->write_some(asio::buffer(password.data(), password.size()), ec);
		length = socket_->read_some(asio::buffer(buf), ec);
		if (static_cast<funct_return::message>(buf[0]) != funct_return::message::successful) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Вы ввели неверный пароль\n"; 
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "You have entered an incorrect password\n";
#endif
			return nullptr;
		}
		length = socket_->read_some(asio::buffer(buf), ec);
		vector<string>accountData;
		string in;
		stringstream ss(string(buf, length));
		while (getline(ss, in)) {
			accountData.push_back(in);
		}

		shared_ptr<Account> account = make_shared<Account>(stoi(accountData[0]), accountData[1], accountData[2], accountData[3]);
		return account;
	}
	else if (command == "--registry" || command == "-r") {
		Console::accountType();

		getline(cin, command);
		while (command != "--temp" && command != "-t" && command != "--no temp" && command != "-nt") {
			Console::accountType();
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Пожалуйста введите правильную команду" << endl;
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "Please enter the correct command" << endl;
#endif 
			getline(cin, command);
		}

		if (command == "--temp" || command == "-t") {
			system("cls");
			char buf[1024];
			error_code ec;

			socket_->write_some(asio::buffer(string("temp").data(), 4), ec);

#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Введите имя - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "Enter username - ";
#endif
			string name;		//name
			safe_getline(name);
			socket_->write_some(asio::buffer(name.data(), name.size()), ec);
			size_t length = socket_->read_some(asio::buffer(buf), ec);
			if (static_cast<funct_return::message>(buf[0]) != funct_return::message::successful) {

			}

#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Введите пароль - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "Enter password - ";
#endif
			string password;	//password
			safe_getline(password);
			socket_->write_some(asio::buffer(password.data(), password.size()), ec);
			length = socket_->read_some(asio::buffer(buf), ec);
			if (static_cast<funct_return::message>(buf[0]) != funct_return::message::successful) {

			}
			length = socket_->read_some(asio::buffer(buf), ec);
			stringstream ss(string(buf, length));
			string in;
			vector<string>accountData;
			while (getline(ss, in)) {
				accountData.push_back(in);
			}
			shared_ptr<Account>account = make_shared<Account>(stoi(accountData[0]), accountData[1]);
			return account;
		}
		else if (command == "--no temp" || command == "-nt") {
			system("cls");
			char buf[1024];
			error_code ec;

			socket_->write_some(asio::buffer(string("no_temp").data(), 7), ec);

#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Введите имя - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "Enter username - ";
#endif
			string name;		//name
			safe_getline(name);
			socket_->write_some(asio::buffer(name.data(), name.size()), ec);
			size_t length = socket_->read_some(asio::buffer(buf), ec);
			if (static_cast<funct_return::message>(buf[0]) != funct_return::message::successful) {

			}

#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Введите пароль - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "Enter password - ";
#endif
			string password;	//password
			safe_getline(password);
			socket_->write_some(asio::buffer(password.data(), password.size()), ec);
			length = socket_->read_some(asio::buffer(buf), ec);
			if (static_cast<funct_return::message>(buf[0]) != funct_return::message::successful) {

			}

#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Введите почту - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "Enter email - ";
#endif
			string email;		//email
			safe_getline(email);
			socket_->write_some(asio::buffer(email.data(), email.size()), ec);
			length = socket_->read_some(asio::buffer(buf), ec);
			if (static_cast<funct_return::message>(buf[0]) != funct_return::message::successful) {

			}

#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
			cout << "Введите номер телефона - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "Enter phone number - ";
#endif
			string phoneNumber;	//phone number
			safe_getline(phoneNumber);
			socket_->write_some(asio::buffer(phoneNumber.data(), phoneNumber.size()), ec);
			length = socket_->read_some(asio::buffer(buf), ec);
			while (static_cast<funct_return::message>(buf[0]) != funct_return::message::successful) {
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
				cout << "Введите правильынй номер телефона - ";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
				cout << "Enter a valid phone number - ";
#endif
				phoneNumber.clear();
				safe_getline(phoneNumber);
				socket_->write_some(asio::buffer(phoneNumber.data(), phoneNumber.size()), ec);
				length = socket_->read_some(asio::buffer(buf), ec);
			}
			length = socket_->read_some(asio::buffer(buf), ec);

			stringstream ss(string(buf, length));
			string in;
			vector<string>accountData;
			while (getline(ss, in)) {
				accountData.push_back(in);
			}
			shared_ptr<Account>account = make_shared<Account>(stoi(accountData[0]), accountData[1], accountData[2], accountData[3]);
			return account;
		}
		else {
			abort();
		}
	}
	else {
		abort();
	}
}

int main(int argc,char* argv[]) {

	shared_ptr<Account> account;
#ifdef _DEBUG
	socket_ = make_shared<asio::ip::tcp::socket>(context);
	socket_->connect(endpoint);

	for (int i = 0; i < argc; i++) {
		cout << argv[i] << endl;
	}
	string a = argv[argc - 1];
	socket_->write_some(asio::buffer(string("no_temp").data(), 7));
	Sleep(150);
	vector<string> comman{ a,a,a,"88005553535" };
	for (auto& ex : comman) {
		socket_->write_some(asio::buffer(ex.data(), ex.length()));
		Sleep(150);
	}
	account = make_shared<Account>(1, "Oleg");

#else
	account = login();
	while (account == nullptr) {
#if(CURRENT_LANGUAGE == LANGUAGE_RU)
		cout << "1 - Повторить попытку\n2 - Выйти\n";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
		cout << "1 - Try again\n2 - Quit\n";
#endif
		char ch;
		cin >> ch;
		while (!strchr("12", ch)) {
			system("cls");
#if(CURRENT_LANGUAGE == LANGUAGE_RU)
			cout << "1 - Повторить попытку\n2 - Выйти\n";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
			cout << "1 - Try again\n2 - Quit\n";
#endif
			cin >> ch;
		}
		if (ch == '1') {
			account = login();
		}
		else if (ch == '2') {
			exit(0);
		}
		else {
			abort();
		}
	}

#endif // DEBUG

account->setSocket(socket_);
Console::help();
account->read();

thread t1([&]() {context.run(); });
for (;;) {
	string msg;
	safe_getline(msg);
	account->write(msg);
}
t1.join();
}