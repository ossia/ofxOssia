////
////  Parameter.cpp
////  ofxOSSIA
////
////  Created by Thomas Pachoud on 24/02/2017.
////
////
//
//#include "Parameter.h"
//
//
////namespace ossia {
////    HasNode::~HasNode() {}
////}
//namespace  ossia {
//    
//    void Parameter<DataValue>::enableLocalUpdate()
//    {
//        this->addListener(_impl.get(), &ParamNode::listen<DataValue>);
//    }
//
//    void Parameter<DataValue>::cleanup()
//    {
//        if(_impl)
//        {
//            this->removeListener(_impl.get(), &ParamNode::listen<DataValue>);
//            if(_impl->_address && _callbackIt)
//            {
//                _impl->_address->remove_callback(*_callbackIt);
//            }
//        }
//    }
//
//    // Add i-score callback
//    void Parameter<DataValue>::enableRemoteUpdate()
//    {
//        if(_impl->_address)
//            _callbackIt = _impl->_address->add_callback(
//                [=](const ossia::value& val)
//                {
//                    using value_type = const typename ossia_type::ossia_type;
//                    if(val.target<value_type>())
//                    {
//                        DataValue data = ossia_type::convertFromOssia(val);
//                        if(data != this->get())
//                        {
//                            this->set(data);
//                        }
//                    }
//                    else
//                    {
//                        std::cerr << "error [ofxOssia::enableRemoteUpdate()] : "<< (int) val.getType()  << " " << (int) ossia_type::val << "\n" ;
//                        return;
//                    }
//                });
//    }
//
//    template <class DataValue>
//    Parameter<DataValue>::Parameter() {
//        _impl = std::make_shared<ParamNode> ();
//    }
//
//    template <class DataValue>
//    void Parameter<DataValue>::cloneFrom(const Parameter& other) {
//        _impl = other._impl;
//        if(other._callbackIt)
//        {
//            enableLocalUpdate();
//            enableRemoteUpdate();
//        }
//    }
//
//    template <class DataValue>
//    Pamareter<DataValue>::Parameter(const Parameter& other):
//    ofParameter<DataValue>{other}
//    {
//        cloneFrom(other);
//    }
//
//    template <class DataValue>
//    Parameter<DataValue>::Parameter(Parameter&& other):
//    ofParameter<DataValue>{other} {
//        cloneFrom(other);
//    }
//
//    template <class DataValue>
//    Parameter<DataValue>::Parameter& operator=(const Parameter& other) {
//        static_cast<ofParameter<DataValue>&>(*this) = other;
//        
//        cleanup();
//        cloneFrom(other);
//        return *this;
//    }
//
//    template <class DataValue>
//    Parameter<DataValue>::Parameter& operator=(Parameter&& other) {
//        static_cast<ofParameter<DataValue>&>(*this) = other;
//        
//        cleanup();
//        cloneFrom(other);
//        return *this;
//    }
//
//    template <class DataValue>
//    Parameter<DataValue>::~Parameter()
//    {
//        ofLog() << "Parameter deleted";
//        cleanup();
//    }
//
//    // creates node and sets the name, the data
//    template <class DataValue>
//    Parameter<DataValue>::Parameter & setup(
//                      ossia::ParameterGroup & parentNode,
//                      const std::string& name,
//                      DataValue data)
//    {
//        _impl->_parentNode = &parentNode.getNode();
//        _impl->createNode(name, data);
//        
//        enableLocalUpdate();
//        enableRemoteUpdate();
//        this->set(name, data);
//        
//        parentNode.add(*this);
//        return *this;
//    }
//
//    // creates node and sets the name, the data, the minimum and maximum value (for the gui)
//    template <class DataValue>
//    Parameter<DataValue>::Parameter & setup(
//                      ossia::ParameterGroup & parentNode,
//                      const std::string& name,
//                      DataValue data, DataValue min, DataValue max)
//    {
//        _impl->_parentNode = &parentNode.getNode();
//        _impl->createNode(name,data,min,max);
//        
//        enableLocalUpdate();
//        enableRemoteUpdate();
//        this->set(name, data, min, max);
//        
//        parentNode.add(*this);
//        return *this;
//    }
//
//    // set without creating node (suppose that a node was created previously)
//    template <class DataValue>
//    Parameter<DataValue>::Parameter & setupNoPublish(
//                               ossia::ParameterGroup & parentNode,
//                               const std::string& name,
//                               DataValue data, DataValue min, DataValue max)
//    {
//        _impl->_parentNode = &parentNode.getNode();
//        this->set(name, data, min, max);
//        
//        parentNode.add(*this);
//    }
//
//
//    // Updates value of the parameter and publish to the node
//    template <class DataValue>
//    void Parameter<DataValue>::update(DataValue data)
//    {
//        _impl->publishValue(data);
//        
//        // change attribute value
//        this->set(data);
//    }
//}
