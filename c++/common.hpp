#ifndef AOC_COMMON_HPP
#define AOC_COMMON_HPP

#include <regex>
#include <string>
#include <string_view>
#include <vector>

#include <ranges>
#include <stdexcept>
#include <utility>

// Miscellaneous utilities, mostly related to parsing, potentially useful
// in multiple solutions.

namespace aoc {

std::vector<std::string> read_lines(std::string const & path);
std::vector<char> read_content(std::string const & path);

decltype(auto) enforce(auto && t)
{
    if (!t) [[unlikely]]
        throw std::runtime_error("unexpected");
    return std::forward<decltype(t)>(t);
}

[[noreturn]] void fail();

template <typename It>
auto to_view(std::sub_match<It> const & m)
{
    return std::ranges::subrange(m.first, m.second);
}

template <typename It>
std::string_view to_string_view(std::sub_match<It> const & m)
{
    return std::string_view(m.first, m.second);
}

}

#endif
