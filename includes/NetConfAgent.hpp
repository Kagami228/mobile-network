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
        std::shared_ptr<sysrepo::Connection> Connection;
        std::shared_ptr<sysrepo::Session> Session;
        std::shared_ptr<sysrepo::Subscribe> Subscribe;

    public:
        bool initSysrepo();
        bool fetchData(std::string _xpath, std::map <std::string, std::string>& map);
        bool subscribeForModelChanges(Mobileclient::MobileClient& client, std::string& xpathForFetch);
        bool registerOperData(Mobileclient::MobileClient& client, std::string xpathForSubscribe);
        bool changeData(std::string _xpath, std::string _newValue);
        bool subscribeForRpc(std::string _module_name, std::string _rpc_xpath);
        bool notifySysrepo(std::string _module_name);
    };
}