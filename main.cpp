#include <iostream>
#include <vector>
#include "libsysrepocpp/headers/Session.hpp"


class NetConfAgent{
    std::shared_ptr<sysrepo::Connection> Connection;
    std::shared_ptr<sysrepo::Session> Session;
    std::shared_ptr<sysrepo::Subscribe> Subscribe;
public:
    NetConfAgent(){
      
    }
    bool initSysrepo(){
        Connection = std::make_shared<sysrepo::Connection>();
        Session = std::make_shared<sysrepo::Session>(Connection);
        Subscribe = std::make_shared<sysrepo::Subscribe>(Session);
        
    }
    bool closeSysrepo(){
        Subscribe.reset();
        Session.reset();
        Connection.reset();

    }
    bool fetchDsts();
    bool syscrybeForModelChanges();
    bool registrOpenData();
    bool sybscribeForRpc();
    bool notifySysrepo();
    bool changeData();
};

void Registr_user(const std::string &phone, const std::string &user)
{
    std::cout<<user<<" was register by " <<phone<<std::endl;
}
void Unregistr(){
    std::cout<<"You unregistrd your phone"<<std::endl;
}
void Calling_to_phone(const std::string phone)
{
    std::cout<<"calling..."<<std::endl;
}
void Answer()
{
    std::cout<<"conversation"<<std::endl;
}
void CallEnd(){
    std::cout<<"callEnd"<<std::endl;
}
void Reject(){
    std::cout<<"rejected"<<std::endl;
}
void Change_Name(const std::string &name){
    std::cout<<"Your new name "<<name<<std::endl;
}
void cmd_call(std::vector<std::string> &s)
{
    std::map<std::string,std::function<void(std::string,std::string)>> commands;
    commands["register"] =  [](const std::string &p1,const std::string &p2 ) { Registr_user(p1,p2);  };
    commands["call"]     =  [](const std::string &p1,const std::string &p2 ) { Calling_to_phone(p1); };
    commands["change"]   =  [](const std::string &p1,const std::string &p2 ) { Change_Name(p1);       };
    commands["reject"]   =  [](const std::string &p1,const std::string &p2 ) { Reject(); };
    commands["Callend"]  =  [](const std::string &p1,const std::string &p2 ) { CallEnd(); };
    commands["unregistr"] =  [](const std::string &p1,const std::string &p2 ) { Unregistr(); };
    commands["answer"] =  [](const std::string &p1,const std::string &p2 ) { Answer(); };

    
    if(s.size()==1)
     commands[s[0]]("","");
    if(s.size()==2)
     commands[s[0]](s[1],"");
    if(s.size()==3)
     commands[s[0]](s[1],s[2]);
     


}

int main()
{
    std::string command;
    std::getline(std::cin,command);
    std::string delimiter = " ";
    while(true)
    {
        std::vector<std::string> cmd;
        size_t pos = 0;
        std::string token;
        int i=0;
        while ((pos = command.find(delimiter)) != std::string::npos-1) {
            token = command.substr(0, pos);
            cmd.push_back(token);
            //std::cout << token << std::endl;
            command.erase(0, pos + delimiter.length());

            if(pos==std::string::npos){
                break;
            }
        }
        cmd_call(cmd);

        std::getline(std::cin,command);
    }
    return 0;
}