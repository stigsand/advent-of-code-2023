// https://adventofcode.com/2023/day/5

#include "common.hpp"
// #include <algorithm>
#include <cstddef>
// #include <iterator>
#include <map>
// #include <numeric>
#include <print>
// #include <ranges>
#include <regex>
// #include <utility>

using namespace aoc;
using namespace std::string_literals;
//using namespace std::string_view_literals;

std::vector const example_almanac {
    "seeds: 79 14 55 13"s,
    ""s,
    "seed-to-soil map:"s,
    "50 98 2"s,
    "52 50 48"s,
    "0 15 37"s,
    ""s,
    "soil-to-fertilizer map:"s,
    "0 15 37"s,
    "37 52 2"s,
    "39 0 15"s,
    ""s,
    "fertilizer-to-water map:"s,
    "49 53 8"s,
    "0 11 42"s,
    "42 0 7"s,
    "57 7 4"s,
    ""s,
    "water-to-light map:"s,
    "88 18 7"s,
    "18 25 70"s,
    ""s,
    "light-to-temperature map:"s,
    "45 77 23"s,
    "81 45 19"s,
    "68 64 13"s,
    ""s,
    "temperature-to-humidity map:"s,
    "0 69 1"s,
    "1 0 69"s,
    ""s,
    "humidity-to-location map:"s,
    "60 56 37"s,
    "56 93 4"s };

struct kind_map {
    std::string dst_cat;
    std::string src_cat;

    struct range_map {
        unsigned int dst_kind;
        unsigned int src_kind;
        std::size_t len;
    };
    std::vector<range_map> ranges;
};

struct almanac {
    std::vector<unsigned int> seeds;
    std::map<std::string, kind_map> kind_maps;  // src_cat -> kind_map
};

almanac parse_almanac(std::ranges::range auto const & lines)
{
    auto it = lines.begin();

    (void) lines;
    return {};
}

std::vector<unsigned int> run_to(std::string const & cat, almanac const & a, unsigned int const & seed)
{
    (void) cat;
    (void) a;
    (void) seed;
    return {};
}

namespace part1 {



unsigned int lowest_location_number(almanac const & a)
{
    (void) a;
    return 0U;
}

void test()
{
    auto a = parse_almanac(example_almanac);
    for (auto const & [cat, km] : a.kind_maps) {
        std::println("Src cat: {}", km.src_cat);
        std::println("Dst cat: {}", km.dst_cat);
        for (auto const & rm : km.ranges)
            std::println("  {} <- {} (len {})", rm.dst_kind, rm.src_kind, rm.len);
        std::println("");
    }

    enforce(lowest_location_number(parse_almanac(example_almanac)) == 35);
}

}

namespace part2 {

// ...

unsigned int sum()
{
    return 0U;
}

void test()
{
    enforce(sum(/* ... */) == 33);
}

}

int main()
{
    part1::test();
    //part2::test();

    auto const almanac = parse_almanac(read_lines("input/day-05"));

    std::println("Lowest location number: {}", part1::lowest_location_number(almanac));
    std::println("Sum: {}", part2::sum(/* ... */));
}
