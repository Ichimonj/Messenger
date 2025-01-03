#pragma once
#include "ChatManager.hpp"

class Account {
public:
	Account(const uint64_t ID, const string& userName);
	Account(const uint64_t ID, const string& userName, const string& email, const string& phoneNumber);

public:
	void setSocket	(const shared_ptr<asio::ip::tcp::socket> socket);
	void setPassword(const string& password);
public:
	void read();
	void write(const string& str);

public:
	void accountOperation();
	void chatsOperation();

public:
	void changeUserName();
	void changePassword();
	void changeEmail();
	void changePhoneNumber();
	void changeLineSize();
	void exitAccount();
	void deleteAccount();

public:
	uint8_t serializationLogin();

private: 
	char buf[1024];
	shared_ptr<asio::ip::tcp::socket> socket_;

	ChatManager chatManager_;
	uint64_t	ID_;
	string		userName_;
	string		email_;
	string		phoneNumber_;
	string		password_;
};