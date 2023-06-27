// Imports necessary libraries
#include <vector>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <typeinfo>
//#include "Reservation.hpp"

using namespace std;

// Creates Schedule class
class Schedule {

  // No private or public attributes
  private:
  public:

    // Main constructor
    Schedule() {}

    // Copy constructor
    Schedule(const Schedule& other) {}

    // Copy assignment operator
    Schedule& operator=(const Schedule& other) {
      if (this != &other) {
      }
      return *this;
    }

    // Move constructor
    Schedule(Schedule&& other) {}

    // Move assignment operator
    Schedule& operator=(Schedule&& other) {
      if (this != &other) {
        // Move assignment logic here
      }
      return *this;
    }

    // Deconstructor
    ~Schedule() {}

    // Function to display schedule
    void display_schedule() {
      ifstream inputFile("reservations.txt");
      string line;
      if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
        
          // Splits the line by comma
          vector<string> tokens;
          size_t pos = 0;
          while ((pos = line.find(",")) != string::npos) {
            string token = line.substr(0, pos);
            tokens.push_back(token);
            line.erase(0, pos + 1);
          }

          // Add the last token
          tokens.push_back(line);

          // Format and print the items
          cout << "Day:" << tokens[0] << endl;
          cout << "Time:" << tokens[1] << "-" << tokens[2] << endl;
          cout << "Court:" << tokens[3] << endl;
          cout << "User:" << tokens[4] << endl;

          // Prints a separator
          cout << "---------" << endl; 
        }
        inputFile.close();

      } 
      else {
        cout << "Unable to open the file." << endl;
      }
    }

    // UNFINISHED FUNCTION
    vector<Reservation*> make_group_reservation(vector<Reservation*> reservations, User user) {
    
      cout << "What day do you want to make your group reservation for? (please put in this format '6/26')" << endl;
      string m_day;
      cin >> m_day;
      cout << "What court do you want to reserve? (1, 2, or 3)" << endl;
      string court;
      cin >> court;

      // Check if the reservation already exists
      bool reservationExists = false;
      ifstream inputFile("reservations.txt");
      string line;
      if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
          vector<string> tokens;
          size_t pos = 0;
          while ((pos = line.find(",")) != string::npos) {
            string token = line.substr(0, pos);
            tokens.push_back(token);
            line.erase(0, pos + 1);
          }
          tokens.push_back(line);
          if ((m_day == tokens[0]) && (court == tokens[3])) {
            cout << "A reservation already exists for this day and court." << endl;
            reservationExists = true;
            break;
          }
        }
        inputFile.close();
      }

      // If the reservation does not exist, create a new one
      if (!reservationExists) {
        cout << "What start time do you want to make your reservation for? (any time from 18:00 - 21:00)" << endl;
        string startTime;
        cin >> startTime;

        // Check if the start time is within the allowed range (6-9pm)
        string startHour = startTime.substr(0, 2);
        istringstream iss(startHour);

        // Gets the reservation month and day
        string token;
        getline(iss, token, ':');
        int sh = stoi(token);
        cout<<"start: "<<sh<<endl;
        if (sh < 18 || sh >= 21) {
          cout << "Group reservations can only be made from 6pm to 9pm." << endl;
          return reservations;
        }

        string endTime = startTime;
        
        // Increment the minutes by 30 and handle carry-over to hours
        int minutes = stoi(startTime.substr(3, 2)) + 30;
        int hours = sh + minutes / 60;
        minutes %= 60;
        hours %= 24;
        ostringstream oss;  // Create an ostringstream object
        oss << setfill('0') << setw(2) << hours << ":" << setw(2) << minutes;  // Format the endTime string

        endTime = oss.str();  // Get the formatted string from the ostringstream

        ofstream outfile("reservations.txt", ios::app);
        outfile << endl << m_day << "," << startTime << "," << endTime << "," << court << "," << user.get_user();
        outfile.close();

        Reservation* new_reservation = new Reservation(user, m_day, {startTime, endTime}, stoi(court));
        reservations.push_back(new_reservation);
        cout << "Group reservation successfully created." << endl;
      }

      return reservations;
    }

    // Creates a function to make a reservation
    vector<Reservation *> make_reservation(vector<Reservation *> reservations, User user, int month, int day, string username) {
      
      // Gets desired day, timeslot, and court
      cout<<"What day do you want to make your reservation for? (please put in this format '6/26')"<<endl;
      string m_day;
      cin>>m_day;
      
      // Checking for 7 days in advance
      string m_day2 = m_day+ '/';
  
      istringstream iss(m_day2);

      // Gets the reservation month and day
      string token;
      getline(iss, token, '/');
      int cur_month = stoi(token);
      getline(iss, token, '/');
      int cur_day = stoi(token);
      bool under_seven = true;
      int week_counter = 0;

      // Updates boolean to for if the specified date is within the week
      if(cur_month>month){
        int days_left = 31-day;
        if (days_left+cur_day>7) {
          under_seven=false;
        }
      } else {
        if (cur_day-day>7) {
          under_seven=false;
        }
      }

      // Proceeds if the reservation is within the week
      if(under_seven == true){

        // Gets the time and court from the user
        cout<<"What start time do you want to make your reservation for? (any time from 6:00 - 21:00)"<<endl;
        string time;
        cin>>time;
        cout<<"What court do you want to reserve? (1, 2, or 3)"<<endl;
        string court;
        cin>>court;
        bool res = true;
        ifstream inputFile("reservations.txt");
        string line;
        int week_counter = 0;

        // Reads in reservations.txt line by line
        if (inputFile.is_open()) {
          while (getline(inputFile, line)) {
            
            // Split the line by comma
            vector<string> tokens;
            size_t pos = 0;
            string u;
            while ((pos = line.find(",")) != string::npos) {
              string token = line.substr(0, pos);
              tokens.push_back(token);
              line.erase(0, pos + 1);
  
              if (!line.empty()) {
                tokens.push_back(line);
              }
        
              // Extract the username from the last token
              if (!tokens.empty()) {
                size_t lastPos = tokens.back().find_last_of(" \t");
                if (lastPos != string::npos) {
                  u = tokens.back().substr(lastPos + 1);
                } 
                else {
                  u = tokens.back();
                }
              }
            }
            
            string token;
            string r_date = tokens[0]+'/';

            // Splits the reservation date
            istringstream iss(r_date);
            getline(iss, token, '/');
            int r_month = stoi(token);
            getline(iss, token, '/');
            int r_day = stoi(token);

            // Checks if each reservation is within the week
            bool u_seven = true;
            if(cur_month>month){
              int days_left = 31-day;
              if (days_left+r_day>7) {
                u_seven=false;
              }
            }       
            else {
              if (r_day-day>7) {
                u_seven=false;
              }
            }

            // Checks if the reservation is within the week, and if the username matches, then updates the counter accordingly
            if(u_seven == true && username == u){
              week_counter++;
              if(week_counter >= 2){
                cout<<"You already have an hour of bookings this week"<<endl;
                res = false;
              }
              cout<<week_counter<<endl;
            }
            
            // Add the last token 
            tokens.push_back(line); 

            // Checks to see if the slot is open
            if ((m_day == tokens[0]) && (time == tokens[1]) && (court == tokens[3])) {
              cout<<"This reservation already exists."<<endl;
              res = false;
              break;
            }

            // Checks to see if there is already a reservation on the day
            if ((m_day == tokens[0]) && (username == tokens[4])) {
              cout << "You have already made a reservation on this day." <<endl;
              res = false;
              break;
            }    

            // Checks if the reservation is on a day in the future
            if (cur_month <= month) {
              if (cur_day < day) {
                cout << "This day has already passed."<<endl;
                res = false;
                break;
              }
            }
          }
          inputFile.close();

          // If none of the checks fail, proceeds
          if (res == true) {
            istringstream iss(time);
            int hours, minutes;
            char separator;
            if (!(iss >> hours >> separator >> minutes)) {
            cout << "Invalid time format." << endl;
            } else {
            minutes += 30;
              
            // Carry over extra minutes to hours
            hours += minutes / 60;  
    
            // Keep minutes within the 0-59 range
            minutes %= 60;          
            hours %= 24;            
            ostringstream oss;
            oss << hours << ":" << setfill('0') << setw(2) << minutes;
            string time2 = oss.str();
            ofstream outfile("reservations.txt", ios::app);
            outfile<<endl<<m_day<<","<<time<<","<<time2<<","<<court<<","<<user.get_user();
            outfile.close();
            Reservation *new_reservation = new Reservation(user, m_day, {time, time2}, stoi(court));
            reservations.push_back(new_reservation);
            }
          }
        }
        return reservations;
      }

      // If the reservation is over a week away, it is not made
      else{
        cout<<"You cannot reserve more than 7 days in advance"<<endl;
        return reservations;
      }
    }

    // Function to request a cancellation
    void request_cancel_reservation(string username) {

      // Opens file
      ifstream inputFile("reservations.txt");
      if (!inputFile) {
          cout << "Failed to open file." << endl;
          return;
      }
  
      ofstream tempFile("temp.txt");
      if (!tempFile) {
          cout << "Failed to create temporary file." << endl;
          inputFile.close();
          return;
      }

      // Gets day, time, and court from the user
      string line;
      cout << "What day do you want to cancel your reservation? (please put in this format '6/26')" << endl;
      string m_day;
      cin >> m_day;
      cout << "When is the start time? (any time from 8:00 - 21:00)" << endl;
      string time;
      cin >> time;
      cout << "What court did you reserve? (1, 2, or 3)" << endl;
      string court;
      cin >> court;

      // Reads in file line by line
      while (getline(inputFile, line)) {
        vector<string> tokens;
        size_t pos = 0;
        while ((pos = line.find(",")) != string::npos) {
          string token = line.substr(0, pos);
          tokens.push_back(token);
          line.erase(0, pos + 1);
        }
  
        // Add the last token
        tokens.push_back(line); 
  
        if (!(m_day == tokens[0] && time == tokens[1] && court == tokens[3] && username == tokens[4])) {
          tempFile << tokens[0] <<"," <<tokens[1] <<"," <<tokens[2] << "," << tokens[3] << "," << tokens[4] << endl;
        }
      }

      // Closes files
      inputFile.close();
      tempFile.close();

      const char* filename = "reservations.txt";
      if (remove(filename) != 0) {
        cout << "Failed to remove file: " << filename << endl;
      } 
      else {
        if (rename("temp.txt", filename) != 0) {
          cout << "Failed to replace file: " << filename << endl;
        } 
        else {
          cout << "Reservation canceled successfully." << endl;
        }
      }
      
    }
  
    // Shows closest reservation for a user
    void view_my_reservations(string username) {

      // Reads in file line by line
      ifstream inputFile("reservations.txt");
      string line;
      if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
          
          // Split the line by comma
          vector<string> tokens;
          size_t pos = 0;
          while ((pos = line.find(",")) != string::npos) {
            string token = line.substr(0, pos);
            tokens.push_back(token);
            line.erase(0, pos + 1);
          }
  
          // Add the last token
          tokens.push_back(line); 
          if (tokens[4] == username) {
            
            // Format and print the items
            cout << "Day:" << tokens[0] << endl;
            cout << "Time:" << tokens[1] << "-" << tokens[2] << endl;
            cout << "Court:" << tokens[3] << endl;
            cout << "User:" << tokens[4] << endl;
            cout << "---------" << endl; 
          }
        }
        inputFile.close();
  
      } 
      else {
        cout << "Unable to open the file." << endl;
      }
    }
  
    // Creates a function to send a message to an officer
    void message_officer(string username){
      string officer_user;
      string message;
  
      // Gets officer username
      cout<<"What is the username of the officer you would like to message:"<<endl;
      cin>>officer_user;
      cin.ignore();
  
      // Gets the message
      cout<<"What is your message (first, ):"<<endl;
      getline(cin, message);
  
      ofstream outfile("messages.txt", ios::app);
      if (!outfile) {
        cout << "Failed to open messages file." << endl;
      } 
      else {
        outfile<<endl<<officer_user<<"~"<<username<<"~"<<message;
      }
      outfile.close();
    }
  
    // Creates a function for the officer to read messages
    void read_messages(string username) {
      ifstream inputFile("messages.txt");
      string line;
      if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
          
          // Split the line by comma
          vector<string> tokens;
          size_t pos = 0;
          while ((pos = line.find("~")) != string::npos) {
            string token = line.substr(0, pos);
            tokens.push_back(token);
            line.erase(0, pos + 1);
          }
          tokens.push_back(line);
          if (tokens[0] == username){
            cout<<"From: " <<tokens[1]<<endl;
            cout<<tokens[2]<<endl;
          }
        }
        
      }
      inputFile.close();
    }
  
     // Creates a function for a coach to make a reservation
     vector<Reservation *> make_coach_reservation(vector<Reservation *> reservations, User user, int month, int day, int dayOfWeek) {
       
      // Insert desired day, timeslot, and court
      cout<<"What day do you want to make your reservation for? (please put in this format '6/26')"<<endl;
      string m_day;
      cin>>m_day;
  
      // Checking for 7 days in advance
      string m_day2 = m_day+ '/';
  
      istringstream iss(m_day2);
  
      string token;
      getline(iss, token, '/');
      int cur_month = stoi(token);
      getline(iss, token, '/');
      int cur_day = stoi(token);
      bool under_seven = true;
      bool under_two = true;
  
      // Checks if it's under seven
      if(cur_month>month){
        int days_left = 31-day;
        if(days_left+cur_day>7){
          under_seven=false;
        }
      } 
      else{
        if(cur_day-day>7){
          under_seven=false;
        }
      }
  
      int day_dif;
       
      // Checks if it's under two
      if(cur_month>month){
        int days_left = 31-day;
        day_dif = days_left+cur_day;
        if(days_left+cur_day>2){
          under_two=false;
        }
      } 
      else{
        if(cur_day-day>2){
          day_dif = cur_day-day;
          under_two=false;
        }
      }

      // Proceeds if the reservation is within the week
      if(under_seven == true){

        // Gets time and court from the user
        cout<<"What start time do you want to make your reservation for? (any time from 9:00 - 12:00 or 3:00 - 6:00 on weekdays)"<<endl;
        string time;
        cin>>time;
        cout<<"What court do you want to reserve? (1, 2, or 3)"<<endl;
        string court;
        cin>>court;
  
        istringstream iss(time);
        string token;
        getline(iss, token, ':');
        int hour = stoi(token);
        bool time_bool = false;
        dayOfWeek = (dayOfWeek + day_dif)%7;

        // Checks if the reservation is within the hours allowed for coaches to book
        if(((hour>=9 && hour<=12) || (hour>=15 && hour<=18)) && (dayOfWeek > 0 && dayOfWeek < 6)){
          time_bool = true;
        }
        bool res = true;
        if(time_bool == false){
          cout<<"Coaches can only book between 9am and noon, or 3pm and 6pm on weekdays"<<endl;
          res = false;
        }

        // Reads in file line by line
        ifstream inputFile("reservations.txt");
        string line;
        if (inputFile.is_open()) {
          while (getline(inputFile, line)) {
            
            // Split the line by comma
            vector<string> tokens;
            size_t pos = 0;
            while ((pos = line.find(",")) != string::npos) {
              string token = line.substr(0, pos);
              tokens.push_back(token);
              line.erase(0, pos + 1);
            }

            // Add the last token
            tokens.push_back(line);  
  
            // If it is under two days, it checks to see if there is a reservation already there
            if(under_two == true){
              if ((m_day == tokens[0]) && (time == tokens[1]) && (court == tokens[3])) {
                cout<<"This reservation already exists."<<endl;
                res = false;
                break;
              }
            }
              
            // If it isn't under two days, it checks to see if there is a reservation in the space, and cancels it if there is
            else{
              if ((m_day == tokens[0]) && (time == tokens[1]) && (court == tokens[3])) {
                ifstream inFile("reservations.txt");
                if (!inFile) {
                  cout << "Failed to open file." << endl;
                  return reservations;
                }
  
                ofstream tFile("temp.txt");
                if (!tFile) {
                  cout << "Failed to create temporary file." << endl;
                  inFile.close();
                  return reservations;
                }
  
                string l;
                while (getline(inFile, l)) {
                  vector<string> ts;
                  size_t p = 0;
                  while ((p = l.find(",")) != string::npos) {
                    string t = l.substr(0, p);
                    ts.push_back(t);
                    l.erase(0, p + 1);
                  }
  
                  // Add the last token
                  ts.push_back(l); 
                  
                  if (!(m_day == ts[0] && time == ts[1] && court == ts[3])) {
                    tFile << ts[0] <<"," <<ts[1] <<"," <<ts[2] << "," << ts[3] << "," << ts[4] << endl;
                  }
                }

                // Closes files
                inFile.close();
                tFile.close();

                const char* fname = "reservations.txt";
                if (remove(fname) != 0) {
                  cout << "Failed to remove file: " << fname << endl;
                } 
                else {
                  if (rename("temp.txt", fname) != 0) {
                    cout << "Failed to replace file: " << fname << endl;
                  } 
                  else {
                    cout << "Reservation canceled successfully." << endl;
                  }
                }
              }
            }
            if (cur_month <= month) {
              if (cur_day < day) {
                cout << "This day has already passed."<<endl;
                res = false;
                break;
              }
            }
          }
          inputFile.close();
          if (res == true) {
            istringstream iss(time);
            int hours, minutes;
            char separator;
            if (!(iss >> hours >> separator >> minutes)) {
            cout << "Invalid time format." << endl;
            } else {
            minutes += 30;
    
            // Carry over extra minutes to hours
            hours += minutes / 60;  
    
            // Keep minutes within the 0-59 range
            minutes %= 60;
            hours %= 24;            
            ostringstream oss;
            oss << hours << ":" << setfill('0') << setw(2) << minutes;
            string time2 = oss.str();
            ofstream outfile("reservations.txt", ios::app);
            outfile<<endl<<m_day<<","<<time<<","<<time2<<","<<court<<","<<user.get_user();
            outfile.close();
            Reservation *new_reservation = new Reservation(user, m_day, {time, time2}, stoi(court));
            reservations.push_back(new_reservation);
            }
          }
        }
        return reservations;
      }

      // If the reservation is over a week away, does not make it
      else{
        cout<<"You cannot reserve more than 7 days in advance"<<endl;
        return reservations;
      }
    }
  
    // Creates a function to modify reservations
    vector<Reservation *> modify_other_reservation(vector<Reservation *> reservations, User user) {
      int command = 0;
      string username;
      string m_day;
      cout<<"Do you want to add a user reservation or delete a user reservation?"<<endl;
      cout<<"1. Add a user reservation"<<endl;
      cout<<"2. Delete a user reservation"<<endl;
      while ((command != 1) && (command != 2)) {
        cin>>command;
        if (command == 2) {
          ifstream inputFile("reservations.txt");
          if (!inputFile) {
            cout << "Failed to open file." << endl;
            return reservations;
          }
          ofstream tempFile("temp.txt");
          if (!tempFile) {
            cout << "Failed to create temporary file." << endl;
            inputFile.close();
            return reservations;
          }
          cout<<"What user wants to delete a reservation?"<<endl;
          cin>>username;
          cout<<"What day does the user want to delete their reservation? (please put in this format '6/26')"<<endl;
          cin>>m_day;
          bool res = true;
          string line;

          // Reads in file line by line
          while (getline(inputFile, line)) {
            vector<string> tokens;
            size_t pos = 0;
            while ((pos = line.find(",")) != string::npos) {
              string token = line.substr(0, pos);
              tokens.push_back(token);
              line.erase(0, pos + 1);
            }
  
            // Add the last token
            tokens.push_back(line); 
            if (!(m_day == tokens[0] && username == tokens[4])) {
              tempFile << tokens[0] <<"," <<tokens[1] <<"," <<tokens[2] << "," << tokens[3] << "," << tokens[4] << endl;
            }
          }

          // Closes files
          inputFile.close();
          tempFile.close();
  
          const char* filename = "reservations.txt";
          if (remove(filename) != 0) {
            cout << "Failed to remove file: " << filename << endl;
          } 
          else {
            if (rename("temp.txt", filename) != 0) {
              cout << "Failed to replace file: " << filename << endl;
            } 
            else {
              cout << "Reservation canceled successfully." << endl;
            }
          }
        } 
        else if (command == 1) {
          
          // Insert desired day, timeslot, and court
          cout<<"What user wants to make a reservation?"<<endl;
          cin>>username;
          cout<<"What day does the user want to make a reservation? (please put in this format '6/26')"<<endl;
          cin>>m_day;
          cout<<"What start time is the user requesting? (any time from 8:00 - 21:00)"<<endl;
          string time;
          cin>>time;
          cout<<"What court do they want to reserve? (1, 2, or 3)"<<endl;
          string court;
          cin>>court;
          bool res = true;
          ifstream inputFile("reservations.txt");
          string line;

          // Reads in file line by line
          if (inputFile.is_open()) {
            while (getline(inputFile, line)) {
              
              // Split the line by comma
              vector<string> tokens;
              size_t pos = 0;
              while ((pos = line.find(",")) != string::npos) {
                string token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + 1);
              }
              tokens.push_back(line);
  
              // Add the last token 
              if ((m_day == tokens[0]) && (time == tokens[1]) && (court == tokens[3])) {
                cout<<"This reservation already exists."<<endl;
                res = false;
                break;
              }
            }
            inputFile.close();
            if (res == true) {
              istringstream iss(time);
              int hours, minutes;
              char separator;
              if (!(iss >> hours >> separator >> minutes)) {
                cout << "Invalid time format." << endl;
              } 
              else {
                minutes += 30;
  
                // Carry over extra minutes to hours
                hours += minutes / 60;  
    
                // Keep minutes within the 0-59 range
                minutes %= 60;          
                hours %= 24;            
                ostringstream oss;
                oss << hours << ":" << setfill('0') << setw(2) << minutes;
                string time2 = oss.str();
                ofstream outfile("reservations.txt", ios::app);
                outfile<<endl<<m_day<<","<<time<<","<<time2<<","<<court<<","<<username;
                outfile.close();
                vector<string> access_user = {username, "i"};
                User new_user = User(access_user[0], access_user[1], 'O');
                Reservation *new_reservation = new Reservation(new_user, m_day, {time, time2}, stoi(court));
                reservations.push_back(new_reservation);
              }
            }
          }
        }
      }
      return reservations;
    }
};