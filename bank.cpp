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
        std::cout << "Creating ";
        if (credit) {
            std::cout << "credit ";
        } else {
            std::cout << "debit ";
        }
        std::cout << "account for " << name_ << std::endl;
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
        std::cout << "Hello, I am Picsou your bank assistant" << std::endl;
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
        std::istringstream iss(line);

        if (iss >> action) {
            if (validActions.find(action) != std::string::npos) {
                if (iss >> name) {
                    try {
                        // Check if the account exists
                        Account account = bank.get(name);
                        // Let's proceed with the action
                        if (action == 'c'){
                            std::cerr << "Error account for " << name <<" already existing " << endl;
                        } else if (action == 'r') {
                            /**
                             * TO DO REMOVE ACCOUNT FROM VECTOR
                             **/
                            std::cerr << "Thanks <" << name <<"> for banking with us. Account deleted " << endl;
                        } else {
                            /**
                             * Only action remaining is t
                             **/
                            if (iss >> amount){
                                bool isCredit = account.getCredit();
                                float newBalance = account.getBalance() + amount;
                                if (isCredit || (newBalance >= 0)) {
                                    account.setBalance(newBalance);
                                    std::cerr << "New balance for " << name << "'s account is: " << newBalance << endl;
                                } else {
                                    std::cerr << "Operation denied. Not enough balance on your account " << name << endl;
                                }
                            } else {
                                std::cerr << "<ERROR> Not able to retrieve amount from line <" << line << ">" << endl;
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
                                    std::cerr <<  "<ERROR> Credit value unknown in line <" << line << ">" << endl;
                                }
                            } else {
                                std::cerr << "<ERROR> Credit value unknown in line <" << line << ">" << endl;
                            }
                        } else {
                            std::cerr << "Account not existing for " << name << endl;
                        }
                    }
                } else {
                    std::cerr << "<ERROR> Name missing for action " << action <<" in line <" << line << ">" << endl;
                }
            } else {
                std::cerr << "<ERROR> Unknown action " << action << " in line <" << line << ">" << std::endl;
            }
        } else {
            std::cerr << "<ERROR> Invalid line <" << line << ">" << std::endl;
        }
    }
}