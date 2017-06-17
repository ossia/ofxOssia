#pragma once
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

#include <ossia/editor/value/value.hpp>
#include <ossia/editor/dataspace/dataspace.hpp>

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

template<> struct MatchingType<ofVec4f> {
  using ofx_type = ofVec4f;
  static constexpr const auto val = ossia::val_type::VEC4F;
  using ossia_type = std::array<float, 4>;
  using ossia_unit = ossia::unit_t;

  static ofx_type convertFromOssia(const ossia::value& v)
  {
    const auto& t = v.get<ossia_type>();
    return ofx_type(t[0], t[1], t[2], t[3]);
  }

  static ossia_type convert(ofx_type f)
  {
    return ossia::make_vec(f.x, f.y, f.z, f.w);
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
}
