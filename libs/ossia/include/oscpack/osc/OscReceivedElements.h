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
#ifndef INCLUDED_OSCPACK_OSCRECEIVEDELEMENTS_H
#define INCLUDED_OSCPACK_OSCRECEIVEDELEMENTS_H

#include <cassert>
#include <cstddef>
#include <cstring> // size_t

#include "OscTypes.h"
#include "OscException.h"
#include "OscUtilities.h"
#include <cstddef> // ptrdiff_t


namespace oscpack{

class MalformedPacketException : public Exception{
  public:
    MalformedPacketException( const char *w="malformed packet" )
      : Exception( w ) {}
};

class MalformedMessageException : public Exception{
  public:
    MalformedMessageException( const char *w="malformed message" )
      : Exception( w ) {}
};

class MalformedBundleException : public Exception{
  public:
    MalformedBundleException( const char *w="malformed bundle" )
      : Exception( w ) {}
};

class WrongArgumentTypeException : public Exception{
  public:
    WrongArgumentTypeException( const char *w="wrong argument type" )
      : Exception( w ) {}
};

class MissingArgumentException : public Exception{
  public:
    MissingArgumentException( const char *w="missing argument" )
      : Exception( w ) {}
};

class ExcessArgumentException : public Exception{
  public:
    ExcessArgumentException( const char *w="too many arguments" )
      : Exception( w ) {}
};


class ReceivedPacket{
  public:
    // Although the OSC spec is not entirely clear on this, we only support
    // packets up to 0x7FFFFFFC bytes long (the maximum 4-byte aligned value
    // representable by an int32_t). An exception will be raised if you pass a
    // larger value to the ReceivedPacket() constructor.

    ReceivedPacket( const char *contents, osc_bundle_element_size_t size )
      : contents_( contents )
      , size_( ValidateSize(size) ) {}

    ReceivedPacket( const char *contents, std::size_t size )
      : contents_( contents )
      , size_( ValidateSize( (osc_bundle_element_size_t)size ) ) {}

    ReceivedPacket(const char *contents, int64_t size)
        : contents_(contents)
        , size_(ValidateSize((osc_bundle_element_size_t)size)) {}

    bool IsMessage() const { return !IsBundle(); }
    bool IsBundle() const
    {
      return (Size() > 0 && Contents()[0] == '#');
    }

    osc_bundle_element_size_t Size() const { return size_; }
    const char *Contents() const { return contents_; }

  private:
    const char *contents_;
    osc_bundle_element_size_t size_;

    static osc_bundle_element_size_t ValidateSize( osc_bundle_element_size_t size )
    {
      // sanity check integer types declared in OscTypes.h
      // you'll need to fix OscTypes.h if any of these asserts fail
      if( !IsValidElementSizeValue(size) )
        throw MalformedPacketException( "invalid packet size" );

      if( size == 0 )
        throw MalformedPacketException( "zero length elements not permitted" );

      if( !IsMultipleOf4(size) )
        throw MalformedPacketException( "element size must be multiple of four" );

      return size;
    }
};


class ReceivedBundleElement{
  public:
    ReceivedBundleElement( const char *sizePtr )
      : sizePtr_( sizePtr ) {}

    friend class ReceivedBundleElementIterator;

    bool IsMessage() const { return !IsBundle(); }
    bool IsBundle() const
    {
      return (Size() > 0 && Contents()[0] == '#');
    }

    osc_bundle_element_size_t Size() const
    {
      return ToInt32( sizePtr_ );
    }
    const char *Contents() const { return sizePtr_ + oscpack::OSC_SIZEOF_INT32; }

  private:
    const char *sizePtr_;
};


class ReceivedBundleElementIterator{
  public:
    ReceivedBundleElementIterator( const char *sizePtr )
      : value_( sizePtr ) {}

    ReceivedBundleElementIterator operator++()
    {
      Advance();
      return *this;
    }

    ReceivedBundleElementIterator operator++(int)
    {
      ReceivedBundleElementIterator old( *this );
      Advance();
      return old;
    }

    const ReceivedBundleElement& operator*() const { return value_; }

    const ReceivedBundleElement* operator->() const { return &value_; }

    friend bool operator==(const ReceivedBundleElementIterator& lhs,
                           const ReceivedBundleElementIterator& rhs );

  private:
    ReceivedBundleElement value_;

    void Advance() { value_.sizePtr_ = value_.Contents() + value_.Size(); }

    bool IsEqualTo( const ReceivedBundleElementIterator& rhs ) const
    {
      return value_.sizePtr_ == rhs.value_.sizePtr_;
    }
};

inline bool operator==(const ReceivedBundleElementIterator& lhs,
                       const ReceivedBundleElementIterator& rhs )
{
  return lhs.IsEqualTo( rhs );
}

inline bool operator!=(const ReceivedBundleElementIterator& lhs,
                       const ReceivedBundleElementIterator& rhs )
{
  return !( lhs == rhs );
}


class ReceivedMessageArgument{
  public:
    ReceivedMessageArgument( ) = default;
    ReceivedMessageArgument( const char *typeTagPtr, const char *argumentPtr )
      : typeTagPtr_( typeTagPtr )
      , argumentPtr_( argumentPtr ) {}

    friend class ReceivedMessageArgumentIterator;

    char TypeTag() const { return *typeTagPtr_; }

    // the unchecked methods below don't check whether the argument actually
    // is of the specified type. they should only be used if you've already
    // checked the type tag or the associated IsType() method.

    bool IsBool() const
    { return *typeTagPtr_ == TRUE_TYPE_TAG || *typeTagPtr_ == FALSE_TYPE_TAG; }
    bool AsBool() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == TRUE_TYPE_TAG )
        return true;
      else if( *typeTagPtr_ == FALSE_TYPE_TAG )
        return false;
      else
        throw WrongArgumentTypeException();
    }
    bool AsBoolUnchecked() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == TRUE_TYPE_TAG )
        return true;
      else
        return false;
    }

    bool IsNil() const { return *typeTagPtr_ == NIL_TYPE_TAG; }
    bool IsInfinitum() const { return *typeTagPtr_ == INFINITUM_TYPE_TAG; }

    bool IsInt32() const { return *typeTagPtr_ == INT32_TYPE_TAG; }
    int32_t AsInt32() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == INT32_TYPE_TAG )
        return AsInt32Unchecked();
      else
        throw WrongArgumentTypeException();
    }
    int32_t AsInt32Unchecked() const
    {
#ifdef OSC_HOST_LITTLE_ENDIAN
      union{
          int32_t i;
          char c[4];
      } u;

      u.c[0] = argumentPtr_[3];
      u.c[1] = argumentPtr_[2];
      u.c[2] = argumentPtr_[1];
      u.c[3] = argumentPtr_[0];

      return u.i;
#else
      return *(int32_t*)argumentPtr_;
#endif
    }

    bool IsFloat() const { return *typeTagPtr_ == FLOAT_TYPE_TAG; }
    float AsFloat() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == FLOAT_TYPE_TAG )
        return AsFloatUnchecked();
      else
        throw WrongArgumentTypeException();
    }
    float AsFloatUnchecked() const
    {
#ifdef OSC_HOST_LITTLE_ENDIAN
      union{
          float f;
          char c[4];
      } u;

      u.c[0] = argumentPtr_[3];
      u.c[1] = argumentPtr_[2];
      u.c[2] = argumentPtr_[1];
      u.c[3] = argumentPtr_[0];

      return u.f;
#else
      return *(float*)argumentPtr_;
#endif
    }

    bool IsChar() const { return *typeTagPtr_ == CHAR_TYPE_TAG; }
    char AsChar() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == CHAR_TYPE_TAG )
        return AsCharUnchecked();
      else
        throw WrongArgumentTypeException();
    }
    char AsCharUnchecked() const
    {
      return (char)ToInt32( argumentPtr_ );
    }

    bool IsRgbaColor() const { return *typeTagPtr_ == RGBA_COLOR_TYPE_TAG; }
    uint32_t AsRgbaColor() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == RGBA_COLOR_TYPE_TAG )
        return AsRgbaColorUnchecked();
      else
        throw WrongArgumentTypeException();
    }
    uint32_t AsRgbaColorUnchecked() const
    {
      return ToUInt32( argumentPtr_ );
    }

    bool IsMidiMessage() const { return *typeTagPtr_ == MIDI_MESSAGE_TYPE_TAG; }
    uint32_t AsMidiMessage() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == MIDI_MESSAGE_TYPE_TAG )
        return AsMidiMessageUnchecked();
      else
        throw WrongArgumentTypeException();
    }
    uint32_t AsMidiMessageUnchecked() const
    {
      return ToUInt32( argumentPtr_ );
    }

    bool IsInt64() const { return *typeTagPtr_ == INT64_TYPE_TAG; }
    int64_t AsInt64() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == INT64_TYPE_TAG )
        return AsInt64Unchecked();
      else
        throw WrongArgumentTypeException();
    }
    int64_t AsInt64Unchecked() const
    {
      return ToInt64( argumentPtr_ );
    }

    bool IsTimeTag() const { return *typeTagPtr_ == TIME_TAG_TYPE_TAG; }
    uint64_t AsTimeTag() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == TIME_TAG_TYPE_TAG )
        return AsTimeTagUnchecked();
      else
        throw WrongArgumentTypeException();
    }
    uint64_t AsTimeTagUnchecked() const
    {
      return ToUInt64( argumentPtr_ );
    }

    bool IsDouble() const { return *typeTagPtr_ == DOUBLE_TYPE_TAG; }
    double AsDouble() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == DOUBLE_TYPE_TAG )
        return AsDoubleUnchecked();
      else
        throw WrongArgumentTypeException();
    }
    double AsDoubleUnchecked() const
    {
#ifdef OSC_HOST_LITTLE_ENDIAN
      union{
          double d;
          char c[8];
      } u;

      u.c[0] = argumentPtr_[7];
      u.c[1] = argumentPtr_[6];
      u.c[2] = argumentPtr_[5];
      u.c[3] = argumentPtr_[4];
      u.c[4] = argumentPtr_[3];
      u.c[5] = argumentPtr_[2];
      u.c[6] = argumentPtr_[1];
      u.c[7] = argumentPtr_[0];

      return u.d;
#else
      return *(double*)argumentPtr_;
#endif
    }

    bool IsString() const { return *typeTagPtr_ == STRING_TYPE_TAG; }
    const char* AsString() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == STRING_TYPE_TAG )
        return argumentPtr_;
      else
        throw WrongArgumentTypeException();
    }
    const char* AsStringUnchecked() const { return argumentPtr_; }

    bool IsSymbol() const { return *typeTagPtr_ == SYMBOL_TYPE_TAG; }
    const char* AsSymbol() const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == SYMBOL_TYPE_TAG )
        return argumentPtr_;
      else
        throw WrongArgumentTypeException();
    }
    const char* AsSymbolUnchecked() const { return argumentPtr_; }

    bool IsBlob() const { return *typeTagPtr_ == BLOB_TYPE_TAG; }
    void AsBlob( const void*& data, osc_bundle_element_size_t& size ) const
    {
      if( !typeTagPtr_ )
        throw MissingArgumentException();
      else if( *typeTagPtr_ == BLOB_TYPE_TAG )
        AsBlobUnchecked( data, size );
      else
        throw WrongArgumentTypeException();
    }
    void AsBlobUnchecked( const void*& data, osc_bundle_element_size_t& size ) const
    {
      // read blob size as an unsigned int then validate
      osc_bundle_element_size_t sizeResult = (osc_bundle_element_size_t)ToUInt32( argumentPtr_ );
      if( !IsValidElementSizeValue(sizeResult) )
        throw MalformedMessageException("invalid blob size");

      size = sizeResult;
      data = (void*)(argumentPtr_+ oscpack::OSC_SIZEOF_INT32);
    }

    bool IsArrayBegin() const { return *typeTagPtr_ == ARRAY_BEGIN_TYPE_TAG; }
    bool IsArrayEnd() const { return *typeTagPtr_ == ARRAY_END_TYPE_TAG; }
    // Calculate the number of top-level items in the array. Nested arrays count as one item.
    // Only valid at array start. Will throw an exception if IsArrayStart() == false.
    std::size_t ComputeArrayItemCount() const
    {
      // it is only valid to call ComputeArrayItemCount when the argument is the array start marker
      if( !IsArrayBegin() )
        throw WrongArgumentTypeException();

      std::size_t result = 0;
      unsigned int level = 0;
      const char *typeTag = typeTagPtr_ + 1;

      // iterate through all type tags. note that ReceivedMessage::Init
      // has already checked that the message is well formed.
      while( *typeTag ) {
        switch( *typeTag++ ) {
          case ARRAY_BEGIN_TYPE_TAG:
            level += 1;
            break;

          case ARRAY_END_TYPE_TAG:
            if(level == 0)
              return result;
            level -= 1;
            break;

          default:
            if( level == 0 ) // only count items at level 0
              ++result;
        }
      }

      return result;
    }

  private:
    const char *typeTagPtr_;
    const char *argumentPtr_;
};


class ReceivedMessageArgumentIterator{
  public:
    ReceivedMessageArgumentIterator( const char *typeTags, const char *arguments )
      : value_( typeTags, arguments ) {}

    ReceivedMessageArgumentIterator operator++()
    {
      Advance();
      return *this;
    }

    ReceivedMessageArgumentIterator operator++(int)
    {
      ReceivedMessageArgumentIterator old( *this );
      Advance();
      return old;
    }

    const ReceivedMessageArgument& operator*() const { return value_; }

    const ReceivedMessageArgument* operator->() const { return &value_; }

    friend bool operator==(const ReceivedMessageArgumentIterator& lhs,
                           const ReceivedMessageArgumentIterator& rhs );

  private:
    ReceivedMessageArgument value_;

    void Advance()
    {
      if( !value_.typeTagPtr_ )
        return;

      switch( *value_.typeTagPtr_++ ){
        case '\0':
          // don't advance past end
          --value_.typeTagPtr_;
          break;

        case TRUE_TYPE_TAG:
        case FALSE_TYPE_TAG:
        case NIL_TYPE_TAG:
        case INFINITUM_TYPE_TAG:

          // zero length
          break;

        case INT32_TYPE_TAG:
        case FLOAT_TYPE_TAG:
        case CHAR_TYPE_TAG:
        case RGBA_COLOR_TYPE_TAG:
        case MIDI_MESSAGE_TYPE_TAG:

          value_.argumentPtr_ += 4;
          break;

        case INT64_TYPE_TAG:
        case TIME_TAG_TYPE_TAG:
        case DOUBLE_TYPE_TAG:

          value_.argumentPtr_ += 8;
          break;

        case STRING_TYPE_TAG:
        case SYMBOL_TYPE_TAG:

          // we use the unsafe function FindStr4End(char*) here because all of
          // the arguments have already been validated in
          // ReceivedMessage::Init() below.

          value_.argumentPtr_ = FindStr4End( value_.argumentPtr_ );
          break;

        case BLOB_TYPE_TAG:
        {
          // treat blob size as an unsigned int for the purposes of this calculation
          uint32_t blobSize = ToUInt32( value_.argumentPtr_ );
          value_.argumentPtr_ = value_.argumentPtr_ + oscpack::OSC_SIZEOF_INT32 + RoundUp4( blobSize );
        }
          break;

        case ARRAY_BEGIN_TYPE_TAG:
        case ARRAY_END_TYPE_TAG:

          //    [ Indicates the beginning of an array. The tags following are for
          //        data in the Array until a close brace tag is reached.
          //    ] Indicates the end of an array.

          // zero length, don't advance argument ptr
          break;

        default:    // unknown type tag
          // don't advance
          --value_.typeTagPtr_;
          break;
      }
    }

    bool IsEqualTo( const ReceivedMessageArgumentIterator& rhs ) const
    {
      return value_.typeTagPtr_ == rhs.value_.typeTagPtr_;
    }
};

inline bool operator==(const ReceivedMessageArgumentIterator& lhs,
                       const ReceivedMessageArgumentIterator& rhs )
{
  return lhs.IsEqualTo( rhs );
}

inline bool operator!=(const ReceivedMessageArgumentIterator& lhs,
                       const ReceivedMessageArgumentIterator& rhs )
{
  return !( lhs == rhs );
}


class ReceivedMessageArgumentStream{
    friend class ReceivedMessage;
    ReceivedMessageArgumentStream( const ReceivedMessageArgumentIterator& begin,
                                   const ReceivedMessageArgumentIterator& end )
      : p_( begin )
      , end_( end ) {}

    ReceivedMessageArgumentIterator p_, end_;

  public:

    // end of stream
    bool Eos() const { return p_ == end_; }

    ReceivedMessageArgumentStream& operator>>( bool& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs = (*p_++).AsBool();
      return *this;
    }

    // not sure if it would be useful to stream Nil and Infinitum
    // for now it's not possible
    // same goes for array boundaries

    ReceivedMessageArgumentStream& operator>>( int32_t& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs = (*p_++).AsInt32();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( float& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs = (*p_++).AsFloat();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( char& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs = (*p_++).AsChar();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( RgbaColor& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs.value = (*p_++).AsRgbaColor();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( MidiMessage& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs.value = (*p_++).AsMidiMessage();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( int64_t& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs = (*p_++).AsInt64();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( TimeTag& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs.value = (*p_++).AsTimeTag();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( double& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs = (*p_++).AsDouble();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( Blob& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      (*p_++).AsBlob( rhs.data, rhs.size );
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( const char*& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs = (*p_++).AsString();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( Symbol& rhs )
    {
      if( Eos() )
        throw MissingArgumentException();

      rhs.value = (*p_++).AsSymbol();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>( MessageTerminator& rhs )
    {
      (void) rhs; // suppress unused parameter warning

      if( !Eos() )
        throw ExcessArgumentException();

      return *this;
    }
};


class ReceivedMessage{
    void Init( const char *message, osc_bundle_element_size_t size )
    {
      if( !IsValidElementSizeValue(size) )
        throw MalformedMessageException( "invalid message size" );

      if( size == 0 )
        throw MalformedMessageException( "zero length messages not permitted" );

      if( !IsMultipleOf4(size) )
        throw MalformedMessageException( "message size must be multiple of four" );

      const char *end = message + size;

      typeTagsBegin_ = FindStr4End( addressPattern_, end );
      if( typeTagsBegin_ == 0 ){
        // address pattern was not terminated before end
        throw MalformedMessageException( "unterminated address pattern" );
      }

      if( typeTagsBegin_ == end ){
        // message consists of only the address pattern - no arguments or type tags.
        typeTagsBegin_ = 0;
        typeTagsEnd_ = 0;
        arguments_ = 0;

      }else{
        if( *typeTagsBegin_ != ',' )
          throw MalformedMessageException( "type tags not present" );

        if( *(typeTagsBegin_ + 1) == '\0' ){
          // zero length type tags
          typeTagsBegin_ = 0;
          typeTagsEnd_ = 0;
          arguments_ = 0;

        }else{
          // check that all arguments are present and well formed

          arguments_ = FindStr4End( typeTagsBegin_, end );
          if( arguments_ == 0 ){
            throw MalformedMessageException( "type tags were not terminated before end of message" );
          }

          ++typeTagsBegin_; // advance past initial ','

          const char *typeTag = typeTagsBegin_;
          const char *argument = arguments_;
          unsigned int arrayLevel = 0;

          do{
            switch( *typeTag ){
              case TRUE_TYPE_TAG:
              case FALSE_TYPE_TAG:
              case NIL_TYPE_TAG:
              case INFINITUM_TYPE_TAG:
                // zero length
                break;

                //    [ Indicates the beginning of an array. The tags following are for
                //        data in the Array until a close brace tag is reached.
                //    ] Indicates the end of an array.
              case ARRAY_BEGIN_TYPE_TAG:
                ++arrayLevel;
                // (zero length argument data)
                break;

              case ARRAY_END_TYPE_TAG:
                --arrayLevel;
                // (zero length argument data)
                break;

              case INT32_TYPE_TAG:
              case FLOAT_TYPE_TAG:
              case CHAR_TYPE_TAG:
              case RGBA_COLOR_TYPE_TAG:
              case MIDI_MESSAGE_TYPE_TAG:

                if( argument == end )
                  throw MalformedMessageException( "arguments exceed message size" );
                argument += 4;
                if( argument > end )
                  throw MalformedMessageException( "arguments exceed message size" );
                break;

              case INT64_TYPE_TAG:
              case TIME_TAG_TYPE_TAG:
              case DOUBLE_TYPE_TAG:

                if( argument == end )
                  throw MalformedMessageException( "arguments exceed message size" );
                argument += 8;
                if( argument > end )
                  throw MalformedMessageException( "arguments exceed message size" );
                break;

              case STRING_TYPE_TAG:
              case SYMBOL_TYPE_TAG:

                if( argument == end )
                  throw MalformedMessageException( "arguments exceed message size" );
                argument = FindStr4End( argument, end );
                if( argument == 0 )
                  throw MalformedMessageException( "unterminated string argument" );
                break;

              case BLOB_TYPE_TAG:
              {
                if( argument + oscpack::OSC_SIZEOF_INT32 > end )
                  MalformedMessageException( "arguments exceed message size" );

                // treat blob size as an unsigned int for the purposes of this calculation
                uint32_t blobSize = ToUInt32( argument );
                argument = argument + oscpack::OSC_SIZEOF_INT32 + RoundUp4( blobSize );
                if( argument > end )
                  MalformedMessageException( "arguments exceed message size" );
              }
                break;

              default:
                throw MalformedMessageException( "unknown type tag" );
            }

          }while( *++typeTag != '\0' );
          typeTagsEnd_ = typeTag;

          if( arrayLevel !=  0 )
            throw MalformedMessageException( "array was not terminated before end of message (expected ']' end of array tag)" );
        }

        // These invariants should be guaranteed by the above code.
        // we depend on them in the implementation of ArgumentCount()
#ifndef NDEBUG
        std::ptrdiff_t argumentCount = typeTagsEnd_ - typeTagsBegin_;
        assert( argumentCount >= 0 );
        assert( argumentCount <= OSC_INT32_MAX );
#endif
      }
    }
  public:
    explicit ReceivedMessage( const ReceivedPacket& packet )
      : addressPattern_( packet.Contents() )
    {
      Init( packet.Contents(), packet.Size() );
    }
    explicit ReceivedMessage( const ReceivedBundleElement& bundleElement )
      : addressPattern_( bundleElement.Contents() )
    {
      Init( bundleElement.Contents(), bundleElement.Size() );
    }

    const char *AddressPattern() const { return addressPattern_; }

    // Support for non-standard SuperCollider integer address patterns:
    bool AddressPatternIsUInt32() const
    {
      return (addressPattern_[0] == '\0');
    }
    uint32_t AddressPatternAsUInt32() const
    {
      return ToUInt32( addressPattern_ );
    }

    uint32_t ArgumentCount() const { return static_cast<uint32_t>(typeTagsEnd_ - typeTagsBegin_); }

    const char *TypeTags() const { return typeTagsBegin_; }


    typedef ReceivedMessageArgumentIterator const_iterator;

    ReceivedMessageArgumentIterator ArgumentsBegin() const
    {
      return ReceivedMessageArgumentIterator( typeTagsBegin_, arguments_ );
    }

    ReceivedMessageArgumentIterator ArgumentsEnd() const
    {
      return ReceivedMessageArgumentIterator( typeTagsEnd_, 0 );
    }

    ReceivedMessageArgumentStream ArgumentStream() const
    {
      return ReceivedMessageArgumentStream( ArgumentsBegin(), ArgumentsEnd() );
    }

  private:
    const char *addressPattern_;
    const char *typeTagsBegin_;
    const char *typeTagsEnd_;
    const char *arguments_;
};


class ReceivedBundle{
    void Init( const char *bundle, osc_bundle_element_size_t size )
    {

      if( !IsValidElementSizeValue(size) )
        throw MalformedBundleException( "invalid bundle size" );

      if( size < 16 )
        throw MalformedBundleException( "packet too short for bundle" );

      if( !IsMultipleOf4(size) )
        throw MalformedBundleException( "bundle size must be multiple of four" );

      if( bundle[0] != '#'
          || bundle[1] != 'b'
          || bundle[2] != 'u'
          || bundle[3] != 'n'
          || bundle[4] != 'd'
          || bundle[5] != 'l'
          || bundle[6] != 'e'
          || bundle[7] != '\0' )
        throw MalformedBundleException( "bad bundle address pattern" );

      end_ = bundle + size;

      timeTag_ = bundle + 8;

      const char *p = timeTag_ + 8;

      while( p < end_ ){
        if( p + oscpack::OSC_SIZEOF_INT32 > end_ )
          throw MalformedBundleException( "packet too short for elementSize" );

        // treat element size as an unsigned int for the purposes of this calculation
        uint32_t elementSize = ToUInt32( p );
        if( (elementSize & ((uint32_t)0x03)) != 0 )
          throw MalformedBundleException( "bundle element size must be multiple of four" );

        p += oscpack::OSC_SIZEOF_INT32 + elementSize;
        if( p > end_ )
          throw MalformedBundleException( "packet too short for bundle element" );

        ++elementCount_;
      }

      if( p != end_ )
        throw MalformedBundleException( "bundle contents " );
    }
  public:
    explicit ReceivedBundle( const ReceivedPacket& packet )
      : elementCount_( 0 )
    {
      Init( packet.Contents(), packet.Size() );
    }
    explicit ReceivedBundle( const ReceivedBundleElement& bundleElement )
      : elementCount_( 0 )
    {
      Init( bundleElement.Contents(), bundleElement.Size() );
    }

    uint64_t TimeTag() const
    {
      return ToUInt64( timeTag_ );
    }

    uint32_t ElementCount() const { return elementCount_; }

    typedef ReceivedBundleElementIterator const_iterator;

    ReceivedBundleElementIterator ElementsBegin() const
    {
      return ReceivedBundleElementIterator( timeTag_ + 8 );
    }

    ReceivedBundleElementIterator ElementsEnd() const
    {
      return ReceivedBundleElementIterator( end_ );
    }

  private:
    const char *timeTag_;
    const char *end_;
    uint32_t elementCount_;
};


inline auto begin(const oscpack::ReceivedMessage& mes)
{
  return mes.ArgumentsBegin();
}

inline auto end(const oscpack::ReceivedMessage& mes)
{
  return mes.ArgumentsEnd();
}

} // namespace osc


#endif /* INCLUDED_OSCPACK_OSCRECEIVEDELEMENTS_H */
