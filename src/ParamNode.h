#pragma once

// #include "OssiaTypes.h" // Probably not necessary any more
#include <ossia-cpp/ossia-cpp98.hpp>

// namespace ossia { // Probably not necessary any more

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
  void cleanup(const opp::node&)
  {
    _currentNode = nullptr;
    _parameter = nullptr;
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
    _parameter.set_value(data);
  }

  // Creates the node setting domain
  template<typename DataValue>
  void createNode(const std::string& name, DataValue data, DataValue min, DataValue max)
  {
    using ossia_type = MatchingType<DataValue>;

    //creates node
    _parameter = ossia_type::create_parameter(name, _parentNode);
    //sets value
    _parameter.set_value(data);

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
    using value_type = typename ossia_type::ossia_type;

    try
    {
      auto val = _parameter.fetch_value();
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
      auto val = _parameter.get_value();
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
      auto val = _parameter.get_value();
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
      _parentNode.remove_child(*_currentNode);
    }
  }
};
// } // namespace ossia 
