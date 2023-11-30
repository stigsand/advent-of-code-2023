#include "common.hpp"
#include <fstream>
#include <iterator>
#include <utility>

namespace aoc {

std::vector<std::string> read_lines(std::string const & path)
{
    std::ifstream in(path);
    std::vector<std::string> lines;
    for (std::string line; std::getline(in, line);)
        lines.push_back(line);
    return lines;
}

std::vector<char> read_content(std::string const & path)
{
    std::ifstream in(path);
    return { std::istreambuf_iterator<char>(in), {} };
}

[[noreturn]] void fail()
{
    enforce(false);
    std::unreachable();
}

}
