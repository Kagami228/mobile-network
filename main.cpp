#include <iostream>

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

int main()
{
    std::string command;
    std::getline(std::cin,command);
    while(true)
    {
        std::string cm="";
        for(int i=0; i<command.size();i++)
        {
            
            if(command[i]==' ' or (i=command.size()-1))
            {
                if(i=command.size()-1)
                {
                    cm=cm+command[i];
                }
                if (cm=="register")
                {
                    std::string phone="";
                    for(int j=i; j<command.size();j++)
                    {
                        if(command[j]==' ' or (j=command.size()-1))
                        {
                            if(j=command.size()-1)
                            {
                                phone=phone+command[j];
                            }
                            std::string name="";
                            for(int o=j; o<command.size();o++)
                            {
                                if(command[o]==' ' or (o=command.size()-1))
                                {
                                    if(o=command.size()-1)
                                    {
                                        name=name+command[o];
                                    }   
                                    Registr_user(phone,name);
                                }else{
                                    name=name+command[o];
                                }
                            }
                        }else{
                            phone=phone+command[j];
                        }
                    }
                    
                } else if(cm=="unregistr")
                {
                    Unregistr();
                       
                }
                else if(cm=="call")
                {
                    std::string calling="";
                    for(int j=i; j<command.size();j++)
                    {
                        if(command[j]==' ' or (j=command.size()-1))
                        {
                            if(j=command.size()-1)
                            {
                                calling=calling+command[j];
                            }
                            Calling_to_phone(calling);
                        }else
                        {
                            calling=calling=command[j];
                        }
                    }
                }
                else if(cm=="answer")
                {
                    Answer();
                       
                }
                else if(cm=="callEnd")
                {
                    CallEnd();
                       
                }
                else if(cm=="reject")
                {
                    Reject();
                       
                }
                else if(cm=="name")
                {
                    std::string newName="";
                    for(int j=i; j<command.size();j++)
                    {
                        if(command[j]==' ' or (j=command.size()-1))
                        {
                            if(j=command.size()-1)
                            {
                                newName=newName+command[j];
                            }
                            ChangeName(newName);
                        }else
                        {
                            newName=newName=command[j];
                        }
                    }
                       
                }
                else if(cm=="exit"){
                    break;
                }
            }
            else {
                cm=cm+command[i];
            }
        }
    }
    return 0;
}