#pragma once

#include "OssiaTypes.h"
#include <ossia/network/base/node.hpp>
#include <ossia/network/base/parameter.hpp>
#include <ossia/editor/value/value.hpp>
#include <ossia/network/domain/domain.hpp>

namespace ossia
{

/*
 * Class encapsulating node_base* to avoid segfault
 * */
class ParamNode {
public:
  ossia::net::node_base* _parentNode{};
  ossia::net::node_base* _currentNode{};
  mutable ossia::net::parameter_base* _parameter{};

  /**
   * Methods to communicate via OSSIA to i-score
   **/
  // Creates the node without setting domain
  void cleanup(const ossia::net::node_base&)
  {
    _currentNode = nullptr;
    _parameter = nullptr;
  }

  void createNode (const std::string& name)
  {
    _currentNode = _parentNode->create_child(name);
    _currentNode->about_to_be_deleted.connect<ParamNode, &ParamNode::cleanup>(this);
  }

  template<typename DataValue>
  void createNode(const std::string& name, DataValue data)
  {
    using ossia_type = MatchingType<DataValue>;

    //creates node
    this->createNode(name);

    //set value
    _parameter = _currentNode->create_parameter(ossia_type::val);
    _parameter->push_value(ossia_type::convert(data));
  }

  // Creates the node setting domain
  template<typename DataValue>
  void createNode(const std::string& name, DataValue data, DataValue min, DataValue max)
  {
    using ossia_type = MatchingType<DataValue>;

    //creates node
    this->createNode(name);

    //set value
    _parameter = _currentNode->create_parameter(ossia_type::val);
    _parameter->push_value(ossia_type::convert(data));
    _parameter->set_domain(ossia::make_domain(ossia_type::convert(min),
                                           ossia_type::convert(max)));
    _parameter->set_unit(typename ossia_type::ossia_unit{});
  }

  // Publishes value to the node
  template<typename DataValue>
  void publishValue(DataValue other)
  {
    using ossia_type = MatchingType<DataValue>;
    _parameter->push_value(ossia_type::convert(other));
  }

  // Pulls the node value
  template<typename DataValue>
  DataValue pullNodeValue()
  {
    using ossia_type = MatchingType<DataValue>;
    using value_type = typename ossia_type::ossia_type;

    try
    {
      auto val = _parameter->fetch_value();
      if(val.template target<value_type>())
        return ossia_type::convertFromOssia(val);
      else
        std::cerr <<  "error [ofxOssia::pullNodeValue()] : "<<(int) val.getType()  << " " << (int) ossia_type::val << "\n" ;
      return {};
    }
    catch(std::exception& e)
    {
      std::cerr <<  "error [ofxOssia::pullNodeValue()] : " << e.what() << "\n" ;
      return {};

    }

    catch(...)
    {
      auto val = _parameter->value();
      std::cerr <<  "error [ofxOssia::pullNodeValue()] : "<< ossia::value_to_pretty_string(val)  << " " << (int) ossia_type::val << "\n" ;
      return {};
    }
  }


  // Pulls the node value
  template<typename DataValue>
  DataValue cloneNodeValue()
  {
    using ossia_type = MatchingType<DataValue>;
    using value_type = typename ossia_type::ossia_type;

    try
    {
      auto val = _parameter->value();
      if(val.template target<value_type>())
        return ossia_type::convertFromOssia(val);
      else
        std::cerr <<  "error [ofxOssia::pullNodeValue()] : "<<(int) val.getType()  << " " << (int) ossia_type::val << "\n" ;
      return {};
    }
    catch(std::exception& e)
    {
      std::cerr <<  "error [ofxOssia::pullNodeValue()] : " << e.what() << "\n" ;
      return {};

    }

    catch(...)
    {
      auto val = _parameter->value();
      std::cerr <<  "error [ofxOssia::pullNodeValue()] : "<< ossia::value_to_pretty_string(val)  << " " << (int) ossia_type::val << "\n" ;
      return {};
    }
  }

  ParamNode () = default;
  ~ParamNode ()
  {
    if (_currentNode && _parentNode)
    {
      _currentNode->clear_children();
      _parentNode->remove_child(*_currentNode);
    }
  }
};
}
