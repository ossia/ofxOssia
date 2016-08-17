#pragma once
#include "OscReceivedElements.h"

#include <type_traits>
namespace oscpack
{
// Helpers to get the values.
template<oscpack::TypeTagValues>
struct OscpackFunction;

// For the ones that requires access to more than the type.
struct object_required_trait {};

// For the ones where the value is included in the type.
struct object_useless_trait   {};

template<>
struct OscpackFunction<oscpack::INT32_TYPE_TAG>
{
    using conversion_mode = object_required_trait;
    static const constexpr auto convert =           &oscpack::ReceivedMessageArgument::AsInt32;
    static const constexpr auto convert_unchecked = &oscpack::ReceivedMessageArgument::AsInt32Unchecked;
};

template<>
struct OscpackFunction<oscpack::INT64_TYPE_TAG>
{
    using conversion_mode = object_required_trait;
    static const constexpr auto convert =           &oscpack::ReceivedMessageArgument::AsInt64;
    static const constexpr auto convert_unchecked = &oscpack::ReceivedMessageArgument::AsInt64Unchecked;
};

template<>
struct OscpackFunction<oscpack::FLOAT_TYPE_TAG>
{
    using conversion_mode = object_required_trait;
    static const constexpr auto convert =           &oscpack::ReceivedMessageArgument::AsFloat;
    static const constexpr auto convert_unchecked = &oscpack::ReceivedMessageArgument::AsFloatUnchecked;
};

template<>
struct OscpackFunction<oscpack::DOUBLE_TYPE_TAG>
{
    using conversion_mode = object_required_trait;
    static const constexpr auto convert =           &oscpack::ReceivedMessageArgument::AsDouble;
    static const constexpr auto convert_unchecked = &oscpack::ReceivedMessageArgument::AsDoubleUnchecked;
};

template<>
struct OscpackFunction<oscpack::CHAR_TYPE_TAG>
{
    using conversion_mode = object_required_trait;
    static const constexpr auto convert =           &oscpack::ReceivedMessageArgument::AsChar;
    static const constexpr auto convert_unchecked = &oscpack::ReceivedMessageArgument::AsCharUnchecked;
};

template<>
struct OscpackFunction<oscpack::STRING_TYPE_TAG>
{
    using conversion_mode = object_required_trait;
    static const constexpr auto convert =           &oscpack::ReceivedMessageArgument::AsString;
    static const constexpr auto convert_unchecked = &oscpack::ReceivedMessageArgument::AsStringUnchecked;
};

template<>
struct OscpackFunction<oscpack::SYMBOL_TYPE_TAG>
{
    using conversion_mode = object_required_trait;
    static const constexpr auto convert =           &oscpack::ReceivedMessageArgument::AsSymbol;
    static const constexpr auto convert_unchecked = &oscpack::ReceivedMessageArgument::AsSymbolUnchecked;
};

template<>
struct OscpackFunction<oscpack::BLOB_TYPE_TAG>
{
    using conversion_mode = object_required_trait;
    static const constexpr auto convert =           &oscpack::ReceivedMessageArgument::AsBlob;
    static const constexpr auto convert_unchecked = &oscpack::ReceivedMessageArgument::AsBlobUnchecked;
};

template<>
struct OscpackFunction<oscpack::TRUE_TYPE_TAG>
{
    using conversion_mode = object_useless_trait;
    static bool true_fun() { return true; }
    static const constexpr auto convert =           &true_fun;
    static const constexpr auto convert_unchecked = &true_fun;
};

template<>
struct OscpackFunction<oscpack::FALSE_TYPE_TAG>
{
    using conversion_mode = object_useless_trait;
    static bool false_fun() { return false; }
    static const constexpr auto convert =           &false_fun;
    static const constexpr auto convert_unchecked = &false_fun;
};

template<>
struct OscpackFunction<oscpack::INFINITUM_TYPE_TAG>
{
    using conversion_mode = object_useless_trait;
    static InfinitumType impulse_fun() { return {}; }
    static const constexpr auto convert =           &impulse_fun;
    static const constexpr auto convert_unchecked = &impulse_fun;
};

template<>
struct OscpackFunction<oscpack::NIL_TYPE_TAG>
{
    using conversion_mode = object_useless_trait;
    static NilType nil_fun() { return {}; }
    static const constexpr auto convert =           &nil_fun;
    static const constexpr auto convert_unchecked = &nil_fun;
};

template<oscpack::TypeTagValues val>
auto convert(oscpack::ReceivedMessageArgument arg,
             std::enable_if_t<
              std::is_same<
               typename OscpackFunction<val>::conversion_mode,
               object_required_trait
              >::value
             >* = nullptr)
{
  return (arg.*OscpackFunction<val>::convert)();
}

template<oscpack::TypeTagValues val>
auto convert(oscpack::ReceivedMessageArgument,
             std::enable_if_t<
              std::is_same<
               typename OscpackFunction<val>::conversion_mode,
               object_useless_trait
              >::value
             >* = nullptr)
{
  return (*OscpackFunction<val>::convert)();
}

}
