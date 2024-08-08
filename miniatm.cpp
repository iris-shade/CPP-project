#include <fstream>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

class ATM {
private:
    string username;
    int password;
    int balance;

public:
    string getUsername() const { return username; }
    int getPassword() const { return password; }

    void getData() {
        cout << "\n------------------------\n";
        cout << "  Enter User Details  ";
        cout << "\n------------------------\n";

        cout << "Enter username: ";
        getline(cin >> ws, username);  // ws to ignore leading whitespace
        cout << "Enter 4-digit password: ";
        while (!(cin >> password) || password < 1000 || password > 9999) {
            cout << "Invalid input! Please enter a 4-digit password: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Enter initial balance: ";
        while (!(cin >> balance) || balance < 0) {
            cout << "Invalid input! Please enter a positive balance: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    void showData() const {
        cout << "Username: " << username
             << ", Password: " << password
             << ", Balance: " << balance << endl;
    }

    int addUser();
    int viewAllUsers();
    int deleteUser(const string& uname);
    void updateUserAsDeposit(const string& uname);
    void updateUserAsWithdraw(const string& uname);
    int searchSpecificUser(const string& uname, int pass);
    int searchAllUsersToDisplay(const string& uname);
};

void clearScreen() {
    cout << "\033[2J\033[1;1H"; // ANSI escape codes for clearing the screen
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printHeader(const string& title) {
    clearScreen();
    cout << "========================" << endl;
    cout << "       " << title << "       " << endl;
    cout << "========================" << endl;
}

void atmUser() {
    ATM atm;
    string uname;
    int pass, ch, ch1, ch2;
    bool exit = false;

    while (!exit) {
        printHeader("Welcome to Mini ATM");
        cout << "1. Admin Login\n";
        cout << "2. User Login\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        cin >> ch;

        switch (ch) {
        case 1: {  // Admin Login
            printHeader("Admin Login");
            cout << "Enter password: ";
            cin >> pass;
            if (pass == 1234) {
                bool adminExit = false;
                while (!adminExit) {
                    printHeader("Admin Menu");
                    cout << "1. Add User\n";
                    cout << "2. Delete User\n";
                    cout << "3. View All Users\n";
                    cout << "4. Exit\n";
                    cout << "Choose an option: ";
                    cin >> ch1;

                    switch (ch1) {
                    case 1:
                        atm.addUser();
                        break;
                    case 2:
                        cout << "Enter the Username to be deleted: ";
                        getline(cin >> ws, uname);
                        atm.deleteUser(uname);
                        break;
                    case 3:
                        atm.viewAllUsers();
                        break;
                    case 4:
                        adminExit = true;
                        break;
                    default:
                        cout << "Invalid choice! Please try again." << endl;
                        break;
                    }
                    pauseScreen();
                }
            } else {
                cout << "Incorrect password! Please try again." << endl;
                pauseScreen();
            }
            break;
        }
        case 2: {  // User Login
            printHeader("User Login");
            cout << "Enter username: ";
            getline(cin >> ws, uname);
            cout << "Enter password: ";
            cin >> pass;

            if (atm.searchSpecificUser(uname, pass)) {
                bool userExit = false;
                while (!userExit) {
                    printHeader("User Menu");
                    cout << "1. Deposit\n";
                    cout << "2. Withdraw\n";
                    cout << "3. View Account\n";
                    cout << "4. Exit\n";
                    cout << "Choose an option: ";
                    cin >> ch2;

                    switch (ch2) {
                    case 1:
                        atm.updateUserAsDeposit(uname);
                        break;
                    case 2:
                        atm.updateUserAsWithdraw(uname);
                        break;
                    case 3:
                        atm.searchAllUsersToDisplay(uname);
                        break;
                    case 4:
                        userExit = true;
                        break;
                    default:
                        cout << "Invalid choice! Please try again." << endl;
                        break;
                    }
                    pauseScreen();
                }
            } else {
                cout << "No account found with username " << uname << endl;
                pauseScreen();
            }
            break;
        }
        case 3:  // Exit
            exit = true;
            cout << "\nThank you for banking with Mini ATM!" << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            break;
        }
    }
}

int ATM::addUser() {
    ATM a;
    ofstream file;

    printHeader("Add User");

    file.open("users.txt", ios::out | ios::app);
    if (!file) {
        cout << "Error in creating file.." << endl;
        return 0;
    }

    a.getData();
    file.write(reinterpret_cast<char*>(&a), sizeof(a));
    file.close();

    cout << "User added successfully!" << endl;
    return 0;
}

int ATM::viewAllUsers() {
    ATM a;
    ifstream file;

    printHeader("All Users");

    file.open("users.txt", ios::in);
    if (!file) {
        cout << "Error in opening file.." << endl;
        return 0;
    }

    file.read(reinterpret_cast<char*>(&a), sizeof(a));
    while (!file.eof()) {
        a.showData();
        file.read(reinterpret_cast<char*>(&a), sizeof(a));
    }

    file.close();
    return 0;
}

int ATM::deleteUser(const string& uname) {
    ATM a;
    fstream original, temp;
    bool found = false;

    printHeader("Delete User");

    original.open("users.txt", ios::in);
    if (!original) {
        cout << "\nFile not found" << endl;
        return 0;
    }

    temp.open("temp.txt", ios::out | ios::app);
    original.read(reinterpret_cast<char*>(&a), sizeof(a));

    while (!original.eof()) {
        if (a.getUsername() == uname) {
            cout << "Data found and deleted: " << a.getUsername() << endl;
            found = true;
        } else {
            temp.write(reinterpret_cast<char*>(&a), sizeof(a));
        }
        original.read(reinterpret_cast<char*>(&a), sizeof(a));
    }

    original.close();
    temp.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (!found) {
        cout << "User not found." << endl;
    }

    return 0;
}

void ATM::updateUserAsDeposit(const string& uname) {
    ATM a;
    fstream file, temp;
    int amount;
    bool found = false;

    printHeader("Deposit");

    file.open("users.txt", ios::in | ios::out | ios::ate);
    temp.open("temp.txt", ios::out | ios::app);
    file.seekg(0);
    file.read(reinterpret_cast<char*>(&a), sizeof(a));

    while (!file.eof()) {
        if (a.getUsername() == uname) {
            cout << "Enter amount to deposit: ";
            cin >> amount;
            a.balance += amount;
            cout << "Balance updated: " << a.balance << endl;
            found = true;
        }
        temp.write(reinterpret_cast<char*>(&a), sizeof(a));
        file.read(reinterpret_cast<char*>(&a), sizeof(a));
    }

    file.close();
    temp.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (!found) {
        cout << "User not found." << endl;
    }
}

void ATM::updateUserAsWithdraw(const string& uname) {
    ATM a;
    fstream file, temp;
    int amount;
    bool found = false;

    printHeader("Withdraw");

    file.open("users.txt", ios::in | ios::out | ios::ate);
    temp.open("temp.txt", ios::out | ios::app);
    file.seekg(0);
    file.read(reinterpret_cast<char*>(&a), sizeof(a));

    while (!file.eof()) {
        if (a.getUsername() == uname) {
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            if (a.balance < amount) {
                cout << "Insufficient balance to withdraw." << endl;
            } else {
                a.balance -= amount;
                cout << "Balance updated: " << a.balance << endl;
            }
            found = true;
        }
        temp.write(reinterpret_cast<char*>(&a), sizeof(a));
        file.read(reinterpret_cast<char*>(&a), sizeof(a));
    }

    file.close();
    temp.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (!found) {
        cout << "User not found." << endl;
    }
}

int ATM::searchSpecificUser(const string& uname, int pass) {
    ATM a;
    fstream file;

    file.open("users.txt", ios::in);
    file.read(reinterpret_cast<char*>(&a), sizeof(a));

    while (!file.eof()) {
        if (a.getUsername() == uname && a.getPassword() == pass) {
            return 1;
        }
        file.read(reinterpret_cast<char*>(&a), sizeof(a));
    }

    file.close();
    return 0;
}

int ATM::searchAllUsersToDisplay(const string& uname) {
    ATM a;
    fstream file;

    file.open("users.txt", ios::in);
    file.read(reinterpret_cast<char*>(&a), sizeof(a));

    while (!file.eof()) {
        if (a.getUsername() == uname) {
            cout << "\nUsername: " << a.getUsername();
            cout << "\nBalance: " << a.balance << endl;
            return 0;
        }
        file.read(reinterpret_cast<char*>(&a), sizeof(a));
    }

    file.close();
    cout << "User not found." << endl;
    return 0;
}

int main() {
    atmUser();
    return 0;
}
