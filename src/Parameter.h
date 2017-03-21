#pragma once
#include "ParamNode.h"
#include "ParameterGroup.h"
#include <types/ofParameter.h>


namespace ossia
{

/*
 * Class inheriting from ofParameter
 * Listeners (listening to i-score and GUI) are enabled
 * By passing a std::type in argument, the OSSIA type is deduced in the class
 **/

template <class DataValue>
class Parameter : public ofParameter<DataValue>
{
private:
  std::shared_ptr<ParamNode> _impl;
  optional<ossia::net::address_base::iterator> _callbackIt;

  using ossia_type = MatchingType<DataValue>;

  // listen to of update (GUI)
  void enableLocalUpdate()
  {
    this->addListener(_impl.get(), &ParamNode::listen<DataValue>);
  }

  void cleanup()
  {
    if(_impl)
    {
      this->removeListener(_impl.get(), &ParamNode::listen<DataValue>);
      if(_impl->_address && _callbackIt)
      {
        _impl->_address->remove_callback(*_callbackIt);
      }
    }
  }

  // Add i-score callback
  void enableRemoteUpdate()
  {
    if(_impl->_address)
      _callbackIt = _impl->_address->add_callback([=](const ossia::value& val)
      {
          using value_type = const typename ossia_type::ossia_type;
          if(val.target<value_type>())
          {
              DataValue data = ossia_type::convertFromOssia(val);
              if(data != this->get())
              {
                  this->set(data);
              }
          }
          else
          {
              std::cerr << "error [ofxOssia::enableRemoteUpdate()] : "<< (int) val.getType()  << " " << (int) ossia_type::val << "\n" ;
              return;
          }
      });
  }

public:
  Parameter() {
    _impl = std::make_shared<ParamNode> ();
  }

  void cloneFrom(const Parameter& other) {
    _impl = other._impl;
    if(other._callbackIt)
    {
      enableLocalUpdate();
      enableRemoteUpdate();
    }
  }

  Parameter(const Parameter& other):
    ofParameter<DataValue>{other}
  {
    cloneFrom(other);
  }

  Parameter(Parameter&& other):
    ofParameter<DataValue>{other} {
    cloneFrom(other);
  }

  Parameter& operator=(const Parameter& other) {
    static_cast<ofParameter<DataValue>&>(*this) = other;

    cleanup();
    cloneFrom(other);
    return *this;
  }

  Parameter& operator=(Parameter&& other) {
    static_cast<ofParameter<DataValue>&>(*this) = other;

    cleanup();
    cloneFrom(other);
    return *this;
  }

  ~Parameter()
  {
    ofLog() << "Parameter deleted";
    cleanup();
  }

  // creates node and sets the name, the data
  Parameter & setup(
      ossia::ParameterGroup & parentNode,
      const std::string& name,
      DataValue data)
  {
    _impl->_parentNode = &parentNode.getNode();
    _impl->createNode(name, data);

    enableLocalUpdate();
    enableRemoteUpdate();
    this->set(name, data);

    parentNode.add(*this);
    return *this;
  }

  // creates node and sets the name, the data, the minimum and maximum value (for the gui)
  Parameter & setup(
      ossia::ParameterGroup & parentNode,
      const std::string& name,
      DataValue data, DataValue min, DataValue max)
  {
    _impl->_parentNode = &parentNode.getNode();
    _impl->createNode(name,data,min,max);

    enableLocalUpdate();
    enableRemoteUpdate();
    this->set(name, data, min, max);

    parentNode.add(*this);
    return *this;
  }

  // set without creating node (suppose that a node was created previously)
  Parameter & setupNoPublish(
      ossia::ParameterGroup & parentNode,
      const std::string& name,
      DataValue data, DataValue min, DataValue max)
  {
    _impl->_parentNode = &parentNode.getNode();
    this->set(name, data, min, max);

    parentNode.add(*this);
  }

  // Get the address of the node
  ossia::net::address_base* getAddress() const
  {
    return _impl->_address;
  }

  // Updates value of the parameter and publish to the node
  void update(DataValue data)
  {
    _impl->publishValue(data);

    // change attribute value
    this->set(data);
  }
};
}
