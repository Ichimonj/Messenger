#pragma once
#include<iostream>
using namespace std;
namespace funct_return {
	enum message
	{
		successful,
		IOerror,
		wrongPassword,
		wrongPhoneNumber,
		msgInDeleteACcount,
		noUser,
		noChat,
		addUserToSoloChat,
		alreadyUser,
		alreadyChat,
		wrongUID
	};
	struct Error {
		static string message(message msg);
	};
}
