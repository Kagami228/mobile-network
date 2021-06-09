#include <iostream>
#include <vector>
#include "libsysrepocpp/headers/Session.hpp"

class NetConfAgent
{
    std::shared_ptr<sysrepo::Connection> Connection;
    std::shared_ptr<sysrepo::Session> Session;
    std::shared_ptr<sysrepo::Subscribe> Subscribe;

public:
    NetConfAgent()
    {
    }
    bool initSysrepo()
    {
        Connection = std::make_shared<sysrepo::Connection>();
        Session = std::make_shared<sysrepo::Session>(Connection);
        Subscribe = std::make_shared<sysrepo::Subscribe>(Session);
        std::cout<<"Session began"<<std::endl;

        return 1;
    }

    bool fetchData(const std::string &s_xpath)
    {
         const char *xpath = "/mobile-network:core/subscribers[number='001']";

    try {
        libyang::S_Data_Node data = Session->get_data(xpath);
        /* go through all top-level siblings */
        for (libyang::S_Data_Node &root : data->tree_for()) {
            /* go through all the children of a top-level sibling */
            //for (libyang::S_Data_Node &node : root->tree_dfs()) {
             //   print_node(node);
         //   }
        }
    } catch( const std::exception& e ) {
        std::cout << e.what() << std::endl;
    }
    return 1;
    }
    bool syscrybeForModelChanges();
    bool registrOpenData();
    bool sybscribeForRpc();
    bool notifySysrepo();
    bool changeData();
};
