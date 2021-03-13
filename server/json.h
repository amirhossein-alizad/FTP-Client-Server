#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <algorithm>
#include <vector>
#include "User.h"

void break_data(int i);
void find_user_data();
void parse_files();
void parse_users();
void parse(std::string jason_data);
std::string rmv_spc_newl(std::string str);
void jsonParser();
std::vector<User> userGetter();
std::vector<std::string> filesGetter();