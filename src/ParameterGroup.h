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
    ParameterGroup() {
        _impl = std::make_shared<ParamNode> ();
        cout << "Create ParameterGroup#" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from parent: " << _impl->_parentNode.get_name() << endl;
    }

    ParameterGroup(const ParameterGroup&) = default;
    ParameterGroup(ParameterGroup&&) = default;
    ParameterGroup& operator=(const ParameterGroup&) = default;
    ParameterGroup& operator=(ParameterGroup&&) = default;

    ~ParameterGroup() {
        cout << "Delete ParameterGroup" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from parent: " << _impl->_parentNode.get_name() << " / Has Parameter ? " << _impl->_currentNode.has_parameter() << endl;
        //_impl.reset();
        //= default
    }

    ParameterGroup & setup(opp::node parentNode,
                           const std::string& name);

    ParameterGroup & setup(ossia::ParameterGroup & parentNode,
                           const std::string& name);
    
//    void createNode(const std::string& name);

    opp::node getNode() const{
    return _impl->_currentNode;
    }

//    void clearNode();

private:
    std::shared_ptr<ParamNode> _impl{};

};
} // namespace ossia 
