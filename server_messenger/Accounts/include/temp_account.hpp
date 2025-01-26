#pragma once
#include "abstract_account.hpp"
#include "account_factory.hpp"
#include "account_base.hpp"
#include "chat_manager.hpp"

// temporary account
class TempAccount : public Account
{
public:
	TempAccount() = delete;
	TempAccount(const TempAccount &) = delete;

	TempAccount(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string& userName, const string& password);

	~TempAccount();
	
public:
	void reading() 	override;
	void print() 	const override;
	void info() 	const override;
	void bufferingMsg(const string& msg)	override;
	void outBuffer()						override;

	const string getAccountData() const override;

private:
	void read_handler(const char* buf, const size_t length);
	bool checkError(const error_code& ec);

	void change_name		(error_code& ec);
	void change_password	(error_code& ec);
	void exit_account		(error_code& ec);
	void delete_account		(error_code& ec);

	void create_solo_chat	(error_code& ec);
	void add_solo_chat		(error_code& ec);
	void create_group_chat	(error_code& ec);
	void add_group_chat		(error_code& ec);
	void invite_user		(error_code& ec);
	void add_user_group_chat(error_code& ec);
	void select_chat		(error_code& ec);

public:
	friend ostream &operator<<(ostream &os, const TempAccount &ex);

private:
	ChatManager chat_manager;
};