#include "User.h"

User::User(std::string username_, std::string password_, bool admin_, int size_){
    username = username_;
    password = password_;
    admin = admin_;
    size = size_;
}

void User::change_size(int size_){
    size += size_;
}

std::string User::get_username(){return username;}

std::string User::get_password(){return password;}

bool User::is_admin(){return admin;}

int User::get_size(){return size;}