/*
  oscpack -- Open Sound Control (OSC) packet manipulation library
    http://www.rossbencina.com/code/oscpack

    Copyright (c) 2004-2013 Ross Bencina <rossb@audiomulch.com>

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
  ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
  The text above constitutes the entire oscpack license; however,
  the oscpack developer(s) also make the following non-binding requests:

  Any person wishing to distribute modifications to the Software is
  requested to send the modifications to the original developer so that
  they can be incorporated into the canonical version. It is also
  requested that these non-binding requests be included whenever the
  above license is reproduced.
*/
#ifndef INCLUDED_OSCPACK_OSCTYPES_H
#define INCLUDED_OSCPACK_OSCTYPES_H
#include <cstdint>


namespace oscpack{

enum ValueTypeSizes{
    OSC_SIZEOF_INT32 = 4,
    OSC_SIZEOF_UINT32 = 4,
    OSC_SIZEOF_INT64 = 8,
    OSC_SIZEOF_UINT64 = 8
};


// osc_bundle_element_size_t is used for the size of bundle elements and blobs
// the OSC spec specifies these as int32_t (signed) but we ensure that they
// are always positive since negative field sizes make no sense.

typedef int32_t osc_bundle_element_size_t;

enum {
    OSC_INT32_MAX = 0x7FFFFFFF,

    // Element sizes are specified to be int32_t, and are always rounded up to nearest
    // multiple of 4. Therefore their values can't be greater than 0x7FFFFFFC.
    OSC_BUNDLE_ELEMENT_SIZE_MAX = 0x7FFFFFFC
};


constexpr inline bool IsValidElementSizeValue( osc_bundle_element_size_t x )
{
    // sizes may not be negative or exceed OSC_BUNDLE_ELEMENT_SIZE_MAX
    return x >= 0 && x <= OSC_BUNDLE_ELEMENT_SIZE_MAX;
}


constexpr inline bool IsMultipleOf4( osc_bundle_element_size_t x )
{
    return (x & ((osc_bundle_element_size_t)0x03)) == 0;
}


enum TypeTagValues {
    TRUE_TYPE_TAG = 'T',
    FALSE_TYPE_TAG = 'F',
    NIL_TYPE_TAG = 'N',
    INFINITUM_TYPE_TAG = 'I',
    INT32_TYPE_TAG = 'i',
    FLOAT_TYPE_TAG = 'f',
    CHAR_TYPE_TAG = 'c',
    RGBA_COLOR_TYPE_TAG = 'r',
    MIDI_MESSAGE_TYPE_TAG = 'm',
    INT64_TYPE_TAG = 'h',
    TIME_TAG_TYPE_TAG = 't',
    DOUBLE_TYPE_TAG = 'd',
    STRING_TYPE_TAG = 's',
    SYMBOL_TYPE_TAG = 'S',
    BLOB_TYPE_TAG = 'b',
    ARRAY_BEGIN_TYPE_TAG = '[',
    ARRAY_END_TYPE_TAG = ']'
};



// i/o manipulators used for streaming interfaces

struct BundleInitiator{
    constexpr explicit BundleInitiator( uint64_t timeTag_ ) : timeTag( timeTag_ ) {}
    uint64_t timeTag{};
};

constexpr BundleInitiator BeginBundle( uint64_t timeTag=1 )
{ return BundleInitiator{timeTag}; }

constexpr BundleInitiator BeginBundleImmediate()
{ return BundleInitiator{1}; }


struct BundleTerminator{ };
constexpr BundleTerminator EndBundle() { return {}; }

struct BeginMessage{
    constexpr explicit BeginMessage( const char *addressPattern_ ) : addressPattern( addressPattern_ ) {}
    const char *addressPattern{};
};

struct MessageTerminator{ };
constexpr MessageTerminator EndMessage()
{ return {}; }

// osc specific types. they are defined as structs so they can be used
// as separately identifiable types with the streaming operators.

struct NilType{ };
constexpr NilType OscNil() { return {}; }


struct InfinitumType{ };
constexpr InfinitumType Infinitum() { return {}; }

struct RgbaColor{
    constexpr RgbaColor() {}
    constexpr explicit RgbaColor( uint32_t value_ ) : value( value_ ) {}
    uint32_t value{};

    constexpr operator uint32_t() const { return value; }
};


struct MidiMessage{
    constexpr MidiMessage() {}
    constexpr explicit MidiMessage( uint32_t value_ ) : value( value_ ) {}
    uint32_t value{};

    constexpr operator uint32_t() const { return value; }
};


struct TimeTag{
    constexpr TimeTag() {}
    constexpr explicit TimeTag( uint64_t value_ ) : value( value_ ) {}
    uint64_t value{};

    constexpr operator uint64_t() const { return value; }
};


struct Symbol{
    constexpr Symbol() {}
    constexpr explicit Symbol( const char* value_ ) : value( value_ ) {}
    const char* value{};

    constexpr operator const char *() const { return value; }
};


struct Blob{
    constexpr Blob() {}
    constexpr explicit Blob( const void* data_, osc_bundle_element_size_t size_ )
            : data( data_ ), size( size_ ) {}
    const void* data{};
    osc_bundle_element_size_t size{};
};

struct ArrayInitiator{ };
constexpr ArrayInitiator BeginArray() { return {}; }

struct ArrayTerminator{ };
constexpr ArrayTerminator EndArray() { return {}; }

} // namespace osc


#endif /* INCLUDED_OSCPACK_OSCTYPES_H */
