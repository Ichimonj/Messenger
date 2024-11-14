#pragma once
#include "AbstractAccount.hpp"

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

public:
	friend ostream &operator<<(ostream &os, const UserAccount &ex);

private:
	string 		password_;
	string 		emale_;
	PhoneNumber phoneNumber_;
};