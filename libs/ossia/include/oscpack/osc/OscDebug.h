#pragma once
#include "OscReceivedElements.h"
#include <iostream>

namespace oscpack
{

template<typename Stream>
auto& debug(Stream& s, const ReceivedMessage& mess)
{
  s << mess.AddressPattern() << " ";
  for(auto arg : mess)
  {
    if(arg.IsString())
    {
      s << arg.AsString() << " ";
    }
    else if(arg.IsInt32())
    {
      s << arg.AsInt32() << " ";
    }
    else if(arg.IsFloat())
    {
      s << arg.AsFloat() << " ";
    }
    else if(arg.IsBool())
    {
      s << arg.AsBool() << " ";
    }
    else if(arg.IsChar())
    {
      s << arg.AsChar() << " ";
    }
    else if(arg.IsInt64())
    {
      s << arg.AsInt64() << " ";
    }
    else if(arg.IsDouble())
    {
      s << arg.AsDouble() << " ";
    }
  }

  return s;
}
}
