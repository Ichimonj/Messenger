#pragma once
#include<iostream>
#include"AbstractAccount.hpp"
#include"map"
#include"asio.hpp"
#include"Hash.hpp"

class Chat;
class SoloChat;
class GroupChat;

class ChatManager {
public:
	ChatManager() {};
	~ChatManager() {};

	uint8_t printChat		(string&& msg, shared_ptr<asio::ip::tcp::socket> socket);
	uint8_t setChatIndex	(const string& chatUID);
	uint8_t createSoloChat	(const uint64_t ID, const shared_ptr<Account> creator);
	uint8_t createGroupChat	(const uint64_t ID, const shared_ptr<Account> creator);
	uint8_t inviteUserGroupChat	(const uint64_t ID, const string& chatUID, const shared_ptr<Account> creator);
	uint8_t addUserGroupChat	(const uint64_t ID, const string& chatUID, const shared_ptr<Account> creator);

	uint8_t addSoloChat		(const string& chatUID,const uint64_t ID);
	uint8_t addGroupChat	(const string& chatUID,const vector<uint64_t> IDs);

	void bufferingMsg	(const string& msg);

	vector<string> getBuffer();
private:
	shared_ptr<Chat>				correspondence;
	map<string, shared_ptr<Chat>>	chats;
	vector<string>					favoriteMessages;
	vector<string>					msgBuffer;
};

class Chat {
public:
	Chat() {};
	Chat(const string& chatUID) :chatUID(chatUID) {};
	
	virtual ~Chat() {};
public:
	virtual void		 printChat		 (const string&& msg, const shared_ptr<asio::ip::tcp::socket> socket) = 0;
	//chatType(0 - soloChat, 1-groupChat)
	virtual void		 generateUID	 (const uint64_t userID) = 0;
	virtual uint8_t		 addUserGroupChat(const shared_ptr<Account> user) = 0;
	virtual const size_t getCountUser() const = 0;
	string chatUID;
private:
};

class SoloChat :public Chat {
public:
	SoloChat(const shared_ptr<Account> user) { this->correspondent_ = user; }
	SoloChat(const string& chatUID, const shared_ptr<Account> user)
		:Chat(chatUID), correspondent_(user) {}

public:
	void	printChat		(const string&& msg, const shared_ptr<asio::ip::tcp::socket> socket) override;
	void	generateUID		(const uint64_t userID)override;
	uint8_t addUserGroupChat(const shared_ptr<Account> user)override;
	const size_t getCountUser()const override { return 1; }

private:
	shared_ptr<Account> correspondent_;
};

class GroupChat : public Chat {
public:
	GroupChat(const shared_ptr<Account> user) { this->correspondents_.push_back(user); }
	GroupChat(const string& chatUID, const vector<shared_ptr<Account>> users)
		:Chat(chatUID), correspondents_(users) {}

public:
	void printChat			(const string&& msg, const shared_ptr<asio::ip::tcp::socket> socket) override;
	void generateUID		(const uint64_t userID) override;
	uint8_t addUserGroupChat(const shared_ptr<Account> user)override;
	const size_t getCountUser()const override;
	const shared_ptr<Account> getCorrespondent(const size_t num);
private:
	vector<shared_ptr<Account>> correspondents_; 
};