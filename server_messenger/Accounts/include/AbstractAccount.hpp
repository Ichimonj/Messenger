#pragma once
#include "PhoneNumber.hpp"
#include "asio.hpp"
using namespace std;

//acanthus statuses
enum status
{
	offline,
	online,
	dont_disturb,
	invisible,

	deleted
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
	const string	getPassword()	const;
	const status 	getStatus() 	const;

	virtual const string getAccountData() const;
	const shared_ptr<asio::ip::tcp::socket>	getSocket()	const;

	virtual void reading() 	= 0;
	virtual void print() 	const;
	virtual void info() 	const;
	virtual void bufferingMsg(const string& msg) = 0;
	virtual void outBuffer() = 0;


	uint8_t login(shared_ptr<asio::ip::tcp::socket> socket);
public:
	friend ostream &operator<<(ostream &os, const Account &ex);
	bool operator<(const Account &ex);

protected:
	char buf[1024];

	shared_ptr<asio::ip::tcp::socket> socket_;
	const uint64_t 	ID_;
	string 			userName_ = "NULL";
	string			password_ = "NULL";
	status 			status_	  = online;
};