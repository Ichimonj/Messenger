#include "Console.hpp"
#include <iomanip>
#include "Lenguage.hpp"
#include "string"

void print(const char* first, const char* second, size_t FILL)
{
	cout << setw(FILL) << left << setfill(' ') << first << second;
}
void print(const string& first, const string& second, size_t FILL)
{
	cout << setw(FILL) << left << setfill(' ') << first << second;
}

void Console::help()
{
	size_t FILL = 18;
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	system("cls");
	print("[--account | -a]", "Операции с вашим аккаунтом\n", FILL);
	print("[--chats   | -c]", "Операции с чатами.\n", FILL);
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	system("cls");
	print("[--account | -a]", "Operations with your account\n", FILL);
	print("[--chats   | -c]", "Operations with chats.\n", FILL);
#endif 
}

void Console::loginHelp()
{
	system("cls");
	cout << "[--registry | -r]\n";
	cout << "[--login    | -l]\n";
}


void Console::accountType()
{
	size_t FILL = 20;
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	system("cls");
	print("[--temp    | -t]", "Временный аккаунт, он будет удален после выхода\n", FILL);
	print("[--no temp | -nt]", "Регулярный аккаунт, для регистрации потребуется больше данных, но он не будет удален после выхода\n", FILL);
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	system("cls");
	print("[--temp    | -t]", "Temporary account, it will be deleted after logout\n", FILL);
	print("[--no temp | -nt]", "Regular account, more data will be required for registration, but it will not be deleted after logout\n", FILL);
#endif 
}

void Console::accountOperation()
{
	size_t FILL = 30;
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	system("cls");
	print("[--ch name   | -chn]", "Изменить имя пользователя\n", FILL);
	print("[--ch passwd | -chp]", "Изменить пароль\n", FILL);
	print("[--ch email  | -che]", "Изменить почту\n", FILL);
	print("[--ch phone number | -chpn]", "Изменить номер телефона\n", FILL);
	print("[--exit]", "Выйти\n", FILL);
	print("[--delete]", "Удалить аккаунт\n", FILL);
	print("[--back]", "Вернутся назад\n", FILL);
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	system("cls");
	print("[--ch name   | -chn]", "Change username\n", FILL);
	print("[--ch passwd | -chp]", "Change password\n", FILL);
	print("[--ch email  | -che]", "Change email\n", FILL);
	print("[--ch phone number | -chpn]", "Change phone number\n", FILL);
	print("[--exit]", "Exit", FILL);
	print("[--delete]", "Delete account", FILL);
	print("[--back]", "Go back\n", FILL);
#endif 
}

void Console::chats(map<string, shared_ptr<Chat>> chats)
{
	size_t FILL = 30;
	size_t i = 1;
	string first;
	string second;
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	cout << "0-избранные сообщения\n";
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	cout << "0-favorite messages\n";
#endif 
	for (auto& chat : chats) {
		first.clear();
		second.clear();

		first += to_string(i);
		first += string("-") += chat.second->chatName();
		if (first.size() > FILL) {
			first.resize(FILL - 4);
			first += "...";
		}
		second += string("new msg-") += to_string(chat.second->getNotViewedMessage());
		print(first, second, FILL);
		cout << '\n';
		i++;
	}
}

void Console::chatsHelp(map<string, shared_ptr<Chat>> chats)
{
	size_t FILL = 35;
	system("cls");
#if(CURRENT_LANGUAGE  == LANGUAGE_RU)
	print("[--make Schat | -msc]", "Добавить персональный чат\n", FILL);
	print("[--make Gchat | -mgc]", "Добавить групповой чат\n", FILL);
	print("[--invite user | -iu]", "добавить пользователя в групповой чат\n", FILL);
	print("[--select | -s]", "Выбрать чат из ниже перечисленных\n", FILL);
	print("[--back]", "Вернутся назад\n", FILL);
#elif(CURRENT_LANGUAGE == LANGUAGE_EN)
	print("[--make Schat | -msc]", "Add personal chat\n", FILL);
	print("[--make Gchat | -mgc]", "Add group chat\n", FILL);
	print("[--invite user | -iu]", "add user to group chat\n", FILL);
	print("[--select | -s]", "Select a chat from the list below\n", FILL);
	print("[--back]", "Go back\n", FILL);
#endif 
	Console::chats(chats);
}
