#pragma once
#include <iostream>
#include "chat_manager.hpp"
using namespace std;

//aligned console output
void print(const char* first, const char* second, size_t FILL);
void print(const string& first, const string& second, size_t FILL);

class Console {
public:
	//first menu
	static void help();
	//login commands
	static void loginHelp();
	//types of accounts that can be registered
	static void accountType();
	//all kinds of account operations
	static void accountOperation();
	//chats output
	static void chats(map<string, shared_ptr<Chat>> chats);
	//commands that can be done with chat
	static void chatsHelp(map<string, shared_ptr<Chat>> chats);
};