#pragma once
#include "AbstractAccount.hpp"
#include "AccountFactory.hpp"
#include "AccountBase.hpp"
#include "ChatManager.hpp"

// temporary account
class TempAccount : public Account
{
public:
	TempAccount() = delete;
	TempAccount(const TempAccount &) = delete;

	TempAccount(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string& userName, const string& password);
	TempAccount(TempAccount &&other) noexcept;

	~TempAccount();
	
public:
	void reading() 	override;
	void print() 	const override;
	void info() 	const override;
	void bufferingMsg(string& msg)	override;
	void outBuffer()				override;

private:
	void read_handler(const char* buf, const size_t length);

public:
	friend ostream &operator<<(ostream &os, const TempAccount &ex);

private:
	ChatManager chatManager;
};