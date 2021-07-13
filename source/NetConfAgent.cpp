#include "NetConfAgent.hpp"
#include "MobileClient.hpp"

using namespace Mobileclient;

namespace
{
    const int MAX_LEN = 100;
    void write_node(libyang::S_Data_Node &node, std::map <std::string, std::string>& map);
    void print_value(sysrepo::S_Val value);
}
    // virtual ~NetConfAgent();
    // bool virtual initSysrepo();
    // bool virtual fetchData(std::map <std::string, std::string>& map);
    // bool virtual subscribeForModelChanges(Mobileclient::MobileClient& client);
    // bool virtual registerOperData(Mobileclient::MobileClient& client, std::string xpathForSubscribe);
    // bool virtual subscribeForRpc(std::string _module_name, std::string _rpc_xpath);
    // bool notifySysrepo(std::string _module_name); 
    // bool virtual changeData(std::string _xpath, std::string newValue);
    // bool virtual deleteData(std::string _xpath);
    // bool virtual closeSysrepo();
    // bool virtual closeSession();


namespace Netconfagent
{

    NetConfAgent::~NetConfAgent() {}

    bool NetConfAgent::initSysrepo() 
    {
        Connection = std::make_unique<sysrepo::Connection>();
        Session = std::make_unique<sysrepo::Session>(Connection);
        Subscribe = std::make_unique<sysrepo::Subscribe>(Session);
        return true;
    }

    bool NetConfAgent::closeSysrepo() 
    {
        Connection.reset();
        Subscribe.reset();
        return true;
    }

    bool NetConfAgent::closeSession() 
    {
        Session->session_stop();
        Session.reset();
        return true;
    }

    bool NetConfAgent::fetchData(std::map <std::string, std::string>& map)
    {
        std::string _xpath = "/mobile-network:core/subscribers";
        const char *xpath = _xpath.c_str();
        try {
            libyang::S_Data_Node data = Session->get_data(xpath);
            for (libyang::S_Data_Node &root : data->tree_for()) {
                for (libyang::S_Data_Node &node : root->tree_dfs()) {
                    write_node(node, map);
                }
            }
        } catch( const std::exception& e ) {
            std::cout << e.what() << std::endl;
        }

        return true;
    }
    bool NetConfAgent::subscribeForModelChanges(Mobileclient::MobileClient& client)
    {
        const char *module_name = "mobile-network";
            auto s_F_M_S = [=, &client] (sysrepo::S_Session Session, const char *module_name, const char *xpath, sr_event_t event,
                uint32_t request_id) mutable {
                    std::map < std::string, std::string > fetch_Data_;
                    if(event == SR_EV_DONE)
                    {
                    fetchData(fetch_Data_);
                    client.handleModuleChange(fetch_Data_);
                    }
                return SR_ERR_OK;
            };
            Subscribe->module_change_subscribe(module_name, s_F_M_S);

        return true;
    }

    bool NetConfAgent::registerOperData(Mobileclient::MobileClient& client, std::string xpathForSubscribe)
    {
        const char *module_name = "mobile-network";

        const char *xpathForSubscribePtr = xpathForSubscribe.c_str();

            auto r_O_D = [=, &client] (sysrepo::S_Session Session, const char *module_name, const char *path, const char *request_xpath,
                uint32_t request_id, libyang::S_Data_Node &parent) {
                std::string name, xPath;
                client.handleOperData(name, xPath);
                libyang::S_Context ctx = Session->get_context();
                libyang::S_Module mod = ctx->get_module(module_name);
                parent->new_path(ctx, xPath.c_str(), name.c_str(), LYD_ANYDATA_CONSTSTRING, 0);
                return SR_ERR_OK;
                
            };
            Subscribe->oper_get_items_subscribe(module_name, r_O_D, xpathForSubscribePtr);
        return true;
    }

    bool NetConfAgent::changeData(std::string _xpath, std::string _newValue)
    {
        const char *xpath = _xpath.c_str();
        const char *newValue = _newValue.c_str();

        try {
            auto value = std::make_shared<sysrepo::Val>(newValue);
            Session->set_item(xpath, value);

            Session->apply_changes();
        } catch( const std::exception& e ) {
            std::cout << e.what() << std::endl;
        }
        return true;
    }

    bool NetConfAgent::subscribeForRpc(std::string _module_name, std::string _rpc_xpath)
    {
        const char *module_name = _module_name.c_str();
        const char *rpc_xpath = _rpc_xpath.c_str();
        try {
            std::cout<<"Application will make an rpc call in "<< module_name<<std::endl;
            auto s_F_R = [](sysrepo::S_Session session, const char* op_path, const sysrepo::S_Vals input, sr_event_t event, 
            uint32_t request_id, sysrepo::S_Vals_Holder output) 
            {
                std::cout << "\n ========== RPC CALLED ==========\n" << std::endl;
                auto out_vals = output->allocate(1);
                    print_value(input->val(0));

                out_vals->val(0)->set("/mobile-network:delete/status",
                        "data deleted",
                        SR_STRING_T);

                return SR_ERR_OK;
            };
            std::cout << "\n ========== SUBSCRIBE TO RPC CALL ==========\n" <<std::endl;
            Subscribe->rpc_subscribe(rpc_xpath, s_F_R, 1);
            auto in_vals = std::make_shared<sysrepo::Vals>(1);
            in_vals->val(0)->set("/mobile-network:dlete/input-number",
                            "00000", SR_STRING_T);
            std::cout << "\n ========== START RPC CALL ==========\n" << std::endl;
            auto out_vals = Session->rpc_send(rpc_xpath, in_vals);
            std::cout << "\n ========== PRINT RETURN VALUE ==========\n" << std::endl;
            for(size_t n=0; n < out_vals->val_cnt(); ++n)
                print_value(out_vals->val(n));
            std::cout << "\n ========== END PROGRAM ==========\n" <<std::endl;
        } catch( const std::exception& e ) {
            std::cout << e.what() << std::endl;
            return -1;
        }
        return true;
    }

    bool NetConfAgent::notifySysrepo(std::string _module_name)
    {
        const char *module_name = _module_name.c_str();
        try {

            std::cout<<"Application will send notification in "<<module_name<<std::endl;

            auto n_S = [] (sysrepo::S_Session Session, const sr_ev_notif_type_t notif_type, const char *path,
                const sysrepo::S_Vals vals, time_t timestamp) {
                std::cout << "\n ========== NOTIF RECEIVED ==========\n" << std::endl;

                for(size_t n = 0; n < vals->val_cnt(); ++n) {
                    print_value(vals->val(n));
                }
            };
            std::cout << "\n ========== SUBSCRIBE TO NOTIF ==========\n" << std::endl;
            Subscribe->event_notif_subscribe(module_name, n_S);
            auto in_vals = std::make_shared<sysrepo::Vals>(2);
            in_vals->val(0)->set("/mobile-network:test-notif/val1", "some-value", SR_STRING_T);
            in_vals->val(1)->set("/mobile-network:test-notif/val2", "some-other-value", SR_STRING_T);
            std::cout << "\n ========== START NOTIF SEND ==========\n" << std::endl;
            Session->event_notif_send("/mobile-network:test-notif", in_vals);
            libyang::S_Context ctx = Connection->get_context();
            libyang::S_Module mod = ctx->get_module(module_name);
            auto in_trees = std::make_shared<libyang::Data_Node>(ctx, "/mobile-network:test-notif", nullptr, LYD_ANYDATA_CONSTSTRING, 0);
            std::make_shared<libyang::Data_Node>(libyang::Data_Node(in_trees, mod, "val1", "some-value"));
            std::make_shared<libyang::Data_Node>(libyang::Data_Node(in_trees, mod, "val2", "some-other-value"));
            std::cout << "\n ========== END PROGRAM ==========\n" << std::endl;
        } catch( const std::exception& e ) {
            std::cout << e.what() << std::endl;
            return -1;
        }
        return true;
    }
    bool NetConfAgent::deleteData(std::string _xpath)
    {
        Session->delete_item(_xpath.c_str());
        Session->apply_changes();
        return true;
    }
    
}

namespace
{
    void write_node(libyang::S_Data_Node &node, std::map <std::string, std::string>& map)
    {
        std::string path, value;
        libyang::S_Schema_Node schema = node->schema();
        path = node->path();
        switch (schema->nodetype()) 
        {
            case LYS_CONTAINER:
            {
                break;
            }
            case LYS_LEAF:
            {
                libyang::Data_Node_Leaf_List leaf(node);
                libyang::Schema_Node_Leaf sleaf(schema);
                value = leaf.value_str();
                break;
            }
            case LYS_LEAFLIST:
            {
                libyang::Data_Node_Leaf_List leaflist(node);
                value = leaflist.value_str();
                break;
            }
            case LYS_LIST:
            {
                break;
            }
            default:
                break;
        }
        map.emplace(path, value);
    }

    void print_value(sysrepo::S_Val value)
    {
        std::cout << value->xpath();
        std::cout << " ";
        switch (value->type())
        {
            case SR_CONTAINER_T:
            case SR_CONTAINER_PRESENCE_T:
                std::cout << "(container)" << std::endl;
                break;
            case SR_LIST_T:
                std::cout << "(list instance)" << std::endl;
                break;
            case SR_STRING_T:
                std::cout << "= " << value->data()->get_string() << std::endl;;
                break;
            case SR_BOOL_T:
            if (value->data()->get_bool())
                std::cout << "= true" << std::endl;
            else
                std::cout << "= false" << std::endl;
                break;
            case SR_ENUM_T:
                std::cout << "= " << value->data()->get_enum() << std::endl;;
                break;
            case SR_UINT8_T:
                std::cout << "= " << unsigned(value->data()->get_uint8()) << std::endl;
                break;
            case SR_UINT16_T:
                std::cout << "= " << unsigned(value->data()->get_uint16()) << std::endl;
                break;
            case SR_UINT32_T:
                std::cout << "= " << unsigned(value->data()->get_uint32()) << std::endl;
                break;
            case SR_UINT64_T:
                std::cout << "= " << unsigned(value->data()->get_uint64()) << std::endl;
                break;
            case SR_INT8_T:
                std::cout << "= " << value->data()->get_int8() << std::endl;
                break;
            case SR_INT16_T:
                std::cout << "= " << value->data()->get_int16() << std::endl;
                break;
            case SR_INT32_T:
                std::cout << "= " << value->data()->get_int32() << std::endl;
                break;
            case SR_INT64_T:
                std::cout << "= " << value->data()->get_int64() << std::endl;
                break;
            case SR_IDENTITYREF_T:
                std::cout << "= " << value->data()->get_identityref() << std::endl;
                break;
            case SR_BITS_T:
                std::cout << "= " << value->data()->get_bits() << std::endl;
                break;
            case SR_BINARY_T:
                std::cout << "= " << value->data()->get_binary() << std::endl;
                break;
            default:
                std::cout << "(unprintable)" << std::endl;
        }
        return;
    }
}