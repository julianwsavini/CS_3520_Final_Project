// Import necessary libraries
#include "User.hpp"
#include <vector>

using namespace std;

// Creates Reservation class
class Reservation {

  // Private attributes
  private:
    User user_reserved;
    vector<string> time_slot;
    int court_num;
    string date;

  public:

    // Main constructor
    Reservation(User user_reserved, string date, vector<string> time_slot, int court_num) : user_reserved(user_reserved), time_slot(time_slot), court_num(court_num), date(date) {}

    // Copy constructor
    Reservation(const Reservation& other)
        : user_reserved(other.user_reserved), time_slot(other.time_slot), court_num(other.court_num), date(other.date) {}

    // Copy assignment operator
    Reservation& operator=(const Reservation& other) {
      if (this != &other) {
        user_reserved = other.user_reserved;
        time_slot = other.time_slot;
        court_num = other.court_num;
        date = other.date;
      }
      return *this;
    }

    // Move constructor
    Reservation(Reservation&& other)
      : user_reserved(move(other.user_reserved)), time_slot(move(other.time_slot)), court_num(move(other.court_num)), date(move(other.date)) {}

    // Move assignment operator
    Reservation& operator=(Reservation&& other) {
      if (this != &other) {
        user_reserved = move(other.user_reserved);
        time_slot = move(other.time_slot);
        court_num = move(other.court_num);
        date = move(other.date);
      }
      return *this;
    }

    // Destructor
    ~Reservation() {}

    // Getters and setters
    void set_user_reserved(User person){
      user_reserved = person;
    }

    User get_user_reserved(){
      return user_reserved;
    }

    void set_time_slot(string time_start, string time_end){}

    vector<string> get_time_slot() {
      return time_slot;
    }

    void set_court_num(int court){
      court_num = court;
    }

    int get_court_num(){
      return court_num;
    }

    void set_date(string day){
      date = day;
    }

    string get_date(){
      return date;
    }
};