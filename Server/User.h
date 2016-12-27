
/*
 * File:   User.h
 * Author: czimbortibor
 *
 * Created on December 17, 2016, 5:29 PM
 */

#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User(std::string loginTime = "") { this->loginTime = loginTime; }
    virtual ~User() {} 

    std::string getUsername() const { return username; }
    std::string getLoginTime() const { return loginTime; }
    
    std::string setLoginTime(const std::string& value) { loginTime = value; }

private:
    std::string username;
    std::string loginTime;
};

#endif /* USER_H */
