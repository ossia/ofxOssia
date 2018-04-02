#pragma once

#include <ossia-cpp98.hpp>
#include "ofxOssia.h"
#include "OssiaTypes.h"

namespace ossia { 

/*
 * Class encapsulating node_base* to avoid segfault
 * */

class ParamNode {
public:
  opp::node _parentNode;
  opp::node _currentNode;

  /**
   * Methods to communicate via OSSIA to score or other OSCquery clients
   **/

  void cleanup(const opp::node)
  {
    _currentNode.~node();
  }

  // Creates the node without setting domain
  void createNode (const std::string& name)
  {
    _currentNode = _parentNode.create_child(name);
  }

  template<typename DataValue>
  void createNode(const std::string& name, DataValue data)
  {
    using ossia_type = MatchingType<DataValue>;

    // creates node with parameter
    _currentNode = ossia_type::create_parameter(name, _parentNode);
    //sets value
    _currentNode.set_value(ossia_type::convert(data));
  }

  // Creates the node setting domain
  template<typename DataValue>
  void createNode(const std::string& name, DataValue data, DataValue min, DataValue max)
  {
    using ossia_type = MatchingType<DataValue>;

    /// creates node with parameter
    _currentNode = ossia_type::create_parameter(name, _parentNode);
    //sets value
    _currentNode.set_value(ossia_type::convert(data));

    //sets domain
    _currentNode.set_min(ossia_type::convert(min));
    _currentNode.set_max(ossia_type::convert(max));
  }

  // Publishes value to the node
  template<typename DataValue>
  void publishValue(DataValue other)
  {
    using ossia_type = MatchingType<DataValue>;
    _currentNode.set_value(ossia_type::convert(other));
  }

  // Pulls the node value
  template<typename DataValue>
  DataValue pullNodeValue()
  {
    using ossia_type = MatchingType<DataValue>;

    try
    {
      auto val = _currentNode.get_value();
      if(ossia_type::is_valid(val))
        return ossia_type::convertFromOssia(val);
      else
        std::cerr <<  "error [ofxOssia::pullNodeValue()] : of and ossia types do not match \n" ; // Was:
                   // <<(int) val.getType()  << " " << (int) ossia_type::val << "\n" ; // Can we still do that with safeC++ ??
      return {};
    }
    catch(std::exception& e)
    {
      std::cerr <<  "error [ofxOssia::pullNodeValue()] : " << e.what() << "\n" ;
      return {};

    }

    catch(...)
    {
      auto val = _currentNode.get_value();
      std::cerr <<  "error [ofxOssia::pullNodeValue()] : : of and ossia types do not match \n" ; // Was:
                 // << ossia::value_to_pretty_string(val)  << " " << (int) ossia_type::val << "\n" ; // Can we still do that with safeC++ ??
      return {};
    }
  }


  // Pulls the node value
  template<typename DataValue>
  DataValue cloneNodeValue()
  {
    using ossia_type = MatchingType<DataValue>;

    try
    {
      auto val = _currentNode.get_value();
      if(ossia_type::is_valid(val))
        return ossia_type::convertFromOssia(val);
      else
          std::cerr <<  "error [ofxOssia::cloneNodeValue()] : of and ossia types do not match\n" ; // Was:
                     // <<(int) val.getType()  << " " << (int) ossia_type::val << "\n" ; // Can we still do that with safeC++ ??
        return {};
    }
    catch(std::exception& e)
    {
      std::cerr <<  "error [ofxOssia::cloneNodeValue()] : " << e.what() << "\n" ;
      return {};

    }

    catch(...)
    {
      auto val = _currentNode.get_value();
      std::cerr <<  "error [ofxOssia::cloneNodeValue()] : : of and ossia types do not match \n" ; // Was:
                 // << ossia::value_to_pretty_string(val)  << " " << (int) ossia_type::val << "\n" ; // Can we still do that with safeC++ ??
      return {};
    }
  }

  ParamNode() = default;

  ParamNode(opp::oscquery_server& dev):
      _parentNode{},
      _currentNode{dev.get_root_node()}
  {
      std::cout << "Create root Node ParamNode " << _currentNode.get_name() << "\n";
  }

  ParamNode(opp::node parentNode,
             const std::string& name):
      _parentNode{parentNode},
      _currentNode{_parentNode.create_child(name)}
  {
      std::cout << "Create ParamNode " << name << " from  parent: " << parentNode.get_name() << "\n";
  }


  ParamNode(const ParamNode&) = default;
  ParamNode(ParamNode&&) = default;
  ParamNode& operator=(const ParamNode&) = default;
  ParamNode& operator=(ParamNode&&) = default;

  ~ParamNode ()

  {
    if (_currentNode && _parentNode)
    {
      cout << " -- Deleting ParamNode: " << _currentNode.get_name() << '\n';
      _currentNode.remove_children();
      _parentNode.remove_child(_currentNode.get_name());
    }
  }
};
} // namespace ossia 
