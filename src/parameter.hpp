#ifndef PARAMETER_H
#define PARAMETER_H

#include "ofMain.h"
#include "network.hpp"
namespace OSSIA
{

/**
 * These classes contain the conversion mechanism from and to
 * the compatible OSSIA & OpenFrameworks types.
 *
 * GenericMatchingType is used for types where it is straightforward
 * (int, bool, string, etc...).
 */
template<typename> struct MatchingType;

template<typename OfType, typename OssiaType, OSSIA::Value::Type OssiaTypeEnum>
struct GenericMatchingType
{
    using ofx_type = OfType;
    using ossia_type = OssiaType;
    static constexpr const auto val = OssiaTypeEnum;

    static ofx_type convertFromOssia(const Value& v)
    {
        return static_cast<const ossia_type&>(v).value;
    }

    static ossia_type convert(ofx_type f)
    {
        return f;
    }
};

template<> struct MatchingType<float> final :
        public GenericMatchingType<float, OSSIA::Float, OSSIA::Value::Type::FLOAT>
{
};

template<> struct MatchingType<int> final :
        public GenericMatchingType<int, OSSIA::Int, OSSIA::Value::Type::INT>
{
};

template<> struct MatchingType<bool> final :
        public GenericMatchingType<bool, OSSIA::Bool, OSSIA::Value::Type::BOOL>
{
};

template<> struct MatchingType<char> final :
        public GenericMatchingType<char, OSSIA::Char, OSSIA::Value::Type::CHAR>
{
};

template<> struct MatchingType<std::string> final :
        public GenericMatchingType<std::string, OSSIA::String, OSSIA::Value::Type::STRING>
{
};

template<> struct MatchingType<double> {
    using ofx_type = double;
    static constexpr const auto val = OSSIA::Value::Type::FLOAT;
    using ossia_type = OSSIA::Float;

    static ofx_type convertFromOssia(const Value& v)
    {
        return static_cast<const ossia_type&>(v).value;
    }

    static ossia_type convert(ofx_type f)
    {
        return float(f);
    }
};

template<> struct MatchingType<ofVec3f> {
    using ofx_type = ofVec3f;
    static constexpr const auto val = OSSIA::Value::Type::TUPLE;
    using ossia_type = OSSIA::Tuple;

    static ofx_type convertFromOssia(const OSSIA::Value& v)
    {
        const auto& t = static_cast<const OSSIA::Tuple&>(v);

        float x{}, y{}, z{};

        if(t.value.size() == 3)
        {
            if(auto ossia_x = dynamic_cast<OSSIA::Float*>(t.value[0]))
                x = ossia_x->value;
            if(auto ossia_y = dynamic_cast<OSSIA::Float*>(t.value[1]))
                y = ossia_y->value;
            if(auto ossia_z = dynamic_cast<OSSIA::Float*>(t.value[2]))
                z = ossia_z->value;
        }
        return ofx_type(x,y,z);
    }

    static ossia_type convert(ofx_type f)
    {
        return OSSIA::Tuple{OSSIA::Tuple::ValueInit{}, OSSIA::Float(f.x), OSSIA::Float(f.y), OSSIA::Float(f.z)};
    }
};

template<> struct MatchingType<ofVec2f>
{
    using ofx_type = ofVec2f;
    static constexpr const auto val = OSSIA::Value::Type::TUPLE;
    using ossia_type = OSSIA::Tuple;

    static ofx_type convertFromOssia(const OSSIA::Value& v) {

        const auto& t = static_cast<const OSSIA::Tuple&>(v);
        float x{}, y{};

        if(t.value.size() == 2)
        {
            if(auto ossia_x = dynamic_cast<OSSIA::Float*>(t.value[0]))
                x = ossia_x->value;
            if(auto ossia_y = dynamic_cast<OSSIA::Float*>(t.value[1]))
                y = ossia_y->value;
        }
        return ofx_type(x,y);
    }

    static ossia_type convert(ofx_type f) {
        return OSSIA::Tuple{OSSIA::Tuple::ValueInit{}, OSSIA::Float(f.x), OSSIA::Float(f.y)};
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
    std::shared_ptr<Node> _parentNode = nullptr;
    std::shared_ptr<Address> _address = nullptr;

    /*
     * Methods to communicate via OSSIA to i-score
     * */
    // Creates the node
    void createNode(const std::string& name, DataValue data)
    {
        //creates node
        std::shared_ptr<Node> node = *(_parentNode->emplace(_parentNode->children().cend(), name));

        //set value
        _address = node->createAddress(MatchingType<DataValue>::val);
        auto val = MatchingType<DataValue>::convert(data);
        _address->pushValue(&val);
    }

    // Publishes value to the node
    void publishValue(DataValue other)
    {
        auto val = MatchingType<DataValue>::convert(other);
        _address->pushValue(&val);
    }

    // Pulls the node value
    DataValue pullNodeValue()
    {
        using value_type = const typename MatchingType<DataValue>::ossia_type*;

        try
        {
            std::unique_ptr<OSSIA::Value> val{this->getAddress()->cloneValue()};
            if(dynamic_cast<value_type>(val.get()))
                return MatchingType<DataValue>::convertFromOssia(*val);
            else
                std::cerr <<  (int) val->getType()  << " " << (int) MatchingType<DataValue>::val << "\n" ;
            return {};
        }
        catch(...)
        {
            auto val = this->getAddress()->getValue();
            if(val)
                std::cerr <<  (int) val->getType()  << " " << (int) MatchingType<DataValue>::val << "\n" ;
            else
                std::cerr << "no val\n";
            return {};
        }
    }

    // Add i-score callback
    void enableRemoteUpdate()
    {
        this->getAddress()->addCallback([&](const OSSIA::Value *v)
        {
            using value_type = const typename MatchingType<DataValue>::ossia_type*;
            auto val = dynamic_cast<value_type>(v);
            if(!val)
            {
                std::cerr <<  (int) val->getType()  << " " << (int) MatchingType<DataValue>::val << "\n" ;
                return;
            }
            DataValue data = MatchingType<DataValue>::convertFromOssia(*val);
            if(data != this->get())
            {
                this->set(data);
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
            const std::shared_ptr<Node>& parentNode,
            const std::string& name,
            DataValue data)
    {
        _parentNode = parentNode;
        createNode(name,data);
        this->set(name,data);
        enableLocalUpdate();
        enableRemoteUpdate();
        return *this;
    }

    // creates node and sets the name, the data, the minimum and maximum value (for the gui)
    Parameter & setup(
            const std::shared_ptr<Node>& parentNode,
            const std::string& name,
            DataValue data, DataValue min, DataValue max)
    {
        _parentNode = parentNode;
        createNode(name,data);
        this->set(name,data,min,max);
        enableLocalUpdate();
        enableRemoteUpdate();
        return *this;
    }

    // set without creating node (suppose that a node was created previously)
    Parameter & setupNoPublish(
            const std::shared_ptr<Node>& parentNode,
            const std::string& name,
            DataValue data, DataValue min, DataValue max)
    {
        _parentNode = parentNode;
        this->set(name,data,min,max);
    }

    // Get the address of the node
    std::shared_ptr<Address> getAddress() const
    {
        if(_address != nullptr)
        {
            return _address;
        }

        for(const auto & child : _parentNode->children())
        {
            if (child->getName().compare(this->getName()) == 0)
            {
                return child->getAddress();
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
