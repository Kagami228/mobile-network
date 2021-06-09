#include "NetConfAgent.hpp"

void Registr_user(const std::string &phone, const std::string &user)
{
    std::cout << user << " was register by " << phone << std::endl;
}
void Unregistr()
{
    std::cout << "You unregistrd your phone" << std::endl;
}
void Calling_to_phone(const std::string phone)
{
    std::cout << "calling..." << std::endl;
}
void Answer()
{
    std::cout << "conversation" << std::endl;
}
void CallEnd()
{
    std::cout << "callEnd" << std::endl;
}
void Reject()
{
    std::cout << "rejected" << std::endl;
}
void Change_Name(const std::string &name)
{
    std::cout << "Your new name " << name << std::endl;
}

struct exit_exception : std::exception
{
    exit_exception():_mess("exit from programm") {

    }
    const char* what() const noexcept override{
        return  _mess.c_str();
    }
    std::string _mess;
};

void cmd_call(std::vector<std::string> &s)
{
    using commandsMap = std::map<std::string, std::function<void(std::string, std::string)>>;

    commandsMap commands;
    commands["register"] = [](const std::string &p1, const std::string &p2)
    { Registr_user(p1, p2); };
    commands["call"] = [](const std::string &p1, const std::string &p2)
    { Calling_to_phone(p1); };
    commands["change"] = [](const std::string &p1, const std::string &p2)
    { Change_Name(p1); };
    commands["reject"] = [](const std::string &p1, const std::string &p2)
    { Reject(); };
    commands["Callend"] = [](const std::string &p1, const std::string &p2)
    { CallEnd(); };
    commands["unregistr"] = [](const std::string &p1, const std::string &p2)
    { Unregistr(); };
    commands["answer"] = [](const std::string &p1, const std::string &p2)
    { Answer(); };
    commands["exit"] = [](const std::string &p1, const std::string &p2)
    { 
        throw exit_exception(); 
    };

    if (commands.find(s[0]) == commands.end())
        return;
    if (s.size() == 1)
        commands[s[0]]("", "");
    if (s.size() == 2)
        commands[s[0]](s[1], "");
    if (s.size() == 3)
        commands[s[0]](s[1], s[2]);
}



int main()
{
    std::string delimiter = " ";
    std::cout<<"sdfsdfdsfdsfds";

    do
    {
        std::vector<std::string> cmd;
        size_t pos = 0;
        std::string token;
        std::string command;
        std::getline(std::cin, command);
 
        int i = 0;
        while ((pos = command.find(delimiter)) != std::string::npos - 1)
        {
            token = command.substr(0, pos);
            cmd.push_back(token);
            //std::cout << token << std::endl;
            command = command.erase(0, pos + delimiter.length());

            if (pos == std::string::npos)
            {
                break;
            }
        }
        try 
        {
            cmd_call(cmd);
        }
        catch (const std::exception &  e)
        {
            std::cout<<"sdfsdfdsfdsfds";
            std::cout<<e.what();
            break;
        }
    }while(true);
    
    NetConfAgent n;
    n.initSysrepo();
    n.fetchData("./testmodel/sports/");
    return 0;
}