#pragma once
#include <types/ofParameterGroup.h>
#include "ParamNode.h"
namespace ossia
{

/*
 * Class inheriting from ofParameterGroup
 * create ossia node + parameterGroup
 * */

class ParameterGroup : public ofParameterGroup
{
public:
    ParameterGroup() {
    _impl = std::make_shared<ParamNode> ();
    }

    ParameterGroup(const ParameterGroup&) = default;
    ParameterGroup(ParameterGroup&&) = default;
    ParameterGroup& operator=(const ParameterGroup&) = default;
    ParameterGroup& operator=(ParameterGroup&&) = default;

    ~ParameterGroup() = default;

    ParameterGroup & setup(ossia::net::node_base & parentNode,
                         const std::string& name);

    ParameterGroup & setup(ossia::ParameterGroup & parentNode,
                         const std::string& name);
    
//    void createNode(const std::string& name);

    ossia::net::node_base& getNode(){
    return  * _impl->_currentNode;
    }

//    void clearNode();

private:
    std::shared_ptr<ParamNode> _impl;

};
}
