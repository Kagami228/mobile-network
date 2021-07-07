#pragma once

#include "MobileClient.hpp"

using namespace Netconfagent;

namespace Mobileclient
{
    std::vector <std::string> listNumbers;
    std::string outcomingNumber;
    std::string myOldState;
    std::string incNumOld;
    bool calstate;
    bool icall = 0;
//             bool registerClient(std::string number, std::string state);
//             bool handleModuleChange(std::map < std::string, std::string >& FetchData);
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
   
    bool MobileClient::registerClient(std::string number, std::string state,std::string name)
    {
        _number = number;
        _name=name;
        agent = std::make_unique<NetConfAgent>();
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
            std::cout<<xpathMyState<<"    "<<state<<std::endl;
            agent->changeData(xpathMyState, state);
            agent->changeData(xpathMyNum, number);
            agent->registerOperData(*this, xpathForSubscribe);
        }
        agent->subscribeForModelChanges(*this, xpathForFetch);
        std::cout  << "Client " << _name << "can registered with number " << _number << std::endl;
    };
    void MobileClient::handleModuleChange(std::map < std::string, std::string >& FetchData) 
{
        std::string xpathMyState = MobileClient::state(_number);
        std::string xpathMyIncNum = MobileClient::incNumber(_number);
        _incomingNumber = FetchData[xpathMyIncNum];
        std::string xpathOtherState = MobileClient::state(_incomingNumber);
        std::string xpathOtherStateOldNum;
         if(icall == 0)
         xpathOtherStateOldNum = MobileClient::state(incNumOld);
         else if(icall == 1)
            xpathOtherStateOldNum = MobileClient::state(outcomingNumber);
        if(FetchData[xpathMyIncNum] != "" && FetchData[xpathMyState] == "active")
        {
            std::cout<<"Incoming number " << _incomingNumber << " call!!"<<std::endl;
        }

        if(FetchData[xpathMyState] == "busy" && FetchData[xpathOtherState] == "busy")
        {
            std::cout<<"Call is active"<<std::endl;
            calstate = true;
        }
        if(FetchData[xpathMyState] == "idle" && FetchData[xpathOtherStateOldNum] == "idle" && FetchData[xpathMyIncNum] == "" && calstate == false)
        {
            std::cout<<"Call rejected"<<std::endl;
            calstate = false;
            incNumOld = "";
        }
        if(FetchData[xpathMyState] == "idle" && FetchData[xpathOtherStateOldNum] == "idle" && FetchData[xpathMyIncNum] == "" && calstate == true)
        {
            std::cout<<"Call ended"<<std::endl;
            calstate = false;
        }
        if(FetchData[xpathMyState] == "busy" && _name == "" && FetchData[xpathMyIncNum] == "")
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
            std::cout<<"Client busu"<<std::endl;
            return false;
        }
        if(CheckNumber[xpathOtherNumber] == _number)
        {
            std::cout<<"this u number "<<std::endl;
            return false;
        }

        else if(CheckNumber[xpathOtherNumber] == number)
        {
            agent->changeData(xpathOtherIncNum, _number);
            agent->changeData(xpathMyState, "active");
            agent->changeData(xpathOtherState, "active");
        }

        outcomingNumber = number;
        icall = 1;
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
        if(icall == 0)
            xpathOtherState = MobileClient::state(incNumOld);
        else if(icall == 1)
            xpathOtherState = MobileClient::state(outcomingNumber);

       std:: map < std::string, std::string > CheckState;
        agent->fetchData(xpathCheckState, CheckState);

        if(CheckState[xpathMyState] == "idle")
        {
            std::cout<<"Call doesn't exist"<<std::endl;
            return false;
        }
        if(CheckState[xpathMyState] == "busy")
        {
            std::cout<<"You already answer the call!!!"<<std::endl;
            return false;
        }
        if(CheckState[xpathMyState] == "active" && icall == 0)
        {
            agent->changeData(xpathMyIncNum, "");
            agent->changeData(xpathMyState, "idle");
            agent->changeData(xpathOtherState, "idle");
        }
        if(CheckState[xpathMyState] == "active" && icall == 1)
        {
            agent->changeData(xpathOtherIncNum, "");
            agent->changeData(xpathMyState, "idle");
            agent->changeData(xpathOtherState, "idle");
        }
        icall = 0;
    }
    bool MobileClient::endCall()
    {
        incNumOld = _incomingNumber;
        std::string xpathMyState = MobileClient::state(_number);
        std::string xpathMyIncNum = MobileClient::incNumber(_number);
        std::string xpathOtherIncNum = MobileClient::incNumber(outcomingNumber);
        std::string xpathCheckState = "/mobile-network:core/subscribers";
        std::string xpathOtherState;
        if(icall == 0){
            xpathOtherState = MobileClient::state(incNumOld);
        }
        else if(icall == 1){
            xpathOtherState = MobileClient::state(outcomingNumber);
        }
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
        if(mapCheckState[xpathMyState] == "busy" && icall == 0)
        {
            agent->changeData(xpathMyState, "idle");
            agent->changeData(xpathOtherState, "idle");
            agent->changeData(xpathMyIncNum, "");
        }
        if(mapCheckState[xpathMyState] == "busy" && icall == 1)
        {
            agent->changeData(xpathMyState, "idle");
            agent->changeData(xpathOtherState, "idle");
            agent->changeData(xpathOtherIncNum, "");
        }
        icall = 0;
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

}