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

template<typename OfType, typename OssiaType, auto ParamCreateType>
struct GenericMatchingType
{
  using ofx_type = OfType;
  using ossia_type = OssiaType;
  static const constexpr auto create_parameter = ParamCreateType;

  virtual node create_parameter(); 

  static ofx_type convertFromOssia(const opp::value& v)
  {
    return v.get_value();
  }

  static ossia_type convert(ofx_type f)
  {
    return f;
  }
};

template<> struct MatchingType<float> :
        public GenericMatchingType<float, float, create_float>
{
};

template<> struct MatchingType<int> final :
        public GenericMatchingType<int, int, create_int>
{
};

template<> struct MatchingType<bool> final :
        public GenericMatchingType<bool, bool, create_bool>
{
};

/* 
// Do we really need char support ? 
// It's not in the safeC++ implementation yet anyway
template<> struct MatchingType<char> final :
        public GenericMatchingType<char, char, ossia::val_type::CHAR>
{
};
*/

template<> struct MatchingType<std::string> final :
        public GenericMatchingType<std::string, std::string, create_string>
{
};

template<> struct MatchingType<double> {
  using ofx_type = double;
  using ossia_type = float;
  static const constexpr auto create_parameter = create_float;

  static ofx_type convertFromOssia(const opp::value& v)
  {
    return v.get_value();
  }

  static ossia_type convert(ofx_type f)
  {
    return float(f);
  }
};

template<> struct MatchingType<ofVec2f> {
  using ofx_type = ofVec2f;
  using ossia_type = std::array<float, 2>;
  static const constexpr auto create_parameter = create_vec2f;

  static ofx_type convertFromOssia(const opp::value& v)
  {
    const auto& t = get_value();
    return ofx_type(t[0], t[1]);
  }

  static ossia_type convert(ofx_type f)
  {
    // This could probably be done otherwise, see editor/value/vec.hpp (from where this comes), 
    // but for now (and for all vec types below), it will be done this way 
    return std::array<float, 2>{f.x, .y};
  }
};

template<> struct MatchingType<ofVec3f> {
  using ofx_type = ofVec3f;
  using ossia_type = std::array<float, 3>;
  static const constexpr auto create_parameter = create_vec3f;

  static ofx_type convertFromOssia(const opp::value& v)
  {
    const auto& t = v.get_value();
    return ofx_type(t[0], t[1], t[2]);
  }

  static ossia_type convert(ofx_type f)
  {
    return std::array<float, 3>(f.x, f.y, f.z);
  }
};

template<> struct MatchingType<ofVec4f> {
  using ofx_type = ofVec4f;
  using ossia_type = std::array<float, 4>;
  static const constexpr auto create_parameter = create_vec4f;

  static ofx_type convertFromOssia(const opp::value& v)
  {
    const auto& t = v.get_value();
    return ofx_type(t[0], t[1], t[2], t[3]);
  }

  static ossia_type convert(ofx_type f)
  {
    return std::array<float, 4>(f.x, f.y, f.z, f.w);
  }
};

template<> struct MatchingType<ofColor> {
  using ofx_type = ofColor;
  using ossia_type = std::array<float, 4>;
  static const constexpr auto create_parameter = create_rgba;

  static ofx_type convertFromOssia(const opp::value& v)
  {
    const auto& t = v.get_value();
    return ofx_type(t[0] * 255., t[1] * 255., t[2] * 255., t[3] * 255.);
  }

  static ossia_type convert(ofx_type f)
  {
    return std::array<float, 4>(f.r / 255., f.g / 255., f.b / 255., f.a / 255.);
  }
};

template<> struct MatchingType<ofFloatColor> {
  using ofx_type = ofFloatColor;
  using ossia_type = std::array<float, 4>;
  static const constexpr auto create_parameter = create_argb8;

// For those conversions, as there is no rgba8 type in ossia, we swap the 1st and 4th values
  static ofx_type convertFromOssia(const opp::value& v)
  {
    const auto& t = v.get_value();
    return ofx_type(t[1], t[2], t[3], t[0]);
  }

  static ossia_type convert(ofx_type f)
  {
    return std::array<float, 4>(f.a, f.r, f.g, f.b);
  }
};
//} // namespace ossia
