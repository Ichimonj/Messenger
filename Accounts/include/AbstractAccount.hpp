#pragma once
#include "PhoneNumber.hpp"
#include "asio.hpp"
using namespace std;

enum status
{
	offline,
	online,
	dont_disturb,
	invisible
};
// virtual classroom for all users
class Account
{
public:
	Account() 					= delete;
	Account(const Account &) 	= delete;

	Account(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string& userName, const string& password);
	Account(Account &&other) noexcept;

	virtual ~Account();

public:
	const uint64_t 	getId() 		const;
	const string 	getUserName() 	const;
	const status 	getStatus() 	const;
	const string	getPassword()	const;

	virtual void reading() 	= 0;
	virtual void print() 	const;
	virtual void info() 	const;

public:
	friend ostream &operator<<(ostream &os, const Account &ex);
	bool operator<(const Account &ex);

protected:
	shared_ptr<asio::ip::tcp::socket> socket_;
	char buf[1024];

private:
	const uint64_t 	ID_;
	string 			userName_ = "NULL";
	string			password_ = "NULL";
	status 			status_ = online;
};