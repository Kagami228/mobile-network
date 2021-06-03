#include <iostream>
#include <vector>

void Registr_user(std::string &phone, std::string &user)
{
    std::cout<<user<<" was register by " <<phone<<std::endl;
}
void Unregistr(){
    std::cout<<"You unregistrd your phone"<<std::endl;
}
void Calling_to_phone(std::string phone)
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
void ChangeName(std::string &name){
    std::cout<<"Your new name "<<name<<std::endl;
}
void cmd_call(std::vector<std::string> &s){
    if(s[0]=="registr")
    {
        Registr_user(s[1],s[2]);
    } 
    if(s[0]=="call")
    {
        Calling_to_phone(s[1]);
    } 
    if(s[0]=="change")
    {
        ChangeName(s[1]);
    } 
    if(s[0]=="reject")
    {
        Reject();
    }
    if(s[0]=="Callend")
    {
        CallEnd();
    }
    if(s[0]=="unregistr")
    {
        Unregistr();
    }
    if(s[0]=="answer")
    {
        Answer();
    }
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