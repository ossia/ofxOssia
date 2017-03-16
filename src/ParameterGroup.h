//
//  ParameterGroup.h
//  ofxOSSIA
//
//  Created by Thomas Pachoud on 24/02/2017.
//
//

#pragma once
#include "ofMain.h"
#include "ParameterNode.h"

/*
 * Class inheriting from ofParameterGroup
 * create ossia node + parameterGroup
 * */
namespace ossia {
    
    class ParameterGroup : public ofParameterGroup
    {
    private:
        
        std::shared_ptr <ParamNode> nodes;
        
        void createNode (const std::string& name);
        
    public:
        ParameterGroup() {
            nodes = std::make_shared<ParamNode> ();
        }
        
        ~ParameterGroup() {
//            clearNode();
        }
        
        ParameterGroup & setup(ossia::net::node_base & parentNode, const std::string& name);
        ParameterGroup & setup(ossia::ParameterGroup & parentNode, const std::string& name);
        
        ossia::net::node_base& getNode(){
            return * nodes->_currentNode;
        }
        
        void clearNode();
        
    };

}
