//
//  ParameterGroup.cpp
//  ofxOSSIA
//
//  Created by Thomas Pachoud on 24/02/2017.
//
//

#include "ParameterGroup.h"
#include "Parameter.h"

namespace ossia {
    void ParameterGroup::createNode(const std::string& name){
        nodes->_currentNode = nodes->_parentNode->createChild(name);
    }

    ParameterGroup & ParameterGroup::setup(ossia::net::node_base & parentNode,
                           const std::string& name)
    {
        nodes->_parentNode = &parentNode;
        nodes->_currentNode = nodes->_parentNode;
        this->setName(name);
        
        return *this;
    }

    ParameterGroup & ParameterGroup::setup(ossia::ParameterGroup & parentNode,
                           const std::string& name)
    {
        nodes->_parentNode = &parentNode.getNode();
        createNode(name);
        this->setName(nodes->_currentNode->getName());
        
        parentNode.add(*this);
        
        return *this;
    }

    void ParameterGroup::clearNode(){
        while (this->size()>0){
            ofAbstractParameter * ap = &(this->back());
            ossia::ParameterGroup * p = dynamic_cast<ossia::ParameterGroup *> (ap);
            if (p != nullptr){
                p->clearNode();
            }
            else {

                // comment clearer parameter simple?
                // encaps dans HasNode?
                
//                ossia::Parameter * pp = dynamic_cast<ossia::Parameter *> (ap);
//                if (pp != nullptr){
//                    pp->clearNode();
//                }
            }
        }
        nodes->clearNode();
    }
}
