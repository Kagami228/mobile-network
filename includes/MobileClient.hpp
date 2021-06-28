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
    std::unique_ptr<NetConfAgent> agent;
    std::string _name;
    std::string _number;
    std::string _incomingNumber;
        
        public:
            bool registerClient(std::string number, std::string state);
            bool handleModuleChange(std::map < std::string, std::string >& mapFetchData);
            bool handleOperData(std::string& name, std::string& xPath) const;
            bool handleRpc();
            bool handleNotification();
            bool makeCall(std::string number);
            bool rejectCall();
            bool answerCall();
            bool endCall();
            bool setName(std::string name);
            bool unRegisterClient();
            std::string userName(std::string arg);
            std::string number(std::string arg);
            std::string incNumber(std::string arg);
            std::string state(std::string arg);

    };
}