#pragma once

#include "MobileClient.hpp"

using namespace Netconfagent;

namespace Mobileclient
{
    std::vector <std::string> listNumbers;
    std::string outcomingNumber;
    std::string myOldState;
    std::string incNumOld;
//             bool registerClient(std::string number, std::string state);
//             bool handleModuleChange(std::map < std::string, std::string >& mapFetchData);
//             bool handleOperData(std::string& name, std::string& xPath) const;
//             bool handleRpc();
//             bool handleNotification();
//             bool makeCall(std::string number);
//             bool rejectCall();
//             bool answerCall();
//             bool endCall();
//             bool setName(std::string name);
//             bool unRegisterClient();
//             std::string userName(std::string arg);
//             std::string number(std::string arg);
//             std::string incNumber(std::string arg);
//             std::string state(std::string arg);
   
    bool MobileClient::registerClient(std::string number, std::string state)
    {
        _number = number;
        agent = make_unique<NetConfAgent>();
        std::string xpathMyState = MobileClient::state(_number);
        std::string xpathMyNum = MobileClient::number(_number);
        std::string xpathForSubscribe = "/mobile-network:core/subscribers[number='" + _number + "']";
        std::string xpathForFetch = "/mobile-network:core/subscribers";
        agent->initSysrepo();
        if(_name == "")
        {
            std::cout << "Name isn`t setted" << std::endl;
            return false;
        }
        if(_name != "")
        {
            agent->changeData(xpathMyState, state);
            agent->changeData(xpathMyNum, number);
            agent->registerOperData(*this, xpathForSubscribe);
        }
        agent->subscribeForModelChanges(*this, xpathForFetch);
        std::cout  << "Client " << _name << " registered with number " << _number << std::endl;
    };
    void MobileClient::handleModuleChange(std::map < std::string, std::string >& mapFetchData) 
    {
        std::string xpathMyState = MobileClient::state(_number);
        std::string xpathMyIncNum = MobileClient::incNumber(_number);
        _incomingNumber = mapFetchData[xpathMyIncNum];
        std::string xpathOtherState = MobileClient::state(_incomingNumber);
        std::string xpathOtherStateOldNum;
         if(iMadeCall == 0)
            xpathOtherStateOldNum = MobileClient::state(incNumOld);
         else if(iMadeCall == 1)
            xpathOtherStateOldNum = MobileClient::state(outcomingNumber);
        if(mapFetchData[xpathMyIncNum] != "" && mapFetchData[xpathMyState] == "active")
        {
            std::cout<<"Incoming Number " << _incomingNumber << " call"<<std::endl;
        }

        if(mapFetchData[xpathMyState] == "busy" && mapFetchData[xpathOtherState] == "busy")
        {
            std::cout<<"Call is active"<<std::endl;
            callState = true;
        }

        if(mapFetchData[xpathMyState] == "idle" && mapFetchData[xpathOtherStateOldNum] == "idle" && mapFetchData[xpathMyIncNum] == "" && callState == false)//need to thinking
        {
            std::cout<<"Call rejected"<<std::endl;
            callState = false;
            incNumOld = "";
        }
        if(mapFetchData[xpathMyState] == "idle" && mapFetchData[xpathOtherStateOldNum] == "idle" && mapFetchData[xpathMyIncNum] == "" && callState == true)
        {
            std::cout<<"Call ended"<<std::endl;
            callState = false;
        }
        if(mapFetchData[xpathMyState] == "busy" && _name == "" && mapFetchData[xpathMyIncNum] == "")
        {
            std::cout<<"Client unregistered"<<std::endl;
        }

    };
    void MobileClient::handleOperData(std::string& name, std::string& xPath) const
    {
        name = _name;
    
        xPath = "/mobile-network:core/subscribers[number='" + _number + "']/userName";
    }
    bool MobileClient::makeCall(std::string number)
    {
        std::string xpathOtherIncNum = MobileClient::incNumber(number);
        std::string xpathOtherState = MobileClient::state(number);
        std::string xpathOtherNumber = MobileClient::number(number);
        std::string xpathOtherClient = "/mobile-network:core/subscribers[number='" + number + "']";
        std::string xpathMyState = MobileClient::state(_number);
        std::map < std::string, std::string > CheckNumber;
        agent->fetchData(xpathOtherClient,CheckNumber);
        if(CheckNumber[xpathOtherState] != "idle")
        {
            std::cout<<"Client busu"<std::<endl;
            return false;
        }
        if(mapCheckNumber[xpathOtherNumber] == _number)
        {
            std::cout<<"this u number "<<std::endl;
            return false;
        }

        else if(mapCheckNumber[xpathOtherNumber] == number)
        {
            agent->changeData(xpathOtherIncNum, _number);
            agent->changeData(xpathMyState, "active");
            agent->changeData(xpathOtherState, "active");
        }

        outcomingNumber = number;
        iMadeCall = 1;
    }
    void MobileClient::setName(std::string name)
    {
        _name = name;

        std::cout << "Name " << _name << " setted " << std::endl;
    }
    bool MobileClient::answerCall()
    {
        std::string xpathMyState = MobileClient::state(_number);
        std::string xpathOtherState = MobileClient::state(_incomingNumber);

        if(_incomingNumber != "")
        {
            agent->changeData(xpathMyState, "busy");
            agent->changeData(xpathOtherState, "busy");
        }
        else
        {
            std::cout<<"You not have incoming call"<<std::endl;
            return false;
        }
    }
    bool MobileClient::rejectCall()
    {
        incNumOld = _incomingNumber;
        std::string xpathMyState = MobileClient::state(_number);
        std::string xpathMyIncNum = MobileClient::incNumber(_number);
        std::string xpathOtherIncNum = MobileClient::incNumber(outcomingNumber);
        std::string xpathCheckState = "/mobile-network:core/subscribers";
        std::string xpathOtherState;
        if(iMadeCall == 0)
            xpathOtherState = MobileClient::state(incNumOld);
        else if(iMadeCall == 1)
            xpathOtherState = MobileClient::state(outcomingNumber);

       std:: map < std::string, std::string > CheckState;
        agent->fetchData(xpathCheckState, CheckState);

        if(CheckState[xpathMyState] == "idle")
        {
            std::cout<<"Call doesn't exist"<<std::endl;
            return false;
        }

        if(mapCheckState[xpathMyState] == "busy")
        {
            std::cout<<"You already answer the call!!!"<<std::endl;
            return false;
        }

        if(mapCheckState[xpathMyState] == "active" && iMadeCall == 0)
        {
            agent->changeData(xpathMyIncNum, "");
            agent->changeData(xpathMyState, "idle");
            agent->changeData(xpathOtherState, "idle");
        }

        if(mapCheckState[xpathMyState] == "active" && iMadeCall == 1)
        {
            agent->changeData(xpathOtherIncNum, "");
            agent->changeData(xpathMyState, "idle");
            agent->changeData(xpathOtherState, "idle");
        }
        iMadeCall = 0;
    }
    bool MobileClient::endCall()
    {
        incNumOld = _incomingNumber;
        std::string xpathMyState = MobileClient::state(_number);
        std::string xpathMyIncNum = MobileClient::incNumber(_number);
        std::string xpathOtherIncNum = MobileClient::incNumber(outcomingNumber);
        std::string xpathCheckState = "/mobile-network:core/subscribers";
        string xpathOtherState;
        if(iMadeCall == 0)
            xpathOtherState = MobileClient::state(incNumOld);
        else if(iMadeCall == 1)
            xpathOtherState = MobileClient::state(outcomingNumber);
        std::map <std::string,std::string > mapCheckState;
        agent->fetchData(xpathCheckState, mapCheckState);
        if(mapCheckState[xpathMyState] == "idle")
        {
            std::cout<<"Call doesn't exist"<<std::endl;
            return false;
        }
        if(mapCheckState[xpathMyState] == "active")
        {
            std::cout<<"You didn't answer the call!!!"<<std::endl;
            return false;
        }
        if(mapCheckState[xpathMyState] == "busy" && iMadeCall == 0)
        {
            agent->changeData(xpathMyState, "idle");
            agent->changeData(xpathOtherState, "idle");
            agent->changeData(xpathMyIncNum, "");
        }
        if(mapCheckState[xpathMyState] == "busy" && iMadeCall == 1)
        {
            agent->changeData(xpathMyState, "idle");
            agent->changeData(xpathOtherState, "idle");
            agent->changeData(xpathOtherIncNum, "");
        }
        iMadeCall = 0;
    }
    bool MobileClient::unRegisterClient()
    {
        std::map < std::string, std::string > mapCheck;
        std::string xpathToMe = "/mobile-network:core/subscribers";
        agent->fetchData(xpathToMe, mapCheck);
        std::string xpathMyState = MobileClient::state(_number);
        std::string xpathMyIncNum = MobileClient::incNumber(_number);
        std::string xpathMyNum = MobileClient::number(_number);
        std::string xpathMyName = MobileClient::userName(_number);
        std::string xpathForSubscribe = "/mobile-network:core/subscribers[number='" + _number + "']";
        if(mapCheck[xpathMyState] == "idle" && mapCheck[xpathMyIncNum] == "")
        {
            _name = "";
            agent->changeData(xpathMyState, "busy");
            _number = "";
            agent->changeData(xpathMyNum, _number);
            agent.~unique_ptr();
        }
        else
        {
            std::cout<<"Client busy!!!"<<std::endl;
            return false;
        }
    }
     std::string MobileClient::userName(std::string arg)
    {
        return "/mobile-network:core/subscribers[number='" + arg + "']/userName";
    }
    std::string MobileClient::number(std::string arg)
    {
        return "/mobile-network:core/subscribers[number='" + arg + "']/number";
    }
    std::string MobileClient::incNumber(std::string arg)
    {
        return "/mobile-network:core/subscribers[number='" + arg + "']/incomingNumber";
    }
    std::string MobileClient::state(std::string arg)
    {
        return "/mobile-network:core/subscribers[number='" + arg + "']/state";
    }
    bool callState;
    bool iMadeCall = 0;
}