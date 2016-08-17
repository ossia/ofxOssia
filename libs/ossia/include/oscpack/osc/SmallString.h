#pragma once
#include <boost/container/static_vector.hpp>
#include <algorithm>

namespace oscpack
{
template<int Count>
struct small_string_base
{
    boost::container::static_vector<char, Count> m_impl;

    small_string_base():
      m_impl(1, 0)
    {

    }

    small_string_base(const std::string& other):
      m_impl(other.begin(), other.end())
    {
      m_impl.push_back(0);
    }

    small_string_base(std::size_t size, char t):
      m_impl(size, t)
    {
      m_impl.push_back(0);
    }

    auto begin()       { return m_impl.begin(); }
    auto end()         { return m_impl.end(); }
    auto begin() const { return m_impl.begin(); }
    auto end() const   { return m_impl.end(); }
    auto size() const  { return m_impl.size(); }
    auto data() const  { return m_impl.data(); }

    auto push_back(char c) {
      m_impl.back() = c;
      m_impl.push_back(0);
    }

    void append(small_string_base src)
    {
      auto old_size = m_impl.size() - 1;
      m_impl.resize(old_size + src.size());
      std::copy(src.begin(), src.end(), m_impl.begin() + old_size);
    }

    template<int N>
    void append(const char (&src)[N])
    {
      auto old_size = m_impl.size() - 1;
      m_impl.resize(old_size + N);
      std::copy_n(std::begin(src), N, m_impl.begin() + old_size);
    }
};

using small_string = small_string_base<16>;
}
