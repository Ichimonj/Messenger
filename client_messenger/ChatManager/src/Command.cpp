#include"Command.hpp"
std::vector<std::string> client_command::Command::createCommand(client_command::commands command)
{
    switch (command)
    {
    case client_command::change_name:
        return vector<string>{"__chAccount", "__chName"};
    case client_command::change_password:
        return vector<string>{"__chAccount", "__chPaswd"};
    case client_command::change_email:
        return vector<string>{"__chAccount", "__chEmail"};
    case client_command::change_phone_number:
        return vector<string>{"__chAccount", "__chPhoneNumber"};
    case client_command::exit_account:
        return vector<string>{"__chAccount", "__exit", "__Y"};
    case client_command::delete_account:
        return vector<string>{"__chAccount", "__delete", "__Y"};
    case client_command::create_solo_chat:
        return vector<string>{"__chat", "__createSoloChat"};
    case client_command::add_solo_chat:
        return vector<string>{"__chat", "__addSoloChat"};
    case client_command::create_group_chat:
        return vector<string>{"__chat", "__createGroupChat"};
    case client_command::add_group_chat:
        return vector<string>{"__chat", "__addGroupChat"};
    case client_command::invite_user:
        return vector<string>{"__chat", "__inviteUserGroupChat"};
    case client_command::add_user_group_chat:
        return vector<string>{"__chat", "__addUserGroupChat"};
    case client_command::select_chat:
        return vector<string>{"__chat", "__selectChat"};
    default:
        return vector<string>{""};
    }
}
