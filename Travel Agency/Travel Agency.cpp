// Juan Valencia - Programming c++ Final Project.
/* This program will function as a travel agency. The user can choose from 5 destinations where they want to go and choose from different packages based on the location chosen. The reservation will be stored in an account that the user created at the beginning and that is stored in a text file.
 */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
using namespace std;

// Structure for creating user's account
struct Account
{
  string username;
  string password;
  int verificationCode;
};

// Reservation Struct
struct Reservation
{
  string location;
  string name;
  int month;
  int day;
  int year;
  int numberPeople;
  int tourPeople;
  int carnivalPeople;
  int climbPeople;
  int totalRental;
  int confirmationCode;
  double totalInstructor;
  double baseCharges;
  double totalGuide;
  double discount;
  double charges;
  double deposit;
};
// Reservation array
const int MAX_RESERVATIONS = 1000;
Reservation lastReservation[MAX_RESERVATIONS];
int reservationCount = 0;

// Constants for the charges
const double MEXICO_RATE = 450.0;   // Ticket per person
const double MACHUP_TOUR = 150.0;   // Base rate - tour to Machu Picchu
const double MACHUP_GUIDE = 75.0;   // Tour guide rate
const double PARIS_RATE = 525.0;    // Ticket per person
const double EIFFEL_TOUR = 200;     // Base rate - Tour around Paris
const double EIFFEL_GUIDE = 55.0;   // Tour guide rate
const double TOKYO_RATE = 600.0;    // Ticket per person
const double FUJI_CLIMB = 275.0;    // Base rate - Climbing Mount Fuji
const double FUJI_INSTRUCT = 75.0;  // Climbing instructor rate
const double FUJI_RENTAL = 40.0;    // Climbing equipment rental
const double VIENNA_RATE = 475.0;   // Ticket per person
const double VIENNA_TOUR = 100.0;   // Base rate - Tour around Vienna
const double VIENNA_GUIDE = 35.0;   // Tour guide rate
const double RIO_RATE = 550.0;      // Ticket per person
const double CARNIVAL_PASS = 125.0; // Base rate - Tickets to carnival

// Function prototypes to create or recover user's account
int menu(Reservation[], int);
int confirmationNumb(int &);
void login(Reservation[], int);
void logout();
void create_account();
void recover_menu();
void recover_username();
void change_password();
bool isValidPassword(string);
string encryptPassword(string);

// Function prototypes for traveling locations
int agencyMenu(string, Reservation[], int);
int travelMenu(string, Reservation[], int);
int finishMenu(string, Reservation[], int);
void Mexico(string);
void France(string);
void Japan(string);
void Australia(string);
void Brazil(string);

// Function prototype for showing stored reservations
void displayReservation(string, Reservation[], int);

// Function prototype for input verification and date verification
int yesNo(char &, int, double &, double);
void verifyDate(int &, int &, int &);

int main()
{

  int selection;

  // Handle the user's choice when entering the main menu
  do
  {
    selection = menu(lastReservation, reservationCount);
    switch (selection)
    {
    case 1:
      login(lastReservation, reservationCount);
      break;
    case 2:
      create_account();
      break;
    case 3:
      recover_menu();
      break;
    case 4:
      cout << "Exiting the program." << endl
           << endl;
    }
  } while (selection != 4);
  return 0;
}

int menu(Reservation lastReservation[], int reservationCount)
{
  int choice;
  int i = 0;
  // Display the menu and get the user's choice
  cout << "    World Travel Agency\n";
  cout << "----------------------------\n";
  cout << "Welcome to World Travel Agency!" << endl
       << endl;
  ;
  cout << "1. Log in" << endl;
  cout << "2. Create a new account" << endl;
  cout << "3. Recover Account" << endl;
  cout << "4. Exit" << endl;
  cout << "Enter 1, 2, 3, or 4: ";
  cin >> choice;
  cout << "\n";

  if (choice < 1 || choice > 4) // Validate input and exits if 3 wrong attempts are made
  {
    for (i = 0; i < 3; i++)
    {
      cout << "Invalid Selection. Enter 1, 2, 3 or 4: ";
      cin >> choice;
      cout << "\n";
      if (choice >= 1 && choice <= 4) // Exit the loop if input is within valid range
      {
        break;
      }
    }
    i++;
    if (i == 4)
    {
      cout << "Maximum number of tries exceeded. Exiting program." << endl;
      exit(0);
    }
  }
  return choice;
}

/**************************************************************************************************
                        Function definition of create account.
It creates a new text file for each user that creates an account and also creates a text file
that works as a database to store all accounts.
**************************************************************************************************/
void create_account()
{
  // Create a new account
  Account user;
  string username, password;
  int verificationCode;

  cout << "Create a new account.\n";
  cout << "Enter a username: "; // Ask to create a username
  cin >> username;
  cout << "\n";

  ofstream datafile("accounts.txt", ios::app);
  // Check if username already exists in file
  ifstream baseFile;
  baseFile.open("accounts.txt");
  if (!(baseFile.is_open()))
  {
    cout << "Error opening file." << endl;
    return;
  }
  bool usernameExists = false;
  while (baseFile >> user.username >> user.password >> user.verificationCode)
  {
    if (user.username == username)
    {
      usernameExists = true;
      cout << "Username already exists. Please choose a different username." << endl; // if username already exists, ask the user to choose a different one
      cout << "Enter a username: ";
      cin >> username;
      cout << "\n";
      break;
    }
  }
  baseFile.close();
  datafile.close();

  cout << "Enter a password (must be at least 8 characters long, have one uppercase, one lowercase, a number and a special character): "; // Ask to create a password
  cin >> password;
  cout << endl;
  while (!isValidPassword(password)) // Check if the password meets the requirements
  {
    cout << "Invalid password. Please enter a password that is at least 8 characters long, has one uppercase, one lowercase, a number and a special character: ";
    cin >> password;
    cout << endl;
  }

  cout << "Enter a 4 digit verification code to allow you to change the password: "; // Ask the user for a 4 digit code that can be used to recover the username or change the password
  cin >> verificationCode;
  cout << endl;

  user.username = username;
  user.password = encryptPassword(password); // Pass the username and the encrypted password to the Account structure
  user.verificationCode = verificationCode;

  ofstream outputFile;
  outputFile.open(username + ".txt", ios::app);
  ofstream dataFile;
  dataFile.open("accounts.txt", ios::app);

  if (!(outputFile.is_open() && dataFile.is_open()))
  {
    cout << "Error opening file." << endl;
    return;
  }

  outputFile << user.username << " " << user.password << " " << user.verificationCode << endl; // Write the username, password and code into user's file
  dataFile << user.username << " " << user.password << " " << user.verificationCode << endl;   // Write the username, password and code into a "database" file

  outputFile.close();
  dataFile.close();

  cout << "Account successfully created. You can log in now.\n\n";
  login(lastReservation, reservationCount); // Sends user to login function after creating account
}

/**************************************************************************************************
                        Function definition of log in.
After creating an account the user can log in with the information previously stored.
**************************************************************************************************/
void login(Reservation lastReservation[], int reservationCount)
{
  // Log in with an existing account
  bool loggedIn = false;
  int tries = 0;
  int decision;
  Account user;
  string username, password;

  cout << "Enter your username: ";
  cin >> username;
  cout << "Enter your password: ";
  cin >> password;
  cout << "\n";

  // Check if the account exists in the Database file
  bool accountExists = false;
  ifstream personalFile(username + ".txt");

  while (personalFile >> user.username >> user.password >> user.verificationCode)
  {
    if ((user.username == username) && (user.password == encryptPassword(password)))
    {
      accountExists = true;
      break;
    }
  }

  while (!accountExists && tries < 3) // Validates the input and if the account doesn't exist in the text file, asks the user to try again
  {
    tries++;
    cout << "Invalid username or password, please try again." << endl
         << endl;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;
    cout << endl;
    personalFile.close();
    personalFile.open(username + ".txt");
    while (personalFile >> user.username >> user.password >> user.verificationCode)
    {
      if (user.username == username && user.password == encryptPassword(password))
      {
        accountExists = true;
        break;
      }
    }
  }
  if (tries == 3 && !accountExists) // If 3 wrong attempts are made, it sends the user to another menu
  {
    cout << "Invalid username or password. Maximum number of tries exceeded." << endl;
    cout << "Would you like to create a new account, recover your username or change your password?" << endl;
    cout << "Enter '1' to create a new account,'2' to recover your username or '3' to change your password: ";

    int choice;
    cin >> choice;
    cout << "\n";

    switch (choice)
    {
    case 1:
      create_account();
      break;
    case 2:
      recover_username();
      break;
    case 3:
      change_password();
      break;
    default:
      cout << "Invalid choice. Exiting program." << endl;
      exit(0);
    }
  }
  personalFile.close();

  if (accountExists) // If the input matches the account information it sends the user to the TravelMenu to choose a location
  {
    cout << "Login successful." << endl
         << endl;
    agencyMenu(username, lastReservation, reservationCount);
  }
}

/**********************************************************
            Function definition of logout.
It finishes the program if the user chooses this option.
***********************************************************/
void logout()
{
  cout << "Logging out..." << endl;
  exit(0);
}

/***********************************************************************************************
                      Function definition of Recover Menu.
It redirects the user to a menu with three different options. To recover the username,
change password or go back to the main menu.
************************************************************************************************/
void recover_menu()
{
  int option;
  int i = 0;

  cout << "To recover your account please select between the following options: \n\n";
  cout << "1. Recover username." << endl;
  cout << "2. Change Password." << endl;
  cout << "3. Go to main menu." << endl;
  cout << "Enter 1, 2, or 3: ";
  cin >> option;
  cout << "\n";

  if (option < 1 || option > 3) // Validate input and exits if 3 wrong attempts are made
  {
    for (i = 0; i < 3; i++)
    {
      cout << "Invalid Selection. Enter 1, 2, 3 or 4: ";
      cin >> option;
      if (option >= 1 && option <= 3) // Exit the loop if input is within valid range
      {
        break;
      }
    }
    i++;
    if (i == 4)
    {
      cout << "Maximum number of tries exceeded. Exiting program." << endl;
      exit(0);
    }
  }

  switch (option)
  {
  case 1:
    recover_username();
    break;
  case 2:
    change_password();
    break;
  case 3:
    main();
    break;
  }
}

/**************************************************************************************************
                    Function definition of recover username.
In case the user doesn't remember their username, it asks for the verification code created with
the account and if it matches, it displays the username for the user.
**************************************************************************************************/
void recover_username()
{
  Account user;
  int code1;
  int tries = 0;
  bool found = false;
  ifstream baseFile("accounts.txt");

  cout << "\nEnter verification code: ";
  cin >> code1;
  cout << "\n";

  while (baseFile >> user.username >> user.password >> user.verificationCode)
  {
    if (code1 == user.verificationCode)
    {
      cout << "Your username is: " << user.username << endl
           << "Please log in.\n\n";
      found = true;
      break;
    }
  }
  baseFile.close();

  while (!found && tries < 3) // Validate input and exits if 3 wrong attempts are made
  {
    tries++;
    cout << "Invalid code, please try again: ";
    cin >> code1;
    cout << "\n";
    baseFile.open("accounts.txt");
    while (baseFile >> user.username >> user.password >> user.verificationCode)
    {
      if (code1 == user.verificationCode)
      {
        cout << "Your username is: " << user.username << endl
             << "Please log in.\n\n";
        found = true;
        break;
      }
    }
    baseFile.close();
    if (found)
    {
      break;
    }
    tries++;
  }
  if (tries == 3 && !found)
  {
    cout << "Invalid code. Maximum number of tries exceeded. Exiting...\n"
         << endl;
    exit(0);
  }
  // Call login function
  login(lastReservation, reservationCount);
}

/*************************************************************************************************
                        Function definition of change the password.
This function allows the user to change the password in case they forget it. It asks for the
username and the verifiaction code to access the account information. If both match to any
account stored, it allows the user to change the password and the new information gets stored.
*************************************************************************************************/
void change_password()
{
  Account user;
  string username;
  string changed_password;
  string password;
  int line_number = 1;
  int code2;
  int tries = 0;
  bool pasword_changed = false;

  bool password_exists = false;
  ifstream baseFile("accounts.txt");

  cout << "Enter your username: ";
  cin >> username;
  cout << "Enter verification code: ";
  cin >> code2;
  cout << "\n";

  if (!baseFile.is_open())
  {
    cout << "Error opening file." << endl;
    return;
  }
  while (baseFile >> user.username >> user.password >> user.verificationCode) // reads the information from the users account
  {
    if ((username == user.username) && (code2 == user.verificationCode)) // Verify if username and verification code match with account
    {
      cout << "Insert new password (must be at least 8 characters long, have one uppercase, one lowercase, a number and a special character): ";
      cin >> changed_password;
      pasword_changed = true;
      cout << "\n";

      while (!isValidPassword(changed_password))
      {
        cout << "Invalid password. Please enter a password that is at least 8 characters long, has one uppercase, one lowercase, a number and a special character: ";
        cin >> changed_password;
      }
      user.password = encryptPassword(changed_password);

      fstream outputFile(username + ".txt", ios::in | ios::out);
      ofstream dataFile("accounts.txt", ios::app);
      if (!outputFile.is_open())
      {
        cout << "Error opening file." << endl;
        return;
      }
      // Clear the first line of the user's file
      outputFile.seekp(0);
      outputFile << "";

      outputFile.seekp(0);
      outputFile << user.username << " " << user.password << " " << user.verificationCode << endl; // Writes the new password into the user's file
      dataFile << user.username << " " << user.password << " " << user.verificationCode << endl;   // Writes the new password into the database file

      outputFile.close();
      dataFile.close();
      if (pasword_changed)
      {
        cout << "Password succesfully changed.\n";
        cout << "Returning to main menu...\n\n";
        main();
      }
    }
  }
  baseFile.close();

  if (!pasword_changed)
  {
    tries++;
    while (tries < 3) // Validates the input and if the information doesn't match in the text file, asks the user to try again
    {
      cout << "Wrong information entered. Please try again." << endl;
      cout << "Enter your username: ";
      cin >> username;
      cout << "Enter verification code: ";
      cin >> code2;
      cout << "\n";

      baseFile.open("accounts.txt");
      while (baseFile >> user.username >> user.password >> user.verificationCode)
      {
        if ((username == user.username) && (code2 == user.verificationCode))
        {
          cout << "Insert new password (must be at least 8 characters long, have one uppercase, one lowercase, a number and a special character): ";
          cin >> changed_password;
          pasword_changed = true;

          while (!isValidPassword(changed_password))
          {
            cout << "Invalid password. Please enter a password that is at least 8 characters long, has one uppercase, one lowercase, a number and a special character: ";
            cin >> changed_password;
            cout << "\n";
          }
          user.password = encryptPassword(changed_password);
          fstream outputFile(username + ".txt", ios::in | ios::out);
          ofstream dataFile("accounts.txt", ios::app);
          if (!outputFile.is_open())
          {
            cout << "Error opening file." << endl;
            return;
          }
          // Clear the first line of the user's file
          outputFile.seekp(0);
          outputFile << "";

          outputFile.seekp(0);
          outputFile << user.username << " " << user.password << " " << user.verificationCode << endl; // Writes the new password into the user's file
          dataFile << user.username << " " << user.password << " " << user.verificationCode << endl;   // Writes the new password into the database file

          outputFile.close();
          dataFile.close();
          if (pasword_changed)
          {
            cout << "Password succesfully changed.\n";
            cout << "Returning to main menu...\n\n";
            main();
          }
        }
      }
      if (pasword_changed)
      {
        break;
      }
      tries++;
    }
    if (tries == 4 && !pasword_changed) // If 3 wrong attempts are made, exits the program
    {
      cout << "Invalid information. Maximum number of tries exceeded. Exiting...\n"
           << endl;
      exit(0);
    }
  }
  baseFile.close();

  baseFile.open("accounts.txt");

  if (!baseFile.is_open())
  {
    cout << "Error opening file." << endl;
    return;
  }
  while (baseFile >> user.username >> user.password >> user.verificationCode)
  {
    if (encryptPassword(changed_password) == user.password)
    {
      password_exists = true;
    }
  }
  baseFile.close();

  if (password_exists) // Checks if password already exists in the user's account
  {
    cout << "Password already exists. Please log in with your existing account.\n\n";
    login(lastReservation, reservationCount); // If account exists, it redirects the user to log in function
  }
}

/***************************************************************************************************
                          Function definition of encrypt password.
This function receives the password from the user, encrypts it and returns it to the past function.
****************************************************************************************************/
string encryptPassword(string password)
{
  // Encrypt the password using a Caesar cipher
  string encryptedPassword;
  for (char &c : password)
  {
    encryptedPassword += char(int(c) + 3); // for each character it moves 3 characters upwards
  }
  return encryptedPassword; // returns encrypted password to the user's account
}

/***************************************************************************************************
                          Function definition of is valid password.
This function receives the password from the user, verifies that meets the requirements and returns
them
****************************************************************************************************/
bool isValidPassword(string password)
{
  // Check if the password meets the requirements
  bool hasUppercase = false, hasLowercase = false, hasNumber = false, hasSpecialChar = false;
  for (char c : password)
  {
    if (isupper(c)) // Checks if password has uppercase letter
    {
      hasUppercase = true;
    }
    else if (islower(c)) // Checks if password has lowercase letter
    {
      hasLowercase = true;
    }
    else if (isdigit(c)) // Checks if password has a digit
    {
      hasNumber = true;
    }
    else if (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '*' || c == '/') // Checks if password has a special character
    {
      hasSpecialChar = true;
    }
  }
  return password.length() >= 8 && hasUppercase && hasLowercase && hasNumber && hasSpecialChar; // Checks if password's lenght is 8 or more characters and returns requirements
}

/*************************************************************************************************
            Definition of agency menu to get the user's decision after loggin in.
Displays a secondary menu and asks the user to select an option. Redirects the user to the location chosen
**************************************************************************************************/

int agencyMenu(string username, Reservation lastReservation[], int reservationCount)
{
  int decision;
  int i = 0;

  cout << "Welcome to World Travel Agency! What would you like to do? \n\n";
  cout << "1. Create reservation" << endl;
  cout << "2. Access reservation" << endl;
  cout << "3. Log out" << endl;
  cout << "Enter 1, 2, or 3: ";
  cin >> decision;
  cout << "\n";

  if (decision < 1 || decision > 3) // Validate input and exits if 3 wrong attempts are made
  {
    for (i = 0; i < 3; i++)
    {
      cout << "Invalid Selection. Enter 1, 2, 3 or 4: ";
      cin >> decision;
      if (decision >= 1 && decision <= 3) // Exit the loop if input is within valid range
      {
        break;
      }
    }
    i++;
    if (i == 4)
    {
      cout << "Maximum number of tries exceeded. Exiting program." << endl;
      exit(0);
    }
  }
  // Handle the user's choice when entering the agency menu
  do
  {
    switch (decision)
    {
    case 1:
      travelMenu(username, lastReservation, reservationCount);
      break;
    case 2:
      displayReservation(username, lastReservation, reservationCount);
      break;
    case 3:
      logout();
      break;
    }
  } while (decision != 3);
  return 0;
}

/*************************************************************************************************
            Definition of travel menu to get the user's desire location to travel
Displays a tertiary menu and asks the user to select a location option. Redirects the user to the location chosen
**************************************************************************************************/

int travelMenu(string username, Reservation lastReservation[], int reservationCount)
{
  int destination;
  int i = 0;
  // Display the travel menu and get the user's choice
  cout << "   World Travel Agency\n";
  cout << "----------------------------\n";
  cout << "Please choose the destination where you would like to fly: " << endl;
  cout << "1. Cancun" << endl;
  cout << "2. Paris" << endl;
  cout << "3. Tokyo" << endl;
  cout << "4. Vienna" << endl;
  cout << "5. Rio" << endl;
  cout << "6. Log out" << endl;
  cout << "Enter 1, 2, 3, 4, 5 or 6: ";
  cin >> destination;
  cout << "\n";

  if (destination < 1 || destination > 6) // Validate input and exits if 3 wrong attempts are made
  {
    for (i = 0; i < 3; i++)
    {
      cout << "Invalid Selection. Enter 1, 2, 3 or 4: ";
      cin >> destination;
      if (destination >= 1 && destination <= 6) // Exit the loop if input is within valid range
      {
        break;
      }
    }
    i++;
    if (i == 4)
    {
      cout << "Maximum number of tries exceeded. Exiting program." << endl;
      exit(0);
    }
  }

  cout << fixed << showpoint << setprecision(2); // Set number of decimals to two in all locations
  do                                             // Handle the user's choice when entering the travel menu
  {
    switch (destination)
    {

    case 1:
      Mexico(username);
      break;
    case 2:
      France(username);
      break;
    case 3:
      Japan(username);
      break;
    case 4:
      Australia(username);
      break;
    case 5:
      Brazil(username);
      break;
    case 6:
      logout();
    }
  } while (destination != 6);
  return 0;
}

/****************************************************************************************
                      Function definition of verify date.
It takes the date from the user and verifies it's withing the correct range.
*****************************************************************************************/
void verifyDate(int &month, int &day, int &year)
{
  bool date = false;
  int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
  {
    // if it's a leap year, February has 29 days
    daysInMonth[2] = 29;
  }
  while (!date)
  {
    if ((month >= 1 && month <= 12) && (day >= 1 && day <= daysInMonth[month]) && (year >= 2023))
    {
      date = true;
      break;
    }
    else if ((month < 1 || month > 12) || (day < 1 || day > daysInMonth[month]) || (year < 2023)) // If date is incorrect it asks the user to try again.
    {
      cout << "Error. Invalid date. Please try again: " << endl;
      cin >> month;
      cin.get();
      cin >> day;
      cin.get();
      cin >> year;
      cin.ignore();
    }
    if (date)
    {
      break;
    }
  }
}

/****************************************************************************************
                      Function definition of yesNo.
It takes the choice from the user and validates the input. It returns the cost for
guide/instructor
*****************************************************************************************/
int yesNo(char &choice, int numberPeople, double &baseGuide, double rate)
{
  while (!(choice == 'Y' || choice == 'y' || choice == 'N' || choice == 'n')) // Validates input, if incorrect asks to try again
  {
    cout << "Wrong answer, please enter Y/y for 'yes', N/n for 'no'" << endl;
    cin >> choice;
  }

  if (choice == 'Y' || choice == 'y') // If correct, proceedes to calculate
  {
    baseGuide = numberPeople * rate;
  }
  else if (choice == 'N' || choice == 'n')
  {
    baseGuide = 0;
  }
  return baseGuide;
}

/****************************************************************************************
                      Function definition of confirmationNumb.
It generates a random number that would be assigned as confirmation number for each reservation
*****************************************************************************************/
int confirmationNumb(int &random)
{
  srand((unsigned)time(NULL));

  random = 0 + (rand() % 999999);

  return random;
}

/****************************************************************************************
Definition of Mexico function to get the user's information when traveling to Cancun.
Asks the user a series of questions and then displays a breakdown of all the charges.
*****************************************************************************************/
void Mexico(string username)
{
  Reservation reservation; // Reservation struct
  reservation.location = "Cancun";
  char guide;
  string name;          // Name for reservation
  int month, day, year; // Date for reservation
  int numberPeople;     // Number in party
  int tourPeople;       // Number for tour
  double totalGuide;    // Cost of guide
  double baseCharges;   // Base charges
  double charges;       // Total charges
  double discount = 0;  // Discount
  double deposit;       // Required deposit

  ofstream outputFile; // Opens user's file to write the reservation info
  outputFile.open(username + ".txt", ios::app);

  if (!outputFile.is_open())
  {
    cout << "Error opening file." << endl;
    return;
  }

  cout << "\t\tYou have selected Cancun!" << endl;
  cout << "Enter the date for the reservation (MM/DD/YYYY): " << endl;
  cin >> reservation.month;
  cin.get();
  cin >> reservation.day;
  cin.get();
  cin >> reservation.year;
  cin.ignore();
  cout << "\n\n";

  verifyDate(reservation.month, reservation.day, reservation.year);

  cout << "Enter the name for the reservation: ";
  getline(cin, reservation.name);
  cout << "\n\n";

  cout << "To travel to Cancun the price per person is: $" << MEXICO_RATE << endl;
  cout << "How many people will be traveling?" << endl;
  cin >> reservation.numberPeople;
  cout << "How many people will be taking the tour through the Mchu Picchu?";
  cout << " The cost per person is: $" << MACHUP_TOUR << endl;
  cin >> reservation.tourPeople;

  while (reservation.tourPeople > reservation.numberPeople) // Verifies that the number of people taking the tour isn't greater than the people traveling
  {
    cout << "Error. The number of people going on tour cannot be greater than the people traveling. Please try again." << endl;
    cin >> reservation.tourPeople;
    cout << "\n";
  }
  cout << "Will you/your group need a guide? (Y/N).";
  cout << " The cost per person is: $" << MACHUP_GUIDE << endl;
  cin >> guide;

  yesNo(guide, reservation.tourPeople, reservation.totalGuide, MACHUP_GUIDE); // Calculates guide costs if answer is "Y", if "N" is 0

  // Calculate base charges.
  reservation.baseCharges = (reservation.numberPeople * MEXICO_RATE) + (reservation.tourPeople * MACHUP_TOUR);
  reservation.charges = reservation.baseCharges;

  // Calculate 10% discount for 5 or more.
  if (reservation.numberPeople > 4)
  {
    reservation.discount = reservation.charges * .1;
    reservation.charges -= reservation.discount;
  }

  reservation.charges += reservation.totalGuide;
  // Calculate required deposit.
  reservation.deposit = reservation.charges / 2.0;

  reservation.confirmationCode = confirmationNumb(reservation.confirmationCode); // Assigns random number to confirmation number into Reservation struct
  lastReservation[reservationCount] = reservation;                               // Assigns the reservation variable from Reservation struct into the array
  reservationCount++;                                                            // Adds one to reservation count after creating one reservation

  // Outputs breakdown of costs and number of people traveling
  cout << setw(10) << "-----------------------------------------------------------" << endl;
  cout << setw(10) << "Your confirmation code is:                 \t\t " << confirmationNumb(reservation.confirmationCode);
  cout << setw(10) << " (Please save this code to access to your reservation)" << endl;
  cout << setw(10) << "Name:                                      \t\t" << reservation.name << endl;
  cout << setw(10) << "The date for the reservation is:           \t\t" << reservation.month << "/" << reservation.day << "/" << reservation.year << endl;
  cout << setw(10) << "Total of people traveling:                 \t\t" << reservation.numberPeople << endl;
  cout << setw(10) << "Total of people taking the tour:           \t\t" << reservation.tourPeople << endl;
  cout << setw(10) << "The base charges are:                     \t\t$" << reservation.baseCharges << endl;
  cout << setw(10) << "Guide cost:                               \t\t$" << reservation.totalGuide << endl;
  cout << setw(10) << "The discount for groups of 5 or more is:  \t\t$" << reservation.discount << endl;
  cout << setw(10) << "The total charges are:                    \t\t$" << reservation.charges << endl;
  cout << setw(10) << "The deposit required is:                  \t\t$" << reservation.deposit << endl;

  cout << "Note: If you cancel your reservation, the deposit would not be reimbrused" << endl;

  // Saves the reservation into the user's file as an array
  for (int i = reservationCount - 1; i < reservationCount; i++)
  {
    Reservation &r = lastReservation[i];
    outputFile << setw(10) << endl
               << "Confirmation Code:                              \t\t" << confirmationNumb(r.confirmationCode) << endl;
    outputFile << setw(10) << "Name:                                           \t\t" << r.name << endl;
    outputFile << setw(10) << "Location:                                       \t\t" << r.location << endl;
    outputFile << setw(10) << "Date:                                           \t\t" << r.month << "/" << r.day << "/" << r.year << endl;
    outputFile << setw(10) << "Total of people traveling:                      \t\t" << r.numberPeople << endl;
    outputFile << setw(10) << "Total of people taking the tour:                \t\t" << r.tourPeople << endl;
    outputFile << setw(10) << "The base charges are:                            \t\t$" << r.baseCharges << endl;
    outputFile << setw(10) << "Guide cost:                                      \t\t$" << r.totalGuide << endl;
    outputFile << setw(10) << "The discount for groups of 5 or more is:         \t\t$" << r.discount << endl;
    outputFile << setw(10) << "The total charges are:                           \t\t$" << r.charges << endl;
    outputFile << setw(10) << "The deposit required is:                         \t\t$" << r.deposit << endl;
    outputFile << "-----------------------------" << endl;
  }
  outputFile.close();

  finishMenu(username, lastReservation, reservationCount); // Redirects the user to the finish menu after creating the reservation
}

/****************************************************************************************
Definition of France function to get the user's information when traveling to Paris.
Asks the user a series of questions and then displays a breakdown of all the charges.
*****************************************************************************************/
void France(string username)
{
  Reservation reservation; // Reservation struct
  char guide;
  string name; // Name for reservation
  reservation.location = "Paris";
  int confirmationCode = 0;
  int month, day, year; // Date for reservation
  int numberPeople;     // Number in party
  int tourPeople;       // Number for tour
  double totalGuide;    // Cost of guide
  double baseCharges;   // Base charges
  double charges;       // Total charges
  double discount = 0;  // Discount
  double deposit;       // Required deposit

  ofstream outputFile; // Opens user's file to write the reservation info
  outputFile.open(username + ".txt", ios::app);

  if (!outputFile.is_open())
  {
    cout << "Error opening file." << endl;
    return;
  }

  cout << "\t\tYou have selected Paris!" << endl;
  cout << "Enter the date for the reservation (MM/DD/YYYY): " << endl;
  cin >> reservation.month;
  cin.get();
  cin >> reservation.day;
  cin.get();
  cin >> reservation.year;
  cin.ignore();
  cout << "\n\n";

  verifyDate(reservation.month, reservation.day, reservation.year);

  cout << "Enter the name for the reservation: ";
  getline(cin, reservation.name);
  cout << "\n\n";

  cout << "To travel to Paris the price per person is: $" << PARIS_RATE << endl;
  cout << "How many people will be traveling?" << endl;
  cin >> reservation.numberPeople;
  cout << "How many people will be taking the tour through the Eiffel Tower?";
  cout << " The cost per person is: $" << EIFFEL_TOUR << endl;
  cin >> reservation.tourPeople;

  while (reservation.tourPeople > reservation.numberPeople) // Verifies that the number of people taking the tour isn't greater than the people traveling
  {
    cout << "Error. The number of people going on tour cannot be greater than the people traveling. Please try again." << endl;
    cin >> reservation.tourPeople;
    cout << "\n";
  }
  cout << "Will you/your group need a guide? (Y/N).";
  cout << " The cost per person is: $" << EIFFEL_GUIDE << endl;
  cin >> guide;

  yesNo(guide, reservation.tourPeople, reservation.totalGuide, EIFFEL_GUIDE); // Calculates guide costs if answer is "Y", if "N" is 0

  // Calculate base charges.
  reservation.baseCharges = (reservation.numberPeople * PARIS_RATE) + (reservation.tourPeople * EIFFEL_TOUR);
  reservation.charges = reservation.baseCharges;

  // Calculate 10% discount for 5 or more.
  if (reservation.numberPeople > 4)
  {
    reservation.discount = (reservation.charges * .1);
    reservation.charges -= reservation.discount;
  }

  reservation.charges += reservation.totalGuide;
  // Calculate required deposit.
  reservation.deposit = reservation.charges / 2.0;

  reservation.confirmationCode = confirmationNumb(reservation.confirmationCode); // Assigns random number to confirmation number into Reservation struct
  lastReservation[reservationCount] = reservation;                               // Assigns the reservation variable from Reservation struct into the array
  reservationCount++;                                                            // Adds one to reservation count after creating one reservation

  // Outputs breakdown of costs and number of people traveling
  cout << setw(10) << "-----------------------------------------------------------" << endl;
  cout << setw(10) << "Your confirmation code is:                 \t\t " << confirmationNumb(reservation.confirmationCode);
  cout << setw(10) << " (Please save this code to access to your reservation)" << endl;
  cout << setw(10) << "Name:                                      \t\t" << reservation.name << endl;
  cout << setw(10) << "The date for the reservation is:           \t\t" << reservation.month << "/" << reservation.day << "/" << reservation.year << endl;
  cout << setw(10) << "Total of people traveling:                 \t\t" << reservation.numberPeople << endl;
  cout << setw(10) << "Total of people taking the tour:           \t\t" << reservation.tourPeople << endl;
  cout << setw(10) << "The base charges are:                     \t\t$" << reservation.baseCharges << endl;
  cout << setw(10) << "Guide cost:                               \t\t$" << reservation.totalGuide << endl;
  cout << setw(10) << "The discount for groups of 5 or more is:  \t\t$" << reservation.discount << endl;
  cout << setw(10) << "The total charges are:                    \t\t$" << reservation.charges << endl;
  cout << setw(10) << "The deposit required is:                  \t\t$" << reservation.deposit << endl;

  cout << "Note: If you cancel your reservation, the deposit would not be reimbrused" << endl;

  // Saves the reservation into the user's file as an array
  for (int i = reservationCount - 1; i < reservationCount; i++)
  {
    Reservation &r = lastReservation[i];
    outputFile << setw(10) << endl
               << "Confirmation Code:                              \t\t" << confirmationNumb(r.confirmationCode) << endl;
    outputFile << setw(10) << "Name:                                           \t\t" << r.name << endl;
    outputFile << setw(10) << "Location:                                       \t\t" << r.location << endl;
    outputFile << setw(10) << "Date:                                           \t\t" << r.month << "/" << r.day << "/" << r.year << endl;
    outputFile << setw(10) << "Total of people traveling:                      \t\t" << r.numberPeople << endl;
    outputFile << setw(10) << "Total of people taking the tour:                \t\t" << r.tourPeople << endl;
    outputFile << setw(10) << "The base charges are:                           \t\t$" << r.baseCharges << endl;
    outputFile << setw(10) << "Guide cost:                                     \t\t$" << r.totalGuide << endl;
    outputFile << setw(10) << "The discount for groups of 5 or more is:        \t\t$" << r.discount << endl;
    outputFile << setw(10) << "The total charges are:                          \t\t$" << r.charges << endl;
    outputFile << setw(10) << "The deposit required is:                        \t\t$" << r.deposit << endl;
    outputFile << "-----------------------------" << endl;
  }
  outputFile.close();

  finishMenu(username, lastReservation, reservationCount); // Redirects the user to the finish menu after creating the reservation
}

/****************************************************************************************
Definition of Japan function to get the user's information when traveling to Tokyo.
Asks the user a series of questions and then displays a breakdown of all the charges.
*****************************************************************************************/
void Japan(string username)
{
  Reservation reservation; // Reservation struct
  char instructor;
  string name; // Name for reservation
  reservation.location = "Tokyo";
  int confirmationCode = 0;
  int numbRental;         // Number renting equipment
  int totalRental;        // Cost for equipment
  int month, day, year;   // Date for reservation
  int numberPeople;       // Number in party
  int climbPeople;        // Number climbing
  double totalInstructor; // Cost of instructor
  double baseCharges;     // Base charges
  double charges;         // Total charges
  double discount = 0;    // Discount
  double deposit;         // Required deposit

  cout << "\t\tYou have selected Tokyo!" << endl;

  cout << "Enter the date for the reservation (MM/DD/YYYY): " << endl;
  cin >> reservation.month;
  cin.get();
  cin >> reservation.day;
  cin.get();
  cin >> reservation.year;
  cin.ignore();
  cout << "\n";

  verifyDate(reservation.month, reservation.day, reservation.year);

  cout << "Enter the name for the reservation: ";
  getline(cin, reservation.name);
  cout << "\n";

  cout << "To travel to Tokyo the price per person is: $" << TOKYO_RATE << endl;
  cout << "How many people will be traveling?" << endl;
  cin >> reservation.numberPeople;
  cout << "\n";

  cout << "How many people will be be attempting to climb the Mount Fuji?";
  cout << " The cost per person is: $" << FUJI_CLIMB << endl;
  cin >> reservation.climbPeople;
  cout << "\n";

  while (reservation.climbPeople > reservation.numberPeople) // Verifies that the number of people climbing isn't greater than the people traveling
  {
    cout << "Error. The number of people going on tour cannot be greater than the people traveling. Please try again." << endl;
    cin >> reservation.climbPeople;
    cout << "\n";
  }

  cout << "Will you/your group need an instructor? (Y/N). \n";
  cout << " The cost per person is: $" << FUJI_INSTRUCT << endl;
  cin >> instructor;
  cout << "\n";

  yesNo(instructor, reservation.climbPeople, reservation.totalInstructor, FUJI_INSTRUCT); // Calculates instructor costs if answer is "Y", if "N" is 0

  cout << "How many will need climbing equipment? ";
  cout << "The cost per person is: $" << FUJI_RENTAL << endl;
  cin >> numbRental;
  cout << "\n";

  while (numbRental > reservation.climbPeople) // Verifies that the number of people renting equipment isn't greater than the people climbing
  {
    cout << "Error. The amount of equipment requested cannot be greater than the people traveling. Please try again." << endl;
    cin >> numbRental;
    cout << "\n";
  }

  reservation.totalRental = numbRental * FUJI_RENTAL;
  // Calculate base charges.
  reservation.baseCharges = (reservation.numberPeople * TOKYO_RATE) + (reservation.climbPeople * FUJI_CLIMB);
  reservation.charges = reservation.baseCharges;

  // Calculate 10% discount for 5 or more.
  if (reservation.numberPeople > 4)
  {
    reservation.discount = (reservation.charges * .1);
    reservation.charges -= reservation.discount;
  }

  // Adds cost of equipment and instructor
  reservation.charges += reservation.totalInstructor + reservation.totalRental;
  // Calculate required deposit.
  reservation.deposit = reservation.charges / 2.0;

  reservation.confirmationCode = confirmationNumb(reservation.confirmationCode); // Assigns random number to confirmation number into Reservation struct
  lastReservation[reservationCount] = reservation;                               // Assigns the reservation variable from Reservation struct into the array
  reservationCount++;                                                            // Adds one to reservation count after creating one reservation

  // Outputs breakdown of costs and number of people traveling
  cout << setw(10) << "-----------------------------------------------------------" << endl;
  cout << setw(10) << "Your confirmation code is:                 \t\t " << confirmationNumb(reservation.confirmationCode);
  cout << setw(10) << " (Please save this code to access to your reservation)" << endl;
  cout << setw(10) << "Name:                                      \t\t" << reservation.name << endl;
  cout << setw(10) << "The date for the reservation is:           \t\t" << reservation.month << "/" << reservation.day << "/" << reservation.year << endl;
  cout << setw(10) << "Total of people traveling:                 \t\t" << reservation.numberPeople << endl;
  cout << setw(10) << "Total of people climbing:                  \t\t" << reservation.climbPeople << endl;
  cout << setw(10) << "The base charges are:                     \t\t$" << reservation.baseCharges << endl;
  cout << setw(10) << "Instructor cost:                          \t\t$" << reservation.totalInstructor << endl;
  cout << setw(10) << "Equipment cost:                           \t\t$" << reservation.totalRental << endl;
  cout << setw(10) << "The discount for groups of 5 or more is:  \t\t$" << reservation.discount << endl;
  cout << setw(10) << "The total charges are:                    \t\t$" << reservation.charges << endl;
  cout << setw(10) << "The deposit required is:                  \t\t$" << reservation.deposit << endl;

  cout << "Note: If you cancel your reservation, the deposit would not be reimbrused" << endl;

  ofstream outputFile(username + ".txt", ios::app); // Opens user's file to write the reservation info

  if (!outputFile.is_open())
  {
    cout << "Error opening file." << endl;
    return;
  }

  // Saves the reservation into the user's file as an array
  for (int i = reservationCount - 1; i < reservationCount; i++)
  {
    Reservation &r = lastReservation[i];
    outputFile << setw(10) << endl
               << "Confirmation Code:                              \t\t" << confirmationNumb(r.confirmationCode) << endl;
    outputFile << setw(10) << "Name:                                           \t\t" << r.name << endl;
    outputFile << setw(10) << "Location:                                       \t\t" << r.location << endl;
    outputFile << setw(10) << "Date:                                           \t\t" << r.month << "/" << r.day << "/" << r.year << endl;
    outputFile << setw(10) << "Total of people traveling:                      \t\t" << r.numberPeople << endl;
    outputFile << setw(10) << "Total of people climbing:                       \t\t" << r.climbPeople << endl;
    outputFile << setw(10) << "The base charges are:                           \t\t$" << r.baseCharges << endl;
    outputFile << setw(10) << "Instructor cost:                                \t\t$" << r.totalInstructor << endl;
    outputFile << setw(10) << "Equipment cost:                                 \t\t$" << r.totalRental << endl;
    outputFile << setw(10) << "The discount for groups of 5 or more is:        \t\t$" << r.discount << endl;
    outputFile << setw(10) << "The total charges are:                          \t\t$" << r.charges << endl;
    outputFile << setw(10) << "The deposit required is:                        \t\t$" << r.deposit << endl;

    outputFile << "-----------------------------" << endl;
  }
  outputFile.close();

  finishMenu(username, lastReservation, reservationCount); // Redirects the user to the finish menu after creating the reservation
}

/****************************************************************************************
Definition of Australia function to get the user's information when traveling to Vienna.
Asks the user a series of questions and then displays a breakdown of all the charges.
****************************************************************************************/
void Australia(string username)
{
  Reservation reservation; // Reservation struct
  char guide;
  string name; // Name for reservation
  reservation.location = "Vienna";
  int confirmationCode = 0;
  int month, day, year; // Date for reservation
  int numberPeople;     // Number in party
  int tourPeople;       // Number for tour
  double totalGuide;    // Cost of guide
  double baseCharges;   // Base charges
  double charges;       // Total charges
  double discount = 0;  // Discount
  double deposit;       // Required deposit

  ofstream outputFile; // Opens user's file to write the reservation info
  outputFile.open(username + ".txt", ios::app);

  if (!outputFile.is_open())
  {
    cout << "Error opening file." << endl;
    return;
  }

  cout << "\t\tYou have selected Vienna!" << endl;
  cout << "Enter the date for the reservation (MM/DD/YYYY): " << endl;
  cin >> reservation.month;
  cin.get();
  cin >> reservation.day;
  cin.get();
  cin >> reservation.year;
  cin.ignore();
  cout << "\n\n";

  verifyDate(reservation.month, reservation.day, reservation.year);

  cout << "Enter the name for the reservation: ";
  getline(cin, reservation.name);
  cout << "\n\n";

  cout << "To travel to Vienna the price per person is: $" << VIENNA_RATE << endl;
  cout << "How many people will be traveling?" << endl;
  cin >> reservation.numberPeople;
  cout << "How many people will be taking the tour through the city?";
  cout << " The cost per person is: $" << VIENNA_TOUR << endl;
  cin >> reservation.tourPeople;

  while (reservation.tourPeople > reservation.numberPeople) // Verifies that the number of people taking the tour isn't greater than the people traveling
  {
    cout << "Error. The number of people going on tour cannot be greater than the people traveling. Please try again." << endl;
    cin >> reservation.tourPeople;
    cout << "\n";
  }
  cout << "Will you/your group need a guide? (Y/N).";
  cout << " The cost per person is: $" << VIENNA_GUIDE << endl;
  cin >> guide;

  yesNo(guide, reservation.tourPeople, reservation.totalGuide, VIENNA_GUIDE); // Calculates guide costs if answer is "Y", if "N" is 0

  // Calculate base charges.
  reservation.baseCharges = (reservation.numberPeople * VIENNA_RATE) + (reservation.tourPeople * VIENNA_TOUR);
  reservation.charges = reservation.baseCharges;

  // Calculate 10% discount for 5 or more.
  if (reservation.numberPeople > 4)
  {
    reservation.discount = (reservation.charges * .1);
    reservation.charges -= reservation.discount;
  }

  reservation.charges += reservation.totalGuide;
  // Calculate required deposit.
  reservation.deposit = reservation.charges / 2.0;

  reservation.confirmationCode = confirmationNumb(reservation.confirmationCode); // Assigns random number to confirmation number into Reservation struct
  lastReservation[reservationCount] = reservation;                               // Assigns the reservation variable from Reservation struct into the array
  reservationCount++;                                                            // Adds one to reservation count after creating one reservation

  // Outputs breakdown of costs and number of people traveling
  cout << setw(10) << "-----------------------------------------------------------" << endl;
  cout << setw(10) << "Your confirmation code is:                 \t\t " << confirmationNumb(reservation.confirmationCode);
  cout << setw(10) << " (Please save this code to access to your reservation)" << endl;
  cout << setw(10) << "Name:                                      \t\t" << reservation.name << endl;
  cout << setw(10) << "The date for the reservation is:           \t\t" << reservation.month << "/" << reservation.day << "/" << reservation.year << endl;
  cout << setw(10) << "Total of people traveling:                 \t\t" << reservation.numberPeople << endl;
  cout << setw(10) << "Total of people taking the tour:           \t\t" << reservation.tourPeople << endl;
  cout << setw(10) << "The base charges are:                     \t\t$" << reservation.baseCharges << endl;
  cout << setw(10) << "Guide cost:                               \t\t$" << reservation.totalGuide << endl;
  cout << setw(10) << "The discount for groups of 5 or more is:  \t\t$" << reservation.discount << endl;
  cout << setw(10) << "The total charges are:                    \t\t$" << reservation.charges << endl;
  cout << setw(10) << "The deposit required is:                  \t\t$" << reservation.deposit << endl;

  cout << "Note: If you cancel your reservation, the deposit would not be reimbrused" << endl;

  // Saves the reservation into the user's file as an array
  for (int i = reservationCount - 1; i < reservationCount; i++)
  {
    Reservation &r = lastReservation[i];
    outputFile << setw(10) << endl
               << "Confirmation Code:                              \t\t" << confirmationNumb(r.confirmationCode) << endl;
    outputFile << setw(10) << "Name:                                           \t\t" << r.name << endl;
    outputFile << setw(10) << "Location:                                       \t\t" << r.location << endl;
    outputFile << setw(10) << "Date:                                           \t\t" << r.month << "/" << r.day << "/" << r.year << endl;
    outputFile << setw(10) << "Total of people traveling:                      \t\t" << r.numberPeople << endl;
    outputFile << setw(10) << "Total of people taking the tour:                \t\t" << r.tourPeople << endl;
    outputFile << setw(10) << "The base charges are:                           \t\t$" << r.baseCharges << endl;
    outputFile << setw(10) << "Guide cost:                                     \t\t$" << r.totalGuide << endl;
    outputFile << setw(10) << "The discount for groups of 5 or more is:        \t\t$" << r.discount << endl;
    outputFile << setw(10) << "The total charges are:                          \t\t$" << r.charges << endl;
    outputFile << setw(10) << "The deposit required is:                        \t\t$" << r.deposit << endl;
    outputFile << "-----------------------------" << endl;
  }
  outputFile.close();

  finishMenu(username, lastReservation, reservationCount); // Redirects the user to the finish menu after creating the reservation
}

/****************************************************************************************
  Definition of Brazil function to get the user's information when traveling to Rio.
Asks the user a series of questions and then displays a breakdown of all the charges.
*****************************************************************************************/
void Brazil(string username)
{
  Reservation reservation; // Reservation struct
  char guide;
  string name; // Name for reservation
  reservation.location = "Rio";
  int confirmationCode = 0;
  int month, day, year; // Date for reservation
  int numberPeople;     // Number in party
  int carnivalPeople;   // Number tickets
  double baseCharges;   // Base charges
  double charges;       // Total charges
  double discount = 0;  // Discount
  double deposit;       // Required deposit

  ofstream outputFile; // Opens user's file to write the reservation info
  outputFile.open(username + ".txt", ios::app);

  if (!outputFile.is_open())
  {
    cout << "Error opening file." << endl;
    return;
  }

  cout << "\t\tYou have selected Rio!" << endl;
  cout << "Enter the date for the reservation (MM/DD/YYYY): " << endl;
  cin >> reservation.month;
  cin.get();
  cin >> reservation.day;
  cin.get();
  cin >> reservation.year;
  cin.ignore();
  cout << "\n\n";

  verifyDate(reservation.month, reservation.day, reservation.year);

  cout << "Enter the name for the reservation: ";
  getline(cin, reservation.name);
  cout << "\n\n";

  cout << "To travel to Vienna the price per person is: $" << RIO_RATE << endl;
  cout << "How many people will be traveling?" << endl;
  cin >> reservation.numberPeople;
  cout << "How many people will be taking buying passes to the Carnival?";
  cout << " The cost per person is: $" << CARNIVAL_PASS << endl;
  cin >> reservation.carnivalPeople;

  while (reservation.carnivalPeople > reservation.numberPeople) // Verifies that the number of people buying carnival tickets isn't greater than the people traveling
  {
    cout << "Error. The number of people buying the passes cannot be greater than the people traveling. Please try again." << endl;
    cin >> reservation.carnivalPeople;
    cout << "\n";
  }

  // Calculate base charges.
  reservation.baseCharges = (reservation.numberPeople * RIO_RATE) + (reservation.carnivalPeople * CARNIVAL_PASS);
  reservation.charges = reservation.baseCharges;

  // Calculate 10% discount for 5 or more.
  if (reservation.numberPeople > 4)
  {
    reservation.discount = (reservation.charges * .1);
    reservation.charges -= reservation.discount;
  }

  reservation.charges += reservation.totalGuide;
  // Calculate required deposit.
  reservation.deposit = reservation.charges / 2.0;

  reservation.confirmationCode = confirmationNumb(reservation.confirmationCode); // Assigns random number to confirmation number into Reservation struct
  lastReservation[reservationCount] = reservation;                               // Assigns the reservation variable from Reservation struct into the array
  reservationCount++;                                                            // Adds one to reservation count after creating one reservation

  // Outputs breakdown of costs and number of people traveling
  cout << setw(10) << "-----------------------------------------------------------" << endl;
  cout << setw(10) << "Your confirmation code is:                 \t\t " << confirmationNumb(reservation.confirmationCode);
  cout << setw(10) << " (Please save this code to access to your reservation)" << endl;
  cout << setw(10) << "Name:                                      \t\t" << reservation.name << endl;
  cout << setw(10) << "The date for the reservation is:           \t\t" << reservation.month << "/" << reservation.day << "/" << reservation.year << endl;
  cout << setw(10) << "Total of people traveling:                 \t\t" << reservation.numberPeople << endl;
  cout << setw(10) << "Total of people buyig carnival passes:     \t\t" << reservation.carnivalPeople << endl;
  cout << setw(10) << "The base charges are:                     \t\t$" << reservation.baseCharges << endl;
  cout << setw(10) << "The discount for groups of 5 or more is:  \t\t$" << reservation.discount << endl;
  cout << setw(10) << "The total charges are:                    \t\t$" << reservation.charges << endl;
  cout << setw(10) << "The deposit required is:                  \t\t$" << reservation.deposit << endl;

  cout << "Note: If you cancel your reservation, the deposit would not be reimbrused" << endl;

  // Saves the reservation into the user's file as an array
  for (int i = reservationCount - 1; i < reservationCount; i++)
  {
    Reservation &r = lastReservation[i];
    outputFile << setw(10) << endl
               << "Confirmation Code:                              \t\t" << confirmationNumb(r.confirmationCode) << endl;
    outputFile << setw(10) << "Name:                                           \t\t" << r.name << endl;
    outputFile << setw(10) << "Location:                                       \t\t" << r.location << endl;
    outputFile << setw(10) << "Date:                                           \t\t" << r.month << "/" << r.day << "/" << r.year << endl;
    outputFile << setw(10) << "Total of people traveling:                      \t\t" << r.numberPeople << endl;
    outputFile << setw(10) << "Total of people buying carnival passes:         \t\t" << r.carnivalPeople << endl;
    outputFile << setw(10) << "The base charges are:                           \t\t$" << r.baseCharges << endl;
    outputFile << setw(10) << "The discount for groups of 5 or more is:        \t\t$" << r.discount << endl;
    outputFile << setw(10) << "The total charges are:                          \t\t$" << r.charges << endl;
    outputFile << setw(10) << "The deposit required is:                        \t\t$" << r.deposit << endl;
    outputFile << setw(10) << "-----------------------------" << endl;
  }
  outputFile.close();

  finishMenu(username, lastReservation, reservationCount); // Redirects the user to the finish menu after creating the reservation
}

/*************************************************************************************************
    Definition of finish menu to get the user's input after fnishing creating a reservation.
Displays a menu and asks the user to select an option. Redirects the user to the option chosen
**************************************************************************************************/
int finishMenu(string username, Reservation lastReservation[], int reservationCount)
{
  cout << "\n\n";
  int finish;
  int i = 0;

  cout << "Your reservation has been saved. Thank you for choosing World Travel Agency. \n";
  cout << "Select what would you like to do next." << endl;
  cout << "1. Create another reservation" << endl;
  cout << "2. Access reservations" << endl;
  cout << "3. Log out" << endl;
  cout << "Enter 1, 2, or 3: ";
  cin >> finish;
  cout << "\n";

  if (finish < 1 || finish > 3) // Validate input and exits if 3 wrong attempts are made
  {
    for (i = 0; i < 3; i++)
    {
      cout << "Invalid Selection. Enter 1, 2, 3 or 4: ";
      cin >> finish;
      if (finish >= 1 && finish <= 3) // Exit the loop if input is within valid range
      {
        break;
      }
    }
    i++;
    if (i == 4)
    {
      cout << "Maximum number of tries exceeded. Exiting program." << endl;
      exit(0);
    }
  }
  // Handle the user's choice when entering the finish menu
  do
  {
    switch (finish)
    {
    case 1:
      travelMenu(username, lastReservation, reservationCount);
      break;
    case 2:
      displayReservation(username, lastReservation, reservationCount);
      break;
    case 3:
      logout();
      break;
    }
  } while (finish != 3);
  return 0;
}

/*************************************************************************************************
                          Definition of Display Reservation function.
It asks the user for name and confirmation code on the reservation, then validates the input
and shows the breakdown of the matching reservation
**************************************************************************************************/
void displayReservation(string username, Reservation lastReservation[], int reservationCount)
{
  ifstream inputFile; // Opens user's file to read the reservation info
  inputFile.open(username + ".txt");

  if (!inputFile.is_open())
  {
    cout << "Error opening file." << endl;
    return;
  }

  string line;
  string name;
  int confirmationCode;
  bool reservationFound = false;

  cin.ignore();

  cout << "Please enter the name on the reservation: ";
  getline(cin, name);
  cout << endl;
  cout << "Please enter the confirmation code: ";
  cin >> confirmationCode;
  cout << endl;

  while (getline(inputFile, line))
  {
    if (line.find("Confirmation Code:") != string::npos && line.find(to_string(confirmationCode)) != string::npos) // Reads the confirmation number and if matches to a reservation proceeds to verify the name
    {
      getline(inputFile, line);                                                  // Read the Name line
      if (line.find("Name:") != string::npos && line.find(name) != string::npos) // Verifies the name on the reservation, if it also matches, displays the reservation info
      {
        reservationFound = true;
        cout << "Reservation found:" << endl;
        cout << "-----------------------------" << endl;
        cout << "Confirmation Code:                              \t\t" << confirmationCode << endl;
        cout << line << endl; // Name
        for (int i = 0; i < 10; i++)
        { // Output the remaining reservation info
          getline(inputFile, line);
          cout << line << endl;
        }
        break;
      }
    }
  }
  if (reservationFound)
  {
    finishMenu(username, lastReservation, reservationCount);
  }

  if (!reservationFound) // If reservation is not found redirects the user to the function to try again
  {
    cout << "Reservation not found. Please try again." << endl
         << endl;
    displayReservation(username, lastReservation, reservationCount);
  }
  inputFile.close();
}
