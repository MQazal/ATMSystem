#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

string const ClientsDatabase = "BankClients_2025.txt";

enum enWithdrawOptions
{
    Twenty = 1, Fifty = 2, One_Hundred = 3, Two_Hundred = 4,
    Four_Hundred = 5, Six_Hundred = 6, Eight_Hundred = 7, One_Thousand = 8, Exit = 9
};

enum enUpdateComponents
{
    UpdateClientBalance = 1, UpdateClientPinCode = 2
};

enum enATM_MenuOptions
{
    QuickWithdraw = 1, NormalWithdraw = 2, Deposit = 3, CheckBalance = 4, ShowMyAccount = 5
    , ChangePinCode = 6, Logout = 7
};

void BankSystemLogo()
{
	cout << R"(
      ____             _    
     |  _ \           | |   
     | |_) | __ _ _ __| | __
     |  _ < / _` | '__| |/ /
     | |_) | (_| | |  |   < 
     |____/ \__,_|_|  |_|\_\
)" << endl << endl;
}

void ClearScreen()
{
	system("cls");
}

string ReadString(string InputMessage)
{
    string Input;
    cout << InputMessage;
    cin >> Input;
    return Input;
}

struct stBankClientInfo
{
    string AccountNumber;
    string PinCode;
    string FullName;
    string PhoneNumber;
    double AccountBalance = 0.0;
};

stBankClientInfo AuthenticatedClient;

vector <stBankClientInfo> vClientsRecords;

char AskToPerformOperation(string Message)
{
    char SurePerform;
    cout << "\n" << Message << ": Y/N? ";
    cin >> SurePerform;
    return SurePerform;
}

void GoBackToMenu(string Message)
{
    cout << "\n\n" << Message;
    system("pause>0");
}

// 1-Quick Withdraw Operation
void ShowATM_Menu();

void ShowQuickWithdrawScreen()
{
    ClearScreen();
    cout << "\t========================================\n";
    cout << "\t|        QUICK WITHDRAW SCREEN         |\n";
    cout << "\t========================================\n\n";
}

void ShowQuickWithdrawMenu()
{
    cout << "\t|---------------------------------------|\n";
    cout << "\t|     [1] 20      |     [2] 50          |\n";
    cout << "\t|     [3] 100     |     [4] 200         |\n";
    cout << "\t|     [5] 400     |     [6] 600         |\n";
    cout << "\t|     [7] 800     |     [8] 1000        |\n";
    cout << "\t|             [9] Exit                  |\n";
    cout << "\t|---------------------------------------|\n\n";
    cout << "Your Balance is " << AuthenticatedClient.AccountBalance << endl;
    cout << "Choose what to withdraw from [1] to [8]: ";
}

enWithdrawOptions ReadQuickWithdrawOption()
{
    short Option = 0;
    cin >> Option;
    return (enWithdrawOptions)Option;
}

bool CheckAccountBalanceAmount(int WithdrawAmount)
{
    return AuthenticatedClient.AccountBalance > WithdrawAmount;
}

int UpdateAccountBalanceByWithdraw(int WithdrawAmount)
{
    return AuthenticatedClient.AccountBalance -= (WithdrawAmount);
}

void RefreshClientRecord(enUpdateComponents CNum = enUpdateComponents::UpdateClientBalance)
{
    for (stBankClientInfo& Client : vClientsRecords)
    {
        if (Client.AccountNumber == AuthenticatedClient.AccountNumber)
        {
            if (CNum == enUpdateComponents::UpdateClientBalance)
            {
                Client.AccountBalance = AuthenticatedClient.AccountBalance;
                break;
            }
            else
            {
                Client.PinCode = AuthenticatedClient.PinCode;
                break;
            }
        }
    }
}

string ConvertClientRecordToDataLine(stBankClientInfo Record, string delimeter = "#//#")
{
    string ClientDateLine;
    ClientDateLine += Record.AccountNumber + delimeter;
    ClientDateLine += Record.PinCode + delimeter;
    ClientDateLine += Record.FullName + delimeter;
    ClientDateLine += Record.PhoneNumber + delimeter;
    ClientDateLine += to_string(Record.AccountBalance);
    return ClientDateLine;
}

void LoadClientsRecordsFromVectorToFile(string FileName)
{
    fstream File;
    string DataLine;

    File.open(FileName, ios::out);
    if (File.is_open())
    {
        for (stBankClientInfo& Client : vClientsRecords)
        {
            DataLine = ConvertClientRecordToDataLine(Client);
            File << DataLine << endl;
        }
        File.close();
    }
}

short GetAmountOfQuickWithdraw(enWithdrawOptions Option)
{
    short arrAmountsOfQuickWithdraw[8] = { 20,50,100,200,400,600,800,1000 };
    return arrAmountsOfQuickWithdraw[Option - 1];
}

void PerformQuickWithdrawFromBalance()
{
    enWithdrawOptions Option;
    do
    {
        ShowQuickWithdrawScreen();
        ShowQuickWithdrawMenu();
        Option = ReadQuickWithdrawOption();
        if (Option == enWithdrawOptions::Exit) break;

        short Amount = GetAmountOfQuickWithdraw(Option);

        if (CheckAccountBalanceAmount(Amount))
        {
            if (AskToPerformOperation("Are you sure you want to perform this transaction") == 'Y')
            {
                AuthenticatedClient.AccountBalance = UpdateAccountBalanceByWithdraw(Amount);
                RefreshClientRecord();
                LoadClientsRecordsFromVectorToFile(ClientsDatabase);
                cout << "\nDone Successfully, your balance is " << AuthenticatedClient.AccountBalance;
            }
            else
                cout << "\nSorry, operation is failed.";
        }
        else
        {
            cout << "\nOops, Withdraw Amount Exceeds the balance, you can withdraw up to [" << AuthenticatedClient.AccountBalance << "], make another choice!";
        }
        GoBackToMenu("Press any tap to continue...");

    } while (Option != enWithdrawOptions::Exit);

    GoBackToMenu("Press any key to go back to ATM System Menu...");
}

// 2-Normal Withdraw Operation
void ShowNormalWithdrawScreen()
{
    ClearScreen();
    cout << "\t========================================\n";
    cout << "\t|        NORMAL WITHDRAW SCREEN        |\n";
    cout << "\t========================================\n\n";
}

int ReadWithdrawAmount()
{
    int Amount = 0;
    cout << "\nEnter a withdrawal amount in multiples of 5: ";
    cin >> Amount;
    return Amount;
}

bool IsWithdrawalAmountMultipleOf5(int WithdrawAmount)
{
    return WithdrawAmount % 5 == 0;
}

void PerformNormalWithdrawFromBalance()
{
    bool ValidBalance;
    do
    {
        ShowNormalWithdrawScreen();
        int Amount = ReadWithdrawAmount();
        bool ValidAmount = IsWithdrawalAmountMultipleOf5(Amount);
        while (!ValidAmount)
        {
            Amount = ReadWithdrawAmount();
            ValidAmount = IsWithdrawalAmountMultipleOf5(Amount);
        }

        ValidBalance = CheckAccountBalanceAmount(Amount);
        if (ValidBalance)
        {
            if (AskToPerformOperation("Are you sure you want to perform this transaction") == 'Y')
            {
                AuthenticatedClient.AccountBalance = UpdateAccountBalanceByWithdraw(Amount);
                RefreshClientRecord();
                LoadClientsRecordsFromVectorToFile(ClientsDatabase);
                cout << "\nDone Successfully, your balance is " << AuthenticatedClient.AccountBalance;
                break;
            }
            else
                cout << "\nOperation is failed.";
        }
        else
        {
            cout << "\nOops, Withdraw Amount Exceeds the balance, you can withdraw up to [" << AuthenticatedClient.AccountBalance << "], make another choice";
        }
        GoBackToMenu("Press any tap to continue...");

    } while (!ValidBalance);

    GoBackToMenu("Press any key to go back to ATM System Menu...");
}

// 3-Deposit Operation
void ShowDepositScreen()
{
    ClearScreen();
    cout << "\t========================================\n";
    cout << "\t|            DEPOSIT SCREEN            |\n";
    cout << "\t========================================\n\n";
}

double ReadDepositAmount()
{
    double Amount = 0;
    cout << "Enter a positive Deposit Amount: ";
    cin >> Amount;
    return Amount;
}

bool IsDepositAmountPositive(double DepositAmount)
{
    return DepositAmount > 0;
}

double UpdateAccountBalanceByDeposit(double DepositAmount)
{
   return UpdateAccountBalanceByWithdraw(DepositAmount * -1);
}

void PerformDepositFromBalance()
{
    ShowDepositScreen();
    double DepositAmount = ReadDepositAmount();
    bool ValidAmount = IsDepositAmountPositive(DepositAmount);
    while (!ValidAmount)
    {
        DepositAmount = ReadDepositAmount();
        ValidAmount = IsDepositAmountPositive(DepositAmount);
    }
    if (AskToPerformOperation("Are you sure you want to perform this transaction") == 'Y')
    {
        AuthenticatedClient.AccountBalance = UpdateAccountBalanceByDeposit(DepositAmount);
        RefreshClientRecord();
        LoadClientsRecordsFromVectorToFile(ClientsDatabase);
        cout << "\nDone Successfully, your balance is " << AuthenticatedClient.AccountBalance;
    }
    else
    {
        cout << "\nSorry, operation is failed.";
    }
    GoBackToMenu("Press any key to go back to ATM System Menu...");
}

// 4-Check Balance Operation
void CheckBalanceScreen()
{
    ClearScreen();
    cout << "\t========================================\n";
    cout << "\t|         BALANCE CHECK SCREEN         |\n";
    cout << "\t========================================\n\n";
}

short ClientBalanceAmount()
{
    return AuthenticatedClient.AccountBalance;
}

void ShowClientBalance()
{
    CheckBalanceScreen();
    cout << "\n\tYour balance is " << ClientBalanceAmount() << endl;
    GoBackToMenu("Press any key to go back to ATM System Menu...");
}

// 5-Show Client's Account Operation
void ShowAccountScreen()
{
    ClearScreen();
    cout << "\t========================================\n";
    cout << "\t|        CLIENT ACCOUNT SCREEN         |\n";
    cout << "\t========================================\n\n";
}

void ShowClientAccount()
{
    ShowAccountScreen();
    cout << "\t|----------------------------------|\n";
    cout << "\t|-Account Number : " << AuthenticatedClient.AccountNumber << endl;
    cout << "\t|-PinCode        : " << AuthenticatedClient.PinCode << endl;
    cout << "\t|-Full Name      : " << AuthenticatedClient.FullName << endl;
    cout << "\t|-Phone Number   : " << AuthenticatedClient.PhoneNumber << endl;
    cout << "\t|-Account Balance: " << AuthenticatedClient.AccountBalance << endl;
    cout << "\t|----------------------------------|\n";
    GoBackToMenu("Press any key to go back to ATM System Menu...");
}

// 6-Change PinCode Operation
void ChangePinCodeScreen()
{
    ClearScreen();
    cout << "\t========================================\n";
    cout << "\t|        CHANGE PINCODE SCREEN         |\n";
    cout << "\t========================================\n\n";
}

bool IsClientExistsByAccountNumber(string AccountNumber)
{
    for (stBankClientInfo& Client : vClientsRecords)
    {
        if (Client.AccountNumber == AccountNumber)
            return true;
    }
    return false;
}

void PerformChangePinCode()
{
    ChangePinCodeScreen();
    if (IsClientExistsByAccountNumber(ReadString("\nEnter account number: ")))
    {
        AuthenticatedClient.PinCode = ReadString("\nEnter new PinCode: ");
        if (AskToPerformOperation("Are you sure you want to change PinCode") == 'Y')
        {
            RefreshClientRecord(enUpdateComponents::UpdateClientPinCode);
            LoadClientsRecordsFromVectorToFile(ClientsDatabase);
            cout << "\nDone Successfully, your new PinCode is " << AuthenticatedClient.PinCode;
        }
        else
            cout << "\nOperation is failed.";
    }
    else
    {
        cout << "\nOops, wrong Account Number.";
    }
    GoBackToMenu("Press any key to go back to ATM System Menu...");
}

// Integration ATM Menu with Operations
void Login();

void ShowATM_Menu()
{
    ClearScreen();
    BankSystemLogo();
    cout << "\t|----------------------------------------|\n";
    cout << "\t|             ATM SYSTEM MENU            |\n";
    cout << "\t|----------------------------------------|\n";
    cout << "\t|          [1] Quick Withdraw            |\n";
    cout << "\t|          [2] Normal Withdraw           |\n";
    cout << "\t|          [3] Deposit                   |\n";
    cout << "\t|          [4] Check Balance             |\n";
    cout << "\t|          [5] Show My Account           |\n";
    cout << "\t|          [6] Change PinCode            |\n";
    cout << "\t|          [7] Logout                    |\n";
    cout << "\t|----------------------------------------|\n\n";
    cout << "\tChoose what do you want from [1] to [7]: ";
}

enATM_MenuOptions ReadATMOption()
{
    short Option = 0;
    cin >> Option;
    return (enATM_MenuOptions)Option;
}

void PerformATMMenuOption(enATM_MenuOptions Option)
{
    switch (Option)
    {
    case enATM_MenuOptions::QuickWithdraw:
        PerformQuickWithdrawFromBalance();
        break;
    case enATM_MenuOptions::NormalWithdraw:
        PerformNormalWithdrawFromBalance();
        break;
    case enATM_MenuOptions::Deposit:
        PerformDepositFromBalance();
        break;
    case enATM_MenuOptions::CheckBalance:
        ShowClientBalance();
        break;
    case enATM_MenuOptions::ShowMyAccount:
        ShowClientAccount();
        break;
    case enATM_MenuOptions::ChangePinCode:
        PerformChangePinCode();
        break;
    case enATM_MenuOptions::Logout:
        Login();
        break;
    }
}

// Play ATM System
void PlayATMSystem()
{
    enATM_MenuOptions Option;
    do
    {
        ShowATM_Menu();
        Option = ReadATMOption();
        PerformATMMenuOption(Option);
    } while (Option != enATM_MenuOptions::Logout);
}

// Login System
void ShowLoginScreen()
{
    ClearScreen();
    cout << "\t========================================\n";
    cout << "\t|       HELLO, THIS LOGIN SCREEN       |\n";
    cout << "\t========================================\n\n";
}

vector<string> SplitString(string Text, string delimiterType)
{
    vector <string> vSplitTextWords{};
    string FullWord = " ";
    short delimiterIndex = 0;

    while ((delimiterIndex = Text.find(delimiterType)) != Text.npos)
    {
        if (Text != " ")
        {
            FullWord = Text.substr(0, delimiterIndex);
            vSplitTextWords.push_back(FullWord);
        }
        Text.erase(0, delimiterIndex + delimiterType.length()); // delete from 0 to number of delimiter characters
    }

    if (Text != " ")
    {
        vSplitTextWords.push_back(Text); // push last word
    }
    return vSplitTextWords;
}

stBankClientInfo ConvertClientDataLineToRecord(string DataLine)
{
    vector <string> vDataLine = SplitString(DataLine, "#//#");
    stBankClientInfo Client;
    Client.AccountNumber = vDataLine[0];
    Client.PinCode = vDataLine[1];
    Client.FullName = vDataLine[2];
    Client.PhoneNumber = vDataLine[3];
    Client.AccountBalance = stod(vDataLine[4]);
    return Client;
}

vector <stBankClientInfo> LoadClientsDataFromFileToVector(string FileName)
{
    fstream File;
    string ClientDataLine;
    stBankClientInfo ClientRecord;
    vector <stBankClientInfo> vClientsRecords;

    File.open(FileName, ios::in);
    if (File.is_open())
    {
        while (getline(File, ClientDataLine))
        {
            ClientRecord = ConvertClientDataLineToRecord(ClientDataLine);
            vClientsRecords.push_back(ClientRecord);
        }
        File.close();
    }

    return vClientsRecords;
}

bool IsClientExistsByAccountNumberAndPinCode(string AccountNumber, string PinCode, stBankClientInfo& AuthenticatedClient)
{
    vClientsRecords = LoadClientsDataFromFileToVector(ClientsDatabase);

    for (stBankClientInfo& Client : vClientsRecords)
    {
        if (Client.AccountNumber == AccountNumber && Client.PinCode == PinCode)
        {
            AuthenticatedClient = Client;
            return true;
        }
    }
    return false;
}

void Login()
{
    ShowLoginScreen();
    bool ValidLogin;
    do
    {
        string AccountNumber, PinCode;
        AccountNumber = ReadString("\nEnter Account Number: ");
        PinCode = ReadString("\nEnter PinCode: ");
        ValidLogin = IsClientExistsByAccountNumberAndPinCode(AccountNumber, PinCode, AuthenticatedClient);
        if (ValidLogin)
        {
            break;
        }
        ShowLoginScreen();
        cout << "\nInvalid Username/Password!!\n\n";
    } while (!ValidLogin);
    PlayATMSystem();
}

int main()
{
    Login();
	return 0;
}