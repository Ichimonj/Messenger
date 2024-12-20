#pragma once
#include "AbstractAccount.hpp"
#include "AccountFactory.hpp"
#include "AccountBase.hpp"
#include "ChatManager.hpp"

// Account for verified clients
class UserAccount : public Account
{
public:
	UserAccount() = delete;
	UserAccount(const UserAccount &) = delete;

	UserAccount(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string& userName, const string& password, const string& emale, const PhoneNumber& phoneNumber);
	UserAccount(UserAccount &&other) noexcept;

	~UserAccount();

public:
	void reading() 	override;
	void print() 	const override;
	void info() 	const override;
	void bufferingMsg(const string& msg)override;
	void outBuffer()					override;

	const string getAccountData() const	override;

private:
	void read_handler(const char* buf, const size_t length);
	bool checkError(const error_code& ec);

public:
	friend ostream &operator<<(ostream &os, const UserAccount &ex);

private:
	string 		emale_;
	PhoneNumber phoneNumber_;

	ChatManager chatManager;
};