#ifndef USER_HPP
#define USER_HPP

using namespace std;

// Creates user parent class
class User {

  // Private attributes
  private:
    string username;
    string password;
    char type;
  public:

    // Constructor
    User(const string& username, const string& password, char type) : username(username), password(password), type(type) {}

    // Copy constructor
    User(const User& other)
        : username(other.username), password(other.password), type(other.type) {}

    // Copy assignment operator
    User& operator=(const User& other) {
        if (this != &other) {
            username = other.username;
            password = other.password;
            type = other.type;
        }
        return *this;
    }

    // Move constructor
    User(User&& other)
        : username(move(other.username)), password(move(other.password)), type(move(other.type)) {}

    // Move assignment operator
    User& operator=(User&& other) {
        if (this != &other) {
            username = move(other.username);
            password = move(other.password);
            type = move(other.type);
        }
        return *this;
    }

    // Destructor
    ~User() {}

    // Setters and getters
    void set_user(string name) {
      username = name;
    }
    string get_user() {
      return username;
    }
    void set_password(string pass) {
      password = pass;
    }
    string get_pass() {
      return password;
    }
    void set_type(char ty) {
      type = ty;
    }
    char get_type() {
      return type;
    }

    virtual char get_skill() const {
    }
    virtual void set_skill(char c) {
    }
};

// Member child class
class Member : public User  {
  private:
    char skill_rating;
  public:

    // Member constructor
    Member(const string& username, const string& password, char skill_rating, char type) : User(username, password, type), skill_rating(skill_rating) {}

    // Getter and setter function
    void set_skill(char c) override{
      skill_rating = c;
    }

    char get_skill() const override{
      return skill_rating;
    }
};

// Officer child class
class Officer : public Member  {
  public:

    // Officer constructor
    Officer(const string& username, const string& password, char skill_rating, char type) : Member(username, password, skill_rating, type) {}
};

// Coach child class
class Coach : public User  {
  public:

    // Coach constructor
    Coach(const string& username, const string& password, char type) : User(username,password, type) {}
};

#endif