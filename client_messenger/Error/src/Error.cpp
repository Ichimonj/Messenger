#include"Error.hpp"
#include"Lenguage.hpp"
string funct_return::Error::message(funct_return::message msg)
{
#if (CURRENT_LANGUAGE == LANGUAGE_RU)
	switch (msg)
	{
	case funct_return::successful:                return "Успешно";
	case funct_return::IOerror:                   return "Ошибка ввода-вывода";
	case funct_return::wrongPassword:             return "Неверный пароль";
	case funct_return::wrongPhoneNumber:          return "Неверный номер телефона";
	case funct_return::msgInDeleteACcount:        return "Сообщение при удалении аккаунта";
	case funct_return::noUser:                    return "Пользователь не найден";
	case funct_return::noChat:                    return "Чат не найден";
	case funct_return::addUserToSoloChat:         return "Добавление пользователя в одиночный чат";
	case funct_return::alreadyUser:               return "Пользователь уже существует";
	case funct_return::alreadyChat:               return "Чат уже существует";
	case funct_return::wrongUID:                  return "Неверный UID";
	default:                                      return "";
	}
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	switch (msg)
	{
	case funct_return::successful:				return "Successful";
	case funct_return::IOerror:					return "Input/Output error";
	case funct_return::wrongPassword:			return "Wrong password";
	case funct_return::wrongPhoneNumber:		return "Wrong phone number";
	case funct_return::msgInDeleteACcount:		return "Message sent to a deleted account";
	case funct_return::noUser:                  return "No such user exists";
	case funct_return::noChat:                  return "No such chat exists";
	case funct_return::addUserToSoloChat:		return "Attempt to add user to a solo chat";
	case funct_return::alreadyUser:             return "User already added";
	case funct_return::alreadyChat:             return "Chat already exists";
	case funct_return::wrongUID:                return "Wrong chat UID";
	default:                                    return "";
	}
#endif  
}
