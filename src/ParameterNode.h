//
//  ParameterNode.h
//  ofxOSSIA
//
//  Created by Thomas Pachoud on 24/02/2017.
//
//

#pragma once

#include <ossia/ossia.hpp>

namespace ossia {
    /*
     * Class encapsulating node_base* for transparent manipulation
     * */
    
    class ParamNode {
    public:
        ossia::net::node_base* _parentNode{};
        ossia::net::node_base* _currentNode{};
        mutable ossia::net::address_base* _address{};
        
        
        ParamNode () = default;
        ~ParamNode () {
            clearNode();
        }
        
        void clearNode(){
            std::cout<<"paramNode destructor"<<endl;
            
            if (_address != nullptr){
                std::cout<<"clear address " <<endl;
                delete _address;
            }
            
            std::cout<<" before test "<<endl;
            if (_currentNode != nullptr){
                std::cout<<"clear child for param "<<_currentNode->getName()<<endl;
//                _currentNode->clearChildren();
                if (_parentNode != nullptr){
                    std::cout<<"remove this child from parent "<<_parentNode->getName()<<endl;
                    _parentNode->removeChild(*_currentNode);
                    _currentNode = nullptr;
                }
            }
            std::cout<<"paramNode destructor done \n --------"<<endl;
        }
    };
}
