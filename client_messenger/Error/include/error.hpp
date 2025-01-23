#pragma once
#include <iostream>
using namespace std;

namespace funct_return {
	//listing the results of functions
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
		//create a message based on the enumeration
		static string message(message msg);
	};
}
