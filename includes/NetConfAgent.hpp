#pragma once
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "libsysrepocpp/headers/Session.hpp"

using namespace sysrepo;
namespace Mobileclient
{
    class MobileClient;
}

namespace Netconfagent
{

class NetConfAgent
{
    S_Connection Connection;
    S_Session Session;
    S_Subscribe Subscribe;
    public:
        virtual ~NetConfAgent();
        bool virtual initSysrepo();
        bool virtual fetchData(std::map <std::string, std::string>& map);
        bool virtual subscribeForModelChanges(Mobileclient::MobileClient& client);
        bool virtual registerOperData(Mobileclient::MobileClient& client, std::string xpathForSubscribe);
        bool virtual subscribeForRpc(std::string _module_name, std::string _rpc_xpath);
        bool notifySysrepo(std::string _module_name); 
        bool virtual changeData(std::string _xpath, std::string newValue);
        bool virtual deleteData(std::string _xpath);
        bool virtual closeSysrepo();
        bool virtual closeSession();
        
};

}