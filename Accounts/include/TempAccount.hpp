#pragma once
#include "AbstractAccount.hpp"
// temporary account
class TempAccount : public Account
{
public:
	TempAccount() = delete;
	TempAccount(const TempAccount &) = delete;

	TempAccount(shared_ptr<asio::ip::tcp::socket> socket, const uint64_t ID, const string &userName);
	TempAccount(TempAccount &&other) noexcept;

	~TempAccount();

public:
	void reading() 	override;
	void print() 	const override;
	void info() 	const override;

public:
	friend ostream &operator<<(ostream &os, const TempAccount &ex);
};