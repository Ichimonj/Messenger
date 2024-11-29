#pragma once
#include<iostream>
#include"AbstractAccount.hpp"
#include"map"
class ChatManager {
public:
	ChatManager();
	int addSoloChat	(uint64_t ID,shared_ptr<asio::ip::tcp::socket> socket);
	int printChat	(string&& msg);
	int setChatIndex(uint64_t index);

	void bufferingMsg(string& msg);
	vector<string> getBuffer();

private:
	shared_ptr<Account> correspondent;
	map<uint64_t, shared_ptr<Account>> soloChat;

	vector<string> bufferMsg;
};