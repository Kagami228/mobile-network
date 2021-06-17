#pragma once

#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "NetConfAgent.hpp"
#include "libsysrepocpp/headers/Session.hpp"

using namespace sysrepo;
using namespace Netconfagent;

namespace Mobileclient
{
    class MobileClient
    {
        NetConfAgent::NetConfAgent agent;
        std::string _name;
        std::string _number;
        
        public:
            bool registerClient(std::string number, std::string state);
            bool handleModuleChange(std::map < std::string, std::string >& mapFetchData) const;
            bool handleOperData(std::string& name, std::string& xPath) const;
            bool handleRpc();
            bool handleNotification();
            bool makeCall(std::string number);
            bool setName(std::string name);
    };
}