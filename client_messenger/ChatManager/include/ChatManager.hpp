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
	User() {}
	User(const uint64_t ID, const string name);
	User(const shared_ptr<User> user);

	void serialization(ofstream& file);
	void deserialization(ifstream& file);

	uint64_t ID;
	string   name;
};

//Message structure: used to store messages
struct Message
{
	Message() {}
	Message(string msg, shared_ptr<User> user);

	friend ostream& operator<<(ostream& os, const Message& ex);

	void serialization(ofstream& file);
	void deserialization(ifstream& file);

	string message;
	shared_ptr<User> user;
};

//The chat class is used to manage chat
//it is a virtual class for SoloChat, GroupChat
class Chat {
public:
	Chat() {};
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
public:
	virtual void serialization	(ofstream& file) = 0;
	void chatSerialization		(ofstream& file);
	void chatDeserialization	(ifstream& file, shared_ptr<User> you, shared_ptr<User> correspondent);
	void chatDeserialization	(ifstream& file, shared_ptr<User> you, map<uint64_t, shared_ptr<User>> correspondents);

private:
	bool isActiveChat_ = false;

	size_t			lineSize_;
	string			chatName_;
	uint32_t		notViewedMessage_ = 0;
	vector<Message> msgBuffer_;
	string			chatUID_;
};

//the SoloChat class is used to manage messages between two clients
class SoloChat :public Chat {
public:
	SoloChat() {}
	SoloChat(const string& chatUID, const User& user);

public:
	void addUser(shared_ptr<User> user)override {}
	const string chatName() const override;
	const bool isUserAvailable(uint64_t ID) const override;

public:
	/*
	records chat type
	0-solo chat
	1-group chat
   */
	void serialization	(ofstream& file) override;
	void deserialization(ifstream& file, shared_ptr<User> you);

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
	GroupChat() {}
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
public:
	/*
	records chat type
	0-solo chat
	1-group chat
   */
	void serialization	(ofstream& file) override;
	void deserialization(ifstream& file, shared_ptr<User> you);

private:
	map<uint64_t, shared_ptr<User>>correspondents_;
};

//the ChatManager class manages all user chats
class ChatManager {
public:
	ChatManager();
	ChatManager(uint64_t ID, const string& name);
	~ChatManager();
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

	void changeLineSize			(size_t lineSize);
	void changeSaveChatManager	(bool saveChatManager);

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

public:
	void serialization	(ofstream& file);
	void deserialization(ifstream& file);

private:
	//settings
	uint32_t lineSize		 = 30;
	bool	 saveChatManager = true;

	shared_ptr<User> you;

	vector<string>				  favoriteMessages_;
	shared_ptr<Chat>			  activeChat_;
	map<string, shared_ptr<Chat>> chats_;
	map<uint32_t, string>		  UIDbase_;
};