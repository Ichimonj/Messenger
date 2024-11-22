#pragma once
#include<iostream>
#include"AbstractAccount.hpp"

class ChatManager {
public:
	ChatManager();
	int addSoloChat(uint64_t ID);
	int printChat(string&& msg);
	int setChatIndex(uint32_t index);

private:
	uint32_t chatIndex{ 0 };
	vector<shared_ptr<Account>> soloChat;
};