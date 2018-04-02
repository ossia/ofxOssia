#pragma once
#include <ossia-cpp98.hpp>
#include <types/ofParameterGroup.h>
#include "ParamNode.h"

namespace ossia { 

/*
 * Class inheriting from ofParameterGroup
 * create ossia node + parameterGroup
 * */

class ParameterGroup : public ofParameterGroup
{
public:
    ParameterGroup()
        //:_impl{std::make_shared<ParamNode>()}
    {
        cout << "Create  EmptyParameterGroup" << endl;// << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from parent: " << _impl->_parentNode.get_name() << endl;
    }

    ParameterGroup(opp::oscquery_server& dev):
        _impl{std::make_shared<ParamNode>(dev)}
    {
        //_impl = std::make_shared<ParamNode>();
        //_impl->_currentNode = dev.get_root_node();
        this->setName(dev.get_root_node().get_name());
        cout << "Create ParameterGroup#" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from root " << _impl->_parentNode.get_name() << endl;
    }

    ParameterGroup(ossia::ParameterGroup & parentNode,
                   const std::string& name):
        _impl{std::make_shared<ParamNode>(parentNode.getNode(), name)}
    {
        //_impl = std::make_shared<ParamNode>();
        //_impl->_parentNode = parentNode.getNode();
        //_impl->createNode(name);
        this->setName(_impl->_currentNode.get_name());
        parentNode.add(*this);
        cout << "Create ParameterGroup#" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from parent: " << _impl->_parentNode.get_name() << endl;
    }


    ~ParameterGroup() {
        cout << "Delete ParameterGroup" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from parent: " << _impl->_parentNode.get_name() << " / Has Parameter ? " << _impl->_currentNode.has_parameter() << endl;
        //_impl.reset();
        //= default
    }

    ParameterGroup& setup(opp::oscquery_server& dev);

    ParameterGroup& setup(ossia::ParameterGroup& parentNode,
                           const std::string& name);
    
//    void createNode(const std::string& name);

    opp::node getNode() const{
    return _impl->_currentNode;
    }

//    void clearNode();

private:
    std::shared_ptr<ParamNode> _impl;

};
} // namespace ossia 
