#pragma once
#include<iostream>
#include"AbstractAccount.hpp"
#include"map"
class ChatManager {
public:
	ChatManager();
	int addSoloChat(uint64_t ID,shared_ptr<asio::ip::tcp::socket> socket);
	int printChat(string&& msg);
	int setChatIndex(uint64_t index);

private:
	uint32_t chatIndex{ 0 };
	shared_ptr<Account> ñorrespondent;
	map<uint64_t, shared_ptr<Account>> soloChat;
};