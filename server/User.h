#include<string>

class User{
    private:
        std::string username;
        std::string password;
        bool admin;
        int size;
    public:
        User(std::string username_, std::string password_, bool admin_, int size_);
        void change_size(int size_);
        std::string get_username();
        std::string get_password();
        bool is_admin();
        int get_size();
};

