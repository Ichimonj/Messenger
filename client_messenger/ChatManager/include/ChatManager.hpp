#pragma once
#include<iostream>
#include<map>
#include<vector>
#include"asio.hpp"

using namespace std;

bool isDigit(string& str);
void safe_getline(string& in);
void stop();

//User structure: used to store message sender data
struct User {
	User(const uint64_t ID, const string name);
	User(const shared_ptr<User> user);

	uint64_t ID;
	string   name;
};

//Message structure: used to store messages
struct Message
{
	Message(string msg, shared_ptr<User> user);

	friend ostream& operator<<(ostream& os, const Message& ex);

	string message;
	shared_ptr<User> user;
};

//The chat class is used to manage chat
//it is a virtual class for SoloChat, GroupChat
class Chat {
public:
	Chat(const string& chatUID);
	
public:	//getters / setters
	void				 setChatName(const string& chatName);
	const vector<Message>getMsgBuffer()			const;
	const bool			 getIsActiveChat()		const;
	void				 setIsActiveChat(const bool isActive);
	const string		 getChatName()			const;
	const string		 getChatUID()			const;
	const uint32_t		 getNotViewedMessage()	const;
	void				 clearNotViewedMessage();

public:
	void printChat			 (size_t lineSize);
	void msgBuffering		 (const Message msg);
	virtual void receivingMsg(const string& msg, const uint64_t ID, const string& name) = 0;

public:
	virtual void addUser(shared_ptr<User> user) = 0;
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

//the SoloChat class is used to manage messages between two clients
class SoloChat :public Chat {
public:
	SoloChat(const string& chatUID, const User& user);

public:
	void addUser(shared_ptr<User> user)override {}
	const string chatName() const override;
	const bool isUserAvailable(uint64_t ID) const override;

private:
	void receivingMsg(const string& msg, const uint64_t ID, const string& name) {
		if (correspondent_->ID != ID) {
			cout << "unknown client got into your chat" << endl;
			abort();
		}
		if (correspondent_->name != name) {
			correspondent_->name = name;
		}
		msgBuffering(Message(msg, correspondent_));
	}
private:
	shared_ptr<User> correspondent_;
};

//the GroupChat class is used to manage messages between multiple clients
class GroupChat : public Chat {
public:
	GroupChat(const string& chatUID, const shared_ptr<User> user);
	GroupChat(const string& chatUID, const vector<shared_ptr<User>> users);

public:
	void			addUser(shared_ptr<User> user)override;
	const string	chatName() const override;
	const bool		isUserAvailable(uint64_t ID) const override;

private:
	void receivingMsg(const string& msg, const uint64_t ID, const string& name) {
		auto user = correspondents_.find(ID);
		if (user == correspondents_.end()) {
			cout << "unknown client got into your chat" << endl;
			abort();
		}
		if (user->second->name != name) {
			user->second->name = name;
		}
		msgBuffering(Message(msg, user->second));
	}

private:
	map<uint64_t, shared_ptr<User>>correspondents_;
};

//the ChatManager class manages all user chats
class ChatManager {
public:
	ChatManager(uint64_t ID, const string& name){
		this->you = make_shared<User>(ID, name);
	};

	//getter
public:
	auto getChats() { return chats_; }

	//chat operation
public:
	void makeSoloChat	(shared_ptr<asio::ip::tcp::socket> socket);
	void makeGroupChat	(shared_ptr<asio::ip::tcp::socket> socket);
	void inviteUser		(shared_ptr<asio::ip::tcp::socket> socket);
	void addUser		(const string& chatUID, const uint64_t ID, shared_ptr<asio::ip::tcp::socket> socket);
	void selectChat		(shared_ptr<asio::ip::tcp::socket> socket);

	void changeUrName(const string& name);

	void exitChat();

	void changeLineSize(size_t lineSize);

	//I/O handlers
public:
	void readHandler(string msg, shared_ptr<asio::ip::tcp::socket> socket);
	void writeHandler(string msg, shared_ptr<asio::ip::tcp::socket> socket);

	//add chats
private:
	void addSoloChat (const string& UID, string& msg, shared_ptr<asio::ip::tcp::socket> socket);
	void addGroupChat(const string& UID, string& msg, shared_ptr<asio::ip::tcp::socket> socket);

	//outputting messages from chats
private:
	void printChat();

private:
	//not yet optimized
	void updateUIDbase();

private:
	//settings
	uint32_t lineSize = 30;
	
private:
	shared_ptr<User> you;

	vector<string>				  favoriteMessages_;
	shared_ptr<Chat>			  activeChat_;
	map<string, shared_ptr<Chat>> chats_;
	map<uint32_t, string>		  UIDbase_;
};