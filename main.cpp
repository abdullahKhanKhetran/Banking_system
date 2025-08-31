#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Account {
    int accNo;
    char name[50];
    int deposit;
    char type;

public:
    void createAccount() {
        cout << "\nEnter Account No. : ";
        cin >> accNo;
        cout << "Enter Name : ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Enter Type (C/S) : ";
        cin >> type;
        type = toupper(type);
        cout << "Enter Initial Deposit : ";
        cin >> deposit;
        cout << "\nAccount Created Successfully...\n";
    }

    void showAccount() const {
        cout << "\nAccount No. : " << accNo;
        cout << "\nName : " << name;
        cout << "\nType : " << type;
        cout << "\nBalance : " << deposit << endl;
    }

    void modify() {
        cout << "\nModify Name : ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Modify Type (C/S) : ";
        cin >> type;
        type = toupper(type);
        cout << "Modify Balance : ";
        cin >> deposit;
    }

    void depositAmount(int x) { deposit += x; }
    void withdrawAmount(int x) { deposit -= x; }
    int getAccNo() const { return accNo; }
    int getDeposit() const { return deposit; }
    char getType() const { return type; }

    void report() const {
        cout << accNo << setw(10) << " " << name << setw(10)
             << " " << type << setw(6) << deposit << endl;
    }
};

void writeAccount() {
    Account ac;
    ofstream outFile;
    outFile.open("account.dat", ios::binary | ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char *>(&ac), sizeof(Account));
    outFile.close();
}

void displaySp(int n) {
    Account ac;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open !! Press any key...";
        return;
    }
    bool flag = false;
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(Account))) {
        if (ac.getAccNo() == n) {
            ac.showAccount();
            flag = true;
        }
    }
    inFile.close();
    if (!flag)
        cout << "\nAccount number does not exist";
}

void modifyAccount(int n) {
    bool found = false;
    Account ac;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be open !!";
        return;
    }
    while (!File.eof() && found == false) {
        File.read(reinterpret_cast<char *>(&ac), sizeof(Account));
        if (ac.getAccNo() == n) {
            ac.showAccount();
            cout << "\nEnter new details: ";
            ac.modify();
            int pos = (-1) * static_cast<int>(sizeof(Account));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(Account));
            cout << "\nRecord Updated";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\nRecord Not Found";
}

void deleteAccount(int n) {
    Account ac;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open !!";
        return;
    }
    outFile.open("Temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(Account))) {
        if (ac.getAccNo() != n) {
            outFile.write(reinterpret_cast<char *>(&ac), sizeof(Account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("Temp.dat", "account.dat");
    cout << "\nRecord Deleted ..";
}

void displayAll() {
    Account ac;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open !!";
        return;
    }
    cout << "\n\n\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c no.      NAME           Type  Balance\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(Account))) {
        ac.report();
    }
    inFile.close();
}

void depositWithdraw(int n, int option) {
    int amt;
    bool found = false;
    Account ac;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be open !!";
        return;
    }
    while (!File.eof() && found == false) {
        File.read(reinterpret_cast<char *>(&ac), sizeof(Account));
        if (ac.getAccNo() == n) {
            ac.showAccount();
            if (option == 1) {
                cout << "\nDeposit Amount: ";
                cin >> amt;
                ac.depositAmount(amt);
            }
            if (option == 2) {
                cout << "\nWithdraw Amount: ";
                cin >> amt;
                int bal = ac.getDeposit() - amt;
                if ((bal < 0)) {
                    cout << "Insufficient balance";
                } else {
                    ac.withdrawAmount(amt);
                }
            }
            int pos = (-1) * static_cast<int>(sizeof(ac));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(Account));
            cout << "\nRecord Updated";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\nRecord Not Found";
}

// 🔥 Entry point with WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    AllocConsole(); // attach a console window
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);

    char ch;
    int num;
    do {
        cout << "\n\n\tMAIN MENU";
        cout << "\n\n\t01. NEW ACCOUNT";
        cout << "\n\n\t02. DEPOSIT AMOUNT";
        cout << "\n\n\t03. WITHDRAW AMOUNT";
        cout << "\n\n\t04. BALANCE ENQUIRY";
        cout << "\n\n\t05. ALL ACCOUNT HOLDER LIST";
        cout << "\n\n\t06. CLOSE AN ACCOUNT";
        cout << "\n\n\t07. MODIFY AN ACCOUNT";
        cout << "\n\n\t08. EXIT";
        cout << "\n\n\tSelect Your Option (1-8): ";
        cin >> ch;

        switch (ch) {
        case '1':
            writeAccount();
            break;
        case '2':
            cout << "\n\nEnter Account No. : "; cin >> num;
            depositWithdraw(num, 1);
            break;
        case '3':
            cout << "\n\nEnter Account No. : "; cin >> num;
            depositWithdraw(num, 2);
            break;
        case '4':
            cout << "\n\nEnter Account No. : "; cin >> num;
            displaySp(num);
            break;
        case '5':
            displayAll();
            break;
        case '6':
            cout << "\n\nEnter Account No. : "; cin >> num;
            deleteAccount(num);
            break;
        case '7':
            cout << "\n\nEnter Account No. : "; cin >> num;
            modifyAccount(num);
            break;
        case '8':
            cout << "\n\nThanks for using Bank Management System";
            break;
        default:
            cout << "\a";
        }
    } while (ch != '8');

    FreeConsole(); // clean exit
    return 0;
}
