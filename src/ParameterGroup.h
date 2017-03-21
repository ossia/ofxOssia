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
private:
  std::shared_ptr<ParamNode> _impl;


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
                         const std::string& name)
  {
    // TODO this is weird
    _impl->_parentNode = nullptr;
    _impl->_currentNode = &parentNode;
    this->setName(name);

    return *this;
  }


  ParameterGroup & setup(ossia::ParameterGroup & parentNode,
                         const std::string& name)
  {
    _impl->_parentNode = &parentNode.getNode();
    _impl->createNode(name);
    this->setName(_impl->_currentNode->getName());

    parentNode.add(*this);

    return *this;
  }

  ossia::net::node_base& getNode(){
    return  * _impl->_currentNode;
  }

};
}
