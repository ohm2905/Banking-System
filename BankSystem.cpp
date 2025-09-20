#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h> 
using namespace std;


class Account {
protected:
    string AccountNo, Password;
    int Balance;

public:
    Account(string id = "", string pw = "", int bal = 0)
        : AccountNo(id), Password(pw), Balance(bal) {}

    void setAccountNo(string id) { AccountNo = id; }
    void setPassword(string pw) { Password = pw; }
    void setBalance(int balance) { Balance = balance; }

    string getAccountNo() { return AccountNo; }
    string getPassword() { return Password; }
    int getBalance() { return Balance; }

    virtual void deposit(int amount) = 0;
    virtual void withdraw(int amount) = 0;

    virtual void saveToFile() {
        ofstream outfile("Account.txt", ios::app); 
        if (!outfile) {
            cout << "\tError: File Can't Open!" << endl;
            return;
        }
        outfile << AccountNo << "|" << Password << "|" << Balance << endl;
        outfile.close();
    }

    virtual ~Account() {}
};


class SavingsAccount : public Account {
public:
    SavingsAccount(string id, string pw, int bal = 0)
        : Account(id, pw, bal) {}

    void deposit(int amount) override {
        Balance += amount;
        cout << "\t[Savings] Deposited: " << amount << " | New Balance: " << Balance << endl;
    }

    void withdraw(int amount) override {
        if (amount <= Balance) {
            Balance -= amount;
            cout << "\t[Savings] Withdrawn: " << amount << " | Remaining Balance: " << Balance << endl;
        } else {
            cout << "\tLow Balance in Savings Account!" << endl;
        }
    }
};


class CurrentAccount : public Account {
public:
    CurrentAccount(string id, string pw, int bal = 0)
        : Account(id, pw, bal) {}

    void deposit(int amount) override {
        Balance += amount;
        cout << "\t[Current] Deposited: " << amount << " | New Balance: " << Balance << endl;
    }

    void withdraw(int amount) override {
        if (Balance - amount >= -500) {
            Balance -= amount;
            cout << "\t[Current] Withdrawn: " << amount << " | Remaining Balance: " << Balance << endl;
        } else {
            cout << "\tOverdraft Limit Reached!" << endl;
        }
    }
};


int main() {
    bool exit = false;

    while (!exit) {
        system("clear"); 
        int val;
        cout << "\tWelcome To Bank Account Management System (OOP)" << endl;
        cout << "\t***********************************************" << endl;
        cout << "\t1. Open Savings Account" << endl;
        cout << "\t2. Open Current Account" << endl;
        cout << "\t3. Deposit Money" << endl;
        cout << "\t4. Withdraw Money" << endl;
        cout << "\t5. Exit" << endl;
        cout << "\tEnter Your Choice: ";
        cin >> val;

        if (val == 1 || val == 2) {
            string id, pw;
            cout << "\tEnter Account No: ";
            cin >> id;
            cout << "\tEnter Password: ";
            cin >> pw;

            Account* acc;
            if (val == 1) {
                acc = new SavingsAccount(id, pw);
                cout << "\tSavings Account Created!" << endl;
            } else {
                acc = new CurrentAccount(id, pw);
                cout << "\tCurrent Account Created!" << endl;
            }

            acc->saveToFile();
            delete acc;
            sleep(3);
        }

        else if (val == 3 || val == 4) {
            string id, pw = "";
            cout << "\tEnter Account No: ";
            cin >> id;

            
            if (val == 4) {
                cout << "\tEnter Password: ";
                cin >> pw;
            }

            ifstream infile("Account.txt");
            if (!infile) {
                cout << "\tError: File Can't Open!" << endl;
                sleep(3);
                continue;
            }

            string line;
            bool found = false;
            while (getline(infile, line)) {
                stringstream ss(line);
                string userID, userPW;
                int userBalance;

                getline(ss, userID, '|');
                getline(ss, userPW, '|');
                ss >> userBalance;

               
                bool valid = (val == 3 && id == userID) || (val == 4 && id == userID && pw == userPW);

                if (valid) {
                    found = true;

                    cout << "\tIs this Savings(1) or Current(2) account? ";
                    int type;
                    cin >> type;

                    Account* acc;
                    if (type == 1)
                        acc = new SavingsAccount(userID, userPW, userBalance);
                    else
                        acc = new CurrentAccount(userID, userPW, userBalance);

                    int amount;
                    cout << "\tEnter Amount: ";
                    cin >> amount;

                    if (val == 3)
                        acc->deposit(amount);
                    else
                        acc->withdraw(amount);

                    
                    ofstream temp("AccountTemp.txt");
                    ifstream reread("Account.txt");
                    string l;
                    while (getline(reread, l)) {
                        stringstream ss2(l);
                        string uid, upw;
                        int ubal;
                        getline(ss2, uid, '|');
                        getline(ss2, upw, '|');
                        ss2 >> ubal;

                        if (uid == userID) {
                            temp << acc->getAccountNo() << "|" << acc->getPassword() << "|" << acc->getBalance() << endl;
                        } else {
                            temp << l << endl;
                        }
                    }
                    reread.close();
                    temp.close();
                    remove("Account.txt");
                    rename("AccountTemp.txt", "Account.txt");

                    delete acc;
                    break;
                }
            }
            if (!found)
                cout << "\tInvalid Account No"
                     << (val == 4 ? " or Password!" : "!") << endl;

            infile.close();
            sleep(3);
        }

        else if (val == 5) {
            system("clear");
            exit = true;
            cout << "\tGood Luck!" << endl;
            sleep(2);
        }
    }
}
