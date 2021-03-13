#include "json.h"

std::string users_str;
std::string files_str;
std::vector<std::string> data;
std::vector<std::string> admin_files;
std::vector<User> user_list;

void break_data(int i){
    int j = 0;
    int size = 0;
    std::string username;
    std::string password;
    bool admin;
    std::string str;
    while(j < data[i].length()){
        if(str == "user"){
            j += 1;
            str = "";
            while(data[i][j] != ',' && j < data[i].length()){
                str += data[i][j];
                j++;
            }
            username = str;
            str = "";
            j++;
            continue;
        }
        else if(str == "password"){
            j += 1;
            str = "";
            while(data[i][j] != ',' && j < data[i].length()){
                str += data[i][j];
                j++;
            }
            password = str;
            str = "";
            j++;
            continue;
        }
        else if(str == "admin"){
            j += 1;
            str = "";
            while(data[i][j] != ',' && j < data[i].length()){
                str += data[i][j];
                j++;
            }
            if(str == "true")
                admin = true;
            else
                admin = false;
            str = "";
            j++;
            continue;
        }
        else if(str == "size"){
            j += 1;
            str = "";
            while(data[i][j] != ',' && j < data[i].length()){
                size = size * 10;
                size = size + (data[i][j] - '0');
                j++;
            }
            str = "";
            j++;
            continue;
        }
        str += data[i][j];
        j++;
    }
    User u = User(username, password, admin, size);
    user_list.push_back(u);
}

void find_user_data(){
    for(int i = 0; i < data.size(); i++)
        break_data(i);
}

void parse_files(){
    int i =0;
    std::string file;
    while(i < files_str.length()){
        if(files_str[i] == ','){
            admin_files.push_back(file);
            i++;
            file = "";
            continue;
        }
        file += files_str[i];
        i++;
    }
    if(file != "")
        admin_files.push_back(file);
}

void parse_users(){
    std::string str;
    int i = 0;
    while(i < users_str.length()){
        if(users_str[i] == '{' ){
            i++;
            continue;
        }
        if(users_str[i] == '}'){
            if(str[str.length() - 1] == ',')
                str.erase(str.end() - 1);
            data.push_back(str);
            str = "";
            i += 3;
        }
        str += users_str[i];
        i++;
    }
}

void parse(std::string jason_data){
    std::string str;
    int i = 0;
    while(i < jason_data.length()){
        if (jason_data[i] == ','){
            i += 1;
            continue;
        }
        if(str == "users"){
            i +=2;
            str = "";
            while(jason_data[i] != ']'){
                str += jason_data[i];
                i++;
            }
            users_str = str;
            str = "";
            i++;
            continue;
        }
        else if(str == "files"){
            i +=2;
            str = "";
            while(jason_data[i] != ']'){
                str += jason_data[i];
                i++;
            }
            files_str = str;
            str = "";
            i++;
            continue;
        }
        str += jason_data[i];
        i++;
    }
}

std::string rmv_spc_newl(std::string str){
    std::string jason_data;
    for(int i = 0; i < str.length(); i++){
        if(isspace(str[i]) || str[i] == '\n' || str[i] == '\t')
            continue;
        jason_data += str[i];
    }
    return jason_data;
}

void jsonParser(){
    std::ifstream fin;
    fin.open("config.json");
    std::string str, jason_data;
    while(getline(fin, str))
        jason_data += str;
    jason_data = rmv_spc_newl(jason_data);
    jason_data.erase(jason_data.begin());
    jason_data.erase(jason_data.end()-1);
    jason_data.erase(std::remove(jason_data.begin(), jason_data.end(), '\"'), jason_data.end());
    parse(jason_data);
    parse_users();
    parse_files();
    find_user_data();
}

std::vector<User> userGetter(){return user_list;}

std::vector<std::string> filesGetter(){return admin_files;}