//
//  ParameterGroup.cpp
//  ofxOSSIA
//
//  Created by Thomas Pachoud on 24/02/2017.
//  Updated to using SafeC++ implementation by Pascal Baltazar in 03/2018
//
//

#include "ParameterGroup.h"
//#include "Parameter.h"

namespace ossia { 
//    void ParameterGroup::createNode(const std::string& name){
//        nodes->_currentNode = nodes->_parentNode->create_child(name);
//    }

    ParameterGroup& ParameterGroup::setup(opp::oscquery_server& dev)
    {
        _impl=std::make_shared<ParamNode>(dev);
        //nodes->_parentNode = &parentNode;
        // TODO this is weird
        //_impl._parentNode = nullptr;
        //_impl->_currentNode = dev.get_root_node();
        //_impl->createNode(name);
        this->setName(_impl->_currentNode.get_name());

        cout << "Setup ParameterGroup#" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from root " << _impl->_parentNode.get_name() << endl;

        return *this;
    }

    ParameterGroup& ParameterGroup::setup(
                            ossia::ParameterGroup& parentNode,
                            const std::string& name)
    {
        _impl=std::make_shared<ParamNode>(parentNode.getNode(), name);
        //_impl->_parentNode = parentNode.getNode();
        //_impl->createNode(name);
        this->setName(_impl->_currentNode.get_name());
        
        parentNode.add(*this);
        cout << "Setup ParameterGroup#" << _impl.use_count() << " : " << _impl->_currentNode.get_name() << " from parent: " << _impl->_parentNode.get_name() << endl;
        
        return *this;
    }
    
//    ParameterGroup::~ParameterGroup(){
//        while (this->size()>0){
//            this->remove(this->back());
//        }
//        this->clear();
//    }
} // namespace ossia 
