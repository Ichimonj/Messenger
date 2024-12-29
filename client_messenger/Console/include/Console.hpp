#pragma once
#include <iostream>
#include "ChatManager.hpp"
using namespace std;
void print(const char* first, const char* second, size_t FILL);
void print(const string& first, const string& second, size_t FILL);
class Console {
public:
	static void help();
	static void loginHelp();
	static void accountType();
	static void accountOperation();
	static void chats(map<string, shared_ptr<Chat>> chats);
	static void chatsHelp(map<string, shared_ptr<Chat>> chats);
};