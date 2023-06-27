// Imports necessary libraries
#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Reservation.hpp"
#include "Schedule.hpp"
#include "System.hpp"
#include "User.hpp"

using namespace std;
using namespace chrono;

// Main function
int main() {

  // Get the current system time
  system_clock::time_point now = system_clock::now();

  // Convert the system time to a time_t object
  time_t currentTime = system_clock::to_time_t(now);

  // Convert the time_t object to a local time
  tm *localTime = localtime(&currentTime);

  // Extract individual components from the local time
  int year = localTime->tm_year + 1900; // Year since 1900
  int month = localTime->tm_mon + 1;    // Month (0-11)
  int day = localTime->tm_mday;         // Day of the month (1-31)
  int hour = localTime->tm_hour;        // Hour of the day (0-23)
  int minute = localTime->tm_min;       // Minute (0-59)
  int second = localTime->tm_sec;       // Second (0-59)

  // Get the day of the week (0-6, Sunday = 0)
  int dayOfWeek = localTime->tm_wday;

  // Array of day names
  const char *dayNames[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                            "Thursday", "Friday", "Saturday"};

  // Print the current date and time
  cout << "Current Date and Time: " << endl;
  cout << "Year: " << year << endl;
  cout << "Month: " << month << endl;
  cout << "Day: " << day << endl;
  cout << "Hour: " << hour << endl;
  cout << "Minute: " << minute << endl;
  cout << "Second: " << second << endl;

  System MainSys;
  Schedule MainSched;
  string user_obj;
  int user_count = 0;
  vector<User *> users;

  // Create initial users
  ifstream infile("users.txt");
  string line;

  // Reads in file line by line
  while (getline(infile, line)) {
    istringstream ss(line);
    string word;
    char token = ' ';
    vector<string> user_pass;
    char type;
    while (getline(ss, word, token)) {

      // Check if word is user and password
      if (word.length() > 1) {

        // Split into user than password
        char delim = ',';
        size_t start = 0;
        size_t end = word.find(delim);

        while (end != string::npos) {
          user_pass.push_back(word.substr(start, end - start));
          start = end + 1;
          end = word.find(delim, start);
        }

        user_pass.push_back(word.substr(start));
      } else {

        // Get type of user
        type = word[0];
      }
    }
    user_obj = "User" + to_string(user_count);

    // Create objects by type
    if (type == 'M') {
      Member *user_obj = new Member(user_pass[0], user_pass[1], 'C', 'M');
      users.push_back(user_obj);
    } else if (type == 'O') {
      Officer *user_obj = new Officer(user_pass[0], user_pass[1], 'A', 'O');
      users.push_back(user_obj);
    } else if (type == 'C') {
      Coach *user_obj = new Coach(user_pass[0], user_pass[1], 'C');
      users.push_back(user_obj);
    }
    user_count += 1;
  }
  infile.close();

  // Creates vector of reservations
  vector<Reservation *> reservations;
  int res_count = 0;
  ifstream infile2("reservations.txt");
  string line2;
  while (getline(infile2, line2)) {
    vector<string> tokens;
    size_t pos = 0;
    while ((pos = line2.find(",")) != string::npos) {
      string token = line2.substr(0, pos);
      tokens.push_back(token);
      line2.erase(0, pos + 1);
    }

    // Add the last token
    tokens.push_back(line2);
    res_count += 1;
    string new_res = "Res" + to_string(res_count);
    for (const auto &user : users) {
      if (user->get_user() == tokens[4]) {
        Reservation *new_res = new Reservation(
            *user, tokens[0], {tokens[1], tokens[2]}, stoi(tokens[3]));
        reservations.push_back(new_res);
      }
    }
  }

  // Run main system
  bool exit = false;
  while (exit == false) {

    // Create a new login and user
    vector<string> login = MainSys.start_program();

    // If user is new create one
    if (login.size() == 3) {
      user_count += 1;
      user_obj = "User" + to_string(user_count);
      if (login[2][0] == 'M') {
        Member *user_obj = new Member(login[0], login[1], 'C', 'M');
        users.push_back(user_obj);
        MainSys.member_menu(user_obj, MainSched, reservations, month, day);
      } else if (login[2][0] == 'O') {
        Officer *user_obj = new Officer(login[0], login[1], 'A', 'O');
        users.push_back(user_obj);
        MainSys.officer_menu(user_obj, MainSched, reservations, month, day);
      } else if (login[2][0] == 'C') {
        Coach *user_obj = new Coach(login[0], login[1], 'C');
        users.push_back(user_obj);
        MainSys.coach_menu(user_obj, MainSched, reservations, month, day,
                           dayOfWeek);
      }
    } else {

      // Do it for existing users
      for (const auto &user : users) {
        if (user->get_user() == login[0]) {
          char type = user->get_type();
          if (type == 'M')
            MainSys.member_menu(user, MainSched, reservations, month, day);
          else if (type == 'O')
            MainSys.officer_menu(user, MainSched, reservations, month, day);
          else if (type == 'C')
            MainSys.coach_menu(user, MainSched, reservations, month, day,
                               dayOfWeek);
        }
      }
    }

    // Prompts the user to exit the program or continue
    int choice;
    cout << "Would you like to exit the program? (1 for yes, 2 for no)" << endl;
    cin >> choice;
    if (choice == 1) {
      exit = true;
    }
  }
  MainSys.end_program();

  // Deallocate dynamically allocated objects
  for (const auto &user : users) {
    delete user;
  }
  users.clear();
  for (const auto &reservation : reservations) {
    delete reservation;
  }
  reservations.clear();

  return EXIT_SUCCESS;
}