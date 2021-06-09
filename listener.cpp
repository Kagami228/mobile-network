#include <iostream>
#include <vector>
#include "libsysrepocpp/headers/Session.hpp"

class NetConfAgent
{
    std::shared_ptr<sysrepo::Connection> Connection;
    std::shared_ptr<sysrepo::Session> Session;

public:
    NetConfAgent()
    {
    }
    bool initSysrepo()
    {
        Connection = std::make_shared<sysrepo::Connection>();
        Session = std::make_shared<sysrepo::Session>(Connection);
        std::shared_ptr<sysrepo::Subscribe> Subscribe;
        Subscribe = std::make_shared<sysrepo::Subscribe>(Session);
        std::cout<<"Session began"<<std::endl;

        return 1;
    }

    bool fetchData(const std::string &s_xpath)
    {
         const char *xpath = "/mobile-network:core/subscribers[number='001']";

    try {
        libyang::S_Data_Node data = Session->get_data(xpath);
        /* go through all top-level siblings */
        for (libyang::S_Data_Node &root : data->tree_for()) {
            /* go through all the children of a top-level sibling */
            //for (libyang::S_Data_Node &node : root->tree_dfs()) {
             //   print_node(node);
         //   }
        }
    } catch( const std::exception& e ) {
        std::cout << e.what() << std::endl;
    }
    return 1;
    }
    bool syscrybeForModelChanges();
    bool registrOpenData();
    bool sybscribeForRpc();
    bool notifySysrepo();
    bool changeData();
};

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