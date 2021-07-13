#include "MobileClient.hpp"
#include <regex>
using namespace Netconfagent;

namespace
{
   std::string inc;
   const std::string mystate = "idle";
   const std::string xPath = "/mobile-network:core/subscribers[number='NUMBER']/LEAF";
   auto construct_X_Path = [](std::string number, std::string leaf)
   {
    std::string result;

    std::regex regexp1("NUMBER");
    std::regex regexp2("LEAF");

    result = regex_replace(xPath, regexp1, number);
    result = regex_replace(result, regexp2, leaf);
    return result;
   };
}
    // MobileClient();
    // MobileClient(std::unique_ptr<Netconfagent::NetConfAgent> agent_NetConf);
    // bool registerClient(std::string number,std::string name);
    // bool unRegisterClient();
    // void handleModuleChange(std::map < std::string, std::string >& _fetch_data) ;
    // void handleOperData(std::string& name, std::string& xPath) const;
    // void handleRpc();
    // void handleNotification();
    // bool makeCall(std::string number);
    // bool answerCall();
    // bool rejectCall();
    // bool endCall();
    // void setName(std::string name);
    // std::string getName();
namespace Mobileclient
{
    MobileClient::MobileClient(std::unique_ptr<Netconfagent::NetConfAgent> agent_NetConf)
      : _agent{move(agent_NetConf)},
        _state{mystate},
        _callState {false}
  {}

    MobileClient::MobileClient() 
    : MobileClient::MobileClient{std::make_unique<Netconfagent::NetConfAgent>()}
  {}
    std::string MobileClient::getName()
    {
        return _name;
    }
    bool MobileClient::registerClient(std::string number,std::string name)
    {
        _name=name;
        _number = number;
        _state = mystate;
        std::string x_path_For_Subscribe = "/mobile-network:core/subscribers[number='" + _number + "']";
        std::string x_path_For_Fetch = "/mobile-network:core/subscribers";
        _agent->initSysrepo();
        if(_name == "")
        {
            std::cout << std::endl << "Name isn`t setted" << std::endl;
            return false;
        }
        else
        {
            _agent->changeData(construct_X_Path(_number, "state"), _state);
            _agent->changeData(construct_X_Path(_number, "number"), number);
            _agent->registerOperData(*this, x_path_For_Subscribe);
            _agent->subscribeForModelChanges(*this);
        }
        std::cout << std::endl << "Client " << _name << " registered with this number " << _number << std::endl;
        return true;
    }
    void MobileClient::handleModuleChange(std::map < std::string, std::string >& _fetch_data) 
    {
        _incomingNumber = _fetch_data[construct_X_Path(_number, "incomingNumber")];
        _state = _fetch_data[construct_X_Path(_number, "state")];
        
        if(_incomingNumber != "" && _state == "active" && _fetch_data[construct_X_Path(_incomingNumber, "state")] == "active")
        {        
            std::cout<<"    Incoming number " << _incomingNumber << " call!"<<std::endl;
            inc = _incomingNumber;
            _callState = false;
        }
        if(_state == "busy" && _fetch_data[construct_X_Path(_outcomingNumber, "state")] == "busy")
        {
            std::cout<<"   Call active  "<<std::endl;
            _callState = true;
        }
        if(_state == "busy" && _fetch_data[construct_X_Path(_incomingNumber, "state")] == "busy")
        {
            std::cout<<"     Call active     "<<std::endl;
            _callState = true;
        }
        if(_callState == false && _state == "idle" && _fetch_data[construct_X_Path(_outcomingNumber, "state")] == "idle" && _outcomingNumber != "")
        {
            std::cout<<"     The call rejected  "<<std::endl;
            _outcomingNumber = "";
        }
        if(_callState == false && _state == "idle" && _fetch_data[construct_X_Path(inc, "state")] == "idle" && inc != "")
        {
            std::cout<<"   The call rejected     "<<std::endl;
            inc = "";
        }
        if(_callState == true && _state == "idle" && _fetch_data[construct_X_Path(_outcomingNumber, "state")] == "idle" )
        {
            std::cout<<"     Call ended     "<<std::endl;
            _outcomingNumber = "";
            _callState = false;
        }
        if(_callState == true && _state == "idle" && _fetch_data[construct_X_Path(inc, "state")] == "idle"  && inc != "")
        {
            std::cout<<"        Call ended     "<<std::endl;
            inc = "";
        }
    }
    void MobileClient::handleOperData(std::string& name, std::string& x_Path) const
    {
        name = _name;
        x_Path = "/mobile-network:core/subscribers[number='" + _number + "']/userName";
    }
    bool MobileClient::makeCall(std::string number)
    {
        std::map < std::string, std::string > check_This_Number;
        _agent->fetchData(check_This_Number);

    if(check_This_Number[construct_X_Path(number, "number")] != number)
    {
        std::cout<<"this number doesn't exist!!!"<<std::endl;
        return false;
    }
    else 
    {
        if(check_This_Number[construct_X_Path(number, "state")] != "idle")
        {
            std::cout<<"Client busy!!!"<<std::endl;
            return false;
        }
        if(check_This_Number[construct_X_Path(number, "number")] == _number)
        {
            std::cout<<"This is your number!!!"<<std::endl;
            return false;
        }
        if(_state != "idle")
        {
            std::cout<<"You already have call!!!"<<std::endl;
            return false;
        }
        else if(check_This_Number[construct_X_Path(number, "number")] == number)
        {
            _agent->changeData(construct_X_Path(number, "incomingNumber"), _number);
            _agent->changeData(construct_X_Path(_number, "state"), "active");
            _agent->changeData(construct_X_Path(number, "state"), "active");
        }
    }
        _outcomingNumber = number;
        return true;
    }

    void MobileClient::setName(std::string name)
    {
        _name = name;
        std::cout << std::endl << "Name " << _name << " setted " << std::endl;
    }

    bool MobileClient::answerCall()
    {
        if(_incomingNumber != "")
        {
        _agent->changeData(construct_X_Path(_number, "state"), "busy");
        _agent->changeData(construct_X_Path(_incomingNumber, "state"), "busy");
        }
        else
        {
            std::cout<<"You don`t have incoming call"<<std::endl;
            return false;
        }
        return true;
    }

    bool MobileClient::rejectCall()
    {
        inc = _incomingNumber;
        if(_state == "idle")
        {
            std::cout<<"Call doesn't exist"<<std::endl;
            return false;
        }
        if(_state == "busy")
        {
            std::cout<<"You already answer the call!!!"<<std::endl;
            return false;
        }
        if(_state == "active" && _incomingNumber != "" && _outcomingNumber == "")
        {
        _agent->changeData(construct_X_Path(_incomingNumber, "state"), "idle");
        _agent->changeData(construct_X_Path(_number, "state"), "idle");
        _agent->changeData(construct_X_Path(_number, "incomingNumber"), "");
        }

        if(_state == "active" && _incomingNumber == "" && _outcomingNumber != "")
        {
        _agent->changeData(construct_X_Path(_outcomingNumber, "incomingNumber"), "");
        _agent->changeData(construct_X_Path(_number, "state"), "idle");
        _agent->changeData(construct_X_Path(_outcomingNumber, "state"), "idle");
        }
        return true;
    }
    bool MobileClient::endCall()
    {
        inc = _incomingNumber;
        if(_state == "idle")
        {
            std::cout<<"Call doesn't exist"<<std::endl;
            return false;
        }
        if(_state == "active")
        {
            std::cout<<"You didn't answer the call!!!"<<std::endl;
            return false;
        }
        if(_state == "busy" && _incomingNumber != "" && _outcomingNumber == "")
        {
        _agent->changeData(construct_X_Path(_incomingNumber, "state"), "idle");
        _agent->changeData(construct_X_Path(_number, "state"), "idle");
        _agent->changeData(construct_X_Path(_number, "incomingNumber"), "");
        }

        if(_state == "busy" && _incomingNumber == "" && _outcomingNumber != "")
        {
        _agent->changeData(construct_X_Path(_number, "state"), "idle");
        _agent->changeData(construct_X_Path(_outcomingNumber, "incomingNumber"), "");
        _agent->changeData(construct_X_Path(_outcomingNumber, "state"), "idle");
        }
        return true;
    }
    bool MobileClient::unRegisterClient()
    {
        std::string x_Path_For_Delete = "/mobile-network:core/subscribers[number='" + _number + "']";
        if(_state == "idle" && _incomingNumber == "")
        {
            _agent->closeSysrepo();

            _agent->deleteData(x_Path_For_Delete);

            _agent->closeSession();
   
            _name.erase();
            _number.erase();
            _incomingNumber.erase();
            _outcomingNumber.erase();
            _state.erase();
            std::cout<<"   Client unregistered "<<std::endl;
            std::cout<<std::endl;
        }
        else
        {
            std::cout<<"Client busy!!!"<<std::endl;
            return false;
        }
        return true;
    }
}