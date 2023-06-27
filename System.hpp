// Imports necessary libraries
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// Creates system class
class System {
  private:
  public:

    // Constructor
    System() {}
  
    // Copy constructor
    System(const System& other) {}
  
    // Copy assignment operator
    System& operator=(const System& other) {
      if (this != &other) {
        // Copy assignment logic here
      }
      return *this;
    }
  
    // Move constructor
    System(System&& other) {}
  
    // Move assignment operator
    System& operator=(System&& other) {
      if (this != &other) {
        // Move assignment logic here
      }
      return *this;
    }
  
    // Destructor
    ~System() {}
  
    // Menus
    void main_menu() {}
  
    // Members menu
    void member_menu(User *user, Schedule mainSched, vector<Reservation *> res_vect, int month, int day) {
      bool leave = false;
      while (leave == false) {
        int decision;

        // Prints the member menu
        cout << "Member Menu:" << endl;
        cout << "1. Update skill ranking" << endl;
        cout << "2. Reserve a court" << endl;
        cout << "3. Display the court schedule" << endl;
        cout << "4. Remove a reservation" << endl;
        cout << "5. Message officer" << endl;
        cout << "6. View Your Reservations" << endl;
        cout << "7. Exit" << endl;
        cin >> decision;

        // Updates skill ranking
        if (decision == 1) {
          string new_rating;
          cout << "Your current skill ranking is " << user->get_skill() << endl;
          cout << "What would you like it to be? (A, B, or C)" << endl;
          cin >> new_rating;
          user->set_skill(new_rating[0]);
        } 

        // Reserves a court
        else if (decision == 2) {
          mainSched.make_reservation(res_vect, *user, month, day, user->get_user());
        } 

        // Displays court schedule
        else if (decision == 3) {
          mainSched.display_schedule();
        } 

        // Removes a reservation
        else if (decision == 4) {
          mainSched.request_cancel_reservation(user->get_user());
        }

        // Messages officer
        else if (decision == 5) {
          mainSched.message_officer(user->get_user());
        } 

        // Views reservations
        else if (decision == 6) {
          mainSched.view_my_reservations(user->get_user());
        }  

        // Exits menu
        else if (decision == 7) {
          leave = true;
        }
        
        else {
          throw runtime_error("Please insert a correct number");
        }
      }
    }
  
    // Officer menu
    void officer_menu(User *user, Schedule mainSched, vector<Reservation *> res_vect, int month, int day) { 
      bool leave = false;
      while (leave == false) {
        int decision;

        // Prints menu
        cout << "Officer Menu" << endl;
        cout << "1. Update skill ranking" << endl;
        cout << "2. Reserve a court" << endl;
        cout << "3. Make group reservation" << endl;
        cout << "4. Display the court schedule" << endl;
        cout << "5. Remove a reservation" << endl;
        cout << "6. Read Messages" << endl;
        cout << "7. View Your Reservations" << endl;
        cout << "8. Change other's reservations" <<endl;
        cout << "9. Exit" << endl;
        cin >> decision;

        // Updates skill rankings
        if (decision == 1) {
          string new_rating;
          cout << "Your current skill ranking is " << user->get_skill() << endl;
          cout << "What would you like it to be? (A, B, or C)" << endl;
          cin >> new_rating;
          user->set_skill(new_rating[0]);
        } 

        // Reserves court
        else if (decision == 2) {
          mainSched.make_reservation(res_vect, *user, month, day, user->get_user());
        }

        // Makes a group reservation
        else if (decision == 3){
          mainSched.make_group_reservation(res_vect, *user);
        }

        // Displays schedule
        else if (decision == 4) {
          mainSched.display_schedule();
        } 

        // Removes reservation
        else if (decision == 5) {
          mainSched.request_cancel_reservation(user->get_user());
        } 

        // Reads messages
        else if (decision == 6) {
          mainSched.read_messages(user->get_user());
        } 

        // Views reservations
        else if (decision == 7) {
          mainSched.view_my_reservations(user->get_user());
        } 

        // Change others reservations
        else if (decision == 8) {
          
          mainSched.modify_other_reservation(res_vect, *user);
        } 

        // Exits menu
        else if (decision == 9) {
          leave = true;
        } 
        else {
          throw runtime_error("Please insert a correct number");
        }
      }
    }
  
    // Caoch menu
    void coach_menu(User *user, Schedule mainSched, vector<Reservation *> res_vect, int day, int month, int dayOfWeek) { 
      bool leave = false;
      while (leave == false) {
        int decision;

        // Prints coach menu
        cout << "Coach Menu" << endl; 
        cout << "1. Reserve a court" << endl;
        cout << "2. Display the court schedule" << endl;
        cout << "3. Message officer" << endl;
        cout << "4. View Your Reservations" << endl;
        cout << "5. Exit" << endl;
        cin >> decision;

        // Reserves a court
        if (decision == 1) {
          mainSched.make_coach_reservation(res_vect, *user, day, month, dayOfWeek);
        } 

        // Displays schedule
        else if (decision == 2) {
          mainSched.display_schedule();
        } 

        // Messages officer
        else if (decision == 3) {
          mainSched.message_officer(user->get_user());
        } 
        
        // Views reservations
        else if (decision == 4) {
          mainSched.view_my_reservations(user->get_user());
        } 
        
        // Exits menu
        else if (decision == 5) {
          leave = true;
        } 
        else {
          throw runtime_error("Please insert a correct number");
        }
      }
    }
  
    // Start program function
    vector<string> start_program() {
      vector<string> login;
      int exist;
      bool contin = false;
      
      // Ask if user exists
      cout << "Are you an existing user? (1 for yes, 2 for no)" << endl;
      cin >> exist;
  
      if (exist == 1) {
        
        // Insert user and password
        string user;
        string pass;
        cout << "Enter your username" << endl;
        cin >> user;
        cout << "Enter your password" << endl;
        cin >> pass;
        string user_pass = user + "," + pass;
        
        // Open file
        ifstream infile("users.txt");
        if (!infile) {
          cout << "Failed to open file" << endl;
        } 
        else {
          string line;
          bool exist = false;
          
          // Look if username and password combo exists
          while (getline(infile, line)) {
            istringstream ss(line);
            string word;
            char token = ' ';
            while (getline(ss, word, token)) {
              if (word == user_pass) {
                exist = true;
              }
            }
          }
          if (exist == false) {
            cout << "There is no account found" << endl;
          } 
          else {
            login = {user, pass};
          }
          infile.close();
        }
      }
  
      else if (exist == 2) {
        
        // Create new user and password
        string new_user;
        string new_pass;
        string type;
        cout << "Enter a new username" << endl;
        cin >> new_user;
        cout << "Enter a new password" << endl;
        cin >> new_pass;
        cout << "Are you a member, coach, or officer? (M for member, C for "
                "coach, O for officer)"
             << endl;
        cin >> type;
        string user_pass = new_user + "," + new_pass;
        
        // Open file
        ifstream infile("users.txt");
        if (!infile) {
          cout << "Failed to open file" << endl;
        } 
        else {
          string line;
          bool exist = false;
          
          // Look if username and password combo exists
          while (getline(infile, line)) {
            istringstream ss(line);
            string word;
            char token = ' ';
            while (getline(ss, word, token)) {
              if (word == user_pass) {
                cout << "This username and password already exist" << endl;
                exist = true;
              }
            }
          }
          infile.close();
          
          // If it doesnt exist write it in
          if (exist == false) {
            ofstream outfile("users.txt", ios::app);
            login = {new_user, new_pass, type};
            outfile << user_pass << " " << type << endl;
            outfile.close();
          }
        }
      } 
      else {
        throw runtime_error("Please insert a correct number");
      }
      return login;
    }
  
    void end_program() {}
};