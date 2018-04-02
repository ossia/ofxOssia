#pragma once
#include "ParamNode.h"
#include "ParameterGroup.h"
#include <ossia-cpp98.hpp>
#include <types/ofParameter.h>
#include <iostream>

namespace ossia
{

/*
 * Class inheriting from ofParameter
 * Listeners (listening to OSCquery client(s) and GUI) are enabled
 * By passing a std::type in argument, the OSSIA type is deduced in the class
 **/

template <class DataValue>
class Parameter : public ofParameter<DataValue>
{
private:
  std::shared_ptr<ParamNode> _impl;
  opp::callback_index _callbackIt;

  using ossia_type = MatchingType<DataValue>;

  // Listener for the GUI (but called also when OSCquery client(s) send value)
  void listen(DataValue &data)
  {
    // check if the value to be published is not already published
    if(_impl->cloneNodeValue<DataValue>() != data)
    { // i-score->GUI OK
      _impl->publishValue(data);
    }
  }

  // listen to of update (GUI)
  void enableLocalUpdate()
  {
    this->addListener(this, &Parameter::listen);
  }

  void cleanup()
  {
    if(_impl->_currentNode)
    {
      this->removeListener(this, &Parameter::listen);
      if(_impl->_currentNode.has_parameter()  && _callbackIt)
      {
        _impl->_currentNode.remove_value_callback(_callbackIt);
      }
    }
  }

  // Add remote (e.g. score) callback
  void enableRemoteUpdate()
  {
    if(_impl->_currentNode) //.has_parameter())
    {
      _callbackIt = _impl->_currentNode.set_value_callback([](void* context, const opp::value& val)
      {
          Parameter* self = reinterpret_cast<Parameter*>(context);
          //using value_type = const typename ossia_type::ossia_type;
          if(ossia_type::is_valid(val))
          {
              DataValue data = ossia_type::convertFromOssia(val);
              if(data != self->get())
              {
                  self->set(data);
              }
          }
          else
          {
              std::cerr << "error [ofxOssia::enableRemoteUpdate()] : of and ossia types do not match \n" ;
              // Was: "<< (int) val.getType()  << " " << (int) ossia_type::val << "\n" ;
              return;
          }
      },  this);
    }
  }

public:
  Parameter():
      _impl{std::make_shared<ParamNode>()}
  {
    //_impl = std::make_shared<ParamNode> ();
    cout << "Create Parameter" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from parent: " << _impl->_parentNode.get_name() << endl;
  }

  // creates node and sets the name, the data
  Parameter(
      ossia::ParameterGroup & parentNode,
      const std::string& name,
      DataValue data):
      _impl{std::make_shared<ParamNode>()}
  {
    //_impl = std::make_shared<ParamNode> ();
    _impl->_parentNode = parentNode.getNode();
    _impl->createNode(name, data);
    cout << "Create Parameter" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from parent: " << _impl->_parentNode.get_name() << endl;

    enableLocalUpdate();
    enableRemoteUpdate();
    this->set(name, data);

    parentNode.add(*this);
    return *this;
  }

  // creates node and sets the name, the data, the minimum and maximum value (for the gui)
  Parameter(
      ossia::ParameterGroup & parentNode,
      const std::string& name,
      DataValue data, DataValue min, DataValue max):
      _impl{std::make_shared<ParamNode>()}
  {
    //_impl = std::make_shared<ParamNode> ();
    _impl->_parentNode = parentNode.getNode();
    _impl->createNode(name,data,min,max);
    cout << "Create Parameter" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from parent: " << _impl->_parentNode.get_name() << endl;

    enableLocalUpdate();
    enableRemoteUpdate();
    this->set(name, data, min, max);

    parentNode.add(*this);
    return *this;
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
    ofParameter<DataValue>{other}
  {
    cloneFrom(other);
  }

  Parameter& operator=(const Parameter& other)
  {
    static_cast<ofParameter<DataValue>&>(*this) = other;

    cleanup();
    cloneFrom(other);
    return *this;
  }

  Parameter& operator=(Parameter&& other)
  {
    static_cast<ofParameter<DataValue>&>(*this) = other;

    cleanup();
    cloneFrom(other);
    return *this;
  }

  ~Parameter()
  {
    cout << "Delete Parameter" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from parent: " << _impl->_parentNode.get_name() << " / Has Parameter ? " << _impl->_currentNode.has_parameter() << endl;
    cleanup();
    //_impl.reset();
  }

  // creates node and sets the name, the data
  Parameter & setup(
      ossia::ParameterGroup & parentNode,
      const std::string& name,
      DataValue data)
  {
    _impl->_parentNode = parentNode.getNode();
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
    _impl->_parentNode = parentNode.getNode();
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
    _impl->_parentNode = parentNode.getNode();
    this->set(name, data, min, max);

    parentNode.add(*this);
  }

  // Get the parameter of the node
  opp::node* getAddress() const
  {
    return _impl->_currentNode;
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
