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
    ParameterGroup() {}

    ParameterGroup(const ParameterGroup&) = default;
    ParameterGroup(ParameterGroup&&) = default;
    ParameterGroup& operator=(const ParameterGroup&) = default;
    ParameterGroup& operator=(ParameterGroup&&) = default;

    ~ParameterGroup() = default;

    ParameterGroup & setup(opp::node parentNode,
                           const std::string& name);

    ParameterGroup & setup(ossia::ParameterGroup & parentNode,
                           const std::string& name);
    
//    void createNode(const std::string& name);

    opp::node getNode() const{
    return _impl._currentNode;
    }

//    void clearNode();

private:
    ParamNode _impl{};

};
} // namespace ossia 
