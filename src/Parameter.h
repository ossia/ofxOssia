#pragma once

#include "ofMain.h"

namespace ossia
{


    /**
     * These classes contain the conversion mechanism from and to
     * the compatible OSSIA & OpenFrameworks types.
     *
     * GenericMatchingType is used for types where it is straightforward
     * (int, bool, string, etc...).
     */
    template<typename> struct MatchingType;

    template<typename OfType, typename OssiaType, ossia::val_type OssiaTypeEnum>
    struct GenericMatchingType
    {
        using ofx_type = OfType;
        using ossia_type = OssiaType;
        static constexpr const auto val = OssiaTypeEnum;
        using ossia_unit = ossia::unit_t;

        static ofx_type convertFromOssia(const ossia::value& v)
        {
            return v.get<ossia_type>();
        }

        static ossia_type convert(ofx_type f)
        {
            return f;
        }
    };

    template<> struct MatchingType<float> final :
            public GenericMatchingType<float, float, ossia::val_type::FLOAT>
    {
    };

    template<> struct MatchingType<int> final :
            public GenericMatchingType<int, int, ossia::val_type::INT>
    {
    };

    template<> struct MatchingType<bool> final :
            public GenericMatchingType<bool, bool, ossia::val_type::BOOL>
    {
    };

    template<> struct MatchingType<char> final :
            public GenericMatchingType<char, char, ossia::val_type::CHAR>
    {
    };

    template<> struct MatchingType<std::string> final :
            public GenericMatchingType<std::string, std::string, ossia::val_type::STRING>
    {
    };

    template<> struct MatchingType<double> {
        using ofx_type = double;
        static constexpr const auto val = ossia::val_type::FLOAT;
        using ossia_type = float;
        using ossia_unit = ossia::unit_t;

        static ofx_type convertFromOssia(const ossia::value& v)
        {
            return v.get<ossia_type>();
        }

        static ossia_type convert(ofx_type f)
        {
            return float(f);
        }
    };

    template<> struct MatchingType<ofVec2f> {
        using ofx_type = ofVec2f;
        static constexpr const auto val = ossia::val_type::VEC2F;
        using ossia_type = std::array<float, 2>;
        using ossia_unit = ossia::unit_t;

        static ofx_type convertFromOssia(const ossia::value& v)
        {
            const auto& t = v.get<ossia_type>();
            return ofx_type(t[0], t[1]);
        }

        static ossia_type convert(ofx_type f)
        {
            return ossia::make_vec(f.x, f.y);
        }
    };

    template<> struct MatchingType<ofVec3f> {
        using ofx_type = ofVec3f;
        static constexpr const auto val = ossia::val_type::VEC3F;
        using ossia_type = std::array<float, 3>;
        using ossia_unit = ossia::unit_t;

        static ofx_type convertFromOssia(const ossia::value& v)
        {
            const auto& t = v.get<ossia_type>();
            return ofx_type(t[0], t[1], t[2]);
        }

        static ossia_type convert(ofx_type f)
        {
            return ossia::make_vec(f.x, f.y, f.z);
        }
    };

    template<> struct MatchingType<ofColor> {
        using ofx_type = ofColor;
        static constexpr const auto val = ossia::val_type::VEC4F;
        using ossia_type = std::array<float, 4>;
        using ossia_unit = ossia::rgba_u;

        static ofx_type convertFromOssia(const ossia::value& v)
        {
            const auto& t = v.get<ossia_type>();
            return ofx_type(t[0] * 255., t[1] * 255., t[2] * 255., t[3] * 255.);
        }

        static ossia_type convert(ofx_type f)
        {
            return ossia::make_vec(f.r / 255., f.g / 255., f.b / 255., f.a / 255.);
        }
    };
        
        template<> struct MatchingType<ofFloatColor> {
            using ofx_type = ofFloatColor;
            static constexpr const auto val = ossia::val_type::VEC4F;
            using ossia_type = std::array<float, 4>;
            using ossia_unit = ossia::rgba_u;
            
            static ofx_type convertFromOssia(const ossia::value& v)
            {
                const auto& t = v.get<ossia_type>();
                return ofx_type(t[0], t[1], t[2], t[3]);
            }
            
            static ossia_type convert(ofx_type f)
            {
                return ossia::make_vec(f.r, f.g, f.b, f.a);
            }
        };

    /*
     * Class encapsulating node_base* to avoid segfault
     * */
    
    class ParamNode {
    public:
        ossia::net::node_base* _parentNode{};
        ossia::net::node_base* _currentNode{};
        mutable ossia::net::address_base*  _address{};

        
        ParamNode () = default;
        ~ParamNode () {
            std::cout<<"delete param node"<<endl;
            if (_currentNode != nullptr)
                _currentNode->clearChildren();
            if (_parentNode != nullptr)
                _parentNode->removeChild(*_currentNode);
            if (_currentNode != nullptr)
                delete _currentNode;
            if (_address != nullptr)
                delete _address;
        }
        
        
    };
    
    /*
     * Class inheriting from ofParameterGroup
     * create ossia node + parameterGroup
     * */
    
    class ParameterGroup : public ofParameterGroup
    {
    private:
        
        std::shared_ptr <ParamNode> nodes;
        
        void createNode (const std::string& name)
        {
            nodes->_currentNode = nodes->_parentNode->createChild(name);
        }
        
    public:
        ParameterGroup() {
            nodes = std::make_shared<ParamNode> ();
        }
        
        ~ParameterGroup() = default;

        ParameterGroup & setup(ossia::net::node_base & parentNode,
                               const std::string& name)
        {
            nodes->_parentNode = &parentNode;
            nodes->_currentNode = nodes->_parentNode;
            this->setName(name);
            
            return *this;
        }

        
        ParameterGroup & setup(ossia::ParameterGroup & parentNode,
                               const std::string& name)
        {
            nodes->_parentNode = &parentNode.getNode();
            createNode(name);
            this->setName(nodes->_currentNode->getName());
            
            parentNode.add(*this);
            
            return *this;
        }
        
        ossia::net::node_base& getNode(){
            return * nodes->_currentNode;
        }
        
    };

    /*
     * Class inheriting from ofParameter
     * Listeners (listening to i-score and GUI) are enabled
     * By passing a std::type in argument, the OSSIA type is deduced in the class
     * */

    template <class DataValue>
    class Parameter : public ofParameter<DataValue>
    {
    private:
        std::shared_ptr <ParamNode> nodes;
        
        using ossia_type = MatchingType<DataValue>;
//        ossia::net::node_base* _parentNode{};
//        ossia::net::node_base* _currentNode{};
//        mutable ossia::net::address_base*  _address{};

        /*
         * Methods to communicate via OSSIA to i-score
         * */
        // Creates the node without setting domain
        void createNode(const std::string& name, DataValue data)
        {
            //creates node
            nodes->_currentNode = nodes->_parentNode->createChild(name);

            //set value
            nodes->_address = nodes->_currentNode->createAddress(ossia_type::val);
            nodes->_address->pushValue(ossia_type::convert(data));
         }
        
        // Creates the node setting domain
        void createNode(const std::string& name, DataValue data, DataValue min, DataValue max)
        {
            //creates node
            auto node = nodes->_parentNode->createChild(name);

            //set value
            nodes->_address = node->createAddress(ossia_type::val);
            nodes->_address->pushValue(ossia_type::convert(data));
            nodes->_address->setDomain(ossia::make_domain(ossia_type::convert(min),
                                                        ossia_type::convert(max)));
            nodes->_address->setUnit(typename ossia_type::ossia_unit{});
        }
        // Publishes value to the node
        void publishValue(DataValue other)
        {
            nodes->_address->pushValue(ossia_type::convert(other));
        }

        // Pulls the node value
        DataValue pullNodeValue()
        {
            using value_type = const typename ossia_type::ossia_type;

            try
            {
                auto val = this->getAddress()->fetchValue();
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
                auto val = this->getAddress()->cloneValue();
                std::cerr <<  "error [ofxOssia::pullNodeValue()] : "<< ossia::value_to_pretty_string(val)  << " " << (int) ossia_type::val << "\n" ;
                return {};
            }
        }

        // Add i-score callback
        void enableRemoteUpdate()
        {
            this->getAddress()->add_callback([&](const ossia::value& val)
            {
                using value_type = const typename ossia_type::ossia_type;
                if(val.target<value_type>())
                {
                    DataValue data = ossia_type::convertFromOssia(val);
                    if(data != this->get())
                    {
                        this->set(data);
                    }
                }
                else
                {
                    std::cerr <<   "error [ofxOssia::enableRemoteUpdate()] : "<< (int) val.getType()  << " " << (int) ossia_type::val << "\n" ;
                    return;
                }
            });
        }

        // Listener for the GUI (but called also when i-score sends value)
        void listen(DataValue &data)
        {
            // check if the value to be published is not already published
            if(pullNodeValue() != data)
            {// i-score->GUI OK
                publishValue(data);
            }
        }

        // listen to of update (GUI)
        void enableLocalUpdate()
        {
            this->addListener(this, &Parameter<DataValue>::listen);
        }

    public:
        Parameter() {
            nodes = std::make_shared<ParamNode> ();
        }


        ~Parameter()
        {
            this->removeListener(this, &Parameter<DataValue>::listen);
        }

        // creates node and sets the name, the data
        Parameter & setup(
                ossia::ParameterGroup & parentNode,
                const std::string& name,
                DataValue data)
        {
            nodes->_parentNode = &parentNode.getNode();
            createNode(name,data);
//            this->set(_currentNode->getName(),data);
            this->set(name,data);
            enableLocalUpdate();
            enableRemoteUpdate();
            parentNode.add(*this);
            return *this;
        }

        // creates node and sets the name, the data, the minimum and maximum value (for the gui)
        Parameter & setup(
                ossia::ParameterGroup & parentNode,
                const std::string& name,
                DataValue data, DataValue min, DataValue max)
        {
            nodes->_parentNode = &parentNode.getNode();
            createNode(name,data,min,max);
//            this->set(_currentNode->getName(),data,min,max);
            this->set(name,data,min,max);
            enableLocalUpdate();
            enableRemoteUpdate();
            parentNode.add(*this);
            return *this;
        }

        // set without creating node (suppose that a node was created previously)
        Parameter & setupNoPublish(
                ossia::ParameterGroup & parentNode,
                const std::string& name,
                DataValue data, DataValue min, DataValue max)
        {
            nodes->_parentNode = &parentNode.getNode();
            this->set(name,data,min,max);
        }

        // Get the address of the node
        ossia::net::address_base* getAddress() const
        {
            if(nodes->_address != nullptr)
            {
                return nodes->_address;
            }

            for(const auto & child : nodes->_parentNode->children())
            {
                if (child->getName().compare(this->getName()) == 0)
                {
                    nodes->_address = child->getAddress();
                    return nodes->_address;
                }
            }
            return nullptr;
        }

        // Updates value of the parameter and publish to the node
        void update(DataValue data)
        {
            publishValue(data);

            // change attribute value
            this->set(data);
        }
    };
}
