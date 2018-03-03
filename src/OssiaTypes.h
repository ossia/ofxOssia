#pragma once
#include <ossia-cpp98.hpp>
#include <types/ofBaseTypes.h>
#include <math/ofVectorMath.h>
#include <string>
#include <array>

#undef Status
#undef Bool
#undef bool
#undef False
#undef status
#undef None

// namespace ossia // Probably not necessary any more
// {
/**
 * These classes contain the conversion mechanism from and to
 * the compatible OSSIA & OpenFrameworks types.
 *
 * GenericMatchingType is used for types where it is straightforward
 * (int, bool, string, etc...).
 */
template<typename> struct MatchingType;

template<> struct MatchingType<float> {
  using ofx_type = float;
  using ossia_type = float;

  opp::node create_parameter(const std::string& _name,
                             opp::node _parent)
       {return _parent.create_float(_name);}

  static ofx_type convertFromOssia(const opp::value& v)
  {
    return v.to_float();
  }

  static ossia_type convert(ofx_type f)
  {
    return float(f);
  }
};


template<> struct MatchingType<int> {
  using ofx_type = int;
  using ossia_type = int;

  opp::node create_parameter(const std::string& _name,
                             opp::node _parent)
      {return _parent.create_int(_name);}

  static ofx_type convertFromOssia(const opp::value& v)
  {
    return v.to_int();
  }

  static ossia_type convert(ofx_type f)
  {
    return int(f);
  }
};


template<> struct MatchingType<bool> {
  using ofx_type = bool;
  using ossia_type = bool;

  opp::node create_parameter(const std::string& _name,
                             opp::node _parent)
       {return _parent.create_bool(_name);}

  static ofx_type convertFromOssia(const opp::value& v)
  {
    return v.to_bool();
  }

  static ossia_type convert(ofx_type f)
  {
    return bool(f);
  }
};


/* 
// Do we really need char support ? 
// It's not in the safeC++ implementation yet anyway
template<> struct MatchingType<char> final :
        public GenericMatchingType<char, char, ossia::val_type::CHAR>
{
};
*/


template<> struct MatchingType<std::string> {
  using ofx_type = string;
  using ossia_type = string;

  opp::node create_parameter(const std::string& _name,
                             opp::node _parent)
       {return _parent.create_string(_name);}

  static ofx_type convertFromOssia(const opp::value& v)
  {
    return v.to_string();
  }

  static ossia_type convert(ofx_type f)
  {
    return std::string(f);
  }
};

template<> struct MatchingType<double> {
  using ofx_type = double;
  using ossia_type = float;

  opp::node create_parameter(const std::string& _name, opp::node _parent)
      {return _parent.create_float(_name);}

  static ofx_type convertFromOssia(const opp::value& v)
  {
    return double(v.to_float());
  }

  static ossia_type convert(ofx_type f)
  {
    return float(f);
  }
};

template<> struct MatchingType<ofVec2f> {
  using ofx_type = ofVec2f;
  using ossia_type = std::array<float, 2>;

  opp::node create_parameter(const std::string& _name, opp::node _parent)
        {return _parent.create_vec2f(_name);}

  static ofx_type convertFromOssia(const opp::value& v)
  {
    //const auto& t = v;
    return ofx_type(v[0], v[1]);
  }

  static ossia_type convert(ofx_type f)
  {
    // This could probably be done otherwise, see editor/value/vec.hpp (from where this comes), 
    // but for now (and for all vec types below), it will be done this way 
    return std::array<float, 2>{f.x, f.y};
  }
};

template<> struct MatchingType<ofVec3f> {
  using ofx_type = ofVec3f;
  using ossia_type = std::array<float, 3>;

  opp::node create_parameter(const std::string& _name, opp::node _parent)
          {return _parent.create_vec3f(_name);}

  static ofx_type convertFromOssia(const opp::value& v)
  {
    //const auto& t = v.get_value();
    return ofx_type(v[0], v[1], v[2]);
  }

  static ossia_type convert(ofx_type f)
  {
      return std::array<float, 3>{{f.x, f.y, f.z}};
  }
};

template<> struct MatchingType<ofVec4f> {
  using ofx_type = ofVec4f;
  using ossia_type = std::array<float, 4>;

  opp::node create_parameter(const std::string& _name, opp::node _parent)
            {return _parent.create_vec4f(_name);}


  static ofx_type convertFromOssia(const opp::value& v)
  {
    //const auto& t = v.get_value();
    return ofx_type(v[0], v[1], v[2], v[3]);
  }

  static ossia_type convert(ofx_type f)
  {
    return std::array<float, 4>{f.x, f.y, f.z, f.w};
  }
};

template<> struct MatchingType<ofColor> {
  using ofx_type = ofColor;
  using ossia_type = std::array<float, 4>;

  opp::node create_parameter(const std::string& _name, opp::node _parent)
              {return _parent.create_rgba(_name);}

  static ofx_type convertFromOssia(const opp::value& v)
  {
    //const auto& t = v.get_value();
    return ofx_type(v[0] * 255., v[1] * 255., v[2] * 255., v[3] * 255.);
  }

  static ossia_type convert(ofx_type f)
  {
    return std::array<float, 4>(f.r / 255., f.g / 255., f.b / 255., f.a / 255.);
  }
};

template<> struct MatchingType<ofFloatColor> {
  using ofx_type = ofFloatColor;
  using ossia_type = std::array<float, 4>;

    opp::node create_parameter(const std::string& _name, opp::node _parent)
                {return _parent.create_argb8(_name);}

// For those conversions, as there is no rgba8 type in ossia, we swap the 1st and 4th values
  static ofx_type convertFromOssia(const opp::value& v)
  {
    //const auto& t = v.get_value();
    return ofx_type(v[1], v[2], v[3], v[0]);
  }

  static ossia_type convert(ofx_type f)
  {
    return std::array<float, 4>(f.a, f.r, f.g, f.b);
  }
};
//} // namespace ossia
