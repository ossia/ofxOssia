#pragma once

#include <ossia-cpp98.hpp>
#include "OssiaTypes.h"

namespace ossia { 

/*
 * Class encapsulating node_base* to avoid segfault
 * */
class ParamNode {
public:
  opp::node _parentNode{};
  opp::node _currentNode{};

// TODO: check if we still need it to be mutable
  mutable opp::node _parameter{};
  /**
   * Methods to communicate via OSSIA to i-score
   **/
  // Creates the node without setting domain
  void cleanup(const opp::node)
  {
    _currentNode.~node();
  }

  void createNode (const std::string& name)
  {
    _currentNode = _parentNode.create_child(name);
    // TODO : do we still need this next one ? I assumed not
    // _currentNode->about_to_be_deleted.connect<ParamNode, &ParamNode::cleanup>(this);
  }

  template<typename DataValue>
  void createNode(const std::string& name, DataValue data)
  {
    using ossia_type = MatchingType<DataValue>;

    //creates node
    _parameter = ossia_type::create_parameter(name, _parentNode);
    //sets value
    _parameter.set_value(ossia_type::convert(data));
  }

  // Creates the node setting domain
  template<typename DataValue>
  void createNode(const std::string& name, DataValue data, DataValue min, DataValue max)
  {
    using ossia_type = MatchingType<DataValue>;

    //creates node
    _parameter = ossia_type::create_parameter(name, _parentNode);
    //sets value
    _parameter.set_value(ossia_type::convert(data));

    //sets domain
    _parameter.set_min(ossia_type::convert(min));
    _parameter.set_max(ossia_type::convert(max));
  }

  // Publishes value to the node
  template<typename DataValue>
  void publishValue(DataValue other)
  {
    using ossia_type = MatchingType<DataValue>;
    _parameter.set_value(ossia_type::convert(other));
  }

  // Pulls the node value
  template<typename DataValue>
  DataValue pullNodeValue()
  {
    using ossia_type = MatchingType<DataValue>;

    try
    {
      auto val = _parameter.get_value();
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
      auto val = _parameter.get_value();
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
      auto val = _parameter.get_value();
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
      auto val = _parameter.get_value();
      std::cerr <<  "error [ofxOssia::cloneNodeValue()] : : of and ossia types do not match \n" ; // Was:
                 // << ossia::value_to_pretty_string(val)  << " " << (int) ossia_type::val << "\n" ; // Can we still do that with safeC++ ??
      return {};
    }
  }

  ParamNode () = default;

  ~ParamNode ()
  {
    if (_currentNode.valid() && _parentNode.valid())
    {
      std::vector<opp::node> children_list;
      children_list = _currentNode.get_children();
      for (auto child : children_list)
           _currentNode.remove_child(child.get_name());
      _parentNode.remove_child(_currentNode.get_name());
    }
  }
};
} // namespace ossia 
