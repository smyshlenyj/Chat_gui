#pragma once

struct User
{
private:
	std::string login, password, userName;

public:
	// struct constructors for different purposes:
	User(std::string const&, std::string const&, std::string const&);
	User(std::string const&, std::string const&);
	User(std::string const& = "_all");
	//User() = default;

	std::string getLogin()	  const noexcept;
	std::string getPassword() const noexcept;
	std::string getUserName() const noexcept;

	void setLogin(std::string const&);
	void setPassword(std::string const&);
	void setUserName(std::string const&);
};

std::string hashPassword(const std::string&);