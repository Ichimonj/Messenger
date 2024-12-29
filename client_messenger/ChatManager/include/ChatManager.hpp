#pragma once
#include<iostream>
#include<map>
#include<vector>
#include"asio.hpp"

using namespace std;

bool isDigit(string& str);
void safe_getline(string& in);
void stop();

struct User {
	User(const uint64_t ID, const string name);

	uint64_t ID;
	string   name;
};

struct Message
{
	Message(string msg, User user);

	friend ostream& operator<<(ostream& os, const Message& ex);
	string message;
	User user;
};

class Chat {
public:
	Chat(const string& chatUID);
	
public:
	void				setChatName(const string& chatName);
	const vector<Message>getMsgBuffer()	const;
	const bool			getIsActiveChat() const;
	void				setIsActiveChat(const bool isActive);
	const string		getChatName()	const;
	const string		getChatUID()	const;
	const uint32_t		getNotViewedMessage() const;
	void				clearNotViewedMessage();

public:
	void printChat(size_t lineSize);
	void msgBuffering(const Message msg);

public:
	virtual void addUser(User user) = 0;
	virtual const string chatName() const = 0;

	virtual const bool isUserAvailable(uint64_t ID) const = 0;

private:
	bool isActiveChat_ = false;

	size_t			lineSize_;
	string			chatName_;
	uint32_t		notViewedMessage_ = 0;
	vector<Message> msgBuffer_;
	const string	chatUID_;
};

class SoloChat :public Chat {
public:
	SoloChat(const string& chatUID, const User& user);

public:
	void addUser(User user)override {}

	const string chatName() const override;

	const bool isUserAvailable(uint64_t ID) const override;

private:
	User correspondent_;
};
class GroupChat : public Chat {
public:
	GroupChat(const string& chatUID, const User& user);
	GroupChat(const string& chatUID, const vector<User>& users);

public:
	void			addUser(User user)override;
	const string	chatName() const override;
	const bool		isUserAvailable(uint64_t ID) const override;

private:
	map<uint64_t,User>correspondents_;
};

class ChatManager {
public:
	ChatManager(uint64_t ID, const string& name)
		:you(ID, name) {
	};
public:
	auto getChats() { return chats_; }

public:
	void makeSoloChat	(shared_ptr<asio::ip::tcp::socket> socket);
	void makeGroupChat	(shared_ptr<asio::ip::tcp::socket> socket);
	void inviteUser		(shared_ptr<asio::ip::tcp::socket> socket);
	void addUser		(const string& chatUID, const uint64_t ID, shared_ptr<asio::ip::tcp::socket> socket);
	void selectChat		(shared_ptr<asio::ip::tcp::socket> socket);

	void changeUrName(const string& name);

	void exitChat();

public:
	void readHandler(string msg, shared_ptr<asio::ip::tcp::socket> socket);
	void writeHandler(string msg, shared_ptr<asio::ip::tcp::socket> socket);

private:
	void addSoloChat (const string& UID, string& msg, shared_ptr<asio::ip::tcp::socket> socket);
	void addGroupChat(const string& UID, string& msg, shared_ptr<asio::ip::tcp::socket> socket);

private:
	void printChat();

private:
	//not yet optimized
	void updateUIDbase();

private:
	//settings
	uint32_t lineSize = 30;
	
private:
	User you;

	vector<string>				  favoriteMessages_;
	shared_ptr<Chat>			  activeChat_;
	map<string, shared_ptr<Chat>> chats_;
	map<uint32_t, string>		  UIDbase_;
};