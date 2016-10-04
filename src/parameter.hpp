#ifndef PARAMETER_H
#define PARAMETER_H

#include "ofMain.h"
#include "network.hpp"
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

    static ofx_type convertFromOssia(const ossia::value& v)
    {
        return v.get<ossia_type>().value;
    }

    static ossia_type convert(ofx_type f)
    {
        return f;
    }
};

template<> struct MatchingType<float> final :
        public GenericMatchingType<float, ossia::Float, ossia::val_type::FLOAT>
{
};

template<> struct MatchingType<int> final :
        public GenericMatchingType<int, ossia::Int, ossia::val_type::INT>
{
};

template<> struct MatchingType<bool> final :
        public GenericMatchingType<bool, ossia::Bool, ossia::val_type::BOOL>
{
};

template<> struct MatchingType<char> final :
        public GenericMatchingType<char, ossia::Char, ossia::val_type::CHAR>
{
};

template<> struct MatchingType<std::string> final :
        public GenericMatchingType<std::string, ossia::String, ossia::val_type::STRING>
{
};

template<> struct MatchingType<double> {
    using ofx_type = double;
    static constexpr const auto val = ossia::val_type::FLOAT;
    using ossia_type = ossia::Float;

    static ofx_type convertFromOssia(const ossia::value& v)
    {
        return v.get<ossia_type>().value;
    }

    static ossia_type convert(ofx_type f)
    {
        return float(f);
    }
};

template<> struct MatchingType<ofVec3f> {
    using ofx_type = ofVec3f;
    static constexpr const auto val = ossia::val_type::TUPLE;
    using ossia_type = ossia::Tuple;

    static ofx_type convertFromOssia(const ossia::value& v)
    {
        // TODO vec2, 3, 4
        const auto& t = v.get<ossia::Tuple>();

        float x{}, y{}, z{};

        if(t.value.size() == 3)
        {
            if(auto ossia_x = t.value[0].try_get<Float>())
                x = ossia_x->value;
            if(auto ossia_y = t.value[1].try_get<Float>())
                y = ossia_y->value;
            if(auto ossia_z = t.value[2].try_get<Float>())
                z = ossia_z->value;
        }
        return ofx_type(x,y,z);
    }

    static ossia_type convert(ofx_type f)
    {
        return ossia::Tuple{ossia::Float(f.x), ossia::Float(f.y), ossia::Float(f.z)};
    }
};

template<> struct MatchingType<ofVec2f>
{
    using ofx_type = ofVec2f;
    static constexpr const auto val = ossia::val_type::TUPLE;
    using ossia_type = ossia::Tuple;

    static ofx_type convertFromOssia(const ossia::value& v) {

        const auto& t = v.get<ossia::Tuple>();
        float x{}, y{};

        if(t.value.size() == 2)
        {
            if(auto ossia_x = t.value[0].try_get<Float>())
                x = ossia_x->value;
            if(auto ossia_y = t.value[1].try_get<Float>())
                y = ossia_y->value;
        }
        return ofx_type(x,y);
    }

    static ossia_type convert(ofx_type f) {
        return ossia::Tuple{ossia::Float(f.x), ossia::Float(f.y)};
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
    ossia::net::node_base* _parentNode{};
    mutable ossia::net::address_base*  _address{};

    /*
     * Methods to communicate via OSSIA to i-score
     * */
    // Creates the node without setting domain
    void createNode(const std::string& name, DataValue data)
    {
        //creates node
        auto node = _parentNode->createChild(name);

        //set value
        _address = node->createAddress(MatchingType<DataValue>::val);
        _address->pushValue(MatchingType<DataValue>::convert(data));
     }
    // Creates the node setting domain
    void createNode(const std::string& name, DataValue data, DataValue min, DataValue max)
    {
        //creates node
        auto node = _parentNode->createChild(name);

        //set value
        _address = node->createAddress(MatchingType<DataValue>::val);
        _address->pushValue(MatchingType<DataValue>::convert(data));
        _address->setDomain(ossia::net::make_domain(MatchingType<DataValue>::convert(min),
                                                    MatchingType<DataValue>::convert(max)));
    }
    // Publishes value to the node
    void publishValue(DataValue other)
    {
        _address->pushValue(MatchingType<DataValue>::convert(other));
    }

    // Pulls the node value
    DataValue pullNodeValue()
    {
        using value_type = const typename MatchingType<DataValue>::ossia_type;

        try
        {
            auto val = this->getAddress()->fetchValue();
            if(val.template try_get<value_type>())
                return MatchingType<DataValue>::convertFromOssia(val);
            else
                std::cerr <<  "error [ofxOssia::pullNodeValue()] : "<<(int) val.getType()  << " " << (int) MatchingType<DataValue>::val << "\n" ;
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
            std::cerr <<  "error [ofxOssia::pullNodeValue()] : "<<ossia::to_pretty_string(val)  << " " << (int) MatchingType<DataValue>::val << "\n" ;
            return {};
        }
    }

    // Add i-score callback
    void enableRemoteUpdate()
    {
        this->getAddress()->add_callback([&](const ossia::value& val)
        {
            using value_type = const typename MatchingType<DataValue>::ossia_type;
            if(val.try_get<value_type>())
            {
                DataValue data = MatchingType<DataValue>::convertFromOssia(val);
                if(data != this->get())
                {
                    this->set(data);
                }
            }
            else
            {
                std::cerr <<   "error [ofxOssia::enableRemoteUpdate()] : "<< (int) val.getType()  << " " << (int) MatchingType<DataValue>::val << "\n" ;
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
    Parameter() = default;

    ~Parameter()
    {
        this->removeListener(this, &Parameter<DataValue>::listen);
    }

    // creates node and sets the name, the data
    Parameter & setup(
            ossia::net::node_base& parentNode,
            const std::string& name,
            DataValue data)
    {
        _parentNode = &parentNode;
        createNode(name,data);
        this->set(name,data);
        enableLocalUpdate();
        enableRemoteUpdate();
        return *this;
    }

    // creates node and sets the name, the data, the minimum and maximum value (for the gui)
    Parameter & setup(
            ossia::net::node_base& parentNode,
            const std::string& name,
            DataValue data, DataValue min, DataValue max)
    {
        _parentNode = &parentNode;
        createNode(name,data,min,max);
        this->set(name,data,min,max);
        enableLocalUpdate();
        enableRemoteUpdate();
        return *this;
    }

    // set without creating node (suppose that a node was created previously)
    Parameter & setupNoPublish(
            ossia::net::node_base& parentNode,
            const std::string& name,
            DataValue data, DataValue min, DataValue max)
    {
        _parentNode = &parentNode;
        this->set(name,data,min,max);
    }

    // Get the address of the node
    ossia::net::address_base* getAddress() const
    {
        if(_address != nullptr)
        {
            return _address;
        }

        for(const auto & child : _parentNode->children())
        {
            if (child->getName().compare(this->getName()) == 0)
            {
                _address = child->getAddress();
                return _address;
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
#endif // PARAMETER_H
