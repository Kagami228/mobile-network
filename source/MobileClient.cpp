#pragma once

#include "MobileClient.hpp"

using namespace Netconfagent;

namespace Mobileclient
{
//             bool registerClient(std::string number, std::string state);
//             void handleModuleChange(std::map < std::string, std::string >& mapFetchData) const;
//             void handleOperData(std::string& name, std::string& xPath) const;
//             void handleRpc();
//             void handleNotification();
//             bool makeCall(std::string number);
//             void setName(std::string name);
    bool MobileClient::registerClient(std::string number, std::string state)
    {
        _number = number;
        
        std::string _xpathForChangeState = "/mobile-network:core/subscriber[number='" + number + "']/state";
        std::string _xpathForChangeNum = "/mobile-network:core/subscriber[number='" + number + "']/number";
        std::string _xpathForSubscribe = "/mobile-network:core/subscriber[number='" + number + "']";
        std::string _xpathForFetch = "/mobile-network:core/subscriber[number='" + number + "']";


        agent.initSysrepo();
        agent.changeData(_xpathForChangeState, state);
        agent.changeData(_xpathForChangeNum, number);
        if(_name != "")
            agent.registerOperData(*this, _xpathForSubscribe);
        else
        {
            std::cout << "Name don`t setted" << std::endl;
            return false;
        }
        agent.subscribeForModelChanges(*this, _xpathForFetch);
        std::cout << "Client with this number " << _number << " the creat " << std::endl;
    };
    bool MobileClient::handleModuleChange(std::map < std::string, std::string >& mapFetchData) const
    {
		std::map < std::string, std::string >::iterator it;
		it = mapFetchData.begin();
		mapFetchData.erase(it);
		it = mapFetchData.begin();
		mapFetchData.erase(it);
    	for (auto i : mapFetchData)
        {
    	    std::cout << i.first << " : " << i.second << std::endl;
    	}
    };
    // bool MobileClient::handleOperData(std::string& n, std::string& xPath) const
    // {
    //     n = _name;
    //     xPath = "/mobile-network:core/subscriber[number='" + _number + "']/userName";
    //     return 1;
    // }
    // bool MobileClient::makeCall(std::string numberphone)
    // {
    //     std::string _xpathForChangeIncNum = "/mobile-network:core/subscriber[number='" + numberphone + "']/incomingNumber";
    //     agent.changeData(_xpathForChangeIncNum, _number);
    //     return 1;
    // }
    bool MobileClient::setName(std::string n)
    {
        _name = n;
        std::cout <<  "Client with this number " << _number << " changed be name " << std::endl;
        return 1;
    }
}