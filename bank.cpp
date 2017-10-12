#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <ctype.h>
#include <cassert>
#include <algorithm>
#include <exception>

using namespace std;

class Account {
private:
	std::string name_;
	bool credit_;
	float balance_;
public:
    std::string getName(){
    	return name_;
    }

    bool getCredit(){
    	return credit_;
    }

    float getBalance(){
    	return balance_;
    }

    void setBalance(float newBalance){
    	balance_ = newBalance;
    }

	Account(const std::string& name, const bool credit) {
		name_ = name;
		credit_ = credit;
		balance_ = 0.0;
		std::cout << "Creating account " << name_ << "[" << credit_ << "]!" << std::endl;
	};
};

class AccountException: public exception
{
  virtual const char* what() const throw()
  {
    return "No Account Exception";
  }
} accountException;

class Bank {
private:
	vector<Account> accounts;
public:
	Bank(){
		std::cout << "Creating Bank " << std::endl;
	}

	Account get(std::string name) {

		auto it = find_if(accounts.begin(), accounts.end(), [&name](Account& obj) {return obj.getName() == name;});

		if (it != accounts.end()) {
			return *it;
		} else {
			throw accountException;
		}
	}

	void set(const Account account) {
		accounts.push_back(account);
	}
};

int main(){
	std::string line;
	std::string name;
	float amount;
	char action;
	char credit;
	const std::string validActions = "crt";
	const std::string validAcknowledgments = "yn";
	Bank bank;

	while (std::getline(std::cin, line)) {
		std::cout << line << endl;
		std::istringstream iss(line);

		if (iss >> action) {
			if (validActions.find(action) != std::string::npos) {
				if (iss >> name) {
					try {
						// Check if the account exists
						Account account = bank.get(name);
						// Let's proceed with the action
						if (action == 'c'){
							std::cerr << "Error account <" << name <<"> already existing " << endl;
						} else if (action == 'r') {
							/**
							 * TO DO REMOVE ACCOUNT FROM VECTOR
							 **/
							std::cerr << "Thanks <" << name <<"> for making business with us. Account deleted " << endl;
						} else {
							/**
							 * Only action remaining is t
							 **/
							if (iss >> amount){
								bool isCredit = account.getCredit();
								float newBalance = account.getBalance() + amount;
								if (isCredit || (newBalance >= 0)) {
									account.setBalance(newBalance);
								    std::cerr << "New balance for account <" << name << "> is: " << newBalance << endl;
								} else {
									std::cerr << "Not enough balance on your account <" << name << ">" << endl;
								}
							} else {
								std::cerr << "Not able to retrieve anount from: " << line << endl;
							}
						}
					} catch(AccountException accountException){
						// The account does not exist
						if (action == 'c'){
							if (iss >> credit) {
								if (validAcknowledgments.find(credit) != std::string::npos) {
									Account *account = new Account(name, credit == 'y');
									bank.set(*account);
								} else {
									std::cerr << "Credit value unknown <" << credit <<"> in line: " << line << endl;
								}
							} else {
								std::cerr << "Credit value unknown <" << credit <<"> in line: " << line << endl;
							}
						} else {
							std::cerr << "Account <" << name <<"> not exiting in line: " << line << endl;
						}
					}
				} else {
					std::cerr << "Name missing for action <" << action <<"> in line: " << line << endl;
				}
			} else {
				std::cerr << "Unknown action <" << action << "> in line: " << line << std::endl;
			}
		} else {
			std::cerr << "Invalid line: " << line << std::endl;
		}
	}
}