#pragma once
#include<iostream>
#include<vector>
using namespace std;
namespace client_command {
	//enumeration of commands
	enum commands {
		change_name,
		change_password,
		change_email,
		change_phone_number,
		exit_account,
		delete_account,

		create_solo_chat,
		add_solo_chat,
		create_group_chat,
		add_group_chat,
		invite_user,
		add_user_group_chat,
		select_chat
	};
	struct Command
	{
		//creating a command based on an enumeration
		static std::vector<std::string> createCommand(client_command::commands command);
	};
}