#include "includes/NetConfAgent.hpp"
#include "includes/MobileClient.hpp"
#include <thread>
#include <functional>

using namespace Netconfagent;
using namespace Mobileclient;
MobileClient client;

void Registr_user(const std::string phone, const std::string user)
{
    client.registerClient(phone, "idle",user);
}
void Unregistr()
{
    client.unRegisterClient();
}
void Calling_to_phone(const std::string phone)
{
    client.makeCall(phone);
}
void Answer()
{
    client.answerCall();
}
void CallEnd()
{
    client.endCall();
}
void Reject()
{
    client.rejectCall();
}
void Change_Name(const std::string &name)
{
    client.setName(name);
}
struct exit_exception : std::exception
{
    std::string  _mess;
    exit_exception():_mess("exit from programm") {
    }
    const char* what() const noexcept override{
        return  _mess.c_str();
    }
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

std::vector<std::string> split(std::string str, const std::string & delimiters)
{
    std::vector<std::string> result;

    //is_delimiter is_del(delimeters);
    auto is_del = [delimiters](char c){
        bool res=false;
        for(auto cc : delimiters)
        {
            res = c==cc;
            if(res)
                break;
        }
        return res;
    };


    for(int i=0; i<= str.length();i++){
        if(is_del(str[i])){
            result.push_back(str.substr(0,i));
            str.erase(0,i+1);
            i=0;
        }
    }
   // if(result.empty())
     result.push_back(str);
    return result;
}

int main()
{
    do
    {
        std::vector<std::string> cmd;

        std::string command;
        size_t pos = 0;

        std::getline(std::cin, command);
       // std::cout << "token" << std::endl;
        int i = 0;
        cmd= split(command," ");
        try 
        {
             if(!cmd.empty())
                cmd_call(cmd);
        }
        catch (const exit_exception &  e)
        {
            
            std::cout<<e.what()<<std::endl;
            break;
        }
    }while(true);
    
    return 0;
}