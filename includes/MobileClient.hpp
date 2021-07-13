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
    std::unique_ptr<Netconfagent::NetConfAgent> _agent;
    std::string _name;
    std::string _number;
    std::string _incomingNumber;
    std::string _outcomingNumber;
    std::string _state;
    bool _callState;
    public:
        MobileClient();
        MobileClient(std::unique_ptr<Netconfagent::NetConfAgent> agent_NetConf);
        bool registerClient(std::string number,std::string name);
        bool unRegisterClient();
        void handleModuleChange(std::map < std::string, std::string >& _fetch_data) ;
        void handleOperData(std::string& name, std::string& xPath) const;
        void handleRpc();
        void handleNotification();
        bool makeCall(std::string number);
        bool answerCall();
        bool rejectCall();
        bool endCall();
        void setName(std::string name);
        std::string getName();
    
};

}